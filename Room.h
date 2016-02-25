#pragma once

class session;
class Room
{
private:
	unsigned int object_id_;
	std::vector<boost::shared_ptr<session> > sessions_;

public:
	Room(unsigned int object_id = 0)
		: object_id_(object_id)
	{
	}
	~Room(void)
	{
	}

	unsigned int object_id() {return object_id_;}

	bool enter(boost::shared_ptr<session> & target);
	bool leave(boost::shared_ptr<session> & target);

	bool is_empty() const {return sessions_.empty();}
};


class RoomMgr 
{
private:
	std::map<unsigned int, Room> rooms_;

public:
	Room * find_room(unsigned int object_id);

	bool dispatch(boost::shared_ptr<network_message::create_room> & msg, boost::shared_ptr<session> & target);
	bool dispatch(boost::shared_ptr<network_message::join_room> & msg, boost::shared_ptr<session> & target);

	bool leave(boost::shared_ptr<session> & target);

};

