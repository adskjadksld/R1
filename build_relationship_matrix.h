#include <cstdio>

int **RM;

// 建立关系矩阵，即第i行数据是除第i个城市之外，其余n-1个城市下标，且从左到右与第i个城市距离依次递增
int **relationship_matrix(int **d)
{
    int **RM = (int **)malloc(sizeof(int *) * n_citys); //定义关系矩阵

    //开辟关系矩阵的空间
    for (int i = 0; i < n_citys; i++)
        RM[i] = (int *)malloc(sizeof(int) * n_citys); 
    

    for (int i = 0; i < n_citys; i++)
    {

        //对剩余n-1个城市按照与当前城市的距离进行从小到大排序
        for (int j = 0; j < n_citys; j++)
        {
            int k = j;
            for (; k > 0; k--)
            {
                if (d[i][j] < d[i][RM[i][k-1]])
                {
                    RM[i][k] = RM[i][k-1];
                }
                else
                {
                    break;
                }
            }
            RM[i][k] = j;
        }
    }
    return RM;
}