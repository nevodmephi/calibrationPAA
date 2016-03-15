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
#include "../tekVisa/processing.h"
#include "chip/blocks.h"

class MainWindow : public QMainWindow	{
	Q_OBJECT

public:
	using	itemsArray	=	std::array<	std::array<QTableWidgetItem, 4>, 4>;
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

private:
	QWidget		*centralWidget;
	QPushButton *startButton;
	QPushButton *goButton;
	QPushButton	*stopButton;
	QPushButton	*exitButton;
	QPushButton *endButton;
	QPushButton	*showSettings;
	QPushButton	*showTables;
	QLineEdit	*pathToSaveL;
	QLineEdit	*numberChipL;
	QCheckBox	*isCountChCheck;
	QHBoxLayout *mainHLayout, *controlHLayout, *settingsHLayout;
	QVBoxLayout *rightVLayout, *goApplyVLayout, *addPlotsVLayout;
	QFormLayout	*saveVLayout;

	QWidget		*tablesWidget;
	QTableWidget*dataTable;
	QTableWidget*homingTable;
	QHBoxLayout	*tableHLayout;
	itemsArray	homingItems;
	itemsArray	dataItems;

	tekModule	*module;
	tekOutput	*output;
	tekSettings *settings;
	chipModule	*chip;
	plotsOutput	*plots;
	processing	*calculation;
	QTimer*		_timerUpdate;
};

#endif // MAINWINDOW_H
