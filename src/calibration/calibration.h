#ifndef CALIBRATION_H
#define CALIBRATION_H

#include <memory>

#include "processing.h"
#include "calculation.h"
#include "../module/paamodule.h"
#include "../tekVisa/genmodule.h"

class Calibration	{
public:
	using	GenModulePtr	= std::shared_ptr<GenModule>;
	using	PaaModulePtr	= std::shared_ptr<PaaModule>;
	using	ProcessingPtr	= std::shared_ptr<Processing>;
	using	CalculationPtr	= std::shared_ptr<Calculation>;

	enum class mode
	{amp, form};

	Calibration(const std::string& ip, const std::string& resName);
	~Calibration();

	std::pair<bool, bool>	openSessions();
	void	closeSessions();
	void	calibrateAmplitude();
	void	calibrateThreshold();
	void	stopCalibration();

	void	setChannels(const std::pair<int, int>& channel);
	void	setGroupChannel(int channel);
	void	setPathToSave(const std::string& pathToSave);

	void	readAndSaveInformation();

	std::shared_ptr<GenModule>	getGenModule()	const;
	std::shared_ptr<PaaModule>	getPaaModule()	const;

protected:
	void	searchThreshold();
	void	setThreshold(int numberChipChannel, int codeThresh);

private:
	GenModulePtr	genModule_;
	PaaModulePtr	paaModule_;
	ProcessingPtr	processing_;
	CalculationPtr	calculation_;

	std::pair<int, int>	howChipChannel_;
};

#endif // CALIBRATION_H
