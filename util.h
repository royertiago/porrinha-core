#ifndef CORE_UTIL_H
#define CORE_UTIL_H

/* Utility functions to query the current state of the game.
 *
 * These functions may be called only when the game is running;
 * that is, after the first call to Player::hand.
 *
 * All these values are deductible from the parameters
 * passed to the player methods.
 * They are only replicated here for convenience.
 */

#include "player.h"

namespace core {

// Overall game queries

    /* Returns the number of players of this game.
     * The count includes the inactive players. */
    int player_count();

    /* Returns the number of active players of this game;
     * that is, players that are still playing the game. */
    int active_player_count();

    /* Returns the sum of chopsticks avaliable to all players. */
    int chopstick_count();

// Individual player queries

    /* Returns the index of the target player.
     *
     * This function is intended to be used like
     *  core::index( this );
     *
     * The game assumes that this function will be called
     * using only pointers to players that are actually playing the game
     * (that is, their methods Player::hand, Player::guess and
     * Player::settle_round are being called).
     */
    int index( Player * me );

    /* Returns the number of chopsticks that the respective player have.
     *
     * Note that this is _not_ the number of chopsticks
     * returned by the Player::hand() method.
     *
     * player_index is assumed to be between 0 and player_count()-1.
     */
    int chopsticks( int player_index );

    /* Returns the current guess of the selected player.
     * The player might have still not guessed,
     * or not playing anymore,
     * or made an invalid movement;
     * the returned values in this case follows the convention for
     * the `other_guesses` vector, in Player::guess method.
     *
     * player_index is assumed to be between 0 and player_count()-1.
     */
    int guess( int player_index );

    /* Tests if 'possible_guess' is a valid guess according to
     * both the game rules and two "stupidity rules"
     * imposed by the game engine.
     *
     * A guess is invalid if either
     *  - Is negative.
     *  - Is greater than chopstick_count().
     *  - Is equal to some other player guess.
     * In all other cases, the guess is valid.
     */
    bool valid_guess( int possible_guess );

} // namespace core
#endif // CORE_UTIL_H
