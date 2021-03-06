#include "util.h"
#include "core/detail/variables.h"

namespace core {
    int global_player_count() {
        return detail::players.size();
    }

    int player_count() {
        return detail::chopsticks.size();
    }

    int active_player_count() {
        return detail::active_player_count;
    }

    int chopstick_count() {
        return detail::chopstick_count;
    }

    int index( Player * me ) {
        auto it = detail::position.find(me);
        if( it == detail::position.end() )
            return -1;
        return it->second;
    }

    const Player * player( int index ) {
        return detail::players[index].get();
    }

    int chopsticks( int player_index ) {
        return detail::chopsticks[player_index];
    }

    const std::vector<int> & chopsticks() {
        return detail::chopsticks;
    }

    int guess( int player_index ) {
        return detail::guesses[player_index];
    }

    const std::vector<int> & guess() {
        return detail::guesses;
    }

    bool valid_guess( int possible_guess ) {
        if( possible_guess < 0 || possible_guess > chopstick_count() )
            return false;
        for( auto guess: detail::guesses )
            if( guess == possible_guess )
                return false;
        return true;
    }

    const std::vector<int>& hand() {
        return detail::last_hand;
    }

    int hand( int player_index ) {
        return detail::last_hand[player_index];
    }

    int last_winner() {
        return detail::last_winner;
    }

} // namespace core
