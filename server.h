#pragma once

#include <hash_map>

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/locale.hpp>
#include <boost/shared_ptr.hpp>

#include "session.h"

using boost::asio::ip::tcp;

class udp_server;
class server
{
public:
	std::hash_map<int, boost::shared_ptr<session> > session_map_;

private:

	boost::shared_ptr<udp_server> udp_server_;

	boost::shared_ptr<boost::asio::io_service> io_service_;
	boost::shared_ptr<tcp::acceptor> acceptor_;

	boost::shared_ptr<boost::asio::deadline_timer> monitor_timer_;
	boost::asio::ip::udp::endpoint load_balance_endpoint_;
	boost::asio::ip::tcp::endpoint bind_endpoint_;
	boost::shared_ptr<boost::asio::deadline_timer> server_reset_timer_;

public:
	server()
	{
	}

	const boost::asio::ip::tcp::endpoint & bind_endpoint() const {return bind_endpoint_;}

	
	void run();
	void stop()
	{
		io_service_->stop();
	}


private:
	void start_accept()
	{
		boost::shared_ptr<session> new_session(new session(*io_service_));

		acceptor_->async_accept(new_session->socket(),
			boost::bind(&server::handle_accept, this, new_session,
			boost::asio::placeholders::error));
	}

	void handle_accept(boost::shared_ptr<session> new_session,
		const boost::system::error_code& error)
	{
		if (!error)
		{
			new_session->start();
		}

		start_accept();
	}
};

