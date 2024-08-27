#ifndef __CONFIG_H
#define __CONFIG_H

#include <stdio.h>
#include <cstring>
#include <fstream>
#include <vector>
#include <map>
#include <iostream>
#include <cassert>
#include <cmath>

#include "ConfigAccess.h"
#include "ConfigType.h"
#include "ConfigAddr.h"
#include "INIReader.h"

namespace RAMbench
{
    class Config
    {
    public:
        Config(int argc, const char *argv[]);

        ConfigAddr getConfigAddr() { return m_config_addr; };
        ConfigType getConfigType() { return m_config_type; };
        ConfigAccess getConfigAccess() { return m_config_access; };

        //~Config();
        int getNumReqs() { return m_num_requests; };

    private:
        ConfigAddr m_config_addr;
        ConfigType m_config_type;
        ConfigAccess m_config_access;

        int m_num_requests;

        ///
        addr_pattern m_address_pattern;
        int m_address_width;

        type_pattern m_type_pattern;
        int m_switch_percentage;

        seg_pattern m_custom_channel;
        int m_set_channel;
        int m_period_channel;
        int m_eff_channels;
        int m_intrlve_pct_channel;

        seg_pattern m_custom_rank;
        int m_set_rank;
        int m_period_rank;
        int m_eff_ranks;
        int m_intrlve_pct_rank;

        seg_pattern m_custom_bankgroup;
        int m_set_bankgroup;
        int m_period_bankgroup;
        int m_eff_bankgroups;
        int m_intrlve_pct_bankgroup;

        seg_pattern m_custom_bank;
        int m_set_bank;
        int m_period_bank;
        int m_eff_banks;
        int m_intrlve_pct_bank;

        seg_pattern m_custom_row;
        int m_set_row;
        int m_period_row;
        int m_hit_percentage;

        seg_pattern m_custom_column;
        int m_set_column;
        int m_period_column;

        ///
        int m_channels;
        int m_ranks;
        int m_bankgroups;
        int m_banks;
        int m_rows;
        int m_columns;

        int m_channelMapStart;
        int m_channelMapEnd;
        int m_rankMapStart;
        int m_rankMapEnd;
        int m_bankgroupMapStart;
        int m_bankgroupMapEnd;
        int m_bankMapStart;
        int m_bankMapEnd;
        int m_columnMapStart;
        int m_columnMapEnd;
        int m_rowMapStart;
        int m_rowMapEnd;

        std::string m_flexible_addr;
        std::string m_address_mapping;

        int m_transaction_size;

        std::string m_access_mode;
        std::string m_addr_format;
        std::string m_read_format;
        std::string m_write_format;

        std::string m_protocol;

        void provideHelp(const char *argv);
        void initAppParams(const std::string &fname);
        void initSystemParams(const std::string &fname);
        void initDeviceParams(const std::string &fname);

        std::tuple<uint64_t, uint8_t> parseFlexibleAddress(std::string addr, char seg);

        void debugAdressWidth();
        void debugSegmentWidth(int msb, int lsb, int seg_num, std::string seg_name);
        void debugSetSegment(int set_value, int seg_num, std::string seg_name);

        //s
        INIReader *reader_c;
        int getInteger(const std::string &sec, const std::string &opt, int default_val) const;
    };

} /* namespace RAMBench */

#endif /* _CONFIG_H */
