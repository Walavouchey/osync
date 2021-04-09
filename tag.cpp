#include <tag.h>
#include <fileref.h>
#include <tpropertymap.h>

#include <chcp.hpp>
#include <exc.hpp>
#include <strext.hpp>

#include <exception>
#include <filesystem>
#include <iostream>
#include <string>


int main(int argc, char **argv)
{
    try
    {
        UTF8CodePage use_utf8;
        std::filesystem::path filePath;
        if (argc > 1) filePath = std::filesystem::path(argv[1]);
        else throw std::exception("Must specify a file");

        TagLib::FileRef handle(filePath.string().c_str());
        if (!handle.isNull() && handle.tag())
        {
            for(auto tag : handle.file()->properties())
                for(auto s : tag.second)
                    std::cout << tag.first << ": " << s.to8Bit(true) << std::endl;
        }
        if (!handle.isNull() && handle.audioProperties())
        {
            TagLib::AudioProperties *properties = handle.audioProperties();

            std::cout << "BITRATE: " << properties->bitrate() << std::endl;
            std::cout << "SAMPLE RATE: " << properties->sampleRate() << std::endl;
            std::cout << "CHANNELS: " << properties->channels() << std::endl;
            std::cout << "LENGTH: " << strext::formatTime(properties->length()) << std::endl;
        }
        if (!handle.isNull() && !handle.tag() && !handle.audioProperties()) throw std::exception("There is nothing to display");
        if (handle.isNull()) throw std::exception("File couldn't be opened");
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
