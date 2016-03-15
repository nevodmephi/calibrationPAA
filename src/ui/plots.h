#ifndef PLOTS_H
#define PLOTS_H

#include <QWidget>
#include <QCheckBox>
#include <QVector>
#include <QBoxLayout>

#include "../observer/observer.h"
#include "../tekVisa/chipmodule.h"
#include "qcustomplot.h"

class plotsOutput : public QWidget, public Observer	{
	Q_OBJECT

public:
	using	qvecdoubPair	=	std::pair<QVector<double>,	QVector<double>>;

	explicit plotsOutput(chipModule *chip, QWidget *parent = nullptr);
	~plotsOutput();

	void	update(const Subject *subject);
	void	allClear();

public slots:
	void	dataUpdate();

protected slots:
	void	renderFirst();
	void	renderSecond();
	void	renderForm();

protected:
	void	createFirst();
	void	createSecond();
	void	createForm();

private:
	chipModule*		_chip;
	QCustomPlot*	_ampFirst;
	QCustomPlot*	_ampSecond;
	QCustomPlot*	_form;
	qvecdoubPair	_ampFirstData;
	qvecdoubPair	_ampSecondData;
	qvecdoubPair	_formData;

	QHBoxLayout*	_ampHLayout;
	QVBoxLayout*	_formVLayout;
	QCheckBox*		_isRescaleButton;
};

#endif // PLOTS_H
