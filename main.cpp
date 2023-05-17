#include <cstdlib>
#include "stdio.h"

int main() {

    int a = 1.00;
    printf("%d", a);
    float num = -173.141586;
    int len = snprintf(NULL, 0, "%f", num);
    char* res = (char *)(malloc(len + 1));
    snprintf(res, len, "%f", num);
    printf("%s", res);


    return 0;
}
