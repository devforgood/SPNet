#include "stdafx.h"
#include "server.h"

#include <boost/property_tree/ptree.hpp> 
#include <boost/property_tree/ini_parser.hpp> 
#include "stun.h"


void server::run()
{
	io_service_.reset(new boost::asio::io_service());
	udp_server_.reset(new udp_server(io_service_, 14343));
	GlobalVariable::getInstance().udp_server_ = udp_server_.get();
	GlobalVariable::getInstance().server_ = this;

	acceptor_.reset(new tcp::acceptor(*io_service_, tcp::endpoint(tcp::v4(), 11000)));
	start_accept();

	io_service_->run();
}

