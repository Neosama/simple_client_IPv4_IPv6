
std::string char_to_string(char x[], int size_recv)
{
	int num_car = 0;
	int stop_while = size_recv;
	std::string output = "";

	while (stop_while > 0)
	{
		output += x[num_car];
		if (num_car < size_recv) { num_car++; }

		stop_while--;
	}

	return output;
}

std::string hex_string(std::string hexstr)
{
	std::string str = "";
	str.resize((hexstr.size() + 1) / 2);
	for (size_t i = 0, j = 0; i < str.size(); i++, j++)
	{
		char at = '@';
		str[i] = (hexstr[j] & at ? hexstr[j] + 9 : hexstr[j]) << 4, j++;
		str[i] |= (hexstr[j] & at ? hexstr[j] + 9 : hexstr[j]) & 0xF;
	}
	return str;
}

std::string string_hex(std::string str, const bool capital = false)
{
	std::string hexstr = "";
	hexstr.resize(str.size() * 2);
	static const char a = capital ? 0x40 : 0x60;
	for (size_t i = 0; i < str.size(); i++)
	{
		char c = (str[i] >> 4) & 0xF;
		hexstr[i * 2] = c > 9 ? (c - 9) | a : c | '0';
		hexstr[i * 2 + 1] = (str[i] & 0xF) > 9 ? (str[i] - 9) & 0xF | a : str[i] & 0xF | '0';
	}
	return hexstr;
}

std::string addr_to_str(addrinfo *addr)
{
	char str[128];
	int ret = 0;

	switch (addr->ai_family)
	{

	case AF_INET: // IPv4
	{
		if (inet_ntop(AF_INET, &(((sockaddr_in*)(addr->ai_addr))->sin_addr), str, INET_ADDRSTRLEN))
			return str;

		ret = WSAGetLastError();
		break;
	}

	case AF_INET6: // IPv6
	{
		if (inet_ntop(AF_INET6, &(((sockaddr_in6*)(addr->ai_addr))->sin6_addr), str, INET6_ADDRSTRLEN))
			return str;

		ret = WSAGetLastError();
		break;
	}

	default:
		ret = WSAEAFNOSUPPORT;
		break;
	}

	std::cerr << "inet_ntop error: " << ret << std::endl;
	return "";
}
