#include "processing.h"
#include <QFile>
#include <iostream>

Processing::Processing(QueuePtr	clientQueue)
	:	clientQueue_(clientQueue)	{
	clientQueue_->attach(this);

	std::thread	processingThread(&Processing::controlQueue, this);
	processingThread.detach();
}

Processing::~Processing()	{
	clientQueue_->detach(this);
}

void	Processing::update(const Subject *subject)	{
	if (subject == clientQueue_.get())	{
		if (clientQueue_->getMessage() == QueueOfMessages::Message::dataRead)	{
			buffer_.push(clientQueue_->getData());
			queueCheck_.notify_one();
			clientQueue_->clearData();
		}
	}
}

void Processing::setPathToSave(const std::string& path)	{
	pathToSave_ = path;
}

void	Processing::controlQueue()	{
	while (true)	{
		std::unique_lock<std::mutex>	locker(lockQueueCheck_);
		queueCheck_.wait(locker);
		static int number = 0;
		static int i = 0;
		while (buffer_.empty() != true)	{
			data_.clear();
			parse(buffer_.front());

//			std::fstream	file;
//			file.open("/home/main/datda", std::ios_base::app | std::ios_base::binary);
//			file.write(reinterpret_cast<const char*>(buffer_.front().data()), buffer_.front().size());
//			file.close();

			saveDataHowOldProgram(data_, pathToSave_, number);
			i++;
			if (i == 50)	{
				i = 0;
				number++;
			}
			buffer_.pop();
		}
	}
}

void	Processing::parse(const std::vector<uchar>& data)	{
	std::vector<char>	buf;
	for (int i = 0; i < data.size(); i++)
		buf.push_back(data[i]);

	int pHeader = 0;
	int pHeaderCount = 0;

	int pData = 0;
	int pDataCount = 0;

	int sampleNumber = 1;
	int channelNumber = 0;
	int channelHeader = 0;
	int channelHeaderCount = 0;
	int channelDataCount = 0;

	int channelLength = 0;

	int	typePacket	= -1;
	bool	isTypePacketActive = false;
	std::vector<uint32_t>	timeFirst;
	std::vector<uint32_t>	timeSecond;

	std::vector<std::pair<int, std::vector<std::vector<uint32_t>>>>	DB;
	std::map<int, std::vector<uint32_t>>	time;
	DB.resize(1000);
	for (int i = 0; i < (int)DB.size(); i++)
		DB[i].second.resize(16);
	for (int i = 0; i < (int)DB.size(); i++)
		for (int j = 0; j < (int)DB[i].second.size(); j++)
			DB[i].second[j].resize(1024);

	int fileLength = (buf).size();
	for (int i = 3; i < fileLength; i++)	{
		if (	(uint16_t)(buf)[i - 3] == (uint16_t)0xffffffff
			&&	(uint16_t)(buf)[i - 2] == (uint16_t)0xffffffff
			&&	(uint16_t)(buf)[i - 1] == (uint16_t)0xffffffff
			&&	(uint16_t)(buf)[i] == (uint16_t)0xffffffff)				{

			pHeader = 1;
			pHeaderCount = 0;
			pData = 0;
			continue;
		}

		if ((uint8_t)buf[i - 1] == 0xd8 &&
				(uint8_t)buf[i] == 0x0b && isTypePacketActive == false)	{
			typePacket	= 0;
			isTypePacketActive = true;
		}

		if ((uint8_t)buf[i - 1] == 0xd9 &&
				(uint8_t)buf[i] == 0x0b && isTypePacketActive == false)	{
			typePacket	= 1;
			isTypePacketActive = true;
		}

		if ((uint8_t)buf[i - 1] == 0xda &&
				(uint8_t)buf[i] == 0x0b && isTypePacketActive == false)	{
			typePacket	= 2;
			isTypePacketActive = true;
		}

		if ((uint8_t)buf[i - 1] == 0xdb &&
				(uint8_t)buf[i] == 0x0b && isTypePacketActive == false)	{
			typePacket	= 3;
			isTypePacketActive = true;
		}

		if (pHeader == 1)	{
			if (pHeaderCount == 23)	{
				pHeader = 0;
				pData = 1;
				pDataCount = 0;
				channelHeader = 1;
				channelHeaderCount = 0;
				channelDataCount = 0;
				continue;
			}
			pHeaderCount++;
		}
		if (pData == 1)	{
			if (channelHeader == 1)	{
				if (channelHeaderCount == 4)
					channelLength = (int)(buf)[i];
				if (channelHeaderCount == 5)	{
					channelLength += ((int)((buf)[i] & 0xff) * 256);
					channelLength *= 2;
				}
				if (channelHeaderCount == 6)	{
					channelNumber = (int)((buf)[i] & 0xf);
				}
				if (channelHeaderCount == 7)	{
					if (channelLength != 0)	{
						channelHeader = 0;
						channelDataCount = 0;
						if (channelLength == 8)	{
							if (channelNumber == 8)
								for (int j = i + 1; j < i + 9; j++)
									timeFirst.push_back((uint8_t)buf[j]);
							if (channelNumber == 9)
								for (int j = i + 1; j < i + 9; j++)
									timeSecond.push_back((uint8_t)buf[j]);
							int a = 15;
							a--;
						}
					}
					else	{
						channelHeaderCount = 0;
						if (channelNumber == 7)	{
							pData = 0;
						}
						else if (channelNumber == 15)	{
							pData = 0;
							DB[sampleNumber].first = typePacket;
							isTypePacketActive = false;
							typePacket = -1;
//							time.push_back(std::pair<int, std::vector<uint32_t>>(sampleNumber, timeFirst));
							time[sampleNumber] = timeFirst;
							sampleNumber++;
							timeFirst.clear();
							timeSecond.clear();
						}
					}
					continue;
				}
				channelHeaderCount++;
			}
			else	{
				if ((channelDataCount % 2) == 0)	{
					DB[sampleNumber].second[channelNumber][channelDataCount / 2] = ((uint8_t)((buf)[i]));
				}
				else
					DB[sampleNumber].second[channelNumber][channelDataCount / 2] += (uint32_t)(((buf)[i]) << 8);

				if (channelDataCount == (channelLength - 1))	{
					if (channelNumber == 7)	{
						pData = 0;
					}
					else if (channelNumber == 15)	{
							pData = 0;
							sampleNumber++;
						}
						else	{
							channelHeader = 1;
							channelHeaderCount = 0;
							continue;
						}
				}
				channelDataCount++;
			}
		}
	}

	std::vector<std::vector<int16_t>>	moveVector;
	moveVector.resize(16);
	for (int i = 0; i < (int)moveVector.size(); i++)
		moveVector[i].resize(1024);

	for (int i = 1, ii = 0; i < sampleNumber; i++, ii++)	{
		moveVector.clear();

		moveVector.resize(16);
		for (int iii = 0; iii < (int)moveVector.size(); iii++)
			moveVector[iii].resize(1024);

		int right = 0;
		for (int j = 0; j < 16; j++)
			for (int k = 0; k < 1024; k++)	{
				if (DB[i].second[j][k] == 0)
					right++;
			}
		if (right == 16384)
			continue;

		for (int j = 0; j < 16; j++)
			for (int k = 0; k < 1024; k++)
				moveVector[j][k] = DB[i].second[j][k] - 2048;

		std::array<uint32_t, 8>	newTime;
		std::copy(time[i].begin(), time[i].end(), newTime.begin());
		data_.push_back(Data{DB[i].first, newTime, moveVector});
	}
}

void	Processing::createFolders(const std::string&	path)	{
	for (int i = 0; i < 4; i++)
		QDir().mkdir(QString::fromStdString(path) + QString("/host_") + QString::number(i));
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			QDir().mkdir(QString::fromStdString(path) + QString("/host_") + QString::number(i) + QString("/cluster_") + QString::number(j));
	std::ofstream	file(pathToSave_ + "/host_0/opticlengths.json");
	file << (json::parse("[]")).dump(4);
	file.close();
}

void	Processing::saveDataHowOldProgram(const std::vector<Data>& parserData, const std::string& path, int numberFile)	{
		if (parserData.size() == 0)
			return;

		std::array<std::ofstream, 4>	files;
		static std::array<unsigned long, 4>		countMeasure = {0};
		static std::array<unsigned long, 4>		numberEvent = {0};
		static std::array<unsigned int, 4>		numbersOfFiles = {0};
		static int32_t countOfTimeOverflows = 0;
		for (int i = 0; i < files.size(); i++)
			files[i].open(path + std::string("/host_0/cluster_") + std::to_string(i) +
						  std::string("/data") + std::to_string(numbersOfFiles[i]) + std::string(".txt"), std::ios_base::app);

		for (int i = 0; i < parserData.size(); i++)	{
			Time rawTime = fromRawTime(parserData[i].time);
			if (oldTime_.hours == 23 && rawTime.hours == 0)
				countOfTimeOverflows++;
			oldTime_ = rawTime;
			rawTime.hours += countOfTimeOverflows * 24;
			Time addedTime = plusTime(rawTime, startedTime_);

			files[parserData[i].typePacket] << rawTime.hours << ":" << rawTime.minutes << ":"
					 << rawTime.seconds << "." << rawTime.milliseconds << "."
					 << rawTime.microseconds << "." << rawTime.ns10	 << "\t\t"
					 << addedTime.hours << ":" << addedTime.minutes << ":" << addedTime.seconds << "."
					 << addedTime.milliseconds << "." << addedTime.microseconds << "." << addedTime.ns10 << "\t\t"
					 << numberEvent[parserData[i].typePacket] << std::endl;

			for (int j = 0; j < 1024; j++)	{
				files[parserData[i].typePacket] << "\t" << j << "\t\t" << parserData[i].data[0][j] << "\t" << parserData[i].data[1][j]
					 << "\t" << parserData[i].data[2][j] << "\t" << parserData[i].data[3][j] << "\t\t";
				files[parserData[i].typePacket] << parserData[i].data[4][j] << "\t" << parserData[i].data[5][j] << "\t" << parserData[i].data[6][j]
					 << "\t" << parserData[i].data[7][j] << std::endl;
			}
			countMeasure[parserData[i].typePacket]++;
			numberEvent[parserData[i].typePacket]++;

			for (int j = 0; j < 4; j++)
				if (countMeasure[j] >= 100)	{
					numbersOfFiles[j]++;
					files[j].close();
					files[j].open(path + std::string("/host_0/cluster_") + std::to_string(j) +
								  std::string("/data") + std::to_string(numbersOfFiles[j]) + std::string(".txt"), std::ios_base::app);
					countMeasure[j] = 0;
				}
		}

		for (int i = 0; i < files.size(); i++)
			files[i].close();
}

Processing::Time Processing::fromRawTime(const std::array<uint32_t, 8> &time)	{
	Time result = {0};
	std::vector<uint16_t>	timeVec;
	for (auto item : time)
		timeVec.push_back(static_cast<uint8_t>(item));
	std::bitset<16 * 4>	allBits;
	for (size_t i = 0; i < time.size(); i++)	{
		std::bitset<8>	bits(time[i]);
		for (size_t j = i * 8, k = 0; k < bits.size(); j++, k++)
			allBits.set(j, bits[k]);
	}

	result.ns10			= sliceAllBits(allBits, 0, 6);
	result.microseconds	= sliceAllBits(allBits, 7, 16);
	result.milliseconds	= sliceAllBits(allBits, 17, 26);
	result.seconds		= sliceAllBits(allBits, 27, 32);
	result.minutes		= sliceAllBits(allBits, 33, 38);
	result.hours		= sliceAllBits(allBits, 39, 43);

	return result;
}

uint32_t Processing::sliceAllBits(const std::bitset<16 * 4> &allBits, int begin, int end)	{
	std::bitset<16>	result;
	for (size_t i = begin, j = 0; i <= end; i++, j++)
		result.set(j, allBits[i]);
	return static_cast<uint32_t>(result.to_ulong());
}

void Processing::setStartedTime()	{
	auto now = std::chrono::high_resolution_clock::now().time_since_epoch();
	auto currentTime = std::chrono::duration_cast<std::chrono::nanoseconds>(now).count();

	startedTime_.hours			= ((currentTime / 1000000000 / 60 / 60) % 24) + 3; // +3 is needed to switch to Moscow Time
	startedTime_.minutes		= (currentTime / 1000000000 / 60) % 60;
	startedTime_.seconds		= (currentTime / 1000000000) % 60;
	startedTime_.milliseconds	= (currentTime / 1000000) % 1000;
	startedTime_.microseconds	= (currentTime / 1000) % 1000;
	startedTime_.ns10			= (currentTime / 10) % 1000;

	saveStartedTime(startedTime_, pathToSave_);
}

Processing::Time Processing::getStartedTime()	const	{
	return startedTime_;
}

void Processing::saveOpticalLength(const std::array<int32_t, 4> &lengths_)	{
	static int writeNumber = 0;
	std::fstream	file;
	json data;
	file.open(pathToSave_ + std::string("/host_0/opticlengths.json"), std::ios_base::in);
	file >> data;
	file.close();

	for (size_t i = 0; i < lengths_.size(); i++)
		data[writeNumber][std::string("link_") + std::to_string(i)] = lengths_[i];

	file.open(pathToSave_ + std::string("/host_0/opticlengths.json"), std::ios_base::out);
	file << data.dump(4);
	file.close();
	writeNumber++;
}

Processing::Time Processing::plusTime(const Time& first, const Time& second)	{
	return fromNanosecondsToTime(fromTimeToNanoseconds(first) + fromTimeToNanoseconds(second));
}

uint64_t Processing::fromTimeToNanoseconds(const Processing::Time &time)	{
	uint64_t result = 0;
	result += (static_cast<uint64_t>(time.ns10) * 10);
	result += (static_cast<uint64_t>(time.microseconds) * 1000);
	result += (static_cast<uint64_t>(time.milliseconds) * 1000000);
	result += (static_cast<uint64_t>(time.seconds) * 1000000000);
	result += (static_cast<uint64_t>(time.minutes) * 1000000000 * 60);
	result += (static_cast<uint64_t>(time.hours) * 1000000000 * 60 * 60);
	return result;
}

Processing::Time Processing::fromNanosecondsToTime(uint64_t time)	{
	Time result = {0};
	result.hours		= static_cast<uint32_t>(time / 1000000000 / 60 / 60);	//!!!!!!! % 24 is absent
	result.minutes		= static_cast<uint32_t>((time / 1000000000 / 60) % 60);
	result.seconds		= static_cast<uint32_t>((time / 1000000000) % 60);
	result.milliseconds	= static_cast<uint32_t>((time / 1000000) % 1000);
	result.microseconds	= static_cast<uint32_t>((time / 1000) % 1000);
	result.ns10			= static_cast<uint32_t>((time / 10) % 1000);
	return result;
}

void Processing::saveStartedTime(const Processing::Time &time, const std::string &path)	{
	json timeJson;
	timeJson["AllIsInNanoseconds"]	= fromTimeToNanoseconds(time);
	timeJson["nanoseconds10"]	= time.ns10;
	timeJson["microseconds"]	= time.microseconds;
	timeJson["milliseconds"]	= time.milliseconds;
	timeJson["seconds"]			= time.seconds;
	timeJson["minutes"]			= time.minutes;
	timeJson["hours"]			= time.hours;

	std::ofstream file(path + "/startedtime.json");
	file << timeJson;
	file.close();
}
