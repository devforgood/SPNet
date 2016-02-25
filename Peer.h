#pragma once

#include <boost/asio.hpp>
#include <hash_map>

class Peer
{
public:
	enum 
	{ 
		register_peer_timeout = 180, // seconds
	};

	boost::asio::ip::udp::endpoint internal_ip_addr_;
	boost::asio::ip::udp::endpoint external_ip_addr_;
	unsigned long long peer_id_;
	time_t last_recv_time_;

public:
	Peer(void);
	~Peer(void);
};


class PeerMgr
{
public:
	std::hash_map<unsigned long long, Peer> peer_map_;

public:
	void InsertPeer(unsigned long long peer_id, boost::asio::ip::udp::endpoint internal_ip_addr, boost::asio::ip::udp::endpoint external_ip_addr);
	Peer * FindPeer(unsigned long long peer_id);
};
