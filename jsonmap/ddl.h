#ifndef DDL_H
#define DDL_H

#include <vector>
class AliMpDDLStore;
class AliMpDDL;

void ddl_read_mapping(AliMpDDLStore* ddlStore,
        std::vector<AliMpDDL*>& ddls,
        std::vector<std::vector<int>>& desperdll);

#endif
