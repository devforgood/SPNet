#include "StdAfx.h"
#include "Room.h"
#include "session.h"

bool Room::enter(boost::shared_ptr<session> & target)
{
	for(size_t i=0;i<sessions_.size();++i)
	{
		network_message::room_in_user send_msg;
		send_msg.object_id_ = sessions_[i]->object_id();
		target->Send(send_msg);

		send_msg.object_id_ = target->object_id();
		sessions_[i]->Send(send_msg);
	}

	sessions_.push_back(target);
	target->set_room_id(object_id_);

	return true;
}
bool Room::leave(boost::shared_ptr<session> & target)
{
	for(std::vector<boost::shared_ptr<session> >::iterator itr = sessions_.begin();itr!=sessions_.end();++itr)
	{
		if( (*itr) == target)
		{
			sessions_.erase(itr);
			return true;
		}
	}
	return false;
}

Room * RoomMgr::find_room(unsigned int object_id)
{
	std::map<unsigned int, Room>::iterator itr = rooms_.find(object_id);
	if(itr!=rooms_.end())
	{
		return &(itr->second);
	}
	return nullptr;
}
bool RoomMgr::leave(boost::shared_ptr<session> & target)
{
	std::map<unsigned int, Room>::iterator itr = rooms_.find(target->room_id());
	if(itr==rooms_.end())
		return false;

	if(itr->second.leave(target)==false)
		return false;

	if(itr->second.is_empty()==true)
	{
		rooms_.erase(itr);
	}

	return true;
}

bool RoomMgr::dispatch(boost::shared_ptr<network_message::create_room> & msg, boost::shared_ptr<session> & target)
{
	Room tmp(GlobalVariable::getInstance().object_id_pool_.getUid());

	std::pair<std::map<unsigned int, Room>::iterator, bool> ret = rooms_.insert(std::make_pair(tmp.object_id(), tmp));
	if(ret.second==false)
	{
		GlobalVariable::getInstance().object_id_pool_.freeUid(tmp.object_id());
		return false;
	}

	if(ret.first->second.enter(target)==false)
	{
		GlobalVariable::getInstance().object_id_pool_.freeUid(tmp.object_id());
		return false;
	}

	network_message::result_create_room send_msg;
	send_msg.room_id_ = tmp.object_id();
	target->Send(send_msg);
	return true;
}

bool RoomMgr::dispatch(boost::shared_ptr<network_message::join_room> & msg, boost::shared_ptr<session> & target)
{
	std::map<unsigned int, Room>::iterator itr = rooms_.begin();
	if(itr==rooms_.end())
	{
		return false;
	}

	if(itr->second.enter(target)==false)
	{
		return false;
	}

	network_message::result_join_room send_msg;
	target->Send(send_msg);
	return true;
}
