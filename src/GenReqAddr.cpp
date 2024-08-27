#include "GenReqAddr.h"

//using namespace std;
using namespace RAMbench;

GenReqAddr::GenReqAddr(Request *request, const ConfigAddr config, const int num_requests)
{
	m_num_reqs = num_requests;
	addr_pattern addr_ptrn = config.getAddrPtrn();
	m_addr_width = config.getAddrWidth();

	if (addr_ptrn == addr_pattern::sequential ||
		addr_ptrn == addr_pattern::seq_customized)
		sequentialIndexArray(request);
	else if (addr_ptrn == addr_pattern::random ||
			 addr_ptrn == addr_pattern::rnd_customized)
		randomIndexArray(request);

	if (addr_ptrn == addr_pattern::customized ||
		addr_ptrn == addr_pattern::seq_customized ||
		addr_ptrn == addr_pattern::rnd_customized)
		customIndexArray(request, config);
}

void GenReqAddr::sequentialIndexArray(Request *request)
{
	for (int i = 0; i < m_num_reqs; i++)
		request[i].addr = i;
}

void GenReqAddr::randomIndexArray(Request *request)
{
	std::random_device addr_rnd;
	std::mt19937 rng(addr_rnd());
	std::uniform_int_distribution<long unsigned int> uni(0, pow(2, m_addr_width) - 1);

	// create an array for the random access order
	for (int i = 0; i < m_num_reqs; i++)
	{
		long unsigned int random_value = uni(rng);
		request[i].addr = random_value;
	}
}

void GenReqAddr::customIndexArray(Request *request, const ConfigAddr config)
{
	// Channel
	if (config.m_config_ch.getSegPtrn() != seg_pattern::no_change)
	{
		printf("custom Channel\n");
		SegChannel(request, config.m_config_ch, m_num_reqs, m_addr_width);
	}

	// Rank
	if (config.m_config_rk.getSegPtrn() != seg_pattern::no_change)
	{
		printf("custom Rank\n");
		SegRank(request, config.m_config_rk, m_num_reqs, m_addr_width);
	}

	// BankGroup
	if (config.m_config_bg.getSegPtrn() != seg_pattern::no_change)
	{
		printf("custom Bankgroup\n");
		SegBankgroup(request, config.m_config_bg, m_num_reqs, m_addr_width);
	}

	// Bank
	if (config.m_config_bk.getSegPtrn() != seg_pattern::no_change)
	{
		printf("custom Bank\n");
		SegBank(request, config.m_config_bk, m_num_reqs, m_addr_width);
	}

	// Row
	if (config.m_config_ro.getSegPtrn() != seg_pattern::no_change)
	{
		printf("custom Row\n");
		SegRow(request, config.m_config_ro, m_num_reqs, m_addr_width);
	}

	// Column
	if (config.m_config_co.getSegPtrn() != seg_pattern::no_change)
	{
		printf("custom Column\n");
		SegColumn(request, config.m_config_co, m_num_reqs, m_addr_width);
	}
}
