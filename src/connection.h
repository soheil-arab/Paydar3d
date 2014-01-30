#ifndef _CONNECTION_H
#define _CONNECTION_H

#include <string>

class connection
{
public:
	connection();
	void connect_to(const char *, int);
	void disconnect();
	void receive(std::string &);
	void send(const std::string &);
private:
	void select_input();

	char buffer[10000];
	int socket_descriptor;
	int readed_bytes;
};

#endif
