#ifndef JSON_H
#define JSON_H

#include <iostream>
#include <string>
#include <vector>

std::ostream& curlybrace(std::ostream& out, void (*f)(std::ostream& out));

std::string quote(const char* s);

void outputArray(std::ostream& out, const char* key, std::vector<int> v);

#endif

