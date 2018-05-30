#include <cstring>
#include <cstdlib>
#include <string>
#include <iostream>
#include "parser.hpp"

using namespace protocol;

parser::parser()
{

}

/* protocol::Header parser::parse_header() {
    std::string header(data_, protocol::HEADER_LENGTH);

    std::cout << "Header as string is:" << header << std::endl;

    auto type = static_cast<unsigned int>(std::stoi(header));

    std::cout << "Header as int is: " << type << std::endl;

    return protocol::Header(type);
} */
