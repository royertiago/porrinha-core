#ifndef CORE_UTIL_H
#define CORE_UTIL_H

/* Utility functions to query the current state of the game.
 *
 * Not all functions may be called at any moment.
 * There are four sections
 * ("global information", "overall game queries",
 * "individual round queries" and "end round information"),
 * each with appropriate call time.
 * These call times are described in each section.
 */

#include "player.h"

namespace core {

/* Global information
 *
 * These methods give information that is persistent across games.
 * They are all avaliable when the factory functions are called,
 * and shall remain valid until the destruction of every player object.
 */

    /* Returns the number of avaliable players that can appear in a game.
     *
     * Currently, since there is only one game per program execution,
     * this value equals player_count().
     */
    int global_player_count();

/* Overall game queries
 *
 * This infromation exists "internally" in a game.
 * It is updated only after the round ended.
 *
 * It shall be valid when Player::begin_game is called for the first player,
 * and shall remain valid until Player::end_game is called for the last player.
 */

    /* Returns the number of players of this game.
     *
     * This number includes the players that are now active,
     * by emptying their hands during this game.
     */
    int player_count();

    /* Returns the number of active players of this game;
     * that is, players that are still playing the game. */
    int active_player_count();

    /* Returns the sum of chopsticks avaliable to all players. */
    int chopstick_count();

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

    /* Returns the player given the index.
     *
     * Note that the player is const; the game assumes
     * you will not call Player::hand, Player::guess etc in that player.
     */
    const Player * player( int player_index );

    /* Returns the number of chopsticks that the respective player have.
     *
     * Note that this is _not_ the number of chopsticks
     * returned by the Player::hand() method.
     *
     * player_index is assumed to be between 0 and player_count()-1.
     */
    int chopsticks( int player_index );

    /* Returns the choptsick vector.
     * chopsticks(index) is the same as chopsticks()[index].
     */
    const std::vector<int>& chopsticks();

/* Individual round queries
 *
 * This information is updated during a round.
 * The information begins the round "scarce" and is built as the round progresses.
 * Any data created during a round shall remain avaliable through these functions
 * until the call of Player::end_round.
 */

    /* Returns the current guess of the selected player.
     *
     * If guess(i) >= 0, then guess(i) is guaranteed to be
     * between 0 and chopstick_count().
     * Negative values codify status of the guess.
     * They can be PENDING_GUESS, NOT_PLAYING or INVALID_GUESS;
     * their meaning are documented below.
     *
     * player_index is assumed to be between 0 and player_count()-1.
     */
    int guess( int player_index );

    /* Returns the guess vector.
     * guess(index) is the same as guess()[index].
     */
    const std::vector<int>& guess();


    /* Non-guess values returned by core::guess(). */
    enum {
        /* The player has still not made its guess, but its guess will,
         * eventually, be updated.
         * Note that, if invoked from inside Player::guess, core::guess
         * will return PENDING_GUESS for the current player.
         */
        PENDING_GUESS = -1,

        /* The player is not playing anymore.
         * Currently, this only happens when the player's chopstick number
         * drops to zero.
         */
        NOT_PLAYING = -2,

        /* The player had made an invalid guess for this round.
         */
        INVALID_GUESS = -3,
    };

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

/* End round information
 *
 * Information that becomes avaliable only at the end of a round.
 *
 * It shall be avaliable when Player::end_round is called for the first player
 * and shall remain valid until the first Player::end_round call
 * of the following round.
 */

    /* Returns the number of chopsticks the player had in hand
     * in the last round.
     *
     * Is -1 if the player had not played last round,
     * or if it is the first round of the game. */
    int hand( int player_index );

    /* Returns the hand vector.
     * hand(index) is hand()[index].
     */
    const std::vector<int>& hand();

    /* Returns the winner of the last round.
     * This value is -1 in the first round of the game.
     */
    int last_winner();

} // namespace core
#endif // CORE_UTIL_H
