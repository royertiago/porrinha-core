namespace core { namespace command_line {
    const char help_message[] =
"[options] player_names ...\n"
"Generates a porrinha round with the given player names.\n"
"\n"
"If the player names are given normally, the game will assume that\n"
"there is no command line options for all players.\n"
"To pass these options, use the syntax '[player]' instead of 'player';\n"
"everything between one player name and another\n"
"will be considered as command line options.\n"
"\n"
"For example,\n"
"    ./main [human] --name Zero [human] --name One\n"
"\n"
"Note that, when using this format, you need to supply any game's core argument\n"
"before the first player name.\n"
"\n"
"Artificial intelligence programmers are advised to implement the option\n"
"'--help' for their AI's. Refer to that help, in each artificial intelligence,\n"
"for more information.\n"
"\n"
"Core options:\n"
"--chopsticks <N>\n"
"    Chose the initial number of chopsticks.\n"
"    Default value: 3.\n"
"\n"
"--games <N>\n"
"    Chose the number of games to be run.\n"
"    Default value: 1\n"
"\n"
"--disable-game-output\n"
"    Disable the output of the game outcome every round.\n"
"\n"
"--list\n"
"    List avaliable players and quit.\n"
"\n"
"--help\n"
"    Display this help and quit.\n"
;
}} // namespace core::command_line

#include <iostream>
#include <map>
#include <memory>
#include <vector>
#include <getopt.h>
#include "game.h"
#include "core/util.h"
#include "core/detail/run.h"
#include "core/detail/variables.h"

namespace {
    /* Unopened output stream.
     * Outputting to this stream produces nothing.
     */
    std::ostream null_os(0);
} // anonymous namespace

namespace core {

    std::map< std::string, PlayerFactory > factories;

    namespace command_line {

        int chopsticks = 3;
        int games = 1;
        std::vector< std::pair<PlayerFactory, cmdline::args> > player_list;

        /* Returns true if str is of the format [string]. */
        bool is_player( const std::string& str ) {
            if( str == "" ) return false;
            return *str.begin() == '[' && *str.rbegin() == ']';
        }


        void parse( cmdline::args&& args ) {
            while( args.size() > 0 ) {
                std::string arg = args.next();

                if( arg == "--help" ) {
                    std::cout << help_message;
                    std::exit(0);
                }
                if( arg == "--list" ) {
                    for( auto pair : factories )
                        std::cout << pair.first << "\n";
                    std::exit(0);
                }
                if( arg == "--chopsticks" ) {
                    args >> chopsticks;
                    continue;
                }
                if( arg == "--games" ) {
                    args >> games;
                    continue;
                }
                if( arg == "--disable-game-output" ) {
                    detail::out(null_os);
                    continue;
                }
                if( factories.count(arg) == 1 ) {
                    player_list.push_back( std::make_pair(
                        factories[arg],
                        cmdline::args()
                    ));
                    continue;
                }
                if( arg == "" ) {
                    std::cerr << "Unknown option \"\".\n";
                    continue;
                }
                if( is_player(arg) ) {
                    std::string player( ++arg.begin(), --arg.end() );
                    if( factories.count(player) == 0 ) {
                        std::cerr << "Player " << player << " not in player database.\n";
                        std::exit(1);
                    }
                    cmdline::args subargs = args.subarg_until( is_player );
                    subargs.program_name(player);

                    player_list.emplace_back( std::make_pair(
                        factories[player],
                        std::move(subargs)
                    ));
                    continue;
                }

                std::cerr << "'" << arg << "' is neither a player nor a valid option.\n";
                std::exit(1);
            }
        } // void parse( cmdline::args&& )
    } // namespace command_line

    void run_several_games();
    void run_single_game();

    int play(
        cmdline::args&& args,
        std::vector< std::pair<const char *, PlayerFactory> > player_options
    ) {
        factories.clear();
        factories.insert( player_options.begin(), player_options.end() );

        using namespace command_line;

        command_line::parse(std::move(args));

        if( player_list.size() < 2 ) {
            std::cerr << "There must be at least two players in this game!\n";
            std::exit(1);
        }

        detail::set_players( std::move(player_list) );

        if( games == 1 )
            run_single_game();
        else
            run_several_games();

        return 0;
    }

    void run_single_game() {
        auto ranking = detail::run_game( command_line::chopsticks );

        std::cout << "\n\tRanking:\n";
        for( unsigned i = 0; i < ranking.size(); i++ )
            std::cout << '[' << ranking[i] << "] "
                << player(ranking[i])->name() << '\n';
    }

    void run_several_games() {
        std::vector< int > first( global_player_count() );
        std::vector< int > second( global_player_count() );
        std::vector< int > third( global_player_count() );

        for( int game = 0; game < command_line::games; game++ ) {
            auto ranking = detail::run_game( command_line::chopsticks );
            first[ranking[0]]++;
            second[ranking[1]]++;
            if( global_player_count() >= 3 )
                third[ranking[2]]++;
        }

        if( global_player_count() >= 3 ) {
            std::cout << "Player - First places / second / third\n";
            for( int p = 0; p < global_player_count(); p++ )
                std::cout << player(p)->name() << " - "
                    << first[p] << " / "
                    << second[p] << " / "
                    << third[p] << "\n";
        }
        else {
            std::cout << "Player - wins\n";
            for( int p = 0; p < global_player_count(); p++ )
                std::cout << player(p)->name() << " - "
                    << first[p] << "\n";
        }
    }

} // namespace core
