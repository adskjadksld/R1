#include <cstdio>

int* scout_bee_generate_new_route2(int *old_route)
{
    int *new_route = (int *)malloc(sizeof(int) * (n_citys + 1));
    int start;

    // 使产生的新路径更倾向于从没有使用过的城市作为开始点进行构建
    while (1)
    {
        srand(new_seed());
        start = rand_area(0, n_citys-1);

        if (start_used[start] == 0)
        {
            break;
        }
        else
        {
            srand(new_seed());
            if (rand_area(0, 1000) > 500)
            {
                break;
            }
            else
            {
                start = rand_area(0, n_citys-1);
            }
        }
    }
    
    
    int path_len = 0;
    int left = RM[start][start_used[start]+1], right = RM[start][start_used[start]+2], left_ind = 1, right_ind = n_citys - 1;
    int* food_temp = (int *)malloc(sizeof(int) * (n_citys + 1));
    for(int j = 0; j < n_citys + 1; j++)
        food_temp[j] = -1;

    food_temp[0] = start; food_temp[n_citys] = start; food_temp[left_ind++] = left; food_temp[right_ind--] = right; path_len += 4;
    for (; path_len <= n_citys; )
    {
        for (int j = 0; j < n_citys; j++)
        {
            if (is_in(RM[left][j], food_temp, n_citys + 1) == 0)
            {
                left = RM[left][j];
                food_temp[left_ind++] = left;
                path_len++;
                break;
            }
        }
        for (int j = 0; j < n_citys; j++)
        {
            if (is_in(RM[right][j], food_temp, n_citys + 1) == 0)
            {
                right = RM[right][j];
                food_temp[right_ind--] = right;
                path_len++;
                break;
            }
        }
        // print_list(path, n_citys + 1, 0);
    }
    // 将随机点为开始点的回路转化为第0个城市为开始点的回路
    int zero_ind;
    for (int j = 0; j < n_citys + 1; j++)
    {
        if (food_temp[j] == 0)
        {
            zero_ind = j;
            break;
        }
    }
    list_cpy(&food_temp[zero_ind], new_route, n_citys - zero_ind + 1);
    list_cpy(&food_temp[1], &new_route[n_citys - zero_ind + 1], zero_ind); 
    free(food_temp);       
    start_used[start]++;
    return new_route;
}