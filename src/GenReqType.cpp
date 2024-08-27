#include "GenReqType.h"

//using namespace std;
using namespace RAMbench;

GenReqType::GenReqType(Request *request, const ConfigType config, const int num_requests)
{
	m_num_reqs = num_requests;
	type_pattern type_ptrn = config.getTypePtrn();
	m_switch_percentage = config.getTypeSwitchPct();

	if (type_ptrn == type_pattern::all_read)
		readAccess(request);
	else if (type_ptrn == type_pattern::all_write)
		writeAccess(request);
	else if (type_ptrn == type_pattern::rw_random)
		randomAccess(request);
	else if (type_ptrn == type_pattern::rw_switch_pct)
		customReadWrite(request);
}

void GenReqType::readAccess(Request *request)
{
	for (int i = 0; i < m_num_reqs; i++)
	{
		request[i].type = Request::Type::READ;
	}
}

void GenReqType::writeAccess(Request *request)
{
	for (int i = 0; i < m_num_reqs; i++)
	{
		request[i].type = Request::Type::WRITE;
	}
}

void GenReqType::randomAccess(Request *request)
{
	std::random_device req_rnd;

	std::mt19937 rng(req_rnd());
	std::uniform_int_distribution<int> uni((int)Request::Type::READ, (int)Request::Type::WRITE);
	for (int i = 0; i < m_num_reqs; i++)
	{
		request[i].type = uni(rng) ? Request::Type::WRITE : Request::Type::READ;
	}
}

void GenReqType::customReadWrite(Request *request)
{

	int patternAccuracy = 100;
	double switchRate = double(m_switch_percentage) / double(100);

	//printf("%d\n", switch_percentage);
	//printf("%f \n", switchRate);
	//int matchPercentage  = 100 - Config.switch_percentage;
	//int matchRate        = matchPercentage/100;

	int patternSwitches, periodSize;
	std::tie(patternSwitches, periodSize) = reducedPeriod(m_switch_percentage, patternAccuracy);
	//finds the smallest fraction that represents the number of interleaves/pattern size

	//printf("%d  %d\n", patternSwitches, periodSize);

	int numOfSwitches = round(switchRate * m_num_reqs);

	//printf("%d \n", numOfSwitches);
	//double numOfMatches = Config.num_requests - numOfSwitches;
	int switchTracker = 0;
	int matchTracker = 0;
	int writeTracker = 0;
	int currentType = int(Request::Type::READ);

	for (int i = 0; i < m_num_reqs; i++)
	{
		request[i].type = (currentType % 2) ? Request::Type::WRITE : Request::Type::READ;
		if (((i % periodSize < patternSwitches) && (switchTracker < numOfSwitches)) || (writeTracker % 2 == 1))
		{
			//request[i].type = currentType % Config.numOfTypes;
			++currentType;
			++switchTracker;
			++writeTracker;
		}
		else
		{
			//request[i].type = currentType % Config.numOfTypes;
			++matchTracker;
		}

		/*printf("%d %d %d %d %d %d %d %d %d\n",
		i, periodSize, i % periodSize, patternSwitches,
		switchTracker, numOfSwitches,
		currentType, matchTracker,
		request[i].type);*/
	}

	//std::cout << "number of switches: " << switchTracker << " number of matches: " << matchTracker <<"\n";
}

std::tuple<int, int> GenReqType::reducedPeriod(int numerator, int denominator)
{

	int gcd = std::__gcd(numerator, denominator);
	int numReduced = numerator / gcd;
	int denomReduced = denominator / gcd;

	return {numReduced, denomReduced};
}