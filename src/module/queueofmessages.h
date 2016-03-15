#ifndef STACKOFMESSAGES_H
#define STACKOFMESSAGES_H

#include <queue>
#include <memory>
#include <string>
#include <list>

#include "../observer/observer.h"
#include "client.h"
#include "registers.h"

class QueueOfMessages : public Observer, public Subject, public std::enable_shared_from_this<QueueOfMessages>	{
public:
	using	ClientPtr = std::shared_ptr<Client>;
	using	ObserverPtr = std::shared_ptr<Observer>;

	enum class Message	{
		connected,
		disconnected,
		recordRead,
		recordWrite,
		dataRead,
		error,
		null
	};

    QueueOfMessages(const std::string& ip);
	~QueueOfMessages();

	void	update(const Subject*	subject);

	void	connectToHost();
	void	disconnectFromHost();
	void	addCommandToStack(const Record&	record, ObserverPtr	sender);
	void	runStack();
	void	clearData();

	const Record&					getRecord()		const;
	Message							getMessage()	const;
	const std::vector<unsigned char>&	getData()		const;

protected:
	void	writeRegister(const Record&	record);
	int		fillValuesInCommandsHaveBeenDone(const std::vector<unsigned char>&	data, int commandNumber);

private:
	ClientPtr									clientReg_;
	ClientPtr									clientData_;
	std::queue<std::pair<Record, ObserverPtr>>	commandsWillBeDone_;
	std::vector<std::pair<Record, ObserverPtr>>	commandsHaveBeenDone_;
	Record										answerRecord_;
	Message										message_;
	std::vector<unsigned char>					data_;
//	string			ip_;
};

#endif // STACKOFMESSAGES_H
