#include "Movie.hpp"

Movie::Movie(const std::string& pathToMovie) :
    videoStream_(pathToMovie), audioStream_(pathToMovie) {}
