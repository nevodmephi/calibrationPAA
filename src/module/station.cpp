#include "station.h"

Station::Station()	{

}

Station::~Station()	{

}

Station::HostPtr	Station::addHost(uint32_t numberHost, const std::string &ip)	{
	HostPtr	host = std::make_shared<Host>(ip);
//	hosts_.insert(hosts_.begin() + numberHost, host);
	hosts_.insert(std::pair<uint32_t, HostPtr>(numberHost, host));
	return	host;
}

void	Station::deleteHost(uint32_t numberHost)	{
	hosts_.erase(numberHost);
}

std::shared_ptr<Station>	Station::getPointer()	{
	return	shared_from_this();
}
