/*
    Covid-19 Data Analysis

    Obtains the data of one covid data file

    Data Structure: Hashing

    Goals: Input Data, 
    Accumulate Totals, 
    Search & output by country, 
    Use Hash for efficient search
*/

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

// Data structure is hash where the array contains the struct CountryData 
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

// Given a 2-letter country code, returns an array index based on 
// weighted position of each letter
int Hash(string country) {

    // check if country is valid
    if (country.length() != 2)
        return -1;

    if (!((isalpha(country[0])) && (isalpha(country[1])))) 
        return -1;

    if (!((isupper(country[0])) && (isupper(country[1])))) 
        return -1;

    int letter1 = country[0] - 'A';         // subtract ASCII value to get regular digit
    int letter2 = country[1] - 'A';
    
    int index = (letter1 * 26) + letter2;   // given 676 combinations (0 - 675) | letter2 is the offset

    return index;
}

// Converts a string to an integer, unless string is empty, in which
// case 0 is returned.
int string2Int(string s) {
    if (s == "")
        return 0;
    else
        return stoi(s);
}

// Given a filename denoting a daily Covid-19 report, builds and 
// returns a true/false value (data exists/data does not exist)
bool inputData(string filename, CountryData* HT) {
    ifstream infile(filename);

    if (!infile.good()) {
        cout << "** Error: unable to open '" << filename << "'..." << endl;
        return false;
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

    CountryData& cd = HT[Hash(country)];    // get reference to data and update it

    cd.Name = country;
    cd.Confirmed += numconfirmed;
    cd.Deaths += numdeaths;
    cd.Recovered += numrecovered;
    }

    return true;    // data exists
}

int main() {
    string filename = "03-28-2020.csv";

    // allocate hash table for country data:
    int N = 676;
    CountryData* HT = new CountryData[N];   // create a hash table pointer
    cout << "** Welcome to Covid-19 analysis program **\n" << endl;
    cout << "File loaded: " << filename << endl;
    cout << endl;

    bool success = inputData(filename, HT);    // HT is a pointer allowed to modify the HT

    // check to see if anything is inputted
    if (!success) {
        cout << "No data, file not found?" << endl;
        return 0;
    }

    // output totals:

    int totalconfirmed, totaldeaths, totalrecovered, numcountries;
    totalconfirmed = totaldeaths = totalrecovered = numcountries = 0;

    for (int i = 0; i < N; i++) {
        if (HT[i].Name == "")
            ;
        else {
            // count the country
            numcountries++;

            totalconfirmed += HT[i].Confirmed;
            totaldeaths += HT[i].Deaths;
            totalrecovered += HT[i].Recovered;
        }
    }

    cout << "Number of confirmed: " << numcountries << endl;
    cout << "Number of confirmed: " << totalconfirmed << endl;
    cout << "Number of deaths: " << totaldeaths << endl;
    cout << "Number of recovered: " << totalrecovered << endl;

    // now allow user to search by country 2-letter code:

    string country;

    cout << endl;
    cout << "Please enter a country or # to exit> ";
    cin >> country;

    while (country != "#") {
        int index = Hash(country);  // hash country to its array index

        if (index < 0)
            cout << "Sorry, no data for that country, use 2 capital letters" << endl;
        else if (HT[index].Name == "") // not found:
            cout << "Sorry, no data for that country" << endl;
        else {
            cout << "Number of confirmed: " << HT[index].Confirmed << endl;
            cout << "Number of deaths: " << HT[index].Deaths << endl;
            cout << "Number of recovered: " << HT[index].Recovered << endl;        
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

