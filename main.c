#include <stdlib.h>
#include "printout.h"

int main(int argc, char *argv[]) {
    if(argc != 2)
    {
        run(0);
    }
    else{
        run(atoi(argv[1]));
    }
    return 0;
}
