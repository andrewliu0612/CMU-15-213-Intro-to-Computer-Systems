/*
 * csim.c - cache simulator
 * 
 * Zhaoning Kong
 * May 9th, 2018
 */


#include "cachelab.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>


/* Global argument storage and option string  */
struct globalArgs_t {
    int help;           /* -h option, optional */
    int verbosity;      /* -v option, optional */
    int s;              /* -s option */
    int E;              /* -E option */
    int b;              /* -b option */
    char *inputFile;    /* -t option */
} globalArgs;


/* Cache block */
typedef struct {
    int valid;          /* valid bit 0/1 */
    unsigned long tag;  /* tag */
    // char *data;         /* cached data, requires malloc */
    unsigned long time_updated;     /* Last update time for LRU */
} cache_block;

cache_block *cache;         /* Cache */
FILE *fp;                   /* Trace file */
unsigned long tag_mask = 0; /* Masks */
unsigned long set_mask = 0;
unsigned long offset_mask = 0;
enum { BUF_SIZE = 64 }; /* Buffer for input */


void print_usage() {
    puts("Usage: ./csim-ref [-hv] -s <num> -E <num> -b <num> -t <file>\n"
        "Options:\n"
        "  -h         Print this help message.\n"
        "  -v         Optional verbose flag.\n"
        "  -s <num>   Number of set index bits.\n"
        "  -E <num>   Number of lines per set.\n"
        "  -b <num>   Number of block offset bits.\n"
        "  -t <file>  Trace file.\n"
        "\n"
        "Examples:\n"
        "  linux>  ./csim -s 4 -E 1 -b 4 -t traces/yi.trace\n"
        "  linux>  ./csim -v -s 8 -E 2 -b 4 -t traces/yi.trace");
}


/* Cache initialization */
void cache_init() {
    /* Init cache as 1-d array */
    int num_blocks = (1 << globalArgs.s) * globalArgs.E * (1 << globalArgs.b);
    cache = (cache_block *)calloc(num_blocks, sizeof(cache_block));                
    /* Init file ptr */
    fp = fopen(globalArgs.inputFile, "r");
    /* Init masks */
    offset_mask = (1ULL << globalArgs.b) - 1;
    set_mask = ((1ULL << (globalArgs.b + globalArgs.s)) - 1) ^ offset_mask;
    tag_mask = 0xffffffffffffffff - offset_mask - set_mask;
}

unsigned long get_tag(unsigned long addr) {
    return (addr & tag_mask) >> (globalArgs.s + globalArgs.b);
}
unsigned long get_set(unsigned long addr) {
    return (addr & set_mask) >> globalArgs.b;
}

/* Access memory, hit->0, cold miss->1, miss->2*/
int access_mem(unsigned long addr, int size, int type, int time) {
    unsigned long tag = get_tag(addr);
    unsigned long set = get_set(addr);
    int first_empty = -1;       /* First empty slot */
    int lru_time = 0x7fffffff;  /* Last time victim used in this set */
    int lru_pos = -1;           /* Position of victim in this set */
    
    /* Loop through set */
    for(int i = 0; i < globalArgs.E; ++i) {
        /* Cache hit */
        if(cache[set * globalArgs.E + i].tag == tag && 
                cache[set * globalArgs.E + i].valid) {
            cache[set * globalArgs.E + i].time_updated = time;
            if(globalArgs.verbosity) {
                printf("hit ");
                /* If type is M, second access must hit */
                if(type == 'M')
                    printf("hit \n");
                else
                    printf("\n");
            }
            return 0;
        }
        /* Detect empty slot */
        else if(cache[set * globalArgs.E + i].valid == 0) {
            first_empty = i;
        }
        /* Find LRU slot */
        else if(cache[set * globalArgs.E + i].time_updated < lru_time) {
            lru_time = cache[set * globalArgs.E + i].time_updated;
            lru_pos = i;
        }
    }

    /* Cold miss */
    if(first_empty != -1) {
        cache[set * globalArgs.E + first_empty].valid = 1;
        cache[set * globalArgs.E + first_empty].tag = tag;
        cache[set * globalArgs.E + first_empty].time_updated = time;
        if(globalArgs.verbosity) {
            printf("miss ");
            /* If type is M, second access must hit */
            if(type == 'M')
                printf("hit \n");
            else
                printf("\n");
        }
        return 1;
    }
    /* Capacity miss, need eviction */
    else {
        cache[set * globalArgs.E + lru_pos].tag = tag;
        cache[set * globalArgs.E + lru_pos].time_updated = time;
        if(globalArgs.verbosity) {
            printf("miss eviction ");
            if(type == 'M') 
                printf("hit \n");
            else 
                printf("\n");
        }
        return 2;
    }
}

/* Simulated LRU */
void run(int *dest) {
    char buf[BUF_SIZE];         /* Input buffer */
    unsigned long addr;         /* Address accessed */
    int size;                   /* Size of access */
    char type;                  /* Type of access */
    int time = 0;               /* Time for LRU */
    
    while(fgets(buf, BUF_SIZE, fp)) {
        /* Ignore instruction cache */
        if(buf[0] == 'I') 
            continue;
        /* Parse line */
        int delimiter = 0;
        while(buf[delimiter] != ',')
            delimiter++;
        buf[delimiter] = 0;
        type = buf[1];
        addr = strtol(buf + 3, NULL, 16);
        size = atoi(buf + delimiter + 1);
        if(globalArgs.verbosity) {
            printf("%c %lx,%d ",type, addr, size);
        }
        switch(access_mem(addr, size, type, time++)) {
            case 0:
                dest[0]++;
                break;
            case 2:
                dest[2]++;
                /* Fall through */
            case 1:
                dest[1]++;
        }
        /* If type is M, second ref must be hit */
        if(type == 'M')
            dest[0]++;
    }
}

/* Termination */
void cache_exit() {
    free(cache);
    fclose(fp);
}


int main(int argc, char **argv) {
    
    /* Initialization */
    int opt = 0;
    globalArgs.help = 0;        /* Optional */
    globalArgs.verbosity = 0;   /* Optional */
    globalArgs.s = -1;
    globalArgs.E = -1;
    globalArgs.b = -1;
    globalArgs.inputFile = NULL;

    while((opt = getopt(argc, argv, "hvs:E:b:t:")) != -1) {
        switch(opt) {
            case 'v':
                globalArgs.verbosity = 1;
                break;
            case 's':
                globalArgs.s = atoi(optarg);
                break;
            case 'E':
                globalArgs.E = atoi(optarg);
                break;
            case 'b':
                globalArgs.b = atoi(optarg);
                break;
            case 't':
                globalArgs.inputFile = optarg;
                break;
            case '?':
                /* Fall through */
            case 'h':
                print_usage();
                return 0;
            default:
                /* You won't actually get here. */
                break;
        }
    }
 
    /* Check for mandatory options and constrains */
    if(globalArgs.s <= 0 || globalArgs.E <= 0 ||
            globalArgs.b <= 0 || globalArgs.inputFile == NULL) {
        printf("%s: Missing required command line argument\n", argv[0]);
        print_usage();
        return 0;
    }

    int dest[3] = {0};      /* Hit, miss and evic count */
    cache_init();
    run(dest);
    cache_exit();
    printSummary(dest[0], dest[1], dest[2]);
    return 0;
}