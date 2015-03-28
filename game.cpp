#include <iostream>
#include "game.h"

namespace core {

    int play(
        int argc,
        char ** argv, 
        std::vector< std::pair<const char *, PlayerFactory> > players
    ) {
//int play( int, int, std::vector< std::pair<const char*, PlayerFactory> > players ) {
    std::cout << "The game is not implemented.\n"
        "Avalable players:\n";
    for( auto pair : players )
        std::cout << pair.first << std::endl;

    return 0;
}

} // namespace core
