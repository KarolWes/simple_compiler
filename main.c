#include <stdlib.h>
#include "printout.h"



int main(int argc, char *argv[]) {
    printf("Main, hello\n");
    if(argc != 2)
    {
        printf("running\n");
        run(0);
    }
    else{
        printf("running\n");
        run(atoi(argv[1]));
    }
    return 0;
}
