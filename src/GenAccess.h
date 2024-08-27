#ifndef __GEN_ACCESS_H
#define __GEN_ACCESS_H

#include <stdio.h>
#include <cstring>
#include <fstream>
#include <vector>
#include <random>
#include <map>
#include <iostream>
#include <cassert>
#include <chrono>

#include "ConfigAccess.h"
#include "Request.h"

namespace RAMbench
{
    class GenAccess
    {
    public:
        GenAccess(Request *request, const ConfigAccess config, const int num_requests);

    private:
        int m_num_reqs;
        int m_transaction_size;
        std::string m_read_format;
        std::string m_write_format;

        void WriteDRAM(Request *request);
        void WriteCPU(Request *request);
        void AccessMEM(Request *request);
    };

    //extern int num_requests;

} /* namespace RAMBench */

#endif /* _GEN_ACCESS_H */