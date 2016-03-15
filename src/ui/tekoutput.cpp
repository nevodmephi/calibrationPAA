#include "tekoutput.h"

tekOutput::tekOutput(tekModule *mod, chipModule *chip, QWidget *parent)
	: QPlainTextEdit(parent) , _module(mod) , _chip(chip),homeID(std::this_thread::get_id())	{
	setReadOnly(true);
	logStream.open("log",ofstream::binary | ofstream::app);
	print("Starting tekControl");
	print(tr("Date: [") + getDate(QDateTime::currentDateTime()) + tr("]"));
	print(tr("Time: [") + getTime(QDateTime::currentDateTime()) + tr("]"));
}

tekOutput::~tekOutput()	{
	print("Closing tekControl");
	if(logStream.is_open())
		logStream.close();
}

void tekOutput::update(const Subject *subject)	{
	if(subject == _module)	{
//		while( _module->getActions().size() )
//		{
//			printAction( _module->getActions().front() );
//			_module->getActions().pop();
//		}
	}
	if (subject == _chip)	{
//		if (_chip->returnMode() == chipModule::mode::amp)
////			print(QString("Amplitude Go:") += QString::number(_chip->returnLastAmp().first));
//			print(QString("Amplitude Go:") += QString::number(_chip->returnLastAmp()->first));
//		if (_chip->returnMode() == chipModule::mode::form)
//			print(QString("Threshold Go:") += QString::number(_chip->returnLastThresh().first));
	}
}

void tekOutput::appendText(const string &text)	{
	appendPlainText(QString::fromStdString(text));
}

void tekOutput::print(const QString &text)	{
	outputLock.lock();
//	appendPlainText(text);
	if(logStream.is_open())
		logStream << text.toUtf8().data() << std::endl;
	outputLock.unlock();
}

void tekOutput::printAction(const actionInfo_s &act)	{
	if(act.errCode == VI_SUCCESS)
		print(getDataTime(QDateTime::currentDateTime()) + tr("\t") +
			  tr("%1 - OK").fromStdString(_module->decodeAction(act.actCode)) );
	else
		print(getDataTime(QDateTime::currentDateTime()) +tr("\t") +
			  tr("%1 - FAILED").fromStdString(_module->decodeAction(act.actCode)) );
	ensureCursorVisible();
}

QString tekOutput::getDate(const QDateTime &dt)	{
	return	tr("%1.%2.%3").
			arg(dt.date().day(),2,10,QChar('0')).
			arg(dt.date().month(),2,10,QChar('0')).
			arg(dt.date().year(),4,10,QChar('0'));
}

QString tekOutput::getTime(const QDateTime &tm)	{
	return	tr("%1:%2:%3").
			arg(tm.time().hour(),2,10,QChar('0')).
			arg(tm.time().minute(),2,10,QChar('0')).
			arg(tm.time().second(),2,10,QChar('0'));
}

QString tekOutput::getDataTime(const QDateTime &tmdt)	{
	return tr("[") + getTime(tmdt) + tr("]");
}
