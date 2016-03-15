#include "adc.h"
#include <iostream>

Adc::Adc(uint32_t	offset, QueuePtr clientQueue)
	:	clientQueue_(clientQueue)	{

	offset_	= offset + 0x10000;
	clientQueue_->attach(this);
}

Adc::~Adc()	{
//	clientQueue_->detach(this);
}

void	Adc::update(const Subject *subject)	{
	if (subject == clientQueue_.get())	{
		if (clientQueue_->getMessage() == QueueOfMessages::Message::recordRead)	{
			Record record = clientQueue_->getRecord();
			std::cout << record.address << "\t" << record.value << std::endl;
			for (auto reg : registers_.readRecords)
				if (reg->address == record.address - offset_)
					reg->value = record.value;
		}

		if (clientQueue_->getMessage() == QueueOfMessages::Message::recordWrite)	{
			Record record = clientQueue_->getRecord();
			std::cout << record.address << "\t" << record.value << std::endl;
		}

	}
}

void	Adc::readState()	{
	for (auto record : registers_.readRecords)	{
		Record	recordWithOffset(*record);
		recordWithOffset.address += offset_;
		clientQueue_->addCommandToStack(recordWithOffset, shared_from_this());
	}
}

void Adc::writeControlRegister()	{
	Record	recordWithOffset(registers_.write.control_register);
	recordWithOffset.address += offset_;
	recordWithOffset.value	= 0;
	clientQueue_->addCommandToStack(recordWithOffset, shared_from_this());
}

void Adc::writeThreshold(uint32_t channel, uint16_t threshold)	{
	Record	recordWithValue;

	if (channel == 0)
		recordWithValue = registers_.write.first_threshold;
	if (channel == 1)
		recordWithValue = registers_.write.second_threshold;

	recordWithValue.address += offset_;
	recordWithValue.value	= threshold;
	clientQueue_->addCommandToStack(recordWithValue, shared_from_this());
}

void Adc::writeDelay(uint32_t channel, uint16_t delay)	{
	Record	recordWithValue;

	if (channel == 0)
		recordWithValue = registers_.write.first_delay;
	if (channel == 1)
		recordWithValue = registers_.write.second_delay;

	recordWithValue.address += offset_;
	recordWithValue.value	= delay;
	clientQueue_->addCommandToStack(recordWithValue, shared_from_this());
}

void Adc::writeResetChannel(uint32_t channel)	{
	Record	recordWithValue;

	if (channel == 0)
		recordWithValue = registers_.write.reset_first_channel;
	if (channel == 1)
		recordWithValue = registers_.write.reset_second_channel;

	recordWithValue.address += offset_;
	recordWithValue.value	= 1;
	clientQueue_->addCommandToStack(recordWithValue, shared_from_this());
}

void Adc::writeSmoothing(uint16_t smoothing)	{
	Record	recordWithValue(registers_.write.smoothing);
	recordWithValue.address	+= offset_;
	recordWithValue.value	= smoothing;
	clientQueue_->addCommandToStack(recordWithValue, shared_from_this());
}
