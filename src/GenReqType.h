#ifndef __GEN_REQ_Type_H
#define __GEN_REQ_Type_H

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

#include "ConfigType.h"
#include "Config.h"
#include "Request.h"

namespace RAMbench
{

    class GenReqType
    {
    public:
        GenReqType(Request *request, const ConfigType config, const int num_requests);
        //~GenReq();

    private:
        int m_num_reqs;
        int m_switch_percentage;

        void readAccess(Request *request);
        void writeAccess(Request *request);
        void randomAccess(Request *request);
        void customReadWrite(Request *request);
        std::tuple<int, int> reducedPeriod(int numerator, int denominator);
    };

} /* namespace RAMBench */

#endif /* _GEN_REQ_Type_H */
