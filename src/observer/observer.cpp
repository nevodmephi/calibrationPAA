#include "observer.h"

Observer::Observer(){}

Subject::Subject(){}

Observer::~Observer(){}

Subject::~Subject(){}

void Subject::attach(Observer*	obs)	{
	observers_.push_back(obs);
}

void Subject::detach(Observer *obs)	{
	observers_.remove(obs);
}

void Subject::notify()	{
	for(auto &obs : observers_)
		obs->update(this);
}

void Subject::notify(Observer*	obs)	{
	obs->update(this);
}
