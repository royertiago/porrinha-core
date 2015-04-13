#ifndef CORE_DETAIL_RUN_H
#define CORE_DETAIL_RUN_H

/* Internal header that declares all internal structures of the game.
 * It also provides the 'run_game' function.
 *
 * This header merely runs the game;
 * command-line parsing is left to core/game.cpp.
 *
 * All the variables referred to in the functions' documentation
 * are declared in detail/variables.h.
 */

namespace core { namespace detail {
    /* Run a game with the specified number of chopsticks.
     *
     * This function assumse that both the variables
     * 'global_player_count' and 'players'
     * had already been set.
     *
     * This is the "main" function of this file.
     * All the remaining functions are called by this one.
     */
    void run_game( int choptsicks );

    /* Initialize the variables of the game.
     *
     * Variables assumed valid:
     *  players
     *
     * Updated variables:
     *  position
     *  chopsticks
     *  current_hand
     *  last_hand
     *  guesses
     *  guess_template
     *  chopstick_count
     *  active_player_count
     *  starting_player
     *  last_winner
     *
     * Essentially, all variables except players,
     * global_player_count, and hand_sum.
     */
    void init( int initial_chopsticks );

    /* Calls player[index]->hand() and apply sanity checks.
     * Returns zero if the player did not return a valid value;
     * otherwise, return the correct player hand
     * (which might be zero).
     *
     * Variables assumed valid:
     *  players
     *  chopsticks
     *
     * Updated variables:
     *  players (calls non-const method on one of them).
     */
    int get_hand( int index );

    /* Calls player[index]->guess() and apply sanity checks.
     * Returns INVALID_GUESS if the player
     *  - made a negative guess; or
     *  - made a guess higher than chopstick_count; or
     *  -guessed an already guessed value.
     * Otherwise, returns the correct player guess.
     *
     * Variables assumed valid:
     *  players
     *  chopstick_count
     *
     * Updated variables:
     *  players (calls non-const method on one of them).
     */
    int get_guess( int index );

    /* Decides if someone has won this round.
     *
     * Variables assumed valid:
     *  current_hand
     *
     * Updated vairables:
     *  chopsticks
     *  last_hand
     *  guess_template
     *  chopstick_count
     *  active_player_count
     *  starting_player
     */
    void contabilize_round_winner();

    /* Execute a round with the current players.
     *
     * Variables assumed valid:
     *  players
     *  chopsticks
     *  guess_template
     *  starting_player
     *
     * Updated variables:
     *  chopsticks
     *  current_hand
     *  last_hand
     *  guesses
     *  guess_template
     *  chopstick_count
     *  active_player_count
     *  starting_player
     *  last_winner
     */
    void run_round();
}} // namespace core::detail

#endif // CORE_DETAIL_RUN_H
