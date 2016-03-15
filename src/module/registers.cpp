#include "registers.h"

HostRegisters::HostRegisters()	{
	write.ban_of_timestamps.address		= 0;
	write.cluster_number_link_0.address	= 0x34;
	write.cluster_number_link_1.address	= 0x36;
	write.cluster_number_link_2.address	= 0x38;
	write.cluster_number_link_3.address	= 0x3a;
	write.coarse_reset.address			= 0x0c;
	write.resolution_of_links.address	= 0x0e;
	write.exchange_register.address		= 0x14;
	write.control_dma.address			= 0x20;
	write.start_task_link_0.address		= 0x40;
	write.start_task_link_1.address		= 0x42;
	write.start_task_link_2.address		= 0x44;
	write.start_task_link_3.address		= 0x46;
	write.register_of_masks.address		= 0x06;
	write.test_register.address			= 0x08;

	write.register_of_read_data.address	= 0x01;
	write.resolution_of_data.address	= 0x10;
	write.forbid_of_data.address		= 0x11;
	write.clear_data_buffer.address		= 0x12;
	write.register_of_read_data.type	= Record::Type::Two;
	write.resolution_of_data.type		= Record::Type::Two;
	write.forbid_of_data.type			= Record::Type::Two;
	write.clear_data_buffer.type		= Record::Type::Two;

	read.cluster_number_link_0.address	= 0x34;
	read.cluster_number_link_1.address	= 0x36;
	read.cluster_number_link_2.address	= 0x38;
	read.cluster_number_link_3.address	= 0x3a;
	read.exchange_register.address		= 0x14;
	read.optic_length_link_0.address	= 0x0a;
	read.optic_length_link_1.address	= 0x0c;
	read.optic_length_link_2.address	= 0x0e;
	read.optic_length_link_3.address	= 0x10;
	read.start_task_link_0.address		= 0x40;
	read.start_task_link_1.address		= 0x42;
	read.start_task_link_2.address		= 0x44;
	read.start_task_link_3.address		= 0x46;

	writeRecords.push_back(&write.ban_of_timestamps);
	writeRecords.push_back(&write.cluster_number_link_0);
	writeRecords.push_back(&write.cluster_number_link_1);
	writeRecords.push_back(&write.cluster_number_link_2);
	writeRecords.push_back(&write.cluster_number_link_3);
	writeRecords.push_back(&write.coarse_reset);
	writeRecords.push_back(&write.resolution_of_links);
	writeRecords.push_back(&write.register_of_masks);
	writeRecords.push_back(&write.exchange_register);
	writeRecords.push_back(&write.control_dma);
	writeRecords.push_back(&write.test_register);
	writeRecords.push_back(&write.start_task_link_0);
	writeRecords.push_back(&write.start_task_link_1);
	writeRecords.push_back(&write.start_task_link_2);
	writeRecords.push_back(&write.start_task_link_3);

	for (auto record : writeRecords)
		record->type = Record::Type::Zero;

	readRecords.push_back(&read.cluster_number_link_0);
	readRecords.push_back(&read.cluster_number_link_1);
	readRecords.push_back(&read.cluster_number_link_2);
	readRecords.push_back(&read.cluster_number_link_3);
	readRecords.push_back(&read.exchange_register);
	readRecords.push_back(&read.optic_length_link_0);
	readRecords.push_back(&read.optic_length_link_1);
	readRecords.push_back(&read.optic_length_link_2);
	readRecords.push_back(&read.optic_length_link_3);
	readRecords.push_back(&read.start_task_link_0);
	readRecords.push_back(&read.start_task_link_1);
	readRecords.push_back(&read.start_task_link_2);
	readRecords.push_back(&read.start_task_link_3);
	readRecords.push_back(&read.test_register);

	for (auto record : readRecords)
		record->type = Record::Type::Read;
}

MasterRegisters::MasterRegisters()	{
	write.ch_error_count.address			= 0x18;
	write.ch_error_count.address			= 0x0;
	write.resolution_of_timer.address		= 0x02;
	write.test_register.address				= 0x04;
	write.coincidende.address				= 0x06;
	write.reset_counter.address				= 0x08;
	write.temporary_window.address			= 0x0a;
	write.mask_of_active_channels.address	= 0x0c;
	write.control_trigger.address			= 0x1c;
	write.program_trigger.address			= 0x22;
	write.reset_vme.address					= 0x24;
	write.time_first.address				= 0x10;
	write.time_second.address				= 0x12;
	write.time_third.address				= 0x14;
	write.time_fourth.address				= 0x16;

	read.status.address						= 0x0;
	read.test_register.address				= 0x04;
	read.coincidence.address				= 0x06;
	read.count_of_bank_0_first.address		= 0x2e;
	read.count_of_bank_0_second.address		= 0x30;
	read.count_of_bank_1_first.address		= 0x32;
	read.count_of_bank_1_second.address		= 0x34;
	read.time_first.address					= 0x10;
	read.time_second.address				= 0x12;
	read.time_third.address					= 0x14;
	read.time_fourth.address				= 0x16;

	writeRecords.push_back(&write.ch_error_count);
	writeRecords.push_back(&write.ch_error_count);
	writeRecords.push_back(&write.resolution_of_timer);
	writeRecords.push_back(&write.test_register);
	writeRecords.push_back(&write.coincidende);
	writeRecords.push_back(&write.reset_counter);
	writeRecords.push_back(&write.temporary_window);
	writeRecords.push_back(&write.mask_of_active_channels);
	writeRecords.push_back(&write.control_trigger);
	writeRecords.push_back(&write.program_trigger);
	writeRecords.push_back(&write.reset_vme);
	writeRecords.push_back(&write.time_first);
	writeRecords.push_back(&write.time_second);
	writeRecords.push_back(&write.time_third);
	writeRecords.push_back(&write.time_fourth);

	for (auto record : writeRecords)
		record->type = Record::Type::Zero;

	readRecords.push_back(&read.coincidence);
	readRecords.push_back(&read.count_of_bank_0_first);
	readRecords.push_back(&read.count_of_bank_0_second);
	readRecords.push_back(&read.count_of_bank_1_first);
	readRecords.push_back(&read.count_of_bank_1_second);
	readRecords.push_back(&read.status);
	readRecords.push_back(&read.test_register);
	readRecords.push_back(&read.time_first);
	readRecords.push_back(&read.time_second);
	readRecords.push_back(&read.time_third);
	readRecords.push_back(&read.time_fourth);
	for (auto record : readRecords)
		record->type = Record::Type::Read;
}

AdcRegisters::AdcRegisters()	{
	write.control_register.address		= 0x0;
	write.first_threshold.address		= 0x10;
	write.second_threshold.address		= 0x14;
	write.first_delay.address			= 0x12;
	write.second_delay.address			= 0x16;
	write.reset_first_channel.address	= 0x18;
	write.reset_second_channel.address	= 0x1a;
	write.smoothing.address				= 0x28;

	writeRecords.push_back(&write.control_register);
	writeRecords.push_back(&write.first_threshold);
	writeRecords.push_back(&write.second_threshold);
	writeRecords.push_back(&write.first_delay);
	writeRecords.push_back(&write.second_delay);
	writeRecords.push_back(&write.reset_first_channel);
	writeRecords.push_back(&write.reset_second_channel);
	writeRecords.push_back(&write.smoothing);

	for (auto record : writeRecords)
		record->type = Record::Type::Zero;

	read.status_register.address		= 0x0;

	read.status_register.type = Record::Type::Read;

	readRecords.push_back(&read.status_register);
}
