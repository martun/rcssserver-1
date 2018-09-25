#ifndef SERVERCLIENTMEDIATOR_H
#define SERVERCLIENTMEDIATOR_H
#include <queue>
#include <utility>

class ServerClientMediator
{
private:
	std::queue<std::pair<const char*, int>> to_server_queue;
	std::queue<std::pair<const char*, int>> to_client_queue;
public:
	ServerClientMediator();
	void sendToClients(const char* in_buffer, int size);
	void sendToServer(const char* in_buffer, int size);
	int recvFromClients(char* out_buffer, int size);
	int recvFromServer(char* out_buffer, int size);
};

#endif
