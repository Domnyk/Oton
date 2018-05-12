#define BOOST_LOG_DYN_LINK

#include <iostream>
#include <boost/asio.hpp>
#include <boost/log/trivial.hpp>
#include "network/tcp_server.hpp"
#include "network/tcp_connection.hpp"
#include "movie/VideoStream.hpp"

using namespace cv;
using namespace std;

int main() {
	VideoStream vs("./data/Megamind.avi");
    vs.getFrame(0);
	/* 
	try {
    	boost::asio::io_context io_context;
    	tcp_server server(io_context);

    	BOOST_LOG_TRIVIAL(info) << "Server is running on port: " << server.get_port();

    	io_context.run();
  	} catch (std::exception& e) {
    	std::cerr << e.what() << std::endl;
  	}
  	*/

  	return 0;
}