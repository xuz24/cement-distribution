#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include "cli_handler.hpp"
#include "options.hpp"
#include "cement_distribution.hpp"
#include <iomanip>

using namespace std;

int main(int argc, char *argv[])
{
    // This should be in all of your projects, speeds up I/O
    ios_base::sync_with_stdio(false);
    cout << std::setprecision(2); // Always show 2 decimal places
    cout << std::fixed;           // Disable scientific notation for large numbers
    // Get the mode from the command line and read in the data
    Options options;
    getOptions(argc, argv, options);
    cement_distribution distributor(options);
    distributor.parse_input();
    distributor.parse_supply_demand();
    distributor.push_flow();
    return 0;
} // main()
