#!/bin/awk -f
# input format: [01]\t[0-9]+ (miss or hit, tab, cycle count)

BEGIN { \
  hit = 0; \
  miss = 0; \
  hit_cycles = 0; \
  miss_cycles = 0; \
}

/^0/ { \
  miss += 1; \
  miss_cycles += $2; \
}

/^1/ { \
  hit += 1; \
  hit_cycles += $2; \
}

END { \
  printf("hits:%d\ncycles:%d\naverage:%f\n", hit, hit_cycles, hit_cycles / hit); \
  printf("miss:%d\ncycles:%d\naverage:%f\n", miss, miss_cycles, miss_cycles / miss); \
}
