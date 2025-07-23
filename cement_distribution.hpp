#include "options.hpp"
#include <iostream>
#include <vector>
#include <limits>
#include <cmath>
#include <queue>
#include <istream>
#include <sstream>
#ifndef CEMENT_DISTRIBUTION_HPP
#define CEMENT_DISTRIBUtION_HPP

using namespace std;

struct Edge
{
    string origin;
    string destination;
    int dist;
};

struct edge_greater
{
    bool operator()(const Edge &left, const Edge &right) const
    {
        if (left.dist == right.dist)
            return left.origin > right.origin;
        else
            return left.dist > right.dist;
    }
};

ostream &operator<<(ostream &os, const Edge &edge)
{
    os << "Edge(" << edge.origin << " -> " << edge.destination << ", " << edge.dist << ")";
    return os;
}

class cement_distribution
{
private:
    Options options;
    vector<Edge> edges;
    vector<pair<string, int>> supply_demand_vec;
    // state, supply/demand
    // supply is positive, demand is negative

    int seperator;

public:
    cement_distribution(Options options_in)
        : options(options_in)
    {
        switch (options.mode)
        {
        case Mode::kCities:
            cout << "Using averaged CITY distances as weights\n";
            seperator = 3;
            break;
        case Mode::kMax:
            cout << "Using averaged MAX border distances as weights\n";
            seperator = 1;
            break;
        case Mode::kMin:
            cout << "Using averaged MIN border distances as weights\n";
            seperator = 0;
            break;

        default:
            seperator = 0;
        }
    }

    void reformat(std::string &raw)
    {
        if (raw.empty())
            return;

        size_t start = 0;
        size_t end = raw.size();

        auto isSpecial = [](char c)
        {
            return c == '"' || c == '\n' || c == '\r' || c == '\t' || isspace(static_cast<unsigned char>(c));
        };

        // Move start forward past leading special characters
        while (start < end && isSpecial(raw[start]))
            ++start;

        // Move end backward past trailing special characters
        while (end > start && isSpecial(raw[end - 1]))
            --end;

        if (start < end)
            raw = raw.substr(start, end - start);
        else
            raw.clear(); // Entire string was special characters
    }

    void parse_state(ifstream &in)
    {
        // parse header
        vector<string> cities;
        string trash;
        string city;
        while (true)
        {
            while (trash != "Major Cities:")
            {
                getline(in, trash, ',');
            }
            for (int i = 0; i < 5; i++)
            {
                getline(in, city, ',');
                reformat(city);
                cities.push_back(city);
            }
            getline(in, trash, ',');
            if (trash != "Neighboring state")
                break;
        }
        // parse plant dists
        while (trash == "")
        {
            getline(in, trash, ',');
        }
        // first row
        string curr_state;
        string neighbor_state;
        int curr_dist;
        vector<string> curr_plants;
        vector<Edge> curr_edges;

        reformat(trash);
        curr_state = trash;
        getline(in, trash, ',');
        reformat(trash);
        curr_plants.push_back(trash);
        getline(in, trash, ',');

        while (trash != "")
        {
            reformat(trash);
            neighbor_state = trash;

            for (int i = 0; i < seperator; i++)
            {
                getline(in, trash, ',');
            }

            if (options.mode == Mode::kCities)
            {
                int dist_sum = 0;
                for (int i = 0; i < 5; i++)
                {
                    getline(in, trash, ',');
                    reformat(trash);
                    dist_sum += stoi(trash);
                }
                curr_dist = static_cast<int>(round(dist_sum / 5.0));
            }

            else
            {
                getline(in, trash, ',');
                reformat(trash);
                curr_dist = stoi(trash);
                for (int i = 0; i < 7 - seperator; i++)
                {
                    getline(in, trash, ',');
                }
            }

            curr_edges.push_back(Edge{curr_state, neighbor_state, curr_dist});

            // cout << curr_edges.back() << endl;

            getline(in, trash, ','); // next state up
        }
        while (trash == "" && !in.eof())
        {
            getline(in, trash, ',');
        }

        reformat(trash);

        // rest of rows
        while (trash != "State" && !in.eof())
        {
            getline(in, trash, ',');
            reformat(trash);
            curr_plants.push_back(trash);
            // cout << trash << endl;
            getline(in, trash, ',');
            getline(in, trash, ',');
            size_t k = 0;

            while (trash != "")
            {
                for (int i = 0; i < seperator; i++)
                {
                    getline(in, trash, ',');
                }

                if (options.mode == Mode::kCities)
                {
                    int dist_sum = 0;
                    for (int i = 0; i < 5; i++)
                    {
                        reformat(trash);
                        dist_sum += stoi(trash);
                        getline(in, trash, ',');
                    }
                    curr_dist = static_cast<int>(round(dist_sum / 5.0));
                }

                else
                {
                    // getline(in, trash, ',');
                    reformat(trash);
                    curr_dist = stoi(trash);
                    for (int i = 0; i < 7 - seperator + 1; i++)
                    {
                        getline(in, trash, ',');
                    }
                }

                curr_edges[k].dist += curr_dist;
                // cout << curr_edges[k] << endl;
                k++;

                getline(in, trash, ','); // next state up
            }
            while (trash == "" && !in.eof())
            {
                getline(in, trash, ',');
            }
            reformat(trash);
        }

        for (size_t i = 0; i < curr_edges.size(); i++)
        {
            curr_edges[i].dist /= static_cast<int>(curr_plants.size());
            edges.push_back(curr_edges[i]);
        }
    }

    void parse_input()
    {
        ifstream input(options.input_file);
        while (!input.eof())
        {
            parse_state(input);
        }
                cout << "\n--------------------------\n"
             << "     EDGES BEING USED\n"
             << "--------------------------\n";
        for (auto &edge : edges)
        {
            cout << edge << '\n';
        }
    }

    vector<string> split(const string &s, const string &delimiter)
    {
        vector<string> tokens;
        size_t pos = 0;
        string token;
        string str = s; // Make a local copy to modify
        while ((pos = str.find(delimiter)) != string::npos)
        {
            token = str.substr(0, pos);
            tokens.push_back(token);
            str.erase(0, pos + delimiter.length());
        }
        tokens.push_back(str);

        return tokens;
    }

    void parse_supply_demand()
    {
        ifstream supply_demand(options.supply_demand_file);
        string line;
        getline(supply_demand, line);
        while (!supply_demand.eof())
        {
            getline(supply_demand, line);
            vector<string> words = split(line, ",");
            string state;
            int supply;
            if (words.size() > 2)
            {
                supply = stoi(words.back());
                for (size_t i = 0; i < words.size() - 1; i++)
                {
                    reformat(words[i]);
                    state += (words[i] + " ");
                }
                reformat(state);
            }
            else
            {
                state = words[0];
                reformat(state);
                supply = stoi(words[1]);
            }

            supply_demand_vec.push_back(pair<string, int>{state, supply});
        }

        cout << "\n--------------------------\n"
             << "STARTING SUPPLY AND DEMAND\n"
             << "--------------------------\n";
        for (const auto &pair : supply_demand_vec)
        {
            cout << pair.first << ", " << pair.second << '\n';
        }
    }

    void push_flow()
    {
        cout << "\n--------------------------\n"
             << "   OPTIMAL DISTRIBUTION    \n"
             << "--------------------------\n";
        // Print all edges in the priority queue order
        priority_queue<Edge, vector<Edge>, edge_greater> edge_pq(edge_greater(), edges);
        while (!edge_pq.empty())
        {
            const Edge &edge = edge_pq.top();
            string origin = edge.origin;
            string destination = edge.destination;
            pair<string, int> *origin_supply = nullptr;
            pair<string, int> *destination_demand = nullptr;
            for (auto &pair : supply_demand_vec)
            {
                if (pair.first.find(origin) != std::string::npos && pair.second < 0) // check origin has supply
                    origin_supply = &pair;
                if (pair.first.find(destination) != std::string::npos && pair.second > 0) // check destination has demand
                    destination_demand = &pair;
            }
            if (origin_supply && destination_demand)
            {
                if (destination_demand->second > abs(origin_supply->second))
                {
                    cout << "Pushed " << abs(origin_supply->second) << " flow from " << origin << " to " << destination << '\n';
                    destination_demand->second += origin_supply->second;
                    origin_supply->second = 0;
                }
                else
                {
                    cout << "Pushed " << destination_demand->second << " flow from " << origin << " to " << destination << '\n';
                    origin_supply->second += destination_demand->second;
                    destination_demand->second = 0;
                }
            }
            else
            {
                edge_pq.pop();
            }
        }

        cout << "\n--------------------------\n"
             << " ENDING SUPPLY AND DEMAND\n"
             << "--------------------------\n";
        for (const auto &pair : supply_demand_vec)
        {
            cout << pair.first << ", " << pair.second << '\n';
        }
    }
};

#endif
