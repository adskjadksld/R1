#include <cstdio>  
#include<stdlib.h>
#include <iostream>
#include <string.h>

int n_citys; 

typedef struct citys{
    float x;
	float y; 
    struct citys *next; 
}citys;

citys *citys_position;

void ReadTSPFile();
void InitialCitys();
char *substr(char *str, int start, int end);

//以下为函数实现

void ReadTSPFile(char *file_name)
{
	FILE *fp;
	char str[50];
	int num;
	float x, y;

	if((fp=fopen(file_name,"r"))==NULL) printf("Cannot open!");
	
	InitialCitys();
	citys *tail = citys_position;
	citys *temp;

	int count = 0;
	while(fscanf(fp,"%[^\n]%*c\n",str)!=EOF)
	{
		
		if(!(str[0] < 'Z' && str[0] > 'A')){
		//读取每个城市的序号、x、y
			count ++; //计算城市数量
			int i = 0;
			int num_index = 1; //记录当前是在读取第几个数字
			int space_index = 0; //记录空格的下标
			while(i < strlen(str)){
				if(str[i] == ' ' && num_index == 1){
					num = atoi(substr(str, 0, i));
					space_index = i;
					num_index++;
				}
				else if (str[i] == ' ' && num_index == 2)
				{
					x = atof(substr(str, space_index+1, i));
					space_index = i;
				}
				i++;
			}
			y = atof(substr(str, space_index+1, strlen(str)));

			
		//将读取的序号、x、y输入到结点链数据结构里
			temp = (citys *)malloc(sizeof(citys));
			temp->x = x;
			temp->y = y;
			temp->next = tail->next;
			tail->next = temp;
			tail = tail->next;
		}

		n_citys = count;

	}
	fclose(fp);
} 

void InitialCitys(){
	citys_position = (citys *)malloc(sizeof(citys));//头结点申请地址
	if (citys_position == NULL)
		printf("申请空间失败");
	citys_position->next = NULL;
}



