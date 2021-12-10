//
// Search Database Project 2 CS251 UIC
// Author: Sharbel Homa, Unviersity of Illinois at Chicago
//
// Basic program that is able to parse text files with 
// very specific format and organize data in map data 
// structure and contain the appropriate key-value pairs
// for the search keywords and their corresponding urls.

#include <iostream>
#include "search.h"
using namespace std;

int main() {
    
    searchEngine("cplusplus.txt");
    //map<string, set<string>> index;
    
    //buildIndex("tiny.txt", index, true);
    
    return 0;
}

