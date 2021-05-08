#include "Utils.h"

// from https://stackoverflow.com/questions/10058606/splitting-a-string-by-a-character/10058756
void Split(std::string str, std::string splitBy, std::vector<std::string>& tokens)
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

int Sum(const std::list<int>& l)
{
	int s = 0;
	for (int v : l)
		s += v;
	return s;
}

unsigned int Sum(const std::list<unsigned int>& l)
{
	unsigned int s = 0;
	for (unsigned int v : l)
		s += v;
	return s;
}