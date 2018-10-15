#ifndef IEVENTCOUNTER_H_
#define IEVENTCOUNTER_H_

#include <iostream>
#include <fstream>
#include <mutex>
#include <vector>
#include <utility>
#include <cstring>
#include <string>
#include <atlstr.h>
#include <map>
#include <iomanip>
#include <sstream>
#include <ctime>

class IEventCounter
{
	std::mutex safe;
	std::map<CString, int> eventByIDMap;

public:
	IEventCounter();
	~IEventCounter();
	void ParseEvents(CString deviceID, const char* logName);
	int GetEventCount(CString deviceID);
	
	bool IsItLogErrorStep1(std::time_t past, std::time_t current);
	int IsLogErrorComplete(std::vector<std::pair<std::time_t, int>> fileContent, int stepOneIndex);
	//std::vector<std::pair<std::time_t, int>> CutToNextStageZero(std::vector<std::pair<std::time_t, int>> fileContent, int index);
};

#endif