#ifndef PAAMODULE_H
#define PAAMODULE_H

#include <vector>
#include <algorithm>
#include <map>
#include <queue>
#include <fstream>
#include <thread>
#include <mutex>
#include <ctime>
#include <memory>

#include "../observer/observer.h"
#include "../tekVisa/genmodule.h"
#include "../calibration/calculation.h"
#include "../module/host.h"

class PaaModule : public Subject, public Observer, public std::enable_shared_from_this<PaaModule>	{
public:
	using	HostPtr			= std::shared_ptr<Host>;
	using	MasterPtr		= std::shared_ptr<Master>;
	using	AdcPtr			= std::shared_ptr<Adc>;
	using	QueuePtr		= std::shared_ptr<QueueOfMessages>;
	using	GenModulePtr	= std::shared_ptr<GenModule>;
	using	CalculationPtr	= std::shared_ptr<Calculation>;

	PaaModule(GenModulePtr tekMod, CalculationPtr calculation, const std::string& ip);
	~PaaModule();

	void	update(const Subject *subject);

	bool	openSession();
	void	closeSession();

	void	setThresh(int numberChipChannel, int codeThresh);

protected:
	void	tekModuleActivateChannel(int channel, bool status);
	void	tekModuleSetHighLevel(int channel, int level);
	void	tekModuleSetLowLevel(int channel, int level);

private:
	GenModulePtr	module_;
	CalculationPtr	calculation_;

	HostPtr			host_;
	MasterPtr		master_;
	AdcPtr			adc_;
	QueuePtr		clientQueue_;
	std::vector<uchar>	data_;
};

#endif // PAAMODULE_H
