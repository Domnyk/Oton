#include "Movie.hpp"

Movie::Movie(const std::string& pathToMovie) :
    videoStream_(pathToMovie), audioStream_(pathToMovie) {}

string Movie::get_filename(const string & filepath) {
    size_t found = filepath.find_last_of("/\\");
    string filename = filepath.substr(found + 1);
    return filename;
}
