#ifndef TEKTYPES_H
#define TEKTYPES_H

#include <string>
#include <vector>
#include <array>

#include "include/visa/visatype.h"

class GenModule;

struct GenSettings_t	{
	int leftFront;
	int rightFront;
	int interval;
	int width;
	int highLevel;
	int lowLevel;
	int ping;
	int activeChannel;
	int countSignals;
	int frequency;
};

struct CalibrSettings_t	{
	int ampStart;
	int ampEnd;
	int ampStep;
};

enum class GenActions	{
	init,
	close,
	setWidth,
	setPing,
	setLeftFront,
	setCountSignals,
	setFrequency,
	setAmplitudeLow,
	setAmplitudeHigh,
	setRightFront,
	setInterval,
	setChannel,
	setDefaultSettings,
	activateChannel,
	getWidth,
	getAmplitudeHigh,
	getAmplitudeLow,
	getLeftFront,
	getRightFront,
	getInterval
};

struct ActionInfo_s	{
	ActionInfo_s(){}
	ActionInfo_s(GenActions act,ViStatus err)
		: actCode(act),errCode(err){}
	GenActions	actCode;
	ViStatus	errCode;
};

struct DataChip	{
	using	array4	=	std::array<double, 4>;
	using	array2	=	std::array<double, 2>;

	array4	transformationADC;
	array4	errorADC;
	array4	constADC;
	array4	errorConstADC;
	array2	transformationForm;
	array2	errorForm;
	array2	constForm;
	array2	errorConstForm;
};

#endif // TEKTYPES_H
