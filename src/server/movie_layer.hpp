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
class movie_layer
{
public:
    typedef map<string, Movie> movies;

    movie_layer();

    void add_new_movie(const string&);
    void delete_movie(const string&);
    Movie& get_movie(const string&);

private:
    bool is_movie_loaded(const string&);

    movies movies_;
};

#endif // MOVIE_LAYER_HPP
