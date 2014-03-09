/*
 * File: ConnectionManager.cpp
 *
 * Description: Implementation file
 *
 * Author: UI-AI 3D Robocup Soccer Team <ui_3dsoccer@eng.ui.ac.ir>, (c) 2007
 *
 */

/*
    Copyright (C) 2009  UI-AI 3D Team

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "ConnectionManager.h"
#include <string>
#include <arpa/inet.h>


using namespace std;

ConnectionManager::ConnectionManager(): input(socket), output(socket)
{
    input.exceptions(ios::eofbit|ios::badbit|ios::failbit);
}

void ConnectionManager::Connect(rcss::net::Addr addr)
{
    socket.connect(addr);
}

void ConnectionManager::Reconnect()
{
    rcss::net::Addr peer = socket.getPeer();
    Disconnect();
    socket.open();
    Connect(peer);
}

void ConnectionManager::Disconnect()
{
    socket.close();
}

void ConnectionManager::Send(string msg)
{
    uint32_t len = htonl(msg.size());

    output.write(reinterpret_cast<char *>(&len), sizeof(len));
    output.write(msg.c_str(), msg.size());
    output.flush();
}


string ConnectionManager::Receive()
{
    uint32_t msgsize;
    input.read(reinterpret_cast<char *>(&msgsize), sizeof(msgsize));
    msgsize = ntohl(msgsize);

    char buffer[msgsize+1];
    input.read(buffer, msgsize);
    buffer[msgsize]='\0';

    return buffer;
}

ConnectionManager::~ConnectionManager()
{
    Disconnect();
}
