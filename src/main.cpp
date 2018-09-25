// -*-c++-*-

/***************************************************************************
                                  main.cpp
                           Main for rcssserver
                             -------------------
    begin                : 1996
    copyright            : (C) 1996-2000 Electrotechnical Laboratory.
                           Itsuki Noda, Yasuo Kuniyoshi and Hitoshi Matsubara.
                           (C) 2001- by The RoboCup Soccer Server
                           Maintenance Group.
    email                : sserver-admin@lists.sourceforge.net
***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU LGPL as published by the Free Software  *
 *   Foundation; either version 3 of the License, or (at your option) any  *
 *   later version.                                                        *
 *                                                                         *
 ***************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "stadium.h"
#include "serverparam.h"
#include "version.h"

#include "stdtimer.h"
#include "synctimer.h"

#include <memory>
#include <iostream>
#include <locale>
#include <cmath>
#include <cstring>
#include <csignal>
#include <cerrno>

#include "RemoteMain.h"
#include "PlayerState.h"
#include <thread>
#include <chrono>

namespace {

Stadium Std;

void
sigHandle( int )
{
    Std.finalize( "Server Killed. Exiting..." );
}

}
int
main( int argc, char *argv[] )
{
    std::locale::global( std::locale::classic() );

    std::cout << PACKAGE << "-" << VERSION << "\n\n"
              << Copyright << std::endl;

    if ( ! ServerParam::init( argc, argv ) )
    {
        return 1;
    }

    struct sigaction sig_action;
    sig_action.sa_handler = &sigHandle;
    sig_action.sa_flags = 0;
    if ( sigaction( SIGINT, &sig_action , NULL ) != 0
         || sigaction( SIGTERM, &sig_action , NULL ) != 0
         || sigaction( SIGHUP, &sig_action , NULL ) != 0 )
    {
        std::cerr << __FILE__ << ": " << __LINE__
                  << ": could not set signal handler: "
                  << strerror( errno ) << std::endl;
        ServerParam::instance().clear();
        return 1;
    }

    if ( ! Std.init() )
    {
        ServerParam::instance().clear();
        return 1;
    }

    std::shared_ptr< Timer > timer;
    if ( ServerParam::instance().synchMode() )
    {
        timer = std::shared_ptr< Timer >( new SyncTimer( Std ) );
    }
    else
    {
        timer = std::shared_ptr< Timer >( new StandardTimer( Std ) );
    }

// // without thread
//     std::vector<char *> parameters_for_player1 { 
//                   "here should be path to exe", 
//                   "-host", "localhost", 
//                   "-port", "6000", 
//                   "-coach_port", "6001", 
//                   "-olcoach_port", "6002",
//                   "-team_name", "A",
//                   "-goalie", "off",
//                   "-coach", "off"
//                   };

//     std::vector<char *> parameters_for_goalie1 = parameters_for_player1;
//     parameters_for_goalie1[parameters_for_goalie1.size() - 3] = "on";

//     std::vector<char *> parameters_for_coach1 = parameters_for_player1;
//     parameters_for_coach1[parameters_for_coach1.size() - 1] = "on";

//     std::cout << "Initializing team A...\n";

//     std::cout << "Running team goalie...\n";
//     RemoteMain::run(parameters_for_goalie1.size(), parameters_for_goalie1.data());

//     for ( int i = 2 ; i <= 11; ++i )
//     {
//       std::cout << "Running team player " << i << "...\n";
//       RemoteMain::run(parameters_for_player1.size(), parameters_for_player1.data());
//     }

//     std::cout << "Running team coach...\n";
//     RemoteMain::run(parameters_for_coach1.size(), parameters_for_coach1.data());



// threads

    // 1
    using namespace std::chrono_literals;
    std::vector<char *> parameters_for_player1 { 
                  "here should be path to exe", 
                  "-host", "localhost", 
                  "-port", "6000", 
                  "-coach_port", "6001", 
                  "-olcoach_port", "6002",
                  "-team_name", "A",
                  "-goalie", "off",
                  "-coach", "off"
                  };

    std::vector<char *> parameters_for_goalie1 = parameters_for_player1;
    parameters_for_goalie1[parameters_for_goalie1.size() - 3] = "on";

    std::vector<char *> parameters_for_coach1 = parameters_for_player1;
    parameters_for_coach1[parameters_for_coach1.size() - 1] = "on";

    std::cout << "Initializing team A...\n";

    std::cout << "Running team goalie...\n";
    std::thread goalie1(RemoteMain::run, parameters_for_goalie1.size(), parameters_for_goalie1.data());
    goalie1.detach();
    
    std::cout << "Waiting for goalie... ";
    std::this_thread::sleep_for(1s);
    std::cout << "Done." << std::endl;

    for ( int i = 0 ; i < 10; ++i )
    {
        std::cout << "Running team player " << i << "...\n";
        std::thread player1(RemoteMain::run, parameters_for_player1.size(), parameters_for_player1.data());
        player1.detach();
        std::cout << "Waiting for player... ";
        std::this_thread::sleep_for(0.1s);
        std::cout << "Done." << std::endl;
    }


    std::cout << "Waiting for coach1... ";
    std::this_thread::sleep_for(1s);
    std::cout << "Done." << std::endl;


    std::cout << "Running team coach...\n";
    std::thread coach1(RemoteMain::run, parameters_for_coach1.size(), parameters_for_coach1.data());
    coach1.detach();

    std::cout << "Waiting for coach2... ";
    std::this_thread::sleep_for(1s);
    std::cout << "Done." << std::endl;

    // 2
    std::vector<char *> parameters_for_player2 { 
              "here should be path to exe", 
              "-host", "localhost", 
              "-port", "6000", 
              "-coach_port", "6001", 
              "-olcoach_port", "6002",
              "-team_name", "B",
              "-goalie", "off",
              "-coach", "off"
              };

    std::vector<char *> parameters_for_goalie2 = parameters_for_player2;
    parameters_for_goalie2[parameters_for_goalie2.size() - 3] = "on";

    std::vector<char *> parameters_for_coach2 = parameters_for_player2;
    parameters_for_coach2[parameters_for_coach2.size() - 1] = "on";

    std::cout << "Initializing team B...\n";

    std::cout << "Running team goalie...\n";
    std::thread goalie2(RemoteMain::run, parameters_for_goalie2.size(), parameters_for_goalie2.data());
    goalie2.detach();
    
    std::cout << "Waiting for goalie... ";
    std::this_thread::sleep_for(1s);
    std::cout << "Done." << std::endl;

    for ( int i = 0 ; i < 10; ++i )
    {
        std::cout << "Running team player " << i << "...\n";
        std::thread player2(RemoteMain::run, parameters_for_player2.size(), parameters_for_player2.data());
        player2.detach();
        
        std::cout << "Waiting for player... ";
        std::this_thread::sleep_for(0.1s);
        std::cout << "Done." << std::endl;
    }

    std::cout << "Waiting for coach... ";
    std::this_thread::sleep_for(1s);
    std::cout << "Done." << std::endl;

    std::cout << "Running team coach...\n";
    std::thread coach2(RemoteMain::run, parameters_for_coach2.size(), parameters_for_coach2.data());
    coach2.detach();

    std::cout << "Waiting for coach... ";
    std::this_thread::sleep_for(1s);
    std::cout << "Done." << std::endl;


    // for ( int i = 0; i < 2; i++)
    // {
    //   std::vector<thread> threads;
    //   std::vector<char *> parameters_for_player { 
    //                 "here should be path to exe", 
    //                 "-host", "localhost", 
    //                 "-port", "6000", 
    //                 "-coach_port", "6001", 
    //                 "-olcoach_port", "6002",
    //                 "-team_name", "A"
    //                 };
    //   if(i == 1)
    //     parameters_for_player[parameters_for_player.size() - 1] = "B"; // changing team name

    //   std::vector<char *> parameters_for_goalie = parameters_for_player;
    //   parameters_for_goalie.push_back("-goalie");
    //   parameters_for_goalie.push_back("on");

    //   std::vector<char *> parameters_for_coach = parameters_for_player;
    //   parameters_for_coach.push_back("-coach");
    //   parameters_for_coach.push_back("on");



    //   std::cout << "Initializing team " << i << "...\n";

    //   // std::cout << "Running team coach...\n";
    //   // std::thread coach(RemoteMain::run, parameters_for_coach.size(), parameters_for_coach.data());
    //   // coach.detach();
    //   // std::cout << "Running team goalie...\n";
    //   // std::thread goalie(RemoteMain::run, parameters_for_goalie.size(), parameters_for_goalie.data());
    //   // goalie.detach();

    //   parameters_for_player.push_back("-goalie");
    //   parameters_for_player.push_back("off");
    //   parameters_for_player.push_back("-coach");
    //   parameters_for_player.push_back("off");
    //   // threads.push_back(std::move(goalie));
    //   // r.run(parameters.size(), parameters.data()); // goalie
    //   for ( int i = 2 ; i <= 11; ++i )
    //   {
    //     std::cout << "Running team player " << i << "...\n";
    //     std::thread player(RemoteMain::run, parameters_for_player.size(), parameters_for_player.data());
    //     player.detach();
    //     // threads.push_back(std::move(player));
    //     // r.run(parameters.size(), parameters.data()); // players
    //   }
      
    //   // threads.push_back(std::move(coach));
    //   //r.run(parameters.size(), parameters.data()); // coach
    //   // parameters.pop_back();
    //   // parameters.push_back("off");

    //   // parameters.pop_back();
    //   // parameters.pop_back(); // delete coach
    //   // parameters.pop_back();
    //   // parameters.pop_back(); // delete goalie
    //   // parameters.pop_back();
    //   // parameters.pop_back(); // delete team
    //   // for ( auto& th : threads )
    //   // {
    //   //   th.detach();
    //   // }
    // }



    // vector<char *> parameters { "here should be path to exe", "-host", "localhost", "-port", "6000", "-coach_port", "6001", "-olcoach_port", "6002"};
    // for ( int i = 0; i < 2; i++)
    // {
    //   parameters.push_back("-team_name");
    //   std::cout << "Initializing team ";
    //   if ( i == 0 )
    //   {
    //     parameters.push_back("A");
    //   }
    //   else
    //   {
    //     parameters.push_back("B");
    //   }
    //   std::cout << parameters.back() << "...\n";

    //   parameters.push_back("-goalie");
    //   parameters.push_back("on");
    //   std::cout << "Running team goalie...\n";
    //   std::thread goalie(RemoteMain::run, parameters.size(), parameters.data());
    //   goalie.detach();
    //   // r.run(parameters.size(), parameters.data()); // goalie
    //   parameters.pop_back();
    //   parameters.push_back("off");
    //   for ( int i = 1 ; i < 11; ++i )
    //   {
    //     std::cout << "Running team player " << i << "...\n";
    //     std::thread player(RemoteMain::run, parameters.size(), parameters.data());
    //     player.detach();
    //     // r.run(parameters.size(), parameters.data()); // players
    //   }
    //   parameters.push_back("-coach");
    //   parameters.push_back("on");
    //   std::cout << "Running team coach...\n";
    //   std::thread coach(RemoteMain::run, parameters.size(), parameters.data());
    //   coach.detach();
    //   //r.run(parameters.size(), parameters.data()); // coach

    //   parameters.pop_back();
    //   parameters.pop_back(); // delete coach
    //   parameters.pop_back();
    //   parameters.pop_back(); // delete goalie
    //   parameters.pop_back();
    //   parameters.pop_back(); // delete team
    // }
    

    std::cout << "\nHit CTRL-C to exit\n";

    timer->run();

    ServerParam::instance().clear();

    return 0;
}
