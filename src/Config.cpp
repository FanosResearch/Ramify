#include "Config.h"

//using namespace std;
using namespace RAMbench;

Config::Config(int argc, const char *argv[])
{
    if (argc < 2)
    {
        provideHelp(argv[0]);
        assert(false);
    }

    for (int i = 1; i < argc; i++)
    {
        if ((strcmp(argv[i], "--help") == 0) || (strcmp(argv[i], "-h") == 0))
            provideHelp(argv[0]);
        else if (strcmp(argv[i], "--app") == 0)
            initAppParams(argv[i + 1]);
        else if (strcmp(argv[i], "--system") == 0)
            initSystemParams(argv[i + 1]);
        else if (strcmp(argv[i], "--device") == 0)
            initDeviceParams(argv[i + 1]);
    }

    debugAdressWidth();

    debugSegmentWidth(m_columnMapStart, m_columnMapEnd, m_columns, "columns");
    debugSegmentWidth(m_rowMapStart, m_rowMapEnd, m_rows, "rows");
    debugSegmentWidth(m_bankMapStart, m_bankMapEnd, m_banks, "banks");
    debugSegmentWidth(m_bankgroupMapStart, m_bankgroupMapEnd, m_bankgroups, "bankgroups");
    debugSegmentWidth(m_rankMapStart, m_rankMapEnd, m_ranks, "ranks");
    debugSegmentWidth(m_channelMapStart, m_channelMapEnd, m_channels, "channels");

    debugSetSegment(m_set_column, m_columns, "columns");
    debugSetSegment(m_set_row, m_rows, "rows");
    debugSetSegment(m_set_bank, m_banks, "banks");
    debugSetSegment(m_set_bankgroup, m_bankgroups, "bankgroups");
    debugSetSegment(m_set_rank, m_ranks, "ranks");
    debugSetSegment(m_set_channel, m_channels, "channels");

    m_config_addr.setAddrPtrn(m_address_pattern);
    m_config_addr.setAddrWidth(m_address_width);

    m_config_addr.m_config_co.setSegParam(m_custom_column, m_set_column, m_columns, m_period_column, m_columnMapStart, m_columnMapEnd);
    m_config_addr.m_config_ro.setRoParam(m_custom_row, m_set_row, m_rows, m_period_row, m_rowMapStart, m_rowMapEnd, m_hit_percentage);
    m_config_addr.m_config_bk.setInterleaveParam(m_custom_bank, m_set_bank, m_banks, m_period_bank, m_bankMapStart, m_bankMapEnd, m_eff_banks, m_intrlve_pct_bank);
    m_config_addr.m_config_bg.setInterleaveParam(m_custom_bankgroup, m_set_bankgroup, m_bankgroups, m_period_bankgroup, m_bankgroupMapStart, m_bankgroupMapEnd,
                                                 m_eff_bankgroups, m_intrlve_pct_bankgroup);
    m_config_addr.m_config_rk.setInterleaveParam(m_custom_rank, m_set_rank, m_ranks, m_period_rank, m_rankMapStart, m_rankMapEnd, m_eff_ranks, m_intrlve_pct_rank);
    m_config_addr.m_config_ch.setInterleaveParam(m_custom_channel, m_set_channel, m_channels, m_period_channel, m_channelMapStart, m_channelMapEnd, m_eff_channels, m_intrlve_pct_channel);

    uint64_t seg_mask;
    uint8_t seg_bits;
    std::tie(seg_mask, seg_bits) = parseFlexibleAddress(m_flexible_addr, 'C');
    m_config_addr.m_config_ch.setSegMask(seg_mask);
    m_config_addr.m_config_ch.setSegBits(seg_bits);
    std::tie(seg_mask, seg_bits) = parseFlexibleAddress(m_flexible_addr, 'R');
    m_config_addr.m_config_rk.setSegMask(seg_mask);
    m_config_addr.m_config_rk.setSegBits(seg_bits);
    std::tie(seg_mask, seg_bits) = parseFlexibleAddress(m_flexible_addr, 'G');
    m_config_addr.m_config_bk.setSegMask(seg_mask);
    m_config_addr.m_config_bk.setSegBits(seg_bits);
    std::tie(seg_mask, seg_bits) = parseFlexibleAddress(m_flexible_addr, 'B');
    m_config_addr.m_config_bg.setSegMask(seg_mask);
    m_config_addr.m_config_bg.setSegBits(seg_bits);
    std::tie(seg_mask, seg_bits) = parseFlexibleAddress(m_flexible_addr, 'W');
    m_config_addr.m_config_ro.setSegMask(seg_mask);
    m_config_addr.m_config_ro.setSegBits(seg_bits);
    std::tie(seg_mask, seg_bits) = parseFlexibleAddress(m_flexible_addr, 'L');
    m_config_addr.m_config_co.setSegMask(seg_mask);
    m_config_addr.m_config_co.setSegBits(seg_bits);

    m_config_addr.m_config_ch.setAddressMapping(m_address_mapping);
    m_config_addr.m_config_rk.setAddressMapping(m_address_mapping);
    m_config_addr.m_config_bk.setAddressMapping(m_address_mapping);
    m_config_addr.m_config_bg.setAddressMapping(m_address_mapping);
    m_config_addr.m_config_ro.setAddressMapping(m_address_mapping);
    m_config_addr.m_config_co.setAddressMapping(m_address_mapping);
    
    m_config_type.setTypePtrn(m_type_pattern);
    m_config_type.setTypeSwitchPct(m_switch_percentage);

    m_config_access.setTranSize(m_transaction_size);
    m_config_access.setAccessMode(m_access_mode);
    m_config_access.setAddrFormat(m_addr_format);
    m_config_access.setReadFormat(m_read_format);
    m_config_access.setWriteFormat(m_write_format);
}

/*
Config::~Config() {
    delete m_config_sys;
    delete m_config_app;
    delete m_config_dev;
    //delete Config::Config;
}
*/

void Config::provideHelp(const char *argv)
{
    printf("Usage: %s --app <application_config> --system <system_config> --device <device_config>\n"
           "Example: %s --app ./config/app/app.ini --system ./config/system/system.ini --device ./config/device/device.ini\n",
           argv, argv);
}

void Config::initAppParams(const std::string &fname)
{
    reader_c = new INIReader(fname);
    if (reader_c->ParseError() < 0)
    {
        std::cerr << "Can't load config file - " << fname << std::endl;
        //AbruptExit(__FILE__, __LINE__);
    }

    m_num_requests = getInteger("requests", "num_requests", 100);

    m_address_pattern = m_config_addr.parseAddrPtrn(reader_c->Get("address", "address_pattern", "sequential"));

    m_type_pattern = m_config_type.parseTypePtrn(reader_c->Get("type", "Type_pattern", "all_read"));
    m_switch_percentage = getInteger("type", "switch_percentage", 100);

    m_access_mode = reader_c->Get("access", "access_mode", "no_change");
    m_addr_format = reader_c->Get("access", "addr_format", "HEX");
    m_read_format = reader_c->Get("access", "read_format", "READ");
    m_write_format = reader_c->Get("access", "write_format", "WRITE");

    m_custom_channel = m_config_addr.m_config_ch.parseSegPtrn(reader_c->Get("channel", "custom_channel", "no_change"));
    m_set_channel = getInteger("channel", "set_channel", 0);
    m_period_channel = getInteger("channel", "period_channel", 1);
    m_eff_channels = getInteger("channel", "eff_channels", 1);
    m_intrlve_pct_channel = getInteger("channel", "interleave_pct_channel", 100);

    m_custom_rank = m_config_addr.m_config_rk.parseSegPtrn(reader_c->Get("rank", "custom_rank", "no_change"));
    m_set_rank = getInteger("rank", "set_rank", 0);
    m_period_rank = getInteger("rank", "period_rank", 1);
    m_eff_ranks = getInteger("rank", "eff_ranks", 1);
    m_intrlve_pct_rank = getInteger("rank", "interleave_pct_rank", 100);

    m_custom_bankgroup = m_config_addr.m_config_bg.parseSegPtrn(reader_c->Get("bankgroup", "custom_bankgroup", "no_change"));
    m_set_bankgroup = getInteger("bankgroup", "set_bankgroup", 0);
    m_period_bankgroup = getInteger("bankgroup", "period_bankgroup", 1);
    m_eff_bankgroups = getInteger("bankgroup", "eff_bankgroups", 1);
    m_intrlve_pct_bankgroup = getInteger("bankgroup", "interleave_pct_bankgroup", 100);

    m_custom_bank = m_config_addr.m_config_bk.parseSegPtrn(reader_c->Get("bank", "custom_bank", "no_change"));
    m_set_bank = getInteger("bank", "set_bank", 0);
    m_period_bank = getInteger("bank", "period_bank", 1);
    m_eff_banks = getInteger("bank", "eff_banks", 1);
    m_intrlve_pct_bank = getInteger("bank", "interleave_pct_bank", 100);

    m_custom_row = m_config_addr.m_config_ro.parseSegPtrn(reader_c->Get("row", "custom_row", "no_change"));
    m_set_row = getInteger("row", "set_row", 0);
    m_period_row = getInteger("row", "period_row", 1);
    m_hit_percentage = getInteger("row", "hit_percentage", 100);

    m_custom_column = m_config_addr.m_config_co.parseSegPtrn(reader_c->Get("column", "custom_column", "no_change"));
    m_set_column = getInteger("column", "set_column", 1);
    m_period_column = getInteger("column", "period_column", 0);
}

void Config::initSystemParams(const std::string &fname)
{
    reader_c = new INIReader(fname);
    if (reader_c->ParseError() < 0)
    {
        std::cerr << "Can't load config file - " << fname << std::endl;
        //AbruptExit(__FILE__, __LINE__);
    }

    m_address_width = getInteger("system", "address_width", 32);
    m_transaction_size = getInteger("system", "transactionSize", 32);

    m_channelMapStart = getInteger("system", "channelMapStart", 32);
    m_channelMapEnd = getInteger("system", "channelMapEnd", 32);
    m_rankMapStart = getInteger("system", "rankMapStart", 32);
    m_rankMapEnd = getInteger("system", "rankMapEnd", 32);
    m_bankgroupMapStart = getInteger("system", "bankgroupMapStart", 32);
    m_bankgroupMapEnd = getInteger("system", "bankgroupMapEnd", 32);
    m_bankMapStart = getInteger("system", "bankMapStart", 32);
    m_bankMapEnd = getInteger("system", "bankMapEnd", 32);
    m_rowMapStart = getInteger("system", "rowMapStart", 32);
    m_rowMapEnd = getInteger("system", "rowMapEnd", 32);
    m_columnMapStart = getInteger("system", "columnMapStart", 32);
    m_columnMapEnd = getInteger("system", "columnMapEnd", 32);

    m_flexible_addr = reader_c->Get("system", "flex_addr", "-");
    printf("%s   %lu\n", m_flexible_addr.c_str(), m_flexible_addr.length());

    m_address_mapping = reader_c->Get("system", "address_mapping", "Segment");
    printf("%s\n", m_address_mapping.c_str());
}

void Config::initDeviceParams(const std::string &fname)
{
    reader_c = new INIReader(fname);
    if (reader_c->ParseError() < 0)
    {
        std::cerr << "Can't load config file - " << fname << std::endl;
        //AbruptExit(__FILE__, __LINE__);
    }

    m_protocol = reader_c->Get("dram_structure", "protocol", "DDR3");

    m_channels = getInteger("dram_structure", "channels", 1);
    m_ranks = getInteger("dram_structure", "ranks", 1);
    m_bankgroups = getInteger("dram_structure", "bankGroups", 1);
    m_banks = getInteger("dram_structure", "banks", 4);
    m_rows = getInteger("dram_structure", "rows", 65536);
    m_columns = getInteger("dram_structure", "columns", 1024);
}

int Config::getInteger(const std::string &sec, const std::string &opt, int default_val) const
{
    return static_cast<int>(reader_c->GetInteger(sec, opt, default_val));
}

std::tuple<uint64_t, uint8_t> Config::parseFlexibleAddress(std::string addr, char seg)
{
    //printf("%s   %lu\n", addr.c_str(), addr.length());
    uint64_t seg_mask = 0;
    uint8_t seg_bits = 0;
    for (size_t i = 0; i < addr.length(); i++)
    {
        if (addr[i] == seg)
        {
            seg_mask += pow(2, addr.length() - i - 1);
            seg_bits++;
        }
    }
    //printf("%lu   %u\n", seg_mask, seg_bits);
    return {seg_mask, seg_bits};
}

void Config::debugAdressWidth()
{
    int calc_addr_width = 0;
    calc_addr_width += ceil(log(m_columns) / log(2));
    calc_addr_width += ceil(log(m_rows) / log(2));
    calc_addr_width += ceil(log(m_banks) / log(2));
    calc_addr_width += ceil(log(m_bankgroups) / log(2));
    calc_addr_width += ceil(log(m_ranks) / log(2));
    calc_addr_width += ceil(log(m_channels) / log(2));

    if (m_address_width != calc_addr_width)
    {
        printf("Address mapping %0d is incompatible with the calculated from segments %0d\n",
               m_address_width, calc_addr_width);

        //assert(false);
    }
}

void Config::debugSegmentWidth(int lsb, int msb, int seg_num, std::string seg_name)
{
    if ((msb - lsb + 1) != ceil(log(seg_num) / log(2)))
    {
        printf("Segment mapping [%0d:%0d] is incompatible with the number of %s = %0d\n",
               msb, lsb, seg_name.c_str(), seg_num);

        //assert(false);
    }
}

void Config::debugSetSegment(int set_val, int seg_num, std::string seg_name)
{
    if (set_val >= seg_num)
    {
        printf("Segment set value = %0d is out of range for the %s = [%0d:0]\n",
               set_val, seg_name.c_str(), seg_num - 1);

        //assert(false);
    }
}