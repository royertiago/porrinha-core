// Implementation of core/detail/run.h.
#include <iostream>
#include "run.h"

namespace core { namespace detail {

std::map< Player *, int > position;
std::vector<int> chopsticks;
std::vector<int> guesses;
int chopstick_count;
int active_player_count;

void run_game( std::vector<std::unique_ptr<Player>>&& players, int initial_chopsticks ) {
    guesses.resize( players.size() );

    for( unsigned i = 0; i < players.size(); i++ )
        position[players[i].get()] = i;

    chopsticks.resize( players.size() );
    std::fill( chopsticks.begin(), chopsticks.end(), initial_chopsticks );

    chopstick_count = initial_chopsticks * players.size();
    active_player_count = players.size();
    int starting_player = 0;

    /* This vector will be the initial empty guess vector
     * for each round.
     *
     * It will start populated as Player::PENDING.
     * We will change it to Player::NOT_PLAYING as the game progresses.
     */
    std::vector< int > guess_template( players.size(), Player::PENDING );

    // Number of chopsticks each player has in hand.
    std::vector< int > hands(players.size());

    while( active_player_count >= 2 ) {
        guesses = guess_template;

        std::cout << chopstick_count << " chopsticks on the table...\n";

        // Pick each player hand
        int hand_sum = 0;
        for( int i = 0; i < players.size(); ++i ) {
            int p = (i + starting_player) % players.size();
            if( guesses[p] == Player::NOT_PLAYING ) continue;
            hands[p] = players[p]->hand();
            if( hands[p] < 0 || hands[p] > chopsticks[p] ) {
                std::clog << "Player " << players[p]->name()
                    << ", at position " << p << ", chosen "
                    << hands[p] << " chopsticks as its hand, "
                    "despite having only " << chopsticks[p] << " left.\n"
                    "Resetting its hand to 0...\n";
                hands[p] = 0;
            }
            hand_sum += hands[p];
        }

        // Pick each player guess
        int winner = -1;
        for( int i = 0; i < players.size(); ++i ) {
            int p = (i + starting_player) % players.size();
            if( guesses[p] == Player::NOT_PLAYING ) continue;
            guesses[p] = players[p]->guess( guesses );
            if( guesses[p] < 0 ) {
                std::clog << "Player " << players[p]->name()
                    << ", at position " << p << ", stupidly guessed "
                    << guesses[i] << ".\n"
                    << "I will reset it to a negative value"
                    << "to indicate an invalid guess.\n";
                guesses[p] = Player::INVALID;
                continue;
            }
            if( guesses[p] > chopstick_count ) {
                std::clog << "Player " << players[p]->name()
                    << ", at position " << i << ", stupidly guessed "
                    << guesses[p] << ", despite having only "
                    << chopstick_count << " chopsticks left on the table.\n"
                    << "I will reset it to a negative value"
                    << "to indicate an invalid guess.\n";
                guesses[p] = Player::INVALID;
                continue;
            }
            for( int j = 0; j < players.size(); ++j )
                if( j != p && guesses[p] == guesses[j] ) {
                    std::clog << "Player " << players[p]->name()
                        << ", at position " << p << ", guessed the value "
                        << guesses[p] << " - thats the same value that player "
                        << players[j]->name() << " guessed.\n"
                        << "I will reset it to a negative value"
                        << "to indicate an invalid guess.\n";
                    guesses[p] = Player::INVALID;
                    break;
                    /* Note we are only breaking from the inner `for`;
                     * that's okay since the test below this value
                     * will alway fail, and we get to restart the cycle. */
                }

            /* Its easier to do the winner test now
             * than to loop through the vector again.
             * No one will know... */
            if( guesses[p] == hand_sum )
                winner = p;
        }

        // Settling the round
        for( int i = 0; i < players.size(); ++i ) {
            int p = (i + starting_player) % players.size();
            if( guesses[p] == Player::NOT_PLAYING ) continue;
            players[p]->settle_round( hands, guesses );
            std::cout << "Player " << p << " (" << players[p]->name() << ")"
                << " - hand: " << hands[p] << " - guess: " << guesses[p] << '\n';
        }

        // Contabilizing the winner
        if( winner == -1 ) {
            std::cout << "No one guessed the right value (" << hand_sum << ").\n"
                "Next round...\n";

            do {
                starting_player = (starting_player + 1) % players.size();
            } while( guess_template[starting_player] == Player::NOT_PLAYING );

            continue;
        }

        std::cout << "Player " << winner << " (" << players[winner]->name() << ")"
            << " guessed right!\n";

        chopstick_count--;
        starting_player = winner;
        chopsticks[winner]--;
        if( chopsticks[winner] != 0 )
            continue;

        std::cout << "Player " << winner << " (" << players[winner]->name() << ")"
            << " left the game.\n";

        guess_template[winner] = Player::NOT_PLAYING;
        active_player_count--;
        while( guess_template[starting_player] == Player::NOT_PLAYING )
            starting_player = (starting_player + 1) % players.size();

    } // while( active_player_count >= 2 )

    std::cout << "Game ended. \n"
        << "Loser: player " << starting_player
        << " (" << players[starting_player]->name() << ")"
        << ", with " << chopsticks[starting_player] << " choptsticks.\n";

} // void run_game( std::vector<std::unique_ptr<Player>>&&, int )

}} // namespace core::detail
