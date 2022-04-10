#include <cstdio>  
#include <math.h>

int **d; //距离矩阵（n_citys * n_citys）

int **disctances()
{
    //开辟存储距离矩阵的存储空间
    int **d = (int **)malloc(sizeof(int *) * n_citys);
	for (int i = 0; i < n_citys; ++i)
	{
		d[i] = (int *)malloc(sizeof(int) * n_citys);
	}
    
    //将距离计算出来存储到矩阵种
    citys *city1 = citys_position->next, *city2 = citys_position->next;
    for (int i=0; i < n_citys; ++i){
        for (int j=0; j < n_citys; ++j){
            d[i][j] = (int)(sqrt(pow(city1->x - city2->x, 2) + pow(city1->y - city2->y, 2)) + 0.5);
            city2 = city2->next;
        }
        city2 = citys_position->next;
        city1 = city1->next;
    }
        

    return d;
}