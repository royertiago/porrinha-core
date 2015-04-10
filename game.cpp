namespace core { namespace command_line {
    const char help_message[] =
"[options] player_names ...\n"
"Generates a porrinha round with the given player names.\n"
"Currently, there is no mechanism to pass arguments\n"
"to artificial intelligences.\n"
"\n"
"Options:\n"
"--chopsticks <N>\n"
"    Chose the initial number of chopsticks.\n"
"    Default value: 3.\n"
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
#include "detail/run.h"

namespace core {

    std::map< std::string, PlayerFactory > factories;

    namespace command_line {

        int chopsticks = 3;
        std::vector< std::string > player_list;

        void parse( int argc, char ** argv ) {
            static option options[] = {
                {"chopsticks", required_argument, 0, 'c'},
                {"list", no_argument, 0, 'l'},
                {"help", no_argument, 0, 'h'},
                {0, 0, 0, 0},
            };
            int opt;
            int dummy_option_index; // unused
            while( (opt = getopt_long( argc, argv, "c:lh",
                            options, &dummy_option_index
                    )) != -1 ) {
                switch( opt ) {
                    case 'c':
                        if( std::sscanf( optarg, "%i", &chopsticks ) != 1 ) {
                            std::cerr << "core: Not a valid number: "
                                << optarg << '\n';
                            std::exit( 1 );
                        }
                        if( chopsticks <= 0 ) {
                            std::cerr << "core: Chopstick number must be positive.\n";
                            std::exit( 1 );
                        }
                        break;
                    case 'l':
                        for( auto pair : factories )
                            std::cout << pair.first << std::endl;
                        std::exit( 0 );
                        break;
                    case 'h':
                        std::cout << argv[0] << " " << help_message;
                        std::exit( 0 );
                        break;
                    default:
                        std::cerr << "core: Unknown option: " << (char) optopt << '\n';
                        std::exit( 1 );
                        break;
                }
            }
            while( optind < argc )
                player_list.emplace_back( argv[optind++] );
        } // void parse( int, char** )
    } // namespace command_line

    int play(
        int argc,
        char ** argv, 
        std::vector< std::pair<const char *, PlayerFactory> > player_options
    ) {
        factories.clear();
        factories.insert( player_options.begin(), player_options.end() );

        command_line::parse(argc, argv);

        if( command_line::player_list.size() < 2 ) {
            std::cerr << "There must be at least two players in this game!\n";
            return 1;
        }

        std::vector< std::unique_ptr<Player> > players;
        for( auto type : command_line::player_list ) {
            std::vector< char > string(type.begin(), type.end());
            string.push_back('\0');
            char * args[] = {string.data(), nullptr};
            players.emplace_back( factories[type]( 1, args ) );
        }

        detail::run_game( std::move(players), command_line::chopsticks );
        return 0;
    }

} // namespace core
