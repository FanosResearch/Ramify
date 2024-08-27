#include "Config.h"
#include "GenReqAddr.h"
#include "GenReqType.h"
#include "GenAccess.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stdlib.h>
#include <functional>
#include <map>
#include <iostream>
#include <random>
#include <chrono>
#include <bitset>
#include <cmath>
#include <array>
#include <numeric>
#include <algorithm>
#include <tuple>

//using namespace std;
using namespace RAMbench;

int main(int argc, const char *argv[])
{
    Config config = Config(argc, argv);

    int num_requests = config.getNumReqs();

    //printf("%d\n", num_requests);

    Request *requests = new Request[num_requests];
    GenReqAddr(requests, config.getConfigAddr(), num_requests);
    GenReqType(requests, config.getConfigType(), num_requests);
    GenAccess(requests, config.getConfigAccess(), num_requests);
    delete[] requests;

    return 0;
}