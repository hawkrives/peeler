#ifndef  __common__
#define  __common__

#include <vector>
#include <array>
#include <string>
#include "elapsed_time.h"
#include "overrides.h"

int nextPrime = 500009;
array<int, 4> seeds = {{216, 0, 9, 111}};

template<typename T>
int sum(vector<T> &vals) {
	int sum = 0;
	size_t size = vals.size();
	for (size_t i = 0; i < size; i++) {
		sum += vals[i];
	}
	return sum;
}

template<typename T>
int average(vector<T> &vals) {
	if (vals.size() <= 0)
		return 0;
  return sum(vals) / vals.size();
}

int asciiify(string input) {
	size_t ascii = 0;
	for (char ch : input) {
		ascii += ch;
	}
	return ascii;
}

void getWords(const char *filename, vector<string> &vec) {
	ifstream f(filename);
	if ( ! f.good() ) {
		cerr << "Error:  unable to open " << filename << endl;
		exit(-1);
	}
	string s;
	cerr << "reading/hashing file" << endl;
	while ( f >> s ) {
		vec.push_back(s);
	}
	cerr << "done with file; " << vec.size() << " items." << endl;
}

void getWords(string &filename, vector<string> &vec) {
	getWords(filename.c_str(), vec);
}

#endif
