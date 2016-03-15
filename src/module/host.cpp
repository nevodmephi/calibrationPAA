#include "host.h"
#include <iostream>
#include <fstream>

Host::Host(const std::string& ip)	{
	clientQueue_ = std::make_shared<QueueOfMessages>(ip);

	clientQueue_->attach(this);
	offset_ = 0x400000;
}

Host::~Host()	{
	clientQueue_.reset();
//	clientQueue_->detach(this);
}

void	Host::update(const Subject *subject)	{
	if (subject == clientQueue_.get())	{
		if (clientQueue_->getMessage() == QueueOfMessages::Message::recordRead)	{
			Record record = clientQueue_->getRecord();
			std::cout << record.value << std::endl;

//			std::fstream	file;
//			file.open("/home/main/data", std::ios_base::app);
//			file << record.value << std::endl;
//			file.close();

			for (auto reg : registers_.readRecords)
				if (reg->address == record.address - offset_)
					reg->value = record.value;

			resultRecord_ = record;
			resultRecord_.address -= offset_;		//!!!!
			notify();
		}

		if (clientQueue_->getMessage() == QueueOfMessages::Message::recordWrite)	{
			Record record = clientQueue_->getRecord();
			resultRecord_	= record;
			std::cout << record.value << std::endl;
			notify();
		}

		if (clientQueue_->getMessage() == QueueOfMessages::Message::connected)	{
			std::cout << "Connected!" << std::endl;
		}

		if (clientQueue_->getMessage() == QueueOfMessages::Message::disconnected)	{
			std::cout << "Disconnected!" << std::endl;
		}

		if (clientQueue_->getMessage() == QueueOfMessages::Message::error)	{
			std::cout << "Error!" << std::endl;
		}
	}
}

void	Host::connectToHost()	{
	clientQueue_->connectToHost();
}

void	Host::disconnectFromHost()	{
	clientQueue_->disconnectFromHost();
}

Host::MasterPtr	Host::addMaster(uint32_t numberMaster)	{
	MasterPtr	master = std::make_shared<Master>(0x1000000 * numberMaster, clientQueue_);
	masters_.insert(std::pair<uint32_t, MasterPtr>(numberMaster, master));
	return master;
}

void	Host::deleteMaster(uint32_t numberMaster)	{
	masters_.erase(numberMaster);
}

void	Host::readState()	{
	for (auto record : registers_.readRecords)
		clientQueue_->addCommandToStack(getRecordWithOffset(*record), shared_from_this());
}

void	Host::runQueue()	{
	clientQueue_->runStack();
}

void Host::writeBanOfTimestamps(bool ban)	{
	Record	recordWithValue(registers_.write.ban_of_timestamps);
	recordWithValue.address += offset_;

	if (ban == true)
		recordWithValue.value	= 1;
	else
		recordWithValue.value	= 0;

	clientQueue_->addCommandToStack(recordWithValue, shared_from_this());
}

void Host::writeTestRegister(uint16_t data)	{
	Record	recordWithValue(registers_.write.test_register);
	recordWithValue.address += offset_;
	recordWithValue.value	= data;
	clientQueue_->addCommandToStack(recordWithValue, shared_from_this());
}

void Host::writeCoarseReset(const std::array<bool, 4>& resetLink)	{
	Record	recordWithValue(registers_.write.coarse_reset);
	recordWithValue.address += offset_;
	recordWithValue.value = 0;
	for (int i = 0; i < resetLink.size(); i++)
		if (resetLink[i] == true)
			recordWithValue.value += (1 << i);
	clientQueue_->addCommandToStack(recordWithValue, shared_from_this());
}

void Host::writeResolutionOfLinks(const std::array<bool, 4>& resolutionLink)	{
	Record	recordWithValue(registers_.write.resolution_of_links);
	recordWithValue.address += offset_;
	recordWithValue.value = 0;
	for (int i = 0; i < resolutionLink.size(); i++)
		if (resolutionLink[i] == true)
			recordWithValue.value += (1 << i);
	clientQueue_->addCommandToStack(recordWithValue, shared_from_this());
}

void Host::writeExchangeRegister(uint16_t data)	{
	Record	recordWithValue(registers_.write.exchange_register);
	recordWithValue.address += offset_;
	recordWithValue.value	= data;
	clientQueue_->addCommandToStack(recordWithValue, shared_from_this());
}

void Host::writeControlDma(int oneOnTwoReset)	{
	Record	recordWithValue(registers_.write.control_dma);
	recordWithValue.address += offset_;
	if (oneOnTwoReset == 0)
		recordWithValue.value = 0;
	if (oneOnTwoReset == 1)
		recordWithValue.value = 1;
	clientQueue_->addCommandToStack(recordWithValue, shared_from_this());

}

void Host::writeClusterNumberLink(uint32_t numberLink, uint16_t numberCluster)	{
	Record	recordWithValue;
	switch (numberLink) {
		case 0:	recordWithValue = registers_.write.cluster_number_link_0;
			break;
		case 1:	recordWithValue = registers_.write.cluster_number_link_1;
			break;
		case 2:	recordWithValue = registers_.write.cluster_number_link_2;
			break;
		case 3:	recordWithValue = registers_.write.cluster_number_link_3;
			break;
	}
	recordWithValue.address += offset_;
	recordWithValue.value	= numberCluster;
	clientQueue_->addCommandToStack(recordWithValue, shared_from_this());
}

void Host::writeStartTaskLink(uint32_t numberLink, uint32_t numberBank)	{
//	Record	recordWithValue;
//	switch (numberLink) {
//		case 0:	recordWithValue = registers_.write.start_task_link_0;
//			break;
//		case 1:	recordWithValue = registers_.write.start_task_link_1;
//			break;
//		case 2:	recordWithValue = registers_.write.start_task_link_2;
//			break;
//		case 3:	recordWithValue = registers_.write.start_task_link_3;
//			break;
//	}
//	recordWithValue.address += offset_;
//	recordWithValue.value	= numberBank;
//	clientQueue_->addCommandToStack(recordWithValue, shared_from_this());
}

void Host::writeRegisterOfReadData(const std::array<bool, 4>&	readLink)	{
	Record	recordWithValue(registers_.write.register_of_read_data);
//	recordWithValue.address += offset_;
	recordWithValue.value = 0;
	for (int i = 0; i < readLink.size(); i++)
		if (readLink[i] == true)
			recordWithValue.value += (3 << (i * 2));
	clientQueue_->addCommandToStack(recordWithValue, shared_from_this());
}

void Host::writeResolutionAndForbidOfData(bool resolution)	{
	Record	recordWithValue;
	if (resolution == true)
		recordWithValue = registers_.write.resolution_of_data;
	else
		recordWithValue = registers_.write.forbid_of_data;

//	recordWithValue.address += offset_;
	recordWithValue.value	= 1;
	clientQueue_->addCommandToStack(recordWithValue, shared_from_this());
}

void Host::writeClearDataBuffer(bool clear)	{
	if (clear == true)	{
		Record	recordWithValue(registers_.write.clear_data_buffer);
//		recordWithValue.address += offset_;
		recordWithValue.value = 1;
		clientQueue_->addCommandToStack(recordWithValue, shared_from_this());
	}
}

Record	Host::getRecordWithOffset(Record record)	{
	Record	recordWithOffset(record);
	recordWithOffset.address += offset_;
	return recordWithOffset;
}

void	Host::writeRegister(uint32_t	address, uint32_t	data, Record::Type	type)	{
	Record	record;
	record.address = address;
	record.value = data;
	record.type = type;
	clientQueue_->addCommandToStack(record, shared_from_this());
	clientQueue_->runStack();
}

void	Host::readRegister(uint32_t	address)	{
	Record	record;
	record.address = address;
	record.type	= Record::Type::Read;
	clientQueue_->addCommandToStack(record, shared_from_this());
	clientQueue_->runStack();
}

void	Host::initializeTable(uint32_t numberMaster, const std::vector<uint32_t> &numbersOfChannels)	{
	uint32_t	address = offset_ + 0x10000 + 0x4000 * numberMaster;
//	uint32_t	address = offset_ + 0x1000000 * numberMaster;
	std::vector<uint16_t>	table;
	table.resize(68);
	for (auto channel : numbersOfChannels)	{
		uint32_t	channelOffset = 8 * channel;

		if (channel % 2 == 0)	{
			table[channelOffset]		=	0x0;
			table[channelOffset + 4]	=	0x800;
		}
		else	{
			table[channelOffset]		=	0x4000;
			table[channelOffset + 4]	=	0x4800;
		}

		table[channelOffset + 1]	=	0x8008 + 0x2 * (channel / 2);
		table[channelOffset + 2]	=	1024;
		table[channelOffset + 3]	=	0;
		table[channelOffset + 5]	=	0x8008 + 0x2 * (channel / 2);
		table[channelOffset + 6]	=	1024;
		table[channelOffset + 7]	=	0;
	}

	table[8 * numbersOfChannels.size()]		= 0x0018;
	table[8 * numbersOfChannels.size() + 1]	= 0x8020;
	table[8 * numbersOfChannels.size() + 2]	= 4;
	table[8 * numbersOfChannels.size() + 3]	= 0;
	table[8 * numbersOfChannels.size() + 4]	= 0x0020;
	table[8 * numbersOfChannels.size() + 5]	= 0x8020;
	table[8 * numbersOfChannels.size() + 6]	= 4;
	table[8 * numbersOfChannels.size() + 7]	= 0;

	for (int i = 0; i < table.size(); i++)	{
		clientQueue_->addCommandToStack(Record{address, table[i], Record::Type::Zero}, shared_from_this());
		address	+= 2;
	}
}

void	Host::initializeTable(uint32_t numberMaster)	{
	initializeTable(numberMaster, std::vector<uint32_t>{0, 1, 2, 3});
}

void	Host::writeSleep(uint32_t	milliseconds)	{
	Record	record{milliseconds, 0, Record::Type::Sleep};
	clientQueue_->addCommandToStack(record, shared_from_this());
}

Host::QueuePtr	Host::getQueue()	{
	return	clientQueue_;
}

Record Host::getResult()	const	{
	return	resultRecord_;
}
