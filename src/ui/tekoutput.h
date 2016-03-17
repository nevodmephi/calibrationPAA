#ifndef TEKOUTPUT_H
#define TEKOUTPUT_H

#include <QPlainTextEdit>
#include <QDateTime>
#include <QString>
#include <fstream>
#include <mutex>
#include <thread>

#include "../observer/observer.h"
#include "../tekVisa/genmodule.h"
#include "../module/paamodule.h"
#include "types.h"

class TekOutput : public QPlainTextEdit, public Observer	{
	Q_OBJECT

public:
	using	mutex		=	std::mutex;
	using	threadId	=	std::thread::id;
	using	ofstream	=	std::ofstream;
	using	string		=	std::string;

	explicit TekOutput(tekModule *sub, PaaModule *chip, QWidget *parent = nullptr);
	~TekOutput();

	void	printAction(const ActionInfo_s &act);
	void	printInfo(const ActionInfo_s &tdcAction,int16_t data);
	void	update(const Subject *subject);
	void	appendText(const string&	text);

protected:
	void	print(const QString& text);
	void	printDate();
	QString	getDate(const QDateTime &dt);
	QString	getTime(const QDateTime &tm);
	QString	getDataTime(const QDateTime &tmdt);

private:
	mutex		outputLock_;
	tekModule	*module_;
	PaaModule	*chip_;
	ofstream	logStream_;
	threadId	homeID_;
};

#endif // TEKOUTPUT_H
