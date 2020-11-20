#include "../include/details.h"

// Reads arguments from command line
struct Details *read_arguments(int argc, char** argv){
    struct Details *temp = malloc(sizeof(struct Details));
    int round_robin = 0;
    int unlimited = 0;
    for(int i =0; i < argc; i++){
        if(!strcmp (argv[i], FILENAME_FLAG)){
            temp->filename = argv[++i];
        } else if (!strcmp (argv[i], S_ALGO_FLAG)){
            temp->s_algo = argv[++i];
            if(!strcmp (temp->s_algo, ROUND_ROBIN)){
                round_robin = 1;
                temp->quantum = DEFAULT_QUANTUM;
            }
        } else if (!strcmp (argv[i], M_ALGO_FLAG)){
            temp->m_algo = argv[++i];
            
            if(!strcmp (temp->m_algo, UNLIMITED)){
                unlimited = 1;
            }
        } else if (!unlimited && !strcmp (argv[i], MEMSIZE_FLAG)){
            temp->memsize = atoll(argv[++i]);
        } else if (round_robin && !strcmp (argv[i], QUANTUM_FLAG)){
            temp->quantum = atoll(argv[++i]);
        } 
    }
    return temp;
}