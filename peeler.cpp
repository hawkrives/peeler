#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <array>
using namespace std;
#include "elapsed_time.h"

template<typename T>
ostream& operator<< (ostream& out, const vector<T>& v) {
	out << "[";
	size_t last = v.size() - 1;
	for(size_t i = 0; i < v.size(); ++i) {
		out << v[i];
		if (i != last)
			out << ", ";
	}
	out << "]";
	return out;
}

vector<int> seed(size_t max) {
	vector<int> vals = {216, 0, 9, 111};
	return vals;
}

int sum(vector<int> vals) {
	int sum = 0;
	size_t size = vals.size();
	for (size_t i = 0; i < size; i++) {
		sum += vals[i];
	}
	return sum;
}

int asciiify(string input) {
	size_t ascii = 0;
	for (char ch : input) {
		ascii += ch;
	}
	return ascii;
}

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

	int m = 500009;
	array<int, 4> seeds = {216, 0, 9, 111};

	int hash1 = asciiify(str1) * seeds[0];
	int hash2 = asciiify(str2) * seeds[1];
	int hash3 = asciiify(str3) * seeds[2];
	int hash4 = asciiify(str4) * seeds[3];

	int hash_sum = hash1 + hash2 + hash3 + hash4;

	return hash_sum % m;
}

struct Dictionary {
	vector<string> wordArray;   // store the data
	unordered_map<int, vector<string> > hashTable;

	Dictionary(const char *filename);  // constructor

	bool inWordArray(string &s);       // single query
	void check(const char *filename);  // multiple queries
};

void getWords(const char *filename, vector<string> &vec, unordered_map<int, vector<string> > &map) {
	ifstream f(filename);
	if ( ! f.good() ) {
		cerr << "Error:  unable to open " << filename << endl;
		exit(-1);
	}
	string s;
	cout << "reading/hashing file" << endl;
	while ( f >> s ) {
		vec.push_back(s);
		int hash = hash_string(s);
		try {
			vector<string> &current = map.at(hash);
			current.push_back(s);
		} catch (...) {
			vector<string> strings = {s};
			map.insert(make_pair(hash, strings));
		}
	}
	cout << "done with file" << endl;
}

void getWords(const char *filename, vector<string> &vec) {
	ifstream f(filename);
	if ( ! f.good() ) {
		cerr << "Error:  unable to open " << filename << endl;
		exit(-1);
	}
	string s;
	cout << "reading/hashing file" << endl;
	while ( f >> s ) {
		vec.push_back(s);
	}
	cout << "done with file" << endl;
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
				return true;
	} catch (...) {}

	return false;

	// int n = wordArray.size();
	// for ( int i = 0; i < n; ++i ) {
	// 	if ( s == wordArray[i] ) {
	// 		return true;
	// 	}
	// }
	//
	// return false;
}

void Dictionary::check( const char *filename ) {
	vector<string> query;
	getWords(filename, query);

	cout << "checking" << endl;
	start_timer();  // from elapsed_time.h

	int counter = 0, n = query.size();
	for ( int i = 0; i < n; ++i ) {
		if ( !inWordArray(query[i]) ) {
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

	// vector<string> test = quadrize("pneumonoultramicroscopicsilicovolcanoconiosis");
	// cout << test << endl;

	// vector<string> test2 = quadrize("aa");
	// cout << test2 << endl;

	// vector<int> summed = strs_to_ascii(test);
	// cout << summed << endl;

	// vector<int> summed2 = strs_to_ascii(test2);
	// cout << summed2 << endl;

	// size_t hash = hash_vector(summed);
	// cout << hash << endl;

	// cout << hash_string("pneumonoultramicroscopicsilicovolcanoconiosis") << endl;

	// size_t hash2 = hash_vector(summed2);
	// cout << hash2 << endl;

	// cout << hash_string("aa") << endl;
}
