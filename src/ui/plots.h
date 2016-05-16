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
	using	QvecdoubPair	=	std::pair<QVector<double>,	QVector<double>>;

	explicit Plots(PaaModule *chip, QWidget *parent = nullptr);
	~Plots();

	void	update(const Subject *subject);
	void	clearAll();

public slots:
	void	updateAll();

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
	QvecdoubPair	ampFirstData_;
	QvecdoubPair	ampSecondData_;
	QvecdoubPair	formData_;

	QHBoxLayout*	ampHLayout_;
	QVBoxLayout*	formVLayout_;
	QCheckBox*		isRescaleButton_;
};

#endif // PLOTS_H
