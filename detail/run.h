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
    /* List of players, indexed by their position.
     */
    extern std::vector<std::unique_ptr<Player>> players;

    /* Reverse map: it gives the player position
     * based on a pointer to it.
     */
    extern std::map< Player *, int > position;

    /* Number of chopsticks each player have avaliable. */
    extern std::vector<int> chopsticks;

    /* Number of chopsticks each player held in hand last round. */
    extern std::vector<int> last_hand;

    /* Number of chopsticks each player holds in hand in this round.
     *
     * This vector is swapped with `last_hand` every round.
     */
    extern std::vector<int> current_hand;

    /* Guesses each player has made this round.
     * This is the vector `other_guesses` passed to the players
     * when invoking Player::guess.
     */
    extern std::vector<int> guesses;

    /* Initial empty guess vector for each round.
     *
     * At the beginning of the round, the vector `guesses`
     * will be replaced by this vector.
     *
     * It will start populated as PENDING_GUESS.
     * We will change it to NOT_PLAYING as the game progresses.
     */
    extern std::vector<int> guess_template;

    /* Sum of all avaliable chopsticks in the table. */
    extern int chopstick_count;

    /* Sum of the vector current_hand.
     * That is, the correct guess for this round.
     *
     * It is only updated after current_hand is populated.
     */
    extern int hand_sum;

    /* Number players that are still playing the game. */
    extern int active_player_count;

    /* Number of players that are avaliable to create games.
     * This value is independent of the current game.
     *
     * This variable is set directly by core::play,
     * since it needs to be avaliable since before any PlayerFactory invocation.
     */
    extern int global_player_count;

    /* Player that won last round. */
    extern int last_winner;

    /* Player that starts guessing this round. */
    extern int starting_player;

    /* Run a game with the specified number of chopsticks.
     *
     * This function assumse that both the variables
     * 'global_player_count' and 'players'
     * had already been set.
     */
    void run_game( int choptsicks );
}} // namespace core::detail

#endif // CORE_DETAIL_RUN_H
