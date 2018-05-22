#include <vector>
#include <string>


using namespace std;

class Server {
public:
	Server() : moviesNames_(), maxNumOfClients_(5) {}

private:
	void prepareToDistribution() {
	}

	void loadMovies() {
	}

	void startListeningForConns() {
	}

	vector<string> moviesNames_;
	unsigned short maxNumOfClients_;
};
