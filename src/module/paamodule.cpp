#include "paamodule.h"

PaaModule::PaaModule(GenModulePtr tekMod, CalculationPtr calculation, const std::string& ip)
	:	module_(tekMod), calculation_(calculation)	{

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
