#ifndef MOVIE_LAYER_HPP
#define MOVIE_LAYER_HPP

#include <list>
#include <string>
#include "Movie.hpp"

using namespace std;

/*
 *  There can't be two films with the same filename but in diffrent locations
 *  See method "is_movie_loaded"
 */
class MovieList
{
public:
    MovieList();

    bool add_movie(const string&);
    void delete_movie(const string&);
    unsigned get_num_of_movies() const;

    std::string get_movie_location(const string&) const;
    string friend encode_movie_list_as_string(const MovieList&);
private:
    list<string>::const_iterator find_movie_location(const string&) const;

    list<string> movies_filepaths_;
};

#endif
