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
#include <ostream>
#include "player.h"

namespace core { namespace detail {
    /* Sets the players for this game.
     * This function is like a "constructor" for this namespace.
     *
     * Each argument in the list is a pair<factory, args>.
     * Each factory will be called with the arguments it is paired with.
     *
     * This function guarantees that the variable 'players'
     * will have its final size before calling any factory function.
     *
     * Updated variables:
     *  players
     */
    void set_players( std::vector<std::pair<PlayerFactory, cmdline::args>>&& );

    /* Sets/retrieves the output stream for the functions of this namespace.
     *
     * Defaults to std::cout.
     */
    void out( std::ostream& );
    std::ostream & out();

    /* Run a game with the specified number of chopsticks.
     *
     * This function assumse that set_players had already been called.
     *
     * This is the "main" function of this file.
     * All the remaining functions are called by this one.
     *
     * Returns the ranking of each player.
     *
     * Variables assumed valid:
     *  players
     */
    std::vector<int> run_game( int choptsicks );

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
     *  out_of_game
     *
     * Essentially, all variables except players and hand_sum.
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
     *  out_of_game
     *
     * Updated vairables:
     *  chopsticks
     *  last_hand
     *  guess_template
     *  chopstick_count
     *  active_player_count
     *  starting_player
     *  out_of_game
     */
    void contabilize_round_winner();

    /* Execute a round with the current players.
     *
     * Variables assumed valid:
     *  players
     *  chopsticks
     *  guess_template
     *  starting_player
     *  out_of_game
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
     *  out_of_game
     */
    void run_round();
}} // namespace core::detail

#endif // CORE_DETAIL_RUN_H
