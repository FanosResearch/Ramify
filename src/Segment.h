#ifndef __SEGMENT_H
#define __SEGMENT_H

#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <iostream>
#include <cassert>
#include <cmath>

#include "GenReqAddr.h"
#include "Request.h"

namespace RAMbench
{

    class Segment
    {
    public:
        Segment(Request *request, const ConfigSeg config, const int num_reqs, const int addr_width);

    private:
    protected:
        long unsigned int SegMap(long unsigned int current_addr, int calculated_seg);
        void SegSet(Request *request);
        void SegSequential(Request *request);
        void SegRandom(Request *request);
        void SegDigitsAndClearing(int *seg_map_digits, int *seg_clear_num);
        std::tuple<int, int> ReducedPeriod(int numerator, int denominator);

        int m_num_reqs;
        seg_pattern m_seg_pattern;
        int m_set_value;
        int m_period_value;

        int m_addr_width;

        int m_seg_start;
        int m_seg_end;

        uint64_t m_seg_mask;
        uint8_t m_seg_bits;

        std::string m_address_mapping;

        int m_seg_num;

        int m_seg_map_digits;
        int m_seg_clear_num;

        int m_calculated_seg;
        long unsigned int m_current_addr;
    };

    // SegColumn publicly inherits from Segment
    class SegColumn : public Segment
    {
    public:
        SegColumn(Request *request, const ConfigSeg config, const int num_reqs, const int addr_width);

    private:
    protected:
    };

    // SegRow publicly inherits from Segment
    class SegRow : public Segment
    {
    public:
        SegRow(Request *request, const ConfigSeg config, const int num_reqs, const int addr_width);
        bool SegState();

    private:
        int m_hit_percentage;

    protected:
        void Hit(Request *request);
        void Miss(Request *request);
        void customHit(Request *request, int percentage);
    };

    // SegInterleaver publicly inherits from Segment
    class SegInterleaver : public Segment
    {
    public:
        SegInterleaver(Request *request, const ConfigSeg config, const int num_reqs, const int addr_width);

    private:
    protected:
        int m_effective_value;
        int m_interleave_percentage;

        void CustomInterleaver(Request *request);
        void CustomEffInterleaver(Request *request);
        void Interleaver(Request *request, int m_target_num);
    };

    // SegBank publicly inherits from SegInterleaver
    class SegBank : public SegInterleaver
    {
    public:
        SegBank(Request *request, const ConfigSeg config, const int num_reqs, const int addr_width);

    private:
    protected:
    };

    // SegBankgroup publicly inherits from SegInterleaver
    class SegBankgroup : public SegInterleaver
    {
    public:
        SegBankgroup(Request *request, const ConfigSeg config, const int num_reqs, const int addr_width);

    private:
    protected:
    };

    // SegRank publicly inherits from SegInterleaver
    class SegRank : public SegInterleaver
    {
    public:
        SegRank(Request *request, const ConfigSeg config, const int num_reqs, const int addr_width);

    private:
    protected:
    };

    // SegChannel publicly inherits from SegInterleaver
    class SegChannel : public SegInterleaver
    {
    public:
        SegChannel(Request *request, const ConfigSeg config, const int num_reqs, const int addr_width);

    private:
    protected:
    };

} /* namespace RAMBench */

#endif /* _SEGMENT_H */
