#ifndef TEKSETTINGS_H
#define TEKSETTINGS_H

#include <QWidget>
#include <QGroupBox>
#include <QFormLayout>
#include <QRadioButton>
#include <QCheckBox>
#include <QPushButton>
#include <QLineEdit>

#include "../observer/observer.h"
#include "../tekVisa/genmodule.h"
#include "../tekVisa/chipmodule.h"
#include "types.hpp"

class TekSettings : public QGroupBox, public Observer	{
	Q_OBJECT

public:
	using	charVec	=	std::vector<char>;

	explicit TekSettings(tekModule *mod,const QString &title, QWidget *parent = nullptr);
	~TekSettings();
	int		currentChannel () {return channel_;}
	void	update(const Subject *subject);

	void	setHighLevel();
	void	setLowLevel();
	charVec getChipChannel()	const;

protected:
	void	setSettings();
	void	writeSettings();
	void	readSettings();
	int		getCurrentChannel();
	void	createWidgets();
	void	createLayouts();

protected:
	void	setLeftFront();
	void	setRightFront();
	void	setInterval();
	void	setWidth();
	void	setChannel();
	void	updateSettings();

private:
	tekModule	*module_;
	ChipModule	*chip_;
	int			channel_;

	QFormLayout	*layout_;
	QHBoxLayout	*mainLayout_;
	QVBoxLayout	*chipLayout_;

	QRadioButton	*channelOneC_;
	QRadioButton	*channelTwoC_;
	std::vector<QCheckBox*>	chipChannel_;

	QLineEdit	*leftFrontL_;
	QLineEdit	*rightFrontL_;
	QLineEdit	*intervalL_;
	QLineEdit	*widthL_;
	QLineEdit	*lowLevelL_;
	QLineEdit	*highLevelL_;
	QLineEdit	*ampStartL_;
	QLineEdit	*ampEndL_;
	QLineEdit	*ampStepL_;
};

#endif // TEKSETTINGS_H
