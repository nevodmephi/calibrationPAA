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
	using	HostPtr		= std::shared_ptr<Host>;
	using	MasterPtr	= std::shared_ptr<Master>;
	using	AdcPtr		= std::shared_ptr<Adc>;
	using	QueuePtr	= std::shared_ptr<QueueOfMessages>;

	enum class mode
	{amp, form};

	PaaModule(GenModule* tekMod, Calculation* calculation, const std::string& ip);
	~PaaModule();

	void	update(const Subject *subject);

	bool	openSession();
	void	closeSession();
	void	calibration(int stepAmp, int stepThresh);
	void	stopCalibration();
	void	setDataUpdateFalse();
	void	setChannels(const std::pair<int, int>& channel);
	void	setGroupChannel(int channel);
	void	setPathToSave(const std::string& pathToSave);

	mode	returnMode()	const;
	std::pair<int, int>	returnLastThresh()	const;
	int		searchAndSetChipChannel();
	void	saveToFileAmp(const std::string& pathToFile, int chipChannel);
	void	saveToFileThresh(const std::string& pathToFile, int chipChannel);
	void	threshCalibration(int step);
	bool	getDataUpdate()	const;
	std::pair<std::vector<int>, std::vector<int>> searchAiming(int chipChannel, int amp);
	std::pair<int, std::array<int, 2> > returnLastAmp()	const;

	void	setThresh(int numberChipChannel, int codeThresh);

protected:
	void	mountThresh();
	void	setSettings(int channel, int ampStart, int ampEnd, int ampStep);
	void	ampCalibration(int ampStep, int threshStep);
	void	tekModuleActivateChannel(int channel, bool status);
	void	tekModuleSetHighLevel(int channel, int level);
	void	tekModuleSetLowLevel(int channel, int level);
	std::vector<int>	sendOneSignal(int channel, int amp);
	int		searchThresh();
	void	readFromFileAmp();

private:
	GenModule*				module_;
	Calculation*			calculation_;
	std::pair<int, int>		howChipChannel_;
	int						groupChannel_;
	int						thresh_[4 * 2];
	CalibrSettings_t		settings_[2];
	mode					activeMode_;
	bool					stopFlag_;
	bool					dataUpdate_;
	std::string				pathToSave_;
	std::map<int, std::array<int, 2>>	calibr_;
	std::vector<std::pair<int, int>>	threshList_;

	HostPtr		host_;
	MasterPtr	master_;
	AdcPtr		adc_;
	QueuePtr	clientQueue_;
	std::vector<uchar>	data_;
};

#endif // PAAMODULE_H
