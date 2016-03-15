#ifndef CHIPMODULE_H
#define CHIPMODULE_H

#include <vector>
#include <algorithm>
#include <map>
#include <queue>
#include <fstream>
#include <thread>
#include <mutex>
#include <ctime>

#include "../observer/observer.h"
#include "tekmodule.h"
#include "chip/blocks.h"
#include "processing.h"

class chipModule : public Subject	{
public:
	using	string		=	std::string;
	using	int32Vec	=	std::vector<int>;
	using	int32Pair	=	std::pair<int, int>;
	using	vecint32Pair=	std::pair<std::vector<int>, std::vector<int>>;
	using	calibrMap	=	std::map<int, std::array<int, 2>>;
	using	mapCRIter	=	std::map<int, std::array<int, 2>>::const_reverse_iterator;
	using	pairVec		=	std::vector<std::pair<int, int>>;

	enum class interest
	{statusUpdate};
	enum class mode
	{amp, form};
	explicit	chipModule(genModule *tekMod, processing*  calculation);

	bool		openSession();
	void		closeSession();
	void		calibration(int stepAmp, int stepThresh);
	void		stopCalibration();
	void		setDataUpdateFalse();
	void		setChannels(const int32Pair&	channel);
	void		setGroupChannel(int	channel);
	void		setPathToSave(const string&	pathToSave);

	mode		returnMode()	const;
	int32Pair	returnLastThresh()	const;
	int			searchAndSetChipChannel();
	void		saveToFileAmp(const string& pathToFile, int chipChannel);
	void		saveToFileThresh(const string& pathToFile, int chipChannel);
	void		threshCalibration(int step);
	vecint32Pair searchAiming(int chipChannel, int amp);
	std::pair<int, std::array<int, 2> > returnLastAmp()	const;
	bool		getDataUpdate()	const;

private:
	void		setThresh(int numberChipChannel, int codeThresh);
	void		mountThresh();
	void		setSettings(int channel, int ampStart, int ampEnd, int ampStep);
	void		ampCalibration(int ampStep, int threshStep);
	void		tekModuleActivateChannel(int channel, bool status);
	void		tekModuleSetHighLevel(int channel, int level);
	void		tekModuleSetLowLevel(int channel, int level);
	int32Vec	sendOneSignal(int channel, int amp);
	int			searchThresh();
	void		readFromFileAmp();

	genModule*				_module;
	processing*				_calculation;
	int32Pair				_howChipChannel;
	int						_groupChannel;
	int						_thresh[NCLAST * NDET];
	calibrMap				_calibr;
	pairVec					_threshList;
	calibrSettings_t		_settings[2];
	mode					_activeMode;
	bool					_stopFlag;
	bool					_dataUpdate;
	string					_pathToSave;
};

#endif // CHIPMODULE_H
