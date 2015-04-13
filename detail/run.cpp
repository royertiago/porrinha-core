// Implementation of core/detail/run.h.
#include <iostream>
#include "run.h"
#include "core/util.h" // constants PENDING_GUESS, NOT_PLAYING, INVALID_GUESS

namespace core { namespace detail {

std::vector<std::unique_ptr<Player>> players;
std::map< Player *, int > position;
std::vector<int> chopsticks;
std::vector<int> last_hand;
std::vector<int> current_hand;
std::vector<int> guesses;
std::vector<int> guess_template;
int chopstick_count;
int active_player_count;
int global_player_count;
int last_winner;
int starting_player;
int hand_sum;

void init( int initial_chopsticks ) {
    guesses.resize( players.size() );
    last_winner = -1;

    for( unsigned i = 0; i < players.size(); i++ )
        position[players[i].get()] = i;

    chopsticks.resize( players.size() );
    std::fill( chopsticks.begin(), chopsticks.end(), initial_chopsticks );

    chopstick_count = initial_chopsticks * players.size();
    active_player_count = players.size();
    starting_player = 0;

    guess_template = std::vector< int >( players.size(), PENDING_GUESS );
    current_hand = std::vector< int >( players.size(), -1 );
    last_hand = current_hand;
}

void run_game( int initial_chopsticks ) {
    init( initial_chopsticks );

    for( int i = 0; i < players.size(); ++i )
        players[i]->begin_game();

    while( active_player_count >= 2 ) {
        guesses = guess_template;

        std::cout << chopstick_count << " chopsticks on the table...\n";

        // Pick each player hand
        hand_sum = 0;
        for( int i = 0; i < players.size(); ++i ) {
            int p = (i + starting_player) % players.size();
            if( guesses[p] == NOT_PLAYING ) continue;
            current_hand[p] = players[p]->hand();
            if( current_hand[p] < 0 || current_hand[p] > chopsticks[p] ) {
                std::clog << "Player " << players[p]->name()
                    << ", at position " << p << ", chosen "
                    << current_hand[p] << " chopsticks as its hand, "
                    << "despite having only " << chopsticks[p] << " left.\n"
                    << "Resetting its hand to 0...\n";
                current_hand[p] = 0;
            }
            hand_sum += current_hand[p];
        }

        // Pick each player guess
        last_winner = -1;
        for( int i = 0; i < players.size(); ++i ) {
            int p = (i + starting_player) % players.size();
            if( guesses[p] == NOT_PLAYING ) continue;
            guesses[p] = players[p]->guess();
            if( guesses[p] < 0 ) {
                std::clog << "Player " << players[p]->name()
                    << ", at position " << p << ", stupidly guessed "
                    << guesses[i] << ".\n"
                    << "I will reset it to a negative value "
                    << "to indicate an invalid guess.\n";
                guesses[p] = INVALID_GUESS;
                continue;
            }
            if( guesses[p] > chopstick_count ) {
                std::clog << "Player " << players[p]->name()
                    << ", at position " << i << ", stupidly guessed "
                    << guesses[p] << ", despite having only "
                    << chopstick_count << " chopsticks left on the table.\n"
                    << "I will reset it to a negative value "
                    << "to indicate an invalid guess.\n";
                guesses[p] = INVALID_GUESS;
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
                    guesses[p] = INVALID_GUESS;
                    break;
                    /* Note we are only breaking from the inner `for`;
                     * that's okay since the test below this value
                     * will alway fail, and we get to restart the cycle. */
                }

            /* Its easier to do the last_winner test now
             * than to loop through the vector again.
             * No one will know... */
            if( guesses[p] == hand_sum )
                last_winner = p;
        }

        /* Settling the round
         * We need first to keep the integrity of last_hand.
         * Since we will not use the vector current_hand in this iteration,
         * we may simply swap both values.
         *
         * Note we must not std::move current_hand to last_hand
         * because in the next loop we will assume current_hand has size players.size(),
         * so we would need to realocate space.
         */
        last_hand.swap( current_hand );

        // Contabilizing the winner
        if( last_winner == -1 ) {
            std::cout << "No one guessed the right value (" << hand_sum << ").\n"
                << "Next round...\n";

            do {
                starting_player = (starting_player + 1) % players.size();
            } while( guess_template[starting_player] == NOT_PLAYING );
        }
        else {
            std::cout << "Player " << last_winner
                << " (" << players[last_winner]->name() << ")"
                << " guessed right!\n";

            chopstick_count--;
            starting_player = last_winner;
            chopsticks[last_winner]--;
            if( chopsticks[last_winner] == 0 ) {
                std::cout << "Player " << last_winner
                    << " (" << players[last_winner]->name() << ")"
                    << " left the game.\n";

                // We need a new starter.
                guess_template[last_winner] = NOT_PLAYING;
                active_player_count--;
                while( guess_template[starting_player] == NOT_PLAYING )
                    starting_player = (starting_player + 1) % players.size();
            }
        }

        // Calling Player::end_round() for each player
        for( int i = 0; i < players.size(); ++i ) {
            int p = (i + starting_player) % players.size();
            if( guesses[p] == NOT_PLAYING ) continue;
            players[p]->end_round();
            std::cout << "Player " << p << " (" << players[p]->name() << ")"
                << " - hand: " << last_hand[p] << " - guess: " << guesses[p] << '\n';
        }

    } // while( active_player_count >= 2 )

    for( int i = 0; i < players.size(); ++i )
        players[i]->begin_game();

    std::cout << "Game ended. \n"
        << "Loser: player " << starting_player
        << " (" << players[starting_player]->name() << ")"
        << ", with " << chopsticks[starting_player] << " choptsticks.\n";

} // void run_game( std::vector<std::unique_ptr<Player>>&&, int )

}} // namespace core::detail
