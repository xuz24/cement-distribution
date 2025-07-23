#ifndef OPTIONS_HPP
#define OPTIONS_HPP

#include <string>

using namespace std;

// These modes represent the different ways to process the input data.
// There are three modes and the state where no mode has been selected.
enum class Mode
{
    kMin,
    kMax,
    kCities
}; // Mode{}

// This struct is used to pass options read from the command line, to
// the functions that actually do the work. Project 0 is simple enough to
// not need this, but it is a technique that could be helpful in doing more
// complicated command-line processing in future projects.
struct Options
{
    Mode mode;
    string input_file;
    string supply_demand_file;
}; // Options{}

#endif