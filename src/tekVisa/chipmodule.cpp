#include "chipmodule.h"

chipModule::chipModule(genModule *tekMod, processing *calculation)	{
	_module = tekMod;
	_calculation = calculation;
	_howChipChannel.first = 0;
	_howChipChannel.second= 1;
	_dataUpdate		= false;
	_groupChannel	= 0;
}

bool chipModule::openSession()	{
	Create_Dir_Files();
	ReadStopTime();
	ReadConfigFile(Thresh, HV);
	for (int i = 0; i < NCLAST * NDET; i++)
		_thresh[i] = Thresh[i];
	char host[] = "192.168.1.89";
	bool status = Connect(host, sizeof(host));
	sleep(2);
	return status;
}

void chipModule::closeSession()	{
	endMainBlock();
	UnConnect();
}

void chipModule::tekModuleActivateChannel(int channel, bool status)	{
	_module->setChannel(channel + 1);
	_module->activateChannel(status);
}

void chipModule::tekModuleSetHighLevel(int channel, int level)	{
	_module->setChannel(channel + 1);
	_module->setHighLevel(level);
}

void chipModule::tekModuleSetLowLevel(int channel, int level)	{
	_module->setChannel(channel + 1);
	_module->setLowLevel(level);
}

void chipModule::setThresh(int numberChipChannel, int codeThresh)	{
	_thresh[numberChipChannel + 12] = codeThresh;
}

void chipModule::mountThresh()	{
	byte Claster;
	for (int i = 0; i < NCLAST * NDET; i++)
		std::cout << _thresh[i] << " ";
	std::cout << std::endl;
	for (int ii=0; ii<NCLASTREADY; ii++)	{
		Claster = ClasterReady[ii];
		printf("Claster=%i\n",Claster+1);
		SetThresholdWithBaseLine ( Claster, _thresh);
	}
	printf("\n");
}

chipModule::int32Vec chipModule::sendOneSignal(int channel, int amp)	{
	std::vector<int> max;
	std::cout << std::endl;
//	setThresh(0, 3);
//	setThresh(1, 3);
//	setThresh(2, 3);
//	setThresh(3, 3);
	for (int i = 0; i < NCLAST * NDET; i++)
		_thresh[i]	= 2300;
	mountThresh();
	for (int i = 0; i < NCLAST * NDET; i++)
		_thresh[i]	= 3;
	mountThresh();
	if (channel == 0)	{
		tekModuleActivateChannel(0, true);
		_module->setLowLevel(-amp);
		tekModuleSetHighLevel(0, 0);
		for (int i = 0; i < 5; i++)
			max = sniffMainBlockThree(nnn, 1);
		for (int i = 0; i < max.size(); i++)
			std::cout << max[i] << " ";
		std::cout << std::endl;
		tekModuleActivateChannel(0, false);
	}

	if (channel == 1)	{
		tekModuleActivateChannel(1, true);
		_module->setLowLevel(0);
		tekModuleSetHighLevel(1, amp);
		for (int i = 0; i < 5; i++)
			max = sniffMainBlockThree(nnn, 1);
		tekModuleActivateChannel(1, false);
	}
	return max;
}

int chipModule::searchAndSetChipChannel()	{
	std::vector<int>	response = sendOneSignal(0, 1500);
	if (response[0] < -50 || response[1] < -50)
	{
		return 0;
	}
	if (response[2] < -50 || response[3] < -50)
	{
		return 1;
	}
	if (response[4] < -50 || response[5] < -50)
	{
		return 2;
	}
	if (response[6] < -50 || response[7] < -50)
	{
		return 3;
	}
	return -1;
}

void chipModule::saveToFileAmp(const string& pathToFile, int chipChannel)	{
	std::fstream saveFile;
	std::vector<std::pair<int, std::array<int, 2>>>	saveVec;
	for (auto item : _calibr)
		saveVec.push_back(item);
	std::reverse(saveVec.begin(), saveVec.end());
	for (auto&	item : saveVec)	{
		item.first =		-item.first;
		item.second[0] =	-item.second[0];
		item.second[1] =	-item.second[1];
	}
	if (_groupChannel	== 0)	{
		saveFile.open(pathToFile, std::ios_base::out);
		std::cout << pathToFile << std::endl;
		for (auto it : saveVec)
			saveFile << it.first << "\t" << it.second[0] << "\t" << it.second[1] << std::endl;
		saveFile.close();
	}
	if (_groupChannel	== 1)	{
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

void chipModule::setSettings(int channel, int ampStart, int ampEnd, int ampStep)	{
	_settings[channel].ampStart	= ampStart;
	_settings[channel].ampEnd	= ampEnd;
	_settings[channel].ampStep	= ampStep;
}

void chipModule::ampCalibration(int ampStep, int threshStep)	{
	std::mutex	block;
	_stopFlag = false;
	_activeMode = chipModule::mode::amp;
	int	ampStart = 125;
	int	ampEnd = 10000;
//	setThresh(0, 3);
//	setThresh(1, 3);
//	setThresh(2, 3);
//	setThresh(3, 3);
	for (int i = 0; i < NCLAST * NDET; i++)
		_thresh[i]	= 3;
	mountThresh();
	block.lock();
	_calibr.clear();
	block.unlock();
	tekModuleActivateChannel(0, true);
	tekModuleSetHighLevel(0, 0);
	tekModuleSetLowLevel(0, -100);
	tekModuleActivateChannel(1, true);
	tekModuleSetHighLevel(1, 0);
	std::cout << _howChipChannel.first << " " << _howChipChannel.second << std::endl;
	tekModuleSetLowLevel(1, -100);
	std::vector<int>	max;
	std::vector<int>	oldMax;
	int countNotZero = 0;
	max	= sniffMainBlockThree(nnn, 1);
	for (int ampGo = -50; ampGo >= -3000; ampGo-=10)	{
		if (_stopFlag == true)
			break;
		tekModuleSetLowLevel(0, ampGo);
		tekModuleSetLowLevel(1, ampGo);
		if (countNotZero > 3)
			while (true)	{
				for (int i = 0; i < 2; i++)
					max	= sniffMainBlockThree(nnn, 1);
				if (max[_howChipChannel.first] < oldMax[_howChipChannel.first] / 2
						&& max[_howChipChannel.second] < oldMax[_howChipChannel.second] / 2)
					break;
			}
		else
			for (int i = 0; i < 2; i++)
				max	= sniffMainBlockThree(nnn, 1);
		if (max[_howChipChannel.first] < -10)
			countNotZero++;
		else
			countNotZero = 0;
		block.lock();
		_calibr[ampGo]	= std::array<int, 2>({max[_howChipChannel.first], max[_howChipChannel.second]});
		block.unlock();
		oldMax = max;
		if (max[_howChipChannel.first] <= -2040 || max[_howChipChannel.second] <= -2040)
			break;
//		std::cout << max[0] << "  " << max[1] << "  " << max[2] << "  " << max[3] <<
//							  " " << max[4] << " " << max[5] << " " << max[6] << " " << max[7] << std::endl;
		_dataUpdate	= true;
//		std::cout << "ampGo: " << ampGo << std::endl;
	}
	tekModuleActivateChannel(0, false);
	tekModuleActivateChannel(1, false);

	saveToFileAmp(_pathToSave + string("amp"), _howChipChannel.first);
	_calculation->computeForOneRecordAmp();
	_calibr.clear();
	_dataUpdate = false;
	_calculation->computeForOneRecordAmp();

	std::thread calibrThresh(&chipModule::threshCalibration, this, threshStep);
	calibrThresh.detach();
}

int chipModule::searchThresh()	{
//	_calibr.clear();
	tekModuleActivateChannel(0, true);
	tekModuleSetLowLevel(0, -50);
	tekModuleSetHighLevel(0, 0);
	std::vector<std::array<int, 2>>	check;
	for (int ampGo = -50; ampGo >= -3000; ampGo -= 10)	{
		if (_stopFlag == true)
			break;
		tekModuleSetLowLevel(0, ampGo);
		std::vector<int> max = sniffMainBlockThree(nnn, 1);
		std::cout << max[0] << "  " << max[1] << "  " << max[2] << "  " << max[3] << std::endl;
		if (max[_howChipChannel.first] != 0 || max[_howChipChannel.second] != 0)
			check.push_back(std::array<int, 2>({max[_howChipChannel.first], max[_howChipChannel.second]}));
		else
			check.clear();
		if (check.size() == 3)	{
			int	countGood = 0;
			int shift = 0;
			while (countGood < 5)	{
				tekModuleSetLowLevel(0, ampGo + 30 - shift);
				for (int i = 0; i < 10; i++)	{
					max = sniffMainBlockThree(nnn, 1);
					if (max[_howChipChannel.first] != 0)
						countGood++;
				}
				shift++;
				std::cout << "Shift:" << shift << std::endl;
			}
			return (ampGo + 30 - shift);
		}
		std::cout << "ampGo: " << ampGo << std::endl;
	}
	tekModuleActivateChannel(0, false);
	return -1;
}

void chipModule::threshCalibration(int step)	{
	_stopFlag = false;
	_dataUpdate	= false;
	_threshList.clear();
	_activeMode = chipModule::mode::form;
	for (int threshGo = 25; threshGo <= 225; threshGo += step)	{
		if (_stopFlag == true)
			break;
		for (int i = 0; i < NCLAST * NDET; i++)
			_thresh[i]	= threshGo;
		mountThresh();
		int	ampThresh	= searchThresh();
		_threshList.push_back(std::pair<int, int>(threshGo, ampThresh));
//		std::cout << threshGo << " " << ampThresh << std::endl;
		_dataUpdate = true;
	}
	tekModuleActivateChannel(0, false);

	saveToFileThresh(_pathToSave + string("thresh"), _howChipChannel.first);
	_calculation->computeForOneRecordForm();
}

void chipModule::saveToFileThresh(const string& pathToFile, int chipChannel)	{
	std::fstream saveFile;
	std::vector<std::pair<int, int>>	saveVec;
	for (auto item : _threshList)
		saveVec.push_back(item);
	for (auto&	item : saveVec)	{
		item.first	= item.first;
		item.second	= -item.second;
	}
	if (_groupChannel	== 0)	{
		saveFile.open(pathToFile, std::ios_base::out);
		for (auto it : saveVec)
			saveFile << it.first << "\t" << it.second << std::endl;
		saveFile.close();
	}
	if (_groupChannel	== 1)	{
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

chipModule::mode chipModule::returnMode()	const	{
	return _activeMode;
}

std::pair<int, std::array<int, 2>> chipModule::returnLastAmp()	const	{
	return std::pair<int, std::array<int, 2>>(_calibr.cbegin()->first, _calibr.cbegin()->second);
}

chipModule::int32Pair	chipModule::returnLastThresh()	const	{
	if (_threshList.size() == 0)
		return std::pair<int, int>(-100, -100);
	else
		return _threshList.back();
}

void chipModule::stopCalibration()	{
	_stopFlag = true;
}

void chipModule::calibration(int stepAmp, int stepThresh)	{
	std::thread calibrAmp(&chipModule::ampCalibration, this, stepAmp, stepThresh);
	calibrAmp.detach();
}

chipModule::vecint32Pair chipModule::searchAiming(int chipChannel, int amp)	{
	_calibr.clear();
	int chipV4channel;
	chipModule::vecint32Pair	deviation;
	if (chipChannel == 0)
		chipV4channel = 0;
	if (chipChannel == 1)
		chipV4channel = 2;
	tekModuleActivateChannel(0, true);
	_module->setLowLevel(0);
	tekModuleSetHighLevel(0, amp);
	for (int i = 0; i < 100; i++)	{
		std::vector<int> max = sniffMainBlockThree(nnn, 1);
		for (int j = 0; j < 4; j++)
			if (deviation.first[j] < max[j])
				deviation.first[j] = max[j];
	}
	tekModuleActivateChannel(0, false);
	deviation.first[chipV4channel] = -1000;

	if (chipChannel == 0)
		chipV4channel = 1;
	if (chipChannel == 1)
		chipV4channel = 3;
	tekModuleActivateChannel(1, true);
	_module->setLowLevel(0);
	tekModuleSetHighLevel(1, amp);
	for (int i = 0; i < 100; i++)	{
		std::vector<int> max = sniffMainBlockThree(nnn, 1);
		for (int j = 0; j < 4; j++)
			if (deviation.second[j] < max[j])
				deviation.second[j] = max[j];
	}
	tekModuleActivateChannel(1, false);
	deviation.second[chipV4channel] = -1000;
	return deviation;
}

void	chipModule::setDataUpdateFalse()	{
	_dataUpdate	= false;
}

void	chipModule::setGroupChannel(int	channel)	{
	_groupChannel	= channel;
}

void	chipModule::setPathToSave(const string &pathToSave)	{
	_pathToSave	= pathToSave;
}

bool	chipModule::getDataUpdate()	const	{
	return _dataUpdate;
}

void	chipModule::setChannels(const int32Pair &channel)	{
	_howChipChannel	= channel;
}
