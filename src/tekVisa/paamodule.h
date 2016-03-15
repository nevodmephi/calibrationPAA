#ifndef PAAMODULE_H
#define PAAMODULE_H

#include <vector>
#include <algorithm>
#include <map>
#include <queue>
#include <fstream>
#include <thread>
#include <mutex>
#include <ctime>

#include "../observer/observer.h"
#include "genmodule.h"
#include "processing.h"

class PaaModule	{
public:
	enum class mode
	{amp, form};

	PaaModule(GenModule* tekMod, Processing* calculation);
	~PaaModule();

	bool	openSession();

protected:

private:

};

#endif // PAAMODULE_H
