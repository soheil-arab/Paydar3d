#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <netdb.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <thread>

#include <iostream>
using namespace std;

#include "connection.h"

connection::connection()
{
	socket_descriptor = 0;
	readed_bytes = -4;
}

void connection::connect_to(const char *ip, int port)
{
	sockaddr_in server;
	server.sin_family = PF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = inet_addr(ip);
	bzero(&(server.sin_zero), 8);
    if ( (socket_descriptor = socket(PF_INET, SOCK_STREAM, 0)) ==-1 )
    {
        cerr << "Socket reserv failed" << endl;
        exit(0);
//        exit(-1);
    }
    if ( connect(socket_descriptor, (struct sockaddr *) & server,
                 sizeof(sockaddr_in)) == -1)
    {
        cerr << "failed to connect" << endl;
        exit(0);
//        exit(-1);
    }
}

void connection::disconnect()
{
	close(socket_descriptor);
}

void connection::send( string message )
{
    if ( message.empty())
        return;
    message +="(syn)";

	unsigned int len = htonl( message.size() );
	string strPrefix( (const char*)&len, 4 );
	string str = strPrefix + message;
    if ( write( socket_descriptor, str.data(), str.size() ) < 1 )
    {
        cerr << "failed to write" << endl;
        exit(0);
//        exit(-1);
    }
}

void connection::receive( string &message )
{
	while(readed_bytes<0)
	{
//		select_input();
		int t=readed_bytes;
		readed_bytes += read(socket_descriptor, buffer+readed_bytes+4, 
			sizeof(buffer) - readed_bytes - 4);
        if ( t== readed_bytes )
        {
            cerr << "Readed Byte Of msg Length Zero" << endl;
            exit(0);
//            exit(-1);
        }
	}
	int msg_length = ntohl(*(int*)buffer);
	while (readed_bytes < msg_length)
	{
//		select_input();
		int t=readed_bytes;
		readed_bytes += read(socket_descriptor, buffer+readed_bytes+4, 
			sizeof(buffer) - readed_bytes - 4);
        if ( t==readed_bytes)
        {
            cerr << "Readed Byte Zero" << endl;
            exit(0);
//            exit(-1);
        }
	}
	buffer[readed_bytes+4] = 0;//Next message lenght must be lower than 2^24
	message = string(buffer+4);
	for(int i=0;i<readed_bytes-msg_length;i++)
		buffer[i]=buffer[i+msg_length+4];
	readed_bytes = readed_bytes - msg_length - 4;
}

void connection::select_input()
{
	fd_set readFDS;
	FD_ZERO( &readFDS );
	FD_SET( socket_descriptor, &readFDS );

    if ( select( socket_descriptor + 1, &readFDS, 0, 0, 0 ) <  0 )
    {
        cerr << "Can not select" << endl;
        exit(0);
//        exit(-1);
    }
}
