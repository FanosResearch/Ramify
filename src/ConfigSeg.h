#ifndef __CONFIGSEG_H
#define __CONFIGSEG_H

#include <stdio.h>
#include <cstring>
#include <map>

namespace RAMbench
{

    enum seg_pattern
    {
        no_change,
        set,
        seq,
        rnd,

        hit,
        miss,
        custom_hit,

        interleave,
        eff_interleave
    };

    class ConfigSeg
    {
    public:
        ConfigSeg(){};

        seg_pattern parseSegPtrn(std::string pattern_str) const
        {
            std::map<std::string, seg_pattern> seg_pattern_pairs = {
                {"no_change", seg_pattern::no_change},
                {"set", seg_pattern::set},
                {"seq", seg_pattern::seq},
                {"rnd", seg_pattern::rnd},

                {"hit", seg_pattern::hit},
                {"miss", seg_pattern::miss},
                {"custom_hit", seg_pattern::custom_hit},

                {"interleave", seg_pattern::interleave},
                {"eff_interleave", seg_pattern::eff_interleave}};

            if (seg_pattern_pairs.find(pattern_str) == seg_pattern_pairs.end())
            {
                std::cout << "Unkwown/Unsupported Seg Pattern: " << pattern_str
                          << " Aborting!" << std::endl;
                //AbruptExit(__FILE__, __LINE__);
            }

            return seg_pattern_pairs[pattern_str];
        }

        void setSegPtrn(seg_pattern pattern) { m_seg_pattern = pattern; };
        seg_pattern getSegPtrn() const { return m_seg_pattern; };

        void setSegVal(int value) { m_set_seg = value; };
        int getSegVal() const { return m_set_seg; };

        void setSegNum(int value) { m_num_seg = value; };
        int getSegNum() const { return m_num_seg; };

        void setSegPeriod(int value) { m_period_seg = value; };
        int getSegPeriod() const { return m_period_seg; };

        void setAddrLSB(int value) { m_addr_map_lsb = value; };
        int getAddrLSB() const { return m_addr_map_lsb; };

        void setAddrMSB(int value) { m_addr_map_msb = value; };
        int getAddrMSB() const { return m_addr_map_msb; };

        void setRoHitPct(int hit_percentage) { m_hit_percentage = hit_percentage; };
        int getRoHitPct() const { return m_hit_percentage; };

        void setInterleaveEff(int interleave_eff) { m_interleave_eff = interleave_eff; };
        int getInterleaveEff() const { return m_interleave_eff; };

        void setInterleavePct(int interleave_percentage) { m_interleave_percentage = interleave_percentage; };
        int getInterleavePct() const { return m_interleave_percentage; };

        void setSegMask(uint64_t seg_mask) { m_seg_mask = seg_mask; };
        int getSegMask() const { return m_seg_mask; };

        void setSegBits(uint8_t seg_bits) { m_seg_bits = seg_bits; };
        uint8_t getSegBits() const { return m_seg_bits; };

        void setAddressMapping(std::string address_mapping) { m_address_mapping = address_mapping; };
        std::string getAddressMapping() const { return m_address_mapping; };

        void setSegParam(seg_pattern pattern, int set_value, int num_value, int period_seg, int lsb_value, int msb_value)
        {
            m_seg_pattern = pattern;
            m_set_seg = set_value;
            m_num_seg = num_value;
            m_period_seg = period_seg;
            m_addr_map_lsb = lsb_value;
            m_addr_map_msb = msb_value;
        };

        void setRoParam(seg_pattern pattern, int set_value, int num_value, int period_seg,
                        int lsb_value, int msb_value, int hit_percentage)
        {
            setSegParam(pattern, set_value, num_value, period_seg, lsb_value, msb_value);

            m_hit_percentage = hit_percentage;
        };

        void setInterleaveParam(seg_pattern pattern, int set_value, int num_value, int period_seg,
                                int lsb_value, int msb_value, int interleave_eff, int interleave_percentage)
        {
            setSegParam(pattern, set_value, num_value, period_seg, lsb_value, msb_value);

            m_interleave_eff = interleave_eff;
            m_interleave_percentage = interleave_percentage;
        };

    private:
        seg_pattern m_seg_pattern = seg_pattern::no_change;
        std::string m_address_mapping;

        int m_num_seg = 0;

        int m_set_seg = 0;
        int m_period_seg = 0;

        int m_addr_map_lsb = 0;
        int m_addr_map_msb = 0;

        uint64_t m_seg_mask = 0;
        uint8_t m_seg_bits = 0;

        int m_hit_percentage = 0;

        int m_interleave_eff = 0;
        int m_interleave_percentage = 0;
    };

};

#endif /* _CONFIGSEG_H */