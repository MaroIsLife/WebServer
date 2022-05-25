#pragma once

#include <poll.h>
#include <vector>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fstream>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>



class Mysocket
{
	public:

		Mysocket(int domain, int type, int protocol, int port, int max_connections);
		~Mysocket();

		void setup_socket(int domain, int type, int protocol);
		void bind_socket(int port);
		void listen_socket();
		void accept_connection();

		
	private:
	
	std::vector<struct pollfd> pollfds;
	int					socketfd;
	struct sockaddr_in	server_addr;
	int					new_socketfd;
	int					max_connections;
};
