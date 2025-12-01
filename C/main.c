#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
int main(int argc, char *argv[])
{
    if (argc < 4 || argc > 8) {
        printf("Usage: %s -f fichier.tsp (-c | -m METHOD)\n"
            "\n"
            "METHOD:\n"
            "  bf | nn | rw | 2optrw | 2optnn\n"
            "  ga | gadxp [default | <population_size generations mutation_rate>]\n",
            argv[0]);    
       exit(1);
    }
    

    if (argc == 4 && (strcmp(argv[3], "-c") == 0)) {
        char* args[] = {"../exec/main0", argv[1], argv[2], argv[3], NULL};
        execvp(args[0], args);
        exit(0);
    }

    else if (argc == 5 && (strcmp(argv[3], "-m") == 0)){
        if (strcmp(argv[4], "bf") == 0)
        {
            char* args[] = {"../exec/main1", argv[1], argv[2], argv[3], argv[4], NULL};
            execvp(args[0], args);
            exit(0);
        }
        else if (strcmp(argv[4], "nn") == 0 || strcmp(argv[4], "rw") == 0 || strcmp(argv[4], "2optnn") == 0  || strcmp(argv[4], "2optrw") == 0 )
        {
            char* args[] = {"../exec/main2", argv[1], argv[2], argv[3], argv[4], NULL};
            execvp(args[0], args);
            exit(0);
        }
    } 
    
    else if ((argc == 6 || argc == 8) && (strcmp(argv[4], "ga") == 0 || strcmp(argv[4], "gadxp")==0))
        {
            char* args[] = {"../exec/main3", argv[1], argv[2], argv[3], argv[4], argv[5], argv[6], argv[7], NULL};   //tsp_tri_light
            execvp(args[0], args);
            exit(0);
        }
       
    else 
    {
        printf("Usage: %s -f fichier.tsp (-c | -m METHOD)\n"
            "\n"
            "METHOD:\n"
            "  bf | nn | rw | 2optrw | 2optnn\n"
            "  ga | gadxp [default | <population_size generations mutation_rate>]\n",
            argv[0]);    
       exit(1);
    }
}