#ifndef STATION_H
#define STATION_H

#include "host.h"

class Station : public std::enable_shared_from_this<Station>	{
public:
	using HostPtr = std::shared_ptr<Host>;

	Station();
	~Station();

	HostPtr	addHost(uint32_t	numberHost, const std::string&	ip);
	void	deleteHost(uint32_t	numberHost);

	std::shared_ptr<Station>	getPointer();
protected:

private:
	std::map<uint32_t, HostPtr>	hosts_;
};

#endif // STATION_H
