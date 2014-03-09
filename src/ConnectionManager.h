/*
 * File: ConnectionManager.h
 *
 * Description: Manages server connection
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

#ifndef CONNECTIONMANAGER_H_
#define CONNECTIONMANAGER_H_

#include <string>
#include "rcssnet/tcpsocket.hpp"
//#include "rcssnet/udpsocket.hpp"

#include "rcssnet/addr.hpp"
#include "rcssnet/isocketstream.hpp"
#include "rcssnet/osocketstream.hpp"

class ConnectionManager
{
    public:
        ConnectionManager();
        virtual ~ConnectionManager();

        void Connect(rcss::net::Addr addr);
        void Reconnect();
        void Disconnect();
        void Send(std::string msg);
        std::string Receive();

    private:
        rcss::net::TCPSocket socket;
        //rcss::net::UDPSocket socket;
        rcss::net::ISocketStream input;
        rcss::net::OSocketStream output;
};

#endif /*CONNECTIONMANAGER_H_*/
