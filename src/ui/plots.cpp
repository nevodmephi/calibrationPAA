#include "plots.h"

plotsOutput::plotsOutput(chipModule *chip, QWidget *parent)
	: QWidget(parent), _chip(chip)	{
	_ampFirst	=	new QCustomPlot(this);
	_ampSecond	=	new QCustomPlot(this);
	_form		=	new QCustomPlot(this);

	_ampHLayout	=	new QHBoxLayout;
	_formVLayout	=	new QVBoxLayout;
	_isRescaleButton	= new QCheckBox("auto Rescale", this);
	_ampHLayout->	addWidget(_ampFirst);
	_ampHLayout->	addWidget(_ampSecond);
	_formVLayout->	addWidget(_isRescaleButton);
	_formVLayout->	addLayout(_ampHLayout);
	_formVLayout->	addWidget(_form);

	setLayout(_formVLayout);
	createFirst();
	createSecond();
	createForm();
}

plotsOutput::~plotsOutput()	{
	delete _isRescaleButton;
	delete _ampFirst;
	delete _ampSecond;
	delete _form;
	delete _ampHLayout;
	delete _formVLayout;
}

void plotsOutput::update(const Subject *subject)	{
//	if (subject == _chip)
//	{
//		if (_chip->returnMode() == chipModule::mode::amp)
//		{
//			ampFirstData.first.push_back(_chip->returnLastAmp()->first);
//			ampFirstData.second.push_back(_chip->returnLastAmp()->second[0]);
//			ampSecondData.first.push_back(_chip->returnLastAmp()->first);
//			ampSecondData.second.push_back(_chip->returnLastAmp()->second[1]);
//			renderFirst();
//			renderSecond();
//		}
//		if (_chip->returnMode() == chipModule::mode::form)
//		{
//			formData.first.push_back(_chip->returnLastThresh().first);
//			formData.second.push_back(_chip->returnLastThresh().second);
//			renderForm();
//		}
//	}
}

void plotsOutput::dataUpdate()	{
	std::mutex	block;
	block.lock();
	if (_chip->getDataUpdate() == true)
	{
		if (_chip->returnMode() == chipModule::mode::amp)
		{
			_ampFirstData.first.push_back(-_chip->returnLastAmp().first);
			_ampFirstData.second.push_back(-_chip->returnLastAmp().second[0]);
			_ampSecondData.first.push_back(-_chip->returnLastAmp().first);
			_ampSecondData.second.push_back(-_chip->returnLastAmp().second[1]);
			renderFirst();
			renderSecond();
		}

		if (_chip->returnMode() == chipModule::mode::form)
		{
			auto thresh = _chip->returnLastThresh();
			if (thresh.first != -100 && thresh.second != -100)
			{
				_formData.first.push_back(_chip->returnLastThresh().first);
				_formData.second.push_back(-_chip->returnLastThresh().second);
				renderForm();
			}
		}
		_chip->setDataUpdateFalse();
	}
	block.unlock();
}

void plotsOutput::allClear()	{
	_ampFirstData.first.clear();
	_ampFirstData.second.clear();
	_ampSecondData.first.clear();
	_ampSecondData.second.clear();
	_formData.first.clear();
	_formData.second.clear();
	_ampFirst->replot();
	_ampSecond->replot();
	_form->replot();
}

void plotsOutput::createFirst()	{
	_ampFirst->addGraph();
//	ampFirst->graph(0)->setPen(QPen(QColor(0, 0, 255, 20)));
	_ampFirst->graph(0)->setLineStyle(QCPGraph::lsLine);
	_ampFirst->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));
	_ampFirst->graph(0)->setName(QString("First Channel"));
//	ampFirst->addGraph();
//	ampFirst->graph(1)->setPen(QPen(QColor(0, 255, 0, 20)));
//	ampFirst->graph(1)->setLineStyle(QCPGraph::lsNone);
//	ampFirst->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));
//	ampFirst->graph(1)->setName(QString("Second channel"));
	_ampFirst->xAxis->setLabel(QString("Amplitude, mV"));
	_ampFirst->yAxis->setLabel(QString("Code PAA"));

	_ampFirst->rescaleAxes();

	_ampFirst->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
}

void plotsOutput::createSecond()	{
	_ampSecond->addGraph();
	_ampSecond->graph(0)->setLineStyle(QCPGraph::lsLine);
	_ampSecond->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));
	_ampSecond->graph(0)->setName(QString("Second Channel"));
	_ampSecond->xAxis->setLabel(QString("Amplitude, mV"));
	_ampSecond->yAxis->setLabel(QString("Code PAA"));
//	ampSecond->graph(0)->setName(QString("Second Channel"));

	_ampSecond->rescaleAxes();

	_ampSecond->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
}

void plotsOutput::createForm()	{
	_form->addGraph();
	_form->graph(0)->setLineStyle(QCPGraph::lsNone);
	_form->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));
	_form->xAxis->setLabel(QString("Threshold"));
	_form->yAxis->setLabel(QString("Amplitude, mV"));
	_form->graph(0)->setName(QString("Form"));

	_form->rescaleAxes();

	_form->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
}

void plotsOutput::renderFirst()	{
	_ampFirst->graph(0)->setData(_ampFirstData.first, _ampFirstData.second);
	_ampFirst->replot();
	if (_isRescaleButton->isChecked() == true)
		_ampFirst->rescaleAxes();
}

void plotsOutput::renderSecond()	{
	_ampSecond->graph(0)->setData(_ampSecondData.first, _ampSecondData.second);
	_ampSecond->replot();
	if (_isRescaleButton->isChecked() == true)
		_ampSecond->rescaleAxes();
}

void plotsOutput::renderForm()	{
	_form->graph(0)->setData(_formData.first, _formData.second);
	_form->replot();
	if (_isRescaleButton->isChecked() == true)
		_form->rescaleAxes();
}
