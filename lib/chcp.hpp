#pragma once

// class that temporarily makes utf-8 work on windows consoles (that support it)
// taken from https://stackoverflow.com/questions/23471873/change-console-code-page-in-windows-c#23472894
class UTF8CodePage
{
public:
    UTF8CodePage();
    ~UTF8CodePage();
private:
    unsigned int oldCodePage;
};
