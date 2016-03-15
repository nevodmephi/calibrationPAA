#include "plots.h"

Plots::Plots(ChipModule *chip, QWidget *parent)
	: QWidget(parent), chip_(chip)	{
	ampFirst_	=	new QCustomPlot(this);
	ampSecond_	=	new QCustomPlot(this);
	form_		=	new QCustomPlot(this);

	ampHLayout_	=	new QHBoxLayout;
	formVLayout_	=	new QVBoxLayout;
	isRescaleButton_	= new QCheckBox("auto Rescale", this);
	ampHLayout_->	addWidget(ampFirst_);
	ampHLayout_->	addWidget(ampSecond_);
	formVLayout_->	addWidget(isRescaleButton_);
	formVLayout_->	addLayout(ampHLayout_);
	formVLayout_->	addWidget(form_);

	setLayout(formVLayout_);
	createFirst();
	createSecond();
	createForm();
}

Plots::~Plots()	{
	delete isRescaleButton_;
	delete ampFirst_;
	delete ampSecond_;
	delete form_;
	delete ampHLayout_;
	delete formVLayout_;
}

void Plots::update(const Subject *subject)	{
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

void Plots::dataUpdate()	{
	std::mutex	block;
	block.lock();
	if (chip_->getDataUpdate() == true)
	{
		if (chip_->returnMode() == ChipModule::mode::amp)
		{
			ampFirstData_.first.push_back(-chip_->returnLastAmp().first);
			ampFirstData_.second.push_back(-chip_->returnLastAmp().second[0]);
			ampSecondData_.first.push_back(-chip_->returnLastAmp().first);
			ampSecondData_.second.push_back(-chip_->returnLastAmp().second[1]);
			renderFirst();
			renderSecond();
		}

		if (chip_->returnMode() == ChipModule::mode::form)
		{
			auto thresh = chip_->returnLastThresh();
			if (thresh.first != -100 && thresh.second != -100)
			{
				formData_.first.push_back(chip_->returnLastThresh().first);
				formData_.second.push_back(-chip_->returnLastThresh().second);
				renderForm();
			}
		}
		chip_->setDataUpdateFalse();
	}
	block.unlock();
}

void Plots::allClear()	{
	ampFirstData_.first.clear();
	ampFirstData_.second.clear();
	ampSecondData_.first.clear();
	ampSecondData_.second.clear();
	formData_.first.clear();
	formData_.second.clear();
	ampFirst_->replot();
	ampSecond_->replot();
	form_->replot();
}

void Plots::createFirst()	{
	ampFirst_->addGraph();
//	ampFirst->graph(0)->setPen(QPen(QColor(0, 0, 255, 20)));
	ampFirst_->graph(0)->setLineStyle(QCPGraph::lsLine);
	ampFirst_->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));
	ampFirst_->graph(0)->setName(QString("First Channel"));
//	ampFirst->addGraph();
//	ampFirst->graph(1)->setPen(QPen(QColor(0, 255, 0, 20)));
//	ampFirst->graph(1)->setLineStyle(QCPGraph::lsNone);
//	ampFirst->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));
//	ampFirst->graph(1)->setName(QString("Second channel"));
	ampFirst_->xAxis->setLabel(QString("Amplitude, mV"));
	ampFirst_->yAxis->setLabel(QString("Code PAA"));

	ampFirst_->rescaleAxes();

	ampFirst_->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
}

void Plots::createSecond()	{
	ampSecond_->addGraph();
	ampSecond_->graph(0)->setLineStyle(QCPGraph::lsLine);
	ampSecond_->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));
	ampSecond_->graph(0)->setName(QString("Second Channel"));
	ampSecond_->xAxis->setLabel(QString("Amplitude, mV"));
	ampSecond_->yAxis->setLabel(QString("Code PAA"));
//	ampSecond->graph(0)->setName(QString("Second Channel"));

	ampSecond_->rescaleAxes();

	ampSecond_->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
}

void Plots::createForm()	{
	form_->addGraph();
	form_->graph(0)->setLineStyle(QCPGraph::lsNone);
	form_->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));
	form_->xAxis->setLabel(QString("Threshold"));
	form_->yAxis->setLabel(QString("Amplitude, mV"));
	form_->graph(0)->setName(QString("Form"));

	form_->rescaleAxes();

	form_->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
}

void Plots::renderFirst()	{
	ampFirst_->graph(0)->setData(ampFirstData_.first, ampFirstData_.second);
	ampFirst_->replot();
	if (isRescaleButton_->isChecked() == true)
		ampFirst_->rescaleAxes();
}

void Plots::renderSecond()	{
	ampSecond_->graph(0)->setData(ampSecondData_.first, ampSecondData_.second);
	ampSecond_->replot();
	if (isRescaleButton_->isChecked() == true)
		ampSecond_->rescaleAxes();
}

void Plots::renderForm()	{
	form_->graph(0)->setData(formData_.first, formData_.second);
	form_->replot();
	if (isRescaleButton_->isChecked() == true)
		form_->rescaleAxes();
}
