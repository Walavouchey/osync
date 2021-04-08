#pragma once

#include <id3/tag.h>

#include <cstddef>
#include <string>
#include <vector>

class ID3H
{
public:
    ID3H(const std::string &filepath);
    std::string GetFieldText(ID3_FrameID id) const;
    std::string GetFieldText(ID3_Frame* frame) const;
    std::vector<ID3_Frame*> Frames();
    void Save();
private:
    static std::string convertISO_8859_1toUTF_8(const char *p, std::size_t size);
    static std::string convertUTF_16toUTF_8(const uint16_t *p, std::size_t size);
    static std::string convertUTF_16BEtoUTF_8(const uint16_t *p, std::size_t size);
    ID3_Tag tag;
};
