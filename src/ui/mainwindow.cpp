#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)	{
	module			= new tekModule;
	calculation		= new processing;
	chip			= new chipModule	(module, calculation);
	output			= new tekOutput		(module, chip, this);
	settings		= new tekSettings	(module,tr("Settings"));
	plots			= new plotsOutput	(chip, this);
	_timerUpdate	= new QTimer		(this);

	initializeElements();
	initializeLayouts();
	centralWidget->setLayout(addPlotsVLayout);
	settings->hide();
	tablesWidget->setWindowTitle("Tables Window");
	dataTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	dataTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	homingTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	homingTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			homingTable->setItem(i, j, &homingItems[i][j]);

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			dataTable->setItem(i, j, &dataItems[i][j]);

	connect(exitButton,		&QPushButton::clicked,
			this,			&MainWindow::close);
	connect(startButton,	&QPushButton::clicked,
			this,			&MainWindow::startButtonClick);
	connect(endButton,		&QPushButton::clicked,
			this,			&MainWindow::endButtonClick);
	connect(goButton,		&QPushButton::clicked,
			this,			&MainWindow::goButtonClick);
	connect(showSettings,	&QPushButton::clicked,
			this,			&MainWindow::showSettingsClick);
	connect(stopButton,		&QPushButton::clicked,
			this,			&MainWindow::stopButtonClick);
	connect(showTables,		&QPushButton::clicked,
			this,			&MainWindow::showTablesClick);
	connect(_timerUpdate,	&QTimer::timeout,
			this,			&MainWindow::dataUpdate);

	resize(1024, 768);
	module->attach(output);
	module->attach(settings);
	chip->attach(output);
	chip->attach(plots);
}

MainWindow::~MainWindow()	{
	disconnect(exitButton,		&QPushButton::clicked,
			this,				&MainWindow::close);
	disconnect(startButton,		&QPushButton::clicked,
			this,				&MainWindow::startButtonClick);
	disconnect(endButton,		&QPushButton::clicked,
			this,				&MainWindow::endButtonClick);
	disconnect(goButton,		&QPushButton::clicked,
			this,				&MainWindow::goButtonClick);
	disconnect(showSettings,	&QPushButton::clicked,
			this,				&MainWindow::showSettingsClick);
	disconnect(stopButton,		&QPushButton::clicked,
			this,				&MainWindow::stopButtonClick);
	disconnect(showTables,		&QPushButton::clicked,
			this,				&MainWindow::showTablesClick);
	disconnect(_timerUpdate,	&QTimer::timeout,
			this,				&MainWindow::dataUpdate);

	delete	centralWidget;
//	delete	calculation;
	delete	chip;
	delete	_timerUpdate;
}

void MainWindow::initializeLayouts()	{
	centralWidget = new QWidget(this);
	tablesWidget =	new QWidget;
	setCentralWidget(centralWidget);

	mainHLayout =		new QHBoxLayout;
	rightVLayout =		new QVBoxLayout;
	controlHLayout =	new QHBoxLayout;
	settingsHLayout =	new QHBoxLayout;
	goApplyVLayout =	new QVBoxLayout;
	addPlotsVLayout =	new QVBoxLayout;
	tableHLayout =		new QHBoxLayout;
	saveVLayout =		new QFormLayout;
	settingsHLayout->	addLayout(goApplyVLayout);
	controlHLayout->	addWidget(startButton);
	controlHLayout->	addWidget(showSettings);
	controlHLayout->	addWidget(showTables);
	controlHLayout->	addWidget(goButton);
	controlHLayout->	addWidget(stopButton);
	controlHLayout->	addWidget(endButton);
	controlHLayout->	addWidget(exitButton);
	saveVLayout->		addRow("Path to save folder:", pathToSaveL);
	saveVLayout->		addRow("Name chip", numberChipL);\
	rightVLayout->		addLayout(controlHLayout);
	rightVLayout->		addLayout(saveVLayout);
	rightVLayout->		addWidget(isCountChCheck);
//	rightVLayout->		addWidget(settings);
	mainHLayout->		addWidget(output);
	mainHLayout->		addLayout(rightVLayout);
	addPlotsVLayout->	addLayout(mainHLayout, 1);
	addPlotsVLayout->	addWidget(plots, 4);

	tableHLayout->		addWidget(dataTable);
	tableHLayout->		addWidget(homingTable);
	tablesWidget->		setLayout(tableHLayout);
}

void MainWindow::initializeElements()	{
	startButton =	new QPushButton("Start session",this);
	goButton =		new QPushButton("Go Calibration", this);
	endButton =		new QPushButton("End session",this);
	exitButton =	new QPushButton("Exit",this);
	showSettings =	new QPushButton("Show Settings", this);
	showTables =	new QPushButton("Show Tables", this);
	stopButton =	new QPushButton("Stop Calibration", this);
	pathToSaveL =	new QLineEdit("/home/main/data/", this);
	numberChipL =	new QLineEdit(this);
	isCountChCheck =new QCheckBox(QString("One Or Two Calibration"), this);

	dataTable =		new QTableWidget(4, 4, this);
	homingTable =	new QTableWidget(4, 4, this);
}

void MainWindow::addToList(const string &addStr)	{
	output->appendPlainText(QString::fromStdString(addStr));
	output->ensureCursorVisible();
}

void MainWindow::startButtonClick()	{
	module->setChannel( settings->currentChannel() + 1);
	bool status = module->openSession();
	if (status == true)
		output->appendText(string("Generator Connected"));
	else
		output->appendText(string("Generator Connection Error"));
	module->notify();
	status = chip->openSession();
	if (status == true)
		output->appendText(string("Controller Connected"));
	else
		output->appendText(string("Controller Connection Error"));
}

void MainWindow::endButtonClick()	{
	module->closeSession();
	chip->closeSession();
	module->notify();
}

void MainWindow::showSettingsClick()	{
	if (settings->isHidden() == true)
		settings->show();
	else
		settings->hide();
}

void MainWindow::goButtonClick()	{
	if (numberChipL->text().isEmpty() == true)
		return;
	int howChipChannel = chip->searchAndSetChipChannel();
	if (howChipChannel == -1)	{
		std::cout << "dsd" << std::endl;
		return;
	}
	if (howChipChannel == 0)	{
		chip->setChannels(std::pair<int, int>(0, 1));
		chip->setGroupChannel(0);
		calculation->setChipChannel(0);
	}
	if (howChipChannel == 1)	{
		chip->setChannels(std::pair<int, int>(2, 3));
		chip->setGroupChannel(1);
		calculation->setChipChannel(1);
	}
	if (howChipChannel == 2)	{
		chip->setChannels(std::pair<int, int>(4, 5));
		chip->setGroupChannel(0);
		calculation->setChipChannel(0);
	}
	if (howChipChannel == 3)	{
		chip->setChannels(std::pair<int, int>(6, 7));
		chip->setGroupChannel(1);
		calculation->setChipChannel(1);
	}

	string	pathToSave	= pathToSaveL->text().toStdString() + numberChipL->text().toStdString() + string("/");
	QDir().mkdir(pathToSaveL->text() + numberChipL->text());
	chip->setPathToSave(pathToSave);
	calculation->setPathToFile(pathToSave);
	plots->allClear();
	chip->calibration(50, 10);
	_timerUpdate->start(20);
}

void MainWindow::stopButtonClick()	{
	chip->stopCalibration();
	_timerUpdate->stop();
}

void MainWindow::showTablesClick()	{
	if (tablesWidget->isHidden() == true)	{
		calculation->setPathToFile("/home/main/data/");
		calculation->readHomingFromFiles();
		writeToHomingTable();
		writeToDataTable();
		tablesWidget->show();
	}
	else
		tablesWidget->hide();
}

void MainWindow::writeToHomingTable()	{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)	{
			homingItems[i][j].setText(
						QString::number(calculation->returnHoming()[i][j]));
		}
}

void MainWindow::writeToDataTable()	{
//	for (int j = 0; j < 4; j++)
//	{
//		dataItems[j][0].setText(
//					QString::number(calculation->returnData().transformationADC[j]));
//		dataItems[j][1].setText(
//					QString::number(calculation->returnData().errorADC[j]));
//		dataItems[j][2].setText(
//					QString::number(calculation->returnData().nonlinearity[j]));
//	}
}

void MainWindow::dataUpdate()	{
	std::mutex	block;
	block.lock();
	if (chip->getDataUpdate() == true)	{
		plots->dataUpdate();
		auto mode = chip->returnMode();
		if (mode == chipModule::mode::amp)	{
			auto	lastAmp	= chip->returnLastAmp();
			output->appendText(string("Amp:    ") + std::to_string(lastAmp.first));
			output->appendText(string("Code:   ") + std::to_string(lastAmp.second[0]) + string("\t") + std::to_string(lastAmp.second[1]));
		}
		if (mode == chipModule::mode::form)	{
			auto	lastThresh	= chip->returnLastThresh();
			output->appendText(string("Thresh: ") + std::to_string(lastThresh.first));
			output->appendText(string("Code:   ") + std::to_string(lastThresh.second));
		}
	}
	block.unlock();
}
