#ifndef PLOTS_H
#define PLOTS_H

#include <QWidget>
#include <QCheckBox>
#include <QVector>
#include <QBoxLayout>
#include <qcustomplot.h>

#include "../observer/observer.h"
#include "../module/paamodule.h"

class Plots : public QWidget, public Observer	{
	Q_OBJECT

public:
	using	qvecdoubPair	=	std::pair<QVector<double>,	QVector<double>>;

	explicit Plots(PaaModule *chip, QWidget *parent = nullptr);
	~Plots();

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
	PaaModule*		chip_;
	QCustomPlot*	ampFirst_;
	QCustomPlot*	ampSecond_;
	QCustomPlot*	form_;
	qvecdoubPair	ampFirstData_;
	qvecdoubPair	ampSecondData_;
	qvecdoubPair	formData_;

	QHBoxLayout*	ampHLayout_;
	QVBoxLayout*	formVLayout_;
	QCheckBox*		isRescaleButton_;
};

#endif // PLOTS_H
