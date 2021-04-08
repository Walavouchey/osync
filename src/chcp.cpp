#include <chcp.hpp>

// class that temporarily makes utf-8 work on windows consoles (that support it)
// taken from https://stackoverflow.com/questions/23471873/change-console-code-page-in-windows-c#23472894
#ifdef _WIN32
#include <windows.h>
UTF8CodePage::UTF8CodePage()
    :
    oldCodePage(GetConsoleOutputCP())
{
    SetConsoleOutputCP(CP_UTF8);
}

UTF8CodePage::~UTF8CodePage()
{
    SetConsoleOutputCP(oldCodePage);
}
#else
UTF8CodePage::UTF8CodePage() { } 
UTF8CodePage::~UTF8CodePage() { }
#endif
