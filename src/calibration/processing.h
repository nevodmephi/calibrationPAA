#ifndef PROCESSING_H
#define PROCESSING_H

#include <condition_variable>
#include <mutex>
#include <vector>
#include <string>
#include <iomanip>
#include <fstream>
#include <cstdint>
#include <bitset>
#include <chrono>
#include <json.hpp>

#include "../module/queueofmessages.h"
#include "../observer/observer.h"

class Processing : public Observer, public std::enable_shared_from_this<Processing>	{
public:
	using json = nlohmann::json;

	struct Data	{
		int32_t	typePacket;
		std::array<uint32_t, 8>	time;
		std::vector<std::vector<int16_t>>	data;
	};

	struct Time {
		uint32_t	hours;
		uint32_t	minutes;
		uint32_t	seconds;
		uint32_t	milliseconds;
		uint32_t	microseconds;
		uint32_t	ns10;
	};

	using QueuePtr	= std::shared_ptr<QueueOfMessages>;

	Processing(QueuePtr	clientQueue);
	~Processing();

	void	update(const Subject* subject);

	void	setPathToSave(const std::string&	path);
	void	createFolders(const std::string&	path);
	void	setStartedTime();
	Time	getStartedTime()	const;
	void	saveOpticalLength(const std::array<int32_t, 4>& lengths_);

	void	parse(const std::vector<uchar>&	data);	//move to protected

protected:
	void		controlQueue();
	void		saveDataHowOldProgram(const std::vector<Data> &parserData, const std::string& path, int numberFile);
	Time		fromRawTime(const std::array<uint32_t, 8>&	time);
	uint32_t	sliceAllBits(const std::bitset<16 * 4>&	allBits, int begin, int end);
	Time		plusTime(const Time& first, const Time& second);
	uint64_t	fromTimeToNanoseconds(const Time& time);
	Time		fromNanosecondsToTime(uint64_t time);
	void		saveStartedTime(const Time& time, const std::string& path);

private:
	QueuePtr								clientQueue_;
	std::queue<std::vector<unsigned char>>	buffer_;
	std::vector<Data>						data_;
	std::string								pathToSave_;

	std::condition_variable					queueCheck_;
	std::mutex								lockQueueCheck_;
	Time									startedTime_;
	Time									oldTime_;
};

#endif // PROCESSING_H
