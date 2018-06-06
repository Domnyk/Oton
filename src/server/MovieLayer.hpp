#ifndef MOVIE_LAYER_HPP
#define MOVIE_LAYER_HPP

#include <map>
#include <string>
#include "Movie.hpp"

using namespace std;

/*
 *  There can't be two films with the same filename but in diffrent locations
 *  See method "is_movie_loaded"
 */
class MovieLayer
{
public:
    /*
     * Key - std::string - movie location
     * Value - std::string - movie file name
     */
    typedef map<string, string> movies_location;

    MovieLayer();

    std::pair<movies_location::iterator, bool> add_movie(const string&);
    void delete_movie(const string&);
    std::string get_movie_location(const string&) const;
    static std::string get_movie_filename(const string&);

    std::string friend get_movie_list(const MovieLayer&);

private:
    bool is_movie_loaded(const string&) const;

    movies_location movies_location_;
};

#endif
