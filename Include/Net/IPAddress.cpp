#include "IPAddress.hpp"

// Private

IPAddress::IPAddress()
{
}

// Public

// typename<size_t TSize>
// IPAddress(char addr[TSize], short port);

IPAddress::IPAddress(char* addr, size_t size, short port)
	: _Addr(new char[size])
	, _Size(size)
	, _Port(port)
{
	std::memcpy(_Addr, addr, size);
}

IPAddress::~IPAddress()
{
	if (_Addr != nullptr)
	{
		delete [] _Addr;
		_Addr = nullptr;
	}
}

IPAddress* IPAddress::ParseIPv4(const char* str)
{
	IPAddress rv = new IPAddress();
	rv->_Size = 4;
	auto addr = rv->_Addr = new char[4]; 
	
	char pre = '\0';
	size_t pos = 0;
	while (str[pos] != '\0') // Check every char till EOS.
	{
		if (str[pos] >= '0' && str[pos] <= '9') // Is digit?
			*addr = *addr * 10 + (str[pos] - '0');
		else if (str[pos] == '.') // Is dot?
		{
			if (pre < '0' && pre > '9') // Make sure the previous char is a digit.
			{
				delete rv;
				return nullptr;
			}
			++addr;
		}
		else if (str[pos] == ':') // Is colon?
		{
			++pos;
			while (str[pos] != '\0')
			{
				if (pre < '0' && pre > '9') // Make sure the previous char is a digit.
				{
					delete rv;
					return nullptr;
				}
				if (
				++pos;
			}
		}
		else // Invalid char!
		{
			delete rv;
			return nullptr;
		}
		pre = str[pos++];
	}
}

IPAddress::ParseIPv6(const char* str)
{
	for (size_t pos = 0; pos < _Size; ++pos)
	{
		if (TSize[pos] == ':')
		{
			
		}
	}
}