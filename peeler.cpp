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

size_t sum_vector(vector<string> words) {
	size_t sum = 0;
	for (string word : words) {
		for (char ch : word) {
			sum += ch;
		}
	}
	return sum;
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

	size_t hash = sum_vector(test);
	cout << hash << endl;

	size_t hash2 = sum_vector(test2);
	cout << hash2 << endl;
}
