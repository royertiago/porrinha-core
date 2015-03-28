#ifndef GAME_H
#define GAME_H

#include <utility>
#include "player.h"

namespace core {

    /* Runs the game. */
    int play(
        int argc,
        char ** argv, 
        std::vector< std::pair<const char *, PlayerFactory> >
    );

} // namespace core

#endif // GAME_H
