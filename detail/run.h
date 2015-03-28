#ifndef CORE_DETAIL_RUN_H
#define CORE_DETAIL_RUN_H

/* Internal header that declares all internal structures of the game.
 * It also provides the 'run_game' function.
 *
 * This header merely runs the game;
 * command-line parsing is left to core/game.cpp.
 */
#include <map>
#include <memory>
#include <vector>
#include "player.h"

namespace core { namespace detail {
    extern std::map< Player *, int > position;
    extern std::vector<int> chopsticks;
    extern std::vector<int> guesses;
    extern int chopstick_count;
    extern int active_player_count;

    void run_game( std::vector<std::unique_ptr<Player>>&&, int choptsicks );
}} // namespace core::detail

#endif // CORE_DETAIL_RUN_H
