#include "StdAfx.h"
#include "Peer.h"


Peer::Peer(void)
{
}


Peer::~Peer(void)
{
}

void PeerMgr::InsertPeer( unsigned long long peer_id, boost::asio::ip::udp::endpoint internal_ip_addr, boost::asio::ip::udp::endpoint external_ip_addr )
{
	
	std::hash_map<unsigned long long, Peer>::iterator itr = peer_map_.find(peer_id);
	if(itr!=peer_map_.end())
	{
		itr->second.internal_ip_addr_ = internal_ip_addr;
		itr->second.external_ip_addr_ = external_ip_addr;
		itr->second.last_recv_time_ = time(nullptr);
	}
	else
	{
		Peer tmp;
		tmp.peer_id_ = peer_id;
		tmp.internal_ip_addr_ = internal_ip_addr;
		tmp.external_ip_addr_ = external_ip_addr;
		tmp.last_recv_time_ = time(nullptr);
		peer_map_.insert(std::make_pair(peer_id, tmp));
	}
}

Peer * PeerMgr::FindPeer( unsigned long long peer_id )
{
	std::hash_map<unsigned long long, Peer>::iterator itr = peer_map_.find(peer_id);
	if(itr!=peer_map_.end())
	{
		if(itr->second.last_recv_time_ + Peer::register_peer_timeout < time(nullptr) )
		{
			peer_map_.erase(itr);
			return nullptr;
		}

		return &(itr->second);
	}
	return nullptr;
}
