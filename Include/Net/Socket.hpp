// File: Socket.hpp
// Author: Rendong Liang (Liong)

#pragma once
#include "../Fundamental.hpp"
#include "../Buffer.hpp"
#include "SocketAddress.hpp"

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
			StartUpNetModule(StartUpNetModule&) = delete;
			StartUpNetModule(StartUpNetModule&&) = delete;
			~StartUpNetModule();
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

			Socket();
			Socket(HSocket hSocket);

			bool IsErrorOccured(int code);
		public:
			Socket(int addressFamily, int type, int protocal);
			Socket(const Socket&) = delete;
			Socket(Socket&& instance);
			~Socket();

			Socket& operator=(Socket&& instance);
			
			Socket Accept(SocketAddress& addr);
			void Bind(const SocketAddress& addr);
			void Close();
			void Connect(const SocketAddress& addr);
			void Listen(int backlog);
			void Send(const Buffer& buffer);
			void Send(const Buffer& buffer, int flags);
			void Receive(Buffer& buffer);
			void Receive(Buffer& buffer, int flags);
			void SendTo(Buffer& buffer, const SocketAddress& addr);
			void SendTo(Buffer& buffer, const SocketAddress& addr, int flags);
			void ReceiveFrom(Buffer& buffer, SocketAddress& addr);
			void ReceiveFrom(Buffer& buffer, SocketAddress& addr, int flags);
		};
	}
}