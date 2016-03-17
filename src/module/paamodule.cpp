#include "paamodule.h"

PaaModule::PaaModule(GenModule *tekMod, Processing *calculation, const std::string& ip)
	:	module_(tekMod), calculation_(calculation)	{

	howChipChannel_.first = 0;
	howChipChannel_.second = 1;
	dataUpdate_		= false;
	groupChannel_	= 0;

	host_	= std::make_shared<Host>(ip);
	master_ = host_->addMaster(0);
	adc_	= master_->addAdc(0);
	clientQueue_ = host_->getQueue();
}

PaaModule::~PaaModule()	{

}

void	PaaModule::update(const Subject *subject)	{
	if (subject == clientQueue_.get())
		if (clientQueue_->getMessage() == QueueOfMessages::Message::dataRead)	{
			static int packetCount = 0;
			data_.insert(data_.end(), clientQueue_->getData().begin(), clientQueue_->getData().end());
			packetCount++;
//			if (packetCount == )
//				data_.clear();
		}
}

bool	PaaModule::openSession()	{
	host_->connectToHost();
	return true;
}

void	PaaModule::closeSession()	{
	host_->disconnectFromHost();
}

void	PaaModule::tekModuleActivateChannel(int channel, bool status)	{
	module_->setChannel(channel + 1);
	module_->activateChannel(status);
}

void	PaaModule::tekModuleSetHighLevel(int channel, int level)	{
	module_->setChannel(channel + 1);
	module_->setHighLevel(level);
}

void	PaaModule::tekModuleSetLowLevel(int channel, int level)	{
	module_->setChannel(channel + 1);
	module_->setLowLevel(level);
}

void	PaaModule::setThresh(int numberChipChannel, int codeThresh)	{
	//WARNING
	adc_->writeThreshold(numberChipChannel, codeThresh);
}

void	PaaModule::mountThresh()	{

}

std::vector<int>	PaaModule::sendOneSignal(int channel, int amp)	{
	//TODO
	setThresh(0, 2300);
	setThresh(1, 2300);
	if (channel == 0)	{
		tekModuleActivateChannel(0, true);
		module_->setLowLevel(-amp);
		tekModuleSetHighLevel(0, 0);
//		for (int i = 0; i < 5; i++)
//			max = sniffMainBlockThree(nnn, 1);
//		for (int i = 0; i < max.size(); i++)
//			std::cout << max[i] << " ";
//		std::cout << std::endl;
		tekModuleActivateChannel(0, false);
	}

	if (channel == 1)	{
		tekModuleActivateChannel(1, true);
		module_->setLowLevel(0);
		tekModuleSetHighLevel(1, amp);
//		for (int i = 0; i < 5; i++)
//			max = sniffMainBlockThree(nnn, 1);
		tekModuleActivateChannel(1, false);
	}
//	return ;
}

int	PaaModule::searchAndSetChipChannel()	{
//	std::vector<int>	response = sendOneSignal(0, 1500);
//	if (response[0] < -50 || response[1] < -50)
//	{
//		return 0;
//	}
//	if (response[2] < -50 || response[3] < -50)
//	{
//		return 1;
//	}
//	if (response[4] < -50 || response[5] < -50)
//	{
//		return 2;
//	}
//	if (response[6] < -50 || response[7] < -50)
//	{
//		return 3;
//	}
//	return -1;
}

void	PaaModule::saveToFileAmp(const std::string &pathToFile, int chipChannel)	{
	std::fstream saveFile;
	std::vector<std::pair<int, std::array<int, 2>>>	saveVec;
	for (auto item : calibr_)
		saveVec.push_back(item);
	std::reverse(saveVec.begin(), saveVec.end());
	for (auto&	item : saveVec)	{
		item.first =		-item.first;
		item.second[0] =	-item.second[0];
		item.second[1] =	-item.second[1];
	}
	if (groupChannel_	== 0)	{
		saveFile.open(pathToFile, std::ios_base::out);
		std::cout << pathToFile << std::endl;
		for (auto it : saveVec)
			saveFile << it.first << "\t" << it.second[0] << "\t" << it.second[1] << std::endl;
		saveFile.close();
	}
	if (groupChannel_	== 1)	{
		std::vector<std::array<int, 3>>	readAmp;
		int	read1, read2, read3;
		saveFile.open(pathToFile, std::ios_base::in);
		while (!saveFile.eof())	{
			saveFile >> read1 >> read2 >> read3;
			readAmp.push_back(std::array<int, 3>({read1, read2, read3}));
		}
		saveFile.close();
		saveFile.open(pathToFile, std::ios_base::out);
		int count = 0;
		for (auto it : saveVec)	{
			saveFile << readAmp[count][0] << "\t" << readAmp[count][1] << "\t" << readAmp[count][2]
					 << "\t" << it.second[0] << "\t" << it.second[1] << std::endl;
			count++;
		}
		saveFile.close();
	}
}

void	PaaModule::setSettings(int channel, int ampStart, int ampEnd, int ampStep)	{
	settings_[channel].ampStart	= ampStart;
	settings_[channel].ampEnd	= ampEnd;
	settings_[channel].ampStep	= ampStep;
}

void	PaaModule::ampCalibration(int ampStep, int threshStep)	{

}

int	PaaModule::searchThresh()	{

}

void	PaaModule::threshCalibration(int step)	{

}

void	PaaModule::saveToFileThresh(const std::string &pathToFile, int chipChannel)	{
	std::fstream saveFile;
	std::vector<std::pair<int, int>>	saveVec;
	for (auto item : threshList_)
		saveVec.push_back(item);
	for (auto&	item : saveVec)	{
		item.first	= item.first;
		item.second	= -item.second;
	}
	if (groupChannel_	== 0)	{
		saveFile.open(pathToFile, std::ios_base::out);
		for (auto it : saveVec)
			saveFile << it.first << "\t" << it.second << std::endl;
		saveFile.close();
	}
	if (groupChannel_	== 1)	{
		saveFile.open(pathToFile, std::ios_base::in);
		std::vector<std::pair<int, int>>	readThresh;
		int	read1, read2;
		while (!saveFile.eof())	{
			saveFile >> read1 >> read2;
			readThresh.push_back(std::pair<int, int>(read1, read2));
		}
		saveFile.close();
		saveFile.open(pathToFile, std::ios_base::out);
		int count = 0;
		for (auto it : saveVec)	{
			saveFile << readThresh[count].first << "\t" << readThresh[count].second << "\t" << it.second << std::endl;
			count++;
		}
		saveFile.close();
	}
	saveFile.close();
}

PaaModule::mode PaaModule::returnMode()	const	{
	return activeMode_;
}

std::pair<int, std::array<int, 2>> PaaModule::returnLastAmp()	const	{
	return std::pair<int, std::array<int, 2>>(calibr_.cbegin()->first, calibr_.cbegin()->second);
}

std::pair<int, int>	PaaModule::returnLastThresh()	const	{
	if (threshList_.size() == 0)
		return std::pair<int, int>(-100, -100);
	else
		return threshList_.back();
}

void PaaModule::stopCalibration()	{
	stopFlag_ = true;
}

void PaaModule::calibration(int stepAmp, int stepThresh)	{
	std::thread calibrAmp(&PaaModule::ampCalibration, this, stepAmp, stepThresh);
	calibrAmp.detach();
}

void	PaaModule::setDataUpdateFalse()	{
	dataUpdate_	= false;
}

void	PaaModule::setGroupChannel(int	channel)	{
	groupChannel_	= channel;
}

void	PaaModule::setPathToSave(const std::string &pathToSave)	{
	pathToSave_	= pathToSave;
}

bool	PaaModule::getDataUpdate()	const	{
	return dataUpdate_;
}

void	PaaModule::setChannels(const std::pair<int, int> &channel)	{
	howChipChannel_	= channel;
}
