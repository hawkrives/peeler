#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <array>
#include <cstdlib>
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
template<typename T>
ostream& operator<< (ostream& out, const array<T, 4>& v) {
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

int nextPrime = 500009;
array<int, 4> seeds;
void make_random() {
	int min = 0;
	int max = nextPrime;
	seeds[0] = (rand() / (1.0 + RAND_MAX)) * max + min;
	seeds[1] = (rand() / (1.0 + RAND_MAX)) * max + min;
	seeds[2] = (rand() / (1.0 + RAND_MAX)) * max + min;
	seeds[3] = (rand() / (1.0 + RAND_MAX)) * max + min;
	// cout << seeds << endl;
	// std::generate(seeds.begin(), seeds.end(), [min, max]{ return (rand() / (1.0 + RAND_MAX)) * max + min; });
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

void getWords(const char *filename, vector<string> &vec, unordered_map<int, vector<string> > &map) {
	ifstream f(filename);
	if ( !f.good() ) {
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
}

void Dictionary::check( const char *filename ) {
	vector<string> query;
	getWords(filename, query);

	cout << "checking " << filename << endl;
	start_timer();  // from elapsed_time.h

	int counter = 0, n = query.size();
	for ( int i = 0; i < n; ++i ) {
		if ( !inWordArray(query[i]) ) {
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
	// make_random();
	seeds = {216, 0, 9, 111};
	Dictionary d(argv[1]);
	d.check(argv[2]);
}
