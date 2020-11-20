/* 
 * Author: Garvin Prathama Bulkin
 * Student ID: 927842 (gbulkin)
 * Purpose: Header file for details struct, please refer to .c file for
 *          documentation of each function.
 */

#ifndef SYSTEM_H
#define SYSTEM_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// Flags for input arguments
#define FILENAME_FLAG "-f"
#define S_ALGO_FLAG "-a"
#define M_ALGO_FLAG "-m"
#define MEMSIZE_FLAG "-s"
#define QUANTUM_FLAG "-q"

// String arguments for algorithms
#define FIRST_SERVE "ff"
#define ROUND_ROBIN "rr"
#define UNLIMITED "u"
#define BYO_M "cm"
#define BYO_S "cs"

#define DEFAULT_QUANTUM 10

// Struct path to store all given arguments
struct Details{
    char *filename;
    char *s_algo;
    char *m_algo;
    long long int memsize;
    int quantum;
};


#endif