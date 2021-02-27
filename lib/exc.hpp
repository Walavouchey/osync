#include <string>
#include <exception>
#include <filesystem>

#define E(what) exc(what, std::filesystem::path(__FILE__).filename().string(), __LINE__)

#define EX(expr, expected, err) if (expr != expected) throw E(err)

class exc : public std::exception
{
public:
    exc(std::string what, std::string file, size_t line)
        :
        _what(what),
        _file(file),
        _line(line)
    {}
    std::string what()
    {
        return _what;
    }
    std::string file()
    {
        return _file;
    }
    size_t line()
    {
        return _line;
    }
private:
    std::string _what;
    std::string _file;
    size_t _line;
};
