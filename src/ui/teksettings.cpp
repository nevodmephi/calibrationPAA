#include "teksettings.h"

TekSettings::TekSettings(tekModule *mod, const QString &title, QWidget *parent)
	: QGroupBox(title,parent),module_(mod)	{
	createWidgets();
	createLayouts();
	channel_ = 0;
}

TekSettings::~TekSettings()	{
	disconnect(leftFrontL_,	&QLineEdit::editingFinished,
			   this,		&TekSettings::setLeftFront);
	disconnect(rightFrontL_,	&QLineEdit::editingFinished,
			   this,		&TekSettings::setRightFront);
	disconnect(intervalL_,	&QLineEdit::editingFinished,
			   this,		&TekSettings::setInterval);
	disconnect(widthL_,		&QLineEdit::editingFinished,
			   this,		&TekSettings::setWidth);
	disconnect(highLevelL_,	&QLineEdit::editingFinished,
			this,			&TekSettings::setHighLevel);
	disconnect(lowLevelL_,	&QLineEdit::editingFinished,
			this,			&TekSettings::setLowLevel);
	disconnect(channelOneC_,	&QRadioButton::clicked,
			   this,		&TekSettings::setChannel);
	disconnect(channelTwoC_,	&QRadioButton::clicked,
			   this,		&TekSettings::setChannel);

	for (int i = 0; i < (signed)chipChannel_.size(); i++)
		delete chipChannel_[i];
	delete  leftFrontL_;
	delete  rightFrontL_;
	delete  intervalL_;
	delete  widthL_;
	delete	ampStartL_;
	delete	ampEndL_;
	delete	ampStepL_;
	delete	layout_;
	delete	chipLayout_;
	delete  mainLayout_;
}

void TekSettings::createWidgets()	{
	leftFrontL_		= new QLineEdit("0", this);
	rightFrontL_		= new QLineEdit("0", this);
	intervalL_		= new QLineEdit("0", this);
	widthL_			= new QLineEdit("0", this);
	highLevelL_		= new QLineEdit("0", this);
	lowLevelL_		= new QLineEdit("0", this);
	ampStartL_		= new QLineEdit("0", this);
	ampEndL_			= new QLineEdit("0", this);
	ampStepL_		= new QLineEdit("0", this);
	channelOneC_		= new QRadioButton(this);
	channelTwoC_		= new QRadioButton(this);

	for (int i = 0; i < 4; i++)
		chipChannel_.push_back(new QCheckBox(QString::number(i + 1), this));

	connect(leftFrontL_,	&QLineEdit::editingFinished,
			this,		&TekSettings::setLeftFront);
	connect(rightFrontL_,&QLineEdit::editingFinished,
			this,		&TekSettings::setRightFront);
	connect(intervalL_,	&QLineEdit::editingFinished,
			this,		&TekSettings::setInterval);
	connect(widthL_,		&QLineEdit::editingFinished,
			this,		&TekSettings::setWidth);
	connect(highLevelL_,	&QLineEdit::editingFinished,
			this,		&TekSettings::setHighLevel);
	connect(lowLevelL_,	&QLineEdit::editingFinished,
			this,		&TekSettings::setLowLevel);
	connect(channelOneC_,&QRadioButton::clicked,
			this,		&TekSettings::setChannel);
	connect(channelTwoC_,&QRadioButton::clicked,
			this,		&TekSettings::setChannel);
	channelOneC_->setChecked(true);
	setChannel();
}

void TekSettings::createLayouts()	{
	layout_				= new QFormLayout;
	layout_->addRow(tr("Channel One"),channelOneC_);
	layout_->addRow(tr("Channel Two"),channelTwoC_);
	layout_->addRow(tr("Left Front"),leftFrontL_);
	layout_->addRow(tr("Right Front"),rightFrontL_);
	layout_->addRow(tr("Interval"),intervalL_);
	layout_->addRow(tr("Width"),widthL_);
	layout_->addRow(tr("High Level"),highLevelL_);
	layout_->addRow(tr("Low Level"),lowLevelL_);
	layout_->addRow(tr("Start Level"), ampStartL_);
	layout_->addRow(tr("End Level"), ampEndL_);
	layout_->addRow(tr("Step Level"), ampStepL_);
	chipLayout_			= new QVBoxLayout;
	for (int i = 0; i < (signed)chipChannel_.size(); i++)
		chipLayout_->addWidget(chipChannel_[i]);
	mainLayout_			= new QHBoxLayout;
	mainLayout_->addLayout(layout_);
	mainLayout_->addLayout(chipLayout_);
	setLayout(mainLayout_);
}

void TekSettings::update(const Subject *subject)	{
	if(subject == module_)	{
		updateSettings();
	}
}

void TekSettings::updateSettings()	{
	int val;

	int countCheck = 0;
	for (auto check : chipChannel_)
		if (check->isChecked() == true)
			countCheck++;
	if (countCheck != 2)
		return;

	if( module_->getWidth(val) )
		widthL_->setText(QString::number(val,10));
	if( module_->getHighLevel(val) )
		highLevelL_->setText(QString::number(val,10));
	if( module_->getLowLevel(val) )
		lowLevelL_->setText(QString::number(val,10));
	if( module_->getLeftFront(val) )
		leftFrontL_->setText(QString::number(val,10));
	if( module_->getRightFront(val) )
		rightFrontL_->setText(QString::number(val,10));
	if( module_->getInterval(val) )
		intervalL_->setText(QString::number(val,10));
}

void TekSettings::setChannel()	{
	if(channel_ == getCurrentChannel() ) return;
	channel_ = getCurrentChannel();
	module_->setChannel( channel_ );
	module_->notify();
}

void TekSettings::setLeftFront()	{
	module_->setLeftFront( leftFrontL_->text().toInt() );
	module_->notify();
}

void TekSettings::setRightFront()	{
	module_->setRightFront( rightFrontL_->text().toInt() );
	module_->notify();
}

void TekSettings::setInterval()	{
	module_->setInterval( intervalL_->text().toInt() );
	module_->notify();
}

void TekSettings::setWidth()	{
	module_->setWidth( widthL_->text().toInt() );
	module_->notify();
}

void TekSettings::setHighLevel()	{
	module_->setHighLevel( highLevelL_->text().toInt() );
	module_->notify();
}

void TekSettings::setLowLevel()	{
	module_->setLowLevel( lowLevelL_->text().toInt() );
	module_->notify();
}

void TekSettings::setSettings()	{
	module_->setRightFront	( rightFrontL_->text().toInt() );
	module_->setLeftFront	( leftFrontL_->text().toInt() );
	module_->setHighLevel	( highLevelL_->text().toInt() );
	module_->setLowLevel	( lowLevelL_->text().toInt() );
	module_->setInterval	( intervalL_->text().toInt() );
	module_->setWidth		( widthL_->text().toInt() );

}

void TekSettings::readSettings()	{
	highLevelL_->	setText(QString::number(module_->getSettgins().highLevel));
	lowLevelL_->		setText(QString::number(module_->getSettgins().lowLevel));
	intervalL_->		setText(QString::number(module_->getSettgins().interval));
	widthL_->		setText(QString::number(module_->getSettgins().width));
	leftFrontL_->	setText(QString::number(module_->getSettgins().leftFront));
	rightFrontL_->	setText(QString::number(module_->getSettgins().rightFront));
}

int TekSettings::getCurrentChannel()	{
	if(channelOneC_->isChecked()) return 0;
	if(channelTwoC_->isChecked()) return 1;
	return -1;
}

TekSettings::charVec TekSettings::getChipChannel()	const	{
	std::vector<char> result(4);
	for (int i = 0; i < (signed)chipChannel_.size(); i++)
		if (chipChannel_[i]->isChecked() == true)
			result[i] = '1';
		else
			result[i] = '0';
	return result;
}
