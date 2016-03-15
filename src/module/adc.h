#ifndef ADC_H
#define ADC_H

#include <cstdint>
#include <memory>

#include "queueofmessages.h"

class Adc : public Observer, public std::enable_shared_from_this<Adc>	{
public:
	using QueuePtr	= std::shared_ptr<QueueOfMessages>;

	Adc(uint32_t	offset, QueuePtr	clientQueue);
	~Adc();

	void	update(const Subject* subject);
	void	readState();

	void	writeControlRegister();
	void	writeThreshold(uint32_t	channel, uint16_t	threshold);
	void	writeDelay(uint32_t	channel, uint16_t	delay);
	void	writeResetChannel(uint32_t	channel);
	void	writeSmoothing(uint16_t	smoothing);

protected:

private:
	QueuePtr		clientQueue_;
	AdcRegisters	registers_;
	uint32_t		offset_;
};

#endif // DETECTOR_H
