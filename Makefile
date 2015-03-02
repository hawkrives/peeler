peeler: peeler.cpp elapsed_time.h
	g++ --std=c++11 peeler.cpp -o peeler

js: peeler.es
	./node_modules/.bin/babel peeler.es > peeler.js

clean:
	rm -f peeler.o peeler peeler.js

%PHONY: clean
