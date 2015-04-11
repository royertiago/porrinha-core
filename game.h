#ifndef GAME_H
#define GAME_H

#include <utility>
#include "player.h"
#include "cmdline/args.h"

namespace core {

    /* Runs the game. */
    int play(
        cmdline::args&&,
        std::vector< std::pair<const char *, PlayerFactory> >
    );

} // namespace core

#endif // GAME_H
