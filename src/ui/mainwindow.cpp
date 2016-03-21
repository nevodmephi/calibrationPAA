#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)	{

//	module_			= new tekModule;
//	calculation_	= new Calculation;
//	chip_			= new PaaModule	(module_, calculation_, "192.168.2.41");
	calibration_	= new Calibration;
	output_			= new TekOutput		(calibration_->getGenModule().get(), calibration_->getPaaModule().get(), this);
	settings_		= new TekSettings	(calibration_->getGenModule().get(),tr("Settings"));
	plots_			= new Plots			(calibration_->getPaaModule().get(), this);
	timerUpdate_	= new QTimer		(this);

	initializeElements();
	initializeLayouts();
	centralWidget_->setLayout(addPlotsVLayout_);
	settings_->hide();
	tablesWidget_->setWindowTitle("Tables Window");
	dataTable_->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	dataTable_->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	homingTable_->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	homingTable_->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			homingTable_->setItem(i, j, &homingItems_[i][j]);

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			dataTable_->setItem(i, j, &dataItems_[i][j]);

	connect(exitButton_,	&QPushButton::clicked,
			this,			&MainWindow::close);
	connect(startButton_,	&QPushButton::clicked,
			this,			&MainWindow::startButtonClick);
	connect(endButton_,		&QPushButton::clicked,
			this,			&MainWindow::endButtonClick);
	connect(goButton_,		&QPushButton::clicked,
			this,			&MainWindow::goButtonClick);
	connect(showSettings_,	&QPushButton::clicked,
			this,			&MainWindow::showSettingsClick);
	connect(stopButton_,	&QPushButton::clicked,
			this,			&MainWindow::stopButtonClick);
	connect(showTables_,	&QPushButton::clicked,
			this,			&MainWindow::showTablesClick);
	connect(timerUpdate_,	&QTimer::timeout,
			this,			&MainWindow::dataUpdate);

	resize(1024, 768);
	//	module_->attach(output_);
	//	module_->attach(settings_);
	//	chip_->attach(output_);
	//	chip_->attach(plots_);
}

MainWindow::~MainWindow()	{
	disconnect(exitButton_,		&QPushButton::clicked,
			this,				&MainWindow::close);
	disconnect(startButton_,		&QPushButton::clicked,
			this,				&MainWindow::startButtonClick);
	disconnect(endButton_,		&QPushButton::clicked,
			this,				&MainWindow::endButtonClick);
	disconnect(goButton_,		&QPushButton::clicked,
			this,				&MainWindow::goButtonClick);
	disconnect(showSettings_,	&QPushButton::clicked,
			this,				&MainWindow::showSettingsClick);
	disconnect(stopButton_,		&QPushButton::clicked,
			this,				&MainWindow::stopButtonClick);
	disconnect(showTables_,		&QPushButton::clicked,
			this,				&MainWindow::showTablesClick);
	disconnect(timerUpdate_,	&QTimer::timeout,
			this,				&MainWindow::dataUpdate);

	delete	centralWidget_;
//	delete	calculation;
//	delete	chip_;
	delete	timerUpdate_;
}

void MainWindow::initializeLayouts()	{
	centralWidget_ = new QWidget(this);
	tablesWidget_ =	new QWidget;
	setCentralWidget(centralWidget_);

	mainHLayout_ =		new QHBoxLayout;
	rightVLayout_ =		new QVBoxLayout;
	controlHLayout_ =	new QHBoxLayout;
	settingsHLayout_ =	new QHBoxLayout;
	goApplyVLayout_ =	new QVBoxLayout;
	addPlotsVLayout_ =	new QVBoxLayout;
	tableHLayout_ =		new QHBoxLayout;
	saveVLayout_ =		new QFormLayout;
	settingsHLayout_->	addLayout(goApplyVLayout_);
	controlHLayout_->	addWidget(startButton_);
	controlHLayout_->	addWidget(showSettings_);
	controlHLayout_->	addWidget(showTables_);
	controlHLayout_->	addWidget(goButton_);
	controlHLayout_->	addWidget(stopButton_);
	controlHLayout_->	addWidget(endButton_);
	controlHLayout_->	addWidget(exitButton_);
	saveVLayout_->		addRow("Path to save folder:", pathToSaveL_);
	saveVLayout_->		addRow("Name chip", numberChipL_);\
	rightVLayout_->		addLayout(controlHLayout_);
	rightVLayout_->		addLayout(saveVLayout_);
	rightVLayout_->		addWidget(isCountChCheck_);
//	rightVLayout->		addWidget(settings);
	mainHLayout_->		addWidget(output_);
	mainHLayout_->		addLayout(rightVLayout_);
	addPlotsVLayout_->	addLayout(mainHLayout_, 1);
	addPlotsVLayout_->	addWidget(plots_, 4);

	tableHLayout_->		addWidget(dataTable_);
	tableHLayout_->		addWidget(homingTable_);
	tablesWidget_->		setLayout(tableHLayout_);
}

void MainWindow::initializeElements()	{
	startButton_ =	new QPushButton("Start session",this);
	goButton_ =		new QPushButton("Go Calibration", this);
	endButton_ =		new QPushButton("End session",this);
	exitButton_ =	new QPushButton("Exit",this);
	showSettings_ =	new QPushButton("Show Settings", this);
	showTables_ =	new QPushButton("Show Tables", this);
	stopButton_ =	new QPushButton("Stop Calibration", this);
	pathToSaveL_ =	new QLineEdit("/home/main/data/", this);
	numberChipL_ =	new QLineEdit(this);
	isCountChCheck_ =new QCheckBox(QString("One Or Two Calibration"), this);

	dataTable_ =		new QTableWidget(4, 4, this);
	homingTable_ =	new QTableWidget(4, 4, this);
}

void MainWindow::addToList(const string &addStr)	{
	output_->appendPlainText(QString::fromStdString(addStr));
	output_->ensureCursorVisible();
}

void MainWindow::startButtonClick()	{
//	module_->setChannel( settings_->currentChannel() + 1);
//	bool status = module_->openSession();
//	if (status == true)
//		output_->appendText(string("Generator Connected"));
//	else
//		output_->appendText(string("Generator Connection Error"));
//	module_->notify();
//	status = chip_->openSession();
//	if (status == true)
//		output_->appendText(string("Controller Connected"));
//	else
//		output_->appendText(string("Controller Connection Error"));
}

void MainWindow::endButtonClick()	{
//	module_->closeSession();
//	chip_->closeSession();
//	module_->notify();
}

void MainWindow::showSettingsClick()	{
	if (settings_->isHidden() == true)
		settings_->show();
	else
		settings_->hide();
}

void MainWindow::goButtonClick()	{
//	if (numberChipL_->text().isEmpty() == true)
//		return;
//	int howChipChannel = chip_->searchAndSetChipChannel();
//	if (howChipChannel == -1)	{
//		std::cout << "dsd" << std::endl;
//		return;
//	}
//	if (howChipChannel == 0)	{
//		chip_->setChannels(std::pair<int, int>(0, 1));
//		chip_->setGroupChannel(0);
//		calculation_->setChipChannel(0);
//	}
//	if (howChipChannel == 1)	{
//		chip_->setChannels(std::pair<int, int>(2, 3));
//		chip_->setGroupChannel(1);
//		calculation_->setChipChannel(1);
//	}
//	if (howChipChannel == 2)	{
//		chip_->setChannels(std::pair<int, int>(4, 5));
//		chip_->setGroupChannel(0);
//		calculation_->setChipChannel(0);
//	}
//	if (howChipChannel == 3)	{
//		chip_->setChannels(std::pair<int, int>(6, 7));
//		chip_->setGroupChannel(1);
//		calculation_->setChipChannel(1);
//	}

//	string	pathToSave	= pathToSaveL_->text().toStdString() + numberChipL_->text().toStdString() + string("/");
//	QDir().mkdir(pathToSaveL_->text() + numberChipL_->text());
//	chip_->setPathToSave(pathToSave);
//	calculation_->setPathToFile(pathToSave);
//	plots_->allClear();
//	chip_->calibration(50, 10);
//	timerUpdate_->start(20);
}

void MainWindow::stopButtonClick()	{
//	chip_->stopCalibration();
//	timerUpdate_->stop();
}

void MainWindow::showTablesClick()	{
//	if (tablesWidget_->isHidden() == true)	{
//		calculation_->setPathToFile("/home/main/data/");
//		calculation_->readHomingFromFiles();
//		writeToHomingTable();
//		writeToDataTable();
//		tablesWidget_->show();
//	}
//	else
//		tablesWidget_->hide();
}

void MainWindow::writeToHomingTable()	{
//	for (int i = 0; i < 4; i++)
//		for (int j = 0; j < 4; j++)	{
//			homingItems_[i][j].setText(
//						QString::number(calculation_->returnHoming()[i][j]));
//		}
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
//	std::mutex	block;
//	block.lock();
//	if (chip_->getDataUpdate() == true)	{
//		plots_->dataUpdate();
//		auto mode = chip_->returnMode();
//		if (mode == PaaModule::mode::amp)	{
//			auto	lastAmp	= chip_->returnLastAmp();
//			output_->appendText(string("Amp:    ") + std::to_string(lastAmp.first));
//			output_->appendText(string("Code:   ") + std::to_string(lastAmp.second[0]) + string("\t") + std::to_string(lastAmp.second[1]));
//		}
//		if (mode == PaaModule::mode::form)	{
//			auto	lastThresh	= chip_->returnLastThresh();
//			output_->appendText(string("Thresh: ") + std::to_string(lastThresh.first));
//			output_->appendText(string("Code:   ") + std::to_string(lastThresh.second));
//		}
//	}
//	block.unlock();
}
