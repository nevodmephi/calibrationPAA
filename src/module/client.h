#ifndef CLIENT_H
#define CLIENT_H

#include <QtNetwork/QtNetwork>
#include <thread>
#include <memory>
#include <cstdint>

#include "../observer/observer.h"


class Client : public QObject, public Subject	{
	Q_OBJECT
public:

	enum class Message	{
		connected,
		disconnected,
		readyRead,
		error,
		null
	};

	using QTcpSocketPtr		= std::shared_ptr<QTcpSocket>;
	using QHostAddressPtr	= std::shared_ptr<QHostAddress>;

	Client(const std::string&	address, uint16_t	port);
	~Client();

	void		connectToHost();
	void		disconnectFromHost();
	void		readRegister(uint32_t	address);
	void		clearData();
	void		setSleepTime(uint32_t	sleepTime);
	void		writeRegister3000(uint32_t address, uint32_t data);
	void		writeRegister3002(uint32_t address, uint32_t data);

	const std::vector<uchar>&	getData()		const;
	Message							getMessage()	const;

protected slots:
	void		onSocketConnected();
	void		onSocketDisconnected();
	void		onSocketReadyRead();
	void		onSocketError(QAbstractSocket::SocketError	error);

private:
	QTcpSocketPtr		socket_;
	QHostAddressPtr		address_;
	uint16_t			port_;
	std::vector<uchar>	data_;
	Message				message_;
	uint32_t			sleepTime_;
};

#endif // CLIENT_H
