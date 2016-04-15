#include "paamodule.h"

PaaModule::PaaModule(const std::string& ip)	{
	host_		= std::make_shared<Host>(ip);
	master_		= host_->addMaster(0);
	adc_		= master_->addAdc(0);
	clientQueue_= host_->getQueue();
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
