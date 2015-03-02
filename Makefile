all: peeler

peeler: peeler.cpp elapsed_time.h
	g++ --std=c++11 peeler.cpp -o peeler

peeler-spreads: peeler-spreads.cpp elapsed_time.h
	g++ --std=c++11 peeler-spreads.cpp -o peeler-spreads

peeler-avg: peeler-avg.cpp elapsed_time.h
	g++ --std=c++11 peeler-avg.cpp -o peeler-avg

peeler-optim: peeler.cpp elapsed_time.h
	g++ -O9 --std=c++11 peeler.cpp -o peeler-o

node_modules/.bin/babel:
	npm install babel

node_modules/lodash/package.json:
	npm install lodash

peeler.js: peeler.es node_modules/lodash/package.json node_modules/.bin/babel
	node_modules/.bin/babel peeler.es > peeler.js

clean:
	rm -f peeler.o peeler peeler-o peeler-spreads peeler.js

js: peeler.js

run-cpp: peeler
	./peeler words moby

run-optim: peeler-optim
	./peeler-o words moby

run-js: peeler.js
	node ./peeler.js

run-py:
	python3 ./peeler.py

spread.txt: peeler-spreads
	./peeler-spreads words moby | sort -n

averages.txt: peeler-avg
	./peeler-avg words moby > avgs.txt
	awk -f ./average.awk < avgs.txt

data: spread.txt averages.txt


%PHONY: clean js
