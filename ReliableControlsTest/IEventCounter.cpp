#include "IEventCounter.h"

IEventCounter::IEventCounter()
{
}


IEventCounter::~IEventCounter()
{
}

void IEventCounter::ParseEvents(CString deviceID, const char* logName) {

	int eventCount = 0;
	std::ifstream file;
	std::vector<std::pair<std::time_t, int>> fileContent;

	file.open(logName);
	if (!file) {
		std::cout << "Unable to open file";
	}
	else {
		std::string line;
		while (std::getline(file, line))
		{
			int stage;
			struct std::tm tm;

			std::istringstream ss(line);
			ss >> std::get_time(&tm, "%Y-%m-%d %T") >> stage;
			std::time_t date = mktime(&tm);

			fileContent.push_back(std::make_pair(date, stage));
		};
	file.close();
	}

	for (unsigned int index = 0; index < fileContent.size(); index++) {

		if ((fileContent[index].second == 3 && index + 1 < fileContent.size())
			&& this->IsItLogErrorStep1(fileContent[index].first, fileContent[index + 1].first)) {

			int result = this->IsLogErrorComplete(fileContent, index);
			if (result == -2)
				break;
			else if (result > 0){
				eventCount += 1;
				index = result;
			}
		}
	}

	std::lock_guard<std::mutex> lock(this->safe);
	this->eventByIDMap[deviceID] = eventCount;
}

/**
	If the given deviceID corresponds to none of the parsed ids , the function returns -1.
*/
int IEventCounter::GetEventCount(CString deviceID) {
	
	std::lock_guard<std::mutex> lock(this->safe);
	auto it = this->eventByIDMap.find(deviceID);
	if (it == this->eventByIDMap.end())
		return -1;
	
	return (it->second);
}

bool IEventCounter::IsItLogErrorStep1(std::time_t current, std::time_t future) {

	if (double seconds = difftime(future, current) >= 300)
		return true;
	return false;
}

/**
return -1 == error log not complete
return -2 == error log not complete and end of file
anything else -> new value of index
*/
int IEventCounter::IsLogErrorComplete(std::vector<std::pair<std::time_t, int>> fileContent, int stepOneIndex) {

	int step = 1;

	for (unsigned int index = stepOneIndex; index < fileContent.size(); index++)
	{
		if (step == 1) {
			if (fileContent[index].second != 3 && fileContent[index].second != 2)
				return -1;
			else if (fileContent[index].second == 2)
				step += 1;
		}
		else if (step == 2) {
			if (fileContent[index].second != 3
				&& fileContent[index].second != 2
				&& fileContent[index].second != 0)
				return -1;
			else if (fileContent[index].second == 0)
				return index;
		}
	}

	return -2;
}
