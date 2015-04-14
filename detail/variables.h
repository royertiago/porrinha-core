#ifndef DETAIL_VARIABLES_H
#define DETAIL_VARIABLES_H

/* Global variables inside namespace core::detail.
 */

#include <map>
#include <memory>
#include <ostream>
#include <vector>
#include "player.h"

namespace core { namespace detail {
    /* Output stream used by the functions in this namespace.
     */
    extern std::ostream * os;

    /* List of players, indexed by their position.
     */
    extern std::vector<std::unique_ptr<Player>> players;

    /* Reverse map: it gives the player position
     * based on a pointer to it.
     */
    extern std::map< Player *, int > position;

    /* Number of chopsticks each player have avaliable. */
    extern std::vector<int> chopsticks;

    /* Number of chopsticks each player holds in hand in this round.
     *
     * This vector is swapped with `last_hand` every round.
     */
    extern std::vector<int> current_hand;

    /* Number of chopsticks each player held in hand last round. */
    extern std::vector<int> last_hand;

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

    /* Player that starts guessing this round. */
    extern int starting_player;

    /* Player that won last round.
     *
     * It's the same as starting_player,
     * unless no one made the right guess last round.
     * In this case, last_winner == -1.
     */
    extern int last_winner;

    /* List that contains the list of players that are outside of the game
     * due to emptying their hands,
     * in the order they got out.
     *
     * At the end of the game, this will have the "ranking" of each player.
     */
    extern std::vector<int> out_of_game;

}} // namespace core::detail

#endif // DETAIL_VARIABLES_H
