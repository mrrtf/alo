#ifndef PCB2JSON_H
#define PCB2JSON_H

#include <iostream>
#include <string>
#include <vector>

class AliMpPCB;
class AliMpDataStreams;

std::vector<AliMpPCB*> get_allpcbs(AliMpDataStreams& dataStreams);

void all_pcb2json(std::ostream& out);

#endif

