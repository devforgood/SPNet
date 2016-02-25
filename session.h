#pragma once

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/exception/all.hpp>
#include <boost/enable_shared_from_this.hpp>

#include <queue>


#include "demux.h"
#include "dispatcher.default.h"


using boost::asio::ip::tcp;


class session : public boost::enable_shared_from_this<session>, public dispatcher
{
public:
	enum 
	{ 
		ping_period = 30, // seconds
	};

	session(boost::asio::io_service& io_service)
		: socket_(io_service)
	{
		object_id_ = 0;
		room_id_ = 0;
	}

	~session()
	{

		std::cout << __FUNCTION__ << "() " << std::endl;



	}

	tcp::socket& socket()
	{
		return socket_;
	}

	void start()
	{
		std::cout << __FUNCTION__ << "() "<< this->socket().remote_endpoint().address().to_string() << std::endl;

		//user_.session_ = this;
		ping_timer_.reset(new boost::asio::deadline_timer(socket_.get_io_service()));
		post_ping_timer();
		average_roundtrip_time_ = 0;

		connect();

		socket_.async_read_some(boost::asio::buffer(msg_.GetBuffer(), msg_.GetBufferSize()),
			boost::bind(&session::handle_read, shared_from_this(),
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
	}

	void do_write(boost::shared_ptr<NetOutgoingMessage> & msg)
	{
		bool write_in_progress = !write_msgs_.empty();
		write_msgs_.push_back(msg);
		if (!write_in_progress)
		{
			boost::asio::async_write(socket_,
				boost::asio::buffer(write_msgs_.front()->m_data,
				write_msgs_.front()->LengthBytes()),
				boost::bind(&session::handle_write, shared_from_this(),
				boost::asio::placeholders::error));
		}
	}

	template<class T>
	void Send(T & msg)
	{
		boost::shared_ptr<NetOutgoingMessage> send_msg(new NetOutgoingMessage());
		demux::make_message(msg, *send_msg);
		do_write(send_msg);
	}

private:
	void handle_read(const boost::system::error_code& error,
		size_t bytes_transferred)
	{
		if (!error && bytes_transferred!=0)
		{
			//boost::asio::async_write(socket_,
			//	boost::asio::buffer(msg_.GetBuffer(), bytes_transferred),
			//	boost::bind(&session::handle_write, shared_from_this(),
			//	boost::asio::placeholders::error));

			msg_.LengthBytes((int)(msg_.LengthBytes()+bytes_transferred));

			while(msg_.IsReadable() && msg_.PacketLength()<= (msg_.LengthBytes()-msg_.PositionInBytes()))
			{
				msg_.SkipPadBytes(NetBuffer::header_length);
				demux::demultiplexer(msg_, *this);

				if(msg_.CheckReadPosition()==false)
				{
					//LOG(INFO)<< __FUNCTION__ << "() CorrectReadPosition()";
					msg_.CorrectReadPosition();
				}
			}
			msg_.Compact();

			if(msg_.GetBufferSize() <=0)
			{
				std::cout << __FUNCTION__ << "() error" << std::endl;


			}

			socket_.async_read_some(boost::asio::buffer(msg_.GetBuffer(), msg_.GetBufferSize()),
				boost::bind(&session::handle_read, shared_from_this(),
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));
		}
		else
		{
			handle_disconnect(error);
		}
	}



	void handle_write(const boost::system::error_code& error)
	{
		if (!error)
		{

			write_msgs_.pop_front();
			if (!write_msgs_.empty())
			{
				boost::asio::async_write(socket_,
					boost::asio::buffer(write_msgs_.front()->m_data,
					write_msgs_.front()->LengthBytes()),
					boost::bind(&session::handle_write, shared_from_this(),
					boost::asio::placeholders::error));
			}
		}
		else
		{
			handle_disconnect(error);

		}
	}

	void handle_disconnect(const boost::system::error_code& error)
	{
		try
		{
			std::cout << __FUNCTION__ << "() disconnect "<< this->socket().remote_endpoint().address().to_string() << std::endl;

			if(ping_timer_.get())
			{
				ping_timer_->cancel();
			}

			disconnect();

		}
		catch ( boost::exception & e)
		{
			std::cout << diagnostic_information(e) << std::endl;
		}

	}

	tcp::socket socket_;

	NetIncomingMessage msg_;
	std::deque<boost::shared_ptr<NetOutgoingMessage> > write_msgs_;
	


	boost::shared_ptr<boost::asio::deadline_timer> ping_timer_;
	time_t last_send_ping_time_;
	__int64 average_roundtrip_time_;
	unsigned int object_id_;
	unsigned int room_id_;

public:

	__int64 average_roundtrip_time() const {return average_roundtrip_time_;}

	void post_ping_timer( void )
	{

		ping_timer_->expires_from_now(boost::posix_time::seconds(ping_period));
		ping_timer_->async_wait(boost::bind(&session::handle_ping_timer, shared_from_this(), boost::asio::placeholders::error));

	}

	void handle_ping_timer( const boost::system::error_code& error )
	{
		if(!error)
		{

			try
			{
				//network_message::ping send_msg;
				//user_.Send(send_msg);

				last_send_ping_time_ = time(nullptr);

				post_ping_timer();

				//user_.ReflushStamina(last_send_ping_time_);
			}
			catch ( boost::exception & e)
			{
				std::cout << diagnostic_information(e) << std::endl;
			}
			catch (std::exception& e)
			{
				std::cout << "Exception: " << e.what() << std::endl;
			}
		}
	}

	void response_ping( void )
	{
		__int64 rtt  = time(nullptr) - last_send_ping_time_;
		if(average_roundtrip_time_ == 0)
		{
			average_roundtrip_time_  = rtt;
		}
		else
		{
			average_roundtrip_time_  =  (__int64)(average_roundtrip_time_ * 0.7f) + (__int64)(rtt * 0.3f);
		}

		std::cout << __FUNCTION__ << "() rtt : " << rtt << " microseconds,  avg rtt: " << average_roundtrip_time_  <<" microseconds" << std::endl;
	}

	unsigned int object_id() { return object_id_;}
	unsigned int room_id() { return room_id_;}
	void set_room_id(unsigned int value) { room_id_ = value;}
	void connect();
	void disconnect();

	void relay(int msg_index, NetIncomingMessage & msg);

	using dispatcher::dispatch;

	void dispatch(boost::shared_ptr<network_message::login> & msg);
	void dispatch(boost::shared_ptr<network_message::logout> & msg);
	void dispatch(boost::shared_ptr<network_message::create_room> & msg);
	void dispatch(boost::shared_ptr<network_message::join_room> & msg);
};