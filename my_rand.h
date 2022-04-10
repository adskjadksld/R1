#include <cstdio>
#include <stdlib.h>
#include "math.h"


int my_seed = 3;
int my_rand_num = int(pow(2, 30));

void my_srand(int seed)
{
    my_seed = seed;
    my_rand_num = (my_seed * 1103515245 + 12345) % int(pow(2, 30));
}

int my_rand()
{
    if (my_rand_num == int(pow(2, 30)))
    {
        my_rand_num = (my_seed * 1103515245 + 12345) % int(pow(2, 30));
    }
    else
    {
        my_rand_num =  (my_rand_num * 1103515245 + 12345) % int(pow(2, 30));
    }
    return my_rand_num;
}

// 返回在[low, high]之间的一个随机整数， low、high均为整数
int my_rand_area(int low, int high)
{
    if (high < low)
    {
        printf("\nmy_rand_area: high < low!\n");
    }
    
    int rand = my_rand();
    rand = (rand + int(pow(2, 30))) % (high - low + 1) + low;

    return rand;
}

//随机生成一定范围内几个随机数, 并从大到小排列成数组输出, 包含min、max
int *my_rand_area_perm(int min, int max, int num)
{
    int *result = (int *)malloc(sizeof(int) * num);
    int count = 0;

    if (max - min + 1 < num)
    {
        printf("随机数数量超过随机产生的范围");
        return NULL;
    }
    while(count < num)
    {
        int temp = my_rand_area(min, max), i = 0, flag = 0;
        for (i = 0; i < count; i++)
        {
            if (result[i] > temp)
            {
                flag = 1;
                break;
            }
            else if (result[i] == temp)
            {
                break;
            }
        }
        if (count == 0 || flag == 1 || i >= count)
        {
            for (int j = count - 1; j >= i; --j)
            {
                result[j+1] = result[j];
            }
            result[i] = temp;
            count++;
        }
        
    }
    return result;
}