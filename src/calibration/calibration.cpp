#include "calibration.h"

Calibration::Calibration(const std::string &ip, const std::string &resName)	{
//	:	genModule_(genModule), paaModule_(paaModule)	{
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
