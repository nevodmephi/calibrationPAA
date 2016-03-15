#ifndef PROCESSING_H
#define PROCESSING_H

#include <fstream>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstdint>

#include "src/tekVisa/tektypes.h"

class processing	{
public:
	using	int32Vec	=	std::vector<int32_t>;
	using	string		=	std::string;
	using	homingData	=	std::array< std::array< int, 4>, 4>;

	explicit processing();
	~processing();

	struct	factorTransformation
	{
		double	factor;
		double	error;
		double	maxDeviation;
		double	deltaK;
		double	deltaB;
	};

	factorTransformation	getFactorTransformation(const int32Vec& ampX, const int32Vec& codeY);
	double					getFactorIntegral(double maxDeviation);

	void	computeHomingValues(int chipChannelV4, const int32Vec& minDeviation, const int32Vec& maxDeviation);
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
	void	readOneRecordAmp(const string& pathToFile);
	void	readOneRecordForm(const string& pathToFile);
	void	setPathToFile(const string& pathToSaveInput);
	void	setChipChannel(int chipGroupChannel);
	const homingData&	returnHoming()	const;
	const dataChip&		returnData()	const;

private:
	dataChip	data;
	homingData	homing;
	int32Vec	vecX;
	int32Vec	vecFirst;
	int32Vec	vecSecond;
	string      pathToSave;
	int			howChipChannel;
};

#endif // PROCESSING_H
