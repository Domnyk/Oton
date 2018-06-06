#include <iostream>
#include "MovieLayer.hpp"

MovieLayer::MovieLayer() : movies_location_() {}

std::pair<MovieLayer::movies_location::iterator, bool>  MovieLayer::add_movie(const string& movie_path) {
    string movie_filename = get_movie_filename(movie_path);
    return movies_location_.emplace(movie_filename, movie_path);
}

void MovieLayer::delete_movie(const string& movie_filename) {
    movies_location_.erase(movie_filename);
}

bool MovieLayer::is_movie_loaded(const string& movie_filename) const {
    auto it = movies_location_.find(movie_filename);
    return !(it == movies_location_.end());
}

string MovieLayer::get_movie_filename(const string& movie_path) {
    // Based on: http://www.cplusplus.com/reference/string/string/find_last_of/
    size_t found = movie_path.find_last_of("/\\");
    string movie_filename = movie_path.substr(found + 1);

    return movie_filename;
}

string MovieLayer::get_movie_location(const string& movie_filename) const {
    if(!is_movie_loaded(movie_filename)) {
        throw std::runtime_error("No such movie in library");
    }

    return movies_location_.find(movie_filename)->second;
}

std::string get_movie_list(const MovieLayer& movie_layer) {
    string movie_list;

    for(auto& elem: movie_layer.movies_location_) {
        movie_list += elem.first;
        movie_list += '\n';
    }

    return movie_list;
}
