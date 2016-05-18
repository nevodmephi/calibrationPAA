#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QBoxLayout>
#include <QDialog>
#include <QLabel>
#include <QDir>
#include <QRadioButton>
#include <QTableWidget>
#include <string>
#include <thread>

#include "tekoutput.h"
#include "teksettings.h"
#include "plots.h"
#include "../calibration/calibration.h"

class MainWindow : public QMainWindow	{
	Q_OBJECT

public:
	using	string		=	std::string;

	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

protected slots:
	void	startButtonClick();
	void	stopButtonClick();
	void	goButtonClick();
	void	endButtonClick();
	void	showSettingsClick();
	void	showTablesClick();
	void	dataUpdate();

protected:
	void	initializeElements();
	void	initializeLayouts();
	inline void	addToList(const string& addStr);
	void	readFromMemory(int numberChannel);
	void	writeToMemory(int numberChannel);
	void	writeToDataTable();
	void	writeToHomingTable();

	void	initializeFirst();
	void	initializeSecond();

private:
	Calibration	*calibration_;
//	tekModule	*module_;
	TekOutput	*output_;
	TekSettings *settings_;
//	PaaModule	*chip_;
	Plots		*plots_;
//	Calculation	*calculation_;
	QTimer		*timerUpdate_;

	QWidget		*centralWidget_;
	QPushButton *startButton_;
	QPushButton *goButton_;
	QPushButton	*stopButton_;
	QPushButton	*exitButton_;
	QPushButton *endButton_;
	QPushButton	*showSettings_;
	QPushButton	*showTables_;
	QLineEdit	*pathToSaveL_;
	QLineEdit	*numberChipL_;
	QCheckBox	*isCountChCheck_;
	QHBoxLayout *mainHLayout_, *controlHLayout_, *settingsHLayout_;
	QVBoxLayout *rightVLayout_, *goApplyVLayout_, *addPlotsVLayout_;
	QFormLayout	*saveVLayout_;

	QWidget		*tablesWidget_;
	QTableWidget*dataTable_;
	QTableWidget*homingTable_;
	QHBoxLayout	*tableHLayout_;
	std::array<	std::array<QTableWidgetItem, 4>, 4>	homingItems_;
	std::array<	std::array<QTableWidgetItem, 4>, 4>	dataItems_;
};

#endif // MAINWINDOW_H
