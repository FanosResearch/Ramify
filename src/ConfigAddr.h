#ifndef __CONFIGADDR_H
#define __CONFIGADDR_H

#include <stdio.h>
#include <cstring>
#include <map>
#include "ConfigSeg.h"
//#include <cassert>

namespace RAMbench
{
    enum addr_pattern
    {
        sequential,
        random,
        seq_customized,
        rnd_customized,
        customized
    };

    class ConfigAddr
    {
    public:
        ConfigAddr(){};

        addr_pattern parseAddrPtrn(std::string pattern_str) const
        {
            std::map<std::string, addr_pattern> addr_pattern_pairs = {
                {"sequential", addr_pattern::sequential},
                {"seq_customized", addr_pattern::seq_customized},
                {"random", addr_pattern::random},
                {"rnd_customized", addr_pattern::rnd_customized},
                {"customized", addr_pattern::customized}};

            if (addr_pattern_pairs.find(pattern_str) == addr_pattern_pairs.end())
            {
                std::cout << "Unkwown/Unsupported Address Pattern: " << pattern_str
                          << " Aborting!" << std::endl;
                //AbruptExit(__FILE__, __LINE__);
            }

            return addr_pattern_pairs[pattern_str];
        }

        void setAddrPtrn(addr_pattern pattern) { m_addr_pattern = pattern; };
        addr_pattern getAddrPtrn() const { return m_addr_pattern; };

        void setAddrWidth(int width) { m_addr_width = width; };
        int getAddrWidth() const { return m_addr_width; };
     
        ConfigSeg m_config_co, m_config_ro, m_config_bk, m_config_bg, m_config_rk, m_config_ch;

    private:
        addr_pattern m_addr_pattern;
        int m_addr_width;
    };

};

#endif /* _CONFIGADDR_H */
