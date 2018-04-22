
// Thanks Remy Lebeau and rustyx from StackOverflow

#include <winsock2.h>
#include <ws2tcpip.h>

#include <iostream>
#include <string>

#include "lib.h"

#pragma comment(lib, "ws2_32.lib")

int main()
{
	std::cout << "Simple_Client IPv4 & IPv6\n\n" << std::endl;

	// Initiates Winsock
	WSADATA WSAData;
	int ret = WSAStartup(MAKEWORD(2, 0), &WSAData);
	if (ret != 0)
	{
		std::cerr << "WSAStartup error: " << ret << "\n" << std::endl;
		return -1;
	}

	// Get Parameters IP/PORT
	std::string str_HOSTNAME = "cloudserver070177.home.pl";
	int PORT = 21;

	// RESOLVE IP    
	addrinfo hints = { 0 };
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	addrinfo *pResult = NULL;
	ret = getaddrinfo(str_HOSTNAME.c_str(), std::to_string(PORT).c_str(), &hints, &pResult);
	if (ret != 0)
	{
		std::cerr << "getaddrinfo error: " << ret << "\n" << std::endl;
		WSACleanup();
		return -1;
	}

	// Log the IPs
	bool has_IPv4 = false;
	bool has_IPv6 = false;

	for (addrinfo *addr = pResult; addr != NULL; addr = addr->ai_next)
	{
		switch (addr->ai_family)
		{
		case AF_INET: // IPv4
		{
			has_IPv4 = true;
			std::cout << "IPv4 : " << addr_to_str(addr) << "\n" << std::endl;
			break;
		}

		case AF_INET6: // IPv6
		{
			has_IPv6 = true;
			std::cout << "IPv6 : " << addr_to_str(addr) << "\n" << std::endl;
			break;
		}
		}
	}

	// Connect to the HOSTNAME
	SOCKET sock = INVALID_SOCKET;

	if (has_IPv6)
	{
		// try IPv6 first...
		for (addrinfo *addr = pResult; addr != NULL; addr = addr->ai_next)
		{
			if (addr->ai_family != AF_INET6)
				continue;

			std::cout << "Connecting to IPv6 : " << addr_to_str(addr) << " | Port : " << PORT << "\n" << std::endl;

			sock = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
			if (sock == INVALID_SOCKET)
			{
				ret = WSAGetLastError();
				std::cerr << "socket error: " << ret << "\n" << std::endl;
				continue;
			}

			if (connect(sock, addr->ai_addr, addr->ai_addrlen) == SOCKET_ERROR)
			{
				ret = WSAGetLastError();
				std::cerr << "connect error: " << ret << "\n" << std::endl;
				closesocket(sock);
				sock = INVALID_SOCKET;
				continue;
			}

			break;
		}
	}

	if ((sock == INVALID_SOCKET) && (has_IPv4))
	{
		// try IPv4 next...
		for (addrinfo *addr = pResult; addr != NULL; addr = addr->ai_next)
		{
			if (addr->ai_family != AF_INET)
				continue;

			std::cout << "Connecting to IPv4 : " << addr_to_str(addr) << " | Port : " << PORT << "\n" << std::endl;

			sock = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
			if (sock == INVALID_SOCKET)
			{
				ret = WSAGetLastError();
				std::cerr << "socket error: " << ret << "\n" << std::endl;
				continue;
			}

			if (connect(sock, addr->ai_addr, addr->ai_addrlen) == SOCKET_ERROR)
			{
				ret = WSAGetLastError();
				std::cerr << "connect error: " << ret << "\n" << std::endl;
				closesocket(sock);
				sock = INVALID_SOCKET;
				continue;
			}

			break;
		}
	}

	freeaddrinfo(pResult);

	if (sock == INVALID_SOCKET)
	{
		WSACleanup();
		return -2;
	}

	std::cout << "Connect Successful" << "\n" << std::endl;

	char buf[1024 * 4];

	int size_recv = recv(sock, buf, sizeof(buf), 0);
	if (size_recv == SOCKET_ERROR)
	{
		ret = WSAGetLastError();
		std::cerr << "recv error: " << ret << "\n" << std::endl;
	}
	else
	{
		std::cout << "SIZE RECV = " << size_recv;
		if (size_recv > 0)
		{
			std::cout << " | DATA RECV = \n";
			std::cout.write(buf, size_recv);
		}
		std::cout << std::endl;
	}

	closesocket(sock);
	WSACleanup();

	std::cin.get();
	return 0;
}
