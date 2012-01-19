#pragma once

#include <string>
#include "RakPeerInterface.h"
using namespace std;

class OnlinePlayer;
class GUI;

//! Handles all the RakNet things.
class Peer
{
public:
	Peer();
	~Peer();

	void update(double dt);

	bool listenForPackets();
	bool handlePacket(RakNet::Packet *packet);

	bool startServer();
	bool connectToServer(string ip);

	RakNet::RakPeerInterface*	getPeer();
	void setPlayer(OnlinePlayer* player);
	void setGui(GUI* gui);
private:
	RakNet::RakPeerInterface*	mPeer;
	OnlinePlayer*				mPlayer;
	GUI*						mGui;
	bool mRunsServer;
};