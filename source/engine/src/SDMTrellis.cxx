#include "engine/inc/SDMTrellis.h"

//-------------------------------------------------------------------------------------------
namespace omega
{
namespace engine
{
//-------------------------------------------------------------------------------------------

const SDMTrellisFilterBase *getTrellisBaseFilter(int dsdRate, bool isClans)
{
    const SDMTrellisFilterBase *bFilter = nullptr;

    for(int idx = 0; idx < SDM_TRELLIS_NO_OF_FILTERS && bFilter == nullptr; idx++)
    {
        if(dsdRate == c_sdmTrellisFilters[idx].rate)
        {
            if(isClans && strncmp("clans", c_sdmTrellisFilters[idx].name, 5) == 0)
            {
                bFilter = &c_sdmTrellisFilters[idx];
            }
            else if(!isClans && strncmp("sdm", c_sdmTrellisFilters[idx].name, 3) == 0)
            {
                bFilter = &c_sdmTrellisFilters[idx];
            }
        }
    }
    return bFilter;
}

//-------------------------------------------------------------------------------------------
} // namespace engine
} // namespace omega
//-------------------------------------------------------------------------------------------
