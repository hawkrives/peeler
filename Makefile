all: run

COMMONS := peeler-common.h overrides.h elapsed_time.h
OBJECTS := peeler peeler-optim peeler-multimap peeler-list
INPUTS  := $(OBJECTS:.cpp)

CC := g++
CFLAGS := --std=c++11

peeler: peeler.cpp $(COMMONS)
	$(CC) $(CFLAGS) $< -o $@

peeler-list: peeler-list.cpp $(COMMONS)
	$(CC) $(CFLAGS) $< -o $@

peeler-multimap: peeler-multimap.cpp $(COMMONS)
	$(CC) $(CFLAGS) $< -o $@

peeler-optim: peeler.cpp $(COMMONS)
	$(CC) -O9 $(CFLAGS) $< -o $@


clean:
	rm -f $(OBJECTS) vector.reserve


run: peeler
	./peeler words moby

time: peeler
	time ./peeler words moby

data: peeler
	rm -f log
	for i in `seq 1 50`; do \
		./$< words moby | grep -v "./$< words moby" >> log; \
	done

data-o: peeler-optim
	rm -f log
	for i in `seq 1 50`; do \
		./$< words moby | grep -v "./$< words moby" >> log; \
	done

data-ana: peeler
	rm -f log
	for i in `seq 1 50`; do \
		./$< words -w Hamito-Semitic | grep -v "./$< words -w Hamito-Semitic" >> anagrams.log; \
	done


both: peeler peeler-optim
	./peeler words -f moby
	./peeler-optim words -f moby


ana: peeler
	./peeler words moby --count-anagrams

ana-v: peeler
	./peeler words moby --count-anagrams --verbose

anagrams.txt:
	python anagrams_counter.py < words > anagrams.txt

anagram-count: anagrams.txt
	tail -n 1 < anagrams.txt

vector.reserve: vector.reserve.cpp
	$(CC) $(CFLAGS) $< -o $@

%PHONY: clean data time run both anagram-count
