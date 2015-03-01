#include <iostream>
#include <fstream>
#include <vector>
using namespace std;
#include "elapsed_time.h"

struct Dictionary {
	vector<string> wordArray;   // store the data

	Dictionary(const char *filename);  // constructor

	bool inWordArray(string &s);       // single query
	void check(const char *filename);  // multiple queries
};

void getWords( const char *filename, vector<string> &vec ) {
	ifstream f(filename);
	if ( ! f.good() ) {
		cerr << "Error:  unable to open " << filename << endl;
		exit(-1);
	}
	string s;
	while ( f >> s ) vec.push_back(s);
}

Dictionary::Dictionary( const char *filename ) {
	getWords(filename, wordArray);
}

bool Dictionary::inWordArray(string &s) {
	int n = wordArray.size();
	for ( int i = 0; i < n; ++i )
		if ( s == wordArray[i] ) return true;
	return false;
}

void Dictionary::check( const char *filename ) {
	vector<string> query;
	getWords(filename, query);

	start_timer();  // from elapsed_time.h

	int counter = 0, n = query.size();
	for ( int i = 0; i < n; ++i ) {
		if ( ! inWordArray(query[i]) ) {
			// cout << query[i] << " ";
			++counter;
		}
	}
	cout << "Misspelled " << counter << " words." << endl;

	double cycles = elapsed_time();
	cout << " Total cycles: " << cycles << endl;
}

int main(int argc, char **argv) {
	if ( argc != 3 ) {
		cerr << "Usage: spellCheck dictionaryFile inputFile" << endl;
		exit(0);
	}
	Dictionary d(argv[1]);
	d.check(argv[2]);
}
