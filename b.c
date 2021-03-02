#include <stdio.h>

struct point{
    int xa;
    int ya;
};

struct point origin, *pp;



int main(){
    pp = &origin;
    printf("origin is (%d, %d)\n", (*pp).xa, (*pp).ya);
    printf("origin is (%d, %d)\n", (pp)->xa, (pp)->ya);
    return 0;
}