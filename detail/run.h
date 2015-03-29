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
    /* Reverse map: it gives the player position
     * based on a pointer to it.
     */
    extern std::map< Player *, int > position;

    /* Number of chopsticks each player have avaliable. */
    extern std::vector<int> chopsticks;

    /* Guesses each player has made this round.
     * This is the vector `other_guesses` passed to the players
     * when invoking Player::guess.
     */
    extern std::vector<int> guesses;

    /* Sum of all avaliable chopsticks in the table. */
    extern int chopstick_count;

    /* Number players that are still playing the game. */
    extern int active_player_count;

    void run_game( std::vector<std::unique_ptr<Player>>&&, int choptsicks );
}} // namespace core::detail

#endif // CORE_DETAIL_RUN_H
