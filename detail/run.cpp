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

    /* This variable will hold a vector of -1 values.
     * As players get out of the game, we will update this vector
     * to be -2 for that player.
     *
     * Thus, this vector is adequate to be the initial "guesses" vector
     * for any round. */
    std::vector< int > guess_template( players.size(), -1 );

    // Number of chopsticks each player has in hand.
    std::vector< int > hands(players.size());

    while( active_player_count >= 2 ) {
        guesses = guess_template;

        std::cout << chopstick_count << " chopsticks on the table...\n";

        // Pick each player hand
        int hand_sum = 0;
        for( int i = starting_player; i < players.size(); ++i ) {
            int p = i % players.size();
            if( guesses[p] == -2 ) continue;
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
        for( int i = starting_player; i < players.size(); ++i ) {
            int p = i % players.size();
            if( guesses[p] == -2 ) continue;
            guesses[p] = players[p]->guess( guesses );
            if( guesses[p] < 0 ) {
                std::clog << "Player " << players[p]->name()
                    << ", at position " << p << ", stupidly guessed "
                    << guesses[i] << " - it is deemed to be wrong. ";
                continue;
            }
            if( guesses[p] > chopstick_count ) {
                std::clog << "Player " << players[p]->name()
                    << ", at position " << i << ", stupidly guessed "
                    << guesses[p] << ", despite having only "
                    << chopstick_count << " chopsticks left on the table.\n";
                continue;
            }
            for( int j = 0; j < players.size(); ++j )
                if( j != p && guesses[p] == guesses[j] ) {
                    std::clog << "Player " << players[p]->name()
                        << ", at position " << p << ", guessed the value "
                        << guesses[p] << " - thats the same value that player "
                        << players[j]->name() << " guessed.\n"
                        << "Resetting its guess to -1 as a penalty "
                        << "(this way, this player surely will miss).\n";
                    guesses[p] = -1;
                    break;
                }

            /* Its easier to do the winner test now
             * than to loop through the vector again.
             * No one will know... */
            if( guesses[p] == hand_sum )
                winner = p;
        }

        // Settling the round
        for( int i = starting_player; i < players.size(); ++i ) {
            int p = i % players.size();
            if( guesses[p] == -2 ) continue;
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
            } while( guess_template[starting_player] == -2 );

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

        guess_template[winner] = -2;
        active_player_count--;
        while( guess_template[starting_player] == -2 )
            starting_player = (starting_player + 1) % players.size();

    } // while( active_player_count >= 2 )

    std::cout << "Game ended. \n"
        << "Loser: player " << starting_player
        << " (" << players[starting_player]->name() << ")"
        << ", with " << chopsticks[starting_player] << " choptsticks.\n";

} // void run_game( std::vector<std::unique_ptr<Player>>&&, int )

}} // namespace core::detail
