#include "mining.h"

int main(int argc, char *argv[])
{
    int t = 0;
    int  i = 0, s = 0;

    t = atoi(argv[1]);

    for(i=0; i < atoi(argv[2]) ; i++)
    {
        s = mining(atoi(argv[3]),t);
        t = s;
    }

    return 0;
}