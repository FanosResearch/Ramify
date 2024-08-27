#include "GenAccess.h"

//using namespace std;
using namespace RAMbench;

GenAccess::GenAccess(Request *request, const ConfigAccess config, const int num_requests)
{
    m_num_reqs = num_requests;
    //printf("%d\n", m_num_reqs);

    m_transaction_size = config.getTranSize();
    std::string access_mode = config.getAccessMode();
    m_read_format = config.getReadFormat();
    m_write_format = config.getWriteFormat();

    if (access_mode == "DRAM")
        WriteDRAM(request);
    else if (access_mode == "CPU")
        WriteCPU(request);
    else if (access_mode == "RUN")
        AccessMEM(request);
}

void GenAccess::WriteDRAM(Request *request)
{
    for (int i = 0; i < m_num_reqs; i++)
    {
        if (request[i].type == Request::Type::READ)
            //printf("GenReq[%10d] = 0x%05lx R\n", i, request[i].addr);
            printf("0x%011lx %s\n", request[i].addr * m_transaction_size, m_read_format.c_str());
        else if (request[i].type == Request::Type::WRITE)
            //printf("GenReq[%10d] = 0x%05lx W\n", i, request[i].addr);
            printf("0x%011lx %s\n", request[i].addr * m_transaction_size, m_write_format.c_str());
    }
}

void GenAccess::WriteCPU(Request *request)
{
    for (int i = 0; i < m_num_reqs; i++)
    {
        if (request[i].type == Request::Type::READ)
        {
            if (i != 0)
                printf("\n");
            printf("0 %08lx", request[i].addr * m_transaction_size);
        }
        else if (request[i].type == Request::Type::WRITE)
        {
            printf(" %08lx", request[i].addr * m_transaction_size);
        }
    }
    printf("\n");
}

void GenAccess::AccessMEM(Request *request)
{
    //printf("\n\n");
    std::random_device rd;
    char *dataArray = new char[m_num_reqs];
    /*
    for (int i = 0; i < _num_reqs; i++)
    {
        std::mt19937 rng(rd());
        std::uniform_int_distribution<int> uni(33, 126);
        int characterinput = uni(rng); // generates a ascii code of a visible charactdataArrayArray[i] = char(characterinput);
        dataArray[i] = characterinput;

        //if (debugMode == 2)
        //{
        std::cout << i << "\t" << dataArray[i] << "\n";
        //}
    }
*/
    char reader = 0;
    int timeElapsed = 0;
    //int lastType = 0;
    //int switchCounter = 0;

    for (int i = 0; i < m_num_reqs; i++)
    {
        if (request[i].type == Request::Type::READ)
        {
            auto startTime = std::chrono::high_resolution_clock::now();
            reader = dataArray[request[i].addr];
            auto endTime = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);
            timeElapsed += duration.count();
        }
        else
        {
            auto startTime = std::chrono::high_resolution_clock::now();
            dataArray[request[i].addr] = 'w';
            auto endTime = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);
            timeElapsed += duration.count();
        }
        /*if (lastType != request[i].type)
        {
            switchCounter++;
            lastType = request[i].type;
        }*/
    }
    std::cout << timeElapsed << " nanoseconds were taken for this test" << std::endl;
}