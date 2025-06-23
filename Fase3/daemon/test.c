struct Proc {
    int pid;
    char name[256];
    float mem_percentage;
};

struct PageFaults {
    int minor;
    int major;
};

struct AntivirusStats {
    int process_count;
    int files_scanned;
    int quarantined_files;
};

struct MemStats {
    unsigned long total;   // Total memory in MB
    unsigned long used;    // Used memory in MB
    unsigned long free;    // Free memory in MB
    unsigned long cached;  // Cached memory in MB
    unsigned long swapped; // Swapped memory in MB
};

struct PagesStats {
    int active;
    int inactive;
};

struct MemStats get_memory_stats() {
    struct MemStats stats;
    // Simulate memory stats
    stats.total = 16000; // in MB
    stats.used = 8000;   // in MB
    stats.free = 4000;   // in MB
    stats.cached = 2000; // in MB
    stats.swapped = 1000; // in MB
    return stats;
}