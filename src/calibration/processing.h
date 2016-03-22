#ifndef PROCESSING_H
#define PROCESSING_H

#include <condition_variable>
#include <mutex>
#include <vector>
#include <string>
#include <iomanip>
#include <fstream>

#include "../module/queueofmessages.h"
#include "../observer/observer.h"

class Processing : public Observer, public std::enable_shared_from_this<Processing>	{
public:
	using QueuePtr	= std::shared_ptr<QueueOfMessages>;

	Processing(QueuePtr	clientQueue);
	~Processing();

	void	update(const Subject* subject);

	void	setPathToSave(const std::string&	path);
	void	createFolders(const std::string&	path);

protected:
	void	controlQueue();
	void	parse(const std::vector<unsigned char>&	data);
	void	saveDataHowOldProgram(const std::vector<std::pair<int, std::vector<std::vector<int16_t>>>> &parserData,
								  const std::string& path, int numberFile);

private:
	QueuePtr														clientQueue_;
	std::queue<std::vector<unsigned char>>							buffer_;
	std::vector<std::pair<int, std::vector<std::vector<int16_t>>>>	data_;
//	std::vector<std::vector<std::vector<int16_t>>>					data_;
	std::string														pathToSave_;

	std::condition_variable											queueCheck_;
	std::mutex														lockQueueCheck_;
};

#endif // PROCESSING_H
