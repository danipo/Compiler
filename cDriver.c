//COP3402 Project 3

//Daniel Powley, Ming Jiang
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "share.h"
#include "Parser.h"
#include "Analyzer.h"
#include "PMO.h"



int main(int argc, char **argv)
{


  //  char b, c, d, e;
/*
    if(argc == 1)
    {
        while(!feof(ifp))
        {
            b = fgets(ifp);
            printf("%c", b);
        }
        VMU();
    }

    else{*/

        int i;
        for(i=0; i<argc; i++)
            {

            if(strcmp(argv[i],"-l") == 0)
            {
            print_analyzer(1);
            }

            if(strcmp(argv[i],"-a") == 0)
            {
            print_parser(1);
            }

            if(strcmp(argv[i],"-v") == 0)
            {
            print_pmo(1);
            }

            }
            analyzer();
            parser();
            PMO();
    //}

}
