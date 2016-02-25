#include "stdafx.h"
#include "stun.h"


void udp_server::dispatch( NetIncomingMessage & msg, boost::asio::ip::udp::endpoint & recv_endpoint )
{
	// decode header
	//  8 bits - NetMessageType
	//  1 bit  - Fragment?
	// 15 bits - Sequence number
	// 16 bits - Payload length in bits

	msg.SkipPadBytes(5);

	switch(msg.ReadByte())
	{
	case 0: // RegisterHost
		{
			unsigned long long id = msg.ReadInt64(); // server unique identifier
			boost::asio::ip::udp::endpoint internal_ip_addr = msg.ReadIPEndPoint<boost::asio::ip::udp::endpoint, boost::asio::ip::address_v4 >();
			//IPEndPoint external_ip_addr = msg.SenderEndPoint;

			GlobalVariable::getInstance().peer_mgr_.InsertPeer(id, internal_ip_addr, recv_endpoint);

			std::cout << "RegisterHost id:"<< id << ", inter ip: " << internal_ip_addr.address().to_string()
				<< ", port:" << internal_ip_addr.port()<< ", exter ip: " << recv_endpoint.address().to_string() 
				<< ", port:" << recv_endpoint.port() << std::endl;

		}
		break;
	case 3: // RegisterRelayHost
		{
			unsigned long long id = msg.ReadInt64(); // server unique identifier
			boost::asio::ip::udp::endpoint internal_ip_addr = msg.ReadIPEndPoint<boost::asio::ip::udp::endpoint, boost::asio::ip::address_v4 >();
			boost::asio::ip::udp::endpoint external_ip_addr = msg.ReadIPEndPoint<boost::asio::ip::udp::endpoint, boost::asio::ip::address_v4 >();
			//IPEndPoint external_ip_addr = msg.SenderEndPoint;

			GlobalVariable::getInstance().peer_mgr_.InsertPeer(id, internal_ip_addr, external_ip_addr);

			std::cout << "Relay RegisterHost id:"<< id << ", inter ip: " << internal_ip_addr.address().to_string() 
				<< ", port:" << internal_ip_addr.port()<< ", exter ip: " << external_ip_addr.address().to_string() 
				<< ", port:" << external_ip_addr.port() << std::endl;

		}
		break;
	case 1: // RequestHostList
		break;
	case 2: // RequestIntroduction
		{
			// p2p off
			//return;

			boost::asio::ip::udp::endpoint internal_ip_addr = msg.ReadIPEndPoint<boost::asio::ip::udp::endpoint, boost::asio::ip::address_v4 >();
			unsigned long long host_id = msg.ReadInt64();
			std::wstring token = msg.ReadString();

			Peer * host_peer = GlobalVariable::getInstance().peer_mgr_.FindPeer(host_id);
			if(host_peer == nullptr)
			{
				std::cout << "RequestIntroduction find error : " << host_id << std::endl;
				return;
			}

			std::cout << "RequestIntroduction client id:"<< 0 << ", inter ip: " << internal_ip_addr.address().to_string() 
				<< ", port:" << internal_ip_addr.port()<< ", exter ip: " << recv_endpoint.address().to_string() 
				<< ", port:" << recv_endpoint.port() << std::endl;
			std::cout << "RequestIntroduction host id:"<< host_id << ", inter ip: " << host_peer->internal_ip_addr_.address().to_string() 
				<< ", port:" << host_peer->internal_ip_addr_.port()<< ", exter ip: " << host_peer->external_ip_addr_.address().to_string() 
				<< ", port:" << host_peer->external_ip_addr_.port() << std::endl;
	

			{

				// send message to client
				boost::shared_ptr<NetOutgoingMessage> msg(new NetOutgoingMessage);
				msg->Write((byte) 139); // NetMessageType.NatIntroduction
				msg->Write((unsigned int) 0);

				msg->Write((byte)0);
				msg->Write(host_peer->internal_ip_addr_);
				msg->Write(host_peer->external_ip_addr_);
				msg->Write(token);
				GlobalVariable::getInstance().udp_server_->send_to(msg, recv_endpoint);

			}
			{
				// send message to host
				boost::shared_ptr<NetOutgoingMessage> msg(new NetOutgoingMessage);
				msg->Write((byte) 139); // NetMessageType.NatIntroduction
				msg->Write((unsigned int) 0);

				msg->Write((byte)1);
				msg->Write(internal_ip_addr);
				msg->Write(recv_endpoint);
				msg->Write(token);
				GlobalVariable::getInstance().udp_server_->send_to(msg, host_peer->external_ip_addr_);
			}

		}
		break;
	}
}
