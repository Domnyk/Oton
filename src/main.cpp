#include <iostream>
#include <boost/asio.hpp>

#include "tcp_server.hpp"
#include "tcp_connection.hpp"

using namespace std;

int main() {
	try {
    	boost::asio::io_context io_context;
    	tcp_server server(io_context);
    	io_context.run();
  	} catch (std::exception& e) {
    	std::cerr << e.what() << std::endl;
  	}

  	return 0;
}