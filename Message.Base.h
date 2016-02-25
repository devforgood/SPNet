// Generated by the PacketGenerator compiler.  DO NOT EDIT!
#pragma once

#include <string>

namespace network_message
{
	// 에러 메시지
	enum enum_error_code
	{
		kErrorSuccess = 0,
		kErrorAlreadyLogin = 1,
	};


	enum enum_room_state
	{
		kWait = 0,
		kLoad = 1,
		kPlay = 2,
	};

	class logout
	{
	public:
		int reason_;
	};

	class room_info
	{
	public:
		int room_index_;
		int user_count_;
		int max_user_count_;
		enum_room_state state_;
	};

	enum enum_player_team
	{
		kPlayerTeamBlue = 0,
		kPlayerTeamRed = 1,
	};

	class room_in_user
	{
	public:
		__int64 avg_rtt_;
		int object_id_;
		int character_level_;
		int relay_object_id_;
		enum_player_team team_;
	};

	class ping
	{
	};


	class noti_leave_room
	{
	public:
		int object_id_;
	};

	class change_room_info
	{
	public:
		int room_index_;
		int user_count_;
		int max_user_count_;
		enum_room_state state_;
		int master_user_object_id_;
	};


	class noti_intrude
	{
	public:
		int object_id_;
		int enter_sequence_;
	};

	class connect_to_server
	{
	};

	class disconnect_to_server
	{
	};


	// use nGrinder
	class login
	{
	public:
		std::string account_;
		std::string ssh_key_;
		__int64 session_key_;

		__int64 member_no_;
		std::string member_id_;
		std::string profile_img_url_;
		std::string nickname_;
		int account_type_;
		std::string game_access_token_;
	};

	class result_login
	{
	public:
		int object_id_;
		enum_error_code error_code_;
		bool need_representative_name_;
		bool is_gm_;
		int gold_;
		int ruby_;
		int selected_character_slot_no_;
		std::string representative_name_;
		int klein_;
		int skill_point_;
		__int64 next_reset_daily_time_;
	};

    class create_room
    {
    public:
        __int64 session_key_;
        std::string room_name_;
        int player_count_;
        bool infinite_;
        int dungeon_index_;
        int fighting_power_;
    };

    class result_create_room
    {
    public:
        int room_id_;
        int master_user_object_id_;
        int enter_sequence_;
        room_in_user room_in_user_;
    };

    class join_room
    {
    public:
        __int64 session_key_;
        int room_id_;
        int fighting_power_;
    };

    class result_join_room
    {
    public:
        int master_user_object_id_;
        enum_room_state state_;
        int enter_sequence_;
        room_in_user room_in_user_;
    };


    class leave_room
    {
    };

    class result_leave_room
    {
    };

    // @ExceptedMessage
    class vector3
    {
    public:
        float x_;
        float y_;
        float z_;
    };

    // @ExceptedMessage
    class vector4
    {
    public:
        float x_;
        float y_;
        float z_;
        float w_;
    };

}