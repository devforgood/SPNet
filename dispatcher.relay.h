// Generated by the PacketGenerator compiler.  DO NOT EDIT!
#pragma once;

#include <boost/shared_ptr.hpp>


class dispatcher
{
public:

	void dispatch(User * target, boost::shared_ptr<network_message::logout> & msg) {target->RelayMsg(msg);}  
	void dispatch(User * target, boost::shared_ptr<network_message::room_info> & msg) {target->RelayMsg(msg);}  
	void dispatch(User * target, boost::shared_ptr<network_message::room_in_user> & msg) {target->RelayMsg(msg);}  
	void dispatch(User * target, boost::shared_ptr<network_message::ping> & msg) {target->RelayMsg(msg);}  
	void dispatch(User * target, boost::shared_ptr<network_message::noti_leave_room> & msg) {target->RelayMsg(msg);}  
	void dispatch(User * target, boost::shared_ptr<network_message::change_room_info> & msg) {target->RelayMsg(msg);}  
	void dispatch(User * target, boost::shared_ptr<network_message::noti_intrude> & msg) {target->RelayMsg(msg);}  
	void dispatch(User * target, boost::shared_ptr<network_message::connect_to_server> & msg) {target->RelayMsg(msg);}  
	void dispatch(User * target, boost::shared_ptr<network_message::disconnect_to_server> & msg) {target->RelayMsg(msg);}  
	void dispatch(User * target, boost::shared_ptr<network_message::login> & msg) {target->RelayMsg(msg);}  
	void dispatch(User * target, boost::shared_ptr<network_message::result_login> & msg) {target->RelayMsg(msg);}  
	void dispatch(User * target, boost::shared_ptr<network_message::create_room> & msg) {target->RelayMsg(msg);}  
	void dispatch(User * target, boost::shared_ptr<network_message::result_create_room> & msg) {target->RelayMsg(msg);}  
	void dispatch(User * target, boost::shared_ptr<network_message::join_room> & msg) {target->RelayMsg(msg);}  
	void dispatch(User * target, boost::shared_ptr<network_message::result_join_room> & msg) {target->RelayMsg(msg);}  
	void dispatch(User * target, boost::shared_ptr<network_message::leave_room> & msg) {target->RelayMsg(msg);}  
	void dispatch(User * target, boost::shared_ptr<network_message::result_leave_room> & msg) {target->RelayMsg(msg);}  
	void dispatch(User * target, boost::shared_ptr<network_message::fire_bullet> & msg) {target->RelayMsg(msg);}  
	void dispatch(User * target, boost::shared_ptr<network_message::move_object> & msg) {target->RelayMsg(msg);}  

};