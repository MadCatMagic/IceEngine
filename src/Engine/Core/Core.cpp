#include "Engine/Core.h"

// from https://stackoverflow.com/questions/10058606/splitting-a-string-by-a-character/10058756
void StrSplit(std::string str, std::string splitBy, std::vector<std::string>& tokens)
{
    tokens.push_back(str);

    size_t splitAt;
    size_t splitLen = splitBy.size();
    std::string frag;

    while (true)
    {
        frag = tokens.back();
        splitAt = frag.find(splitBy);
        if (splitAt == std::string::npos)
            break;

        tokens.back() = frag.substr(0, splitAt);
        tokens.push_back(frag.substr(splitAt + splitLen, frag.size() - (splitAt + splitLen)));
    }
}