#ifndef CLI_HANDLER_HPP
#define CLI_HANDLER_HPP

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "options.hpp"
#include <getopt.h>
using namespace std;

// TODO
//  Print help for the user when requested.
//  argv[0] is the name of the currently executing program
void printHelp(char *command)
{
    cout << command << " help me please" << endl;
} // printHelp()

// TODO
//  Finish this function, look for individual TODO comments internally.
//  Process the command line; there is no return value, but the Options
//  struct is passed by reference and is modified by this function to send
//  information back to the calling function.
void getOptions(int argc, char **argv, Options &options)
{
    // These are used with getopt_long()
    opterr = static_cast<int>(false); // Let us handle all error output for command line options
    int choice;
    int index = 0;

    // NOLINTBEGIN: getopt is old C code predating C++ style, this usage is from `man getopt`
    option longOptions[] = {
        // Insert two lines for the "mode" ('m') and the "help" ('h') options.
        {"mode", required_argument, nullptr, 'm'},
        {"input", required_argument, nullptr, 'i'},
        {"supply-demand", required_argument, nullptr, 's'},
        {"help", no_argument, nullptr, 'h'},
        {nullptr, 0, nullptr, '\0'}}; // longOptions[]
    // NOLINTEND

    // Fill in the double quotes, to match the mode and help options.
    while ((choice = getopt_long(argc, argv, "hm:i:s:", static_cast<option *>(longOptions), &index)) != -1)
    {
        switch (choice)
        {
        case 'h':
            printHelp(*argv);
            exit(0);

        case 'm':
        {
            string arg { optarg };

            if(arg == "MIN")
                options.mode = Mode::kMin;

            else if(arg == "MAX")
                options.mode = Mode::kMax;

            else if(arg == "CITIES")
                options.mode = Mode::kCities;

            else
            {
                cerr << "Invalid argument is: " << arg << endl; 
                exit(1);
            }

            break;
        }
        case 'i':
        {
            string arg{ optarg };
            ifstream file(arg);
            if(!file)
            {
                cerr << "Input file <" << arg << ">" << " does not exist" << endl;
                exit(1);
            }
            options.input_file = arg;
            break;
        }

        case 's':
        {
            string arg{ optarg };
            ifstream file(arg);
            if(!file)
            {
                cerr << "Supply demand file <" << arg << ">" << " does not exist" << endl;
                exit(1);
            }
            options.supply_demand_file = arg;
            break;
        }

        default:
            cerr << "Error: invalid option" << endl;
            exit(1);

        } // switch ..choice
    } // while
} // getOptions()
#endif