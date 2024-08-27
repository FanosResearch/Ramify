#ifndef __GEN_REQ_Addr_H
#define __GEN_REQ_Addr_H

#include <iostream>
#include <cstdlib>
#include <random>
#include <chrono>
#include <cstring>
#include <bitset>
#include <functional>
#include <cmath>
#include <array>
#include <numeric>
#include <algorithm>
#include <tuple>

#include "ConfigAddr.h"
#include "Config.h"
#include "Request.h"
#include "Segment.h"

namespace RAMbench
{

    class GenReqAddr
    {
    public:
        GenReqAddr(Request *request, const ConfigAddr config, const int num_requests);
        //~GenReqAddr();

    private:
        int m_num_reqs;
        int m_addr_width;

        void sequentialIndexArray(Request *request);
        void randomIndexArray(Request *request);
        void customIndexArray(Request *request, const ConfigAddr config);
    };

} /* namespace RAMBench */

#endif /* _GEN_REQ_Addr_H */
