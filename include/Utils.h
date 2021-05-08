#pragma once

#include <list>
#include <string>
#include <vector>

void Split(std::string str, std::string splitBy, std::vector<std::string>& tokens);

int Sum(const std::list<int>& l);
unsigned int Sum(const std::list<unsigned int>& l);