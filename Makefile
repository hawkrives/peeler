all: both

COMMONS := peeler-common.h overrides.h elapsed_time.h
OBJECTS := peeler peeler-spreads peeler-avg peeler-multimap peeler-list
INPUTS  := $(OBJECTS:.cpp)

CC := g++
CFLAGS := --std=c++11

peeler: peeler.cpp $(COMMONS)
	$(CC) $(CFLAGS) $< -o $@

peeler-spreads: peeler-spreads.cpp $(COMMONS)
	$(CC) $(CFLAGS) $< -o $@

peeler-avg: peeler-avg.cpp $(COMMONS)
	$(CC) $(CFLAGS) $< -o $@

peeler-multimap: peeler-multimap.cpp $(COMMONS)
	$(CC) $(CFLAGS) $< -o $@

peeler-optim: peeler.cpp $(COMMONS)
	$(CC) -O9 $(CFLAGS) $< -o $@


clean:
	rm -f $(OBJECTS)


run: peeler
	./peeler words moby

time: peeler
	time ./peeler words moby

data: peeler
	rm log
	for i in `seq 1 5`; do \
		time ./peeler words moby | grep -v "time ./peeler words moby" >> log; \
	done

both: peeler peeler-optim
	time ./peeler words moby
	time ./peeler-optim words moby


vector.reserve: vector.reserve.cpp
	$(CC) $(CFLAGS) $< -o $@

%PHONY: clean data time run both
