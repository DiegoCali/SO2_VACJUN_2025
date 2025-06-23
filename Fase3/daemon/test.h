#ifndef TEST_H
#define TEST_H

struct MemStats {
    unsigned long total;
    unsigned long used;
    unsigned long free;
    unsigned long cached;
    unsigned long swapped;
};
struct MemStats get_memory_stats(void);

#endif 