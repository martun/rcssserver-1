#include "serverclientmediator.h"
#include <cstring>
#include <cstdio>

ServerClientMediator::ServerClientMediator()
{

}
void ServerClientMediator::sendToClients(const char* in_buffer, int size)
{
	to_client_queue.push({in_buffer, size});
	printf("sendToClients - %s\n", in_buffer);
}
void ServerClientMediator::sendToServer(const char* in_buffer, int size)
{
	to_server_queue.push({in_buffer, size});
	printf("sendToServer - %s\n", in_buffer);
}
int ServerClientMediator::recvFromClients(char* out_buffer, int size)
{
	size = 0;
	if(!to_server_queue.empty())
	{
		memcpy(out_buffer, to_server_queue.front().first, to_server_queue.front().second);
		size = to_server_queue.front().second;
		to_server_queue.pop();
	}
	printf("recvFromClients - %s\n", out_buffer);
	return size;
}
int ServerClientMediator::recvFromServer(char* out_buffer, int size)
{
	size = 0;
	if(!to_client_queue.empty())
	{
		memcpy(out_buffer, to_server_queue.front().first, to_server_queue.front().second);
		size = to_client_queue.front().second;
		to_client_queue.pop();
	}
	printf("recvFromServer - %s\n", out_buffer);
	return size;
}