#include "calibration.h"

Calibration::Calibration(const std::string &ip, const std::string &resName)	{
	genModule_		= std::make_shared<GenModule>(resName);
	calculation_	= std::make_shared<Calculation>();
	paaModule_		= std::make_shared<PaaModule>(ip);
}

Calibration::~Calibration()	{

}

std::pair<bool, bool> Calibration::openSessions()	{
	return std::pair<bool, bool>(genModule_->openSession(), paaModule_->openSession());
}

void Calibration::closeSessions()	{
	genModule_->closeSession();
	paaModule_->closeSession();
}

void Calibration::calibrateAmplitude()	{

}

void Calibration::calibrateThreshold()	{

}

void Calibration::stopCalibration()	{

}

void Calibration::setChannels(const std::pair<int, int> &channel)	{
	howChipChannel_ = channel;
}

void Calibration::setGroupChannel(int channel)	{

}

void Calibration::setPathToSave(const std::string &pathToSave)	{
	pathToSave_ = pathToSave;
	paaModule_->setPathToSave(pathToSave);
}

void Calibration::readAndSaveInformation()	{

}

void Calibration::searchThreshold()	{

}

void Calibration::setThreshold(int numberChipChannel, int codeThresh)	{
//	paaModule_->setThresh(numberChipChannel, codeThresh);
}

std::shared_ptr<GenModule> Calibration::getGenModule() const	{
	return	genModule_;
}

std::shared_ptr<PaaModule> Calibration::getPaaModule() const	{
	return	paaModule_;
}

void Calibration::writeRegister(uint32_t address, uint32_t data, Record::Type type)	{
	paaModule_->writeRegister(address, data, type);
}

void Calibration::initializeTable(uint32_t numberMaster)	{
	paaModule_->initializeTable(numberMaster);
}

void Calibration::stop(int channel)	{
	tekModuleActivateChannel(channel, false);
}

void Calibration::tekModuleActivateChannel(int channel, bool status)	{
	genModule_->setChannel(channel + 1);
	genModule_->activateChannel(status);
}

void Calibration::tekModuleSetHighLevel(int channel, int level)	{
	genModule_->setChannel(channel + 1);
	genModule_->setHighLevel(level);
}

void Calibration::tekModuleSetLowLevel(int channel, int level)	{
	genModule_->setChannel(channel + 1);
	genModule_->setLowLevel(level);
}

void Calibration::sendOneSignal(int channel, int amp)	{
	tekModuleActivateChannel(channel, true);
	genModule_->setInterval(100);
	genModule_->setLowLevel(-amp);
	tekModuleSetHighLevel(channel, 0);
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	tekModuleActivateChannel(channel, false);
}
