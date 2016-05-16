#include "master.h"
#include <iostream>

Master::Master(uint32_t	offset, QueuePtr clientQueue)
	:	clientQueue_(clientQueue)	{

	offset_		= offset + 0x200000;
	offsetLink_	= offset;
	clientQueue_->attach(this);
}

Master::~Master()	{
//	clientQueue_->detach(this);
}

void	Master::update(const Subject *subject)	{
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

Master::AdcPtr	Master::addAdc(uint32_t numberAdc)	{
	AdcPtr	adc = std::make_shared<Adc>(offsetLink_ + 0x80000 + 0x20000 * numberAdc, clientQueue_);
	adcs_.insert(adcs_.begin() + numberAdc, adc);
	return adc;
}

void	Master::deleteAdc(uint32_t numberAdc)	{
	adcs_.erase(adcs_.begin() + numberAdc);
}

void	Master::readState()	{
	for (auto record : registers_.readRecords)	{
		Record	recordWithOffset(*record);
		recordWithOffset.address += offset_;
		clientQueue_->addCommandToStack(recordWithOffset, shared_from_this());
	}
}

void Master::writeResetRegister()	{
	Record	recordWithValue(registers_.write.reset_register);
	recordWithValue.address += offset_;
	recordWithValue.value = 3;
	clientQueue_->addCommandToStack(recordWithValue, shared_from_this());
}

void Master::writeResolutionOfTimer(bool resolution)	{
	if (resolution == true)	{
		Record	recordWithValue(registers_.write.resolution_of_timer);
		recordWithValue.address += offset_;
		recordWithValue.value = 1;
		clientQueue_->addCommandToStack(recordWithValue, shared_from_this());
	}
}

void Master::writeTestRegister(uint16_t data)	{
	Record	recordWithValue(registers_.write.test_register);
	recordWithValue.address += offset_;
	recordWithValue.value = data;
	clientQueue_->addCommandToStack(recordWithValue, shared_from_this());
}

void Master::writeCoincidence(uint32_t coincidence)	{
	Record	recordWithValue(registers_.write.coincidende);
	recordWithValue.address += offset_;
	switch (coincidence) {
		case 0:	recordWithValue.value = 0;
			break;
		case 1:	recordWithValue.value = 1;
			break;
		case 2:	recordWithValue.value = 3;
			break;
		case 3:	recordWithValue.value = 7;
			break;
		case 4:	recordWithValue.value = 15;
			break;
	}
	clientQueue_->addCommandToStack(recordWithValue, shared_from_this());
}

void Master::writeResetCounter()	{
	Record	recordWithValue(registers_.write.reset_counter);
	recordWithValue.address += offset_;
	recordWithValue.value	= 1;
	clientQueue_->addCommandToStack(recordWithValue, shared_from_this());
}

void Master::writeTemporaryWindow(uint16_t window)	{
	Record	recordWithValue(registers_.write.temporary_window);
	recordWithValue.address += offset_;
	recordWithValue.value	= window;
	clientQueue_->addCommandToStack(recordWithValue, shared_from_this());
}

void Master::writeChErrorCount(uint16_t data)	{
	Record	recordWithValue(registers_.write.ch_error_count);
	recordWithValue.address += offset_;
	recordWithValue.value	= data;
	clientQueue_->addCommandToStack(recordWithValue, shared_from_this());
}

void Master::writeControlTrigger(int i)	{
	Record	recordWithValue(registers_.write.control_trigger);
	recordWithValue.address += offset_;
	if (i == 0)
		recordWithValue.value = 0;
	if (i == 1)
		recordWithValue.value = 1;
	if (i == 2)
		recordWithValue.value = 3;
	clientQueue_->addCommandToStack(recordWithValue, shared_from_this());
}

void Master::writeProgramTrigger(bool trigger)	{
	if (trigger == true)	{
		Record	recordWithValue(registers_.write.program_trigger);
		recordWithValue.address += offset_;
		recordWithValue.value	= 1;
		clientQueue_->addCommandToStack(recordWithValue, shared_from_this());
	}
}

void Master::writeResetVme()	{
	Record	recordWithValue(registers_.write.reset_vme);
	recordWithValue.address += offset_;
	recordWithValue.value	= 1;
	clientQueue_->addCommandToStack(recordWithValue, shared_from_this());
}

void Master::writeMaskOfActiveChannels(const std::array<bool, 8>& activeChannels)	{
	Record	recordWithValue(registers_.write.mask_of_active_channels);
	recordWithValue.address += offset_;
	recordWithValue.value	= 0;
	for (int i = 0; i < activeChannels.size(); i++)
		if (activeChannels[i] == true)
			recordWithValue.value += (1 << i);
	clientQueue_->addCommandToStack(recordWithValue, shared_from_this());
}

void Master::writeTime(uint16_t	hours, uint16_t	minutes, uint16_t	seconds, uint16_t	ms)	{
	std::array<Record, 4>	recordsWithValues;
	recordsWithValues[0]	= registers_.write.time_first;
	recordsWithValues[0]	= registers_.write.time_second;
	recordsWithValues[0]	= registers_.write.time_third;
	recordsWithValues[0]	= registers_.write.time_fourth;
	for (auto record : recordsWithValues)	{
		record.address	+= offset_;
		record.value	= 0x34;
	}
	for (auto record: recordsWithValues)	{
		clientQueue_->addCommandToStack(record, shared_from_this());
	}
}
