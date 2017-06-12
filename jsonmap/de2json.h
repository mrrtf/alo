#ifndef DE2JSON_H
#define DE2JSON_H

#include <iostream>
#include <vector>

std::vector<int> de2bp(const std::vector<int> deids);
std::vector<int> get_deids(); 
void all_de2json(std::ostream& out);

#endif

