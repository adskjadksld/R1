#include <cstdio>
#include <time.h>
#include <stdlib.h>

int time_LLH = time(0);
int seed_LLH = time_LLH * 100;

void update_time_LLH();
void reverse(int *list, int len);
int rand_area(int r_min, int r_max);

//随机选取一段子序列并将其倒置（子序列还位于其原来的位置）
int LLH_rrs(int *food)
{
    srand(new_seed());

    int *rand_2 = rand_area_perm(1, n_citys-1, 2);
    int *sub_list = sublist(food, rand_2[0], rand_2[1] + 1);
    reverse(sub_list, rand_2[1] - rand_2[0] + 1);
    list_cpy(sub_list, &food[rand_2[0]], rand_2[1] - rand_2[0] + 1);

    return calculateFitness(food);
}

//在原序列中随机选取一个点，将其抽离出来后再插入到原序列中任意位置（除第一个位置前与最后一个位置后）
int LLH_ri(int *food)
{
    srand(new_seed());
    int r = rand_area(1, n_citys-1);
    int position = rand_area(1, n_citys-2);
    int temp = food[r];

    if (position >= r)
    {
        for (int i = 0; i < position - r + 1; i++)
        {
            food[r + i] = food[r + i + 1];
        }
        food[position + 1] = temp;
    }
    else
    {
        for (int i = 0; i < r - position;  i++)
        {
            food[r - i] = food[r - i -1];
        }
        food[position] = temp;
    }

    return calculateFitness(food);
}

//随机选取三个城市x1、x2、x3（在城市序列中按从左到右排列），
//再将x1与x2之间的子序列（包含x1但不包含x2）和x2与x3之间的子序列（包含x2且包含x3）进行交换
int LLH_ris(int *food)
{
    srand(new_seed());
    int *rand_3 = rand_area_perm(1, n_citys-1, 3);
    int *temp = (int *)malloc(sizeof(int) * (rand_3[1] - rand_3[0]));
    list_cpy(&food[rand_3[0]], temp, rand_3[1] - rand_3[0]);
    list_cpy(&food[rand_3[1]], &food[rand_3[0]], rand_3[2] - rand_3[1] + 1);
    list_cpy(temp, &food[rand_3[0] + rand_3[2] - rand_3[1] + 1], rand_3[1] - rand_3[0]);
    free(temp);

    return calculateFitness(food);
}

//在原序列中随机选取两个点并交换这两个点
int LLH_rs(int *food)
{
    srand(new_seed());
    int *rand_2 = rand_area_perm(1, n_citys-1, 2);

    int temp = food[rand_2[1]];
    food[rand_2[1]] = food[rand_2[0]];
    food[rand_2[0]] = temp;

    return calculateFitness(food);
}

//随机选取两段不相交的子序列，并进行交换
int LLH_rss(int *food)
{
    srand(new_seed());
    int *rand_4 = rand_area_perm(1, n_citys-1, 4);

    int *temp_1 =  (int *)malloc(sizeof(int) * (rand_4[1] - rand_4[0] + 1));
    int *temp_2 =  (int *)malloc(sizeof(int) * (rand_4[2] - rand_4[1] - 1));

    list_cpy(&food[rand_4[0]], temp_1, rand_4[1] - rand_4[0] + 1);
    list_cpy(&food[rand_4[1] + 1], temp_2, rand_4[2] - rand_4[1] - 1);
    list_cpy(&food[rand_4[2]], &food[rand_4[0]], rand_4[3] - rand_4[2] + 1);
    list_cpy(temp_2, &food[rand_4[0] + rand_4[3] - rand_4[2] + 1], rand_4[2] - rand_4[1] - 1);
    list_cpy(temp_1, &food[rand_4[0] + rand_4[3] - rand_4[1]], rand_4[1] - rand_4[0] + 1);

    free(temp_1);
    free(temp_2);

    return calculateFitness(food);
}

//随机一段子序列，将其进行打乱重组
int LLH_ss(int *food)
{
    my_srand(new_seed());                                          
    int *rand_2 = my_rand_area_perm(1, n_citys-1, 2);              
    int *rand_perm = randperm(rand_2[1] - rand_2[0] + 1);        
    int *temp = (int *)malloc(sizeof(int) * (rand_2[1] - rand_2[0] + 1)); 
    for (int i = 0; i < rand_2[1] - rand_2[0] + 1; i++)
    {
        temp[i] = food[rand_2[0] + rand_perm[i]];
    }
    
    list_cpy(temp, &food[rand_2[0]], rand_2[1] - rand_2[0] + 1);
    free(rand_perm);free(rand_2);free(temp);

    return calculateFitness(food);
}

//随机选取一段子序列将其抽离出来，再将其倒置后插入剩下的原序列中的任意一个位置（除第一个位置前与最后一个位置后）
int LLH_rris(int *food)
{
    // printf("entering LLH\n");
    my_srand(my_seed);
    int *rand_2 = my_rand_area_perm(1, n_citys-1, 2);
    while (n_citys-rand_2[1]+rand_2[0]-2 == 0)
    {
        rand_2 = my_rand_area_perm(1, n_citys-1, 2);
    }
    int rand_position = my_rand_area(1, n_citys-rand_2[1]+rand_2[0]-2);
    // printf("%d %d %d\n", rand_2[0], rand_2[1], rand_position);
    reverse(&food[rand_2[0]], rand_2[1] - rand_2[0] + 1);
    // printf("LLL\n");
    int *temp;
    if (rand_position < rand_2[0])
    {
        temp = sublist(food, rand_position, rand_2[0]);
        list_cpy(&food[rand_2[0]], &food[rand_position], rand_2[1] - rand_2[0] + 1);
        list_cpy(temp, &food[rand_position + rand_2[1] - rand_2[0] + 1], rand_2[0] - rand_position);
    }
    else
    {
        temp = sublist(food, rand_2[0], rand_2[1] + 1);
        list_cpy(&food[rand_2[1] + 1], &food[rand_2[0]], rand_position - rand_2[0] + 1);
        list_cpy(temp, &food[rand_position + 1], rand_2[1] - rand_2[0] + 1);
    }
    // printf("kkk");
    free(temp);

    return calculateFitness(food);
}

//随机选取两段不相交的子序列，并进行交换，对于每条子序列有0.5的概率会被倒置
int LLH_rrss(int *food)
{
    my_srand(new_seed());
    int *rand_4 = my_rand_area_perm(1, n_citys-1, 4);
    int rand1, rand2;

    rand1 = my_rand_area(1, 100);
    rand2 = my_rand_area(1, 100);

    if (rand1 > 50)
    {
        reverse(&food[rand_4[0]], rand_4[1] - rand_4[0] + 1);
    }
    if (rand2 > 50)
    {
        reverse(&food[rand_4[2]], rand_4[3] - rand_4[2] + 1);
    }

    // printf("%d %d\n", rand1, rand2);
    // printf("%d %d %d %d\n", rand_4[0], rand_4[1], rand_4[2], rand_4[3]);
    

    int *temp_1 =  (int *)malloc(sizeof(int) * (rand_4[1] - rand_4[0] + 1));
    int *temp_2 =  (int *)malloc(sizeof(int) * (rand_4[2] - rand_4[1] - 1));

    list_cpy(&food[rand_4[0]], temp_1, rand_4[1] - rand_4[0] + 1);
    list_cpy(&food[rand_4[1] + 1], temp_2, rand_4[2] - rand_4[1] - 1);
    list_cpy(&food[rand_4[2]], &food[rand_4[0]], rand_4[3] - rand_4[2] + 1);
    list_cpy(temp_2, &food[rand_4[0] + rand_4[3] - rand_4[2] + 1], rand_4[2] - rand_4[1] - 1);
    list_cpy(temp_1, &food[rand_4[0] + rand_4[3] - rand_4[1]], rand_4[1] - rand_4[0] + 1);

    free(temp_1);
    free(temp_2);

    return calculateFitness(food);
}

//随机选取一段子序列，将其抽出并打乱重组，再插入到原序列任意位置（除第一个位置前与最后一个位置后）
int LLH_rsis(int *food)
{
    my_srand(my_seed);
    int *rand_2 = my_rand_area_perm(1, n_citys-1, 2);
    while (n_citys-rand_2[1]+rand_2[0]-2 == 0)
    {
        rand_2 = my_rand_area_perm(1, n_citys-1, 2);
    }
    int rand_position = my_rand_area(1, n_citys-rand_2[1]+rand_2[0]-2);

    // printf("%d %d\n", rand_2[0], rand_2[1]);
    // printf("%d\n", rand_position);

    //打乱顺序
    int *rand_perm = randperm(rand_2[1] - rand_2[0] + 1);
    int *temp2 = (int *)malloc(sizeof(int) * (rand_2[1] - rand_2[0] + 1));


    for (int i = 0; i < rand_2[1] - rand_2[0] + 1; i++)
    {
        temp2[i] = food[rand_2[0] + rand_perm[i]];
    }
    
    list_cpy(temp2, &food[rand_2[0]], rand_2[1] - rand_2[0] + 1);
    free(temp2);

    //插入到随机位置
    int *temp;
    if (rand_position < rand_2[0])
    {
        temp = sublist(food, rand_position, rand_2[0]);
        list_cpy(&food[rand_2[0]], &food[rand_position], rand_2[1] - rand_2[0] + 1);
        list_cpy(temp, &food[rand_position + rand_2[1] - rand_2[0] + 1], rand_2[0] - rand_position);
    }
    else
    {
        temp = sublist(food, rand_2[0], rand_2[1] + 1);
        list_cpy(&food[rand_2[1] + 1], &food[rand_2[0]], rand_position - rand_2[0] + 1);
        list_cpy(temp, &food[rand_position + 1], rand_2[1] - rand_2[0] + 1);
    }
    free(temp);

    return calculateFitness(food);
}

//随机选取两段不相交的子序列，并进行交换，对于每条子序列有0.5的概率会被打乱重组
int LLH_rsss(int *food)
{
    srand(new_seed());
    int *rand_4 = rand_area_perm(1, n_citys-1, 4);
    int rand1, rand2;

    rand1 = rand_area(1, 100);
    rand2 = rand_area(1, 100);

    //按照概率打乱数组
    if (rand1 > 50)
    {
        int *rand_perm = randperm(rand_4[1] - rand_4[0] + 1);
        int *temp2 = (int *)malloc(sizeof(int) * (rand_4[1] - rand_4[0] + 1));
        for (int i = 0; i < rand_4[1] - rand_4[0] + 1; i++)
        {
            temp2[i] = food[rand_4[0] + rand_perm[i]];
        }
        list_cpy(temp2, &food[rand_4[0]], rand_4[1] - rand_4[0] + 1);
        free(temp2);
    }
    if (rand2 > 50)
    {
        int *rand_perm = randperm(rand_4[3] - rand_4[2] + 1);
        int *temp2 = (int *)malloc(sizeof(int) * (rand_4[3] - rand_4[2] + 1));
        for (int i = 0; i < rand_4[3] - rand_4[2] + 1; i++)
        {
            temp2[i] = food[rand_4[2] + rand_perm[i]];
        }
        list_cpy(temp2, &food[rand_4[2]], rand_4[3] - rand_4[2] + 1);
        free(temp2);
    }

    // printf("%d %d\n", rand1, rand2);
    // printf("%d %d %d %d\n", rand_4[0], rand_4[1], rand_4[2], rand_4[3]);
    

    //交换数组
    int *temp_1 =  (int *)malloc(sizeof(int) * (rand_4[1] - rand_4[0] + 1));
    int *temp_2 =  (int *)malloc(sizeof(int) * (rand_4[2] - rand_4[1] - 1));

    list_cpy(&food[rand_4[0]], temp_1, rand_4[1] - rand_4[0] + 1);
    list_cpy(&food[rand_4[1] + 1], temp_2, rand_4[2] - rand_4[1] - 1);
    list_cpy(&food[rand_4[2]], &food[rand_4[0]], rand_4[3] - rand_4[2] + 1);
    list_cpy(temp_2, &food[rand_4[0] + rand_4[3] - rand_4[2] + 1], rand_4[2] - rand_4[1] - 1);
    list_cpy(temp_1, &food[rand_4[0] + rand_4[3] - rand_4[1]], rand_4[1] - rand_4[0] + 1);

    free(temp_1);
    free(temp_2);

    return calculateFitness(food);
}

void update_time_LLH()
{
    time_LLH = time(0);
}


