#ifndef CH_H
#define CH_H

#include <vector>
class AliMpDDLStore;

std::vector<std::vector<int>> ch_read_mapping(AliMpDDLStore* ddlStore);

#endif
