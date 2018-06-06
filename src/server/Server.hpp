#include <vector>
#include <string>
#include "network_layer.hpp"
#include "movie_layer.hpp"

using namespace std;

class Server {
public:
    Server();
    Server(movie_layer*);

    unique_ptr<network_layer>& get_network_layer() {
        return network_layer_;
    }

    /*
     *  I could return another shared_ptr but maybe returning reference saves a little memory or CPU time ?
     */
    unique_ptr<movie_layer>& get_movie_layer() {
        return movie_layer_;
    }

    // Convienence method
    unsigned short get_udp_port() const {
        return network_layer_->get_udp_port();
    }

    // Convienence method
    unsigned short get_tcp_port() const {
        return network_layer_->get_tcp_port();
    }

    void init_network_layer(const unsigned short);
private:
    unique_ptr<network_layer> network_layer_;
    unique_ptr<movie_layer> movie_layer_;
};
