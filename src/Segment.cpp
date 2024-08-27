#include "Segment.h"

//using namespace std;
using namespace RAMbench;

/*
Segment::Segment(){};

Segment::Segment(int _num_reqs, int _addr_width)
{
	m_num_reqs = _num_reqs;
	m_addr_width = _addr_width;

	//m_seg_pattern = config.m_custom_column;
	//m_set_value = config.m_set_column;

	//m_seg_start = config.m_columnMapStart;
	//m_seg_end = config.m_columnMapEnd;
	//m_seg_num = config.m_columns;

	SegMapDigits = 0;
	SegClearingNum = 0;
	SegDigitsAndClearing(&SegMapDigits, &SegClearingNum);

	std::cout << "cotr" << std::endl;
	std::cout << "m_num_reqs = " << m_num_reqs << std::endl;
	std::cout << "m_addr_width = " << m_addr_width << std::endl;
}

SegColumn::SegColumn(int _num_reqs, int _addr_width) : Segment(_num_reqs, _addr_width)
{
	std::cout << "SegColumn cotr" << std::endl;
	std::cout << "m_num_reqs = " << m_num_reqs << std::endl;
	std::cout << "m_addr_width = " << m_addr_width << std::endl;
};

SegColumn::SegColumn(SegColumn const &seg_co)
{
	std::cout << "coulmn cotr" << std::endl;
	std::cout << "m_num_reqs = " << seg_co.m_num_reqs << std::endl;
	std::cout << "m_addr_width = " << seg_co.m_addr_width << std::endl;
};
*/

Segment::Segment(Request *request, const ConfigSeg config, const int num_reqs, const int addr_width)
{
	m_num_reqs = num_reqs;

	m_seg_pattern = config.getSegPtrn();

	m_set_value = config.getSegVal();
	m_period_value = config.getSegPeriod();

	m_addr_width = addr_width;

	m_seg_start = config.getAddrLSB();
	m_seg_end = config.getAddrMSB();

	m_seg_mask = config.getSegMask();
	m_seg_bits = config.getSegBits();

	m_address_mapping = config.getAddressMapping();

	m_seg_num = config.getSegNum();

	printf("m_seg_mask = %lu\n", m_seg_mask);
	printf("m_seg_bits = %d\n", m_seg_bits);
	printf("m_period_value = %d\n", m_period_value);

	m_seg_map_digits = 0;
	m_seg_clear_num = 0;
	SegDigitsAndClearing(&m_seg_map_digits, &m_seg_clear_num);

	if (m_seg_pattern == seg_pattern::set)
		SegSet(request);
	else if (m_seg_pattern == seg_pattern::seq)
		SegSequential(request);
	else if (m_seg_pattern == seg_pattern::rnd)
		SegRandom(request);
}

std::tuple<int, int> Segment::ReducedPeriod(int numerator, int denominator)
{
	int gcd = std::__gcd(numerator, denominator);
	int numReduced = numerator / gcd;
	int denomReduced = denominator / gcd;

	return {numReduced, denomReduced};
}

void Segment::SegDigitsAndClearing(int *seg_map_digits, int *seg_clear_num)
{
	if (m_seg_num > 1)
	{
		*seg_map_digits = ceil(log(m_seg_num) / log(2));

		//determines the maximum amount of binary digits the segment can take up
		if (m_address_mapping == "Segment")
		{
			for (int i = 0; i < m_addr_width; i++)
			{
				if ((i >= m_seg_start) && (i <= m_seg_end))
				{
					*seg_clear_num += pow(2, i);
				}
			}
		}
		else if (m_address_mapping == "Flexible")
		{
			*seg_clear_num = m_seg_mask;
		}
	}
}

long unsigned int Segment::SegMap(long unsigned int current_addr, int calculated_seg)
{
	int mapped_seg = 0;
	if (m_address_mapping == "Segment")
	{
		// creates number for binary representation of segment, with all 0's around it
		mapped_seg = calculated_seg << m_seg_start;
	}
	else if (m_address_mapping == "Flexible")
	{
		//printf("flexible \n");
		int mask = m_seg_mask;
		int seg = calculated_seg;
		for (int i = 0; i < m_addr_width; i++)
		{
			if (mask % 2 == 1)
			{
				mapped_seg += (seg % 2) * pow(2, i);
				seg /= 2;
				//printf("%d, seg = %d \n", i, seg);
			}
			mask /= 2;
		}
		//printf("mapped_seg = %d \n", mapped_seg);
	}

	// clears all the segment bits from the original address
	current_addr = current_addr & ~m_seg_clear_num;

	// replaces the segment bits with the required ones
	current_addr = current_addr | mapped_seg;

	return current_addr;
}

void Segment::SegSet(Request *request)
{
	// This loop uses the segment clearer and the mapped segment
	// to force every access to go to the set segment value
	for (int i = 0; i < m_num_reqs; i++)
	{
		m_current_addr = request[i].addr;
		request[i].addr = SegMap(m_current_addr, m_set_value);
	}
}

void Segment::SegSequential(Request *request)
{
	int ind = 0;
	for (int i = 0; i < m_num_reqs; i++)
	{
		// determines the current bank, forcing the value to be between 0 and num of banks-1
		m_calculated_seg = i % m_seg_num;

		for (int j = 0; j < m_period_value; j++)
		{
			m_current_addr = request[m_period_value * i + j].addr;
			request[m_period_value * i + j].addr = SegMap(m_current_addr, m_calculated_seg);

			ind++;
			if (ind == m_num_reqs)
				break;
		}
		if (ind == m_num_reqs)
			break;
	}
}

void Segment::SegRandom(Request *request)
{
	std::random_device seg_rnd;
	std::mt19937 rng(seg_rnd());
	std::uniform_int_distribution<long unsigned int> uni(0, m_seg_num - 1);

	int ind = 0;
	for (int i = 0; i < m_num_reqs; i++)
	{
		m_calculated_seg = uni(rng);

		for (int j = 0; j < m_period_value; j++)
		{
			m_current_addr = request[m_period_value * i + j].addr;
			request[m_period_value * i + j].addr = SegMap(m_current_addr, m_calculated_seg);

			ind++;
			if (ind == m_num_reqs)
				break;
		}
		if (ind == m_num_reqs)
			break;
	}
}

SegColumn::SegColumn(Request *request, const ConfigSeg config, const int num_reqs, const int addr_width) : Segment(request, config, num_reqs, addr_width) {}

SegRow::SegRow(Request *request, const ConfigSeg config, const int num_reqs, const int addr_width) : Segment(request, config, num_reqs, addr_width)
{
	m_hit_percentage = config.getRoHitPct();

	if (m_seg_pattern == seg_pattern::hit)
		Hit(request);
	else if (m_seg_pattern == seg_pattern::miss)
		Miss(request);
	else if (m_seg_pattern == seg_pattern::custom_hit)
		customHit(request, m_hit_percentage);
}

void SegRow::Hit(Request *request)
{
	customHit(request, 100);
	//SegSet(request);
}

void SegRow::Miss(Request *request)
{
	customHit(request, 0);
	//SegSequential(request);
}

void SegRow::customHit(Request *request, int hit_percentage)
{
	int pattern_accuracy = 100;
	double hit_rate = double(hit_percentage) / double(pattern_accuracy);
	double num_hits = round(hit_rate * m_num_reqs);
	double num_misses = m_num_reqs - num_hits;

	//int miss_tracker = 0;
	int seg_counter = 0;

	int pattern_size, pattern_hits;
	std::tie(pattern_hits, pattern_size) = ReducedPeriod(hit_percentage, pattern_accuracy);
	int pattern_misses = pattern_size - pattern_hits;

	int ind = 0;
	for (int i = 0; i < m_num_reqs; i++)
	{
		if ((i % pattern_size < pattern_misses) && (seg_counter < num_misses))
		{
			m_calculated_seg = seg_counter % m_seg_num;

			seg_counter++;
			//miss_tracker++;
		}

		for (int j = 0; j < m_period_value; j++)
		{
			//printf("i = %2d, pattern_size = %2d, pattern_misses = %2d, ", i, pattern_size, pattern_misses);
			//printf("miss_tracker = %2d, num_misses = %2f\n", miss_tracker, num_misses);
			m_current_addr = request[m_period_value * i + j].addr;
			request[m_period_value * i + j].addr = SegMap(m_current_addr, m_calculated_seg);

			ind++;
			if (ind == m_num_reqs)
				break;
		}
		if (ind == m_num_reqs)
			break;
	}
	//customHitDebug(num_hits, hit_tracker, num_misses, miss_tracker);
}

SegInterleaver::SegInterleaver(Request *request, const ConfigSeg config, const int num_reqs, const int addr_width) : Segment(request, config, num_reqs, addr_width)
{
	m_effective_value = config.getInterleaveEff();
	m_interleave_percentage = config.getInterleavePct();

	if (m_seg_pattern == seg_pattern::interleave)
		CustomInterleaver(request);
	else if (m_seg_pattern == seg_pattern::eff_interleave)
		CustomEffInterleaver(request);
}

void SegInterleaver::CustomInterleaver(Request *request)
{
	Interleaver(request, m_seg_num);
}

void SegInterleaver::CustomEffInterleaver(Request *request)
{
	Interleaver(request, m_effective_value);
}

void SegInterleaver::Interleaver(Request *request, int target_interleave)
{
	int pattern_accuracy = 100;
	int constant_percentage = 100 - m_interleave_percentage;
	double constant_rate = double(constant_percentage) / double(pattern_accuracy);
	double num_constants = round(constant_rate * m_num_reqs);
	double num_interleaves = m_num_reqs - num_constants;

	//int interleave_tracker = 0;
	//int constant_tracker = 0;
	int seg_counter = 0;

	int pattern_size, pattern_interleaves;
	std::tie(pattern_interleaves, pattern_size) = ReducedPeriod(m_interleave_percentage, pattern_accuracy);
	//finds the smallest fraction that represents the number of interleaves/pattern size
	//int patternConstants = pattern_size - pattern_interleaves;

	if (m_seg_num > 1)
	{
		int ind = 0;
		for (int i = 0; i < m_num_reqs; i++)
		{

			if ((i % pattern_size < pattern_interleaves) && (seg_counter < num_interleaves))
			{
				m_calculated_seg = seg_counter % target_interleave;
				seg_counter++;
				//interleave_tracker++;
			}

			for (int j = 0; j < m_period_value; j++)
			{
				m_current_addr = request[m_period_value * i + j].addr;
				request[m_period_value * i + j].addr = SegMap(m_current_addr, m_calculated_seg);

				ind++;
				if (ind == m_num_reqs)
					break;
			}

			if (ind == m_num_reqs)
				break;
		}
	}
	//customInterLeavesDebug(interleave_tracker, num_interleaves, constant_tracker, num_constants);
}

SegBank::SegBank(Request *request, const ConfigSeg config, const int num_reqs, const int addr_width) : SegInterleaver(request, config, num_reqs, addr_width) {}

SegBankgroup::SegBankgroup(Request *request, const ConfigSeg config, const int num_reqs, const int addr_width) : SegInterleaver(request, config, num_reqs, addr_width) {}

SegRank::SegRank(Request *request, const ConfigSeg config, const int num_reqs, const int addr_width) : SegInterleaver(request, config, num_reqs, addr_width) {}

SegChannel::SegChannel(Request *request, const ConfigSeg config, const int num_reqs, const int addr_width) : SegInterleaver(request, config, num_reqs, addr_width) {}