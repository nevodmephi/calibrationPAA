#include "tekoutput.h"

TekOutput::TekOutput(tekModule *mod, PaaModule *chip, QWidget *parent)
	: QPlainTextEdit(parent) , module_(mod) , chip_(chip),homeID_(std::this_thread::get_id())	{

	setReadOnly(true);
	logStream_.open("log",ofstream::binary | ofstream::app);
	print("Starting tekControl");
	print(tr("Date: [") + getDate(QDateTime::currentDateTime()) + tr("]"));
	print(tr("Time: [") + getTime(QDateTime::currentDateTime()) + tr("]"));
}

TekOutput::~TekOutput()	{
	print("Closing tekControl");
	if(logStream_.is_open())
		logStream_.close();
}

void TekOutput::update(const Subject *subject)	{
	if(subject == module_)	{
//		while( _module->getActions().size() )
//		{
//			printAction( _module->getActions().front() );
//			_module->getActions().pop();
//		}
	}
	if (subject == chip_)	{
//		if (_chip->returnMode() == chipModule::mode::amp)
////			print(QString("Amplitude Go:") += QString::number(_chip->returnLastAmp().first));
//			print(QString("Amplitude Go:") += QString::number(_chip->returnLastAmp()->first));
//		if (_chip->returnMode() == chipModule::mode::form)
//			print(QString("Threshold Go:") += QString::number(_chip->returnLastThresh().first));
	}
}

void TekOutput::appendText(const string &text)	{
	appendPlainText(QString::fromStdString(text));
}

void TekOutput::print(const QString &text)	{
	outputLock_.lock();
//	appendPlainText(text);
	if(logStream_.is_open())
		logStream_ << text.toUtf8().data() << std::endl;
	outputLock_.unlock();
}

void TekOutput::printAction(const ActionInfo_s &act)	{
	if(act.errCode == VI_SUCCESS)
		print(getDataTime(QDateTime::currentDateTime()) + tr("\t") +
			  tr("%1 - OK").fromStdString(module_->decodeAction(act.actCode)) );
	else
		print(getDataTime(QDateTime::currentDateTime()) +tr("\t") +
			  tr("%1 - FAILED").fromStdString(module_->decodeAction(act.actCode)) );
	ensureCursorVisible();
}

QString TekOutput::getDate(const QDateTime &dt)	{
	return	tr("%1.%2.%3").
			arg(dt.date().day(),2,10,QChar('0')).
			arg(dt.date().month(),2,10,QChar('0')).
			arg(dt.date().year(),4,10,QChar('0'));
}

QString TekOutput::getTime(const QDateTime &tm)	{
	return	tr("%1:%2:%3").
			arg(tm.time().hour(),2,10,QChar('0')).
			arg(tm.time().minute(),2,10,QChar('0')).
			arg(tm.time().second(),2,10,QChar('0'));
}

QString TekOutput::getDataTime(const QDateTime &tmdt)	{
	return tr("[") + getTime(tmdt) + tr("]");
}
