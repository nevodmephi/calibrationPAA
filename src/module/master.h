#ifndef MASTER_H
#define MASTER_H

#include <cstdint>
#include <memory>
#include <vector>
#include <array>
#include <map>

#include "queueofmessages.h"
#include "adc.h"
#include "registers.h"

class Master : public Subject, public Observer, public std::enable_shared_from_this<Master>	{
public:
	using QueuePtr	= std::shared_ptr<QueueOfMessages>;
	using AdcPtr	= std::shared_ptr<Adc>;

	Master(uint32_t	offset, QueuePtr	clientQueue);
	~Master();

	void	update(const Subject* subject);

	AdcPtr	addAdc(uint32_t	numberAdc);
	void	deleteAdc(uint32_t	numberAdc);
	void	readState();

	void	writeResetRegister();
	void	writeResolutionOfTimer(bool resolution);
	void	writeTestRegister(uint16_t	data);
	void	writeCoincidence(uint32_t	coincidence);
	void	writeResetCounter();
	void	writeTemporaryWindow(uint16_t	window);
	void	writeMaskOfActiveChannels(const std::array<bool,8>&	activeChannels);
	void	writeChErrorCount(uint16_t	data);
	void	writeControlTrigger(int i);
	void	writeProgramTrigger(bool trigger);
	void	writeResetVme();
	void	writeTime(uint16_t	hours, uint16_t	minutes, uint16_t	seconds, uint16_t	ms);

protected:

private:
	QueuePtr			clientQueue_;
	std::vector<AdcPtr>	adcs_;
	MasterRegisters		registers_;
	uint32_t			offset_;
	uint32_t			offsetLink_;
};

#endif // MASTER_H
