#include <cstdio>
#include <stdlib.h>
#include <time.h>

int *randperm(int num)
{
    int *a = (int *)malloc(sizeof(int) * (num+1));
    int b[num+1];

    for(int i=0;i<num;i++)
        b[i]=i;

    srand(new_seed());
    for(int i=0;i<=num;i++)
    {
        rand();//必须消耗掉第一个rand()
        int j=(int) ((float) ((num-i)*rand())/(RAND_MAX+1.0));
        a[i]=b[j];
        b[j]=b[num-1-i];
    }
    return a;
}

