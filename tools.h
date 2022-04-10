#include <cstdio>
#include <time.h>
#include <stdlib.h>

void reverse(int *list, int len)
{
    int temp;
    for(int i=0; i<len/2; i++)
    {
        temp = list[i];
        list[i] = list[len-1-i];
        list[len-1-i] = temp;
    }
}

//在min~max之间随机生成一个整数，且包含min、max
int rand_area(int min, int max)
{
    int result;
    // srand(new_seed());
    result = rand() % (max - min + 1);
    result += min;
    return result;
}

//随机生成一定范围内几个随机数, 并从大到小排列成数组输出, 包含min、max
int *rand_area_perm(int min, int max, int num)
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
        // srand(new_seed());
        int temp = rand_area(min, max), i = 0, flag = 0;
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

//取字符串的子串：下标start开始，下标end结束，包括start，不包括end
char *substr(char *str, int start, int end)
{
    int len = end - start + 1;
    char *sub = (char *)malloc(sizeof(char) * len);
    for(int i=0; i < len - 1; i++){
        sub[i] = str[start + i];
    }
    sub[len - 1] = '\0';
    return sub;
}

// 包含start，不包含end
int *sublist(int *list, int start, int end)
{
    int len = end - start + 1;
    int *sub = (int *)malloc(sizeof(int) * len);
    for(int i=0; i < len - 1; i++){
        sub[i] = list[start + i];
    }
    sub[len - 1] = -1;
    return sub;
}

//将lst1内容赋值给lst2
void list_cpy(int *lst1, int *lst2, int len) 
{
    int *temp = (int *)malloc(len * sizeof(int));
    for (int i = 0; i < len; i++)
    {
        temp[i] = lst1[i];
    }
    for (int i = 0; i < len; i++)
    {
        lst2[i] = temp[i];
    }
    free(temp);
}

void print_list(int *lst, int len, int is_with_num)
{
    
    for (int i = 0; i < len; i++)
    {
        if (is_with_num == 0)
        {
            printf("%d ", lst[i]);
        }
        else
        {
            printf("%d:%d ", i, lst[i]);
        }
        
    }
    printf("\n");
}

void print_list_float(float *lst, int len, int is_with_num)
{
    
    for (int i = 0; i < len; i++)
    {
        if (is_with_num == 0)
        {
            printf("%f ", lst[i]);
        }
        else
        {
            printf("%d:%f ", i, lst[i]);
        }
        
    }
    printf("\n");
}

//求数列最小值下标
int min(int *list, int len)
{
    int m = list[0], ind = 0;
    for (int i = 0; i < len; i++)
    {
        if (m > list[i])
        {
            m = list[i];
            ind = i;
        }
    }
    return ind;   
}

//求数列最大值下标
int max(int *list, int len)
{
    int m = list[0], ind = 0;
    for (int i = 0; i < len; i++)
    {
        if (m < list[i])
        {
            m = list[i];
            ind = i;
        }
    }
    return ind;   
}

//返回一个数列最小值的个数
int min_num(int *list, int len)
{
    int num = 0;
    int min_ind = min(list, len);
    int min_value = list[min_ind];

    for (int i = 0; i < len; i++)
    {
        if (list[i] == min_value)
        {
            num++;
        }
    }
    return num;
}

//返回一个数列最大值的个数
int max_num(int *list, int len)
{
    int num = 0;
    int max_ind = max(list, len);
    int max_value = list[max_ind];

    for (int i = 0; i < len; i++)
    {
        if (list[i] == max_value)
        {
            num++;
        }
    }
    return num;
}

//求浮点数列数列的和
float sum_float(float *list, int len)
{
    float result = 0;
    for (int i = 0; i < len; i++)
    {
        result += list[i];
    }
    return result;
}

//求整型数列数列的和
int sum_int(int *list, int len)
{
    int result = 0;
    for (int i = 0; i < len; i++)
    {
        result += list[i];
    }
    return result;
}

//将数列变为轮盘赌概率形式, 数值越小几率越大
float *p_to_prob(int *list, int len)
{
    float *result = (float *)malloc(sizeof(float) * len);
    for (int i = 0; i < len; i++)
    {
        result[i] = 1/(float)(1 + list[i]);
    }
    float sum_ = sum_float(result, len);
    for (int i = 0; i < len; i++)
    {
        result[i] /= sum_;
    }
    return result;
}

//将数列变为轮盘赌概率形式, 数值越大几率越大
float *p_to_prob_float(float *list, int len)
{
    float *result = (float *)malloc(sizeof(float) * len);
    for (int i = 0; i < len; i++)
        result[i] = list[i];
        
    float sum_ = sum_float(result, len);
    // printf("%f\n", sum_);
    for (int i = 0; i < len; i++)
    {
        result[i] /= sum_;
    }
    return result;
}

float *cumsum(float *list, int len)
{
    float *result = (float *)malloc(sizeof(float) * len);
    result[0] = list[0];
    for (int i = 0; i < len - 1; i++)
    {
        result[i+1] = result[i] + list[i+1];
    }
    // free(list);
    return result;
}

int *cumsum_int(int *list, int len)
{
    int *result = (int *)malloc(sizeof(int) * len);
    result[0] = list[0];
    for (int i = 0; i < len - 1; i++)
    {
        result[i+1] = result[i] + list[i+1];
    }
    return result;
}

//轮盘赌
int RouletteWheelSelection(float *prob, int len)
{
    float *temp = cumsum(prob, len);
    float rand = (float)rand_area(0, 9999) / 10000;
    int i;
    for ( i = 0; i < len; i++)
    {
        if (rand <= temp[i])
        {
            return i;
        }
    }
    free(temp);
    return len-1;
}

//轮盘赌
int RouletteWheelSelection_my_rand(float *prob, int len)
{
    float *temp = cumsum(prob, len);
    my_srand(new_seed());
    float rand = (float)my_rand_area(0, 9999) / 10000;
    int i;
    for ( i = 0; i < len; i++)
    {
        if (rand <= temp[i])
        {
            return i;
        }
    }
    free(temp);
    return len-1;
}

// 判断一个元素是否在给定列表里
int is_in(int elem, int *list, int n)
{
    for (int i = 0; i < n; i++)
    {
        if(list[i] == elem)
            return 1;
    }
    return 0;
}

// 求一个数组前n个元素的下标（例：2、3、1、5、4 前3个元素下标为：2、0、1）
int* smallest_n_ind(int* list, int len, int n)
{
    int* result = (int *)malloc(sizeof(int) * n);
    for(int i = 0; i < n; i++)
        result[i] = -1;
    
    for(int i = 0; i < n; i++)
    {
        int smallest = -1;
        for (int j = 0; j < len; j++)
        {
            if(!is_in(j, result, i) && j == 0 || !is_in(j, result, i) && list[j] <= list[smallest])
                smallest = j;
        }
        result[i] = smallest;
    }
    return result;
}

// 交换list数组中下标为i、j两个元素的值
void swap(int *list, int i, int j)
{
    int temp = list[i];
    list[i] = list[j];
    list[j] = temp;
}

// 求一个数组的平均值
float average(int *list, int len)
{
    float result = 0;
    for (int i=0; i < len; i++)
        result += float(list[i]);
    return result / float(len);
}

// 求一个数组的平均值
float average(float *list, int len)
{
    float result = 0;
    for (int i=0; i < len; i++)
        result += list[i];
    return result / float(len);
}