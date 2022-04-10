#include <cstdio>

int wolf_scout_behavior(int *food)
{
    int k = 0, k_=0, old_cost, min_dist;
    int *temp = (int *)malloc(sizeof(int) * (n_citys+1));

    old_cost = calculateFitness(food);
    list_cpy(food, temp, n_citys+1);

    for (int i = 1; i < n_citys; i++)
    {
        k = i;
        if (rand_area(1, 1000) > 0) //依概率决定k’是选择离当前城市最近的城市还是随机生成
        {
            // 找到距离第k个城市最近的城市k'
            if (i > 1)
            {
                min_dist = d[food[i]][food[i-1]]; // 初始化最小min_dist，以便比较
            }
            else
            {
                min_dist = d[food[i]][food[i+1]];
            }
            for (int j = 1; j < n_citys; j++)
            {
                // printf("%d %d %d\n", k, j, d[food[k]][food[j]]);
                if (j != k && d[food[k]][food[j]] <= min_dist)
                {
                    min_dist = d[food[k]][food[j]];
                    k_ = j;
                }
            }
        }
        else{
            k_ = rand_area(1, n_citys-1);
        }

        
        // printf("k':%d\n", k_);

        //如果找到的最近的城市k'已经与当前城市k相邻，则k=k+1
        if (k_ == k - 1 || k_ == k + 1)
        {
            continue;
        }

        //如果不相邻,则进行置换子段k~k'操作，将城市k’翻到k的旁边
        if (k_ < k)
        {
            reverse(&temp[k_], k-k_);
        }
        else
        {
            reverse(&temp[k+1], k_-k);
        }
        // print_list(temp, n_citys+1, 0);

        //如果得到更优解，则替换原解
        if (calculateFitness(temp) < old_cost)
        {
            list_cpy(temp, food, n_citys+1);
            old_cost = calculateFitness(temp);
        }
        list_cpy(food, temp, n_citys+1);
        // return;
    }
    free(temp);
    return old_cost;
}