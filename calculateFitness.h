#include <cstdio>

int calculateFitness(int *Food)
{
    int count = 0;
    for (int i = 0; i < n_citys; i++)
    {
        count = count + d[Food[i]][Food[i+1]];
    }
    return count;
}