#include "section.h"

Section::Section()
{
    int i = 0;
    ++i;
}

Section::~Section()
{
    int i = 0;
    ++i;
}

void Section::foo()
{
#ifdef KDAB_TEMPORARILY_REMOVED
    int i = 0;
    ++i;
#else // KDAB_TEMPORARILY_REMOVED
    qDebug("Section::foo is commented out");
    return;
#endif // KDAB_TEMPORARILY_REMOVED
}

int Section::bar()
{
    int i = 0;
    ++i;
    return i;
}

static std::string computeText()
{
    int i = 0;
    ++i;
    return "Hello World!";
}

Section *Section::create()
{
    int i = 0;
    ++i;
    computeText();
    return new Section;
}
