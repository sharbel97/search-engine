/*
**  Author: Sharbel Homa, University of Illinois at Chicago
**
**  Project: Query Search Engine.
**
**
**  Creative Component: Stop Words
**  implemented the suggested stop words (733 words)
**  and all collected in stopwords.txt and only collected
**  if the programmer enables it to do so on line 208
**  by adding a 3rd optional buildIndex paramter: true.
**  the implementation happens mostly in the buildIndex
**  function definition. Mainly collected the words in
**  set of type string and searching it for every single
**  word collected by gatherTokens, if it exists in 
**  stopWords, discard. if it doesn't, add it to the index.
*/

#pragma once

#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <cctype>
#include <set>
#include <map>
using namespace std;

bool isCompound(string s);
bool isAlpha(string s);
set<string> findCompoundQuery(map<string, set<string>>& index,
                              string sentence);

// cleanToken function that takes any single word string s
// and runs 2 while loops for punctuation and 1 forloop
// to lowercase, then returns the cleaned string.
string cleanToken(string s) {
    string token = s;
    if (!isAlpha(token)) {
        return "";
    }
    while (ispunct(token[0])) {
        token.erase(0, 1);
    }
    while (ispunct(token.back())) {
        token.erase(token.size()-1, -1);
    }
    // lowercase
    for (auto &e: token) {
        e = tolower(e);
    }
    return token;
}

// gatherTokens function that takes text string input and
// creates a string stream of tokens which then get cleaned
// then inserted into the set.
set<string> gatherTokens(string text) {
    set<string> tokens;
    string token;
    istringstream is(text);
    while (is >> token) {
        if (isAlpha(token)) {
            tokens.insert(cleanToken(token));
        }
    }
    return tokens;
}

// buildIndex function takes filename, and a map passed
// by reference which populates all content of the file
// to the map and have each keyword pair with a set of
// urls that have that keyword.
// The function uses getline() and gets the url from the
// first line and then continue with another getline
// to get the keywords. the keywords string is streamed
// and inserted into the index map with the respective url.
int buildIndex(string filename, map<string, set<string>>& index,
               bool stop = false) {
    string url, text;
    int pages = 0;
    ifstream infile(filename);

    cout << "Stand by while building index..." << endl;

    if (!infile.good()) {
        cout << "Couldn't read the file" << endl;
        return 0;
    }
    
    set<string> stopWords;
    string word;
    if (stop) {
        // gather stopWords
        ifstream infile ("stopwords.txt");
        while (infile >> word) {
            stopWords.insert(word);
        }
    }

    while (!infile.eof()) {
        getline(infile, url);
        if (!infile.fail()) {
            pages++; // incrementing urls
            getline(infile, text);
            if (!infile.fail()) {
                set<string> words = gatherTokens(text);
                for (auto word : words) {
                    if (stop) {  // stop words excluded
                        if (stopWords.count(word) == 0) {
                            index[word].insert(url);
                        }
                    } else {  // stop words included
                        index[word].insert(url);
                    }
                }
            }
        }
    }
    cout << "Indexed " << pages << " pages containing ";
    cout << index.size() << " unique terms" << endl;
    return pages;
}

// findQueryMatches function takes the index and a search
// sentence, then it searches the index and returns a set
// results with a successful search.
set<string> findQueryMatches(map<string, set<string>>& index,
                             string sentence) {
    set<string> result;
    if (isCompound(sentence)) {
        result = findCompoundQuery(index, sentence);
    } else {
        string key;
        istringstream is(sentence);
        while (is >> key) {
            key = cleanToken(key);
            if (index.count(key) == 1) {
                // token exists, find urls and
                // insert to results
                for (auto url : index[key]) {
                    result.insert(url);
                }
            }
        }
    }
    cout << "Found " << result.size() << " matching pages" << endl;
    return result;
}


//
// findCompoundQuery function is a recomposition function to help
// "findQueryMatches" function be less messy. This function handles
// a compound sentences. sentences that contain a + or -, but not neither.
// it uses string stream and then substr to look for the + or -
// then I use STL library <algorithm> to use set_intersection and
// set_difference to help gather my results.
set<string> findCompoundQuery(map<string, set<string>>& index,
                              string sentence) {
    set<string> result_main;
    string key;
    istringstream is(sentence);
    while (is >> key) {
        if (key.substr(0, 1) != "+" && key.substr(0, 1) != "-") {
           key = cleanToken(key);
           if (index.count(key) == 1) {
                // token exists, find urls and
                // insert to results
                for (auto url : index[key]) {
                    result_main.insert(url);
                }
            }
        } else if (key.substr(0, 1)== "+") {
            key = cleanToken(key);
            set<string> result_temp;
            set_intersection(result_main.begin(), result_main.end(),
                             index[key].begin(), index[key].end(),
                             inserter(result_temp, result_temp.begin()));
            // clear result_main then insert new re-insert result
            result_main.clear();
            for (auto url : result_temp) {
                result_main.insert(url);
            }
        } else if (key.substr(0, 1)== "-") {
            key = cleanToken(key);
            set<string> result_temp;
            set_difference(result_main.begin(), result_main.end(),
                           index[key].begin(), index[key].end(),
                           inserter(result_temp, result_temp.begin()));
            // clear result_main then insert new re-insert result
            result_main.clear();
            for (auto url : result_temp) {
                result_main.insert(url);
            }
        }
    }
    return result_main;
}


// searchEngine function takes in a filename and calls all the previous
// functions to create a user friendly input/output prompts.
void searchEngine(string filename) {
    map<string, set<string>> index;
    buildIndex(filename, index);

    string query;
    cout << "Enter query sentence (press enter to quit): ";
    getline(cin, query);

    while (query != "") {
        set<string> results = findQueryMatches(index, query);
        for (auto e : results) {
            cout << e << endl;
        }
        cout << endl;
        cout << "Enter query sentence (press enter to quit): ";
        getline(cin, query);
    }
    
    cout << "Thank you for searching!" << endl;
}


//
// isAlpha boolean function returns true if a string
// has at least 1 alphabetic character.
bool isAlpha(string s) {
    for (auto e : s) {
        if (isalpha(e)) {
            return true;
        }
    }
    return false;
}


//
// isCompound boolean function returns true if a string
// has at least 1 '+' or '-' character.
bool isCompound(string s) {
    for (auto e : s) {
        if (e == '+' || e == '-') {
            return true;
        }
    }
    return false;
}
