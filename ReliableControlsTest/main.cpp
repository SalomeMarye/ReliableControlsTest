#include <thread>
#include <iostream>
#include <vector>
#include <string>
#include "IEventCounter.h"

struct FileInfo {
	CString deviceId;
	const char *logName;
	int result;
};

int main()
{
	IEventCounter counter;
	std::vector<std::thread> threads;
	const std::vector<FileInfo> files = {
		{ "Device1", "../Tests/test1.txt", 1 },
		{ "Device2", "../Tests/test2.txt", 1 },
		{ "Device3", "../Tests/test3.txt", 1 },
		{ "Device4", "../Tests/test4.txt", 0 },
		{ "Device5", "../Tests/test5.txt", 0 },
		{ "Device6", "../Tests/test6.txt", 5 },
	};

	for (auto &f : files)
		threads.push_back(std::thread(&IEventCounter::ParseEvents, &counter, f.deviceId, f.logName));
	for (auto &it : threads)
		it.join();

	for (auto &f : files) {
		if (counter.GetEventCount(f.deviceId) != f.result) {
			std::cerr << "Device " << f.deviceId << " with log file (" << f.logName << ")" << " result doesn't correspond to attended result (" << f.result << ").";
			getchar();
			return -1;
		}
		else
			std::cout << "Device " << f.deviceId << " with log file (" << f.logName << ")" << " result correspond to attended result (" << f.result << ")." << std::endl;
	}

	std::cout << "end, press key to close";
	getchar();
	return 0;
}