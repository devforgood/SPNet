#pragma once

#include "Peer.h"
#include "UidGenerator.h"

class RoomMgr;
class udp_server;
class server;
class GlobalVariable
{
public:
	PeerMgr peer_mgr_;
	udp_server * udp_server_;
	server * server_;
	RoomMgr * room_mgr_;
	UidGenerator<unsigned short> object_id_pool_;

private:
	GlobalVariable();
	~GlobalVariable();
	
	GlobalVariable(const GlobalVariable& other);
	static GlobalVariable inst;

public:

	static GlobalVariable& getInstance() { return inst; }
};
