#ifndef TEKOUTPUT_H
#define TEKOUTPUT_H

#include <QPlainTextEdit>
#include <QDateTime>
#include <QString>
#include <fstream>
#include <mutex>
#include <thread>

#include "../observer/observer.h"
#include "../tekVisa/tekmodule.h"
#include "../tekVisa/chipmodule.h"
#include "types.hpp"

class tekOutput : public QPlainTextEdit, public Observer	{
	Q_OBJECT

public:
	using	mutex		=	std::mutex;
	using	threadId	=	std::thread::id;
	using	ofstream	=	std::ofstream;
	using	string		=	std::string;

	explicit tekOutput(tekModule *sub, chipModule *chip, QWidget *parent = nullptr);
	~tekOutput();

	void	printAction(const actionInfo_s &act);
	void	printInfo(const actionInfo_s &tdcAction,int16_t data);
	void	update(const Subject *subject);
	void	appendText(const string&	text);

protected:
	void	print(const QString& text);
	void	printDate();
	QString	getDate(const QDateTime &dt);
	QString	getTime(const QDateTime &tm);
	QString	getDataTime(const QDateTime &tmdt);

private:
	mutex		outputLock;
	tekModule	*_module;
	chipModule	*_chip;
	ofstream	logStream;
	threadId	homeID;
};

#endif // TEKOUTPUT_H
