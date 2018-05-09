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
    int valid;          /* valid bit */
    long long tag;      /* tag */
    char *data;         /* cached data, requires malloc */
    unsigned long long time_updated;    /* Last update time for LRU */
} cache_block;


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


/* Simulated LRU */
void simulate(int *dest) {

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

    /* Check for mandatory options */
    if(globalArgs.s == -1 || globalArgs.E == -1 ||
            globalArgs.b == -1 || globalArgs.inputFile == NULL) {
        printf("%s: Missing required command line argument\n", argv[0]);
        print_usage();
        return 0;
    }

    int dest[3];
    simulate(dest);
    printSummary(dest[0], dest[1], dest[2]);
    return 0;
}
