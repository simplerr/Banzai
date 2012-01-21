#include "BitStream.h"
#include "MessageIdentifiers.h"
#include "Peer.h"
#include "Enums.h"
#include "Database.h"
#include "OnlinePlayer.h"
#include "PlayingOnline.h"
#include "GUI.h"
#include "Chat.h"
#include "Sound.h"

//! Constructor.
Peer::Peer()
{
	// Create the RakNet peer
	mPeer = RakNet::RakPeerInterface::GetInstance();
	mPlayer = NULL;
}
	
//! Destructor.
Peer::~Peer()
{
	// Tell the opponent that you left
	RakNet::BitStream bitstream;
	bitstream.Write((unsigned char)ID_LEFT_GAME);
	mPeer->Send(&bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);

	// Remove the server from the server list
	if(mRunsServer)	{
		gDatabase->removeServer(mPlayer->getName());
		mPeer->Shutdown(300);
	}

	RakNet::RakPeerInterface::DestroyInstance(mPeer);
}

//! Listen for packets
/**
@param dt Time since last frame.
@note dt is not needed.
*/
void Peer::update(double dt)
{
	// Listen for packets, returns true if the state should change
	if(listenForPackets())
		PlayingOnline::Instance()->stateChange();
}

//! Listen and handles packets.
/**
@return True if the state changed.
*/
bool Peer::listenForPackets()
{
	bool changedState = false;
	RakNet::Packet *packet = NULL;
	packet = mPeer->Receive();

	if(packet != NULL)	{
		changedState = handlePacket(packet);
		if(!changedState)
			mPeer->DeallocatePacket(packet);
	}

	return changedState;
}

//! Handles packets.
/**
@return True if the state changed.
*/
bool Peer::handlePacket(RakNet::Packet *packet)
{
	// Receive the packet
	RakNet::BitStream bitstream((unsigned char*)packet->data, packet->length, false);

	unsigned char packetID;
	char message[256];

	// Read the ID
	bitstream.Read(packetID);

	// Switch the ID
	switch(packetID)
	{
	case ID_CONNECTION_LOST:
		mGui->displayOpponentLeft();
		PlayingOnline::Instance()->stateChange();
		break;
	case ID_NEW_INCOMING_CONNECTION:	{
		// Send the name to the opponent and request his.
		RakNet::BitStream bitstream;
		bitstream.Write((unsigned char)ID_REQUEST_NAME);
		bitstream.Write(mPlayer->getName().c_str());
		mPeer->Send(&bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
		mPlayer->setWaitingForOpponent(false);
		mGui->setStatus("Your turn!", GREEN, 300.0f);
		break;
	}
	case ID_PIECE_MOVED:	{
		// ID - piece.x - piece.y - new.x - new.y
		Position from, to;
		bitstream.Read(from.x);
		bitstream.Read(from.y);
		bitstream.Read(to.x);
		bitstream.Read(to.y);

		// The opponent moved, do what's neccessary
		mPlayer->opponentMoved(from, to);
		break;
	}
	case ID_REQUEST_REMATCH:
		if(!mPlayer->waitingOnAnswer())
			mGui->displayRematchRequest();
		else
			mPlayer->restartMatch();
		break;
	case ID_DECLINE_REMATCH:
		gSound->playEffect(OPPONENT_LEAVE_SOUND);
		mGui->displayRematchDecline();
		PlayingOnline::Instance()->stateChange();
		break;
	case ID_ACCEPT_REMATCH:	{
		mPlayer->setWaitingOnAnswer(false);
		// Start new game.
		mPlayer->restartMatch();
		gSound->playEffect(NEW_GAME_SOUND);
		break;
	}
	case ID_LEFT_GAME:
		gSound->playEffect(OPPONENT_LEAVE_SOUND);
		mGui->displayOpponentLeft();
		PlayingOnline::Instance()->stateChange();
		return true;
		break;
	case ID_MESSAGE_SENT:	{
		// Let the chat handle this message
		char from[32];
		char message[256];
		bitstream.Read(from);
		bitstream.Read(message);
		mGui->getChat()->addMessage(string(from), string(message));
		break;
	}
	case ID_REQUEST_NAME:	{
		char name[256];
		bitstream.Read(name);
		mPlayer->setOpponent(string(name));
		mGui->initStats();

		// Only send back your name if you aren't the host - to avoid inf. loop
		if(!mRunsServer)	{
			RakNet::BitStream bitstream;
			bitstream.Write((unsigned char)ID_REQUEST_NAME);
			bitstream.Write(mPlayer->getName().c_str());
			mPeer->Send(&bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
		}

		// Sound
		gSound->playEffect(NEW_GAME_SOUND);

		break;
	}
	case ID_SENT_APPLAUD	:	{
		// TODO: Play a sound
		gSound->playEffect(APPLAUD_SOUND);
	}
	}

	return false;
}

//! Start the server.
/**
@return True if the server started.
*/
bool Peer::startServer()
{
	if(mPeer->Startup(10, &RakNet::SocketDescriptor(27020, 0), 1) == RakNet::RAKNET_STARTED)	{
		mPeer->SetMaximumIncomingConnections(10);
		mRunsServer = true;
		return true;
	}
	else
		return false;
}

//! Connect to a server.
/**
@param ip The ip, for example "96.6.12.155".
@return True if connected.
*/
bool Peer::connectToServer(string ip)
{
	// TODO:
	//ip = "127.0.0.1";		//85.8.19.166
	//ip = "85.8.19.166";
	//ip = "192.168.0.52";

	if(mPeer->Startup(1, &RakNet::SocketDescriptor(), 1) == RakNet::RAKNET_STARTED)		{
		if(mPeer->Connect(ip.c_str(), 27020, NULL, NULL) == RakNet::CONNECTION_ATTEMPT_STARTED)		{
			mRunsServer = false;
			return true;
		}
	}

	return false;
}

//! Return the RakNet peer.
RakNet::RakPeerInterface* Peer::getPeer()
{
	return mPeer;
}

//! Set the player.
void Peer::setPlayer(OnlinePlayer* player)
{
	mPlayer = player;
}

//! Set the gui.
void Peer::setGui(GUI* gui)
{
	mGui = gui;
}
