#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <map>
using namespace std;
#include "peeler-common.h"

int hashString(string &input) {
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
	unordered_map<int, vector<string> > hashTable;

	Dictionary(const char *filename);  // constructor

	bool inWordArray(string &s);       // single query
	void check(const char *filename);  // multiple queries
};

void getWords(const char *filename, unordered_map<int, vector<string> > &m) {
	ifstream f(filename);
	if ( !f.good() ) {
		cerr << "Error:  unable to open " << filename << endl;
		exit(-1);
	}
	string s;
	cerr << "reading/hashing file" << endl;
	while ( f >> s ) {
		int hash = hash_string(s);
		try {
			vector<string> &current = m.at(hash);
			current.push_back(s);
		} catch (...) {
			m.emplace(hash, vector<string> {s});
		}
	}

	cerr << "done with file; " << m.size() << " items." << endl;
}

Dictionary::Dictionary( const char *filename ) {
	getWords(filename, hashTable);
}

bool Dictionary::inWordArray(string &s) {
	int hash = hash_string(s);

	try {
		vector<string> &possibilities = hashTable.at(hash);
		int size = possibilities.size();
		for (size_t i = 0; i < size; i++)
			if (possibilities[i] == s)
				return true;
	} catch (...) {}

	return false;
}

void Dictionary::check( const char *filename ) {
	vector<string> query;
	getWords(filename, query);

	cerr << "checking " << filename << endl;
	start_timer();  // from elapsed_time.h

	int counter = 0;
	int n = query.size();
	for (int i = 0; i < n; i++) {
		if ( !inWordArray(query[i]) ) {
			counter++;
		}
	}

	cerr << "Misspelled " << counter << " words." << endl;

	double cycles = elapsed_time();
	cerr << "Total cycles: " << cycles << endl;
}

int main(int argc, char **argv) {
	if ( argc != 3 ) {
		cerr << "Usage: spellCheck dictionaryFile inputFile" << endl;
		exit(0);
	}
	Dictionary d(argv[1]);
	d.check(argv[2]);
}
