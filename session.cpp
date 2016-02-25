#include "stdafx.h"
#include "session.h"
#include "GlobalVariable.h"
#include "Room.h"
#include "server.h"

void session::connect()
{
	object_id_ = GlobalVariable::getInstance().object_id_pool_.getUid();
	GlobalVariable::getInstance().server_->session_map_.insert(std::make_pair(object_id_, shared_from_this()));
}
void session::disconnect()
{
	if(object_id_!=0)
	{
		GlobalVariable::getInstance().server_->session_map_.erase(object_id_);

		GlobalVariable::getInstance().room_mgr_->leave(shared_from_this());

		GlobalVariable::getInstance().object_id_pool_.freeUid(object_id_);
	}
}

void session::relay( int msg_index, NetIncomingMessage & msg )
{
	int from_object_id = msg.ReadInt32();
	int to_object_id = msg.ReadInt32();
	msg.SkipPadBytes(msg.m_lastPacketLength - (+NetBuffer::packet_index_length+8));

	std::hash_map<int, boost::shared_ptr<session> >::iterator itr = GlobalVariable::getInstance().server_->session_map_.find(to_object_id);
	if(itr!=GlobalVariable::getInstance().server_->session_map_.end())
	{
		boost::shared_ptr<NetOutgoingMessage> send_msg(new NetOutgoingMessage());
		send_msg->Write(msg.m_lastPacket, msg.m_lastPacketLength+NetBuffer::header_length);
		itr->second->do_write(send_msg);
	}

}

void session::dispatch(boost::shared_ptr<network_message::login> & msg)
{
	network_message::result_login send_msg;
	send_msg.object_id_ = object_id_;
	send_msg.error_code_ = network_message::kErrorSuccess;
	Send(send_msg);
}

void session::dispatch(boost::shared_ptr<network_message::logout> & msg)
{
}

void session::dispatch(boost::shared_ptr<network_message::create_room> & msg)
{
	GlobalVariable::getInstance().room_mgr_->dispatch(msg, shared_from_this());
}

void session::dispatch(boost::shared_ptr<network_message::join_room> & msg)
{
	GlobalVariable::getInstance().room_mgr_->dispatch(msg, shared_from_this());
}


