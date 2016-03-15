#include "tekmodule.h"

genModule::genModule()	{
	active = false;
	_channel = 0;
	setDefaultSettings(0);
	setDefaultSettings(1);
	loadSettigs();
}

void genModule::setChannel(int channel)	{
	_channel = channel;
//	setChannelSettings();
}

void genModule::setChannelSettings()	{
	setWidth(settings[_channel].width);
	setHighLevel(settings[_channel].highLevel);
	setLowLevel(settings[_channel].lowLevel);
	setInterval(settings[_channel].interval);
	setRightFront(settings[_channel].rightFront);
	setLeftFront(settings[_channel].leftFront);
	loadSettigs();
}

void genModule::loadSettigs()	{
	getWidth(settings[_channel].width);
	getHighLevel(settings[_channel].highLevel);
	getLowLevel(settings[_channel].lowLevel);
	getInterval(settings[_channel].interval);
	getRightFront(settings[_channel].rightFront);
	getLeftFront(settings[_channel].leftFront);
}

bool genModule::openSession()	{
	ViStatus status;
	ViUInt32 numInit;
	const ViString resName = (const ViString)"USB?*INSTR{VI_ATTR_MANF_ID==0x0699}";
	ViChar desc[VI_FIND_BUFLEN+1];
	status = viOpenDefaultRM(&_resMN);
	if (status != VI_SUCCESS)	{
//		pushAction(genActions::init,status);
		return false;
	}
	status = viFindRsrc(_resMN, resName , &_mainFList, &numInit, desc);
//	std::cout << "viFindRsrc errCode = " << status << std::endl;
	if (status != VI_SUCCESS)	{
//		pushAction(genActions::init,status);
		return false;
	}
	status = viOpen(_resMN, desc, VI_NULL, VI_NULL, &_session);
//	std::cout << "viOpen errCode = " << status << std::endl;
	if (status != VI_SUCCESS)	{
//		pushAction(genActions::init,status);
		return false;
	}
	active = true;
//	pushAction(genActions::init,VI_SUCCESS);
//	startMainBlock();
	return true;
}

bool genModule::closeSession()	{
	ViStatus status;
	status = viClose(_session);
//	pushAction(genActions::close,status);
	if (status != VI_SUCCESS)
		return false;
	status = viClose(_resMN);
	if (status != VI_SUCCESS)
		return false;
	active = false;
	return true;
}

genModule::string genModule::whoYou()	{
	ViStatus status;
	ViUInt32 retCount = 0;
	ViChar buffer[256];
	status = viWrite(_session, (unsigned char*)"*IDN?", 5, &retCount);
	if(status == VI_SUCCESS)	{
		status = viRead(_session, (unsigned char*)buffer, 255, &retCount);
		if(status == VI_SUCCESS)		{
			buffer[retCount] = '\0';
			return string(buffer);
		} else
			return string();
	} else
		return string();
}

ViSession	genModule::resMN() const	{
	return _resMN;
}

ViSession	genModule::session() const	{
	return _session;
}

const genSettings_t &genModule::getSettgins() const	{
	return settings[_channel];
}

bool genModule::setDefaultSettings(int ch)	{
	if(!active) return false;
	ViStatus status;
	stringstream data;
	data << "SOUR" << ch << ":BURS:STAT ON";
	status = write(data.str());
	if (status != VI_SUCCESS)
		return false;
	data.str().clear();
	data << "SOUR" << ch << ":FUNC PULS";
	status = write(data.str());
	if (status != VI_SUCCESS)
		return false;
	data.str().clear();
	data << "SOUR" << ch << ":BURS:MODE TRIG";
	status = write(data.str());
	if (status != VI_SUCCESS)
		return false;
	data.str().clear();
	data << "SOUR" << ch << ":FREQ 1000kHz";
	status = write(data.str());
	if (status != VI_SUCCESS)
		return false;
	data.str().clear();
	data << "SOUR" << ch << ":BURS:NCYC 1";
	status = write(data.str());
	if (status != VI_SUCCESS)
		return false;
	return true;
}

bool genModule::setPing(int ping)	{
	if(!active) return false;
	stringstream data;
	data << "SOUR" << _channel << ":BURS:TDEL " << ping << "ms";
	auto status = write(data.str());
//	pushAction(genActions::setPing , status);
	if (status == VI_SUCCESS)	{
		settings[_channel].ping = ping;
		return true;
	}
	else
		return false;
}

bool genModule::setLeftFront(int leftFront)	{
	if(!active) return false;
	stringstream data;
	data << "SOUR" << _channel << ":PULS:TRAN:LEAD " << leftFront << "ns";
	auto status = write(data.str());
//	pushAction(genActions::setLeftFront , status);
	if (status == VI_SUCCESS)	{
		settings[_channel].leftFront = leftFront;
		return true;
	}
	else
		return false;
}

bool genModule::setRightFront(int rightFront)	{
	if(!active) return false;
	stringstream data;
	data << "SOUR" << _channel << ":PULS:TRAN:TRA " << rightFront << "ns";
	auto status = write(data.str());
//	pushAction(genActions::setRightFront , status);
	if (status == VI_SUCCESS)	{
		settings[_channel].rightFront = rightFront;
		return true;
	}
	else
		return false;
}

bool genModule::setCountSignals(int countSignals)	{
	if(!active) return false;
	stringstream data;
	data << "SOUR" << _channel << ":BURS:NCYC " << countSignals;
	auto status = write(data.str());
//	pushAction(genActions::setCountSignals , status);
	if (status == VI_SUCCESS)	{
		settings[_channel].countSignals = countSignals;
		return true;
	}
	else
		return false;
}

bool genModule::setFrequency(int frequency)	{
	if(!active) return false;
	stringstream data;
	data << "SOUR" << _channel << ":FREQ " << frequency << "kHZ";
	auto status = write(data.str());
//	pushAction(genActions::setFrequency , status);
	if (status == VI_SUCCESS)	{
		settings[_channel].frequency = frequency;
		return true;
	}
	else
		return false;
}

bool genModule::setHighLevel(int val)	{
	if(!active) return false;
	stringstream data;
	data << "SOUR" << _channel << ":VOLT:HIGH " << val << "mV";
	auto status = write(data.str());
//	pushAction(genActions::setAmplitudeHigh , status);
	if(status == VI_SUCCESS)	{
		settings[_channel].highLevel = val;
		return true;
	} else
		return false;
}

bool genModule::setLowLevel(int val)	{
	if(!active) return false;
	stringstream data;
	data << "SOUR" << _channel << ":VOLT:LOW " << val << "mV";
	auto status = write(data.str());
//	pushAction(genActions::setAmplitudeLow , status);
	if(status == VI_SUCCESS)	{
		settings[_channel].lowLevel = val;
		return true;
	} else
		return false;
}

bool genModule::setInterval(int interval)	{
	if(!active) return false;
	stringstream data;
	data << "TRIG:SEQ:TIM " << interval << "ms";
	auto status = write(data.str());
//	pushAction(genActions::setInterval , status);
	if (status == VI_SUCCESS)	{
		settings[_channel].interval = interval;
		return true;
	} else
		return false;
}

bool genModule::activateChannel(bool statusChannel)	{
	if(!active) return false;
	stringstream data;
	data << "OUTP" << _channel << ":STAT ";
	if (statusChannel == true)
		data << "ON";
	if (statusChannel == false)
		data << "OFF";
	auto status = write(data.str());
//	pushAction(genActions::activateChannel , status);
	if (status == VI_SUCCESS)	{
		settings[_channel].activeChannel = statusChannel;
		return true;
	} else
		return false;
}

bool genModule::setWidth(int width)	{
	if(!active) return false;
	stringstream data;
	data << "SOUR" << _channel << ":PULS:WIDT " << width << "ns";
	auto status = write(data.str());
//	pushAction(genActions::setWidth , status);
	if (status == VI_SUCCESS)	{
		settings[_channel].width = width;
		return true;
	} else
		return false;
}

bool genModule::getWidth(int width)	{
	if(!active) return false;
	stringstream data;
	string widthStr;
	data << "SOUR" << _channel << ":PULS:WIDT?";
	auto status = read(data.str(),widthStr);
	if(status == VI_SUCCESS)	{
		width = stoi(widthStr);
		settings[_channel].width = width;
		return true;
	} else	{
//		pushAction(genActions::getWidth,status);
		return false;
	}
}

bool genModule::getHighLevel(int val)	{
	if(!active) return false;
	stringstream data;
	string ampStr;
	data << "SOUR" << _channel << ":VOLT:HIGH?";
	auto status = read(data.str(),ampStr);
	if(status == VI_SUCCESS)	{
		val = stoi(ampStr);
		settings[_channel].highLevel = val;
		return true;
	} else	{
//		pushAction(genActions::getAmplitudeHigh,status);
		return false;
	}
}

bool genModule::getLowLevel(int val)	{
	if(!active) return false;
	stringstream data;
	string ampStr;
	data << "SOUR" << _channel << ":VOLT:LOW?";
	auto status = read(data.str(),ampStr);
	if(status == VI_SUCCESS)	{
		val = stoi(ampStr);
		settings[_channel].lowLevel = val;
		return true;
	} else	{
//		pushAction(genActions::getAmplitudeHigh,status);
		return false;
	}
}

bool genModule::getLeftFront(int leftFront)	{
	if(!active) return false;
	stringstream data;
	string lFrontStr;
	data << "SOUR" << _channel << ":PULS:TRAN:LEAD?";
	auto status = read(data.str(),lFrontStr);
	if(status == VI_SUCCESS)	{
		leftFront = stoi(lFrontStr);
		settings[_channel].leftFront = leftFront;
		return leftFront;
	} else	{
//		pushAction(genActions::getLeftFront,status);
		return false;
	}
}

bool genModule::getRightFront(int rightFront)	{
	if(!active) return false;
	stringstream data;
	string rFrontStr;
	data << "SOUR" << _channel << ":PULS:TRAN:TRA?";
	auto status = read(data.str(),rFrontStr);
	if(status == VI_SUCCESS)	{
		rightFront = stoi(rFrontStr);
		settings[_channel].rightFront = rightFront;
		return rightFront;
	} else	{
//		pushAction(genActions::getRightFront,status);
		return false;
	}
}

bool genModule::getInterval(int interval)	{
	if(!active) return false;
	stringstream data;
	string intervalStr;
	data << "TRIG:SEQ:TIM?";
	auto status = read(data.str(),intervalStr);
	if(status == VI_SUCCESS)	{
		interval = stoi(intervalStr);
		settings[_channel].rightFront = interval;
		return interval;
	} else	{
//		pushAction(genActions::getInterval,status);
		return false;
	}
}

ViStatus genModule::write(const string &text) const	{
	ViUInt32 retCount = 0;
	auto status = viWrite(_session, (ViBuf)text.c_str(),text.size(),&retCount);
	if(retCount != text.size())
		return VI_ERROR_BERR;
	return status;
}

ViStatus genModule::read(const string &request,string &answer)	{
	if(!active) return VI_ERROR_BERR;
	auto status = write(request);
	if(status == VI_SUCCESS)	{
		ViUInt32 retCount = 0;
		ViChar buff[256];
		status = viRead(_session, (ViPBuf)buff, 256, &retCount);
		if(status == VI_SUCCESS)	{
//			cout << "readOperation: " << buff << endl;
			buff[retCount - 4] = '\0';
			answer = string(buff);
		}
		return status;
	} else
		return status;
}

std::string genModule::decodeAction(genActions op) const	{
	if(op == genActions::init)
		return std::string("Init");
	if(op == genActions::activateChannel)
		return std::string("Activate Channel");
	if(op == genActions::setAmplitudeHigh)
		return std::string("Set Amplitude[High]");
	if(op == genActions::setAmplitudeLow)
		return std::string("Set Amplitude[Low]");
	if(op == genActions::setChannel)
		return std::string("Set Channel");
	if(op == genActions::setCountSignals)
		return std::string("Set Count Signals");
	if(op == genActions::setDefaultSettings)
		return std::string("Set Default Settings");
	if(op == genActions::setFrequency)
		return std::string("Set Frequency");
	if(op == genActions::setInterval)
		return std::string("Set Interval");
	if(op == genActions::setLeftFront)
		return std::string("Set Left Front");
	if(op == genActions::setPing)
		return std::string("Set Ping");
	if(op == genActions::setRightFront)
		return std::string("Set Right Front");
	if(op == genActions::setWidth)
		return std::string("Set Width");
	return std::string("");
}

int genModule::getChannelActive()	const	{
	return _channel;
}
