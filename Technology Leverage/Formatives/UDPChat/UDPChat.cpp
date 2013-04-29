#define _WIN32_MEAN_AND_LEAN

#include <ctime>
#include <map>
#include <set>

#include <winsock2.h> // do before windows please

#include <windows.h>
#include <stdio.h>
#include <ctype.h>
#include <conio.h>
#include <assert.h>

// now fix FD_SET so it doesn't create nasty warnings:
void __FD_SET( SOCKET s, fd_set* pSet )
{
#pragma warning ( push )
#pragma warning ( disable: 4127 )
	FD_SET( s, pSet );
#pragma warning ( pop )
}
#undef FD_SET
#define FD_SET( s, pSet ) __FD_SET( s, pSet );
// end of fix warnings for FD_SET

#include "ConsoleTools.h"
#include "MessageType.h"

// make sure the winsock lib is included...
#pragma comment(lib,"ws2_32.lib")

const char* cHELP_COMMAND = "Commands:\n? = Show commands\nq = Quit\n";

const static int iKEEP_ALIVE_FREQUENCY = 10;
const static int iKEEP_ALIVE_MULTIPLIER = 3;
const static int iMAX_CLIENTS = 10;
const static int iNET_SLEEP = 30;

const static unsigned short uDEFAULT_CLIENT_PORT = (unsigned short)60012;
const static unsigned short uDEFAULT_SERVER_PORT = (unsigned short)60013;
const static unsigned short uCLIENT_PORT_INCR = (unsigned short)1000;
const static unsigned short uCLIENT_PORT_MAX_INCR = (unsigned short)10000;

const static UINT32 uMAX_MESSAGE_LENGTH = 256;

// globals:
std::set<sockaddr_in> g_clientAddresses;
std::map<sockaddr_in, time_t> g_clientKeepAliveReceiveTimes;
std::map<sockaddr_in, time_t> g_clientKeepAliveSendTimes;
std::map<sockaddr_in, std::string> g_clientNames;
time_t g_clientSendTime = 0;
bool g_bConnected = false;
bool g_bServer = false;

void addClient(SOCKET _socket, sockaddr_in& _clientAddress, char* _pName, int _iNameLength);
int bind(SOCKET _socket, sockaddr_in& _address, bool _bServer, unsigned short _uClientPort);
void broadcastMessage(SOCKET _socket, EMessageType _type, const char* _kpPayload, const int _kiPayloadLength,
		std::set<sockaddr_in>& _addresses);
void unicastMessage(SOCKET _socket, EMessageType _type, const char* _kpPayload, const int _kiPayloadLength,
		sockaddr_in& _address);
bool operator<(const sockaddr_in& _rLhs, const sockaddr_in& _rRhs);
bool receive(SOCKET _socket, sockaddr_in& _sourceAddress, char* _kpPayload, int& _kiPayloadLength);
void sendKeepAlives(SOCKET _socket);
void PrintWSAErrorInfo( int iError );

void addClient(SOCKET _socket, sockaddr_in& _clientAddress, char* _pName, int _iNameLength)
{
	std::string clientName(_pName, _iNameLength);
	for (std::map<sockaddr_in, std::string>::iterator iterator = g_clientNames.begin();
			iterator != g_clientNames.end(); iterator++)
	{
		if (strcmp(clientName.c_str(), iterator->second.c_str()) == 0)
		{
			char cJoinDenied[uMAX_MESSAGE_LENGTH];
			cJoinDenied[0] = 0; // Null terminate.
			strcat_s(cJoinDenied, "The name '");
			strcat_s(cJoinDenied, clientName.c_str());
			strcat_s(cJoinDenied, "' is already taken.");
			unicastMessage(_socket, MT_JOIN_DENIED, cJoinDenied, strlen(cJoinDenied), _clientAddress);

			return;
		}
	}

	char cJoinAccepted[uMAX_MESSAGE_LENGTH];
	cJoinAccepted[0] = 0; // Null terminate.
	strcat_s(cJoinAccepted, "Welcome to the conversation, ");
	strcat_s(cJoinAccepted, clientName.c_str());
	if (g_clientNames.empty())
	{
		strcat_s(cJoinAccepted, ". You are the first member.");
	}
	else
	{
		strcat_s(cJoinAccepted, ". The current members are:\n");
		for (std::map<sockaddr_in, std::string>::iterator iterator = g_clientNames.begin();
				iterator != g_clientNames.end(); iterator++)
		{
			strcat_s(cJoinAccepted, iterator->second.c_str());
			strcat_s(cJoinAccepted, "\n");
		}
	}
	unicastMessage(_socket, MT_JOIN_ACCEPTED, cJoinAccepted, strlen(cJoinAccepted), _clientAddress);

	char cJoinNotify[uMAX_MESSAGE_LENGTH];
	cJoinNotify[0] = 0; // Null terminate.
	strcat_s(cJoinNotify, clientName.c_str());
	strcat_s(cJoinNotify, " has joined the conversation.");
	broadcastMessage(_socket, MT_JOIN_NOTIFY, cJoinNotify, strlen(cJoinNotify), g_clientAddresses);

	g_clientAddresses.insert(_clientAddress);
	g_clientKeepAliveReceiveTimes[_clientAddress] = time(NULL);
	g_clientNames[_clientAddress] = clientName;
}

int bind(SOCKET _socket, sockaddr_in& _address, bool _bServer, unsigned short _uClientPort)
{
	while ( 0 != bind( _socket, reinterpret_cast<sockaddr*>(&_address), sizeof( _address ) ) )
	{
		if (_bServer || _uClientPort > uDEFAULT_CLIENT_PORT + uCLIENT_PORT_MAX_INCR)
		{
			// error
			closesocket( _socket );
			printf( "unable to bind socket to my address - is something else running?\n" );
			assert( false );
			return 1;
		}

		_uClientPort += uCLIENT_PORT_INCR;
		printf("unable to bind socket to my address - trying %i\n", _uClientPort);
		_address.sin_port = htons(_uClientPort);
	}

	return 0;
}

void broadcastMessage(SOCKET _socket, EMessageType _type, const char* _kpPayload, const int _kiPayloadLength,
		std::set<sockaddr_in>& _addresses)
{
	for (std::set<sockaddr_in>::iterator iterator = _addresses.begin();
		iterator != _addresses.end(); iterator++)
	{
		unicastMessage(_socket, _type, _kpPayload, _kiPayloadLength, *iterator);
	}
}

void unicastMessage(SOCKET _socket, EMessageType _type, const char* _kpPayload, const int _kiPayloadLength,
		sockaddr_in& _address)
{
	char* pTypeAndPayload = new char[_kiPayloadLength + 2];
	pTypeAndPayload[0] = (char) _type;

	if (_kpPayload != NULL)
	{
		strncpy_s(pTypeAndPayload + 1, _kiPayloadLength + 1, _kpPayload, _kiPayloadLength + 1);
	}
	else
	{
		pTypeAndPayload[1] = 0; // Null terminate.
	}

	sendto(
		_socket,								// socket to send from
		pTypeAndPayload,						// packet data
		_kiPayloadLength + 2,					// num bytes in packet
		0,										// flags
		reinterpret_cast<sockaddr*>(&_address),	// address to be sent to
		sizeof(_address)						// size of the above address struct.
	);

	delete pTypeAndPayload;
}

// Just so that sockaddr_in can be stored in a set.
bool operator<(const sockaddr_in& _rLhs, const sockaddr_in& _rRhs)
{
	if (_rLhs.sin_addr.S_un.S_addr < _rRhs.sin_addr.S_un.S_addr)
	{
		return true;
	}

	if (_rLhs.sin_port < _rRhs.sin_port)
	{
		return true;
	}

	return false;
}

bool receive(SOCKET _socket, sockaddr_in& _sourceAddress, char* _kpPayload, int& _kiPayloadLength)
{
	FD_SET ErrorDesc;
	FD_SET ReadDesc;
	FD_ZERO(&ErrorDesc);
	FD_SET(_socket, &ErrorDesc);
	FD_ZERO(&ReadDesc);
	FD_SET(_socket, &ReadDesc);

	TIMEVAL timeVal;
	timeVal.tv_sec = 0;
	timeVal.tv_usec = iNET_SLEEP;

	// wait for either a time out (for client) or for something to be ready to process,
	// whichever happens first.
	int iNumReady = select( 
			1,				// only one file descripter in the FileDesc array (or rather not array)
			&ReadDesc,		// Set To check for read notifications
			NULL,			// Set to check for write notifications
							// nothing as we do not neet to block on write being ready
							// as we can just throw datagrams at the system but would matter
							// for the TCP system.
							// if this is set it means that we can write if we wanted to..
			&ErrorDesc,		// Set to check for exception notifications
			&timeVal	);	// timeval structure to say how long to wait for a notification.
							// NULL means block forever until something happens.

	if (SOCKET_ERROR == iNumReady)
	{
		assert( false && "There was an error... exit n clean up?" );
	}

	if (FD_ISSET(_socket, &ErrorDesc))
	{
		// wierd as I don't know what error conditions can occur on UDP connections.
		assert( false && "Ohh, an error on the connection..." );
	}

	if (FD_ISSET(_socket, &ReadDesc))
	{
		// pull off the packet(s) - wonder if we look does it break?
		int iSizeOfAdd = sizeof(_sourceAddress);
		_kiPayloadLength = recvfrom(						// pulls a packet from a single source...
			_socket,										// server socket to read from
			_kpPayload,										// incoming packet to be filled
			uMAX_MESSAGE_LENGTH,							// lengh of incoming packet to be filled
			0,												// flags
			reinterpret_cast<sockaddr*>(&_sourceAddress),	// address to be filled with packet source
			&iSizeOfAdd										// size of the above address struct.
			);

		if (_kiPayloadLength < 0)
		{
			if (WSAGetLastError() == WSAECONNRESET)
			{
				if (!g_bServer)
				{
					printf("Sorry, the conversation has disappeared!\n");
					g_clientSendTime = 0;
				}

				return false;
			}
			else
			{
				PrintWSAErrorInfo(WSAGetLastError());
			}
		}

		if (_kiPayloadLength != (int) strlen(_kpPayload) + 1)
		{
			// we have a problem... nothing good will come of it :-)...
			assert(false && "packet that is not just a string recieved");
		}

		return true;
	}

	return false;
}

void sendKeepAlives(SOCKET _socket)
{
	time_t now = time(NULL);

	std::set<sockaddr_in>::iterator iterator = g_clientAddresses.begin();
	while (iterator != g_clientAddresses.end())
	{
		if (now - g_clientKeepAliveReceiveTimes[*iterator] >= iKEEP_ALIVE_FREQUENCY * iKEEP_ALIVE_MULTIPLIER)
		{
			std::string clientName = g_clientNames[*iterator];

			g_clientKeepAliveReceiveTimes.erase(*iterator);
			g_clientKeepAliveSendTimes.erase(*iterator);
			g_clientNames.erase(*iterator);
			iterator = g_clientAddresses.erase(iterator);

			char cDisconnectNotify[uMAX_MESSAGE_LENGTH];
			cDisconnectNotify[0] = 0; // Null terminate.
			strcat_s(cDisconnectNotify, clientName.c_str());
			strcat_s(cDisconnectNotify, " has been disconnected from the conversation.");
			broadcastMessage(_socket, MT_DISCONNECT_NOTIFY, cDisconnectNotify, strlen(cDisconnectNotify), g_clientAddresses);
		}
		else
		{
			if (now - g_clientKeepAliveReceiveTimes[*iterator] >= iKEEP_ALIVE_FREQUENCY)
			{
				unicastMessage(_socket, MT_KEEP_ALIVE, NULL, 0, *iterator);
				g_clientKeepAliveSendTimes[*iterator] = now;
			}

			iterator++;
		}
	}
}

int main( int , const char*[] )
{
	// query, is this to be a client or a server?
	g_bServer = 'S' == QueryOption( "Do you want to run a client or server (C/S)?", "CS" );

	unsigned short uCLIENT_PORT = uDEFAULT_CLIENT_PORT;
	if (!g_bServer)
	{
		printf( "Client (enter for default): " );
		uCLIENT_PORT = QueryPortNumber(uDEFAULT_CLIENT_PORT);
	}

	sockaddr_in serverAddress;	// address to send to..

	CInputLineBuffer InputBuffer( uMAX_MESSAGE_LENGTH );

	// startup windows sockets:
	WSADATA wsaData;
	WSAStartup(0x0202,&wsaData);

	// this buffer will contain all packet data.
	char PacketData[uMAX_MESSAGE_LENGTH];

	sockaddr_in myAddress;
	memset( &myAddress, 0, sizeof( myAddress ) );
	myAddress.sin_family = AF_INET;
	myAddress.sin_addr.S_un.S_addr = INADDR_ANY;	// as this is a port on this machine to listen to..
	myAddress.sin_port=g_bServer?htons(uDEFAULT_SERVER_PORT):htons(uCLIENT_PORT);

	// now try to create a socket on the above address
	SOCKET sock = socket( PF_INET, SOCK_DGRAM, IPPROTO_UDP );
	if ( INVALID_SOCKET == sock )
	{
		printf( "unable to create socket\n" );
		assert( false );
		return 1;
	}

	// now bind the socket to the address.
	if (bind(sock, myAddress, g_bServer, uCLIENT_PORT) == 1)
	{
		return 1;
	}

    BOOL bOptVal = TRUE;
	setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char*) &bOptVal, sizeof(BOOL));

	std::set<sockaddr_in> serverAddresses;
	if (g_bServer)
	{
		printf("Server ready!\n");
	}
	else
	{
		printf("Scanning for servers...\n");

		sockaddr_in broadcastServerAddress;
		memset(&broadcastServerAddress, 0, sizeof(broadcastServerAddress));
		broadcastServerAddress.sin_family = AF_INET;
		broadcastServerAddress.sin_addr.S_un.S_addr = INADDR_BROADCAST;
		broadcastServerAddress.sin_port = htons(uDEFAULT_SERVER_PORT);
		// Suddenly the naming of this function seems a little silly...
		unicastMessage(sock, MT_SERVER_DETECT, NULL, 0, broadcastServerAddress);

		time_t scanStart = time(NULL);
		while (time(NULL) - scanStart < 3)
		{
			sockaddr_in sourceAddress;
			int iPayloadLength;
			if (receive(sock, sourceAddress, PacketData, iPayloadLength))
			{
				if (PacketData[0] == MT_SERVER_AVAILABLE)
				{
					serverAddresses.insert(sourceAddress);
				}
			}
		}

		printf("Available servers:\n");
		int index = 1;
		for (std::set<sockaddr_in>::iterator iterator = serverAddresses.begin();
			iterator != serverAddresses.end(); iterator++)
		{
			printf("%i > %s:%i\n", index++, inet_ntoa(iterator->sin_addr), ntohs(iterator->sin_port));
		}

		printf("What server do you want to connect to? ");

		char serverOption = (char) getchar();
		std::set<sockaddr_in>::iterator iterator = serverAddresses.begin();
		index = atoi(&serverOption);
		for (int index2 = 1; index2 < index; index2++)
		{
			iterator++;
		}
		serverAddress = *iterator;
		printf("Connected to %s:%i\n", inet_ntoa(serverAddresses.begin()->sin_addr), ntohs(serverAddresses.begin()->sin_port));

		// prepare for reading input:
		InputBuffer.PrintToScreenTop();

		printf("What is your name?\n");
	}

	for (;;)	// ever
	{
		if (g_bServer)
		{
			sendKeepAlives(sock);
		}
		else
		{
			if (g_clientSendTime != 0 && time(NULL) - g_clientSendTime >= 3)
			{
				printf( "Sorry, the conversation has disappeared!\n" );
				g_clientSendTime = 0;
			}

			if ( InputBuffer.Update() )
			{
				const char* input = InputBuffer.GetString();
				int inputLength = strlen(input);

				if (g_bConnected)
				{
					if (input[0] == '!')
					{
						if (input[1] == '!')
						{
							// we completed a message, lets send it please:
							unicastMessage(sock, MT_CHAT, &input[1], inputLength - 1, serverAddress);
							g_clientSendTime = time(NULL);
						}
						else if (input[1] == '?')
						{
							printf(cHELP_COMMAND);
						}
						else if (input[1] == 'q')
						{
							unicastMessage(sock, MT_QUIT_REQUEST, NULL, 0, serverAddress);
							printf("You have left the conversation.\n");
						}
					}
					else
					{
						// we completed a message, lets send it please:
						unicastMessage(sock, MT_CHAT, input, inputLength, serverAddress);
						g_clientSendTime = time(NULL);
					}
				}
				else
				{
					// we completed a message, lets send it please:
					unicastMessage(sock, MT_JOIN_REQUEST, input, inputLength, serverAddress);
					g_clientSendTime = time(NULL);
				}

				InputBuffer.ClearString();
				InputBuffer.PrintToScreenTop();
			}
		}

		sockaddr_in fromAdd;
		int iNumBytes;
		if (!receive(sock, fromAdd, PacketData, iNumBytes))
		{
			continue;
		}

		if ( g_bServer )
		{
			if (PacketData[0] == (char) MT_CHAT)
			{
				printf( "%s:%i > MT_CHAT %s\n", inet_ntoa(fromAdd.sin_addr), fromAdd.sin_port, &PacketData[1] );
				if (g_clientAddresses.find(fromAdd) == g_clientAddresses.end())
				{
					unicastMessage(sock, MT_ERROR_NOT_JOINED, NULL, 0, fromAdd);
				}
				else
				{
					char nameAndMessage[uMAX_MESSAGE_LENGTH];
					nameAndMessage[0] = 0; // Null terminate.
					strcat_s(nameAndMessage, g_clientNames[fromAdd].c_str());
					strcat_s(nameAndMessage, ": ");
					strncat_s(nameAndMessage, &PacketData[1], iNumBytes - 2);
					broadcastMessage(sock, MT_CHAT, nameAndMessage, strlen(nameAndMessage), g_clientAddresses);
				}
			}
			else if (PacketData[0] == (char) MT_JOIN_REQUEST)
			{
				printf( "%s:%i > MT_JOIN_REQUEST %s\n", inet_ntoa(fromAdd.sin_addr), ntohs(fromAdd.sin_port), &PacketData[1] );
				addClient(sock, fromAdd, &PacketData[1], iNumBytes - 2);
			}
			else if (PacketData[0] == (char) MT_KEEP_ALIVE)
			{
				printf( "%s:%i > MT_KEEP_ALIVE\n", inet_ntoa(fromAdd.sin_addr), ntohs(fromAdd.sin_port) );
				g_clientKeepAliveReceiveTimes[fromAdd] = time(NULL);
			}
			else if (PacketData[0] == (char) MT_QUIT_REQUEST)
			{
				printf( "%s:%i > MT_QUIT_REQUEST\n", inet_ntoa(fromAdd.sin_addr), ntohs(fromAdd.sin_port) );

				char quitNotify[uMAX_MESSAGE_LENGTH];
				quitNotify[0] = 0; // Null terminate.
				strcat_s(quitNotify, g_clientNames[fromAdd].c_str());
				strcat_s(quitNotify, " has left the conversation.");
				g_clientAddresses.erase(fromAdd);
				g_clientKeepAliveReceiveTimes.erase(fromAdd);
				g_clientKeepAliveSendTimes.erase(fromAdd);
				g_clientNames.erase(fromAdd);
				broadcastMessage(sock, MT_QUIT_NOTIFY, quitNotify, strlen(quitNotify), g_clientAddresses);
			}
			else if (PacketData[0] == (char) MT_SERVER_DETECT)
			{
				printf( "%s:%i > MT_SERVER_DETECT\n", inet_ntoa(fromAdd.sin_addr), ntohs(fromAdd.sin_port) );

				unicastMessage(sock, MT_SERVER_AVAILABLE, NULL, 0, fromAdd);
			}
			else
			{
				printf("%s:%i > The client is doing it wrong!\n", inet_ntoa(fromAdd.sin_addr), ntohs(fromAdd.sin_port) );
			}
		}
		else
		{
			if (PacketData[0] == (char) MT_CHAT
				|| PacketData[0] == (char) MT_DISCONNECT_NOTIFY
				|| PacketData[0] == (char) MT_JOIN_ACCEPTED
				|| PacketData[0] == (char) MT_JOIN_NOTIFY
				|| PacketData[0] == (char) MT_QUIT_NOTIFY)
			{
				g_bConnected = true;
				g_clientSendTime = 0;
				printf("%s\n", &PacketData[1] );
			}
			else if (PacketData[0] == (char) MT_ERROR_NOT_JOINED)
			{
				printf("Sorry, you've been disconnected from the conversation. Try re-connecting.\n");
			}
			else if (PacketData[0] == (char) MT_JOIN_DENIED)
			{
				printf("Sorry, that name is already taken. Please try again.\n");
			}
			else if (PacketData[0] == (char) MT_KEEP_ALIVE)
			{
				unicastMessage(sock, MT_KEEP_ALIVE, NULL, 0, serverAddress);
			}
			else
			{
				printf("The server is doing it wrong!\n");
			}
		}
	}
//	return 0;
}

/**
 *	Note: look up problems in help for specific function that spawned them as that may be more descriptive.
 */
void PrintWSAErrorInfo( int iError )
{
	switch( iError )
	{
	case WSA_INVALID_HANDLE:
		printf( "WSA_INVALID_HANDLE\n" );
		printf( "Specified event object handle is invalid.\n" );
		printf( "An application attempts to use an event object, but the specified handle is not valid. Note that this error is returned by the operating system, so the error number may change in future releases of Windows.\n" );
		break;
	case WSA_NOT_ENOUGH_MEMORY:
		printf( "WSA_NOT_ENOUGH_MEMORY\n" );
		printf( "Insufficient memory available.\n" );
		printf( "An application used a Windows Sockets function that directly maps to a Windows function. The Windows function is indicating a lack of required memory resources. Note that this error is returned by the operating system, so the error number may change in future releases of Windows.\n" );
		break;
	case WSA_INVALID_PARAMETER:
		printf( "WSA_INVALID_PARAMETER\n" );
		printf( "One or more parameters are invalid.\n" );
		printf( "An application used a Windows Sockets function which directly maps to a Windows function. The Windows function is indicating a problem with one or more parameters. Note that this error is returned by the operating system, so the error number may change in future releases of Windows.\n" );
		break;
	case WSA_OPERATION_ABORTED:
		printf( "WSA_OPERATION_ABORTED\n" );
		printf( "Overlapped operation aborted.\n" );
		printf( "An overlapped operation was canceled due to the closure of the socket, or the execution of the SIO_FLUSH command in WSAIoctl. Note that this error is returned by the operating system, so the error number may change in future releases of Windows.\n" );
		break;
	case WSA_IO_INCOMPLETE:
		printf( "WSA_IO_INCOMPLETE\n" );
		printf( "Overlapped I/O event object not in signaled state.\n" );
		printf( "The application has tried to determine the status of an overlapped operation which is not yet completed. Applications that use WSAGetOverlappedResult (with the fWait flag set to false) in a polling mode to determine when an overlapped operation has completed, get this error code until the operation is complete. Note that this error is returned by the operating system, so the error number may change in future releases of Windows.\n" );
		break;
	case WSA_IO_PENDING:
		printf( "WSA_IO_PENDING\n" );
		printf( "Overlapped operations will complete later.\n" );
		printf( "The application has initiated an overlapped operation that cannot be completed immediately. A completion indication will be given later when the operation has been completed. Note that this error is returned by the operating system, so the error number may change in future releases of Windows.\n" );
		break;
	case WSAEINTR:
		printf( "WSAEINTR\n" );
		printf( "Interrupted function call.\n" );
		printf( "A blocking operation was interrupted by a call to WSACancelBlockingCall.\n" );
		break;
	case WSAEBADF:
		printf( "WSAEBADF\n" );
		printf( "File handle is not valid.\n" );
		printf( "The file handle supplied is not valid.\n" );
		break;
	case WSAEACCES:
		printf( "WSAEACCES\n" );
		printf( "Permission denied.\n" );
		printf( "An attempt was made to access a socket in a way forbidden by its access permissions. An example is using a broadcast address for sendto without broadcast permission being set using setsockopt(SO_BROADCAST).    Another possible reason for the WSAEACCES error is that when the bind function is called (on Windows NT 4.0 with SP4 and later), another application, service, or kernel mode driver is bound to the same address with exclusive access. Such exclusive access is a new feature of Windows NT 4.0 with SP4 and later, and is implemented by using the SO_EXCLUSIVEADDRUSE option.\n" );
		break;
	case WSAEFAULT:
		printf( "WSAEFAULT\n" );
		printf( "Bad address.\n" );
		printf( "The system detected an invalid pointer address in attempting to use a pointer argument of a call. This error occurs if an application passes an invalid pointer value, or if the length of the buffer is too small. For instance, if the length of an argument, which is a sockaddr structure, is smaller than the sizeof(sockaddr).\n" );
		break;
	case WSAEINVAL:
		printf( "WSAEINVAL\n" );
		printf( "Invalid argument.\n" );
		printf( "Some invalid argument was supplied (for example, specifying an invalid level to the setsockopt function). In some instances, it also refers to the current state of the socket—for instance, calling accept on a socket that is not listening.\n" );
		break;
	case WSAEMFILE:
		printf( "WSAEMFILE\n" );
		printf( "Too many open files.\n" );
		printf( "Too many open sockets. Each implementation may have a maximum number of socket handles available, either globally, per process, or per thread.\n" );
		break;
	case WSAEWOULDBLOCK:
		printf( "WSAEWOULDBLOCK\n" );
		printf( "Resource temporarily unavailable.\n" );
		printf( "This error is returned from operations on nonblocking sockets that cannot be completed immediately, for example recv when no data is queued to be read from the socket. It is a nonfatal error, and the operation should be retried later. It is normal for WSAEWOULDBLOCK to be reported as the result from calling connect on a nonblocking SOCK_STREAM socket, since some time must elapse for the connection to be established.\n" );
		break;
	case WSAEINPROGRESS:
		printf( "WSAEINPROGRESS\n" );
		printf( "Operation now in progress.\n" );
		printf( "A blocking operation is currently executing. Windows Sockets only allows a single blocking operation—per- task or thread—to be outstanding, and if any other function call is made (whether or not it references that or any other socket) the function fails with the WSAEINPROGRESS error.\n" );
		break;
	case WSAEALREADY:
		printf( "WSAEALREADY\n" );
		printf( "Operation already in progress.\n" );
		printf( "An operation was attempted on a nonblocking socket with an operation already in progress—that is, calling connect a second time on a nonblocking socket that is already connecting, or canceling an asynchronous request (WSAAsyncGetXbyY) that has already been canceled or completed.\n" );
		break;
	case WSAENOTSOCK:
		printf( "WSAENOTSOCK\n" );
		printf( "Socket operation on nonsocket.\n" );
		printf( "An operation was attempted on something that is not a socket. Either the socket handle parameter did not reference a valid socket, or for select, a member of an fd_set was not valid.\n" );
		break;
	case WSAEDESTADDRREQ:
		printf( "WSAEDESTADDRREQ\n" );
		printf( "Destination address required.\n" );
		printf( "A required address was omitted from an operation on a socket. For example, this error is returned if sendto is called with the remote address of ADDR_ANY.\n" );
		break;
	case WSAEMSGSIZE:
		printf( "WSAEMSGSIZE\n" );
		printf( "Message too long.\n" );
		printf( "A message sent on a datagram socket was larger than the internal message buffer or some other network limit, or the buffer used to receive a datagram was smaller than the datagram itself.\n" );
		break;
	case WSAEPROTOTYPE:
		printf( "WSAEPROTOTYPE\n" );
		printf( "Protocol wrong type for socket.\n" );
		printf( "A protocol was specified in the socket function call that does not support the semantics of the socket type requested. For example, the ARPA Internet UDP protocol cannot be specified with a socket type of SOCK_STREAM.\n" );
		break;
	case WSAENOPROTOOPT:
		printf( "WSAENOPROTOOPT\n" );
		printf( "Bad protocol option.\n" );
		printf( "An unknown, invalid or unsupported option or level was specified in a getsockopt or setsockopt call.\n" );
		break;
	case WSAEPROTONOSUPPORT:
		printf( "WSAEPROTONOSUPPORT\n" );
		printf( "Protocol not supported.\n" );
		printf( "The requested protocol has not been configured into the system, or no implementation for it exists. For example, a socket call requests a SOCK_DGRAM socket, but specifies a stream protocol.\n" );
		break;
	case WSAESOCKTNOSUPPORT:
		printf( "WSAESOCKTNOSUPPORT\n" );
		printf( "Socket type not supported.\n" );
		printf( "The support for the specified socket type does not exist in this address family. For example, the optional type SOCK_RAW might be selected in a socket call, and the implementation does not support SOCK_RAW sockets at all.\n" );
		break;
	case WSAEOPNOTSUPP:
		printf( "WSAEOPNOTSUPP\n" );
		printf( "Operation not supported.\n" );
		printf( "The attempted operation is not supported for the type of object referenced. Usually this occurs when a socket descriptor to a socket that cannot support this operation is trying to accept a connection on a datagram socket.\n" );
		break;
	case WSAEPFNOSUPPORT:
		printf( "WSAEPFNOSUPPORT\n" );
		printf( "Protocol family not supported.\n" );
		printf( "The protocol family has not been configured into the system or no implementation for it exists. This message has a slightly different meaning from WSAEAFNOSUPPORT. However, it is interchangeable in most cases, and all Windows Sockets functions that return one of these messages also specify WSAEAFNOSUPPORT.\n" );
		break;
	case WSAEAFNOSUPPORT:
		printf( "WSAEAFNOSUPPORT\n" );
		printf( "Address family not supported by protocol family.\n" );
		printf( "An address incompatible with the requested protocol was used. All sockets are created with an associated address family (that is, AF_INET for Internet Protocols) and a generic protocol type (that is, SOCK_STREAM). This error is returned if an incorrect protocol is explicitly requested in the socket call, or if an address of the wrong family is used for a socket, for example, in sendto.\n" );
		break;
	case WSAEADDRINUSE:
		printf( "WSAEADDRINUSE\n" );
		printf( "Address already in use.\n" );
		printf( "Typically, only one usage of each socket address (protocol/IP address/port) is permitted. This error occurs if an application attempts to bind a socket to an IP address/port that has already been used for an existing socket, or a socket that was not closed properly, or one that is still in the process of closing. For server applications that need to bind multiple sockets to the same port number, consider using setsockopt (SO_REUSEADDR). Client applications usually need not call bind at all— connect chooses an unused port automatically. When bind is called with a wildcard address (involving ADDR_ANY), a WSAEADDRINUSE error could be delayed until the specific address is committed. This could happen with a call to another function later, including connect, listen, WSAConnect, or WSAJoinLeaf.\n" );
		break;
	case WSAEADDRNOTAVAIL:
		printf( "WSAEADDRNOTAVAIL\n" );
		printf( "Cannot assign requested address.\n" );
		printf( "The requested address is not valid in its context. This normally results from an attempt to bind to an address that is not valid for the local computer. This can also result from connect, sendto, WSAConnect, WSAJoinLeaf, or WSASendTo when the remote address or port is not valid for a remote computer (for example, address or port 0).\n" );
		break;
	case WSAENETDOWN:
		printf( "WSAENETDOWN\n" );
		printf( "Network is down.\n" );
		printf( "A socket operation encountered a dead network. This could indicate a serious failure of the network system (that is, the protocol stack that the Windows Sockets DLL runs over), the network interface, or the local network itself.\n" );
		break;
	case WSAENETUNREACH:
		printf( "WSAENETUNREACH\n" );
		printf( "Network is unreachable.\n" );
		printf( "A socket operation was attempted to an unreachable network. This usually means the local software knows no route to reach the remote host.\n" );
		break;
	case WSAENETRESET:
		printf( "WSAENETRESET\n" );
		printf( "Network dropped connection on reset.\n" );
		printf( "The connection has been broken due to keep-alive activity detecting a failure while the operation was in progress. It can also be returned by setsockopt if an attempt is made to set SO_KEEPALIVE on a connection that has already failed.\n" );
		break;
	case WSAECONNABORTED:
		printf( "WSAECONNABORTED\n" );
		printf( "Software caused connection abort.\n" );
		printf( "An established connection was aborted by the software in your host computer, possibly due to a data transmission time-out or protocol error.\n" );
		break;
	case WSAECONNRESET:
		printf( "WSAECONNRESET\n" );
		printf( "Connection reset by peer.\n" );
		printf( "An existing connection was forcibly closed by the remote host. This normally results if the peer application on the remote host is suddenly stopped, the host is rebooted, the host or remote network interface is disabled, or the remote host uses a hard close (see setsockopt for more information on the SO_LINGER option on the remote socket). This error may also result if a connection was broken due to keep-alive activity detecting a failure while one or more operations are in progress. Operations that were in progress fail with WSAENETRESET. Subsequent operations fail with WSAECONNRESET.\n" );
		break;
	case WSAENOBUFS:
		printf( "WSAENOBUFS\n" );
		printf( "No buffer space available.\n" );
		printf( "An operation on a socket could not be performed because the system lacked sufficient buffer space or because a queue was full.\n" );
		break;
	case WSAEISCONN:
		printf( "WSAEISCONN\n" );
		printf( "Socket is already connected.\n" );
		printf( "A connect request was made on an already-connected socket. Some implementations also return this error if sendto is called on a connected SOCK_DGRAM socket (for SOCK_STREAM sockets, the to parameter in sendto is ignored) although other implementations treat this as a legal occurrence.\n" );
		break;
	case WSAENOTCONN:
		printf( "WSAENOTCONN\n" );
		printf( "Socket is not connected.\n" );
		printf( "A request to send or receive data was disallowed because the socket is not connected and (when sending on a datagram socket using sendto) no address was supplied. Any other type of operation might also return this error—for example, setsockopt setting SO_KEEPALIVE if the connection has been reset.\n" );
		break;
	case WSAESHUTDOWN:
		printf( "WSAESHUTDOWN\n" );
		printf( "Cannot send after socket shutdown.\n" );
		printf( "A request to send or receive data was disallowed because the socket had already been shut down in that direction with a previous shutdown call. By calling shutdown a partial close of a socket is requested, which is a signal that sending or receiving, or both have been discontinued.\n" );
		break;
	case WSAETOOMANYREFS:
		printf( "WSAETOOMANYREFS\n" );
		printf( "Too many references.\n" );
		printf( "Too many references to some kernel object.\n" );
		break;
	case WSAETIMEDOUT:
		printf( "WSAETIMEDOUT\n" );
		printf( "Connection timed out.\n" );
		printf( "A connection attempt failed because the connected party did not properly respond after a period of time, or the established connection failed because the connected host has failed to respond.\n" );
		break;
	case WSAECONNREFUSED:
		printf( "WSAECONNREFUSED\n" );
		printf( "Connection refused.\n" );
		printf( "No connection could be made because the target computer actively refused it. This usually results from trying to connect to a service that is inactive on the foreign host—that is, one with no server application running.\n" );
		break;
	case WSAELOOP:
		printf( "WSAELOOP\n" );
		printf( "Cannot translate name.\n" );
		printf( "Cannot translate a name.\n" );
		break;
	case WSAENAMETOOLONG:
		printf( "WSAENAMETOOLONG\n" );
		printf( "Name too long.\n" );
		printf( "A name component or a name was too long.\n" );
		break;
	case WSAEHOSTDOWN:
		printf( "WSAEHOSTDOWN\n" );
		printf( "Host is down.\n" );
		printf( "A socket operation failed because the destination host is down. A socket operation encountered a dead host. Networking activity on the local host has not been initiated. These conditions are more likely to be indicated by the error WSAETIMEDOUT.\n" );
		break;
	case WSAEHOSTUNREACH:
		printf( "WSAEHOSTUNREACH\n" );
		printf( "No route to host.\n" );
		printf( "A socket operation was attempted to an unreachable host. See WSAENETUNREACH.\n" );
		break;
	case WSAENOTEMPTY:
		printf( "WSAENOTEMPTY\n" );
		printf( "Directory not empty.\n" );
		printf( "Cannot remove a directory that is not empty.\n" );
		break;
	case WSAEPROCLIM:
		printf( "WSAEPROCLIM\n" );
		printf( "Too many processes.\n" );
		printf( "A Windows Sockets implementation may have a limit on the number of applications that can use it simultaneously. WSAStartup may fail with this error if the limit has been reached.\n" );
		break;
	case WSAEUSERS:
		printf( "WSAEUSERS\n" );
		printf( "User quota exceeded.\n" );
		printf( "Ran out of user quota.\n" );
		break;
	case WSAEDQUOT:
		printf( "WSAEDQUOT\n" );
		printf( "Disk quota exceeded.\n" );
		printf( "Ran out of disk quota.\n" );
		break;
	case WSAESTALE:
		printf( "WSAESTALE\n" );
		printf( "Stale file handle reference.\n" );
		printf( "The file handle reference is no longer available.\n" );
		break;
	case WSAEREMOTE:
		printf( "WSAEREMOTE\n" );
		printf( "Item is remote.\n" );
		printf( "The item is not available locally.\n" );
		break;
	case WSASYSNOTREADY:
		printf( "WSASYSNOTREADY\n" );
		printf( "Network subsystem is unavailable.\n" );
		printf( "This error is returned by WSAStartup if the Windows Sockets implementation cannot function at this time because the underlying system it uses to provide network services is currently unavailable. Users should check:\n" );
		break;
	default:
		printf( "Unknown winsock error.....\n" );
			assert( false );
		break;
	}
}

