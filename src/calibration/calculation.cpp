#include "calculation.h"

using namespace std;

Calculation::Calculation()	{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			homing_[i][j] = 0;

	for (int i = 0; i < 4; i++)	{
		data_.transformationADC[i] = 0;
		data_.errorADC[i] = 0;
		data_.constADC[i] = 0;
		data_.errorConstADC[i] = 0;
	}
	for (int i = 0; i < 2; i++)	{
		data_.transformationForm[i]	= 0;
		data_.errorForm[i]	= 0;
		data_.errorConstForm[i]	= 0;
		data_.constForm[i]	= 0;
	}
}

Calculation::~Calculation()	{

}

void Calculation::setChipChannel(int chipGroupChannel)	{
	howChipChannel_ = chipGroupChannel;
}

Calculation::factorTransformation	Calculation::getFactorTransformation(const std::vector<int32_t>& ampX,
																		 const std::vector<int32_t>& codeY)	{
	if (ampX.size() != codeY.size())
		return factorTransformation({-1, -1, -1});

	int sizeInput	= ampX.size();

	for (int i = 0; i < sizeInput; i++)
		if (ampX[i] == 2048 * 6)
			sizeInput = i;

	double xGo	= 0;
	double yGo	= 0;
	double x2Go	= 0;
	double xyGo	= 0;

	for (int i = 0; i < sizeInput; i++)
	{
		xGo		+= ampX[i];
		yGo		+= codeY[i];
		x2Go	+= ampX[i] * ampX[i];
		xyGo	+= ampX[i] * codeY[i];
	}
	double	midX	= xGo / (double) sizeInput;
	double	midY	= yGo / (double) sizeInput;
	double	midXY	= xyGo / (double) sizeInput;
	double	midX2	= x2Go / (double) sizeInput;
	double	factor	= (midXY - midX * midY)	/ (midX2 - midX * midX);
	double	free	= midY - factor * midX;
	double	maxDeviation = 10.0;
	double	sumHelp	= 0;
	for (int i = 0; i < sizeInput; i++)
		sumHelp	+= ((double)codeY[i] - factor * (double)ampX[i] - free) *
				((double)codeY[i] - factor * (double)ampX[i] - free);
	double	delta	= sqrt(1.0 / (double)(sizeInput * (sizeInput - 2)) * sumHelp);
	double	deltaK	= delta * sqrt(1.0 / (midX2 - midX * midX));
	double	deltaB	= delta * sqrt(midX2 / (midX2 - midX * midX));
	return factorTransformation({factor, free, maxDeviation, deltaK, deltaB});
//	return pair<double, double>(factor, free);
}

double	Calculation::getFactorIntegral(double maxDeviation)	{
	const double ampMax = 255.0;
	return maxDeviation / ampMax * 100.0;
}

void Calculation::setPathToFile(const string& pathToSaveInput)	{
	pathToSave_ = pathToSaveInput;

//	data = {};
//	homing = {};
}

void Calculation::writeDataADCToFiles()	{
	ofstream mainFile(pathToSave_ + string("dataADC"));
	for (int i = 0; i < 4; i++)
		mainFile << data_.transformationADC[i] << "\t";
	mainFile << endl;
	for (int i = 0; i < 4; i++)
		mainFile << data_.errorADC[i] << "\t";
	mainFile << endl;
	for (int i = 0; i < 4; i++)
		mainFile << data_.constADC[i] << "\t";
	mainFile << endl;
	for (int i = 0; i < 4; i++)
		mainFile << data_.errorConstADC[i] << "\t";
	mainFile << endl;
	mainFile.close();
}

void Calculation::writeDataFormToFiles()	{
	ofstream mainFile(pathToSave_ + string("dataForm"));
	for (int i = 0; i < 2; i++)
		mainFile << data_.transformationForm[i] << "\t";
	mainFile << endl;
	for (int i = 0; i < 2; i++)
		mainFile << data_.errorForm[i] << "\t";
	mainFile << endl;
	for (int i = 0; i < 2; i++)
		mainFile << data_.constForm[i] << "\t";
	mainFile << endl;
	for (int i = 0; i < 2; i++)
		mainFile << data_.errorConstForm[i] << "\t";
	mainFile << endl;
	mainFile.close();
}

void Calculation::writeHomingToFiles()	{
	ofstream mainFile(string(pathToSave_) += "homing");
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			mainFile << homing_[i][j] << "\t";
		mainFile << endl;
	}
	mainFile.close();
}

void Calculation::readHomingFromFiles()	{
	ifstream mainFile(string(pathToSave_) += "homing");
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			mainFile >> homing_[i][j];
			cout << homing_[i][j] << "  ";
		}
		cout << endl;
	}
	mainFile.close();
}

void Calculation::computeHomingValues(int chipChannelV4, const std::vector<int32_t> &minDeviation,
									  const std::vector<int32_t> &maxDeviation)	{
	readHomingFromFiles();
	vector<double> influenceChannel(4);
	for (int i = 0; i < 4; i++)
		if (i != chipChannelV4)
			influenceChannel[i] = 20.0 * log10(maxDeviation[i] / minDeviation[i]);
	influenceChannel[chipChannelV4] = -1000;
	for (int i = 0; i < 4; i++)
			homing_[chipChannelV4][i] = influenceChannel[i];
	writeHomingToFiles();
}

void Calculation::readOneRecordAmp(const std::string &pathToFile)	{
	vecX_.clear();
	vecFirst_.clear();
	vecSecond_.clear();
	ifstream	mainFile(pathToFile);
	while (!mainFile.eof())
	{
		int readX, readFirst, readSecond;
		if (howChipChannel_ == 0)
		{
			mainFile >> readX >> readFirst >> readSecond;
		}
		if (howChipChannel_ == 1)
		{
			int	readNoOne;
			int	readNoTwo;
			mainFile >> readX >> readNoOne >> readNoTwo >> readFirst >> readSecond;
		}
		vecX_.push_back(readX);
		vecFirst_.push_back(readFirst);
		vecSecond_.push_back(readSecond);
	}
	mainFile.close();
}

void Calculation::readOneRecordForm(const std::string &pathToFile)	{
	vecX_.clear();
	vecFirst_.clear();
	vecSecond_.clear();
	ifstream	mainFile(pathToFile);
	while (!mainFile.eof())
	{
		int readX, readFirst;
		if (howChipChannel_ == 0)
			mainFile >> readX >> readFirst;
		if (howChipChannel_ == 1)
		{
			int	readNo;
			mainFile >> readX >> readNo >> readFirst;
		}
		vecX_.push_back(readX);
		vecFirst_.push_back(readFirst);
	}
	mainFile.close();
}

void Calculation::computeForOneRecordAmp()	{
	readDataFromFiles();
	readOneRecordAmp(pathToSave_ + string("amp"));
//	for (int i = 0; i < vecFirst.size(); i++)
//	{
//		vecFirst[i] = vecFirst[i] * 6;
//		vecSecond[i]= vecSecond[i] * 6;
//	}
	factorTransformation	exchangeFirst = getFactorTransformation(vecFirst_, vecX_);
	factorTransformation	exchangeSecond = getFactorTransformation(vecSecond_, vecX_);
	cout << exchangeFirst.factor << endl;
	if (howChipChannel_ == 0)
	{
		data_.transformationADC[0]	= exchangeFirst.factor;
		data_.errorADC[0]			= exchangeFirst.deltaK;
		data_.constADC[0]			= exchangeFirst.error;
		data_.errorConstADC[0]		= exchangeFirst.deltaB;

		data_.transformationADC[1]	= exchangeSecond.factor;
		data_.errorADC[1]			= exchangeSecond.deltaK;
		data_.constADC[1]			= exchangeSecond.error;
		data_.errorConstADC[1]		= exchangeSecond.deltaB;
	}
	if (howChipChannel_ == 1)
	{
		data_.transformationADC[2]	= exchangeFirst.factor;
		data_.errorADC[2]			= exchangeFirst.deltaK;
		data_.constADC[2]			= exchangeFirst.error;
		data_.errorConstADC[2]		= exchangeFirst.deltaB;

		data_.transformationADC[3]	= exchangeSecond.factor;
		data_.errorADC[3]			= exchangeSecond.deltaK;
		data_.constADC[3]			= exchangeSecond.error;
		data_.errorConstADC[3]		= exchangeSecond.deltaB;
	}
	writeDataToFiles();
}

void Calculation::computeForOneRecordForm()	{
	readDataFromFiles();
	readOneRecordForm(pathToSave_ + string("thresh"));
//	for (int i = 0; i < vecX.size(); i++)
//		vecX[i] = vecX[i] * 6;
	factorTransformation	exchange = getFactorTransformation(vecX_, vecFirst_);
	if (howChipChannel_ == 0)	{
		data_.transformationForm[0]	= exchange.factor;
		data_.errorForm[0]			= exchange.deltaK;
		data_.constForm[0]			= exchange.error;
		data_.errorConstForm[0]		= exchange.deltaB;
	}
	if (howChipChannel_ == 1)	{
		data_.transformationForm[1]	= exchange.factor;
		data_.errorForm[1]			= exchange.deltaK;
		data_.constForm[1]			= exchange.error;
		data_.errorConstForm[1]		= exchange.deltaB;
	}
	writeDataToFiles();
}

const DataChip&	Calculation::returnData()	const	{
	return data_;
}

const std::array<std::array<int, 4>, 4> &Calculation::returnHoming()	const	{
	return homing_;
}

void Calculation::readDataADCFromFiles()	{
	ifstream	mainFile;
	mainFile.open(pathToSave_ + string("dataADC"));
	for (int i = 0; i < 4; i++)
		mainFile >> data_.transformationADC[i];
	for (int i = 0; i < 4; i++)
		mainFile >> data_.errorADC[i];
	for (int i = 0; i < 4; i++)
		mainFile >> data_.constADC[i];
	for (int i = 0; i < 4; i++)
		mainFile >> data_.errorConstADC[i];
	mainFile.close();
}

void Calculation::readDataFormFromFiles()	{
	ifstream	mainFile;
	mainFile.open(pathToSave_ + string("dataForm"));
	for (int i = 0; i < 2; i++)
		mainFile >> data_.transformationForm[i];
	for (int i = 0; i < 2; i++)
		mainFile >> data_.errorForm[i];
	for (int i = 0; i < 2; i++)
		mainFile >> data_.constForm[i];
	for (int i = 0; i < 2; i++)
		mainFile >> data_.errorConstForm[i];
	mainFile.close();
}

void Calculation::writeDataToFiles()	{
	ofstream mainFile(pathToSave_ + string("data"));
//	mainFile << "FaA\t";
	for (int i = 0; i < 4; i++)
		mainFile << setw(5) << data_.transformationADC[i] << "\t";
	mainFile << endl;
//	mainFile << "ErA\t";
	for (int i = 0; i < 4; i++)
		mainFile << setw(5) << data_.errorADC[i] << "\t";
	mainFile << endl;
//	mainFile << "CoA\t";
	for (int i = 0; i < 4; i++)
		mainFile << setw(5) << data_.constADC[i] << "\t";
	mainFile << endl;
//	mainFile << "EcA\t";
	for (int i = 0; i < 4; i++)
		mainFile << setw(5) << data_.errorConstADC[i] << "\t";
	mainFile << endl;
//	mainFile << "FaF\t";
	for (int i = 0; i < 2; i++)
		mainFile << setw(5) << data_.transformationForm[i] << "\t";
	mainFile << endl;
//	mainFile << "ErF\t";
	for (int i = 0; i < 2; i++)
		mainFile << setw(5) << data_.errorForm[i] << "\t";
	mainFile << endl;
//	mainFile << "CoF\t";
	for (int i = 0; i < 2; i++)
		mainFile << setw(5) << data_.constForm[i] << "\t";
	mainFile << endl;
//	mainFile << "EcF\t";
	for (int i = 0; i < 2; i++)
		mainFile << setw(5) << data_.errorConstForm[i] << "\t";
	mainFile << endl;
	mainFile.close();
}

void Calculation::readDataFromFiles()	{
	ifstream	mainFile;
	mainFile.open(pathToSave_ + string("data"));
	for (int i = 0; i < 4; i++)
		mainFile >> data_.transformationADC[i];
	for (int i = 0; i < 4; i++)
		mainFile >> data_.errorADC[i];
	for (int i = 0; i < 4; i++)
		mainFile >> data_.constADC[i];
	for (int i = 0; i < 4; i++)
		mainFile >> data_.errorConstADC[i];
	for (int i = 0; i < 2; i++)
		mainFile >> data_.transformationForm[i];
	for (int i = 0; i < 2; i++)
		mainFile >> data_.errorForm[i];
	for (int i = 0; i < 2; i++)
		mainFile >> data_.constForm[i];
	for (int i = 0; i < 2; i++)
		mainFile >> data_.errorConstForm[i];
	mainFile.close();
}

