// Implementation of core/detail/run.h.
#include <iostream>
#include "run.h"
#include "core/detail/variables.h"
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
int last_winner;
int starting_player;
int hand_sum;
std::vector<int> out_of_game;
std::ostream * os = &std::cout;

void out( std::ostream& new_os ) {
    os = &new_os;
}

std::ostream & out() {
    return *os;
}

void set_players( std::vector<std::pair<PlayerFactory, cmdline::args>>&& list ) {
    players = std::vector<std::unique_ptr<Player>>( list.size() );

    for( unsigned i = 0; i < list.size(); i++ )
        players[i].reset(list[i].first( std::move(list[i].second) ));
}

void init( int initial_chopsticks ) {
    for( unsigned i = 0; i < players.size(); i++ )
        position[players[i].get()] = i;

    chopsticks.resize( players.size() );
    std::fill( chopsticks.begin(), chopsticks.end(), initial_chopsticks );

    current_hand = std::vector< int >( players.size(), -1 );
    last_hand = current_hand;

    guesses.resize( players.size() );
    guess_template = std::vector< int >( players.size(), PENDING_GUESS );

    chopstick_count = initial_chopsticks * players.size();
    active_player_count = players.size();
    starting_player = 0;
    last_winner = -1;

    out_of_game.clear();
}

int get_hand( const int index ) {
    int hand = players[index]->hand();
    if( hand < 0 || hand > chopsticks[index] ) {
        std::clog << "Player " << players[index]->name()
            << ", at position " << index << ", chosen "
            << hand << " chopsticks as its hand, "
            << "despite having only " << chopsticks[index] << " left.\n"
            << "Resetting its hand to 0...\n";
        return 0;
    }

    return hand;
}

int get_guess( const int index ) {
    int guess = players[index]->guess();
    if( guess < 0 ) {
        std::clog << "Player " << players[index]->name()
            << ", at position " << index << ", stupidly guessed "
            << guess << ".\n"
            << "I will reset it to a negative value "
            << "to indicate an invalid guess.\n";
        return INVALID_GUESS;
    }
    if( guess > chopstick_count ) {
        std::clog << "Player " << players[index]->name()
            << ", at position " << index << ", stupidly guessed "
            << guess << ", despite having only "
            << chopstick_count << " chopsticks left on the table.\n"
            << "I will reset it to a negative value "
            << "to indicate an invalid guess.\n";
        return INVALID_GUESS;
    }
    for( int j = 0; j < players.size(); ++j )
        if( j != index && guess == guesses[j] ) {
            std::clog << "Player " << players[index]->name()
                << ", at position " << index << ", guessed the value "
                << guess << " - thats the same value that player "
                << players[j]->name() << " guessed.\n"
                << "I will reset it to a negative value "
                << "to indicate an invalid guess.\n";
            return INVALID_GUESS;
        }

    return guess;
}

void contabilize_round_winner() {
    /* We need first to keep the integrity of last_hand.
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
        out() << "No one guessed the right value (" << hand_sum << ").\n";

        do {
            starting_player = (starting_player + 1) % players.size();
        } while( guess_template[starting_player] == NOT_PLAYING );

        return;
    }

    out() << "Player " << last_winner
        << " (" << players[last_winner]->name() << ")"
        << " guessed right!\n";

    chopstick_count--;
    starting_player = last_winner;
    chopsticks[last_winner]--;

    if( chopsticks[last_winner] != 0 )
        return;

    out() << "Player " << last_winner
        << " (" << players[last_winner]->name() << ")"
        << " left the game.\n";

    out_of_game.push_back( last_winner );

    // We need a new starter.
    guess_template[last_winner] = NOT_PLAYING;
    active_player_count--;
    while( guess_template[starting_player] == NOT_PLAYING )
        starting_player = (starting_player + 1) % players.size();
}

void run_round() {
    guesses = guess_template;

    out() << chopstick_count << " chopsticks on the table...\n";

    // Pick each player hand
    hand_sum = 0;
    for( int i = 0; i < players.size(); ++i ) {
        int p = (i + starting_player) % players.size();
        if( guesses[p] == NOT_PLAYING ) continue;
        current_hand[p] = get_hand(p);
        hand_sum += current_hand[p];
    }

    // Pick each player guess
    last_winner = -1;
    for( int i = 0; i < players.size(); ++i ) {
        int p = (i + starting_player) % players.size();
        if( guesses[p] == NOT_PLAYING ) continue;
        guesses[p] = get_guess(p);

        /* Its easier to do the last_winner test now
         * than to loop through the vector again.
         * No one will know... */
        if( guesses[p] == hand_sum )
            last_winner = p;
    }

    contabilize_round_winner();


    // Calling Player::end_round() for each player
    for( int i = 0; i < players.size(); ++i ) {
        int p = (i + starting_player) % players.size();
        if( guesses[p] == NOT_PLAYING ) continue;
        players[p]->end_round();
        out() << "Player " << p << " (" << players[p]->name() << ")"
            << " - hand: " << last_hand[p] << " - guess: " << guesses[p] << '\n';
    }
}

std::vector<int> run_game( int initial_chopsticks ) {
    init( initial_chopsticks );

    for( int i = 0; i < players.size(); ++i )
        players[i]->begin_game();

    const char * next_round_msg = "";

    while( active_player_count >= 2 ) {
        out() << next_round_msg;
        next_round_msg = "Next round...\n\n";

        run_round();
    }

    for( int i = 0; i < players.size(); ++i )
        players[i]->begin_game();

    out() << "Game ended. \n"
        << "Loser: player " << starting_player
        << " (" << players[starting_player]->name() << ")"
        << ", with " << chopsticks[starting_player] << " choptsticks.\n";

    out_of_game.push_back(starting_player);

    return out_of_game;
}

}} // namespace core::detail
