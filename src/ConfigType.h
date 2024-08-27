#ifndef __CONFIGTYPE_H
#define __CONFIGTYPE_H

#include <stdio.h>
#include <cstring>
#include <map>

namespace RAMbench
{

    enum type_pattern
    {
        all_read,
        all_write,
        rw_random,
        rw_switch_pct
    };

    class ConfigType
    {
    public:
        ConfigType(){};

        type_pattern parseTypePtrn(std::string pattern_str) const
        {
            std::map<std::string, type_pattern> type_pattern_pairs = {
                {"all_read", type_pattern::all_read},
                {"all_write", type_pattern::all_write},
                {"rw_random", type_pattern::rw_random},
                {"rw_switch_pct", type_pattern::rw_switch_pct}};

            if (type_pattern_pairs.find(pattern_str) == type_pattern_pairs.end())
            {
                std::cout << "Unkwown/Unsupported Type Pattern: " << pattern_str
                          << " Aborting!" << std::endl;
                //AbruptExit(__FILE__, __LINE__);
            }

            return type_pattern_pairs[pattern_str];
        }

        void setTypePtrn(type_pattern pattern) { m_type_pattern = pattern; };
        type_pattern getTypePtrn() const { return m_type_pattern; };

        void setTypeSwitchPct(int switch_percentage) { m_switch_percentage = switch_percentage; };
        int getTypeSwitchPct() const { return m_switch_percentage; };

    private:
        type_pattern m_type_pattern;
        int m_switch_percentage;
    };

};

#endif /* _CONFIGTYPE_H */
