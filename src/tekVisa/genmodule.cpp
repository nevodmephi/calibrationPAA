#include "genmodule.h"

GenModule::GenModule()	{
	active = false;
	channel_ = 0;
	setDefaultSettings(0);
	setDefaultSettings(1);
	loadSettigs();
}

void GenModule::setChannel(int channel)	{
	channel_ = channel;
//	setChannelSettings();
}

void GenModule::setChannelSettings()	{
	setWidth(settings[channel_].width);
	setHighLevel(settings[channel_].highLevel);
	setLowLevel(settings[channel_].lowLevel);
	setInterval(settings[channel_].interval);
	setRightFront(settings[channel_].rightFront);
	setLeftFront(settings[channel_].leftFront);
	loadSettigs();
}

void GenModule::loadSettigs()	{
	getWidth(settings[channel_].width);
	getHighLevel(settings[channel_].highLevel);
	getLowLevel(settings[channel_].lowLevel);
	getInterval(settings[channel_].interval);
	getRightFront(settings[channel_].rightFront);
	getLeftFront(settings[channel_].leftFront);
}

bool GenModule::openSession()	{
	ViStatus status;
	ViUInt32 numInit;
	const ViString resName = (const ViString)"USB?*INSTR{VI_ATTR_MANF_ID==0x0699}";
	ViChar desc[VI_FIND_BUFLEN+1];
	status = viOpenDefaultRM(&resMN_);
	if (status != VI_SUCCESS)	{
//		pushAction(genActions::init,status);
		return false;
	}
	status = viFindRsrc(resMN_, resName , &mainFList_, &numInit, desc);
//	std::cout << "viFindRsrc errCode = " << status << std::endl;
	if (status != VI_SUCCESS)	{
//		pushAction(genActions::init,status);
		return false;
	}
	status = viOpen(resMN_, desc, VI_NULL, VI_NULL, &session_);
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

bool GenModule::closeSession()	{
	ViStatus status;
	status = viClose(session_);
//	pushAction(genActions::close,status);
	if (status != VI_SUCCESS)
		return false;
	status = viClose(resMN_);
	if (status != VI_SUCCESS)
		return false;
	active = false;
	return true;
}

GenModule::string GenModule::whoAreYou()	{
	ViStatus status;
	ViUInt32 retCount = 0;
	ViChar buffer[256];
	status = viWrite(session_, (unsigned char*)"*IDN?", 5, &retCount);
	if(status == VI_SUCCESS)	{
		status = viRead(session_, (unsigned char*)buffer, 255, &retCount);
		if(status == VI_SUCCESS)		{
			buffer[retCount] = '\0';
			return string(buffer);
		} else
			return string();
	} else
		return string();
}

ViSession	GenModule::resMN() const	{
	return resMN_;
}

ViSession	GenModule::session() const	{
	return session_;
}

const GenSettings_t &GenModule::getSettgins() const	{
	return settings[channel_];
}

bool GenModule::setDefaultSettings(int ch)	{
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

bool GenModule::setPing(int ping)	{
	if(!active) return false;
	stringstream data;
	data << "SOUR" << channel_ << ":BURS:TDEL " << ping << "ms";
	auto status = write(data.str());
//	pushAction(genActions::setPing , status);
	if (status == VI_SUCCESS)	{
		settings[channel_].ping = ping;
		return true;
	}
	else
		return false;
}

bool GenModule::setLeftFront(int leftFront)	{
	if(!active) return false;
	stringstream data;
	data << "SOUR" << channel_ << ":PULS:TRAN:LEAD " << leftFront << "ns";
	auto status = write(data.str());
//	pushAction(genActions::setLeftFront , status);
	if (status == VI_SUCCESS)	{
		settings[channel_].leftFront = leftFront;
		return true;
	}
	else
		return false;
}

bool GenModule::setRightFront(int rightFront)	{
	if(!active) return false;
	stringstream data;
	data << "SOUR" << channel_ << ":PULS:TRAN:TRA " << rightFront << "ns";
	auto status = write(data.str());
//	pushAction(genActions::setRightFront , status);
	if (status == VI_SUCCESS)	{
		settings[channel_].rightFront = rightFront;
		return true;
	}
	else
		return false;
}

bool GenModule::setCountSignals(int countSignals)	{
	if(!active) return false;
	stringstream data;
	data << "SOUR" << channel_ << ":BURS:NCYC " << countSignals;
	auto status = write(data.str());
//	pushAction(genActions::setCountSignals , status);
	if (status == VI_SUCCESS)	{
		settings[channel_].countSignals = countSignals;
		return true;
	}
	else
		return false;
}

bool GenModule::setFrequency(int frequency)	{
	if(!active) return false;
	stringstream data;
	data << "SOUR" << channel_ << ":FREQ " << frequency << "kHZ";
	auto status = write(data.str());
//	pushAction(genActions::setFrequency , status);
	if (status == VI_SUCCESS)	{
		settings[channel_].frequency = frequency;
		return true;
	}
	else
		return false;
}

bool GenModule::setHighLevel(int val)	{
	if(!active) return false;
	stringstream data;
	data << "SOUR" << channel_ << ":VOLT:HIGH " << val << "mV";
	auto status = write(data.str());
//	pushAction(genActions::setAmplitudeHigh , status);
	if(status == VI_SUCCESS)	{
		settings[channel_].highLevel = val;
		return true;
	} else
		return false;
}

bool GenModule::setLowLevel(int val)	{
	if(!active) return false;
	stringstream data;
	data << "SOUR" << channel_ << ":VOLT:LOW " << val << "mV";
	auto status = write(data.str());
//	pushAction(genActions::setAmplitudeLow , status);
	if(status == VI_SUCCESS)	{
		settings[channel_].lowLevel = val;
		return true;
	} else
		return false;
}

bool GenModule::setInterval(int interval)	{
	if(!active) return false;
	stringstream data;
	data << "TRIG:SEQ:TIM " << interval << "ms";
	auto status = write(data.str());
//	pushAction(genActions::setInterval , status);
	if (status == VI_SUCCESS)	{
		settings[channel_].interval = interval;
		return true;
	} else
		return false;
}

bool GenModule::activateChannel(bool statusChannel)	{
	if(!active) return false;
	stringstream data;
	data << "OUTP" << channel_ << ":STAT ";
	if (statusChannel == true)
		data << "ON";
	if (statusChannel == false)
		data << "OFF";
	auto status = write(data.str());
//	pushAction(genActions::activateChannel , status);
	if (status == VI_SUCCESS)	{
		settings[channel_].activeChannel = statusChannel;
		return true;
	} else
		return false;
}

bool GenModule::setWidth(int width)	{
	if(!active) return false;
	stringstream data;
	data << "SOUR" << channel_ << ":PULS:WIDT " << width << "ns";
	auto status = write(data.str());
//	pushAction(genActions::setWidth , status);
	if (status == VI_SUCCESS)	{
		settings[channel_].width = width;
		return true;
	} else
		return false;
}

bool GenModule::getWidth(int width)	{
	if(!active) return false;
	stringstream data;
	string widthStr;
	data << "SOUR" << channel_ << ":PULS:WIDT?";
	auto status = read(data.str(),widthStr);
	if(status == VI_SUCCESS)	{
		width = stoi(widthStr);
		settings[channel_].width = width;
		return true;
	} else	{
//		pushAction(genActions::getWidth,status);
		return false;
	}
}

bool GenModule::getHighLevel(int val)	{
	if(!active) return false;
	stringstream data;
	string ampStr;
	data << "SOUR" << channel_ << ":VOLT:HIGH?";
	auto status = read(data.str(),ampStr);
	if(status == VI_SUCCESS)	{
		val = stoi(ampStr);
		settings[channel_].highLevel = val;
		return true;
	} else	{
//		pushAction(genActions::getAmplitudeHigh,status);
		return false;
	}
}

bool GenModule::getLowLevel(int val)	{
	if(!active) return false;
	stringstream data;
	string ampStr;
	data << "SOUR" << channel_ << ":VOLT:LOW?";
	auto status = read(data.str(),ampStr);
	if(status == VI_SUCCESS)	{
		val = stoi(ampStr);
		settings[channel_].lowLevel = val;
		return true;
	} else	{
//		pushAction(genActions::getAmplitudeHigh,status);
		return false;
	}
}

bool GenModule::getLeftFront(int leftFront)	{
	if(!active) return false;
	stringstream data;
	string lFrontStr;
	data << "SOUR" << channel_ << ":PULS:TRAN:LEAD?";
	auto status = read(data.str(),lFrontStr);
	if(status == VI_SUCCESS)	{
		leftFront = stoi(lFrontStr);
		settings[channel_].leftFront = leftFront;
		return leftFront;
	} else	{
//		pushAction(genActions::getLeftFront,status);
		return false;
	}
}

bool GenModule::getRightFront(int rightFront)	{
	if(!active) return false;
	stringstream data;
	string rFrontStr;
	data << "SOUR" << channel_ << ":PULS:TRAN:TRA?";
	auto status = read(data.str(),rFrontStr);
	if(status == VI_SUCCESS)	{
		rightFront = stoi(rFrontStr);
		settings[channel_].rightFront = rightFront;
		return rightFront;
	} else	{
//		pushAction(genActions::getRightFront,status);
		return false;
	}
}

bool GenModule::getInterval(int interval)	{
	if(!active) return false;
	stringstream data;
	string intervalStr;
	data << "TRIG:SEQ:TIM?";
	auto status = read(data.str(),intervalStr);
	if(status == VI_SUCCESS)	{
		interval = stoi(intervalStr);
		settings[channel_].rightFront = interval;
		return interval;
	} else	{
//		pushAction(genActions::getInterval,status);
		return false;
	}
}

ViStatus GenModule::write(const string &text) const	{
	ViUInt32 retCount = 0;
	auto status = viWrite(session_, (ViBuf)text.c_str(),text.size(),&retCount);
	if(retCount != text.size())
		return VI_ERROR_BERR;
	return status;
}

ViStatus GenModule::read(const string &request,string &answer)	{
	if(!active) return VI_ERROR_BERR;
	auto status = write(request);
	if(status == VI_SUCCESS)	{
		ViUInt32 retCount = 0;
		ViChar buff[256];
		status = viRead(session_, (ViPBuf)buff, 256, &retCount);
		if(status == VI_SUCCESS)	{
//			cout << "readOperation: " << buff << endl;
			buff[retCount - 4] = '\0';
			answer = string(buff);
		}
		return status;
	} else
		return status;
}

std::string GenModule::decodeAction(GenActions op) const	{
	if(op == GenActions::init)
		return std::string("Init");
	if(op == GenActions::activateChannel)
		return std::string("Activate Channel");
	if(op == GenActions::setAmplitudeHigh)
		return std::string("Set Amplitude[High]");
	if(op == GenActions::setAmplitudeLow)
		return std::string("Set Amplitude[Low]");
	if(op == GenActions::setChannel)
		return std::string("Set Channel");
	if(op == GenActions::setCountSignals)
		return std::string("Set Count Signals");
	if(op == GenActions::setDefaultSettings)
		return std::string("Set Default Settings");
	if(op == GenActions::setFrequency)
		return std::string("Set Frequency");
	if(op == GenActions::setInterval)
		return std::string("Set Interval");
	if(op == GenActions::setLeftFront)
		return std::string("Set Left Front");
	if(op == GenActions::setPing)
		return std::string("Set Ping");
	if(op == GenActions::setRightFront)
		return std::string("Set Right Front");
	if(op == GenActions::setWidth)
		return std::string("Set Width");
	return std::string("");
}

int GenModule::getChannelActive()	const	{
	return channel_;
}
