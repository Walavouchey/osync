#include <cfg.hpp>
#include <chcp.hpp>
#include <exc.hpp>
#include <export.hpp>
#include <ID3H.hpp>
#include <lazerdb.hpp>
#include <Parser.hpp>
#include <progressbar.hpp>
#include <stabledb.hpp>

#include <exception>
#include <filesystem>
#include <iostream>
#include <string>
#include <unordered_set>


int main(int argc, char **argv)
{
    try
    {
        UTF8CodePage use_utf8;
        std::filesystem::path filePath;
        if (argc > 1) filePath = std::filesystem::path(argv[1]);
        else throw E("Must specify a file");

        ID3H handle(filePath.string());
        for (ID3_Frame *frame : handle.Frames())
            std::cout << (frame->GetDescription() ? : "") << ": " << handle.GetFieldText(frame) << std::endl;
    }
    catch (exc e)
    {
        std::cerr << e.file() << ", line " << e.line() << ": " << e.what() << std::endl;
        return 1;
    }
    catch (std::exception e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    catch (...)
    {
        std::cerr << "An unknown error has occured." << std::endl;
        return 1;
    }
    return 0;
}
