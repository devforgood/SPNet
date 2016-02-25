#pragma once

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/exception/all.hpp>
#include <boost/enable_shared_from_this.hpp>

#include <deque>


class udp_server 
{
public:
	struct send_msg
	{
		boost::shared_ptr<NetOutgoingMessage> msg_;
		boost::asio::ip::udp::endpoint sender_endpoint_;

		send_msg(boost::shared_ptr<NetOutgoingMessage> & msg, boost::asio::ip::udp::endpoint & sender_endpoint)
		{
			msg_ = msg;
			sender_endpoint_ = sender_endpoint;
		}
	};

	udp_server(boost::shared_ptr<boost::asio::io_service> & io_service, short port)
		: io_service_(io_service),
		socket_(*io_service, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port))
	{

		socket_.async_receive_from(
			boost::asio::buffer(msg_.GetBuffer(), msg_.GetBufferSize()), 
			recv_endpoint_,
			boost::bind(&udp_server::handle_receive_from, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
	}

	void handle_receive_from(const boost::system::error_code& error, size_t bytes_recvd)
	{
		dispatch(msg_, recv_endpoint_);

		msg_.Clear();
		socket_.async_receive_from(
			boost::asio::buffer(msg_.GetBuffer(), msg_.GetBufferSize()), 
			recv_endpoint_,
			boost::bind(&udp_server::handle_receive_from, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
	}

	void handle_send_to(const boost::system::error_code& error,	size_t bytes_sent)
	{
		write_msgs_.pop_front();
		if (!write_msgs_.empty())
		{
			socket_.async_send_to(
				boost::asio::buffer(write_msgs_.front().msg_->m_data, write_msgs_.front().msg_->LengthBytes()), 
				write_msgs_.front().sender_endpoint_,
				boost::bind(&udp_server::handle_send_to, this,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));
		}
	}

	void send_to(boost::shared_ptr<NetOutgoingMessage> & msg, boost::asio::ip::udp::endpoint & sender_endpoint)
	{
		bool write_in_progress = !write_msgs_.empty();
		write_msgs_.push_back(send_msg(msg, sender_endpoint));
		if (!write_in_progress)
		{
			socket_.async_send_to(
				boost::asio::buffer(write_msgs_.front().msg_->m_data, write_msgs_.front().msg_->LengthBytes()), 
				sender_endpoint,
				boost::bind(&udp_server::handle_send_to, this,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));

		}
	}

private:
	boost::asio::ip::udp::endpoint recv_endpoint_;
	boost::shared_ptr<boost::asio::io_service> io_service_;
	boost::asio::ip::udp::socket socket_;
	NetIncomingMessage msg_;
	std::deque<send_msg> write_msgs_;


public:
	void dispatch(NetIncomingMessage & msg, boost::asio::ip::udp::endpoint & recv_endpoint);


};