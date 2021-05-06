#pragma once

#include "utils/json_utils.h"

#include <nlohmann/json.hpp>

#include <optional>
#include <string>
#include <unordered_map>
#include <variant>

namespace Lsp {
// TODO create real struct later
using ProgressParams = std::nullptr_t;

enum class ErrorCodes {
    // Defined by JSON RPC
    ParseError = -32700,
    InvalidRequest = -32600,
    MethodNotFound = -32601,
    InvalidParams = -32602,
    InternalError = -32603,

    /**
     * This is the start range of JSON RPC reserved error codes.
     * It doesn't denote a real error code. No LSP error codes should
     * be defined between the start and end range. For backwards
     * compatibility the `ServerNotInitialized` and the `UnknownErrorCode`
     * are left in the range.
     *
     * @since 3.16.0
     */
    JsonrpcReservedErrorRangeStart = -32099,
    /** @deprecated use jsonrpcReservedErrorRangeStart */
    ServerErrorStart = JsonrpcReservedErrorRangeStart,

    /**
     * Error code indicating that a server received a notification or
     * request before the server has received the `initialize` request.
     */
    ServerNotInitialized = -32002,
    UnknownErrorCode = -32001,

    /**
     * This is the start range of JSON RPC reserved error codes.
     * It doesn't denote a real error code.
     *
     * @since 3.16.0
     */
    JsonrpcReservedErrorRangeEnd = -32000,
    /** @deprecated use jsonrpcReservedErrorRangeEnd */
    ServerErrorEnd = JsonrpcReservedErrorRangeEnd,

    /**
     * This is the start range of LSP reserved error codes.
     * It doesn't denote a real error code.
     *
     * @since 3.16.0
     */
    LspReservedErrorRangeStart = -32899,

    ContentModified = -32801,
    RequestCancelled = -32800,

    /**
     * This is the end range of LSP reserved error codes.
     * It doesn't denote a real error code.
     *
     * @since 3.16.0
     */
    LspReservedErrorRangeEnd = -32800,
};

struct CancelParams
{
    /**
     * The request id to cancel.
     */
    std::variant<int, std::string> id;
};
JSONIFY(CancelParams, id);

using ProgressToken = std::variant<int, std::string>;

using DocumentUri = std::string;

enum class SymbolKind {
    File = 1,
    Module = 2,
    Namespace = 3,
    Package = 4,
    Class = 5,
    Method = 6,
    Property = 7,
    Field = 8,
    Constructor = 9,
    Enum = 10,
    Interface = 11,
    Function = 12,
    Variable = 13,
    Constant = 14,
    String = 15,
    Number = 16,
    Boolean = 17,
    Array = 18,
    Object = 19,
    Key = 20,
    Null = 21,
    EnumMember = 22,
    Struct = 23,
    Event = 24,
    Operator = 25,
    TypeParameter = 26,
};

struct RegularExpressionsClientCapabilities
{
    /**
     * The engine's name.
     */
    std::string engine;

    /**
     * The engine's version.
     */
    std::optional<std::string> version;
};
JSONIFY(RegularExpressionsClientCapabilities, engine, version);

using ResourceOperationKindType = std::string;

enum class ResourceOperationKind {

    /**
     * Supports creating new files and folders.
     */
    Create,

    /**
     * Supports renaming existing files and folders.
     */
    Rename,

    /**
     * Supports deleting existing files and folders.
     */
    Delete,
};

using FailureHandlingKindType = std::string;

enum class FailureHandlingKind {

    /**
     * Applying the workspace change is simply aborted if one of the changes
     * provided fails. All operations executed before the failing operation
     * stay executed.
     */
    Abort,

    /**
     * All operations are executed transactional. That means they either all
     * succeed or no changes at all are applied to the workspace.
     */
    Transactional,

    /**
     * If the workspace edit contains only textual file changes they are
     * executed transactional. If resource changes (create, rename or delete
     * file) are part of the change the failure handling strategy is abort.
     */
    TextOnlyTransactional,

    /**
     * The client tries to undo the operations already executed. But there is no
     * guarantee that this is succeeding.
     */
    Undo,
};

struct Position
{
    /**
     * Line position in a document (zero-based).
     */
    unsigned int line;

    /**
     * Character offset on a line in a document (zero-based). Assuming that
     * the line is represented as a string, the `character` value represents
     * the gap between the `character` and `character + 1`.
     *
     * If the character value is greater than the line length it defaults back
     * to the line length.
     */
    unsigned int character;
};
JSONIFY(Position, line, character);

struct Range
{
    /**
     * The range's start position.
     */
    Position start;

    /**
     * The range's end position.
     */
    Position end;
};
JSONIFY(Range, start, end);

struct Location
{
    DocumentUri uri;
    Range range;
};
JSONIFY(Location, uri, range);

struct LocationLink
{

    /**
     * Span of the origin of this link.
     *
     * Used as the underlined span for mouse interaction. Defaults to the word
     * range at the mouse position.
     */
    std::optional<Range> originSelectionRange;

    /**
     * The target resource identifier of this link.
     */
    DocumentUri targetUri;

    /**
     * The full target range of this link. If the target for example is a symbol
     * then target range is the range enclosing this symbol not including
     * leading/trailing whitespace but everything else like comments. This
     * information is typically used to highlight the range in the editor.
     */
    Range targetRange;

    /**
     * The range that should be selected and revealed when this link is being
     * followed, e.g the name of a function. Must be contained by the the
     * `targetRange`. See also `DocumentSymbol#range`
     */
    Range targetSelectionRange;
};
JSONIFY(LocationLink, originSelectionRange, targetUri, targetRange, targetSelectionRange);

enum class DiagnosticSeverity {
    /**
     * Reports an error.
     */
    Error = 1,
    /**
     * Reports a warning.
     */
    Warning = 2,
    /**
     * Reports an information.
     */
    Information = 3,
    /**
     * Reports a hint.
     */
    Hint = 4,
};

using DiagnosticSeverityType = int;

enum class DiagnosticTag {
    DiagnosticTag1 = 1,
    DiagnosticTag2 = 2,
    Information = 3,
    Hint = 4,
};
using DiagnosticTagType = int;

struct DiagnosticRelatedInformation
{
    /**
     * The location of this related diagnostic information.
     */
    Location location;

    /**
     * The message of this related diagnostic information.
     */
    std::string message;
};
JSONIFY(DiagnosticRelatedInformation, location, message);

struct CodeDescription
{
    /**
     * An URI to open with more information about the diagnostic error.
     */
    std::string href;
};
JSONIFY(CodeDescription, href);

struct Command
{
    /**
     * Title of the command, like `save`.
     */
    std::string title;
    /**
     * The identifier of the actual command handler.
     */
    std::string command;
    /**
     * Arguments that the command handler should be
     * invoked with.
     */
    std::optional<std::vector<nlohmann::json>> arguments;
};
JSONIFY(Command, title, command, arguments);

struct Diagnostic
{
    /**
     * The range at which the message applies.
     */
    Range range;

    /**
     * The diagnostic's severity. Can be omitted. If omitted it is up to the
     * client to interpret diagnostics as error, warning, info or hint.
     */
    std::optional<DiagnosticSeverity> severity;

    /**
     * The diagnostic's code, which might appear in the user interface.
     */
    std::optional<std::variant<int, std::string>> code;

    /**
     * An optional property to describe the error code.
     *
     * @since 3.16.0
     */
    std::optional<CodeDescription> codeDescription;

    /**
     * A human-readable string describing the source of this
     * diagnostic, e.g. 'typescript' or 'super lint'.
     */
    std::optional<std::string> source;

    /**
     * The diagnostic's message.
     */
    std::string message;

    /**
     * Additional metadata about the diagnostic.
     *
     * @since 3.15.0
     */
    std::optional<std::vector<DiagnosticTag>> tags;

    /**
     * An array of related diagnostic information, e.g. when symbol-names within
     * a scope collide all definitions can be marked via this property.
     */
    std::optional<std::vector<DiagnosticRelatedInformation>> relatedInformation;

    /**
     * A data entry field that is preserved between a
     * `textDocument/publishDiagnostics` notification and
     * `textDocument/codeAction` request.
     *
     * @since 3.16.0
     */
    std::optional<nlohmann::json> data;
};
JSONIFY(Diagnostic, range, severity, code, codeDescription, source, message, tags, relatedInformation, data);

struct TextEdit
{
    /**
     * The range of the text document to be manipulated. To insert
     * text into a document create a range where start === end.
     */
    Range range;

    /**
     * The string to be inserted. For delete operations use an
     * empty string.
     */
    std::string newText;
};
JSONIFY(TextEdit, range, newText);

struct ChangeAnnotation
{
    /**
     * A human-readable string describing the actual change. The string
     * is rendered prominent in the user interface.
     */
    std::string label;

    /**
     * A flag which indicates that user confirmation is needed
     * before applying the change.
     */
    std::optional<bool> needsConfirmation;

    /**
     * A human-readable string which is rendered less prominent in
     * the user interface.
     */
    std::optional<std::string> description;
};
JSONIFY(ChangeAnnotation, label, needsConfirmation, description);

using ChangeAnnotationIdentifier = std::string;

struct AnnotatedTextEdit : public TextEdit
{
    /**
     * The actual annotation identifier.
     */
    ChangeAnnotationIdentifier annotationId;
};
JSONIFY(AnnotatedTextEdit, annotationId, range, newText);

struct TextDocumentIdentifier
{
    /**
     * The text document's URI.
     */
    DocumentUri uri;
};
JSONIFY(TextDocumentIdentifier, uri);

struct OptionalVersionedTextDocumentIdentifier : public TextDocumentIdentifier
{
    /**
     * The version number of this document. If an optional versioned text document
     * identifier is sent from the server to the client and the file is not
     * open in the editor (the server has not received an open notification
     * before) the server can send `null` to indicate that the version is
     * known and the content on disk is the master (as specified with document
     * content ownership).
     *
     * The version number of a document will increase after each change,
     * including undo/redo. The number doesn't need to be consecutive.
     */
    std::variant<int, std::nullptr_t> version;
};
JSONIFY(OptionalVersionedTextDocumentIdentifier, version, uri);

struct TextDocumentEdit
{
    /**
     * The text document to change.
     */
    OptionalVersionedTextDocumentIdentifier textDocument;

    /**
     * The edits to be applied.
     *
     * @since 3.16.0 - support for AnnotatedTextEdit. This is guarded by the
     * client capability `workspace.workspaceEdit.changeAnnotationSupport`
     */
    std::vector<std::variant<TextEdit, AnnotatedTextEdit>> edits;
};
JSONIFY(TextDocumentEdit, textDocument, edits);

struct CreateFileOptions
{
    /**
     * Overwrite existing file. Overwrite wins over `ignoreIfExists`
     */
    std::optional<bool> overwrite;

    /**
     * Ignore if exists.
     */
    std::optional<bool> ignoreIfExists;
};
JSONIFY(CreateFileOptions, overwrite, ignoreIfExists);

struct CreateFile
{
    /**
     * A create
     */
    static inline const std::string kind = "create";

    /**
     * The resource to create.
     */
    DocumentUri uri;

    /**
     * Additional options
     */
    std::optional<CreateFileOptions> options;

    /**
     * An optional annotation identifer describing the operation.
     *
     * @since 3.16.0
     */
    std::optional<ChangeAnnotationIdentifier> annotationId;
};
JSONIFY(CreateFile, kind, uri, options, annotationId);

struct RenameFileOptions
{
    /**
     * Overwrite target if existing. Overwrite wins over `ignoreIfExists`
     */
    std::optional<bool> overwrite;

    /**
     * Ignores if target exists.
     */
    std::optional<bool> ignoreIfExists;
};
JSONIFY(RenameFileOptions, overwrite, ignoreIfExists);

struct RenameFile
{
    /**
     * A rename
     */
    static inline const std::string kind = "rename";

    /**
     * The old (existing) location.
     */
    DocumentUri oldUri;

    /**
     * The new location.
     */
    DocumentUri newUri;

    /**
     * Rename options.
     */
    std::optional<RenameFileOptions> options;

    /**
     * An optional annotation identifer describing the operation.
     *
     * @since 3.16.0
     */
    std::optional<ChangeAnnotationIdentifier> annotationId;
};
JSONIFY(RenameFile, kind, oldUri, newUri, options, annotationId);

struct DeleteFileOptions
{
    /**
     * Delete the content recursively if a folder is denoted.
     */
    std::optional<bool> recursive;

    /**
     * Ignore the operation if the file doesn't exist.
     */
    std::optional<bool> ignoreIfNotExists;
};
JSONIFY(DeleteFileOptions, recursive, ignoreIfNotExists);

struct DeleteFile
{
    /**
     * A delete
     */
    static inline const std::string kind = "delete";

    /**
     * The file to delete.
     */
    DocumentUri uri;

    /**
     * Delete options.
     */
    std::optional<DeleteFileOptions> options;

    /**
     * An optional annotation identifer describing the operation.
     *
     * @since 3.16.0
     */
    std::optional<ChangeAnnotationIdentifier> annotationId;
};
JSONIFY(DeleteFile, kind, uri, options, annotationId);

struct WorkspaceEdit
{
    /**
     * Holds changes to existing resources.
     */
    struct Changes
    {
        std::unordered_map<DocumentUri, std::vector<TextEdit>> map;
    };
    std::optional<Changes> changes;

    /**
     * Depending on the client capability
     * `workspace.workspaceEdit.resourceOperations` document changes are either
     * an array of `TextDocumentEdit`s to express changes to n different text
     * documents where each text document edit addresses a specific version of
     * a text document. Or it can contain above `TextDocumentEdit`s mixed with
     * create, rename and delete file / folder operations.
     *
     * Whether a client supports versioned document edits is expressed via
     * `workspace.workspaceEdit.documentChanges` client capability.
     *
     * If a client neither supports `documentChanges` nor
     * `workspace.workspaceEdit.resourceOperations` then only plain `TextEdit`s
     * using the `changes` property are supported.
     */
    std::optional<std::variant<std::vector<TextDocumentEdit>,
                               std::vector<std::variant<TextDocumentEdit, CreateFile, RenameFile, DeleteFile>>>>
        documentChanges;

    /**
     * A map of change annotations that can be referenced in
     * `AnnotatedTextEdit`s or create, rename and delete file / folder
     * operations.
     *
     * Whether clients honor this property depends on the client capability
     * `workspace.changeAnnotationSupport`.
     *
     * @since 3.16.0
     */
    struct ChangeAnnotations
    {
        std::unordered_map<std::string, ChangeAnnotation> map;
    };
    std::optional<ChangeAnnotations> changeAnnotations;
};
JSONIFY(WorkspaceEdit, changes, documentChanges, changeAnnotations);
JSONIFY(WorkspaceEdit::ChangeAnnotations, map);
JSONIFY(WorkspaceEdit::Changes, map);

struct WorkspaceEditClientCapabilities
{
    /**
     * The client supports versioned document changes in `WorkspaceEdit`s
     */
    std::optional<bool> documentChanges;

    /**
     * The resource operations the client supports. Clients should at least
     * support 'create', 'rename' and 'delete' files and folders.
     *
     * @since 3.13.0
     */
    std::optional<std::vector<ResourceOperationKind>> resourceOperations;

    /**
     * The failure handling strategy of a client if applying the workspace edit
     * fails.
     *
     * @since 3.13.0
     */
    std::optional<FailureHandlingKind> failureHandling;

    /**
     * Whether the client normalizes line endings to the client specific
     * setting.
     * If set to `true` the client will normalize line ending characters
     * in a workspace edit to the client specific new line character(s).
     *
     * @since 3.16.0
     */
    std::optional<bool> normalizesLineEndings;

    /**
     * Whether the client in general supports change annotations on text edits,
     * create file, rename file and delete file changes.
     *
     * @since 3.16.0
     */
    struct ChangeAnnotationSupport
    {
        /**
         * Whether the client groups edits with equal labels into tree nodes,
         * for instance all edits labelled with "Changes in Strings" would
         * be a tree node.
         */
        std::optional<bool> groupsOnLabel;
    };
    std::optional<ChangeAnnotationSupport> changeAnnotationSupport;
};
JSONIFY(WorkspaceEditClientCapabilities, documentChanges, resourceOperations, failureHandling, normalizesLineEndings,
        changeAnnotationSupport);
JSONIFY(WorkspaceEditClientCapabilities::ChangeAnnotationSupport, groupsOnLabel);

struct TextDocumentItem
{
    /**
     * The text document's URI.
     */
    DocumentUri uri;

    /**
     * The text document's language identifier.
     */
    std::string languageId;

    /**
     * The version number of this document (it will increase after each
     * change, including undo/redo).
     */
    int version;

    /**
     * The content of the opened text document.
     */
    std::string text;
};
JSONIFY(TextDocumentItem, uri, languageId, version, text);

struct VersionedTextDocumentIdentifier : public TextDocumentIdentifier
{
    /**
     * The version number of this document.
     *
     * The version number of a document will increase after each change,
     * including undo/redo. The number doesn't need to be consecutive.
     */
    int version;
};
JSONIFY(VersionedTextDocumentIdentifier, version, uri);

struct TextDocumentPositionParams
{
    /**
     * The text document.
     */
    TextDocumentIdentifier textDocument;

    /**
     * The position inside the text document.
     */
    Position position;
};
JSONIFY(TextDocumentPositionParams, textDocument, position);

struct DocumentFilter
{
    /**
     * A language id, like `typescript`.
     */
    std::optional<std::string> language;

    /**
     * A Uri [scheme](#Uri.scheme), like `file` or `untitled`.
     */
    std::optional<std::string> scheme;

    /**
     * A glob pattern, like `*.{ts,js}`.
     *
     * Glob patterns can have the following syntax:
     * - `*` to match one or more characters in a path segment
     * - `?` to match on one character in a path segment
     * - `**` to match any number of path segments, including none
     * - `{}` to group sub patterns into an OR expression. (e.g. `*.{ts,js}`
     *   matches all TypeScript and JavaScript files)
     * - `[]` to declare a range of characters to match in a path segment
     *   (e.g., `example.[0-9]` to match on `example.0`, `example.1`, âŠ)
     * - `[!...]` to negate a range of characters to match in a path segment
     *   (e.g., `example.[!0-9]` to match on `example.a`, `example.b`, but
     *   not `example.0`)
     */
    std::optional<std::string> pattern;
};
JSONIFY(DocumentFilter, language, scheme, pattern);

using DocumentSelector = std::vector<DocumentFilter>;

struct StaticRegistrationOptions
{
    /**
     * The id used to register the request. The id can be used to deregister
     * the request again. See also Registration#id.
     */
    std::optional<std::string> id;
};
JSONIFY(StaticRegistrationOptions, id);

struct TextDocumentRegistrationOptions
{
    /**
     * A document selector to identify the scope of the registration. If set to
     * null the document selector provided on the client side will be used.
     */
    std::variant<DocumentSelector, std::nullptr_t> documentSelector;
};
JSONIFY(TextDocumentRegistrationOptions, documentSelector);

enum class MarkupKind {
    /**
     * Plain text is supported as a content format
     */
    PlainText,

    /**
     * Markdown is supported as a content format
     */
    Markdown,
};

using MarkupKindType = std::string;

struct MarkupContent
{
    /**
     * The type of the Markup
     */
    MarkupKind kind;

    /**
     * The content itself
     */
    std::string value;
};
JSONIFY(MarkupContent, kind, value);

struct MarkdownClientCapabilities
{
    /**
     * The name of the parser.
     */
    std::string parser;

    /**
     * The version of the parser.
     */
    std::optional<std::string> version;
};
JSONIFY(MarkdownClientCapabilities, parser, version);

struct WorkDoneProgressBegin
{

    static inline const std::string kind = "begin";

    /**
     * Mandatory title of the progress operation. Used to briefly inform about
     * the kind of operation being performed.
     *
     * Examples: "Indexing" or "Linking dependencies".
     */
    std::string title;

    /**
     * Controls if a cancel button should show to allow the user to cancel the
     * long running operation. Clients that don't support cancellation are
     * allowed to ignore the setting.
     */
    std::optional<bool> cancellable;

    /**
     * Optional, more detailed associated progress message. Contains
     * complementary information to the `title`.
     *
     * Examples: "3/25 files", "project/src/module2", "node_modules/some_dep".
     * If unset, the previous progress message (if any) is still valid.
     */
    std::optional<std::string> message;

    /**
     * Optional progress percentage to display (value 100 is considered 100%).
     * If not provided infinite progress is assumed and clients are allowed
     * to ignore the `percentage` value in subsequent in report notifications.
     *
     * The value should be steadily rising. Clients are free to ignore values
     * that are not following this rule. The value range is [0, 100]
     */
    std::optional<unsigned int> percentage;
};
JSONIFY(WorkDoneProgressBegin, kind, title, cancellable, message, percentage);

struct WorkDoneProgressReport
{

    static inline const std::string kind = "report";

    /**
     * Controls enablement state of a cancel button. This property is only valid
     * if a cancel button got requested in the `WorkDoneProgressBegin` payload.
     *
     * Clients that don't support cancellation or don't support control the
     * button's enablement state are allowed to ignore the setting.
     */
    std::optional<bool> cancellable;

    /**
     * Optional, more detailed associated progress message. Contains
     * complementary information to the `title`.
     *
     * Examples: "3/25 files", "project/src/module2", "node_modules/some_dep".
     * If unset, the previous progress message (if any) is still valid.
     */
    std::optional<std::string> message;

    /**
     * Optional progress percentage to display (value 100 is considered 100%).
     * If not provided infinite progress is assumed and clients are allowed
     * to ignore the `percentage` value in subsequent in report notifications.
     *
     * The value should be steadily rising. Clients are free to ignore values
     * that are not following this rule. The value range is [0, 100]
     */
    std::optional<unsigned int> percentage;
};
JSONIFY(WorkDoneProgressReport, kind, cancellable, message, percentage);

struct WorkDoneProgressEnd
{

    static inline const std::string kind = "end";

    /**
     * Optional, a final message indicating to for example indicate the outcome
     * of the operation.
     */
    std::optional<std::string> message;
};
JSONIFY(WorkDoneProgressEnd, kind, message);

struct WorkDoneProgressParams
{
    /**
     * An optional token that a server can use to report work done progress.
     */
    std::optional<ProgressToken> workDoneToken;
};
JSONIFY(WorkDoneProgressParams, workDoneToken);

struct WorkDoneProgressOptions
{
    std::optional<bool> workDoneProgress;
};
JSONIFY(WorkDoneProgressOptions, workDoneProgress);

struct PartialResultParams
{
    /**
     * An optional token that a server can use to report partial results (e.g.
     * streaming) to the client.
     */
    std::optional<ProgressToken> partialResultToken;
};
JSONIFY(PartialResultParams, partialResultToken);

enum class TraceValue {
    Off,
    Message,
    Verbose,
};

using TraceValueType = std::string;

struct DidChangeConfigurationClientCapabilities
{
    /**
     * Did change configuration notification supports dynamic registration.
     */
    std::optional<bool> dynamicRegistration;
};
JSONIFY(DidChangeConfigurationClientCapabilities, dynamicRegistration);

struct DidChangeWatchedFilesClientCapabilities
{
    /**
     * Did change watched files notification supports dynamic registration.
     * Please note that the current protocol doesn't support static
     * configuration for file changes from the server side.
     */
    std::optional<bool> dynamicRegistration;
};
JSONIFY(DidChangeWatchedFilesClientCapabilities, dynamicRegistration);

enum class SymbolTag {

    /**
     * Render a symbol as obsolete, usually using a strike-out.
     */
    Deprecated = 1,
};

using SymbolTagType = int;

struct WorkspaceSymbolClientCapabilities
{
    /**
     * Symbol request supports dynamic registration.
     */
    std::optional<bool> dynamicRegistration;

    /**
     * Specific capabilities for the `SymbolKind` in the `workspace/symbol`
     * request.
     */
    struct SymbolKind
    {
        /**
         * The symbol kind values the client supports. When this
         * property exists the client also guarantees that it will
         * handle values outside its set gracefully and falls back
         * to a default value when unknown.
         *
         * If this property is not present the client only supports
         * the symbol kinds from `File` to `Array` as defined in
         * the initial version of the protocol.
         */
        std::optional<std::vector<SymbolKind>> valueSet;
    };
    std::optional<SymbolKind> symbolKind;

    /**
     * The client supports tags on `SymbolInformation`.
     * Clients supporting tags have to handle unknown tags gracefully.
     *
     * @since 3.16.0
     */
    struct TagSupport
    {
        /**
         * The tags supported by the client.
         */
        std::vector<SymbolTag> valueSet;
    };
    std::optional<TagSupport> tagSupport;
};
JSONIFY(WorkspaceSymbolClientCapabilities, dynamicRegistration, symbolKind, tagSupport);
JSONIFY(WorkspaceSymbolClientCapabilities::TagSupport, valueSet);
JSONIFY(WorkspaceSymbolClientCapabilities::SymbolKind, valueSet);

struct ExecuteCommandClientCapabilities
{
    /**
     * Execute command supports dynamic registration.
     */
    std::optional<bool> dynamicRegistration;
};
JSONIFY(ExecuteCommandClientCapabilities, dynamicRegistration);

struct SemanticTokensWorkspaceClientCapabilities
{
    /**
     * Whether the client implementation supports a refresh request sent from
     * the server to the client.
     *
     * Note that this event is global and will force the client to refresh all
     * semantic tokens currently shown. It should be used with absolute care
     * and is useful for situation where a server for example detect a project
     * wide change that requires such a calculation.
     */
    std::optional<bool> refreshSupport;
};
JSONIFY(SemanticTokensWorkspaceClientCapabilities, refreshSupport);

struct CodeLensWorkspaceClientCapabilities
{
    /**
     * Whether the client implementation supports a refresh request sent from the
     * server to the client.
     *
     * Note that this event is global and will force the client to refresh all
     * code lenses currently shown. It should be used with absolute care and is
     * useful for situation where a server for example detect a project wide
     * change that requires such a calculation.
     */
    std::optional<bool> refreshSupport;
};
JSONIFY(CodeLensWorkspaceClientCapabilities, refreshSupport);

struct ShowMessageRequestClientCapabilities
{
    /**
     * Capabilities specific to the `MessageActionItem` type.
     */
    struct MessageActionItem
    {
        /**
         * Whether the client supports additional attributes which
         * are preserved and sent back to the server in the
         * request's response.
         */
        std::optional<bool> additionalPropertiesSupport;
    };
    std::optional<MessageActionItem> messageActionItem;
};
JSONIFY(ShowMessageRequestClientCapabilities, messageActionItem);
JSONIFY(ShowMessageRequestClientCapabilities::MessageActionItem, additionalPropertiesSupport);

struct ShowDocumentClientCapabilities
{
    /**
     * The client has support for the show document
     * request.
     */
    bool support;
};
JSONIFY(ShowDocumentClientCapabilities, support);

struct WorkspaceFolder
{
    /**
     * The associated URI for this workspace folder.
     */
    DocumentUri uri;

    /**
     * The name of the workspace folder. Used to refer to this
     * workspace folder in the user interface.
     */
    std::string name;
};
JSONIFY(WorkspaceFolder, uri, name);

struct SaveOptions
{
    /**
     * The client is supposed to include the content on save.
     */
    std::optional<bool> includeText;
};
JSONIFY(SaveOptions, includeText);

enum class TextDocumentSyncKind {
    /**
     * Documents should not be synced at all.
     */
    None = 0,

    /**
     * Documents are synced by always sending the full content
     * of the document.
     */
    Full = 1,

    /**
     * Documents are synced by sending the full content on open.
     * After that only incremental updates to the document are
     * send.
     */
    Incremental = 2,
};

using TextDocumentSyncKindType = int;

struct TextDocumentSyncOptions
{
    /**
     * Open and close notifications are sent to the server. If omitted open
     * close notification should not be sent.
     */
    std::optional<bool> openClose;
    /**
     * Change notifications are sent to the server. See
     * TextDocumentSyncKind.None, TextDocumentSyncKind.Full and
     * TextDocumentSyncKind.Incremental. If omitted it defaults to
     * TextDocumentSyncKind.None.
     */
    std::optional<TextDocumentSyncKind> change;
    /**
     * If present will save notifications are sent to the server. If omitted
     * the notification should not be sent.
     */
    std::optional<bool> willSave;
    /**
     * If present will save wait until requests are sent to the server. If
     * omitted the request should not be sent.
     */
    std::optional<bool> willSaveWaitUntil;
    /**
     * If present save notifications are sent to the server. If omitted the
     * notification should not be sent.
     */
    std::optional<std::variant<bool, SaveOptions>> save;
};
JSONIFY(TextDocumentSyncOptions, openClose, change, openClose, change, willSave, willSaveWaitUntil, save);

struct CompletionOptions : public WorkDoneProgressOptions
{
    /**
     * Most tools trigger completion request automatically without explicitly
     * requesting it using a keyboard shortcut (e.g. Ctrl+Space). Typically they
     * do so when the user starts to type an identifier. For example if the user
     * types `c` in a JavaScript file code complete will automatically pop up
     * present `console` besides others as a completion item. Characters that
     * make up identifiers don't need to be listed here.
     *
     * If code complete should automatically be trigger on characters not being
     * valid inside an identifier (for example `.` in JavaScript) list them in
     * `triggerCharacters`.
     */
    std::optional<std::vector<std::string>> triggerCharacters;

    /**
     * The list of all possible characters that commit a completion. This field
     * can be used if clients don't support individual commit characters per
     * completion item. See client capability
     * `completion.completionItem.commitCharactersSupport`.
     *
     * If a server provides both `allCommitCharacters` and commit characters on
     * an individual completion item the ones on the completion item win.
     *
     * @since 3.2.0
     */
    std::optional<std::vector<std::string>> allCommitCharacters;

    /**
     * The server provides support to resolve additional
     * information for a completion item.
     */
    std::optional<bool> resolveProvider;
};
JSONIFY(CompletionOptions, triggerCharacters, allCommitCharacters, resolveProvider, workDoneProgress);

enum class CompletionItemKind {
    Text = 1,
    Method = 2,
    Function = 3,
    Constructor = 4,
    Field = 5,
    Variable = 6,
    Class = 7,
    Interface = 8,
    Module = 9,
    Property = 10,
    Unit = 11,
    Value = 12,
    Enum = 13,
    Keyword = 14,
    Snippet = 15,
    Color = 16,
    File = 17,
    Reference = 18,
    Folder = 19,
    EnumMember = 20,
    Constant = 21,
    Struct = 22,
    Event = 23,
    Operator = 24,
    TypeParameter = 25,
};

struct HoverClientCapabilities
{
    /**
     * Whether hover supports dynamic registration.
     */
    std::optional<bool> dynamicRegistration;

    /**
     * Client supports the following content formats if the content
     * property refers to a `literal of type MarkupContent`.
     * The order describes the preferred format of the client.
     */
    std::optional<std::vector<MarkupKind>> contentFormat;
};
JSONIFY(HoverClientCapabilities, dynamicRegistration, contentFormat);

struct HoverOptions : public WorkDoneProgressOptions
{
};
JSONIFY(HoverOptions, workDoneProgress);

struct HoverRegistrationOptions : public TextDocumentRegistrationOptions, public HoverOptions
{
};
JSONIFY(HoverRegistrationOptions, documentSelector, workDoneProgress);

struct HoverParams : public TextDocumentPositionParams, public WorkDoneProgressParams
{
};
JSONIFY(HoverParams, textDocument, position, workDoneToken);

/**
 * MarkedString can be used to render human readable text. It is either a
 * markdown string or a code-block that provides a language and a code snippet.
 * The language identifier is semantically equal to the optional language
 * identifier in fenced code blocks in GitHub issues.
 *
 * The pair of a language and a value is an equivalent to markdown:
 * ```${language}
 * ${value}
 * ```
 *
 * Note that markdown strings will be sanitized - that means html will be
 * escaped.
 *
 * @deprecated use MarkupContent instead.
 */

struct MarkedStringVariant
{
    std::string language;
    std::string value;
};
JSONIFY(MarkedStringVariant, language, value);

using MarkedString = std::variant<std::string, MarkedStringVariant>;

struct Hover
{
    /**
     * The hover's content
     */
    std::variant<MarkedString, std::vector<MarkedString>, MarkupContent> contents;

    /**
     * An optional range is a range inside a text document
     * that is used to visualize a hover, e.g. by changing the background color.
     */
    std::optional<Range> range;
};
JSONIFY(Hover, contents, range);

struct SignatureHelpOptions : public WorkDoneProgressOptions
{
    /**
     * The characters that trigger signature help
     * automatically.
     */
    std::optional<std::vector<std::string>> triggerCharacters;

    /**
     * List of characters that re-trigger signature help.
     *
     * These trigger characters are only active when signature help is already
     * showing. All trigger characters are also counted as re-trigger
     * characters.
     *
     * @since 3.15.0
     */
    std::optional<std::vector<std::string>> retriggerCharacters;
};
JSONIFY(SignatureHelpOptions, triggerCharacters, retriggerCharacters, workDoneProgress);

struct DeclarationOptions : public WorkDoneProgressOptions
{
};
JSONIFY(DeclarationOptions, workDoneProgress);

struct DeclarationRegistrationOptions : public DeclarationOptions,
                                        public TextDocumentRegistrationOptions,
                                        public StaticRegistrationOptions
{
};
JSONIFY(DeclarationRegistrationOptions, workDoneProgress, documentSelector, id);

struct DeclarationClientCapabilities
{
    /**
     * Whether declaration supports dynamic registration. If this is set to
     * `true` the client supports the new `DeclarationRegistrationOptions`
     * return value for the corresponding server capability as well.
     */
    std::optional<bool> dynamicRegistration;

    /**
     * The client supports additional metadata in the form of declaration links.
     */
    std::optional<bool> linkSupport;
};
JSONIFY(DeclarationClientCapabilities, dynamicRegistration, linkSupport);

struct DeclarationParams : public TextDocumentPositionParams, public WorkDoneProgressParams, public PartialResultParams
{
};
JSONIFY(DeclarationParams, textDocument, position, workDoneToken, partialResultToken);

struct DefinitionClientCapabilities
{
    /**
     * Whether definition supports dynamic registration.
     */
    std::optional<bool> dynamicRegistration;

    /**
     * The client supports additional metadata in the form of definition links.
     *
     * @since 3.14.0
     */
    std::optional<bool> linkSupport;
};
JSONIFY(DefinitionClientCapabilities, dynamicRegistration, linkSupport);

struct DefinitionOptions : public WorkDoneProgressOptions
{
};
JSONIFY(DefinitionOptions, workDoneProgress);

struct DefinitionRegistrationOptions : public TextDocumentRegistrationOptions, public DefinitionOptions
{
};
JSONIFY(DefinitionRegistrationOptions, documentSelector, workDoneProgress);

struct DefinitionParams : public TextDocumentPositionParams, public WorkDoneProgressParams, public PartialResultParams
{
};
JSONIFY(DefinitionParams, textDocument, position, workDoneToken, partialResultToken);

struct TypeDefinitionOptions : public WorkDoneProgressOptions
{
};
JSONIFY(TypeDefinitionOptions, workDoneProgress);

struct TypeDefinitionRegistrationOptions : public TextDocumentRegistrationOptions,
                                           public TypeDefinitionOptions,
                                           public StaticRegistrationOptions
{
};
JSONIFY(TypeDefinitionRegistrationOptions, documentSelector, workDoneProgress, id);

struct TypeDefinitionParams : public TextDocumentPositionParams,
                              public WorkDoneProgressParams,
                              public PartialResultParams
{
};
JSONIFY(TypeDefinitionParams, textDocument, position, workDoneToken, partialResultToken);

struct TypeDefinitionClientCapabilities
{
    /**
     * Whether implementation supports dynamic registration. If this is set to
     * `true` the client supports the new `TypeDefinitionRegistrationOptions`
     * return value for the corresponding server capability as well.
     */
    std::optional<bool> dynamicRegistration;

    /**
     * The client supports additional metadata in the form of definition links.
     *
     * @since 3.14.0
     */
    std::optional<bool> linkSupport;
};
JSONIFY(TypeDefinitionClientCapabilities, dynamicRegistration, linkSupport);

struct ImplementationClientCapabilities
{
    /**
     * Whether implementation supports dynamic registration. If this is set to
     * `true` the client supports the new `ImplementationRegistrationOptions`
     * return value for the corresponding server capability as well.
     */
    std::optional<bool> dynamicRegistration;

    /**
     * The client supports additional metadata in the form of definition links.
     *
     * @since 3.14.0
     */
    std::optional<bool> linkSupport;
};
JSONIFY(ImplementationClientCapabilities, dynamicRegistration, linkSupport);

struct ImplementationOptions : public WorkDoneProgressOptions
{
};
JSONIFY(ImplementationOptions, workDoneProgress);

struct ImplementationRegistrationOptions : public TextDocumentRegistrationOptions,
                                           public ImplementationOptions,
                                           public StaticRegistrationOptions
{
};
JSONIFY(ImplementationRegistrationOptions, documentSelector, workDoneProgress, id);

struct ImplementationParams : public TextDocumentPositionParams,
                              public WorkDoneProgressParams,
                              public PartialResultParams
{
};
JSONIFY(ImplementationParams, textDocument, position, workDoneToken, partialResultToken);

struct ReferenceOptions : public WorkDoneProgressOptions
{
};
JSONIFY(ReferenceOptions, workDoneProgress);

struct ReferenceRegistrationOptions : public TextDocumentRegistrationOptions, public ReferenceOptions
{
};
JSONIFY(ReferenceRegistrationOptions, documentSelector, workDoneProgress);

struct ReferenceContext
{
    /**
     * Include the declaration of the current symbol.
     */
    bool includeDeclaration;
};
JSONIFY(ReferenceContext, includeDeclaration);

struct ReferenceParams : public TextDocumentPositionParams, public WorkDoneProgressParams, public PartialResultParams
{
    ReferenceContext context;
};
JSONIFY(ReferenceParams, context, textDocument, position, workDoneToken, partialResultToken);

struct ReferenceClientCapabilities
{
    /**
     * Whether references supports dynamic registration.
     */
    std::optional<bool> dynamicRegistration;
};
JSONIFY(ReferenceClientCapabilities, dynamicRegistration);

struct DocumentHighlightClientCapabilities
{
    /**
     * Whether document highlight supports dynamic registration.
     */
    std::optional<bool> dynamicRegistration;
};
JSONIFY(DocumentHighlightClientCapabilities, dynamicRegistration);

struct DocumentHighlightOptions : public WorkDoneProgressOptions
{
};
JSONIFY(DocumentHighlightOptions, workDoneProgress);

struct DocumentHighlightRegistrationOptions : public TextDocumentRegistrationOptions, public DocumentHighlightOptions
{
};
JSONIFY(DocumentHighlightRegistrationOptions, documentSelector, workDoneProgress);

struct DocumentHighlightParams : public TextDocumentPositionParams,
                                 public WorkDoneProgressParams,
                                 public PartialResultParams
{
};
JSONIFY(DocumentHighlightParams, textDocument, position, workDoneToken, partialResultToken);

enum class DocumentHighlightKind {
    /**
     * A textual occurrence.
     */
    Text = 1,

    /**
     * Read-access of a symbol, like reading a variable.
     */
    Read = 2,

    /**
     * Write-access of a symbol, like writing to a variable.
     */
    Write = 3,
};

using DocumentHighlightKindType = int;

struct DocumentHighlight
{
    /**
     * The range this highlight applies to.
     */
    Range range;

    /**
     * The highlight kind, default is DocumentHighlightKind.Text.
     */
    std::optional<DocumentHighlightKind> kind;
};
JSONIFY(DocumentHighlight, range, kind);

struct DocumentSymbolOptions : public WorkDoneProgressOptions
{
    /**
     * A human-readable string that is shown when multiple outlines trees
     * are shown for the same document.
     *
     * @since 3.16.0
     */
    std::optional<std::string> label;
};
JSONIFY(DocumentSymbolOptions, label, workDoneProgress);

struct DocumentSymbolRegistrationOptions : public TextDocumentRegistrationOptions, public DocumentSymbolOptions
{
};
JSONIFY(DocumentSymbolRegistrationOptions, documentSelector, label, workDoneProgress);

struct DocumentSymbolParams : public WorkDoneProgressParams, public PartialResultParams
{
    /**
     * The text document.
     */
    TextDocumentIdentifier textDocument;
};
JSONIFY(DocumentSymbolParams, textDocument, workDoneToken, partialResultToken);

struct DocumentSymbolClientCapabilities
{
    /**
     * Whether document symbol supports dynamic registration.
     */
    std::optional<bool> dynamicRegistration;

    /**
     * Specific capabilities for the `SymbolKind` in the
     * `textDocument/documentSymbol` request.
     */
    struct SymbolKind
    {
        /**
         * The symbol kind values the client supports. When this
         * property exists the client also guarantees that it will
         * handle values outside its set gracefully and falls back
         * to a default value when unknown.
         *
         * If this property is not present the client only supports
         * the symbol kinds from `File` to `Array` as defined in
         * the initial version of the protocol.
         */
        std::optional<std::vector<SymbolKind>> valueSet;
    };
    std::optional<SymbolKind> symbolKind;

    /**
     * The client supports hierarchical document symbols.
     */
    std::optional<bool> hierarchicalDocumentSymbolSupport;

    /**
     * The client supports tags on `SymbolInformation`. Tags are supported on
     * `DocumentSymbol` if `hierarchicalDocumentSymbolSupport` is set to true.
     * Clients supporting tags have to handle unknown tags gracefully.
     *
     * @since 3.16.0
     */
    struct TagSupport
    {
        /**
         * The tags supported by the client.
         */
        std::vector<SymbolTag> valueSet;
    };
    std::optional<TagSupport> tagSupport;

    /**
     * The client supports an additional label presented in the UI when
     * registering a document symbol provider.
     *
     * @since 3.16.0
     */
    std::optional<bool> labelSupport;
};
JSONIFY(DocumentSymbolClientCapabilities, dynamicRegistration, symbolKind, hierarchicalDocumentSymbolSupport,
        tagSupport, labelSupport);
JSONIFY(DocumentSymbolClientCapabilities::TagSupport, valueSet);
JSONIFY(DocumentSymbolClientCapabilities::SymbolKind, valueSet);

struct DocumentSymbol
{

    /**
     * The name of this symbol. Will be displayed in the user interface and
     * therefore must not be an empty string or a string only consisting of
     * white spaces.
     */
    std::string name;

    /**
     * More detail for this symbol, e.g the signature of a function.
     */
    std::optional<std::string> detail;

    /**
     * The kind of this symbol.
     */
    SymbolKind kind;

    /**
     * Tags for this document symbol.
     *
     * @since 3.16.0
     */
    std::optional<std::vector<SymbolTag>> tags;

    /**
     * Indicates if this symbol is deprecated.
     *
     * @deprecated Use tags instead
     */
    std::optional<bool> deprecated;

    /**
     * The range enclosing this symbol not including leading/trailing whitespace
     * but everything else like comments. This information is typically used to
     * determine if the clients cursor is inside the symbol to reveal in the
     * symbol in the UI.
     */
    Range range;

    /**
     * The range that should be selected and revealed when this symbol is being
     * picked, e.g. the name of a function. Must be contained by the `range`.
     */
    Range selectionRange;

    /**
     * Children of this symbol, e.g. properties of a class.
     */
    std::optional<std::vector<DocumentSymbol>> children;
};
JSONIFY(DocumentSymbol, name, detail, kind, tags, deprecated, range, selectionRange, children);

using CodeActionKindType = std::string;

enum class CodeActionKind {

    /**
     * Empty kind.
     */
    Empty,

    /**
     * Base kind for quickfix actions: 'quickfix'.
     */
    QuickFix,

    /**
     * Base kind for refactoring actions: 'refactor'.
     */
    Refactor,

    /**
     * Base kind for refactoring extraction actions: 'refactor.extract'.
     *
     * Example extract actions:
     *
     * - Extract method
     * - Extract function
     * - Extract variable
     * - Extract interface from class
     * - ...
     */
    RefactorExtract,

    /**
     * Base kind for refactoring inline actions: 'refactor.inline'.
     *
     * Example inline actions:
     *
     * - Inline function
     * - Inline variable
     * - Inline constant
     * - ...
     */
    RefactorInline,

    /**
     * Base kind for refactoring rewrite actions: 'refactor.rewrite'.
     *
     * Example rewrite actions:
     *
     * - Convert JavaScript function to class
     * - Add or remove parameter
     * - Encapsulate field
     * - Make method static
     * - Move method to base class
     * - ...
     */
    RefactorRewrite,

    /**
     * Base kind for source actions: `source`.
     *
     * Source code actions apply to the entire file.
     */
    Source,

    /**
     * Base kind for an organize imports source action:
     * `source.organizeImports`.
     */
    SourceOrganizeImports,
};

struct CodeActionOptions : public WorkDoneProgressOptions
{
    /**
     * CodeActionKinds that this server may return.
     *
     * The list of kinds may be generic, such as `CodeActionKind.Refactor`,
     * or the server may list out every specific kind they provide.
     */
    std::optional<std::vector<CodeActionKind>> codeActionKinds;

    /**
     * The server provides support to resolve additional
     * information for a code action.
     *
     * @since 3.16.0
     */
    std::optional<bool> resolveProvider;
};
JSONIFY(CodeActionOptions, codeActionKinds, resolveProvider, workDoneProgress);

struct CodeActionRegistrationOptions : public TextDocumentRegistrationOptions, public CodeActionOptions
{
};
JSONIFY(CodeActionRegistrationOptions, documentSelector, codeActionKinds, resolveProvider, workDoneProgress);

struct CodeActionContext
{
    /**
     * An array of diagnostics known on the client side overlapping the range
     * provided to the `textDocument/codeAction` request. They are provided so
     * that the server knows which errors are currently presented to the user
     * for the given range. There is no guarantee that these accurately reflect
     * the error state of the resource. The primary parameter
     * to compute code actions is the provided range.
     */
    std::vector<Diagnostic> diagnostics;

    /**
     * Requested kind of actions to return.
     *
     * Actions not of this kind are filtered out by the client before being
     * shown. So servers can omit computing them.
     */
    std::optional<std::vector<CodeActionKind>> only;
};
JSONIFY(CodeActionContext, diagnostics, only);

struct CodeActionParams : public WorkDoneProgressParams, public PartialResultParams
{
    /**
     * The document in which the command was invoked.
     */
    TextDocumentIdentifier textDocument;

    /**
     * The range for which the command was invoked.
     */
    Range range;

    /**
     * Context carrying additional information.
     */
    CodeActionContext context;
};
JSONIFY(CodeActionParams, textDocument, range, context, workDoneToken, partialResultToken);

struct CodeAction
{

    /**
     * A short, human-readable, title for this code action.
     */
    std::string title;

    /**
     * The kind of the code action.
     *
     * Used to filter code actions.
     */
    std::optional<CodeActionKind> kind;

    /**
     * The diagnostics that this code action resolves.
     */
    std::optional<std::vector<Diagnostic>> diagnostics;

    /**
     * Marks this as a preferred action. Preferred actions are used by the
     * `auto fix` command and can be targeted by keybindings.
     *
     * A quick fix should be marked preferred if it properly addresses the
     * underlying error. A refactoring should be marked preferred if it is the
     * most reasonable choice of actions to take.
     *
     * @since 3.15.0
     */
    std::optional<bool> isPreferred;

    /**
     * Marks that the code action cannot currently be applied.
     *
     * Clients should follow the following guidelines regarding disabled code
     * actions:
     *
     * - Disabled code actions are not shown in automatic lightbulbs code
     *   action menus.
     *
     * - Disabled actions are shown as faded out in the code action menu when
     *   the user request a more specific type of code action, such as
     *   refactorings.
     *
     * - If the user has a keybinding that auto applies a code action and only
     *   a disabled code actions are returned, the client should show the user
     *   an error message with `reason` in the editor.
     *
     * @since 3.16.0
     */
    struct Disabled
    {

        /**
         * Human readable description of why the code action is currently
         * disabled.
         *
         * This is displayed in the code actions UI.
         */
        std::string reason;
    };
    std::optional<Disabled> disabled;

    /**
     * The workspace edit this code action performs.
     */
    std::optional<WorkspaceEdit> edit;

    /**
     * A command this code action executes. If a code action
     * provides an edit and a command, first the edit is
     * executed and then the command.
     */
    std::optional<Command> command;

    /**
     * A data entry field that is preserved on a code action between
     * a `textDocument/codeAction` and a `codeAction/resolve` request.
     *
     * @since 3.16.0
     */
    std::optional<nlohmann::json> data;
};
JSONIFY(CodeAction, title, kind, diagnostics, isPreferred, disabled, edit, command, data);
JSONIFY(CodeAction::Disabled, reason);

struct CodeActionClientCapabilities
{
    /**
     * Whether code action supports dynamic registration.
     */
    std::optional<bool> dynamicRegistration;

    /**
     * The client supports code action literals as a valid
     * response of the `textDocument/codeAction` request.
     *
     * @since 3.8.0
     */
    struct CodeActionLiteralSupport
    {
        /**
         * The code action kind is supported with the following value
         * set.
         */
        struct CodeActionKind
        {

            /**
             * The code action kind values the client supports. When this
             * property exists the client also guarantees that it will
             * handle values outside its set gracefully and falls back
             * to a default value when unknown.
             */
            std::vector<CodeActionKind> valueSet;
        };
        CodeActionKind codeActionKind;
    };
    std::optional<CodeActionLiteralSupport> codeActionLiteralSupport;

    /**
     * Whether code action supports the `isPreferred` property.
     *
     * @since 3.15.0
     */
    std::optional<bool> isPreferredSupport;

    /**
     * Whether code action supports the `disabled` property.
     *
     * @since 3.16.0
     */
    std::optional<bool> disabledSupport;

    /**
     * Whether code action supports the `data` property which is
     * preserved between a `textDocument/codeAction` and a
     * `codeAction/resolve` request.
     *
     * @since 3.16.0
     */
    std::optional<bool> dataSupport;

    /**
     * Whether the client supports resolving additional code action
     * properties via a separate `codeAction/resolve` request.
     *
     * @since 3.16.0
     */
    struct ResolveSupport
    {
        /**
         * The properties that a client can resolve lazily.
         */
        std::vector<std::string> properties;
    };
    std::optional<ResolveSupport> resolveSupport;

    /**
     * Whether the client honors the change annotations in
     * text edits and resource operations returned via the
     * `CodeAction#edit` property by for example presenting
     * the workspace edit in the user interface and asking
     * for confirmation.
     *
     * @since 3.16.0
     */
    std::optional<bool> honorsChangeAnnotations;
};
JSONIFY(CodeActionClientCapabilities, dynamicRegistration, codeActionLiteralSupport, isPreferredSupport,
        disabledSupport, dataSupport, resolveSupport, honorsChangeAnnotations);
JSONIFY(CodeActionClientCapabilities::ResolveSupport, properties);
JSONIFY(CodeActionClientCapabilities::CodeActionLiteralSupport, codeActionKind);
JSONIFY(CodeActionClientCapabilities::CodeActionLiteralSupport::CodeActionKind, valueSet);

struct DocumentFormattingClientCapabilities
{
    /**
     * Whether formatting supports dynamic registration.
     */
    std::optional<bool> dynamicRegistration;
};
JSONIFY(DocumentFormattingClientCapabilities, dynamicRegistration);

struct DocumentFormattingOptions : public WorkDoneProgressOptions
{
};
JSONIFY(DocumentFormattingOptions, workDoneProgress);

struct DocumentFormattingRegistrationOptions : public TextDocumentRegistrationOptions, public DocumentFormattingOptions
{
};
JSONIFY(DocumentFormattingRegistrationOptions, documentSelector, workDoneProgress);

struct FormattingOptions
{
    /**
     * Size of a tab in spaces.
     */
    unsigned int tabSize;

    /**
     * Prefer spaces over tabs.
     */
    bool insertSpaces;

    /**
     * Trim trailing whitespace on a line.
     *
     * @since 3.15.0
     */
    std::optional<bool> trimTrailingWhitespace;

    /**
     * Insert a newline character at the end of the file if one does not exist.
     *
     * @since 3.15.0
     */
    std::optional<bool> insertFinalNewline;

    /**
     * Trim all newlines after the final newline at the end of the file.
     *
     * @since 3.15.0
     */
    std::optional<bool> trimFinalNewlines;

    /**
     * Signature for further properties.
     */
    std::unordered_map<std::string, std::variant<bool, int, std::string>> map;
};
JSONIFY(FormattingOptions, tabSize, insertSpaces, trimTrailingWhitespace, insertFinalNewline, trimFinalNewlines, map);

struct DocumentFormattingParams : public WorkDoneProgressParams
{
    /**
     * The document to format.
     */
    TextDocumentIdentifier textDocument;

    /**
     * The format options.
     */
    FormattingOptions options;
};
JSONIFY(DocumentFormattingParams, textDocument, options, workDoneToken);

struct DocumentRangeFormattingClientCapabilities
{
    /**
     * Whether formatting supports dynamic registration.
     */
    std::optional<bool> dynamicRegistration;
};
JSONIFY(DocumentRangeFormattingClientCapabilities, dynamicRegistration);

struct DocumentRangeFormattingOptions : public WorkDoneProgressOptions
{
};
JSONIFY(DocumentRangeFormattingOptions, workDoneProgress);

struct DocumentRangeFormattingRegistrationOptions : public TextDocumentRegistrationOptions,
                                                    public DocumentRangeFormattingOptions
{
};
JSONIFY(DocumentRangeFormattingRegistrationOptions, documentSelector, workDoneProgress);

struct DocumentRangeFormattingParams : public WorkDoneProgressParams
{
    /**
     * The document to format.
     */
    TextDocumentIdentifier textDocument;

    /**
     * The range to format
     */
    Range range;

    /**
     * The format options
     */
    FormattingOptions options;
};
JSONIFY(DocumentRangeFormattingParams, textDocument, range, options, workDoneToken);

struct DocumentOnTypeFormattingClientCapabilities
{
    /**
     * Whether on type formatting supports dynamic registration.
     */
    std::optional<bool> dynamicRegistration;
};
JSONIFY(DocumentOnTypeFormattingClientCapabilities, dynamicRegistration);

struct DocumentOnTypeFormattingOptions
{
    /**
     * A character on which formatting should be triggered, like `}`.
     */
    std::string firstTriggerCharacter;

    /**
     * More trigger characters.
     */
    std::optional<std::vector<std::string>> moreTriggerCharacter;
};
JSONIFY(DocumentOnTypeFormattingOptions, firstTriggerCharacter, moreTriggerCharacter);

struct DocumentOnTypeFormattingRegistrationOptions : public TextDocumentRegistrationOptions,
                                                     public DocumentOnTypeFormattingOptions
{
};
JSONIFY(DocumentOnTypeFormattingRegistrationOptions, documentSelector, firstTriggerCharacter, moreTriggerCharacter);

struct DocumentOnTypeFormattingParams : public TextDocumentPositionParams
{
    /**
     * The character that has been typed.
     */
    std::string ch;

    /**
     * The format options.
     */
    FormattingOptions options;
};
JSONIFY(DocumentOnTypeFormattingParams, ch, options, textDocument, position);

struct CodeLensClientCapabilities
{
    /**
     * Whether code lens supports dynamic registration.
     */
    std::optional<bool> dynamicRegistration;
};
JSONIFY(CodeLensClientCapabilities, dynamicRegistration);

struct CodeLensOptions : public WorkDoneProgressOptions
{
    /**
     * Code lens has a resolve provider as well.
     */
    std::optional<bool> resolveProvider;
};
JSONIFY(CodeLensOptions, resolveProvider, workDoneProgress);

struct CodeLensRegistrationOptions : public TextDocumentRegistrationOptions, public CodeLensOptions
{
};
JSONIFY(CodeLensRegistrationOptions, documentSelector, resolveProvider, workDoneProgress);

struct CodeLensParams : public WorkDoneProgressParams, public PartialResultParams
{
    /**
     * The document to request code lens for.
     */
    TextDocumentIdentifier textDocument;
};
JSONIFY(CodeLensParams, textDocument, workDoneToken, partialResultToken);

struct CodeLens
{
    /**
     * The range in which this code lens is valid. Should only span a single
     * line.
     */
    Range range;

    /**
     * The command this code lens represents.
     */
    std::optional<Command> command;

    /**
     * A data entry field that is preserved on a code lens item between
     * a code lens and a code lens resolve request.
     */
    std::optional<nlohmann::json> data;
};
JSONIFY(CodeLens, range, command, data);

struct DocumentLinkClientCapabilities
{
    /**
     * Whether document link supports dynamic registration.
     */
    std::optional<bool> dynamicRegistration;

    /**
     * Whether the client supports the `tooltip` property on `DocumentLink`.
     *
     * @since 3.15.0
     */
    std::optional<bool> tooltipSupport;
};
JSONIFY(DocumentLinkClientCapabilities, dynamicRegistration, tooltipSupport);

struct DocumentLinkOptions : public WorkDoneProgressOptions
{
    /**
     * Document links have a resolve provider as well.
     */
    std::optional<bool> resolveProvider;
};
JSONIFY(DocumentLinkOptions, resolveProvider, workDoneProgress);

struct DocumentLinkRegistrationOptions : public TextDocumentRegistrationOptions, public DocumentLinkOptions
{
};
JSONIFY(DocumentLinkRegistrationOptions, documentSelector, resolveProvider, workDoneProgress);

struct DocumentLinkParams : public WorkDoneProgressParams, public PartialResultParams
{
    /**
     * The document to provide document links for.
     */
    TextDocumentIdentifier textDocument;
};
JSONIFY(DocumentLinkParams, textDocument, workDoneToken, partialResultToken);

struct DocumentLink
{
    /**
     * The range this link applies to.
     */
    Range range;

    /**
     * The uri this link points to. If missing a resolve request is sent later.
     */
    std::optional<DocumentUri> target;

    /**
     * The tooltip text when you hover over this link.
     *
     * If a tooltip is provided, is will be displayed in a string that includes
     * instructions on how to trigger the link, such as `{0} (ctrl + click)`.
     * The specific instructions vary depending on OS, user settings, and
     * localization.
     *
     * @since 3.15.0
     */
    std::optional<std::string> tooltip;

    /**
     * A data entry field that is preserved on a document link between a
     * DocumentLinkRequest and a DocumentLinkResolveRequest.
     */
    std::optional<nlohmann::json> data;
};
JSONIFY(DocumentLink, range, target, tooltip, data);

struct DocumentColorClientCapabilities
{
    /**
     * Whether document color supports dynamic registration.
     */
    std::optional<bool> dynamicRegistration;
};
JSONIFY(DocumentColorClientCapabilities, dynamicRegistration);

struct DocumentColorOptions : public WorkDoneProgressOptions
{
};
JSONIFY(DocumentColorOptions, workDoneProgress);

struct DocumentColorRegistrationOptions : public TextDocumentRegistrationOptions,
                                          public StaticRegistrationOptions,
                                          public DocumentColorOptions
{
};
JSONIFY(DocumentColorRegistrationOptions, documentSelector, id, workDoneProgress);

struct DocumentColorParams : public WorkDoneProgressParams, public PartialResultParams
{
    /**
     * The text document.
     */
    TextDocumentIdentifier textDocument;
};
JSONIFY(DocumentColorParams, textDocument, workDoneToken, partialResultToken);

enum class PrepareSupportDefaultBehavior {
    /**
     * The client's default behavior is to select the identifier
     * according the to language's syntax rule.
     */
    Identifier = 1,
};

struct RenameClientCapabilities
{
    /**
     * Whether rename supports dynamic registration.
     */
    std::optional<bool> dynamicRegistration;

    /**
     * Client supports testing for validity of rename operations
     * before execution.
     *
     * @since 3.12.0
     */
    std::optional<bool> prepareSupport;

    /**
     * Client supports the default behavior result
     * (`{ defaultBehavior: boolean }`).
     *
     * The value indicates the default behavior used by the
     * client.
     *
     * @since 3.16.0
     */
    std::optional<PrepareSupportDefaultBehavior> prepareSupportDefaultBehavior;

    /**
     * Whether th client honors the change annotations in
     * text edits and resource operations returned via the
     * rename request's workspace edit by for example presenting
     * the workspace edit in the user interface and asking
     * for confirmation.
     *
     * @since 3.16.0
     */
    std::optional<bool> honorsChangeAnnotations;
};
JSONIFY(RenameClientCapabilities, dynamicRegistration, prepareSupport, prepareSupportDefaultBehavior,
        honorsChangeAnnotations);

struct RenameOptions : public WorkDoneProgressOptions
{
    /**
     * Renames should be checked and tested before being executed.
     */
    std::optional<bool> prepareProvider;
};
JSONIFY(RenameOptions, prepareProvider, workDoneProgress);

struct RenameRegistrationOptions : public TextDocumentRegistrationOptions, public RenameOptions
{
};
JSONIFY(RenameRegistrationOptions, documentSelector, prepareProvider, workDoneProgress);

struct RenameParams : public TextDocumentPositionParams, public WorkDoneProgressParams
{
    /**
     * The new name of the symbol. If the given name is not valid the
     * request must return a [ResponseError](#ResponseError) with an
     * appropriate message set.
     */
    std::string newName;
};
JSONIFY(RenameParams, newName, textDocument, position, workDoneToken);

struct PrepareRenameParams : public TextDocumentPositionParams
{
};
JSONIFY(PrepareRenameParams, textDocument, position);

struct FoldingRangeClientCapabilities
{
    /**
     * Whether implementation supports dynamic registration for folding range
     * providers. If this is set to `true` the client supports the new
     * `FoldingRangeRegistrationOptions` return value for the corresponding
     * server capability as well.
     */
    std::optional<bool> dynamicRegistration;
    /**
     * The maximum number of folding ranges that the client prefers to receive
     * per document. The value serves as a hint, servers are free to follow the
     * limit.
     */
    std::optional<unsigned int> rangeLimit;
    /**
     * If set, the client signals that it only supports folding complete lines.
     * If set, client will ignore specified `startCharacter` and `endCharacter`
     * properties in a FoldingRange.
     */
    std::optional<bool> lineFoldingOnly;
};
JSONIFY(FoldingRangeClientCapabilities, dynamicRegistration, rangeLimit, lineFoldingOnly);

struct FoldingRangeOptions : public WorkDoneProgressOptions
{
};
JSONIFY(FoldingRangeOptions, workDoneProgress);

struct FoldingRangeRegistrationOptions : public TextDocumentRegistrationOptions,
                                         public FoldingRangeOptions,
                                         public StaticRegistrationOptions
{
};
JSONIFY(FoldingRangeRegistrationOptions, documentSelector, workDoneProgress, id);

struct FoldingRangeParams : public WorkDoneProgressParams, public PartialResultParams
{
    /**
     * The text document.
     */
    TextDocumentIdentifier textDocument;
};
JSONIFY(FoldingRangeParams, textDocument, workDoneToken, partialResultToken);

enum class FoldingRangeKind {
    /**
     * Folding range for a comment
     */
    Comment,
    /**
     * Folding range for a imports or includes
     */
    Imports,
    /**
     * Folding range for a region (e.g. `#region`)
     */
    Region,
};

struct FoldingRange
{

    /**
     * The zero-based start line of the range to fold. The folded area starts
     * after the line's last character. To be valid, the end must be zero or
     * larger and smaller than the number of lines in the document.
     */
    unsigned int startLine;

    /**
     * The zero-based character offset from where the folded range starts. If
     * not defined, defaults to the length of the start line.
     */
    std::optional<unsigned int> startCharacter;

    /**
     * The zero-based end line of the range to fold. The folded area ends with
     * the line's last character. To be valid, the end must be zero or larger
     * and smaller than the number of lines in the document.
     */
    unsigned int endLine;

    /**
     * The zero-based character offset before the folded range ends. If not
     * defined, defaults to the length of the end line.
     */
    std::optional<unsigned int> endCharacter;

    /**
     * Describes the kind of the folding range such as `comment` or `region`.
     * The kind is used to categorize folding ranges and used by commands like
     * 'Fold all comments'. See [FoldingRangeKind](#FoldingRangeKind) for an
     * enumeration of standardized kinds.
     */
    std::optional<std::string> kind;
};
JSONIFY(FoldingRange, startLine, startCharacter, endLine, endCharacter, kind);

struct SelectionRangeClientCapabilities
{
    /**
     * Whether implementation supports dynamic registration for selection range
     * providers. If this is set to `true` the client supports the new
     * `SelectionRangeRegistrationOptions` return value for the corresponding
     * server capability as well.
     */
    std::optional<bool> dynamicRegistration;
};
JSONIFY(SelectionRangeClientCapabilities, dynamicRegistration);

struct SelectionRangeOptions : public WorkDoneProgressOptions
{
};
JSONIFY(SelectionRangeOptions, workDoneProgress);

struct SelectionRangeRegistrationOptions : public SelectionRangeOptions,
                                           public TextDocumentRegistrationOptions,
                                           public StaticRegistrationOptions
{
};
JSONIFY(SelectionRangeRegistrationOptions, workDoneProgress, documentSelector, id);

struct SelectionRangeParams : public WorkDoneProgressParams, public PartialResultParams
{
    /**
     * The text document.
     */
    TextDocumentIdentifier textDocument;

    /**
     * The positions inside the text document.
     */
    std::vector<Position> positions;
};
JSONIFY(SelectionRangeParams, textDocument, positions, workDoneToken, partialResultToken);

struct SelectionRange
{
    /**
     * The [range](#Range) of this selection range.
     */
    Range range;
    /**
     * The parent selection range containing this range. Therefore
     * `parent.range` must contain `this.range`.
     */
    std::optional<Range> parent; // is SelectionRange in the spec
};
JSONIFY(SelectionRange, range, parent);

struct LinkedEditingRangeClientCapabilities
{
    /**
     * Whether implementation supports dynamic registration.
     * If this is set to `true` the client supports the new
     * `(TextDocumentRegistrationOptions & StaticRegistrationOptions)`
     * return value for the corresponding server capability as well.
     */
    std::optional<bool> dynamicRegistration;
};
JSONIFY(LinkedEditingRangeClientCapabilities, dynamicRegistration);

struct LinkedEditingRangeOptions : public WorkDoneProgressOptions
{
};
JSONIFY(LinkedEditingRangeOptions, workDoneProgress);

struct LinkedEditingRangeRegistrationOptions : public TextDocumentRegistrationOptions,
                                               public LinkedEditingRangeOptions,
                                               public StaticRegistrationOptions
{
};
JSONIFY(LinkedEditingRangeRegistrationOptions, documentSelector, workDoneProgress, id);

struct LinkedEditingRangeParams : public TextDocumentPositionParams, public WorkDoneProgressParams
{
};
JSONIFY(LinkedEditingRangeParams, textDocument, position, workDoneToken);

struct LinkedEditingRanges
{
    /**
     * A list of ranges that can be renamed together. The ranges must have
     * identical length and contain identical text content. The ranges cannot overlap.
     */
    std::vector<Range> ranges;

    /**
     * An optional word pattern (regular expression) that describes valid contents for
     * the given ranges. If no pattern is provided, the client configuration's word
     * pattern will be used.
     */
    std::optional<std::string> wordPattern;
};
JSONIFY(LinkedEditingRanges, ranges, wordPattern);

struct MonikerClientCapabilities
{
    /**
     * Whether implementation supports dynamic registration. If this is set to
     * `true` the client supports the new `(TextDocumentRegistrationOptions &
     * StaticRegistrationOptions)` return value for the corresponding server
     * capability as well.
     */
    std::optional<bool> dynamicRegistration;
};
JSONIFY(MonikerClientCapabilities, dynamicRegistration);

struct MonikerOptions : public WorkDoneProgressOptions
{
};
JSONIFY(MonikerOptions, workDoneProgress);

struct MonikerRegistrationOptions : public TextDocumentRegistrationOptions, public MonikerOptions
{
};
JSONIFY(MonikerRegistrationOptions, documentSelector, workDoneProgress);

struct MonikerParams : public TextDocumentPositionParams, public WorkDoneProgressParams, public PartialResultParams
{
};
JSONIFY(MonikerParams, textDocument, position, workDoneToken, partialResultToken);

enum class UniquenessLevel {
    /**
     * The moniker is only unique inside a document
     */
    Document,

    /**
     * The moniker is unique inside a project for which a dump got created
     */
    Project,

    /**
     * The moniker is unique inside the group to which a project belongs
     */
    Group,

    /**
     * The moniker is unique inside the moniker scheme.
     */
    Scheme,

    /**
     * The moniker is globally unique
     */
    Global,
};

enum class MonikerKind {
    /**
     * The moniker represent a symbol that is imported into a project
     */
    Import,

    /**
     * The moniker represents a symbol that is exported from a project
     */
    Export,

    /**
     * The moniker represents a symbol that is local to a project (e.g. a local
     * variable of a function, a class not visible outside the project, ...)
     */
    Local,
};

struct Moniker
{
    /**
     * The scheme of the moniker. For example tsc or .Net
     */
    std::string scheme;

    /**
     * The identifier of the moniker. The value is opaque in LSIF however
     * schema owners are allowed to define the structure if they want.
     */
    std::string identifier;

    /**
     * The scope in which the moniker is unique
     */
    UniquenessLevel unique;

    /**
     * The moniker kind if known.
     */
    std::optional<MonikerKind> kind;
};
JSONIFY(Moniker, scheme, identifier, unique, kind);

struct TextDocumentSyncClientCapabilities
{
    /**
     * Whether text document synchronization supports dynamic registration.
     */
    std::optional<bool> dynamicRegistration;

    /**
     * The client supports sending will save notifications.
     */
    std::optional<bool> willSave;

    /**
     * The client supports sending a will save request and
     * waits for a response providing text edits which will
     * be applied to the document before it is saved.
     */
    std::optional<bool> willSaveWaitUntil;

    /**
     * The client supports did save notifications.
     */
    std::optional<bool> didSave;
};
JSONIFY(TextDocumentSyncClientCapabilities, dynamicRegistration, willSave, willSaveWaitUntil, didSave);

struct PublishDiagnosticsClientCapabilities
{
    /**
     * Whether the clients accepts diagnostics with related information.
     */
    std::optional<bool> relatedInformation;

    /**
     * Client supports the tag property to provide meta data about a diagnostic.
     * Clients supporting tags have to handle unknown tags gracefully.
     *
     * @since 3.15.0
     */
    struct TagSupport
    {
        /**
         * The tags supported by the client.
         */
        std::vector<DiagnosticTag> valueSet;
    };
    std::optional<TagSupport> tagSupport;

    /**
     * Whether the client interprets the version property of the
     * `textDocument/publishDiagnostics` notification's parameter.
     *
     * @since 3.15.0
     */
    std::optional<bool> versionSupport;

    /**
     * Client supports a codeDescription property
     *
     * @since 3.16.0
     */
    std::optional<bool> codeDescriptionSupport;

    /**
     * Whether code action supports the `data` property which is
     * preserved between a `textDocument/publishDiagnostics` and
     * `textDocument/codeAction` request.
     *
     * @since 3.16.0
     */
    std::optional<bool> dataSupport;
};
JSONIFY(PublishDiagnosticsClientCapabilities, relatedInformation, tagSupport, versionSupport, codeDescriptionSupport,
        dataSupport);
JSONIFY(PublishDiagnosticsClientCapabilities::TagSupport, valueSet);

struct SignatureHelpClientCapabilities
{
    /**
     * Whether signature help supports dynamic registration.
     */
    std::optional<bool> dynamicRegistration;

    /**
     * The client supports the following `SignatureInformation`
     * specific properties.
     */
    struct SignatureInformation
    {
        /**
         * Client supports the following content formats for the documentation
         * property. The order describes the preferred format of the client.
         */
        std::optional<std::vector<MarkupKind>> documentationFormat;

        /**
         * Client capabilities specific to parameter information.
         */
        struct ParameterInformation
        {
            /**
             * The client supports processing label offsets instead of a
             * simple label string.
             *
             * @since 3.14.0
             */
            std::optional<bool> labelOffsetSupport;
        };
        std::optional<ParameterInformation> parameterInformation;

        /**
         * The client supports the `activeParameter` property on
         * `SignatureInformation` literal.
         *
         * @since 3.16.0
         */
        std::optional<bool> activeParameterSupport;
    };
    std::optional<SignatureInformation> signatureInformation;

    /**
     * The client supports to send additional context information for a
     * `textDocument/signatureHelp` request. A client that opts into
     * contextSupport will also support the `retriggerCharacters` on
     * `SignatureHelpOptions`.
     *
     * @since 3.15.0
     */
    std::optional<bool> contextSupport;
};
JSONIFY(SignatureHelpClientCapabilities, dynamicRegistration, signatureInformation, contextSupport);
JSONIFY(SignatureHelpClientCapabilities::SignatureInformation, documentationFormat, parameterInformation,
        activeParameterSupport);
JSONIFY(SignatureHelpClientCapabilities::SignatureInformation::ParameterInformation, labelOffsetSupport);

struct SignatureHelpRegistrationOptions : public TextDocumentRegistrationOptions, public SignatureHelpOptions
{
};
JSONIFY(SignatureHelpRegistrationOptions, documentSelector, triggerCharacters, retriggerCharacters, workDoneProgress);

struct CallHierarchyClientCapabilities
{
    /**
     * Whether implementation supports dynamic registration. If this is set to
     * `true` the client supports the new `(TextDocumentRegistrationOptions &
     * StaticRegistrationOptions)` return value for the corresponding server
     * capability as well.
     */
    std::optional<bool> dynamicRegistration;
};
JSONIFY(CallHierarchyClientCapabilities, dynamicRegistration);

struct CallHierarchyOptions : public WorkDoneProgressOptions
{
};
JSONIFY(CallHierarchyOptions, workDoneProgress);

struct CallHierarchyRegistrationOptions : public TextDocumentRegistrationOptions,
                                          public CallHierarchyOptions,
                                          public StaticRegistrationOptions
{
};
JSONIFY(CallHierarchyRegistrationOptions, documentSelector, workDoneProgress, id);

struct CallHierarchyPrepareParams : public TextDocumentPositionParams, public WorkDoneProgressParams
{
};
JSONIFY(CallHierarchyPrepareParams, textDocument, position, workDoneToken);

struct CallHierarchyItem
{
    /**
     * The name of this item.
     */
    std::string name;

    /**
     * The kind of this item.
     */
    SymbolKind kind;

    /**
     * Tags for this item.
     */
    std::optional<std::vector<SymbolTag>> tags;

    /**
     * More detail for this item, e.g. the signature of a function.
     */
    std::optional<std::string> detail;

    /**
     * The resource identifier of this item.
     */
    DocumentUri uri;

    /**
     * The range enclosing this symbol not including leading/trailing whitespace
     * but everything else, e.g. comments and code.
     */
    Range range;

    /**
     * The range that should be selected and revealed when this symbol is being
     * picked, e.g. the name of a function. Must be contained by the
     * [`range`](#CallHierarchyItem.range).
     */
    Range selectionRange;

    /**
     * A data entry field that is preserved between a call hierarchy prepare and
     * incoming calls or outgoing calls requests.
     */
    std::optional<nlohmann::json> data;
};
JSONIFY(CallHierarchyItem, name, kind, tags, detail, uri, range, selectionRange, data);

struct CallHierarchyIncomingCallsParams : public WorkDoneProgressParams, public PartialResultParams
{
    CallHierarchyItem item;
};
JSONIFY(CallHierarchyIncomingCallsParams, item, workDoneToken, partialResultToken);

struct CallHierarchyIncomingCall
{

    /**
     * The item that makes the call.
     */
    CallHierarchyItem from;

    /**
     * The ranges at which the calls appear. This is relative to the caller
     * denoted by [`this.from`](#CallHierarchyIncomingCall.from).
     */
    std::vector<Range> fromRanges;
};
JSONIFY(CallHierarchyIncomingCall, from, fromRanges);

struct CallHierarchyOutgoingCallsParams : public WorkDoneProgressParams, public PartialResultParams
{
    CallHierarchyItem item;
};
JSONIFY(CallHierarchyOutgoingCallsParams, item, workDoneToken, partialResultToken);

struct CallHierarchyOutgoingCall
{

    /**
     * The item that is called.
     */
    CallHierarchyItem to;

    /**
     * The range at which this item is called. This is the range relative to
     * the caller, e.g the item passed to `callHierarchy/outgoingCalls` request.
     */
    std::vector<Range> fromRanges;
};
JSONIFY(CallHierarchyOutgoingCall, to, fromRanges);

enum class SemanticTokenTypes {
    Namespace,
    /**
     * Represents a generic type. Acts as a fallback for types which
     * can't be mapped to a specific type like class or enum.
     */
    Type,
    Class,
    Enum,
    Interface,
    Struct,
    TypeParameter,
    Parameter,
    Variable,
    Property,
    EnumMember,
    Event,
    Function,
    Method,
    Macro,
    Keyword,
    Modifier,
    Comment,
    String,
    Number,
    Regexp,
    Operator,
};

enum class SemanticTokenModifiers {
    Declaration,
    Definition,
    Readonly,
    Static,
    Deprecated,
    Abstract,
    Async,
    Modification,
    Documentation,
    DefaultLibrary,
};

enum class TokenFormat {
    Relative,
};

using TokenFormatType = std::string;

struct SemanticTokensLegend
{
    /**
     * The token types a server uses.
     */
    std::vector<std::string> tokenTypes;

    /**
     * The token modifiers a server uses.
     */
    std::vector<std::string> tokenModifiers;
};
JSONIFY(SemanticTokensLegend, tokenTypes, tokenModifiers);

struct SemanticTokensClientCapabilities
{
    /**
     * Whether implementation supports dynamic registration. If this is set to
     * `true` the client supports the new `(TextDocumentRegistrationOptions &
     * StaticRegistrationOptions)` return value for the corresponding server
     * capability as well.
     */
    std::optional<bool> dynamicRegistration;

    /**
     * Which requests the client supports and might send to the server
     * depending on the server's capability. Please note that clients might not
     * show semantic tokens or degrade some of the user experience if a range
     * or full request is advertised by the client but not provided by the
     * server. If for example the client capability `requests.full` and
     * `request.range` are both set to true but the server only provides a
     * range provider the client might not render a minimap correctly or might
     * even decide to not show any semantic tokens at all.
     */
    struct Requests
    {
        /**
         * The client will send the `textDocument/semanticTokens/range` request
         * if the server provides a corresponding handler.
         */
        struct Range
        {
            std::optional<bool> ignore; // added as emtpy structs are not supported
        };
        std::optional<std::variant<bool, Range>> range;

        /**
         * The client will send the `textDocument/semanticTokens/full` request
         * if the server provides a corresponding handler.
         */
        struct Full
        {
            /**
             * The client will send the `textDocument/semanticTokens/full/delta`
             * request if the server provides a corresponding handler.
             */
            std::optional<bool> delta;
        };
        std::optional<std::variant<bool, Full>> full;
    };
    Requests requests;

    /**
     * The token types that the client supports.
     */
    std::vector<std::string> tokenTypes;

    /**
     * The token modifiers that the client supports.
     */
    std::vector<std::string> tokenModifiers;

    /**
     * The formats the clients supports.
     */
    std::vector<TokenFormat> formats;

    /**
     * Whether the client supports tokens that can overlap each other.
     */
    std::optional<bool> overlappingTokenSupport;

    /**
     * Whether the client supports tokens that can span multiple lines.
     */
    std::optional<bool> multilineTokenSupport;
};
JSONIFY(SemanticTokensClientCapabilities, dynamicRegistration, requests, tokenTypes, tokenModifiers, formats,
        overlappingTokenSupport, multilineTokenSupport);
JSONIFY(SemanticTokensClientCapabilities::Requests, range, full);
JSONIFY(SemanticTokensClientCapabilities::Requests::Range, ignore);
JSONIFY(SemanticTokensClientCapabilities::Requests::Full, delta);

struct SemanticTokensOptions : public WorkDoneProgressOptions
{
    /**
     * The legend used by the server
     */
    SemanticTokensLegend legend;

    /**
     * Server supports providing semantic tokens for a specific range
     * of a document.
     */
    struct Range
    {
        std::optional<bool> ignore; // not part of the spec, but empty struct is not supported
    };
    std::optional<std::variant<bool, Range>> range;

    /**
     * Server supports providing semantic tokens for a full document.
     */
    struct Full
    {
        /**
         * The server supports deltas for full documents.
         */
        std::optional<bool> delta;
    };
    std::optional<std::variant<bool, Full>> full;
};
JSONIFY(SemanticTokensOptions, legend, range, full);
JSONIFY(SemanticTokensOptions::Full, delta);
JSONIFY(SemanticTokensOptions::Range, ignore);

struct SemanticTokensRegistrationOptions : public TextDocumentRegistrationOptions,
                                           public SemanticTokensOptions,
                                           public StaticRegistrationOptions
{
};
JSONIFY(SemanticTokensRegistrationOptions, documentSelector, legend, range, full, id);

struct SemanticTokensParams : public WorkDoneProgressParams, public PartialResultParams
{
    /**
     * The text document.
     */
    TextDocumentIdentifier textDocument;
};
JSONIFY(SemanticTokensParams, textDocument, workDoneToken, partialResultToken);

struct SemanticTokens
{
    /**
     * An optional result id. If provided and clients support delta updating
     * the client will include the result id in the next semantic token request.
     * A server can then instead of computing all semantic tokens again simply
     * send a delta.
     */
    std::optional<std::string> resultId;

    /**
     * The actual tokens.
     */
    std::vector<unsigned int> data;
};
JSONIFY(SemanticTokens, resultId, data);

struct SemanticTokensPartialResult
{
    std::vector<unsigned int> data;
};
JSONIFY(SemanticTokensPartialResult, data);

struct SemanticTokensDeltaParams : public WorkDoneProgressParams, public PartialResultParams
{
    /**
     * The text document.
     */
    TextDocumentIdentifier textDocument;

    /**
     * The result id of a previous response. The result Id can either point to
     * a full response or a delta response depending on what was received last.
     */
    std::string previousResultId;
};
JSONIFY(SemanticTokensDeltaParams, textDocument, previousResultId, workDoneToken, partialResultToken);

struct SemanticTokensEdit
{
    /**
     * The start offset of the edit.
     */
    unsigned int start;

    /**
     * The count of elements to remove.
     */
    unsigned int deleteCount;

    /**
     * The elements to insert.
     */
    std::optional<std::vector<unsigned int>> data;
};
JSONIFY(SemanticTokensEdit, start, deleteCount, data);

struct SemanticTokensDelta
{
    std::optional<std::string> resultId;
    /**
     * The semantic token edits to transform a previous result into a new
     * result.
     */
    std::vector<SemanticTokensEdit> edits;
};
JSONIFY(SemanticTokensDelta, resultId, edits);

struct SemanticTokensDeltaPartialResult
{
    std::vector<SemanticTokensEdit> edits;
};
JSONIFY(SemanticTokensDeltaPartialResult, edits);

struct SemanticTokensRangeParams : public WorkDoneProgressParams, public PartialResultParams
{
    /**
     * The text document.
     */
    TextDocumentIdentifier textDocument;

    /**
     * The range the semantic tokens are requested for.
     */
    Range range;
};
JSONIFY(SemanticTokensRangeParams, textDocument, range, workDoneToken, partialResultToken);

struct WorkspaceSymbolOptions : public WorkDoneProgressOptions
{
};
JSONIFY(WorkspaceSymbolOptions, workDoneProgress);

struct WorkspaceSymbolRegistrationOptions : public WorkspaceSymbolOptions
{
};
JSONIFY(WorkspaceSymbolRegistrationOptions, workDoneProgress);

struct WorkspaceSymbolParams : public WorkDoneProgressParams, public PartialResultParams
{
    /**
     * A query string to filter symbols by. Clients may send an empty
     * string here to request all symbols.
     */
    std::string query;
};
JSONIFY(WorkspaceSymbolParams, query, workDoneToken, partialResultToken);

struct ExecuteCommandOptions : public WorkDoneProgressOptions
{
    /**
     * The commands to be executed on the server
     */
    std::vector<std::string> commands;
};
JSONIFY(ExecuteCommandOptions, commands, workDoneProgress);

struct ExecuteCommandRegistrationOptions : public ExecuteCommandOptions
{
};
JSONIFY(ExecuteCommandRegistrationOptions, commands, workDoneProgress);

struct ExecuteCommandParams : public WorkDoneProgressParams
{

    /**
     * The identifier of the actual command handler.
     */
    std::string command;
    /**
     * Arguments that the command should be invoked with.
     */
    std::optional<std::vector<nlohmann::json>> arguments;
};
JSONIFY(ExecuteCommandParams, command, arguments, workDoneToken);

struct WorkspaceFoldersServerCapabilities
{
    /**
     * The server has support for workspace folders
     */
    std::optional<bool> supported;

    /**
     * Whether the server wants to receive workspace folder
     * change notifications.
     *
     * If a string is provided, the string is treated as an ID
     * under which the notification is registered on the client
     * side. The ID can be used to unregister for these events
     * using the `client/unregisterCapability` request.
     */
    std::optional<std::variant<std::string, bool>> changeNotifications;
};
JSONIFY(WorkspaceFoldersServerCapabilities, supported, changeNotifications);

enum class FileOperationPatternKind {
    /**
     * The pattern matches a file only.
     */
    File,

    /**
     * The pattern matches a folder only.
     */
    Folder,
};

using FileOperationPatternKindType = std::string;

struct FileOperationPatternOptions
{

    /**
     * The pattern should be matched ignoring casing.
     */
    std::optional<bool> ignoreCase;
};
JSONIFY(FileOperationPatternOptions, ignoreCase);

struct FileOperationPattern
{
    /**
     * The glob pattern to match. Glob patterns can have the following syntax:
     * - `*` to match one or more characters in a path segment
     * - `?` to match on one character in a path segment
     * - `**` to match any number of path segments, including none
     * - `{}` to group sub patterns into an OR expression. (e.g. `*.{ts,js}`
     *   matches all TypeScript and JavaScript files)
     * - `[]` to declare a range of characters to match in a path segment
     *   (e.g., `example.[0-9]` to match on `example.0`, `example.1`, âŠ)
     * - `[!...]` to negate a range of characters to match in a path segment
     *   (e.g., `example.[!0-9]` to match on `example.a`, `example.b`, but
     *   not `example.0`)
     */
    std::string glob;

    /**
     * Whether to match files or folders with this pattern.
     *
     * Matches both if undefined.
     */
    std::optional<FileOperationPatternKind> matches;

    /**
     * Additional options used during matching.
     */
    std::optional<FileOperationPatternOptions> options;
};
JSONIFY(FileOperationPattern, glob, matches, options);

struct FileOperationFilter
{

    /**
     * A Uri like `file` or `untitled`.
     */
    std::optional<std::string> scheme;

    /**
     * The actual file operation pattern.
     */
    FileOperationPattern pattern;
};
JSONIFY(FileOperationFilter, scheme, pattern);

struct FileOperationRegistrationOptions
{
    /**
     * The actual filters.
     */
    std::vector<FileOperationFilter> filters;
};
JSONIFY(FileOperationRegistrationOptions, filters);

struct ServerCapabilities
{
    /**
     * Defines how text documents are synced. Is either a detailed structure
     * defining each notification or for backwards compatibility the
     * TextDocumentSyncKind number. If omitted it defaults to
     * `TextDocumentSyncKind.None`.
     */
    std::optional<std::variant<TextDocumentSyncOptions, TextDocumentSyncKind>> textDocumentSync;

    /**
     * The server provides completion support.
     */
    std::optional<CompletionOptions> completionProvider;

    /**
     * The server provides hover support.
     */
    std::optional<std::variant<bool, HoverOptions>> hoverProvider;

    /**
     * The server provides signature help support.
     */
    std::optional<SignatureHelpOptions> signatureHelpProvider;

    /**
     * The server provides go to declaration support.
     *
     * @since 3.14.0
     */
    std::optional<std::variant<bool, DeclarationOptions, DeclarationRegistrationOptions>> declarationProvider;

    /**
     * The server provides goto definition support.
     */
    std::optional<std::variant<bool, DefinitionOptions>> definitionProvider;

    /**
     * The server provides goto type definition support.
     *
     * @since 3.6.0
     */
    std::optional<std::variant<bool, TypeDefinitionOptions, TypeDefinitionRegistrationOptions>> typeDefinitionProvider;

    /**
     * The server provides goto implementation support.
     *
     * @since 3.6.0
     */
    std::optional<std::variant<bool, ImplementationOptions, ImplementationRegistrationOptions>> implementationProvider;

    /**
     * The server provides find references support.
     */
    std::optional<std::variant<bool, ReferenceOptions>> referencesProvider;

    /**
     * The server provides document highlight support.
     */
    std::optional<std::variant<bool, DocumentHighlightOptions>> documentHighlightProvider;

    /**
     * The server provides document symbol support.
     */
    std::optional<std::variant<bool, DocumentSymbolOptions>> documentSymbolProvider;

    /**
     * The server provides code actions. The `CodeActionOptions` return type is
     * only valid if the client signals code action literal support via the
     * property `textDocument.codeAction.codeActionLiteralSupport`.
     */
    std::optional<std::variant<bool, CodeActionOptions>> codeActionProvider;

    /**
     * The server provides code lens.
     */
    std::optional<CodeLensOptions> codeLensProvider;

    /**
     * The server provides document link support.
     */
    std::optional<DocumentLinkOptions> documentLinkProvider;

    /**
     * The server provides color provider support.
     *
     * @since 3.6.0
     */
    std::optional<std::variant<bool, DocumentColorOptions, DocumentColorRegistrationOptions>> colorProvider;

    /**
     * The server provides document formatting.
     */
    std::optional<std::variant<bool, DocumentFormattingOptions>> documentFormattingProvider;

    /**
     * The server provides document range formatting.
     */
    std::optional<std::variant<bool, DocumentRangeFormattingOptions>> documentRangeFormattingProvider;

    /**
     * The server provides document formatting on typing.
     */
    std::optional<DocumentOnTypeFormattingOptions> documentOnTypeFormattingProvider;

    /**
     * The server provides rename support. RenameOptions may only be
     * specified if the client states that it supports
     * `prepareSupport` in its initial `initialize` request.
     */
    std::optional<std::variant<bool, RenameOptions>> renameProvider;

    /**
     * The server provides folding provider support.
     *
     * @since 3.10.0
     */
    std::optional<std::variant<bool, FoldingRangeOptions, FoldingRangeRegistrationOptions>> foldingRangeProvider;

    /**
     * The server provides execute command support.
     */
    std::optional<ExecuteCommandOptions> executeCommandProvider;

    /**
     * The server provides selection range support.
     *
     * @since 3.15.0
     */
    std::optional<std::variant<bool, SelectionRangeOptions, SelectionRangeRegistrationOptions>> selectionRangeProvider;

    /**
     * The server provides linked editing range support.
     *
     * @since 3.16.0
     */
    std::optional<std::variant<bool, LinkedEditingRangeOptions, LinkedEditingRangeRegistrationOptions>>
        linkedEditingRangeProvider;

    /**
     * The server provides call hierarchy support.
     *
     * @since 3.16.0
     */
    std::optional<std::variant<bool, CallHierarchyOptions, CallHierarchyRegistrationOptions>> callHierarchyProvider;

    /**
     * The server provides semantic tokens support.
     *
     * @since 3.16.0
     */
    std::optional<std::variant<SemanticTokensOptions, SemanticTokensRegistrationOptions>> semanticTokensProvider;

    /**
     * Whether server provides moniker support.
     *
     * @since 3.16.0
     */
    std::optional<std::variant<bool, MonikerOptions, MonikerRegistrationOptions>> monikerProvider;

    /**
     * The server provides workspace symbol support.
     */
    std::optional<std::variant<bool, WorkspaceSymbolOptions>> workspaceSymbolProvider;

    /**
     * Workspace specific server capabilities
     */
    struct Workspace
    {
        /**
         * The server supports workspace folder.
         *
         * @since 3.6.0
         */
        std::optional<WorkspaceFoldersServerCapabilities> workspaceFolders;

        /**
         * The server is interested in file notifications/requests.
         *
         * @since 3.16.0
         */
        struct FileOperations
        {
            /**
             * The server is interested in receiving didCreateFiles
             * notifications.
             */
            std::optional<FileOperationRegistrationOptions> didCreate;

            /**
             * The server is interested in receiving willCreateFiles requests.
             */
            std::optional<FileOperationRegistrationOptions> willCreate;

            /**
             * The server is interested in receiving didRenameFiles
             * notifications.
             */
            std::optional<FileOperationRegistrationOptions> didRename;

            /**
             * The server is interested in receiving willRenameFiles requests.
             */
            std::optional<FileOperationRegistrationOptions> willRename;

            /**
             * The server is interested in receiving didDeleteFiles file
             * notifications.
             */
            std::optional<FileOperationRegistrationOptions> didDelete;

            /**
             * The server is interested in receiving willDeleteFiles file
             * requests.
             */
            std::optional<FileOperationRegistrationOptions> willDelete;
        };
        std::optional<FileOperations> fileOperations;
    };
    std::optional<Workspace> workspace;

    /**
     * Experimental server capabilities.
     */
    std::optional<nlohmann::json> experimental;
};
JSONIFY(ServerCapabilities, textDocumentSync, completionProvider, hoverProvider, signatureHelpProvider,
        declarationProvider, definitionProvider, typeDefinitionProvider, implementationProvider, referencesProvider,
        documentHighlightProvider, documentSymbolProvider, codeActionProvider, codeLensProvider, documentLinkProvider,
        colorProvider, documentFormattingProvider, documentRangeFormattingProvider, documentOnTypeFormattingProvider,
        renameProvider, foldingRangeProvider, executeCommandProvider, selectionRangeProvider,
        linkedEditingRangeProvider, callHierarchyProvider, semanticTokensProvider, monikerProvider,
        workspaceSymbolProvider, workspace, experimental);
JSONIFY(ServerCapabilities::Workspace, workspaceFolders, fileOperations);
JSONIFY(ServerCapabilities::Workspace::FileOperations, didCreate, willCreate, didRename, willRename, didDelete,
        willDelete);

struct InitializeResult
{
    /**
     * The capabilities the language server provides.
     */
    ServerCapabilities capabilities;

    /**
     * Information about the server.
     *
     * @since 3.15.0
     */
    struct ServerInfo
    {
        /**
         * The name of the server as defined by the server.
         */
        std::string name;

        /**
         * The server's version as defined by the server.
         */
        std::optional<std::string> version;
    };
    std::optional<ServerInfo> serverInfo;
};
JSONIFY(InitializeResult, capabilities, serverInfo);
JSONIFY(InitializeResult::ServerInfo, name, version);

enum class InitializeErrorCodes {
    /**
     * If the protocol version provided by the client can't be handled by the
     * server.
     *
     * @deprecated This initialize error got replaced by client capabilities.
     * There is no version handshake in version 3.0x
     */
    UnknownProtocolVersion = 1,
};

struct InitializeError
{
    /**
     * Indicates whether the client execute the following retry logic:
     * (1) show the message provided by the ResponseError to the user
     * (2) user selects retry or cancel
     * (3) if user selected retry the initialize method is sent again.
     */
    bool retry;
};
JSONIFY(InitializeError, retry);

struct InitializedParams
{
};
JSONIFY_EMPTY(InitializedParams);

struct LogTraceParams
{
    /**
     * The message to be logged.
     */
    std::string message;
    /**
     * Additional information that can be computed if the `trace` configuration
     * is set to `'verbose'`
     */
    std::optional<std::string> verbose;
};
JSONIFY(LogTraceParams, message, verbose);

struct SetTraceParams
{
    /**
     * The new value that should be assigned to the trace setting.
     */
    TraceValue value;
};
JSONIFY(SetTraceParams, value);

enum class MessageType {
    /**
     * An error message.
     */
    Error = 1,
    /**
     * A warning message.
     */
    Warning = 2,
    /**
     * An information message.
     */
    Info = 3,
    /**
     * A log message.
     */
    Log = 4,
};

using MessageTypeType = int;

struct ShowMessageParams
{
    /**
     * The message type. See {@link MessageType}.
     */
    MessageType type;

    /**
     * The actual message.
     */
    std::string message;
};
JSONIFY(ShowMessageParams, type, message);

struct MessageActionItem
{
    /**
     * A short title like 'Retry', 'Open Log' etc.
     */
    std::string title;
};
JSONIFY(MessageActionItem, title);

struct ShowMessageRequestParams
{
    /**
     * The message type. See {@link MessageType}
     */
    MessageType type;

    /**
     * The actual message
     */
    std::string message;

    /**
     * The message action items to present.
     */
    std::optional<std::vector<MessageActionItem>> actions;
};
JSONIFY(ShowMessageRequestParams, type, message, actions);

struct ShowDocumentParams
{
    /**
     * The document uri to show.
     */
    std::string uri;

    /**
     * Indicates to show the resource in an external program.
     * To show for example `https://code.visualstudio.com/`
     * in the default WEB browser set `external` to `true`.
     */
    std::optional<bool> external;

    /**
     * An optional property to indicate whether the editor
     * showing the document should take focus or not.
     * Clients might ignore this property if an external
     * program is started.
     */
    std::optional<bool> takeFocus;

    /**
     * An optional selection range if the document is a text
     * document. Clients might ignore the property if an
     * external program is started or the file is not a text
     * file.
     */
    std::optional<Range> selection;
};
JSONIFY(ShowDocumentParams, uri, external, takeFocus, selection);

struct ShowDocumentResult
{
    /**
     * A boolean indicating if the show was successful.
     */
    bool success;
};
JSONIFY(ShowDocumentResult, success);

struct LogMessageParams
{
    /**
     * The message type. See {@link MessageType}
     */
    MessageType type;

    /**
     * The actual message
     */
    std::string message;
};
JSONIFY(LogMessageParams, type, message);

struct WorkDoneProgressCreateParams
{
    /**
     * The token to be used to report progress.
     */
    ProgressToken token;
};
JSONIFY(WorkDoneProgressCreateParams, token);

struct WorkDoneProgressCancelParams
{
    /**
     * The token to be used to report progress.
     */
    ProgressToken token;
};
JSONIFY(WorkDoneProgressCancelParams, token);

struct Registration
{
    /**
     * The id used to register the request. The id can be used to deregister
     * the request again.
     */
    std::string id;

    /**
     * The method / capability to register for.
     */
    std::string method;

    /**
     * Options necessary for the registration.
     */
    std::optional<nlohmann::json> registerOptions;
};
JSONIFY(Registration, id, method, registerOptions);

struct RegistrationParams
{
    std::vector<Registration> registrations;
};
JSONIFY(RegistrationParams, registrations);

struct Unregistration
{
    /**
     * The id used to unregister the request or notification. Usually an id
     * provided during the register request.
     */
    std::string id;

    /**
     * The method / capability to unregister for.
     */
    std::string method;
};
JSONIFY(Unregistration, id, method);

struct UnregistrationParams
{
    // This should correctly be named `unregistrations`. However changing this
    // is a breaking change and needs to wait until we deliver a 4.x version
    // of the specification.
    std::vector<Unregistration> unregisterations;
};
JSONIFY(UnregistrationParams, unregisterations);

struct WorkspaceFoldersChangeEvent
{
    /**
     * The array of added workspace folders
     */
    std::vector<WorkspaceFolder> added;

    /**
     * The array of the removed workspace folders
     */
    std::vector<WorkspaceFolder> removed;
};
JSONIFY(WorkspaceFoldersChangeEvent, added, removed);

struct DidChangeWorkspaceFoldersParams
{
    /**
     * The actual workspace folder change event.
     */
    WorkspaceFoldersChangeEvent event;
};
JSONIFY(DidChangeWorkspaceFoldersParams, event);

struct DidChangeConfigurationParams
{
    /**
     * The actual changed settings
     */
    nlohmann::json settings;
};
JSONIFY(DidChangeConfigurationParams, settings);

struct ConfigurationItem
{
    /**
     * The scope to get the configuration section for.
     */
    std::optional<DocumentUri> scopeUri;

    /**
     * The configuration section asked for.
     */
    std::optional<std::string> section;
};
JSONIFY(ConfigurationItem, scopeUri, section);

struct ConfigurationParams
{
    std::vector<ConfigurationItem> items;
};
JSONIFY(ConfigurationParams, items);

struct FileSystemWatcher
{
    /**
     * The glob pattern to watch.
     *
     * Glob patterns can have the following syntax:
     * - `*` to match one or more characters in a path segment
     * - `?` to match on one character in a path segment
     * - `**` to match any number of path segments, including none
     * - `{}` to group sub patterns into an OR expression. (e.g. `*.{ts,js}`
     *   matches all TypeScript and JavaScript files)
     * - `[]` to declare a range of characters to match in a path segment
     *   (e.g., `example.[0-9]` to match on `example.0`, `example.1`, âŠ)
     * - `[!...]` to negate a range of characters to match in a path segment
     *   (e.g., `example.[!0-9]` to match on `example.a`, `example.b`, but not
     *   `example.0`)
     */
    std::string globPattern;

    /**
     * The kind of events of interest. If omitted it defaults
     * to WatchKind.Create | WatchKind.Change | WatchKind.Delete
     * which is 7.
     */
    std::optional<unsigned int> kind;
};
JSONIFY(FileSystemWatcher, globPattern, kind);

struct DidChangeWatchedFilesRegistrationOptions
{
    /**
     * The watchers to register.
     */
    std::vector<FileSystemWatcher> watchers;
};
JSONIFY(DidChangeWatchedFilesRegistrationOptions, watchers);

enum class WatchKind {
    /**
     * Interested in create events.
     */
    Create = 1,

    /**
     * Interested in change events
     */
    Change = 2,

    /**
     * Interested in delete events
     */
    Delete = 4,
};

struct FileEvent
{
    /**
     * The file's URI.
     */
    DocumentUri uri;
    /**
     * The change type.
     */
    unsigned int type;
};
JSONIFY(FileEvent, uri, type);

struct DidChangeWatchedFilesParams
{
    /**
     * The actual file events.
     */
    std::vector<FileEvent> changes;
};
JSONIFY(DidChangeWatchedFilesParams, changes);

enum class FileChangeType {
    /**
     * The file got created.
     */
    Created = 1,
    /**
     * The file got changed.
     */
    Changed = 2,
    /**
     * The file got deleted.
     */
    Deleted = 3,
};

struct ApplyWorkspaceEditParams
{
    /**
     * An optional label of the workspace edit. This label is
     * presented in the user interface for example on an undo
     * stack to undo the workspace edit.
     */
    std::optional<std::string> label;

    /**
     * The edits to apply.
     */
    WorkspaceEdit edit;
};
JSONIFY(ApplyWorkspaceEditParams, label, edit);

struct ApplyWorkspaceEditResponse
{
    /**
     * Indicates whether the edit was applied or not.
     */
    bool applied;

    /**
     * An optional textual description for why the edit was not applied.
     * This may be used by the server for diagnostic logging or to provide
     * a suitable error for a request that triggered the edit.
     */
    std::optional<std::string> failureReason;

    /**
     * Depending on the client's failure handling strategy `failedChange`
     * might contain the index of the change that failed. This property is
     * only available if the client signals a `failureHandling` strategy
     * in its client capabilities.
     */
    std::optional<unsigned int> failedChange;
};
JSONIFY(ApplyWorkspaceEditResponse, applied, failureReason, failedChange);

struct FileCreate
{

    /**
     * A file:// URI for the location of the file/folder being created.
     */
    std::string uri;
};
JSONIFY(FileCreate, uri);

struct CreateFilesParams
{

    /**
     * An array of all files/folders created in this operation.
     */
    std::vector<FileCreate> files;
};
JSONIFY(CreateFilesParams, files);

struct FileRename
{

    /**
     * A file:// URI for the original location of the file/folder being renamed.
     */
    std::string oldUri;

    /**
     * A file:// URI for the new location of the file/folder being renamed.
     */
    std::string newUri;
};
JSONIFY(FileRename, oldUri, newUri);

struct RenameFilesParams
{

    /**
     * An array of all files/folders renamed in this operation. When a folder
     * is renamed, only the folder will be included, and not its children.
     */
    std::vector<FileRename> files;
};
JSONIFY(RenameFilesParams, files);

struct FileDelete
{

    /**
     * A file:// URI for the location of the file/folder being deleted.
     */
    std::string uri;
};
JSONIFY(FileDelete, uri);

struct DeleteFilesParams
{

    /**
     * An array of all files/folders deleted in this operation.
     */
    std::vector<FileDelete> files;
};
JSONIFY(DeleteFilesParams, files);

struct DidOpenTextDocumentParams
{
    /**
     * The document that was opened.
     */
    TextDocumentItem textDocument;
};
JSONIFY(DidOpenTextDocumentParams, textDocument);

struct TextDocumentChangeRegistrationOptions : public TextDocumentRegistrationOptions
{
    /**
     * How documents are synced to the server. See TextDocumentSyncKind.Full
     * and TextDocumentSyncKind.Incremental.
     */
    TextDocumentSyncKind syncKind;
};
JSONIFY(TextDocumentChangeRegistrationOptions, syncKind, documentSelector);

/**
 * An event describing a change to a text document. If range and rangeLength are
 * omitted the new text is considered to be the full content of the document.
 */
struct TextDocumentContentChangeEvent1
{
    /**
     * The range of the document that changed.
     */
    Range range;

    /**
     * The optional length of the range that got replaced.
     *
     * @deprecated use range instead.
     */
    std::optional<unsigned int> rangeLength;

    /**
     * The new text for the provided range.
     */
    std::string text;
};
JSONIFY(TextDocumentContentChangeEvent1, range, rangeLength, text);

struct TextDocumentContentChangeEvent2
{
    /**
     * The new text of the whole document.
     */
    std::string text;
};
JSONIFY(TextDocumentContentChangeEvent2, text);

using TextDocumentContentChangeEvent = std::variant<TextDocumentContentChangeEvent1, TextDocumentContentChangeEvent2>;

struct DidChangeTextDocumentParams
{
    /**
     * The document that did change. The version number points
     * to the version after all provided content changes have
     * been applied.
     */
    VersionedTextDocumentIdentifier textDocument;

    /**
     * The actual content changes. The content changes describe single state
     * changes to the document. So if there are two content changes c1 (at
     * array index 0) and c2 (at array index 1) for a document in state S then
     * c1 moves the document from S to S' and c2 from S' to S''. So c1 is
     * computed on the state S and c2 is computed on the state S'.
     *
     * To mirror the content of a document using change events use the following
     * approach:
     * - start with the same initial content
     * - apply the 'textDocument/didChange' notifications in the order you
     *   receive them.
     * - apply the `TextDocumentContentChangeEvent`s in a single notification
     *   in the order you receive them.
     */
    std::vector<TextDocumentContentChangeEvent> contentChanges;
};
JSONIFY(DidChangeTextDocumentParams, textDocument, contentChanges);

enum class TextDocumentSaveReason {

    /**
     * Manually triggered, e.g. by the user pressing save, by starting
     * debugging, or by an API call.
     */
    Manual = 1,

    /**
     * Automatic after a delay.
     */
    AfterDelay = 2,

    /**
     * When the editor lost focus.
     */
    FocusOut = 3,
};

using TextDocumentSaveReasonType = int;

struct WillSaveTextDocumentParams
{
    /**
     * The document that will be saved.
     */
    TextDocumentIdentifier textDocument;

    /**
     * The 'TextDocumentSaveReason'.
     */
    TextDocumentSaveReason reason;
};
JSONIFY(WillSaveTextDocumentParams, textDocument, reason);

struct TextDocumentSaveRegistrationOptions : public TextDocumentRegistrationOptions
{
    /**
     * The client is supposed to include the content on save.
     */
    std::optional<bool> includeText;
};
JSONIFY(TextDocumentSaveRegistrationOptions, includeText, documentSelector);

struct DidSaveTextDocumentParams
{
    /**
     * The document that was saved.
     */
    TextDocumentIdentifier textDocument;

    /**
     * Optional the content when saved. Depends on the includeText value
     * when the save notification was requested.
     */
    std::optional<std::string> text;
};
JSONIFY(DidSaveTextDocumentParams, textDocument, text);

struct DidCloseTextDocumentParams
{
    /**
     * The document that was closed.
     */
    TextDocumentIdentifier textDocument;
};
JSONIFY(DidCloseTextDocumentParams, textDocument);

struct PublishDiagnosticsParams
{
    /**
     * The URI for which diagnostic information is reported.
     */
    DocumentUri uri;

    /**
     * Optional the version number of the document the diagnostics are published
     * for.
     *
     * @since 3.15.0
     */
    std::optional<unsigned int> version;

    /**
     * An array of diagnostic information items.
     */
    std::vector<Diagnostic> diagnostics;
};
JSONIFY(PublishDiagnosticsParams, uri, version, diagnostics);

enum class CompletionItemTag {
    /**
     * Render a completion as obsolete, usually using a strike-out.
     */
    Deprecated = 1,
};

using CompletionItemTagType = int;

enum class InsertTextMode {
    /**
     * The insertion or replace strings is taken as it is. If the
     * value is multi line the lines below the cursor will be
     * inserted using the indentation defined in the string value.
     * The client will not apply any kind of adjustments to the
     * string.
     */
    AsIs = 1,

    /**
     * The editor adjusts leading whitespace of new lines so that
     * they match the indentation up to the cursor of the line for
     * which the item is accepted.
     *
     * Consider a line like this: <2tabs><cursor><3tabs>foo. Accepting a
     * multi line completion item is indented using 2 tabs and all
     * following lines inserted will be indented using 2 tabs as well.
     */
    AdjustIndentation = 2,
};

using InsertTextModeType = int;

enum class InsertTextFormat {
    /**
     * The primary text to be inserted is treated as a plain string.
     */
    PlainText = 1,

    /**
     * The primary text to be inserted is treated as a snippet.
     *
     * A snippet can define tab stops and placeholders with `$1`, `$2`
     * and `${3:foo}`. `$0` defines the final tab stop, it defaults to
     * the end of the snippet. Placeholders with equal identifiers are linked,
     * that is typing in one will update others too.
     */
    Snippet = 2,
};

using InsertTextFormatType = int;

struct InsertReplaceEdit
{
    /**
     * The string to be inserted.
     */
    std::string newText;

    /**
     * The range if the insert is requested
     */
    Range insert;

    /**
     * The range if the replace is requested.
     */
    Range replace;
};
JSONIFY(InsertReplaceEdit, newText, insert, replace);

struct CompletionItem
{
    /**
     * The label of this completion item. By default
     * also the text that is inserted when selecting
     * this completion.
     */
    std::string label;

    /**
     * The kind of this completion item. Based of the kind
     * an icon is chosen by the editor. The standardized set
     * of available values is defined in `CompletionItemKind`.
     */
    std::optional<CompletionItemKind> kind;

    /**
     * Tags for this completion item.
     *
     * @since 3.15.0
     */
    std::optional<std::vector<CompletionItemTag>> tags;

    /**
     * A human-readable string with additional information
     * about this item, like type or symbol information.
     */
    std::optional<std::string> detail;

    /**
     * A human-readable string that represents a doc-comment.
     */
    std::optional<std::variant<std::string, MarkupContent>> documentation;

    /**
     * Indicates if this item is deprecated.
     *
     * @deprecated Use `tags` instead if supported.
     */
    std::optional<bool> deprecated;

    /**
     * Select this item when showing.
     *
     * *Note* that only one completion item can be selected and that the
     * tool / client decides which item that is. The rule is that the *first*
     * item of those that match best is selected.
     */
    std::optional<bool> preselect;

    /**
     * A string that should be used when comparing this item
     * with other items. When `falsy` the label is used
     * as the sort text for this item.
     */
    std::optional<std::string> sortText;

    /**
     * A string that should be used when filtering a set of
     * completion items. When `falsy` the label is used as the
     * filter text for this item.
     */
    std::optional<std::string> filterText;

    /**
     * A string that should be inserted into a document when selecting
     * this completion. When `falsy` the label is used as the insert text
     * for this item.
     *
     * The `insertText` is subject to interpretation by the client side.
     * Some tools might not take the string literally. For example
     * VS Code when code complete is requested in this example
     * `con<cursor position>` and a completion item with an `insertText` of
     * `console` is provided it will only insert `sole`. Therefore it is
     * recommended to use `textEdit` instead since it avoids additional client
     * side interpretation.
     */
    std::optional<std::string> insertText;

    /**
     * The format of the insert text. The format applies to both the
     * `insertText` property and the `newText` property of a provided
     * `textEdit`. If omitted defaults to `InsertTextFormat.PlainText`.
     */
    std::optional<InsertTextFormat> insertTextFormat;

    /**
     * How whitespace and indentation is handled during completion
     * item insertion. If not provided the client's default value is used.
     *
     * @since 3.16.0
     */
    std::optional<InsertTextMode> insertTextMode;

    /**
     * An edit which is applied to a document when selecting this completion.
     * When an edit is provided the value of `insertText` is ignored.
     *
     * *Note:* The range of the edit must be a single line range and it must
     * contain the position at which completion has been requested.
     *
     * Most editors support two different operations when accepting a completion
     * item. One is to insert a completion text and the other is to replace an
     * existing text with a completion text. Since this can usually not be
     * predetermined by a server it can report both ranges. Clients need to
     * signal support for `InsertReplaceEdit`s via the
     * `textDocument.completion.insertReplaceSupport` client capability
     * property.
     *
     * *Note 1:* The text edit's range as well as both ranges from an insert
     * replace edit must be a [single line] and they must contain the position
     * at which completion has been requested.
     * *Note 2:* If an `InsertReplaceEdit` is returned the edit's insert range
     * must be a prefix of the edit's replace range, that means it must be
     * contained and starting at the same position.
     *
     * @since 3.16.0 additional type `InsertReplaceEdit`
     */
    std::optional<std::variant<TextEdit, InsertReplaceEdit>> textEdit;

    /**
     * An optional array of additional text edits that are applied when
     * selecting this completion. Edits must not overlap (including the same
     * insert position) with the main edit nor with themselves.
     *
     * Additional text edits should be used to change text unrelated to the
     * current cursor position (for example adding an import statement at the
     * top of the file if the completion item will insert an unqualified type).
     */
    std::optional<std::vector<TextEdit>> additionalTextEdits;

    /**
     * An optional set of characters that when pressed while this completion is
     * active will accept it first and then type that character. *Note* that all
     * commit characters should have `length=1` and that superfluous characters
     * will be ignored.
     */
    std::optional<std::vector<std::string>> commitCharacters;

    /**
     * An optional command that is executed *after* inserting this completion.
     * *Note* that additional modifications to the current document should be
     * described with the additionalTextEdits-property.
     */
    std::optional<Command> command;

    /**
     * A data entry field that is preserved on a completion item between
     * a completion and a completion resolve request.
     */
    std::optional<nlohmann::json> data;
};
JSONIFY(CompletionItem, label, kind, tags, detail, documentation, deprecated, preselect, sortText, filterText,
        insertText, insertTextFormat, insertTextMode, textEdit, additionalTextEdits, commitCharacters, command, data);

struct CompletionClientCapabilities
{
    /**
     * Whether completion supports dynamic registration.
     */
    std::optional<bool> dynamicRegistration;

    /**
     * The client supports the following `CompletionItem` specific
     * capabilities.
     */
    struct CompletionItem
    {
        /**
         * Client supports snippets as insert text.
         *
         * A snippet can define tab stops and placeholders with `$1`, `$2`
         * and `${3:foo}`. `$0` defines the final tab stop, it defaults to
         * the end of the snippet. Placeholders with equal identifiers are
         * linked, that is typing in one will update others too.
         */
        std::optional<bool> snippetSupport;

        /**
         * Client supports commit characters on a completion item.
         */
        std::optional<bool> commitCharactersSupport;

        /**
         * Client supports the following content formats for the documentation
         * property. The order describes the preferred format of the client.
         */
        std::optional<std::vector<MarkupKind>> documentationFormat;

        /**
         * Client supports the deprecated property on a completion item.
         */
        std::optional<bool> deprecatedSupport;

        /**
         * Client supports the preselect property on a completion item.
         */
        std::optional<bool> preselectSupport;

        /**
         * Client supports the tag property on a completion item. Clients
         * supporting tags have to handle unknown tags gracefully. Clients
         * especially need to preserve unknown tags when sending a completion
         * item back to the server in a resolve call.
         *
         * @since 3.15.0
         */
        struct TagSupport
        {
            /**
             * The tags supported by the client.
             */
            std::vector<CompletionItemTag> valueSet;
        };
        std::optional<TagSupport> tagSupport;

        /**
         * Client supports insert replace edit to control different behavior if
         * a completion item is inserted in the text or should replace text.
         *
         * @since 3.16.0
         */
        std::optional<bool> insertReplaceSupport;

        /**
         * Indicates which properties a client can resolve lazily on a
         * completion item. Before version 3.16.0 only the predefined properties
         * `documentation` and `detail` could be resolved lazily.
         *
         * @since 3.16.0
         */
        struct ResolveSupport
        {
            /**
             * The properties that a client can resolve lazily.
             */
            std::vector<std::string> properties;
        };
        std::optional<ResolveSupport> resolveSupport;

        /**
         * The client supports the `insertTextMode` property on
         * a completion item to override the whitespace handling mode
         * as defined by the client.
         *
         * @since 3.16.0
         */
        struct InsertTextModeSupport
        {
            std::vector<InsertTextMode> valueSet;
        };
        std::optional<InsertTextModeSupport> insertTextModeSupport;
    };
    std::optional<CompletionItem> completionItem;

    struct CompletionItemKind
    {
        /**
         * The completion item kind values the client supports. When this
         * property exists the client also guarantees that it will
         * handle values outside its set gracefully and falls back
         * to a default value when unknown.
         *
         * If this property is not present the client only supports
         * the completion items kinds from `Text` to `Reference` as defined in
         * the initial version of the protocol.
         */
        std::optional<std::vector<CompletionItemKind>> valueSet;
    };
    std::optional<CompletionItemKind> completionItemKind;

    /**
     * The client supports to send additional context information for a
     * `textDocument/completion` request.
     */
    std::optional<bool> contextSupport;
};
JSONIFY(CompletionClientCapabilities, dynamicRegistration, completionItem, completionItemKind, contextSupport);
JSONIFY(CompletionClientCapabilities::CompletionItemKind, valueSet);
JSONIFY(CompletionClientCapabilities::CompletionItem, snippetSupport, commitCharactersSupport, documentationFormat,
        deprecatedSupport, preselectSupport, tagSupport, insertReplaceSupport, resolveSupport, insertTextModeSupport);
JSONIFY(CompletionClientCapabilities::CompletionItem::InsertTextModeSupport, valueSet);
JSONIFY(CompletionClientCapabilities::CompletionItem::ResolveSupport, properties);
JSONIFY(CompletionClientCapabilities::CompletionItem::TagSupport, valueSet);

struct CompletionRegistrationOptions : public TextDocumentRegistrationOptions, public CompletionOptions
{
};
JSONIFY(CompletionRegistrationOptions, documentSelector, triggerCharacters, allCommitCharacters, resolveProvider,
        workDoneProgress);

enum class CompletionTriggerKind {
    /**
     * Completion was triggered by typing an identifier (24x7 code
     * complete), manual invocation (e.g Ctrl+Space) or via API.
     */
    Invoked = 1,

    /**
     * Completion was triggered by a trigger character specified by
     * the `triggerCharacters` properties of the
     * `CompletionRegistrationOptions`.
     */
    TriggerCharacter = 2,

    /**
     * Completion was re-triggered as the current completion list is incomplete.
     */
    TriggerForIncompleteCompletions = 3,
};

using CompletionTriggerKindType = int;

struct CompletionContext
{
    /**
     * How the completion was triggered.
     */
    CompletionTriggerKind triggerKind;

    /**
     * The trigger character (a single character) that has trigger code
     * complete. Is undefined if
     * `triggerKind !== CompletionTriggerKind.TriggerCharacter`
     */
    std::optional<std::string> triggerCharacter;
};
JSONIFY(CompletionContext, triggerKind, triggerCharacter);

struct CompletionParams : public TextDocumentPositionParams, public WorkDoneProgressParams, public PartialResultParams
{
    /**
     * The completion context. This is only available if the client specifies
     * to send this using the client capability
     * `completion.contextSupport === true`
     */
    std::optional<CompletionContext> context;
};
JSONIFY(CompletionParams, context, textDocument, position, workDoneToken, partialResultToken);

struct CompletionList
{
    /**
     * This list is not complete. Further typing should result in recomputing
     * this list.
     */
    bool isIncomplete;

    /**
     * The completion items.
     */
    std::vector<CompletionItem> items;
};
JSONIFY(CompletionList, isIncomplete, items);

struct ParameterInformation
{

    /**
     * The label of this parameter information.
     *
     * Either a string or an inclusive start and exclusive end offsets within
     * its containing signature label. (see SignatureInformation.label). The
     * offsets are based on a UTF-16 string representation as `Position` and
     * `Range` does.
     *
     * *Note*: a label of type string should be a substring of its containing
     * signature label. Its intended use case is to highlight the parameter
     * label part in the `SignatureInformation.label`.
     */
    std::variant<std::string, std::unordered_map<unsigned int, unsigned int>> label;

    /**
     * The human-readable doc-comment of this parameter. Will be shown
     * in the UI but can be omitted.
     */
    std::optional<std::variant<std::string, MarkupContent>> documentation;
};
JSONIFY(ParameterInformation, label, documentation);

struct SignatureInformation
{
    /**
     * The label of this signature. Will be shown in
     * the UI.
     */
    std::string label;

    /**
     * The human-readable doc-comment of this signature. Will be shown
     * in the UI but can be omitted.
     */
    std::optional<std::variant<std::string, MarkupContent>> documentation;

    /**
     * The parameters of this signature.
     */
    std::optional<std::vector<ParameterInformation>> parameters;

    /**
     * The index of the active parameter.
     *
     * If provided, this is used in place of `SignatureHelp.activeParameter`.
     *
     * @since 3.16.0
     */
    std::optional<unsigned int> activeParameter;
};
JSONIFY(SignatureInformation, label, documentation, parameters, activeParameter);

struct SignatureHelp
{
    /**
     * One or more signatures. If no signatures are available the signature help
     * request should return `null`.
     */
    std::vector<SignatureInformation> signatures;

    /**
     * The active signature. If omitted or the value lies outside the
     * range of `signatures` the value defaults to zero or is ignored if
     * the `SignatureHelp` has no signatures.
     *
     * Whenever possible implementors should make an active decision about
     * the active signature and shouldn't rely on a default value.
     *
     * In future version of the protocol this property might become
     * mandatory to better express this.
     */
    std::optional<unsigned int> activeSignature;

    /**
     * The active parameter of the active signature. If omitted or the value
     * lies outside the range of `signatures[activeSignature].parameters`
     * defaults to 0 if the active signature has parameters. If
     * the active signature has no parameters it is ignored.
     * In future version of the protocol this property might become
     * mandatory to better express the active parameter if the
     * active signature does have any.
     */
    std::optional<unsigned int> activeParameter;
};
JSONIFY(SignatureHelp, signatures, activeSignature, activeParameter);

enum class SignatureHelpTriggerKind {
    /**
     * Signature help was invoked manually by the user or by a command.
     */
    Invoked = 1,
    /**
     * Signature help was triggered by a trigger character.
     */
    TriggerCharacter = 2,
    /**
     * Signature help was triggered by the cursor moving or by the document
     * content changing.
     */
    ContentChange = 3,
};

using SignatureHelpTriggerKindType = int;

struct SignatureHelpContext
{
    /**
     * Action that caused signature help to be triggered.
     */
    SignatureHelpTriggerKind triggerKind;

    /**
     * Character that caused signature help to be triggered.
     *
     * This is undefined when triggerKind !==
     * SignatureHelpTriggerKind.TriggerCharacter
     */
    std::optional<std::string> triggerCharacter;

    /**
     * `true` if signature help was already showing when it was triggered.
     *
     * Retriggers occur when the signature help is already active and can be
     * caused by actions such as typing a trigger character, a cursor move, or
     * document content changes.
     */
    bool isRetrigger;

    /**
     * The currently active `SignatureHelp`.
     *
     * The `activeSignatureHelp` has its `SignatureHelp.activeSignature` field
     * updated based on the user navigating through available signatures.
     */
    std::optional<SignatureHelp> activeSignatureHelp;
};
JSONIFY(SignatureHelpContext, triggerKind, triggerCharacter, isRetrigger, activeSignatureHelp);

struct SignatureHelpParams : public TextDocumentPositionParams, public WorkDoneProgressParams
{
    /**
     * The signature help context. This is only available if the client
     * specifies to send this using the client capability
     * `textDocument.signatureHelp.contextSupport === true`
     *
     * @since 3.15.0
     */
    std::optional<SignatureHelpContext> context;
};
JSONIFY(SignatureHelpParams, context, textDocument, position, workDoneToken);

struct SymbolInformation
{
    /**
     * The name of this symbol.
     */
    std::string name;

    /**
     * The kind of this symbol.
     */
    SymbolKind kind;

    /**
     * Tags for this symbol.
     *
     * @since 3.16.0
     */
    std::optional<std::vector<SymbolTag>> tags;

    /**
     * Indicates if this symbol is deprecated.
     *
     * @deprecated Use tags instead
     */
    std::optional<bool> deprecated;

    /**
     * The location of this symbol. The location's range is used by a tool
     * to reveal the location in the editor. If the symbol is selected in the
     * tool the range's start information is used to position the cursor. So
     * the range usually spans more then the actual symbol's name and does
     * normally include things like visibility modifiers.
     *
     * The range doesn't have to denote a node range in the sense of a abstract
     * syntax tree. It can therefore not be used to re-construct a hierarchy of
     * the symbols.
     */
    Location location;

    /**
     * The name of the symbol containing this symbol. This information is for
     * user interface purposes (e.g. to render a qualifier in the user interface
     * if necessary). It can't be used to re-infer a hierarchy for the document
     * symbols.
     */
    std::optional<std::string> containerName;
};
JSONIFY(SymbolInformation, name, kind, tags, deprecated, location, containerName);

struct TextDocumentClientCapabilities
{

    std::optional<TextDocumentSyncClientCapabilities> synchronization;

    /**
     * Capabilities specific to the `textDocument/completion` request.
     */
    std::optional<CompletionClientCapabilities> completion;

    /**
     * Capabilities specific to the `textDocument/hover` request.
     */
    std::optional<HoverClientCapabilities> hover;

    /**
     * Capabilities specific to the `textDocument/signatureHelp` request.
     */
    std::optional<SignatureHelpClientCapabilities> signatureHelp;

    /**
     * Capabilities specific to the `textDocument/declaration` request.
     *
     * @since 3.14.0
     */
    std::optional<DeclarationClientCapabilities> declaration;

    /**
     * Capabilities specific to the `textDocument/definition` request.
     */
    std::optional<DefinitionClientCapabilities> definition;

    /**
     * Capabilities specific to the `textDocument/typeDefinition` request.
     *
     * @since 3.6.0
     */
    std::optional<TypeDefinitionClientCapabilities> typeDefinition;

    /**
     * Capabilities specific to the `textDocument/implementation` request.
     *
     * @since 3.6.0
     */
    std::optional<ImplementationClientCapabilities> implementation;

    /**
     * Capabilities specific to the `textDocument/references` request.
     */
    std::optional<ReferenceClientCapabilities> references;

    /**
     * Capabilities specific to the `textDocument/documentHighlight` request.
     */
    std::optional<DocumentHighlightClientCapabilities> documentHighlight;

    /**
     * Capabilities specific to the `textDocument/documentSymbol` request.
     */
    std::optional<DocumentSymbolClientCapabilities> documentSymbol;

    /**
     * Capabilities specific to the `textDocument/codeAction` request.
     */
    std::optional<CodeActionClientCapabilities> codeAction;

    /**
     * Capabilities specific to the `textDocument/codeLens` request.
     */
    std::optional<CodeLensClientCapabilities> codeLens;

    /**
     * Capabilities specific to the `textDocument/documentLink` request.
     */
    std::optional<DocumentLinkClientCapabilities> documentLink;

    /**
     * Capabilities specific to the `textDocument/documentColor` and the
     * `textDocument/colorPresentation` request.
     *
     * @since 3.6.0
     */
    std::optional<DocumentColorClientCapabilities> colorProvider;

    /**
     * Capabilities specific to the `textDocument/formatting` request.
     */
    std::optional<DocumentFormattingClientCapabilities> formatting;

    /**
     * Capabilities specific to the `textDocument/rangeFormatting` request.
     */
    std::optional<DocumentRangeFormattingClientCapabilities> rangeFormatting;

    /** request.
     * Capabilities specific to the `textDocument/onTypeFormatting` request.
     */
    std::optional<DocumentOnTypeFormattingClientCapabilities> onTypeFormatting;

    /**
     * Capabilities specific to the `textDocument/rename` request.
     */
    std::optional<RenameClientCapabilities> rename;

    /**
     * Capabilities specific to the `textDocument/publishDiagnostics`
     * notification.
     */
    std::optional<PublishDiagnosticsClientCapabilities> publishDiagnostics;

    /**
     * Capabilities specific to the `textDocument/foldingRange` request.
     *
     * @since 3.10.0
     */
    std::optional<FoldingRangeClientCapabilities> foldingRange;

    /**
     * Capabilities specific to the `textDocument/selectionRange` request.
     *
     * @since 3.15.0
     */
    std::optional<SelectionRangeClientCapabilities> selectionRange;

    /**
     * Capabilities specific to the `textDocument/linkedEditingRange` request.
     *
     * @since 3.16.0
     */
    std::optional<LinkedEditingRangeClientCapabilities> linkedEditingRange;

    /**
     * Capabilities specific to the various call hierarchy requests.
     *
     * @since 3.16.0
     */
    std::optional<CallHierarchyClientCapabilities> callHierarchy;

    /**
     * Capabilities specific to the various semantic token requests.
     *
     * @since 3.16.0
     */
    std::optional<SemanticTokensClientCapabilities> semanticTokens;

    /**
     * Capabilities specific to the `textDocument/moniker` request.
     *
     * @since 3.16.0
     */
    std::optional<MonikerClientCapabilities> moniker;
};
JSONIFY(TextDocumentClientCapabilities, synchronization, completion, hover, signatureHelp, declaration, definition,
        typeDefinition, implementation, references, documentHighlight, documentSymbol, codeAction, codeLens,
        documentLink, colorProvider, formatting, rangeFormatting, onTypeFormatting, rename, publishDiagnostics,
        foldingRange, selectionRange, linkedEditingRange, callHierarchy, semanticTokens, moniker);

struct ClientCapabilities
{
    /**
     * Workspace specific client capabilities.
     */
    struct Workspace
    {
        /**
         * The client supports applying batch edits
         * to the workspace by supporting the request
         * 'workspace/applyEdit'
         */
        std::optional<bool> applyEdit;

        /**
         * Capabilities specific to `WorkspaceEdit`s
         */
        std::optional<WorkspaceEditClientCapabilities> workspaceEdit;

        /**
         * Capabilities specific to the `workspace/didChangeConfiguration`
         * notification.
         */
        std::optional<DidChangeConfigurationClientCapabilities> didChangeConfiguration;

        /**
         * Capabilities specific to the `workspace/didChangeWatchedFiles`
         * notification.
         */
        std::optional<DidChangeWatchedFilesClientCapabilities> didChangeWatchedFiles;

        /**
         * Capabilities specific to the `workspace/symbol` request.
         */
        std::optional<WorkspaceSymbolClientCapabilities> symbol;

        /**
         * Capabilities specific to the `workspace/executeCommand` request.
         */
        std::optional<ExecuteCommandClientCapabilities> executeCommand;

        /**
         * The client has support for workspace folders.
         *
         * @since 3.6.0
         */
        std::optional<bool> workspaceFolders;

        /**
         * The client supports `workspace/configuration` requests.
         *
         * @since 3.6.0
         */
        std::optional<bool> configuration;

        /**
         * Capabilities specific to the semantic token requests scoped to the
         * workspace.
         *
         * @since 3.16.0
         */
        std::optional<SemanticTokensWorkspaceClientCapabilities> semanticTokens;

        /**
         * Capabilities specific to the code lens requests scoped to the
         * workspace.
         *
         * @since 3.16.0
         */
        std::optional<CodeLensWorkspaceClientCapabilities> codeLens;

        /**
         * The client has support for file requests/notifications.
         *
         * @since 3.16.0
         */
        struct FileOperations
        {
            /**
             * Whether the client supports dynamic registration for file
             * requests/notifications.
             */
            std::optional<bool> dynamicRegistration;

            /**
             * The client has support for sending didCreateFiles notifications.
             */
            std::optional<bool> didCreate;

            /**
             * The client has support for sending willCreateFiles requests.
             */
            std::optional<bool> willCreate;

            /**
             * The client has support for sending didRenameFiles notifications.
             */
            std::optional<bool> didRename;

            /**
             * The client has support for sending willRenameFiles requests.
             */
            std::optional<bool> willRename;

            /**
             * The client has support for sending didDeleteFiles notifications.
             */
            std::optional<bool> didDelete;

            /**
             * The client has support for sending willDeleteFiles requests.
             */
            std::optional<bool> willDelete;
        };
        std::optional<FileOperations> fileOperations;
    };
    std::optional<Workspace> workspace;

    /**
     * Text document specific client capabilities.
     */
    std::optional<TextDocumentClientCapabilities> textDocument;

    /**
     * Window specific client capabilities.
     */
    struct Window
    {
        /**
         * Whether client supports handling progress notifications. If set
         * servers are allowed to report in `workDoneProgress` property in the
         * request specific server capabilities.
         *
         * @since 3.15.0
         */
        std::optional<bool> workDoneProgress;

        /**
         * Capabilities specific to the showMessage request
         *
         * @since 3.16.0
         */
        std::optional<ShowMessageRequestClientCapabilities> showMessage;

        /**
         * Client capabilities for the show document request.
         *
         * @since 3.16.0
         */
        std::optional<ShowDocumentClientCapabilities> showDocument;
    };
    std::optional<Window> window;

    /**
     * General client capabilities.
     *
     * @since 3.16.0
     */
    struct General
    {
        /**
         * Client capabilities specific to regular expressions.
         *
         * @since 3.16.0
         */
        std::optional<RegularExpressionsClientCapabilities> regularExpressions;

        /**
         * Client capabilities specific to the client's markdown parser.
         *
         * @since 3.16.0
         */
        std::optional<MarkdownClientCapabilities> markdown;
    };
    std::optional<General> general;

    /**
     * Experimental client capabilities.
     */
    std::optional<nlohmann::json> experimental;
};
JSONIFY(ClientCapabilities, workspace, textDocument, window, general, experimental);
JSONIFY(ClientCapabilities::General, regularExpressions, markdown);
JSONIFY(ClientCapabilities::Window, workDoneProgress, showMessage, showDocument);
JSONIFY(ClientCapabilities::Workspace, applyEdit, workspaceEdit, didChangeConfiguration, didChangeWatchedFiles, symbol,
        executeCommand, workspaceFolders, configuration, semanticTokens, codeLens, fileOperations);
JSONIFY(ClientCapabilities::Workspace::FileOperations, dynamicRegistration, didCreate, willCreate, didRename,
        willRename, didDelete, willDelete);

struct InitializeParams : public WorkDoneProgressParams
{
    /**
     * The process Id of the parent process that started the server. Is null if
     * the process has not been started by another process. If the parent
     * process is not alive then the server should exit (see exit notification)
     * its process.
     */
    std::variant<int, std::nullptr_t> processId;

    /**
     * Information about the client
     *
     * @since 3.15.0
     */
    struct ClientInfo
    {
        /**
         * The name of the client as defined by the client.
         */
        std::string name;

        /**
         * The client's version as defined by the client.
         */
        std::optional<std::string> version;
    };
    std::optional<ClientInfo> clientInfo;

    /**
     * The locale the client is currently showing the user interface
     * in. This must not necessarily be the locale of the operating
     * system.
     *
     * Uses IETF language tags as the value's syntax
     * (See https://en.wikipedia.org/wiki/IETF_language_tag)
     *
     * @since 3.16.0
     */
    std::optional<std::string> locale;

    /**
     * The rootPath of the workspace. Is null
     * if no folder is open.
     *
     * @deprecated in favour of `rootUri`.
     */
    std::optional<std::variant<std::string, std::nullptr_t>> rootPath;

    /**
     * The rootUri of the workspace. Is null if no
     * folder is open. If both `rootPath` and `rootUri` are set
     * `rootUri` wins.
     *
     * @deprecated in favour of `workspaceFolders`
     */
    std::variant<DocumentUri, std::nullptr_t> rootUri;

    /**
     * User provided initialization options.
     */
    std::optional<nlohmann::json> initializationOptions;

    /**
     * The capabilities provided by the client (editor or tool)
     */
    ClientCapabilities capabilities;

    /**
     * The initial trace setting. If omitted trace is disabled ('off').
     */
    std::optional<TraceValue> trace;

    /**
     * The workspace folders configured in the client when the server starts.
     * This property is only available if the client supports workspace folders.
     * It can be `null` if the client supports workspace folders but none are
     * configured.
     *
     * @since 3.6.0
     */
    std::optional<std::variant<std::vector<WorkspaceFolder>, std::nullptr_t>> workspaceFolders;
};
JSONIFY(InitializeParams, processId, clientInfo, locale, rootPath, rootUri, initializationOptions, capabilities, trace,
        workspaceFolders, workDoneToken);
JSONIFY(InitializeParams::ClientInfo, name, version);

struct Color
{

    /**
     * The red component of this color in the range [0-1].
     */
    float red;

    /**
     * The green component of this color in the range [0-1].
     */
    float green;

    /**
     * The blue component of this color in the range [0-1].
     */
    float blue;

    /**
     * The alpha component of this color in the range [0-1].
     */
    float alpha;
};
JSONIFY(Color, red, green, blue, alpha);

struct ColorInformation
{
    /**
     * The range in the document where this color appears.
     */
    Range range;

    /**
     * The actual color value for this color range.
     */
    Color color;
};
JSONIFY(ColorInformation, range, color);

struct ColorPresentationParams : public WorkDoneProgressParams, public PartialResultParams
{
    /**
     * The text document.
     */
    TextDocumentIdentifier textDocument;

    /**
     * The color information to request presentations for.
     */
    Color color;

    /**
     * The range where the color would be inserted. Serves as a context.
     */
    Range range;
};
JSONIFY(ColorPresentationParams, textDocument, color, range, workDoneToken, partialResultToken);

struct ColorPresentation
{
    /**
     * The label of this color presentation. It will be shown on the color
     * picker header. By default this is also the text that is inserted when
     * selecting this color presentation.
     */
    std::string label;
    /**
     * An [edit](#TextEdit) which is applied to a document when selecting
     * this presentation for the color. When `falsy` the
     * [label](#ColorPresentation.label) is used.
     */
    std::optional<TextEdit> textEdit;
    /**
     * An optional array of additional [text edits](#TextEdit) that are applied
     * when selecting this color presentation. Edits must not overlap with the
     * main [edit](#ColorPresentation.textEdit) nor with themselves.
     */
    std::optional<std::vector<TextEdit>> additionalTextEdits;
};
JSONIFY(ColorPresentation, label, textEdit, additionalTextEdits);

struct RenamePlaceholder
{
    Range range;
    std::string placeholder;
};
JSONIFY(RenamePlaceholder, range, placeholder);

struct RenameDefaultBehavior
{
    bool defaultBehavior;
};
JSONIFY(RenameDefaultBehavior, defaultBehavior);
}
