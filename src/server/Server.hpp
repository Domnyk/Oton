#include <vector>
#include <string>
#include "network_layer.hpp"
#include "movie_layer.hpp"

using namespace std;

class Server {
public:
    Server();
    Server(const std::string&);

    network_layer& get_network_layer() {
        return network_layer_;
    }

    // Convienence method
    unsigned short get_udp_port() const {
        return network_layer_.get_udp_server().get_port();
    }

    // Convienence method
    unsigned short get_tcp_port() const {
        return network_layer_.get_tcp_server().get_port();
    }
private:
	void prepareToDistribution() {
	}

	void loadMovies() {
	}

	void startListeningForConns() {
	}

    network_layer network_layer_;
    movie_layer movie_layer_;
};
