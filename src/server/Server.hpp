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
