#include "client.h"

Client::Client(const std::string& address, uint16_t port)
	:	address_(std::make_unique<QHostAddress>(QString::fromStdString(address))), port_(port) {

	socket_		= std::make_unique<QTcpSocket>(this);
	message_	= Message::error;
	sleepTime_	= 0;

	connect(socket_.get(),	SIGNAL(connected()),	this,	SLOT(onSocketConnected()));
	connect(socket_.get(),	SIGNAL(disconnected()), this,	SLOT(onSocketDisconnected()));
	connect(socket_.get(),	SIGNAL(readyRead()),	this,	SLOT(onSocketReadyRead()));
	connect(socket_.get(),	SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onSocketError(QAbstractSocket::SocketError)));
}

Client::~Client()	{

}

void	Client::onSocketConnected()	{
	message_ = Message::connected;
	this->notify();
}

void	Client::onSocketDisconnected()	{
	message_ = Message::disconnected;
	this->notify();
}

void	Client::onSocketReadyRead()	{
	message_ = Message::readyRead;
	QByteArray	array = socket_->readAll();
	data_.assign(array.begin(), array.end());
//	std::cout << data_.size() << std::endl;
	this->notify();
//	std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime_));
//	std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

void	Client::onSocketError(QAbstractSocket::SocketError )	{
	message_ = Message::error;
	this->notify();
}

void	Client::connectToHost()	{
	std::cout << (*address_.get()).toString().toStdString() << "\t" << port_ << std::endl;
	socket_->connectToHost(*address_.get(), port_);
}

void	Client::disconnectFromHost()	{
	socket_->disconnectFromHost();
}

void	Client::readRegister(uint32_t address)	{
	QByteArray	sendMessage;
	sendMessage.resize(12);
	sendMessage[0]	= 0x0b;
	sendMessage[1]	= 0xb9;

	sendMessage[2]	= 0x0b;
	sendMessage[3]	= 0xb9;

	sendMessage[4]	= 0;
	sendMessage[5]	= 12;

	sendMessage[6]	= 0;
	sendMessage[7]	= 0;

	sendMessage[8]	= address & 0xff;
	sendMessage[9]	= (address >> 8) & 0xff;

	sendMessage[10]	= (address >> 16) & 0xff;
	sendMessage[11]	= (address >> 24) & 0xff;

	socket_->write(sendMessage);
}

void	Client::writeRegister3000(uint32_t address, uint32_t data)	{
	QByteArray	sendMessage;
	sendMessage.resize(14);
	sendMessage[0]	= 0x0b;
	sendMessage[1]	= 0xb8;

	sendMessage[2]	= 0x0b;
	sendMessage[3]	= 0xb8;

	sendMessage[4]	= 0;
	sendMessage[5]	= 14;

	sendMessage[6]	= 0;
	sendMessage[7]	= 0;

	sendMessage[8]	= address & 0xff;
	sendMessage[9]	= (address >> 8) & 0xff;

	sendMessage[10]	= (address >> 16) & 0xff;
	sendMessage[11]	= (address >> 24) & 0xff;

	sendMessage[12]	= data & 0xff;
	sendMessage[13]	= (data >> 8) & 0xff;

	socket_->write(sendMessage);
}

void	Client::writeRegister3002(uint32_t address, uint32_t data)	{
	QByteArray	sendMessage;
	sendMessage.resize(14);
	sendMessage[0]	= 0x0b;
	sendMessage[1]	= 0xba;

	sendMessage[2]	= 0x0b;
	sendMessage[3]	= 0xba;

	sendMessage[4]	= 0;
	sendMessage[5]	= 14;

	sendMessage[6]	= 0;
	sendMessage[7]	= 0;

	sendMessage[8]	= address & 0xff;
	sendMessage[9]	= (address >> 8) & 0xff;

	sendMessage[10]	= (address >> 16) & 0xff;
	sendMessage[11]	= (address >> 24) & 0xff;

	sendMessage[12]	= data & 0xff;
	sendMessage[13]	= (data >> 8) & 0xff;

	socket_->write(sendMessage);
}

void	Client::clearData()	{
	message_ = Message::null;
	data_.clear();
}

void	Client::setSleepTime(uint32_t	sleepTime)	{
	sleepTime_ = sleepTime;
}

Client::Message	Client::getMessage()	const	{
	return message_;
}

const std::vector<unsigned char>&	Client::getData() const	{
	return data_;
}
