#include <vector>
#include <iostream>

#include "BitStream.h"
#include "MessageIdentifiers.h"
#include "RakNetTypes.h"
#include "RakPeerInterface.h"

#include "Thread.h"

#define SERVER_PORT 60005
#define CLIENT_PORT 60006
#define MAX_CLIENTS 10

enum MyMessageIdentifiers
{
	ID_CHAT = ID_USER_PACKET_ENUM + 1
};

std::vector<RakNet::SystemAddress> clientAddresses;
RakNet::RakPeerInterface *peer = NULL;
RakNet::SystemAddress serverAddress;

void inputMessage(void* data)
{
	while (true)
	{
		char message[256];
		std::cin >> message;

		RakNet::BitStream stream;
		stream.Write((RakNet::MessageID) ID_CHAT);
		stream.Write(message);

		RakNet::SystemAddress connections[MAX_CLIENTS];
		unsigned short connectionCount = MAX_CLIENTS;
		peer->GetConnectionList(connections, &connectionCount);
		for (unsigned short index = 0; index < connectionCount; index++)
		{
			peer->Send(&stream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, connections[index], false);
		}
	}
}

int main(int argc, char** argv)
{
	peer = RakNet::RakPeerInterface::GetInstance();

	std::cout << "(C)lient or (S)erver? ";
	char clientServer;
	std::cin >> clientServer;
	bool isServer = clientServer == 's' || clientServer == 'S';

	if (isServer)
	{	
		std::cout << "Starting the server!" << std::endl;
		RakNet::SocketDescriptor sd(SERVER_PORT, 0);
		peer->Startup(MAX_CLIENTS, &sd, 1);
		// We need to let the server accept incoming connections from the clients
		peer->SetMaximumIncomingConnections(MAX_CLIENTS);
	}
	else
	{
		std::cout << "Starting the client!" << std::endl;
		RakNet::SocketDescriptor sd;
		peer->Startup(1, &sd, 1);
		peer->Connect("localhost", SERVER_PORT, 0);
	}

	TTask inputMessageTask;
	inputMessageTask.data = NULL;
	inputMessageTask.runnable = inputMessage;
	CThread inputMessageThread(inputMessageTask);
	inputMessageThread.Start();

	while (true)
	{
		RakNet::Packet* packet = peer->Receive();
		if (packet == NULL)
		{
			continue;
		}

		if (packet->data[0] == ID_REMOTE_DISCONNECTION_NOTIFICATION)
		{
			std::cout << "Another client has disconnected." << std::endl;
		}
		else if (packet->data[0] == ID_REMOTE_CONNECTION_LOST)
		{
			std::cout << "Another client has lost the connection." << std::endl;
		}
		else if (packet->data[0] == ID_REMOTE_NEW_INCOMING_CONNECTION)
		{
			std::cout << "Another client has connected." << std::endl;
		}
		else if (packet->data[0] == ID_CONNECTION_REQUEST_ACCEPTED)
		{
			std::cout << "Our connection request has been accepted." << std::endl;

			serverAddress = packet->systemAddress;
		}
		else if (packet->data[0] == ID_NEW_INCOMING_CONNECTION)
		{
			std::cout << "A connection is incoming." << std::endl;
		}
		else if (packet->data[0] == ID_NO_FREE_INCOMING_CONNECTIONS)
		{
			std::cout << "The server is full." << std::endl;
		}
		else if (packet->data[0] == ID_DISCONNECTION_NOTIFICATION)
		{
			if (isServer)
			{
				std::cout << "A client has disconnected." << std::endl;
			}
			else
			{
				std::cout << "We have been disconnected." << std::endl;
			}
		}
		else if (packet->data[0] == ID_CONNECTION_LOST)
		{
			if (isServer)
			{
				std::cout << "A client lost the connection." << std::endl;
			}
			else
			{
				std::cout << "Connection lost." << std::endl;
			}
		}
		else if (packet->data[0] == ID_CHAT)
		{
			RakNet::BitStream stream(packet->data,packet->length,false);
			stream.IgnoreBytes(sizeof(RakNet::MessageID));

			RakNet::RakString message;
			stream.Read(message);

			std::cout << message << std::endl;
		}
		else
		{
			std::cout << "Message with identifier " << packet->data[0] << " has arrived." << std::endl;
		}

		peer->DeallocatePacket(packet);
	}

	peer->DestroyInstance(peer);

	return 0;
}
