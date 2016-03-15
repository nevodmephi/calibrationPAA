#ifndef OBSERVER_H
#define OBSERVER_H

#include <list>

class Subject;
class Observer;

class Observer	{
public:
	virtual ~Observer();
	virtual void update(const Subject*	subject) = 0;
protected:
	Observer();
};

class Subject	{
public:
	virtual ~Subject();
	virtual void attach(Observer*	obs);
	virtual void detach(Observer *obs);
	virtual void notify();
	virtual void notify(Observer*	obs);
protected:
	Subject();
private:
	std::list<Observer*>	observers_;
};

#endif // OBSERVER_H
