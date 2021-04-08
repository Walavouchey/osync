#include <ID3H.hpp>

ID3H::ID3H(const std::string &filepath)
    :
    tag(filepath.c_str())
{ }

std::vector<ID3_Frame*> ID3H::Frames()
{
    std::vector<ID3_Frame*> frames;
    ID3_Tag::Iterator* iter = tag.CreateIterator();
    ID3_Frame* field = nullptr;
    while (nullptr != (field = iter->GetNext()))
        frames.push_back(field);
    delete iter;
    return frames;
}

void ID3H::Save()
{
    tag.Update();
}

std::string ID3H::convertISO_8859_1toUTF_8(const char *p, std::size_t size)
{
    std::string output;
    for (unsigned char c : std::string(p, size))
        if (c < 128) output += c;
        else output += 0xC2 + (c > 0xBF), output += (c & 0x3F) + 0x80;
    return output;
}

std::string ID3H::convertUTF_16BEtoUTF_8(const uint16_t *p, std::size_t size)
{
    std::string output;
    for (int i = 0; i < size / 2; i++)
    {
        if (p[i] <= 0x7F)
        {
            output += (char) p[i];
        }
        else if (p[i] <= 0x7FF)
        {
            output += 0xC0 | (p[i] >> 6);
            output += 0x80 | ((p[i] >> 0) & 0x3F);
        }
        else if (p[i] <= 0xFFFF)
        {
            output += 0xE0 | (p[i] >> 12);
            output += 0x80 | ((p[i] >> 6) & 0x3F);
            output += 0x80 | ((p[i] >> 0) & 0x3F);
        }
    }
    return output;
}

std::string ID3H::convertUTF_16toUTF_8(const uint16_t *p, std::size_t size)
{
    std::basic_string<uint16_t> output;
    output.reserve(size);
    for (int i = 0; i < size / 2; i++)
        output += p[i] >> 8 | p[i] << 8;
    return convertUTF_16BEtoUTF_8(output.data(), size);
}

std::string ID3H::GetFieldText(ID3_FrameID id) const
{
    return GetFieldText(tag.Find(id));
}

std::string ID3H::GetFieldText(ID3_Frame* frame) const
{
    std::string str;
    if (frame)
    {
        ID3_Field* field = frame->GetField(ID3FN_TEXT);
        if (field)
        {
            std::size_t size = field->Size();
            ID3_TextEnc encoding = field->GetEncoding();
            switch (encoding)
            {
            case ID3TE_ISO8859_1:
                std::cout << "ISO-8859-1: ", str = convertISO_8859_1toUTF_8(field->GetRawText(), size);
                break;
            case ID3TE_UTF16:
                std::cout << "UTF-16: ", str = convertUTF_16toUTF_8(field->GetRawUnicodeText(), size);
                break;
            case ID3TE_UTF16BE:
                std::cout << "UTF-16-BE: ", str = convertUTF_16BEtoUTF_8(field->GetRawUnicodeText(), size);
                break;
            case ID3TE_UTF8:
                std::cout << "UTF-8: ";
                break;
            case ID3TE_NUMENCODINGS:
                std::cout << "NUMENCODINGS: ";
                break;
            case ID3TE_NONE:
                std::cout << "NONE: ";
                break;
            }
        }
    }
    return str;
}
