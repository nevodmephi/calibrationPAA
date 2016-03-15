#ifndef INTERLOCUTOR_H
#define INTERLOCUTOR_H

#include <string>
#include <queue>
#include <sstream>
#include <thread>
#include "include/visa/visa.h"
#include "include/visa/visatype.h"
#include "src/observer/observer.h"
#include "tektypes.h"

#include "chip/defines.h"

class genModule : public Subject	{
public:
	using	string			=	std::string;
	using	stringstream	=	std::stringstream;

	enum class interest
	{statusUpdate};

	genModule();
	bool openSession();
	bool closeSession();
	bool setPing(int ping);
	bool setWidth(int width);
	bool setLowLevel(int val);
	bool setHighLevel(int val);
	bool setFrequency(int frequency);
	bool setCountSignals(int countSignals);
	bool setLeftFront(int leftFront);
	bool setRightFront(int rightFront);
	bool setInterval(int interval);
	void setChannel(int channel);
	bool setDefaultSettings(int ch);
	bool activateChannel(bool statusChannel);

	bool getWidth(int width);
	bool getLeftFront(int leftFront);
	bool getRightFront(int leftFront);
	bool getInterval(int interval);
	bool getHighLevel(int val);
	bool getLowLevel(int val);
	int  getChannelActive()	const;

	string		whoYou();

	ViSession	resMN()		const;
	ViSession	session()	const;
	const genSettings_t &getSettgins() const;

	std::string decodeAction(genActions op) const;

protected:
	void	 pushAction(genActions action,ViStatus status);
	ViStatus write(const string &text) const;
	ViStatus read(const string &request, string &answer);
	void	setChannelSettings();
	void	loadSettigs();

private:
	bool			active;
	ViSession		_resMN;
	ViSession		_session;
	ViFindList		_mainFList;
	int				_channel;
	genSettings_t	settings[2];
};

#endif // INTERLOCUTOR_H
