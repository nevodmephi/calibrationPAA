#include "paamodule.h"

PaaModule::PaaModule(const std::string& ip)	{
	host_		= std::make_shared<Host>(ip);
	master_		= host_->addMaster(0);
	adc_		= master_->addAdc(0);
	clientQueue_= host_->getQueue();
	processing_	= std::make_shared<Processing>(clientQueue_);
}

PaaModule::~PaaModule()	{

}

void	PaaModule::update(const Subject *subject)	{
	if (subject == clientQueue_.get())
		if (clientQueue_->getMessage() == QueueOfMessages::Message::dataRead)	{
//			static int packetCount = 0;
			data_.insert(data_.end(), clientQueue_->getData().begin(), clientQueue_->getData().end());
//			packetCount++;
		}
}

bool	PaaModule::openSession()	{
	host_->connectToHost();
	return true;
}

void	PaaModule::closeSession()	{
	host_->disconnectFromHost();
}

void PaaModule::setPathToSave(const std::string &pathToSave)	{
	processing_->setPathToSave(pathToSave);
	processing_->createFolders(pathToSave);
}

void PaaModule::initializeTable(uint32_t numberMaster)	{
	host_->initializeTable(numberMaster);
}

void PaaModule::writeRegister(uint32_t address, uint32_t data, Record::Type type)	{
	host_->writeRegister(address, data, type);
}
