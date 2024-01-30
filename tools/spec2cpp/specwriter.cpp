#include "specwriter.h"

#include <QFile>
#include <QTextStream>

#include <algorithm>
#include <unordered_set>

struct QStringHash
{
    size_t operator()(const QString &string) const { return qHash(string); }
};

SpecWriter::SpecWriter(Data data)
    : m_data(std::move(data))
{
}

static QString methodToName(const QString &method)
{
    auto names = method.split('/');

    // Remove some specific method prefix : $, window, client
    if (names.first() == "$" || names.first() == "window" || names.first() == "client")
        names.removeFirst();

    QString name;
    for (const auto &word : std::as_const(names)) {
        name += word[0].toUpper() + word.mid(1);
    }
    return name;
}

static constexpr char NotificationHeader[] = R"(// File generated by spec2cpp tool
// DO NOT MAKE ANY CHANGES HERE

#pragma once

#include "notificationmessage.h"
#include "types.h"

namespace Lsp {
%1
}
)";

// %1 notification name
// %2 notification method
// %3 notification params
static constexpr char NotificationImpl[] = R"(
inline constexpr char %1Name[] = "%2";
struct %1Notification : public NotificationMessage<%1Name, %3>
{};
)";

void SpecWriter::saveNotifications()
{
    QFile file(LSP_SOURCE_PATH "/notifications.h");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return;

    QString text;
    for (const auto &notification : m_data.notifications) {
        const QString name = methodToName(notification.method);
        text += QString(NotificationImpl).arg(name, notification.method, notification.params);
    }

    QTextStream stream(&file);
    stream << QString(NotificationHeader).arg(text);
}

static constexpr char RequestHeader[] = R"(// File generated by spec2cpp tool
// DO NOT MAKE ANY CHANGES HERE

#pragma once

#include "requestmessage.h"
#include "types.h"

namespace Lsp {
%1
}
)";

// %1 request name
// %2 request method
// %3 request params
// %4 request result
// %5 request error
static constexpr char RequestImpl[] = R"(
inline constexpr char %1Name[] = "%2";
struct %1Request : public RequestMessage<%1Name, %3, %4, %5>
{};
)";

void SpecWriter::saveRequests()
{
    QFile file(LSP_SOURCE_PATH "/requests.h");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return;

    QString text;
    for (const auto &request : m_data.requests) {
        const QString name = methodToName(request.method);
        text += QString(RequestImpl).arg(name, request.method, request.params, request.result, request.error);
    }

    QTextStream stream(&file);
    stream << QString(RequestHeader).arg(text);
}

static constexpr char CodeHeader[] = R"(// File generated by spec2cpp tool
// DO NOT MAKE ANY CHANGES HERE

#pragma once

#include <nlohmann/json.hpp>

#include <memory>
#include <optional>
#include <string>
#include <tuple>
#include <unordered_map>
#include <variant>

namespace Lsp {
%1
}
)";

static constexpr char CodeJsonHeader[] = R"(// File generated by spec2cpp tool
// DO NOT MAKE ANY CHANGES HERE

#pragma once

#include "json.h"
#include "types.h"

namespace Lsp {
%1
}
)";

void SpecWriter::saveCode()
{
    cleanCode();

    {
        QFile file(LSP_SOURCE_PATH "/types.h");
        if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
            return;

        QString text;
        text += writeEnums();
        text += writeTypesAndInterfaces();

        QTextStream stream(&file);
        stream << QString(CodeHeader).arg(text);
    }
    {
        QFile file(LSP_SOURCE_PATH "/types_json.h");
        if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
            return;

        QString text;
        text += writeJsonEnums();
        for (const auto &interface : m_data.interfaces)
            text += writeJsonInterface(interface);

        QTextStream stream(&file);
        stream << QString(CodeJsonHeader).arg(text);
    }
}

// %1 comment
// %2 enum name
// %3 enum content
static constexpr char EnumImpl[] = R"(
%1enum class %2 {
%3};
)";

QString SpecWriter::writeEnums()
{
    QString result;
    for (const auto &enumeration : m_data.enumerations) {
        QString content;
        for (const auto &value : enumeration.values) {
            if (enumeration.isString)
                content += QString("%1\t%2,\n").arg(value.comment, value.name);
            else
                content += QString("%1\t%2 = %3,\n").arg(value.comment, value.name, value.value);
        }
        result += QString(EnumImpl).arg(enumeration.comment, enumeration.name, content);
    }
    return result;
}

// %1 enum name
// %2 content: lines like {ValueName, ValueString},
static constexpr char EnumJsonImpl[] = R"(
JSONIFY_ENUM( %1, {
%2})
)";

QString SpecWriter::writeJsonEnums()
{
    QString result;
    for (const auto &enumeration : m_data.enumerations) {
        if (enumeration.isString) {
            QString content;
            for (const auto &value : enumeration.values)
                content += QString("    {%1::%2, \"%3\"},\n").arg(enumeration.name, value.name, value.value);
            result += QString(EnumJsonImpl).arg(enumeration.name, content);
        }
    }
    return result;
}

QString SpecWriter::writeTypesAndInterfaces()
{
    QString result;
    QStringList existingNames;

    const auto sortByDependencies = [](const auto &lhs, const auto &rhs) {
        return !lhs.dependencies.contains(rhs.name);
    };

    auto &types = m_data.types;
    std::ranges::sort(types, sortByDependencies);

    auto &interfaces = m_data.interfaces;
    std::ranges::sort(interfaces, sortByDependencies);

    auto startType = types.begin();
    auto endType = types.begin();

    auto startStruct = interfaces.begin();
    auto endStruct = interfaces.begin();

    while (startType != types.end() || startStruct != interfaces.end()) {
        // Write only types without any dependencies
        if (startType != types.end()) {
            endType = std::stable_partition(startType, types.end(), [](const auto &type) {
                return type.dependencies.isEmpty();
            });

            for (; startType != endType; ++startType) {
                result += writeType(*startType);
                existingNames.push_back(startType->name);
            }
        }

        // Write only interfaces without any dependencies
        if (startStruct != interfaces.end()) {
            endStruct = std::stable_partition(startStruct, interfaces.end(), [](const auto &interface) {
                return interface.dependencies.isEmpty();
            });

            for (; startStruct != endStruct; ++startStruct) {
                result += writeMainInterface(*startStruct);
                existingNames.push_back(startStruct->name);
            }
        }

        // Cleanup dependencies
        for (auto it = endType; it != types.end(); ++it) {
            for (const auto &name : std::as_const(existingNames))
                it->dependencies.removeAll(name);
        }
        for (auto it = endStruct; it != interfaces.end(); ++it) {
            for (const auto &name : std::as_const(existingNames))
                it->dependencies.removeAll(name);
        }
        existingNames.clear();
    }
    return result;
}

// %1 comment
// %2 type name
// %3 type datatype
static constexpr char TypeImpl[] = R"(
%1using %2 = %3;
)";
QString SpecWriter::writeType(const Data::Type &type)
{
    static std::unordered_set<QString, QStringHash> exceptions = {"integer", "uinteger", "decimal"};
    if (exceptions.contains(type.name))
        return {};
    return QString(TypeImpl).arg(type.comment, type.name, type.dataType);
}

static QString writeProperty(const Data::Interface::Property &property, const QString &interface)
{
    QString name = property.name;
    name.remove("readonly ");

    bool isOptional = property.name.contains('?');
    bool isConstString = property.dataType.startsWith('\'');
    bool isPtr = property.dataType == interface;

    if (isPtr) {
        return QString("%1std::unique_ptr<%2> %3;\n").arg(property.comment, property.dataType, name.remove('?'));
    } else if (isOptional) {
        return QString("%1std::optional<%2> %3;\n").arg(property.comment, property.dataType, name.remove('?'));
    } else if (isConstString) {
        QString text = property.dataType;
        return QString("%1static inline const std::string %2 = %3;\n")
            .arg(property.comment, name, text.replace('\'', '\"'));
    }
    return QString("%1%2 %3;\n").arg(property.comment, property.dataType, name);
}

// %1 interface name
// %2 child interfaces and properties
static constexpr char ChildInterfaceImpl[] = R"(struct %1 {
%2};
)";
static QString writeChildInterface(const Data::Interface &interface, QStringList parent)
{
    parent.push_back(interface.name);

    QString content;

    for (const auto &child : interface.children)
        content += writeChildInterface(child, parent);
    for (const auto &property : interface.properties)
        content += writeProperty(property, interface.name);

    return QString(ChildInterfaceImpl).arg(interface.name, content);
}

// %1 comment
// %2 interface name
// %3 interface extend: ": public Extend1, public Extend2
// %4 child interfaces and properties
static constexpr char MainInterfaceImpl[] = R"(
%1struct %2%3 {
%4};
)";
QString SpecWriter::writeMainInterface(const Data::Interface &interface)
{
    QString extends = interface.extends.join(", public ");
    if (!extends.isEmpty())
        extends.prepend(": public ");

    QString content;
    for (const auto &child : interface.children)
        content += writeChildInterface(child, {interface.name});
    for (const auto &property : interface.properties)
        content += writeProperty(property, interface.name);

    return QString(MainInterfaceImpl).arg(interface.comment, interface.name, extends, content);
}

QStringList interfaceProperties(const Data::Interface &interface, const std::vector<Data::Interface> &interfaces)
{
    QStringList properties;
    for (const auto &prop : interface.properties) {
        QString name = prop.name;
        name.remove('?');
        name.remove("readonly ");
        properties.push_back(name);
    }

    for (const auto &extend : interface.extends) {
        auto it = std::ranges::find_if(interfaces, [extend](const auto &interface) {
            return interface.name == extend;
        });
        if (it != interfaces.cend()) {
            properties.append(interfaceProperties(*it, interfaces));
        }
    }
    return properties;
}

QString SpecWriter::writeJsonInterface(const Data::Interface &interface, QStringList parent)
{
    static std::unordered_set<QString> exceptions = {"SelectionRange", "FormattingOptions", "ChangeAnnotationsType"};

    parent.push_back(interface.name);
    if (exceptions.contains(interface.name))
        return QString("\nJSONIFY_FWD(%1)\n").arg(parent.join("::"));

    QString result;
    if (parent.count() == 1)
        result += "\n";

    QStringList properties = interfaceProperties(interface, m_data.interfaces);

    for (const auto &child : interface.children)
        result += writeJsonInterface(child, parent);

    if (properties.empty())
        result += QString("JSONIFY_EMPTY(%1)\n").arg(parent.join("::"));
    else
        result += QString("JSONIFY(%1, %2)\n").arg(parent.join("::"), properties.join(", "));
    return result;
}

void SpecWriter::cleanCode()
{
    auto &enumerations = m_data.enumerations;
    auto &types = m_data.types;
    auto &interfaces = m_data.interfaces;

    // Remove enumeration duplicates
    {
        std::unordered_set<QString, QStringHash> enumSet;
        auto itEnum = std::ranges::remove_if(enumerations, [&enumSet](const auto &e) {
                          return !enumSet.insert(e.name).second;
                      }).begin();
        enumerations.erase(itEnum, enumerations.end());
    }

    // Cleanup enumeration names and enumeration values
    {
        std::map<QString, QString> specialEnumNames = {{"InitializeError", "InitializeErrorCodes"}};
        for (auto &enumeration : enumerations) {
            if (specialEnumNames.contains(enumeration.name))
                enumeration.name = specialEnumNames.at(enumeration.name);

            for (auto &value : enumeration.values) {
                value.name[0] = value.name[0].toUpper();
                if (enumeration.isString)
                    value.value = value.value.remove('\'');
                else
                    value.value[0] = value.value[0].toUpper();
            }
        }
    }

    // Remove some specific struct
    {
        std::unordered_set<QString, QStringHash> removeStructNames = {
            "Message", "RequestMessage", "ResponseMessage", "ResponseError", "NotificationMessage", "LSPObject", "T"};

        auto it = interfaces.begin();
        while (it != interfaces.end()) {
            if (removeStructNames.contains(it->name))
                it = interfaces.erase(it);
            else
                ++it;
        }
    }

    // Remove some types
    {
        std::unordered_set<QString, QStringHash> removeTypeNames = {"LSPAny"};

        auto it = types.begin();
        while (it != types.end()) {
            if (removeTypeNames.contains(it->name))
                it = types.erase(it);
            else
                ++it;
        }
    }

    // Remove duplicate types (from enums or interfaces)
    QStringList enumNames;
    {
        for (auto &enumeration : enumerations)
            enumNames.push_back(enumeration.name);

        QStringList existingTypeNames = enumNames;
        for (const auto &interface : interfaces)
            existingTypeNames.push_back(interface.name);

        auto itRemove = std::ranges::remove_if(types, [existingTypeNames](const auto &t) {
                            return existingTypeNames.contains(t.name);
                        }).begin();
        types.erase(itRemove, types.end());
    }

    // Cleanup dependencies for types and interfaces
    {
        for (auto &type : types) {
            for (const auto &enumName : std::as_const(enumNames))
                type.dependencies.removeAll(enumName);
        }
        for (auto &interface : m_data.interfaces) {
            for (const auto &enumName : std::as_const(enumNames))
                interface.dependencies.removeAll(enumName);
        }
    }
}

// Newer versions
MetaSpecWriter::MetaSpecWriter(MetaData data)
    : m_data(std::move(data))
{
}

void MetaSpecWriter::saveNotifications()
{
    QFile file(LSP_SOURCE_PATH "/notifications.h");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return;

    QString text;
    for (const auto &notification : m_data.notifications) {
        const auto name = methodToName(notification.name);

        const auto params = notification.params ? notification.params->value : QStringLiteral("std::nullptr_t");

        text += QString(NotificationImpl).arg(name, notification.name, params);
    }

    QTextStream stream(&file);
    stream << QString(NotificationHeader).arg(text);
}

void MetaSpecWriter::saveRequests()
{
    QFile file(LSP_SOURCE_PATH "/requests.h");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return;

    QString text;
    for (const auto &request : m_data.requests) {
        const auto name = methodToName(request.name);

        const auto params = request.params ? request.params->value : QStringLiteral("std::nullptr_t");
        const auto result = request.result ? request.result->value : QStringLiteral("std::nullptr_t");

        text += QString(RequestImpl).arg(name, request.name, params, result, request.error);
    }

    QTextStream stream(&file);
    stream << QString(RequestHeader).arg(text);
}

void MetaSpecWriter::saveCode()
{
    cleanCode();

    {
        QFile file(LSP_SOURCE_PATH "/types.h");
        if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
            return;

        QString text;
        text += writeEnums();
        text += writeTypesAndInterfaces();

        QTextStream stream(&file);
        stream << QString(CodeHeader).arg(text);
    }
    {
        QFile file(LSP_SOURCE_PATH "/types_json.h");
        if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
            return;

        QString text;
        text += writeJsonEnums();
        for (const auto &interface : m_data.interfaces)
            text += writeJsonInterface(interface);

        QTextStream stream(&file);
        stream << QString(CodeJsonHeader).arg(text);
    }
}

QString MetaSpecWriter::writeEnums()
{
    QString result;
    for (const auto &enumeration : m_data.enumerations) {
        QString content;
        for (const auto &value : enumeration.values) {
            if (enumeration.type == MetaData::Enumeration::String)
                content += QString("%1\t%2,\n").arg(value.documentation, value.name);
            else
                content += QString("%1\t%2 = %3,\n").arg(value.documentation, value.name, value.value);
        }
        result += QString(EnumImpl).arg(enumeration.documentation, enumeration.name, content);
    }
    return result;
}

QString MetaSpecWriter::writeJsonEnums()
{
    QString result;
    for (const auto &enumeration : m_data.enumerations) {
        if (enumeration.type == MetaData::Enumeration::String) {
            QString content;
            for (const auto &value : enumeration.values)
                content += QString("    {%1::%2, \"%3\"},\n").arg(enumeration.name, value.name, value.value);
            result += QString(EnumJsonImpl).arg(enumeration.name, content);
        }
    }
    return result;
}

QString MetaSpecWriter::writeTypesAndInterfaces()
{
    QString result;
    QStringList existingNames;

    m_rootTypes.clear();
    for (auto &value : m_data.types) {
        m_rootTypes.push_back(value);
    }

    for (auto &value : m_data.interfaces) {
        m_rootTypes.push_back(value);
    }

    // Sort types and interfaces
    std::ranges::sort(m_rootTypes, [](const auto &lhs, const auto &rhs) {
        return !lhs->dependencies.contains(rhs->name);
    });

    for (auto startType = m_rootTypes.begin(); startType != m_rootTypes.end();) {
        // Write only types without any dependencies
        auto endType = std::stable_partition(startType, m_rootTypes.end(), [](const auto &type) {
            return type->dependencies.isEmpty();
        });

        for (; startType != endType; ++startType) {
            auto type = *startType;
            if (type->is_interface())
                result += writeMainInterface(std::static_pointer_cast<MetaData::Interface>(type));
            else
                result += writeType(type);

            existingNames.push_back(type->name);
        }

        // Cleanup dependencies
        for (auto it = endType; it != m_rootTypes.end(); ++it) {
            for (const auto &name : std::as_const(existingNames)) {
                (*it)->dependencies.removeAll(name);
            }
        }

        existingNames.clear();
    }

    return result;
}

QString MetaSpecWriter::writeType(const MetaData::TypePtr &type)
{
    static std::unordered_set<QString, QStringHash> exceptions = {"integer", "uinteger", "decimal"};
    if (exceptions.contains(type->name))
        return {};

    return QString(TypeImpl).arg(type->documentation, type->name, type->value);
}

QString MetaSpecWriter::writeProperty(const MetaData::TypePtr &property, const QString &interface)
{
    QString name = property->name;

    bool isOptional = property->name.contains('?');
    bool isConstString = property->kind == MetaData::TypeKind::StringLiteral;
    bool isPtr = property->value == interface;

    // We might have to change the order of the values
    if (property->kind == MetaData::TypeKind::Or) {
        const auto fetchType = [this](const auto &type) -> MetaData::TypePtr {
            if (type->kind != MetaData::TypeKind::Reference)
                return type;

            auto it = std::ranges::find_if(m_rootTypes, [&type](const auto &value) {
                return type->value == value->name;
            });
            if (it != m_rootTypes.end())
                return *it;

            return type;
        };

        // Change order of values
        std::ranges::sort(property->items, [&](const auto &lhs, const auto &rhs) {
            const auto lhsType = fetchType(lhs);
            const auto rhsType = fetchType(rhs);
            return std::make_tuple(!lhsType->is_deprecated(), lhsType->since)
                < std::make_tuple(!rhsType->is_deprecated(), rhsType->since);
        });

        // Set the value using the new order
        QStringList stringList;
        for (const auto &item : property->items) {
            stringList.append(item->value);
        }

        property->value = QString("std::variant<%1>").arg(stringList.join(", "));
    }

    if (isConstString && !property->items.empty()) {
        // Write all stringliterals to "documentation"
        for (const auto &item : property->items) {
            property->documentation.append(writeProperty(item, interface));
        }
    }

    if (isPtr) {
        return QString("%1std::unique_ptr<%2> %3;\n").arg(property->documentation, property->value, name.remove('?'));
    } else if (isOptional) {
        return QString("%1std::optional<%2> %3;\n").arg(property->documentation, property->value, name.remove('?'));
    } else if (isConstString) {
        return QString("%1static inline const std::string %2 = \"%3\";\n")
            .arg(property->documentation, name, property->value);
    }

    return QString("%1%2 %3;\n").arg(property->documentation, property->value, name);
}

QString MetaSpecWriter::writeChildInterface(const MetaData::TypePtr &type, QStringList parent)
{
    QString name = type->name;
    name.remove('?');

    parent.push_back(name);

    QString content;
    for (const auto &item : type->items) {
        if (item->is_interface())
            content += writeChildInterface(item, parent);
        else
            content += writeProperty(item, name);
    }

    return QString(ChildInterfaceImpl).arg(name, content);
}

QString MetaSpecWriter::writeMainInterface(const MetaData::InterfacePtr &interface)
{
    QString extends;
    for (const auto &extend : interface->extends) {
        extends.append(extend->value).append(", public ");
    }

    if (!interface->extends.empty()) {
        extends.remove(extends.lastIndexOf(", public "), 9);
        extends.prepend(" : public ");
    }

    QString content;
    for (const auto &item : interface->items) {
        if (item->is_interface())
            content += writeChildInterface(item, {interface->name});
        else
            content += writeProperty(item, interface->name);
    }

    return QString(MainInterfaceImpl).arg(interface->documentation, interface->name, extends, content);
}

QStringList interfaceProperties(const MetaData::InterfacePtr &interface,
                                const std::vector<MetaData::InterfacePtr> &interfaces)
{
    QStringList properties;
    for (const auto &prop : interface->items) {
        if (prop->is_interface())
            continue;

        QString name = prop->name;
        name.remove('?');
        properties.push_back(name);
    }

    for (const auto &extend : interface->extends) {
        const auto it = std::ranges::find_if(interfaces, [&extend](const auto &interface) {
            return interface->name == extend->value;
        });
        if (it != interfaces.cend()) {
            properties.append(interfaceProperties(*it, interfaces));
        }
    }
    return properties;
}

QString MetaSpecWriter::writeJsonInterface(const MetaData::InterfacePtr &interface, QStringList parent)
{
    static std::unordered_set<QString> exceptions = {"SelectionRange", "FormattingOptions", "ChangeAnnotationsType"};

    QString name = interface->name;
    name.remove('?');

    parent.push_back(name);
    if (exceptions.contains(name))
        return QString("\nJSONIFY_FWD(%1)\n").arg(parent.join("::"));

    QString result;
    if (parent.count() == 1)
        result += "\n";

    QStringList properties = interfaceProperties(interface, m_data.interfaces);

    for (const auto &child : interface->items) {
        if (child->is_interface())
            result += writeJsonInterface(std::static_pointer_cast<MetaData::Interface>(child), parent);
    }

    if (properties.empty())
        result += QString("JSONIFY_EMPTY(%1)\n").arg(parent.join("::"));
    else
        result += QString("JSONIFY(%1, %2)\n").arg(parent.join("::"), properties.join(", "));
    return result;
}

void MetaSpecWriter::cleanCode()
{
    auto &enumerations = m_data.enumerations;
    auto &types = m_data.types;
    auto &interfaces = m_data.interfaces;

    // Remove enumeration duplicates
    {
        std::unordered_set<QString, QStringHash> enumSet;
        auto itEnum = std::ranges::remove_if(enumerations, [&enumSet](const auto &e) {
                          return !enumSet.insert(e.name).second;
                      }).begin();
        enumerations.erase(itEnum, enumerations.end());
    }

    // Cleanup enumeration names and enumeration values
    {
        std::map<QString, QString> specialEnumNames = {{"InitializeError", "InitializeErrorCodes"}};
        for (auto &enumeration : enumerations) {
            if (specialEnumNames.contains(enumeration.name))
                enumeration.name = specialEnumNames.at(enumeration.name);

            for (auto &value : enumeration.values) {
                value.name[0] = value.name[0].toUpper();
                if (enumeration.type != MetaData::Enumeration::String)
                    value.value[0] = value.value[0].toUpper();
            }
        }
    }

    // Remove some specific struct
    {
        std::unordered_set<QString, QStringHash> removeStructNames = {
            "Message", "RequestMessage", "ResponseMessage", "ResponseError", "NotificationMessage", "LSPObject", "T"};

        auto it = interfaces.begin();
        while (it != interfaces.end()) {
            if (removeStructNames.contains((*it)->name))
                it = interfaces.erase(it);
            else
                ++it;
        }
    }

    // Remove some types
    {
        std::unordered_set<QString, QStringHash> removeTypeNames = {"LSPAny"};

        auto it = types.begin();
        while (it != types.end()) {
            if (removeTypeNames.contains((*it)->name))
                it = types.erase(it);
            else
                ++it;
        }
    }

    // Remove duplicate types (from enums or interfaces)
    QStringList enumNames;
    {
        for (auto &enumeration : enumerations)
            enumNames.push_back(enumeration.name);

        QStringList existingTypeNames = enumNames;
        for (const auto &interface : interfaces)
            existingTypeNames.push_back(interface->name);

        auto itRemove = std::ranges::remove_if(types, [existingTypeNames](const auto &t) {
                            return existingTypeNames.contains(t->name);
                        }).begin();
        types.erase(itRemove, types.end());
    }

    // Cleanup dependencies for types and interfaces
    for (const auto &type : types) {
        for (const auto &enumName : std::as_const(enumNames)) {
            type->dependencies.removeAll(enumName);
        }
    }

    for (const auto &interface : m_data.interfaces) {
        for (const auto &enumName : std::as_const(enumNames)) {
            interface->dependencies.removeAll(enumName);
        }
    }
}
