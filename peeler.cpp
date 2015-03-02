#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <unordered_map>
using namespace std;
#include "elapsed_time.h"

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
	while ( f >> s ) {
		vec.push_back(s);
		int hash = hash_string(s);
		try {
			vector<string> *current = map.at(hash);
			current.push_back(s);
		} catch (int e) {
			vector<string> strings = {s};
			map.insert(make_pair(hash, strings));
		}
	}
}

Dictionary::Dictionary( const char *filename ) {
	getWords(filename, wordArray, hashTable);
}

bool Dictionary::inWordArray(string &s) {
	int hash = hash_string(*s);

	try {
		vector<string> possibilities = hashTable.at(hash);
		for (string str : possibilities)
			if (str == *s)
				return true;
	} catch (int err) {}

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
	unordered_map<int, vector<string> > queryHash;
	getWords(filename, query, queryHash);

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

vector<string> quadrize(string input) {
	size_t len = input.size();
	size_t quadtrant = len / 4;

	if (len < 4) {
		vector<string> output = {input, "", "", ""};
		return output;
	}

	string str1 = input.substr(quadtrant*0, quadtrant);
	string str2 = input.substr(quadtrant*1, quadtrant);
	string str3 = input.substr(quadtrant*2, quadtrant);
	string str4 = input.substr(quadtrant*3, len);

	vector<string> output = {str1, str2, str3, str4};

	return output;
}

vector<int> strs_to_ascii(vector<string> words) {
	vector<int> asciiified;
	for (string word : words) {
		size_t sum = 0;
		for (char ch : word) {
			sum += ch;
		}
		asciiified.push_back(sum);
	}
	return asciiified;
}

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
	for (int item : vals) {
		sum += item;
	}
	return sum;
}

size_t hash_vector(vector<int> vec) {
	int m = *max_element(vec.begin(), vec.end());
	vector<int> seeds = seed(m);

	vector<int> hashed;
	for (int i = 0; i < vec.size(); i++) {
		hashed.push_back(vec.at(i) * seeds.at(i));
	}
	int hash_sum = sum(hashed);

	return hash_sum % m;
}

int main(int argc, char **argv) {
	// if ( argc != 3 ) {
	// 	cerr << "Usage: spellCheck dictionaryFile inputFile" << endl;
	// 	exit(0);
	// }
	// Dictionary d(argv[1]);
	// d.check(argv[2]);

	vector<string> test = quadrize("pneumonoultramicroscopicsilicovolcanoconiosis");
	cout << test << endl;

	vector<string> test2 = quadrize("aa");
	cout << test2 << endl;

	vector<int> summed = strs_to_ascii(test);
	cout << summed << endl;

	vector<int> summed2 = strs_to_ascii(test2);
	cout << summed2 << endl;

	size_t hash = hash_vector(summed);
	cout << hash << endl;

	size_t hash2 = hash_vector(summed2);
	cout << hash2 << endl;
}
