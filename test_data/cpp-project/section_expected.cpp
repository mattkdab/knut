#include "section.h"

Section::Section()
{
#ifdef KDAB_TEMPORARILY_REMOVED
    int i = 0;
    ++i;
#else // KDAB_TEMPORARILY_REMOVED
    qDebug("Section::Section is commented out");
    return;
#endif // KDAB_TEMPORARILY_REMOVED
}

Section::~Section()
{
#ifdef KDAB_TEMPORARILY_REMOVED
    int i = 0;
    ++i;
#else // KDAB_TEMPORARILY_REMOVED
    qDebug("Section::~Section is commented out");
    return;
#endif // KDAB_TEMPORARILY_REMOVED
}

void Section::foo()
{
    int i = 0;
    ++i;
}

int Section::bar()
{
#ifdef KDAB_TEMPORARILY_REMOVED
    int i = 0;
    ++i;
    return i;
#else // KDAB_TEMPORARILY_REMOVED
    qDebug("Section::bar is commented out");
    return -1;
#endif // KDAB_TEMPORARILY_REMOVED
}

static std::string computeText()
{
#ifdef KDAB_TEMPORARILY_REMOVED
    int i = 0;
    ++i;
    return "Hello World!";
#else // KDAB_TEMPORARILY_REMOVED
    qDebug("computeText is commented out");
    return {};
#endif // KDAB_TEMPORARILY_REMOVED
}

Section *Section::create()
{
#ifdef KDAB_TEMPORARILY_REMOVED
    int i = 0;
    ++i;
    computeText();
    return new Section;
#else // KDAB_TEMPORARILY_REMOVED
    qDebug("Section::create is commented out");
    return nullptr;
#endif // KDAB_TEMPORARILY_REMOVED
}
