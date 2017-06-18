#include "ch.h"
#include "de.h"

std::vector<std::vector<int>> get_desperch(AliMpDDLStore* ddlStore)
{
    std::vector<std::vector<int>> desperch;

    for ( int ich = 0; ich < 10; ++ich )
    {
        std::vector<int> deids = get_deids(ddlStore,ich);

        desperch.push_back(deids);
    }

    return desperch;
}

