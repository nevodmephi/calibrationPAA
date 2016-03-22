#ifndef CALCULATION_H
#define CALCULATION_H

#include <fstream>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstdint>

#include "src/tekVisa/tektypes.h"

class Calculation	{
public:

	explicit Calculation();
	~Calculation();

	struct	factorTransformation	{
		double	factor;
		double	error;
		double	maxDeviation;
		double	deltaK;
		double	deltaB;
	};

	factorTransformation	getFactorTransformation(const std::vector<int32_t>& ampX,
													const std::vector<int32_t>& codeY);
	double					getFactorIntegral(double maxDeviation);

	void	computeHomingValues(int chipChannelV4, const std::vector<int32_t>& minDeviation,
								const std::vector<int32_t>& maxDeviation);
	void	readDataADCFromFiles();
	void	readDataFormFromFiles();
	void	writeDataToFiles();
	void	readDataFromFiles();
	void	writeDataADCToFiles();
	void	writeDataFormToFiles();
	void	readHomingFromFiles();
	void	writeHomingToFiles();
	void	computeForOneRecordAmp();
	void	computeForOneRecordForm();
	void	readOneRecordAmp(const std::string& pathToFile);
	void	readOneRecordForm(const std::string& pathToFile);
	void	setPathToFile(const std::string& pathToSaveInput);
	void	setChipChannel(int chipGroupChannel);
	const std::array<std::array<int, 4>, 4>&	returnHoming()	const;
	const DataChip&		returnData()	const;

private:
	DataChip	data_;
	std::string	pathToSave_;
	int			howChipChannel_;
	std::vector<int32_t>	vecX_;
	std::vector<int32_t>	vecFirst_;
	std::vector<int32_t>	vecSecond_;
	std::array<std::array<int, 4>, 4>	homing_;
};

#endif // CALCULATION_H
