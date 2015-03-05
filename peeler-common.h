#ifndef  __common__
#define  __common__

#include <vector>
#include <numeric>
#include <array>
#include <string>
#include "elapsed_time.h"
#include "overrides.h"
#include <time.h>
#include <stdlib.h>

int nextPrime = 500009;
array<int, 4> seeds = {{216, 0, 9, 111}};
array<int, 256> letterHashes;

template<typename T>
int sum(vector<T> &vals) {
	return accumulate(vals.begin(), vals.end(), 0);
}

template<typename T>
int average(vector<T> &vals) {
	if (vals.size() <= 0)
		return 0;
	return sum(vals) / vals.size();
}

int asciiify(string input) {
	// size_t ascii = 0;
	// for (char &ch : input) {
	// 	ascii += ch;
	// }
	// return ascii;
	return accumulate(input.begin(), input.end(), 0);
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

unordered_map<string, string> get_args(int argc, char **argv) {
	string dictionaryFile = argv[1];
	string inputFile = argv[2];
	unordered_map<string, string> args = {
		{"dictionaryFile", dictionaryFile}
	};

	for (int i = 0; i < argc; i++) {
		string arg = argv[i];
		if (arg == "-w" && i < argc - 1) {
			args.emplace("-w", argv[i+1]);
			inputFile = "";
			i++;
			continue;
		}
		else if (arg == "-f" && i < argc - 1) {
			inputFile = argv[i+1];
			i++;
			continue;
		}
	}

	args.emplace("inputFile", inputFile);

	return args;
}

void initLetterHashes() {
	for (int i = 0; i < 256; ++i)
		letterHashes[i] = int(rand() / (1.0 + RAND_MAX) * nextPrime);
}

#endif
