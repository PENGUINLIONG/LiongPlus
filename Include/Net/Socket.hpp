// File: Socket.hpp
// Author: Rendong Liang (Liong)

#pragma once
#include "../Fundamental.hpp"
#include "../Buffer.hpp"
#include "SocketAddress.hpp"
#include "HttpMessage.hpp"

namespace LiongPlus
{
	namespace Net
	{
		using namespace LiongPlus;

		class StartUpNetModule
		{
		private:
#ifdef _L_WINDOWS
			WSADATA _Data;
#else
			bool _Trash;
#endif
		public:
			StartUpNetModule();
			StartUpNetModule(const StartUpNetModule&) = delete;
			StartUpNetModule(StartUpNetModule&&) = delete;
			~StartUpNetModule();
		};

		struct SocketResult
		{
			bool IsErrorOccurred = false;
			size_t Amount = 0;
		};

		class Socket
		{
		private:
#ifdef _L_WINDOWS
			typedef SOCKET HSocket;
#else
			typedef int HSocket;
#endif
			HSocket _HSocket;

			Socket(HSocket hSocket);

			bool IsErrorOccured(int code);
		public:
			Socket();
			Socket(int addressFamily, int type, int protocal);
			Socket(const Socket&) = delete;
			Socket(Socket&& instance);
			~Socket();

			Socket& operator=(Socket&& instance);
			
			Socket Accept(SocketAddress& addr);
			bool Bind(const SocketAddress& addr);
			bool Close();
			bool Connect(const SocketAddress& addr);
			bool Listen(int backlog);
			SocketResult Send(const Buffer& buffer, size_t offset = 0);
			SocketResult Send(const Buffer& buffer, size_t offset, int flags);
			bool SetSendTimeOut(uint32_t ms);
			bool SetOption(int flags, uint32_t value);
			bool SetOption(int flags, Buffer& value);
			bool SetReceiveTimeOut(uint32_t ms);
			SocketResult Receive(Buffer& buffer, size_t offset = 0);
			SocketResult Receive(Buffer& buffer, size_t offset, int flags);
			SocketResult SendTo(Buffer& buffer, size_t offset, const SocketAddress& addr);
			SocketResult SendTo(Buffer& buffer, size_t offset, const SocketAddress& addr, int flags);
			SocketResult ReceiveFrom(Buffer& buffer, size_t offset, SocketAddress& addr);
			SocketResult ReceiveFrom(Buffer& buffer, size_t offset, SocketAddress& addr, int flags);

			bool IsValid() const;
		};
	}
}