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
//	clientQueue_->detach(this);
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

//			QFile file(QString("/home/main/data") + QString::number(number));
//			file.open(QIODevice::WriteOnly | QIODevice::Append);
//			file.write(reinterpret_cast<const char*>(buffer_.front().data()));
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

void	Processing::parse(const std::vector<unsigned char>& data)	{
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

		int		typePacket	= -1;
		bool	isTypePacketActive = false;

		std::vector<std::pair<int, std::vector<std::vector<uint32_t>>>>	DB;
		std::vector<uint32_t>	time;
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
						channelLength = (int32_t)(buf)[i];
					if (channelHeaderCount == 5)	{
						channelLength += ((int32_t)((buf)[i] & 0xff) * 256);
						channelLength *= 2;
					}
					if (channelHeaderCount == 6)	{
						channelNumber = (int32_t)((buf)[i] & 0xf);
					}
					if (channelHeaderCount == 7)	{
						if (channelLength != 0)	{	//!= 0
							channelHeader = 0;
							channelDataCount = 0;
						}
						if (channelLength == 8)	{
							for (int j = i - 2; j < i + 37; j++)
								time.push_back((uint8_t)buf[j]);
							int a = 15;
							a--;
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
								typePacket = -1;	//TODO It will be crash!!!!!
								sampleNumber++;
								time.clear();
							}
						}
						continue;
					}
//					if (channelHeaderCount >= 21 && channelHeaderCount < 37)	{
//						time.push_back((uint8_t)buf[i]);
//					}
//					if (channelHeaderCount == 37)	{
//						int a = 15;
//						a--;
//					}
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

			data_.push_back(std::pair<int, std::vector<std::vector<int16_t>>>(DB[i].first, moveVector));
		}
}

void	Processing::createFolders(const std::string&	path)	{
	for (int i = 0; i < 4; i++)
		QDir().mkdir(QString::fromStdString(path) + QString("/host_") + QString::number(i));
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			QDir().mkdir(QString::fromStdString(path) + QString("/host_") + QString::number(i) + QString("/cluster_") + QString::number(j));
}

void	Processing::saveDataHowOldProgram(const std::vector<std::pair<int, std::vector<std::vector<int16_t> > > >& parserData, const std::string& path, int numberFile)	{
//	std::vector<std::vector<int16_t> > > >& parserData, const std::string& path, int numberFile)	{
		if (parserData.size() == 0)
			return;
		std::array<std::ofstream, 4>	files;
		static std::array<unsigned long, 4>		countMeasure = {0};
		static std::array<unsigned long, 4>		numberEvent = {0};
		static std::array<unsigned int, 4>		numbersOfFiles = {0};
		for (int i = 0; i < files.size(); i++)
			files[i].open(path + std::string("/host_0/cluster_") + std::to_string(i) +
						  std::string("/data") + std::to_string(numbersOfFiles[i]) + std::string(".txt"), std::ios_base::app);

		for (int i = 0; i < parserData.size(); i++)	{
			QTime currentTime	= QTime::currentTime();
			files[parserData[i].first] << currentTime.hour() << ":" << currentTime.minute() << ":"
					 << currentTime.second() << "." << currentTime.msec() << "\t\t"
					 << numberEvent[parserData[i].first] << std::endl;
			for (int j = 0; j < 1024; j++)	{
				files[parserData[i].first] << "\t" << j << "\t\t" << parserData[i].second[0][j] << "\t" << parserData[i].second[1][j]
					 << "\t" << parserData[i].second[2][j] << "\t" << parserData[i].second[3][j] << "\t\t";
				files[parserData[i].first] << parserData[i].second[4][j] << "\t" << parserData[i].second[5][j] << "\t" << parserData[i].second[6][j]
					 << "\t" << parserData[i].second[7][j] << std::endl;
			}
			countMeasure[parserData[i].first]++;
			numberEvent[parserData[i].first]++;

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
	//	for (int i = 0; i < parserData.size(); i++)	{
	//		std::ofstream	file;
	//		file.open(path + std::string("/host_0/cluster_") + std::to_string(parserData[i].first) +
	//				  std::string("/data") + std::to_string(numberFile) + std::string("__") + std::to_string(countFile) + std::string(".txt"));
	//			QTime startTime		= QTime(10, 12, 23, 34);
	//			QTime currentTime	= QTime::currentTime();
	//			file << currentTime.hour() << ":" << currentTime.minute() << ":"
	//				 << currentTime.second() << "." << currentTime.msec() << "\t\t"
	//				 << countMeasure << "\t\t" << countMeasure % 2 << "\t" << startTime.hour() << std::endl;
	//			for (int j = 0; j < 1024; j++)	{
	//				file << "\t" << j << "\t\t" << parserData[i].second[0][j] << "\t" << parserData[i].second[1][j]
	//					 << "\t" << parserData[i].second[2][j] << "\t" << parserData[i].second[3][j] << "\t\t";
	//				file << parserData[i].second[4][j] << "\t" << parserData[i].second[5][j] << "\t" << parserData[i].second[6][j]
	//					 << "\t" << parserData[i].second[7][j] << std::endl;
	//			}
	//			countMeasure++;
	//		file.close();
	//		countFile++;
	//	}}
}
