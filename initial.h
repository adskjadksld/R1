#include <cstdio>

int **initial(int FoodNumber)
{
    //开辟存储矿点的矩阵(FoodNumber * (n_citys+1))
    int **Foods = (int **)malloc(sizeof(int *) * FoodNumber);
    if (Foods == NULL)
    {
        printf("initial 分配失败");
    }
	for (int i = 0; i < FoodNumber; ++i)
	{
		Foods[i] = (int *)malloc(sizeof(int) * (n_citys + 1));
        if (Foods[i] == NULL)
        {
            printf("initial 分配失败");
        }
        
	}
    //随机生成矿点FoodNumber次，每次生成完都存入矿点矩阵内
    for (int i = 0; i < FoodNumber; i++)
    {
        srand(new_seed()); //每次生成一个矿点都要更新seed
        int *temp = randperm(n_citys - 1);
        Foods[i][0] = 0;
        for (int j = 0; j < n_citys - 1; j++)
            Foods[i][j+1] = temp[j] + 1;
        Foods[i][n_citys] = 0;
    }
    return Foods;
}