#ifndef REGISTERS_H
#define REGISTERS_H

#include <cstdint>
#include <list>

struct	Record	{
	enum class Type	{
		Zero,
		Two,
		Read,
		Sleep
	};

	uint32_t	address;
	uint16_t	value;
	Type		type;
};

struct	HostRegisters	{
	struct	Write	{
		Record	ban_of_timestamps;		//0x0
		Record	register_of_masks;		//0x06
		Record	test_register;			//0x08
		Record	coarse_reset;			//0x0c
		Record	resolution_of_links;	//0x0e
		Record	exchange_register;		//0x14
		Record	control_dma;			//0x20
		Record	cluster_number_link_0;	//0x34
		Record	cluster_number_link_1;	//0x36
		Record	cluster_number_link_2;	//0x38
		Record	cluster_number_link_3;	//0x3a
		Record	start_task_link_0;		//0x40
		Record	start_task_link_1;		//0x42
		Record	start_task_link_2;		//0x44
		Record	start_task_link_3;		//0x46

		Record	register_of_read_data;	//0x01
		Record	resolution_of_data;		//0x10
		Record	forbid_of_data;			//0x11
		Record	clear_data_buffer;		//0x12
	};

	struct	Read	{
		Record	test_register;			//0x08
		Record	optic_length_link_0;	//0x0a
		Record	optic_length_link_1;	//0x0c
		Record	optic_length_link_2;	//0x0e
		Record	optic_length_link_3;	//0x10
		Record	exchange_register;		//0x14
		Record	cluster_number_link_0;	//0x34
		Record	cluster_number_link_1;	//0x36
		Record	cluster_number_link_2;	//0x38
		Record	cluster_number_link_3;	//0x3a
		Record	start_task_link_0;		//0x40
		Record	start_task_link_1;		//0x42
		Record	start_task_link_2;		//0x44
		Record	start_task_link_3;		//0x46
	};

	HostRegisters();

	Write	write;
	Read	read;
	std::list<Record*>	readRecords;
	std::list<Record*>	writeRecords;
};

struct	MasterRegisters	{
	struct	Write	{
		Record	reset_register;			//0x0
		Record	resolution_of_timer;	//0x02
		Record	test_register;			//0x04
		Record	coincidende;			//0x06
		Record	reset_counter;			//0x08
		Record	temporary_window;		//0x0a
		Record	mask_of_active_channels;//0x0c
		Record	time_first;				//0x10
		Record	time_second;			//0x12
		Record	time_third;				//0x14
		Record	time_fourth;			//0x16
//		Record	timer!!
		Record	ch_error_count;			//0x18
		Record	control_trigger;		//0x1c
		Record	program_trigger;		//0x22
		Record	reset_vme;				//0x24
	};

	struct	Read	{
		Record	status;					//0x0
		Record	test_register;			//0x04
		Record	coincidence;			//0x06
		Record	count_of_bank_0_first;	//0x2e
		Record	count_of_bank_0_second;	//0x30
		Record	count_of_bank_1_first;	//0x32
		Record	count_of_bank_1_second;	//0x34
		Record	time_first;				//0x10
		Record	time_second;			//0x12
		Record	time_third;				//0x14
		Record	time_fourth;			//0x16
	};

	MasterRegisters();

	Write	write;
	Read	read;
	std::list<Record*>	readRecords;
	std::list<Record*>	writeRecords;
};

struct	AdcRegisters	{
	struct	Write	{
		Record	control_register;		//0x0
		Record	first_threshold;		//0x10
		Record	second_threshold;		//0x14
		Record	first_delay;			//0x12
		Record	second_delay;			//0x16
		Record	reset_first_channel;	//0x18
		Record	reset_second_channel;	//0x1a
		Record	smoothing;				//0x28
//		Record
	};

	struct	Read	{
		Record	status_register;		//0x0
	};

	AdcRegisters();

	Write	write;
	Read	read;
	std::list<Record*>	readRecords;
	std::list<Record*>	writeRecords;
};

#endif // REGISTERS_H
