/*
    Covid-19 Data Analysis

    Obtains the data of one covid data file

    Data Structure: Maps (Red-Black Tree)

    Goals: Input Data, 
    Accumulate Totals, 
    Search & output by country, 
    Use map for efficient search
*/

#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <sstream>

using namespace std;

// Data structure is map<key, value> where the 
// key is the country code (2 letter) and the value
// is a struct of the totals for that country
struct CountryData {
    string Name;
    int Confirmed;
    int Deaths;
    int Recovered;

    // default constructor
    CountryData() {
        Name = "";
        Confirmed = 0;
        Deaths = 0;
        Recovered = 0;
    }
};

// Converts a string to an integer, unless string is empty, in which
// case 0 is returned.
int string2Int(string s) {
    if (s == "")
        return 0;
    else
        return stoi(s);
}

// Given a filename denoting a daily Covid-19 report, builds and 
// returns a map<country, CountryData> (red-black tree underneath).
// Abstraction: Key, Value pairs
map<string, CountryData> inputData(string filename) {
    ifstream infile(filename);
    map<string, CountryData> data;

    if (!infile.good()) {
        cout << "** Error: unable to open '" << filename << "'..." << endl;
        return data;
    }

    // file is open, start processing:
    string line;

    getline(infile, line);  // input and discard first row -- header

    string region, country, datetime;
    string confirmed, deaths, recovered;

    while (getline(infile, line)) {
        stringstream s(line);   // allow to manipulate string/line; calling stream "s"

        getline(s, region, ',');    // first value ==> region
        getline(s, country, ',');   // second value ==> country
        getline(s, datetime, ',');
        getline(s, confirmed, ',');
        getline(s, deaths, ',');
        getline(s, recovered, ',');

    int numconfirmed, numdeaths, numrecovered;

    numconfirmed = string2Int(confirmed);
    numdeaths = string2Int(deaths);
    numrecovered = string2Int(recovered);

    // accumulate data in the map for this country

    CountryData& cd = data[country];    // get reference to data and update it

    cd.Name = country;
    cd.Confirmed += numconfirmed;
    cd.Deaths += numdeaths;
    cd.Recovered += numrecovered;
    }

    return data;
}

int main() {
    string filename = "03-28-2020.csv";
    map<string, CountryData> data;

    cout << "** Welcome to Covid-19 analysis program **" << endl;

    data = inputData(filename);

    // check to see if anything is inputted
    if (data.size() == 0) {
        cout << "No data, file not found?" << endl;
        return 0;
    }

    // we have data, output totals:
    cout << "Number of countries: " << data.size() << endl;

    int totalconfirmed, totaldeaths, totalrecovered;
    totalconfirmed = totaldeaths = totalrecovered = 0;

    for (const auto& kvpair : data) {
        totalconfirmed += kvpair.second.Confirmed;
        totaldeaths += kvpair.second.Deaths;
        totalrecovered += kvpair.second.Recovered;
    }

    cout << "Number of confirmed: " << totalconfirmed << endl;
    cout << "Number of deaths: " << totaldeaths << endl;
    cout << "Number of recovered: " << totalrecovered << endl;

    // now allow user to search by country 2-letter code:

    string country;

    cout << endl;
    cout << "Please enter a country or # to exit> ";
    cin >> country;

    while (country != "#") {
        auto iter = data.find(country); // search for country:

        if (iter == data.end()) // not found:
            cout << "Sorry, no data for that country" << endl;
        else {
            cout << "Number of confirmed: " << iter->second.Confirmed << endl;
            cout << "Number of deaths: " << iter->second.Deaths << endl;
            cout << "Number of recovered: " << iter->second.Recovered << endl;        
        }
        
        cout << endl;
        cout << "Please enter a country or # to exit> ";
        cin >> country;
    }

    // done


    cout << endl;
    cout << "** Done **" << endl;

    return 0;
}