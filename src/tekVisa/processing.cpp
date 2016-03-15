#include "processing.h"

using namespace std;

processing::processing()	{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			homing[i][j] = 0;

	for (int i = 0; i < 4; i++)	{
		data.transformationADC[i] = 0;
		data.errorADC[i] = 0;
		data.constADC[i] = 0;
		data.errorConstADC[i] = 0;
	}
	for (int i = 0; i < 2; i++)	{
		data.transformationForm[i]	= 0;
		data.errorForm[i]	= 0;
		data.errorConstForm[i]	= 0;
		data.constForm[i]	= 0;
	}
}

processing::~processing()	{

}

void processing::setChipChannel(int chipGroupChannel)	{
	howChipChannel = chipGroupChannel;
}

processing::factorTransformation	processing::getFactorTransformation(const int32Vec& ampX, const int32Vec& codeY)	{
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

double	processing::getFactorIntegral(double maxDeviation)	{
	const double ampMax = 255.0;
	return maxDeviation / ampMax * 100.0;
}

void processing::setPathToFile(const string& pathToSaveInput)	{
	pathToSave = pathToSaveInput;

//	data = {};
//	homing = {};
}

void processing::writeDataADCToFiles()	{
	ofstream mainFile(pathToSave + string("dataADC"));
	for (int i = 0; i < 4; i++)
		mainFile << data.transformationADC[i] << "\t";
	mainFile << endl;
	for (int i = 0; i < 4; i++)
		mainFile << data.errorADC[i] << "\t";
	mainFile << endl;
	for (int i = 0; i < 4; i++)
		mainFile << data.constADC[i] << "\t";
	mainFile << endl;
	for (int i = 0; i < 4; i++)
		mainFile << data.errorConstADC[i] << "\t";
	mainFile << endl;
	mainFile.close();
}

void processing::writeDataFormToFiles()	{
	ofstream mainFile(pathToSave + string("dataForm"));
	for (int i = 0; i < 2; i++)
		mainFile << data.transformationForm[i] << "\t";
	mainFile << endl;
	for (int i = 0; i < 2; i++)
		mainFile << data.errorForm[i] << "\t";
	mainFile << endl;
	for (int i = 0; i < 2; i++)
		mainFile << data.constForm[i] << "\t";
	mainFile << endl;
	for (int i = 0; i < 2; i++)
		mainFile << data.errorConstForm[i] << "\t";
	mainFile << endl;
	mainFile.close();
}

void processing::writeHomingToFiles()	{
	ofstream mainFile(string(pathToSave) += "homing");
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			mainFile << homing[i][j] << "\t";
		mainFile << endl;
	}
	mainFile.close();
}

void processing::readHomingFromFiles()	{
	ifstream mainFile(string(pathToSave) += "homing");
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			mainFile >> homing[i][j];
			cout << homing[i][j] << "  ";
		}
		cout << endl;
	}
	mainFile.close();
}

void processing::computeHomingValues(int chipChannelV4, const int32Vec &minDeviation, const int32Vec &maxDeviation)	{
	readHomingFromFiles();
	vector<double> influenceChannel(4);
	for (int i = 0; i < 4; i++)
		if (i != chipChannelV4)
			influenceChannel[i] = 20.0 * log10(maxDeviation[i] / minDeviation[i]);
	influenceChannel[chipChannelV4] = -1000;
	for (int i = 0; i < 4; i++)
			homing[chipChannelV4][i] = influenceChannel[i];
	writeHomingToFiles();
}

void processing::readOneRecordAmp(const string &pathToFile)	{
	vecX.clear();
	vecFirst.clear();
	vecSecond.clear();
	ifstream	mainFile(pathToFile);
	while (!mainFile.eof())
	{
		int readX, readFirst, readSecond;
		if (howChipChannel == 0)
		{
			mainFile >> readX >> readFirst >> readSecond;
		}
		if (howChipChannel == 1)
		{
			int	readNoOne;
			int	readNoTwo;
			mainFile >> readX >> readNoOne >> readNoTwo >> readFirst >> readSecond;
		}
		vecX.push_back(readX);
		vecFirst.push_back(readFirst);
		vecSecond.push_back(readSecond);
	}
	mainFile.close();
}

void processing::readOneRecordForm(const string &pathToFile)	{
	vecX.clear();
	vecFirst.clear();
	vecSecond.clear();
	ifstream	mainFile(pathToFile);
	while (!mainFile.eof())
	{
		int readX, readFirst;
		if (howChipChannel == 0)
			mainFile >> readX >> readFirst;
		if (howChipChannel == 1)
		{
			int	readNo;
			mainFile >> readX >> readNo >> readFirst;
		}
		vecX.push_back(readX);
		vecFirst.push_back(readFirst);
	}
	mainFile.close();
}

void processing::computeForOneRecordAmp()	{
	readDataFromFiles();
	readOneRecordAmp(pathToSave + string("amp"));
//	for (int i = 0; i < vecFirst.size(); i++)
//	{
//		vecFirst[i] = vecFirst[i] * 6;
//		vecSecond[i]= vecSecond[i] * 6;
//	}
	factorTransformation	exchangeFirst = getFactorTransformation(vecFirst, vecX);
	factorTransformation	exchangeSecond = getFactorTransformation(vecSecond, vecX);
	cout << exchangeFirst.factor << endl;
	if (howChipChannel == 0)
	{
		data.transformationADC[0]	= exchangeFirst.factor;
		data.errorADC[0]			= exchangeFirst.deltaK;
		data.constADC[0]			= exchangeFirst.error;
		data.errorConstADC[0]		= exchangeFirst.deltaB;

		data.transformationADC[1]	= exchangeSecond.factor;
		data.errorADC[1]			= exchangeSecond.deltaK;
		data.constADC[1]			= exchangeSecond.error;
		data.errorConstADC[1]		= exchangeSecond.deltaB;
	}
	if (howChipChannel == 1)
	{
		data.transformationADC[2]	= exchangeFirst.factor;
		data.errorADC[2]			= exchangeFirst.deltaK;
		data.constADC[2]			= exchangeFirst.error;
		data.errorConstADC[2]		= exchangeFirst.deltaB;

		data.transformationADC[3]	= exchangeSecond.factor;
		data.errorADC[3]			= exchangeSecond.deltaK;
		data.constADC[3]			= exchangeSecond.error;
		data.errorConstADC[3]		= exchangeSecond.deltaB;
	}
	writeDataToFiles();
}

void processing::computeForOneRecordForm()	{
	readDataFromFiles();
	readOneRecordForm(pathToSave + string("thresh"));
//	for (int i = 0; i < vecX.size(); i++)
//		vecX[i] = vecX[i] * 6;
	factorTransformation	exchange = getFactorTransformation(vecX, vecFirst);
	if (howChipChannel == 0)
	{
		data.transformationForm[0]	= exchange.factor;
		data.errorForm[0]			= exchange.deltaK;
		data.constForm[0]			= exchange.error;
		data.errorConstForm[0]		= exchange.deltaB;
	}
	if (howChipChannel == 1)
	{
		data.transformationForm[1]	= exchange.factor;
		data.errorForm[1]			= exchange.deltaK;
		data.constForm[1]			= exchange.error;
		data.errorConstForm[1]		= exchange.deltaB;
	}
	writeDataToFiles();
}

const dataChip&	processing::returnData()	const	{
	return data;
}

const processing::homingData&	processing::returnHoming()	const	{
	return homing;
}

void processing::readDataADCFromFiles()	{
	ifstream	mainFile;
	mainFile.open(pathToSave + string("dataADC"));
	for (int i = 0; i < 4; i++)
		mainFile >> data.transformationADC[i];
	for (int i = 0; i < 4; i++)
		mainFile >> data.errorADC[i];
	for (int i = 0; i < 4; i++)
		mainFile >> data.constADC[i];
	for (int i = 0; i < 4; i++)
		mainFile >> data.errorConstADC[i];
	mainFile.close();
}

void processing::readDataFormFromFiles()	{
	ifstream	mainFile;
	mainFile.open(pathToSave + string("dataForm"));
	for (int i = 0; i < 2; i++)
		mainFile >> data.transformationForm[i];
	for (int i = 0; i < 2; i++)
		mainFile >> data.errorForm[i];
	for (int i = 0; i < 2; i++)
		mainFile >> data.constForm[i];
	for (int i = 0; i < 2; i++)
		mainFile >> data.errorConstForm[i];
	mainFile.close();
}

void processing::writeDataToFiles()	{
	ofstream mainFile(pathToSave + string("data"));
//	mainFile << "FaA\t";
	for (int i = 0; i < 4; i++)
		mainFile << setw(5) << data.transformationADC[i] << "\t";
	mainFile << endl;
//	mainFile << "ErA\t";
	for (int i = 0; i < 4; i++)
		mainFile << setw(5) << data.errorADC[i] << "\t";
	mainFile << endl;
//	mainFile << "CoA\t";
	for (int i = 0; i < 4; i++)
		mainFile << setw(5) << data.constADC[i] << "\t";
	mainFile << endl;
//	mainFile << "EcA\t";
	for (int i = 0; i < 4; i++)
		mainFile << setw(5) << data.errorConstADC[i] << "\t";
	mainFile << endl;
//	mainFile << "FaF\t";
	for (int i = 0; i < 2; i++)
		mainFile << setw(5) << data.transformationForm[i] << "\t";
	mainFile << endl;
//	mainFile << "ErF\t";
	for (int i = 0; i < 2; i++)
		mainFile << setw(5) << data.errorForm[i] << "\t";
	mainFile << endl;
//	mainFile << "CoF\t";
	for (int i = 0; i < 2; i++)
		mainFile << setw(5) << data.constForm[i] << "\t";
	mainFile << endl;
//	mainFile << "EcF\t";
	for (int i = 0; i < 2; i++)
		mainFile << setw(5) << data.errorConstForm[i] << "\t";
	mainFile << endl;
	mainFile.close();
}

void processing::readDataFromFiles()	{
	ifstream	mainFile;
	mainFile.open(pathToSave + string("data"));
	for (int i = 0; i < 4; i++)
		mainFile >> data.transformationADC[i];
	for (int i = 0; i < 4; i++)
		mainFile >> data.errorADC[i];
	for (int i = 0; i < 4; i++)
		mainFile >> data.constADC[i];
	for (int i = 0; i < 4; i++)
		mainFile >> data.errorConstADC[i];
	for (int i = 0; i < 2; i++)
		mainFile >> data.transformationForm[i];
	for (int i = 0; i < 2; i++)
		mainFile >> data.errorForm[i];
	for (int i = 0; i < 2; i++)
		mainFile >> data.constForm[i];
	for (int i = 0; i < 2; i++)
		mainFile >> data.errorConstForm[i];
	mainFile.close();
}

