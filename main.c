#include <stdlib.h>
#include "printout.h"

int main(int argc, char *argv[]) {
//    printf("Main, hello; argc: %d\n", argc);
//    if(argc != 2)
//    {
//        printf("running\n");
//        run(1);
//    }
//    else{
//        printf("running\n");
//        run(atoi(argv[1]));
//    }
int x=0;
int y=0;
int res = (x=y=2)+3*(x=5);
    printf("%d %d %d", x,y,res);
    return 0;
}
