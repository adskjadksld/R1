#include <cstdio>


//4_opt断点连接顺序
int four_opt_order[31][6] = {2, 1, 3, 4, 6, 5,   2, 1, 4, 3, 6, 5,   2, 1, 5, 6, 3, 4,   2, 1, 6, 5, 3, 4,   2, 1, 5, 6, 4, 3,
                             3, 4, 1, 2, 5, 6,   3, 4, 1, 2, 6, 5,   3, 4, 2, 1, 5, 6,   3, 4, 2, 1, 6, 5,   3, 4, 5, 6, 1, 2,   3, 4, 5, 6, 2, 1,   3, 4, 6, 5, 1, 2,   3, 4, 6, 5, 2, 1,
                             4, 3, 1, 2, 5, 6,   4, 3, 1, 2, 6, 5,   4, 3, 5, 6, 1, 2,   4, 3, 5, 6, 2, 1,   4, 3, 6, 5, 1, 2,   4, 3, 6, 5, 2, 1, 
                             5, 6, 1, 2, 4, 3,   5, 6, 2, 1, 3, 4,   5, 6, 2, 1, 4, 3,   5, 6, 3, 4, 1, 2,   5, 6, 3, 4, 2, 1,   5, 6, 4, 3, 1, 2, 
                             6, 5, 1, 2, 4, 3,   6, 5, 2, 1, 3, 4,   6, 5, 2, 1, 4, 3,   6, 5, 3, 4, 1, 2,   6, 5, 3, 4, 2, 1,   6, 5, 4, 3, 1, 2, };

//3_opt断点连接顺序
int three_opt_order[4][4] = {2, 1, 4, 3,   3, 4, 1, 2,   3, 4, 2, 1,   4, 3, 1, 2}; //1 2,  3 4

//不连续3_opt断点连接顺序
int discontinuous_three_opt_order[8][10] = {1, 3, 8, 9, 6, 4, 5, 2, 7, 10,   1, 6, 9, 8, 3, 5, 4, 2, 7, 10,   
                                            1, 4, 5, 3, 8, 9, 6, 2, 7, 10,   1, 5, 4, 6, 9, 8, 3, 2, 7, 10,
                                            7, 10, 1, 3, 8, 9, 6, 4, 5, 2,   7, 10, 1, 6, 9, 8, 3, 5, 4, 2, 
                                            7, 10, 1, 4, 5, 3, 8, 9, 6, 2,   7, 10, 1, 5, 4, 6, 9, 8, 3, 2};

int two_opt_LLH(int *food) //颠倒子段
{
    int old_cost, new_cost, start_point;
    int *temp_food = (int *)malloc(sizeof(int) * (n_citys + 1));

    old_cost = calculateFitness(food);
    // srand(new_seed());
    start_point = rand_area(1, n_citys-1);
    list_cpy(food, temp_food, n_citys + 1);

    for (int i = 1; i < n_citys; i++)
    {
        //正方向
        if (start_point + i < n_citys)
        {
            reverse(&temp_food[start_point], i + 1);
        }
        new_cost = calculateFitness(temp_food);
        if (new_cost < old_cost)
        {

            list_cpy(temp_food, food, n_citys+1);
            //free(temp_food);
            old_cost = new_cost;
            //return new_cost;
        }
        
        //反方向    
        if (start_point - i > 0)
        {
            reverse(&temp_food[start_point - i], i + 1);
        }
        new_cost = calculateFitness(temp_food);
        if (new_cost < old_cost)
        {
            list_cpy(temp_food, food, n_citys+1);
            //free(temp_food);
            old_cost = new_cost;
            //return new_cost;
        }

        list_cpy(food, temp_food, n_citys+1);
    }
    free(temp_food);
    return old_cost;
}

int three_opt(int *food)
{
    int old_cost, new_cost, start_point1, start_point2, point_order[4] = {0};
    int *temp_food = (int *)malloc(sizeof(int) * (n_citys + 1));

    old_cost = calculateFitness(food);
    list_cpy(food, temp_food, n_citys + 1);

    srand(new_seed());
    start_point1 = rand_area(1, n_citys-3);
    //printf("startpoint1:%d\n", start_point1);
    for (int i = 1; i < 10; i++)
    {
        if (start_point1 + i + 2 < n_citys) //判断后面的长度能不能塞下两段
        {
            start_point2 = start_point1 + i + 1;
            for (int j = 1; j < 10; j++)
            {
                if (start_point2 + j < n_citys) //判断后面的长度能不能塞下最后段
                {
                    //将各断点放入顺序表中，以供four_opt_order来安排顺序
                    point_order[0] = start_point1; point_order[1] = start_point1 + i; 
                    point_order[2] = start_point2; point_order[3] = start_point2 + j;

                    int *temp = (int *)malloc(sizeof(int) * (start_point2 + j - start_point1 + 1)); //临时存放三段总和
                    for (int m = 0; m < 4; m++)
                    {
                        int temp_index = 0;

                        //进行一次3_opt操作
                        for (int n = 0; n < 4; n += 2) 
                        {
                            int point1 = point_order[three_opt_order[m][n] - 1], point2 = point_order[three_opt_order[m][n+1] - 1];
                            if (point1 > point2) //正向复制
                            {
                                for (int p = 0; p < point1 - point2 + 1; ++p)
                                {
                                    temp[temp_index++] = temp_food[point1 - p]; 
                                }
                            }
                            else //反向复制
                            {
                                for (int p = 0; p < point2 - point1 + 1; ++p)
                                {
                                    temp[temp_index++] = temp_food[point1 + p]; 
                                }
                            }
                        }
                        
                        list_cpy(temp, &temp_food[start_point1], start_point2 + j - start_point1 + 1);
                        new_cost = calculateFitness(temp_food);
                        if (new_cost < old_cost)
                        {
                            list_cpy(temp_food, food, n_citys+1);
                            free(temp_food);free(temp);
                            return new_cost;
                        }
                        list_cpy(food, temp_food, n_citys+1);
                    }
                    free(temp);
                    
                    
                }
            }
        }
    }
    free(temp_food);
    return old_cost;
}

int four_opt(int *food)
{
    int old_cost, new_cost, start_point1, start_point2, start_point3, point_order[6] = {0};
    int *temp_food = (int *)malloc(sizeof(int) * (n_citys + 1));

    old_cost = calculateFitness(food);
    list_cpy(food, temp_food, n_citys + 1);


    srand(new_seed());
    start_point1 = rand_area(1, n_citys-5);
    //printf("startpoint1:%d\n", start_point1);
    for (int i = 1; i < 10; i++)
    {
        if (start_point1 + i + 4 < n_citys) //判断后面的长度能不能塞下三段
        {
            start_point2 = start_point1 + i + 1;
            for (int j = 1; j < 10; j++)
            {
                if (start_point2 + j + 2 < n_citys) //判断后面的长度能不能塞下两段
                {
                    start_point3 = start_point2 + j + 1;
                    for (int k = 1; k < 10; k++)
                    {
                        if (start_point3 + k < n_citys) //判断后面的长度能不能塞下最后段
                        {
                            //将各断点放入顺序表中，以供four_opt_order来安排顺序
                            point_order[0] = start_point1; point_order[1] = start_point1 + i; point_order[2] = start_point2; 
                            point_order[3] = start_point2 + j; point_order[4] = start_point3; point_order[5] = start_point3 + k;

                            int *temp = (int *)malloc(sizeof(int) * (start_point3 + k - start_point1 + 1)); //临时存放三段总和
                            for (int m = 0; m < 31; m++)
                            {
                                int temp_index = 0;

                                //进行一次3_opt操作
                                for (int n = 0; n < 6; n += 2) 
                                {
                                    int point1 = point_order[four_opt_order[m][n] - 1], point2 = point_order[four_opt_order[m][n+1] - 1];
                                    // print_list(four_opt_order[m], 6, 0);
                                    // printf("%d %d\n", point1, point2);
                                    // printf("\n");
                                    if (point1 > point2) //正向复制
                                    {
                                        for (int p = 0; p < point1 - point2 + 1; ++p)
                                        {
                                            temp[temp_index++] = temp_food[point1 - p]; 
                                        }
                                    }
                                    else //反向复制
                                    {
                                        for (int p = 0; p < point2 - point1 + 1; ++p)
                                        {
                                            temp[temp_index++] = temp_food[point1 + p]; 
                                        }
                                    }
                                }
                                
                                list_cpy(temp, &temp_food[start_point1], start_point3 + k - start_point1 + 1);
                                new_cost = calculateFitness(temp_food);
                                if (new_cost < old_cost)
                                {
                                    list_cpy(temp_food, food, n_citys+1);
                                    free(temp_food);free(temp);
                                    return new_cost;
                                }
                                list_cpy(food, temp_food, n_citys+1);
                            }
                            free(temp);
                        }
                    }
                }
            }
        }
    }
    free(temp_food);
    return old_cost;
}

int discontinuous_two_opt(int *food)
{   int *points_1, *temp_food = (int *)malloc(sizeof(int) * (n_citys+1));
    int old_cost, new_cost;

    old_cost = calculateFitness(food);

    //随机找能够生成总路径比原路径短的两小环的端点，如果找了k次没找到，则退出 
    int count = 100;
    while (count > 0)
    {
        // srand(new_seed());
        points_1 = rand_area_perm(1, n_citys-2, 2); //记录两个端点，这两个端点将原本的环变成两个小环 
        // break;
        if(points_1[0] == points_1[1] - 1) //若两随机点是挨着的，则继续循环至两随机点之间有一定距离
            continue;
        if (d[food[points_1[0]]][food[points_1[0]+1]] + d[food[points_1[1]]][food[points_1[1]+1]] > d[food[points_1[0]+1]][food[points_1[1]]] + d[food[points_1[0]]][food[points_1[1]+1]])
        {
            break;
        }
        --count;
    }
    if (count <= 0)
    {
        free(temp_food);free(points_1);
        return old_cost;
    }
    
    //在两个环上各选一个端点，进行断开再互联，若形成的新大环更短，则返回
    for (int i = points_1[0]+1; i < points_1[1]; i++) //0对面的环的循环
    {
        for (int j = 0; j < n_citys; j++) //0所在环的循环
        {
            if (j == points_1[0])
            {
                j = points_1[1] + 1;
            }
            
            if (j >= points_1[1] + 1) //如果0所在半环的随机点在靠近终点的那一半
            {
                list_cpy(food, temp_food, points_1[0] + 1);
                temp_food[points_1[0] + 1] = food[points_1[1] + 1];
                list_cpy(&food[points_1[1] + 2], &temp_food[points_1[0] + 2], j - points_1[1] - 1);
                temp_food[j + points_1[0] - points_1[1] + 1] = food[i + 1];
                list_cpy(&food[i + 2], &temp_food[j + points_1[0] - points_1[1] + 2], points_1[1] - i - 1);
                temp_food[points_1[0] + j - i + 1] = food[points_1[0] + 1];
                list_cpy(&food[points_1[0] + 2], &temp_food[points_1[0] + j - i + 2], i - points_1[0] - 1);
                temp_food[j + 1] = food[j + 1];
                list_cpy(&food[j + 2], &temp_food[j + 2], n_citys - j - 1);
            }
            else //如果0所在半环的随机点在靠近起点的那一半
            {
                list_cpy(food, temp_food, j + 1);
                temp_food[j + 1] = food[i + 1];
                list_cpy(&food[i + 2], &temp_food[j + 2], points_1[1] - i - 1);
                temp_food[points_1[1] + j - i + 1] = food[points_1[0] + 1];
                list_cpy(&food[points_1[0] + 2], &temp_food[points_1[1] + j - i + 2], i - points_1[0] - 1);
                temp_food[j + points_1[1] - points_1[0] + 1] = food[j + 1];
                list_cpy(&food[j + 2], &temp_food[j + points_1[1] - points_1[0] + 2], points_1[0] - j - 1);
                temp_food[points_1[1] + 1] = food[points_1[1] + 1];
                list_cpy(&food[points_1[1] + 2], &temp_food[points_1[1] + 2], n_citys - points_1[1] - 1);
            }
            
            new_cost = calculateFitness(temp_food);
            if (new_cost < old_cost)
            {
                list_cpy(temp_food, food, n_citys + 1);
                // free(temp_food); free(points_1);
                old_cost = new_cost;
                // return new_cost;
            }
        }
    }
free(temp_food);free(points_1);
return old_cost;   
}

int discontinuous_three_opt(int *food)
{
    int *points_1, *temp_food = (int *)malloc(sizeof(int) * (n_citys+1));
    int old_cost, new_cost;

    old_cost = calculateFitness(food);

    //随机找能够生成总路径比原路径短的两小环的端点，如果找了k次没找到，则退出 
    int count = 100;
    while (count > 0)
    {
        points_1 = rand_area_perm(1, n_citys-2, 2); //记录两个端点，这两个端点将原本的环变成两个小环
        if(points_1[0] == points_1[1] - 1) //若两随机点是挨着的，则继续循环至两随机点之间有一定距离
            continue;
        if (d[food[points_1[0]]][food[points_1[0]+1]] + d[food[points_1[1]]][food[points_1[1]+1]] > d[food[points_1[0]+1]][food[points_1[1]]] + d[food[points_1[0]]][food[points_1[1]+1]])
        {
            break;
        }
        --count;
    }
    if (count <= 0)
    {
        free(temp_food);free(points_1);
        return old_cost;
    }

    //在两个半环上找三个点断开，并连接成一个大环。
    int points[11] = {0};
    for (int i = points_1[0] + 1; i < points_1[1] - 2; i++)
    {
        for (int j = i+2; j < points_1[1]; j++)
        {
            for (int k = 0; k < n_citys-1; k++)
            {
                if (k == points_1[0])
                {
                    k = points_1[1] + 1;
                }

                points[1] = k; points[2] = k + 1; points[3] = i; points[4] = i + 1; points[5] = j;
                points[6] = j + 1; points[7] = points_1[0]; points[8] = points_1[0] + 1; points[9] = points_1[1]; points[10] = points_1[1] + 1;

                
                
                for (int o = 0; o < 4; o++)
                {
                    int order = o;
                    if (k >= points_1[0])
                    {
                        order += 4;
                    }
                    
                    int ind = points[discontinuous_three_opt_order[order][0]] + 1;
                    list_cpy(food, temp_food, ind);
                    for (int p = 2; p <= 10; p++)
                    {
                        if (p % 2 == 0)
                        {
                            temp_food[ind++] = food[points[discontinuous_three_opt_order[order][p-1]]]; 
                        }
                        else
                        {
                            int point1 = points[discontinuous_three_opt_order[order][p-2]];
                            int point2 = points[discontinuous_three_opt_order[order][p-1]];
                            if (point1 < point2)
                            {
                                for (int m = point1 + 1; m <= point2; m++)
                                {
                                    temp_food[ind++] = food[m];
                                }
                            }
                            else
                            {
                                for (int m = point1 - 1; m >= point2; m--)
                                {
                                    temp_food[ind++] = food[m];
                                }
                            }
                        }
                    }
                    list_cpy(&food[ind], &temp_food[ind], n_citys - ind + 1);

                    new_cost = calculateFitness(temp_food);
                    if (new_cost < old_cost)
                    {
                        free(temp_food);free(points_1);
                        list_cpy(temp_food, food, n_citys + 1);
                        return new_cost;
                    }
                }
            }
        }
    }
free(temp_food);free(points_1);
return old_cost;   
}


//将当前的点依次放在不同点之间,如果改善则按照策略跳出（改善则眺或者概率性跳），然后同理操作下一个点，直至所有点操作完毕
int two_opt_swap(int *food) 
{
    int new_cost, start_point; // current保存的是当前操作的点下标，next保存的是下一个要操作的点的下标
    int *temp_food = (int *)malloc(sizeof(int) * (n_citys + 1)), *temp_best = (int *)malloc(sizeof(int) * (n_citys + 1));

    list_cpy(food, temp_food, n_citys + 1);
    // srand(new_seed());
    start_point = rand_area(1, n_citys-1);

    //先将当前城市推到第一个位置去
    for (int j = 0; j < start_point-1; j++)
    {
        int temp = temp_food[start_point-j];
        temp_food[start_point-j] = temp_food[start_point-j-1];
        temp_food[start_point-j-1] = temp;
    }

    new_cost = calculateFitness(temp_food);
    list_cpy(temp_food, temp_best, n_citys+1);
    
    for (int j = 1; j < n_citys-1; j++)
    {   
        // 将当前城市往后推一位
        int temp = temp_food[j];
        temp_food[j] = temp_food[j+1];
        temp_food[j+1] = temp;

        // 如果当前的邻位置换能改善，则比较此次改善后的解与之前的最优改善有没有提升，若有则将其替换之前的最优改善
        if(d[temp_food[j]][temp_food[j-1]] + d[temp_food[j+1]][temp_food[j+2]] < d[temp_food[j+1]][temp_food[j-1]] + d[temp_food[j]][temp_food[j+2]])
        {
            int temp_cost = calculateFitness(temp_food);
            if (temp_cost <= new_cost)
            {
                new_cost = temp_cost;
                list_cpy(temp_food, temp_best, n_citys+1);
            }
        }
    }

    list_cpy(temp_best, food, n_citys+1);
    free(temp_food); free(temp_best);
    return calculateFitness(food);
}

// 随机选择一个子段，将它插入到所有可能的位置，选择最优的位置进行插入
int segment_insert(int *food)
{
    int best_insert_ind = -1, best_insert_diff = 0;
    int *seg = my_rand_area_perm(1, n_citys - 1, 2);

    for (int i = 1; i <= n_citys; i++)
    {
        if (i >= seg[0] && i <= seg[1] + 1)
            continue;

        int inset_diff = (d[food[seg[0]-1]][food[seg[0]]] + d[food[seg[1]]][food[seg[1]+1]] + d[food[i]][food[i-1]]) -
                        (d[food[seg[0]-1]][food[seg[1]+1]] + d[food[i-1]][food[seg[0]]] + d[food[seg[1]]][food[i]]);
        if (inset_diff > best_insert_diff)
        {
            best_insert_diff = inset_diff;
            best_insert_ind = i;
        }
    }
    
    if (best_insert_ind > 0)
    {
        int *temp = (int *)malloc((seg[1] - seg[0] + 1) * sizeof(int));
        list_cpy(&food[seg[0]], temp, seg[1] - seg[0] + 1);
        if (best_insert_ind < seg[0])
        {
            list_cpy(&food[best_insert_ind], &food[seg[1] + 1 - (seg[0] - best_insert_ind)], seg[0] - best_insert_ind);
            list_cpy(temp, &food[best_insert_ind], seg[1] - seg[0] + 1);
        }
        else
        {
            list_cpy(&food[seg[1] + 1], &food[seg[0]], best_insert_ind - seg[1] - 1);
            list_cpy(temp, &food[seg[0] + best_insert_ind - seg[1] - 1], seg[1] - seg[0] + 1);
        }
        free(temp);
    }
    free(seg);
    return calculateFitness(food);  
}


// 随机选择一个点，和所有其他点交换一遍，选择最优的进行交换
int point_swap(int *food)
{
    int best_swap_ind = -1, best_swap_diff = 0;
    int p = my_rand_area(1, n_citys-1);

    for (int i = 1; i <= n_citys - 1; i++)
    {
        if (i == p)
            continue;
        int diff = d[i][i-1] + d[i][i+1] + d[p][p-1] + d[p][p+1] -
                   (d[i][p-1] + d[i][p+1] + d[p][i-1] + d[p][i+1]);
        if (diff > best_swap_diff)
        {
            best_swap_diff = diff;
            best_swap_ind = i;
        } 
    }
    
    if (best_swap_ind > 0)
        swap(food, best_swap_ind, p);
    
    return calculateFitness(food);
}








