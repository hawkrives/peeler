#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <map>
#include <cstdlib>
using namespace std;
#include "peeler-common.h"

int hash_string(string input) {
	size_t len = input.size();
	size_t quadtrant = len / 4;

	if (len < 4) {
		return asciiify(input);
	}

	string str1 = input.substr(quadtrant*0, quadtrant);
	string str2 = input.substr(quadtrant*1, quadtrant);
	string str3 = input.substr(quadtrant*2, quadtrant);
	string str4 = input.substr(quadtrant*3, len);

	int hash1 = asciiify(str1) * seeds[0];
	int hash2 = asciiify(str2) * seeds[1];
	int hash3 = asciiify(str3) * seeds[2];
	int hash4 = asciiify(str4) * seeds[3];

	int hash_sum = hash1 + hash2 + hash3 + hash4;

	return hash_sum % nextPrime;
}

struct Dictionary {
	vector<string> wordArray;   // store the data
	unordered_map<int, vector<string> > hashTable;

	Dictionary(const char *filename);  // constructor

	bool inWordArray(string &s);       // single query
	void check(const char *filename);  // multiple queries
};

void getWords(const char *filename, vector<string> &vec, unordered_map<int, vector<string> > &m) {
	ifstream f(filename);
	if ( !f.good() ) {
		cerr << "Error:  unable to open " << filename << endl;
		exit(-1);
	}
	string s;
	cerr << "reading/hashing file" << endl;
	while ( f >> s ) {
		vec.push_back(s);
		int hash = hash_string(s);
		try {
			vector<string> &current = m.at(hash);
			current.push_back(s);
		} catch (...) {
			vector<string> strings = {s};
			m.insert(make_pair(hash, strings));
		}
	}
	cerr << "done with file; " << m.size() << endl;
	map<int, int> sizes;
	for (auto pair : m) {
		sizes.insert(make_pair(pair.first, pair.second.size()));
	}
	for (auto item : sizes)
		cout << item << endl;
}

Dictionary::Dictionary( const char *filename ) {
	getWords(filename, wordArray, hashTable);
}

bool Dictionary::inWordArray(string &s) {
	int hash = hash_string(s);

	try {
		vector<string> possibilities = hashTable.at(hash);
		for (string str : possibilities)
			if (str == s)
				return 1;
	} catch (...) {}

	return 0;
}

void Dictionary::check( const char *filename ) {
	vector<string> query;
	getWords(filename, query);

	cerr << "checking " << filename << endl;
	start_timer();  // from elapsed_time.h

	int counter = 0, n = query.size();
	bool in_array;
	double cycles;
	for ( int i = 0; i < n; ++i ) {
		start_timer();
		in_array = inWordArray(query[i]);
		cycles = elapsed_time();
		if ( !in_array ) {
			++counter;
		}
		cout << (in_array ? 1 : 0) << "\t" << cycles << endl;
	}
	cerr << "Misspelled " << counter << " words." << endl;

	cycles = elapsed_time();
	cerr << " Total cycles: " << cycles << endl;
}

int main(int argc, char **argv) {
	if ( argc != 3 ) {
		cerr << "Usage: spellCheck dictionaryFile inputFile" << endl;
		exit(0);
	}
	Dictionary d(argv[1]);
	d.check(argv[2]);
}
