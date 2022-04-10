// 编译语句： g++ -o main -g framework_final_newScoringMethod.cpp
// 运行语句：./main
// 8 
#include<stdio.h>
#include "seed.h"
#include "my_rand.h"
#include "tools.h"
#include "ReadTSPFile.h"
#include "distances.h"
#include "initialize_params.h"
#include "randperm.h"
#include "initial.h"
#include "calculateFitness.h"
#include "LLH.h"
#include "LLH_opt.h"
#include "wolf_scout_behavior.h"
#include "selection_single.h"
#include "build_relationship_matrix.h"
#include "scout_bee_generate_new_route2.h"
#include <time.h>
#pragma comment( lib,"winmm.lib" )
using namespace std;

//"oliver30.tsp", "eil51.tsp", "st70.tsp","eil76.tsp", "pr76.tsp", "kroA100.tsp", "kroB100.tsp", "kroC100.tsp", "kroD100.tsp", "kroE100.tsp",
//"rd100.tsp", "eil101.tsp", "lin105.tsp", "pr107.tsp", "pr124.tsp", "ch130.tsp", "pr136.tsp", "pr144.tsp", "ch150.tsp", "kroA150.tsp", "kroB150.tsp", "pr152.tsp", "rat195.tsp"
//"kroA200.tsp", "kroB200.tsp", "tsp225.tsp", "ts225.tsp", "pr226.tsp", "gil262.tsp", "pr264.tsp", "pr299.tsp", "lin318.tsp", "rd400.tsp", "fl417.tsp", "pr439.tsp", "rat575.tsp"
int main()
{
	// "all_TSP/eil101.tsp", "all_TSP/ch130.tsp", "all_TSP/kroA150.tsp", "all_TSP/kroA200.tsp", "all_TSP/tsp225.tsp"
	// "all_TSP/rd100.tsp", "all_TSP/lin105.tsp", "all_TSP/pr107.tsp", "all_TSP/pr124.tsp", "all_TSP/pr136.tsp",
	// 						"all_TSP/pr144.tsp", "all_TSP/ch150.tsp", "all_TSP/kroB150.tsp", "all_TSP/pr152.tsp", "all_TSP/rat195.tsp",
	// 						"all_TSP/kroB200.tsp", "all_TSP/ts225.tsp", "all_TSP/pr226.tsp", "all_TSP/gil262.tsp", "all_TSP/pr264.tsp",
	char* dataset_Name[] = {"kroA100.tsp", "all_TSP/rat195.tsp",
"all_TSP/kroA200.tsp", "all_TSP/kroB200.tsp", "all_TSP/ts225.tsp", "all_TSP/pr226.tsp", "all_TSP/gil262.tsp", "all_TSP/pr264.tsp", "all_TSP/pr299.tsp", "all_TSP/rd400.tsp", "all_TSP/fl417.tsp", "all_TSP/pr439.tsp", "all_TSP/rat575.tsp"};
	int dataset_num = 1;
	char filename[] = "newScoringMethod.csv";	
	int run_num_sum_list[40] = {30, 30, 30, 30, 30, 30, 30, 30, 30, 10, 10, 10, 10, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 10, 10, 10, 10};
	for (int dataset_ind = 0; dataset_ind < dataset_num; dataset_ind++)
	{
		ReadTSPFile(dataset_Name[dataset_ind]); // 读取所有城市坐标及城市数
		d = disctances(); // 计算城市距离矩阵
		RM = relationship_matrix(d);
		int run_num_sum = run_num_sum_list[dataset_ind];
		int result[run_num_sum];
		int times[run_num_sum];
		int two_opt_sum = 0, three_opt_sum = 0, discontinuous_two_opt_sum = 0, discontinuous_three_opt_sum = 0, two_opt_swap_sum = 0,
    		LLH_ss_sum = 0, LLH_rss_sum = 0, LLH_rs_sum = 0, LLH_ris_sum = 0, LLH_ri_sum = 0, LLH_rrs_sum = 0, LLH_rris_sum = 0,
    		LLH_rrss_sum = 0, LLH_rsis_sum = 0, LLH_rsss_sum = 0;
		for(int run_num = 0; run_num < run_num_sum; run_num++)
		{
			int FoodNumber = 100; // 矿点数量
			int resetNumber = 2;
			int is_active[FoodNumber]; // 标记矿点是否有效
			for (int i = 0; i < FoodNumber; i++)
			{
				is_active[i] = resetNumber;
			}
			int inactive_count = 0; // 记录失效矿点的数量
			int Limit = 1000; // 侦察蜂更新矿点
			int maxCycle = 3000; 
			int Fitness[FoodNumber];
			int trial[FoodNumber] = {0}, trial_temp[FoodNumber] = {0};
			int LLH_num = 10; // 使用到的LLH数量
			int LLH_pool[20] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}; // 使用到的LLH下标
			int *LLH_pool_temp = (int *)malloc(sizeof(int) * LLH_num); // 每次迭代都得把LLH_pool重新载入这个当中，以免pool的内存被莫名其妙占用
			float upValue = float(Limit);
			float LLH_single_scores[FoodNumber][LLH_num]; // 单个LLH自身的分数
			float LLH_single_score_min = 1, LLH_single_score_max = upValue; // 单个LLH分数上下限
			float LLH_pair_scores[FoodNumber][LLH_num][LLH_num]; // 一对LLH的分数，即前一次迭代使用到的LLH和当前使用到的LLH结合起来考虑的序列性分数
			float LLH_pair_score_min = 1, LLH_pair_score_max = upValue; // LLH对分数上下限
			float LLH_triple_scores[FoodNumber][LLH_num][LLH_num][LLH_num]; // 三个连续LLH的分数，即前两次迭代使用到的LLH和当前使用到的LLH结合起来考虑的序列性分数
			float LLH_triple_score_min = 1, LLH_triple_score_max = upValue; // 三LLH序列分数上下限。
			int LLH_this_iter_pool[FoodNumber][LLH_num]; // 存放当前迭代每个矿点使用的LLH池
			int LLH_this_iter_pool_len[FoodNumber]; // 记录当前迭代每个矿点使用的LLH池中LLH数量
			// float LLH_pair_scores_for_select_LLH[FoodNumber][LLH_num]; // 用于选择LLH的LLH对分数
			// float LLH_triple_scores_for_select_LLH[FoodNumber][LLH_num]; // 用于选择LLH的三LLH序列分数
			for(int i=0; i < FoodNumber; i++) //初始化LLH分数、LLH对分数及三LLH序列分数
				for(int j=0; j < LLH_num; j++)
				{
					LLH_single_scores[i][j] = LLH_single_score_min; // 初始化LLH分数
					for (int k = 0; k < LLH_num; k++)     // 初始化LLH对分数
					{
						LLH_pair_scores[i][j][k] = LLH_pair_score_min;
						for (int l = 0; l < LLH_num; l++)	// 初始化三LLH序列分数
							LLH_triple_scores[i][j][k][l] = LLH_triple_score_min;
					}
				}
			int LLH_this_iter_improve[FoodNumber][LLH_num]; // 用于记录当前迭代每个矿点LLH中各LLH的改善次数。		
			// int LLH_this_bee_count[FoodNumber]; // 用于记录每个采蜜蜂矿点被观察蜂使用的次数
			int LLH_each_bee_each_llh[FoodNumber][LLH_num];  // 用于当前迭代记录每个矿点LLH池中每个LLH对于路径挖掘的平均程度
			int LLH_winner_each_bee[FoodNumber] = {0}; // 记录每个矿点当前使用的LLH池中表现最佳的LLH
			int LLH_last_each_bee[FoodNumber] = {0}; // 记录每个矿点上一迭代使用的LLH
			int LLH_last_2_each_bee[FoodNumber] = {0}; // 记录每个矿点上上一次迭代使用的LLH
			float score_decay = 0.9; // LLH分数衰退率

			int **Foods = initial(FoodNumber); //初始化所有矿点


			//统计一些运行中的数据
			int scout_count = 0;
			int _2opt_count = 0, disc_2opt_count = 0; // 记录每次迭代2opt和不连续2opt用到的次数
			int not_improve_count = 0, not_improve_flag = 1; //记录没有改善的步数,not_improve_flag则标记了当前循环有没有出现改善现象


			//计算适应度函数
			for (int i = 0; i < FoodNumber; i++)
			{
				Fitness[i] = calculateFitness(Foods[i]);
				// printf("%d\n",Fitness[i]);
			}

			//一些参数
			int BestInd = min(Fitness, FoodNumber);
			int GlobalMin = Fitness[BestInd];
			int iter = 1;
			int min_count = 0; // 记录群体中路径长度最小的路径数量

			int t_start, t_end;
			t_start = time(0);


			////////////////////////////////////// 主循环 //////////////////////////////////////
			initialize_params();
			printf("start the main loop!\n");
			// while (iter < 100)
			while (inactive_count < FoodNumber - 1)
			{	
				// 初始化一些本次循环需要的参数
				int this_LLH_improve[FoodNumber] = {0}; // 统计该LLH在哪些矿点出现了改善
				list_cpy(trial, trial_temp, FoodNumber);

				// 每个矿点根据LLH的分数通过轮盘赌选择LLH
				list_cpy(LLH_pool, LLH_pool_temp, LLH_num);
				if(iter > 2) 	// 从第三次迭代开始，记录上上一次迭代每个矿点使用的LLH
					list_cpy(LLH_last_each_bee, LLH_last_2_each_bee, FoodNumber);
				if(iter > 1)	// 从第二次迭代开始，记录前一次迭代每个矿点使用到的LLH
					list_cpy(LLH_winner_each_bee, LLH_last_each_bee, FoodNumber);

				//////////////////////// 建立每次迭代各矿点使用的LLH池
				int LLH_each_bee_count[LLH_num] = {0}; //记录所有路径选择的各LLH数量
				if(iter == 1) // 如果是第一次迭代，则只考虑单个LLH分数即可
				{
					for (int i = 0; i < FoodNumber; i++)
					{
						if(is_active[i] > 0)
						{
							float* prob_temp = p_to_prob_float(LLH_single_scores[i], LLH_num);
							int LLH_ind = RouletteWheelSelection(prob_temp, LLH_num);
							LLH_winner_each_bee[i] = LLH_pool[LLH_ind]; // 直接将当前轮盘赌选出的LLH作为此次迭代胜出者LLH
							LLH_this_iter_pool_len[i] = 1;  // 若在选择LLH阶段就已选择出胜出者LLH，则说明该LLH是多数LLH或者当前迭代是第一次迭代，
															// 则将当前迭代的矿点LLH池数量记为1，在后面操作路径的时候直接根据这个变量是否为一来决定要不要遍历矿点LLH池
						}
					}
					print_list(LLH_winner_each_bee, FoodNumber, 0);
				}
				else if(iter == 2) // 如果是第二次迭代，则要考虑单个LLH分数及LLH对分数
				{
					for (int i = 0; i < FoodNumber; i++)
					{
						if(is_active[i] > 0)
						{
							float* prob_temp1 = p_to_prob_float(LLH_single_scores[i], LLH_num);
							int LLH_ind1 = RouletteWheelSelection(prob_temp1, LLH_num);// 由单个LLH分数取轮盘赌选择出的LLH
							float* prob_temp2 = p_to_prob_float(LLH_pair_scores[i][LLH_last_each_bee[i]], LLH_num);
							int LLH_ind2 = RouletteWheelSelection(prob_temp2, LLH_num);// 由LLH对分数轮盘赌选择出的LLH
							if (LLH_ind1 == LLH_ind2)
							{
								LLH_winner_each_bee[i] = LLH_pool[LLH_ind1];
								LLH_this_iter_pool_len[i] = 1;
							}
							else
							{
								LLH_this_iter_pool[i][0] = LLH_pool[LLH_ind1]; LLH_this_iter_pool[i][1] = LLH_pool[LLH_ind2];
								LLH_this_iter_pool_len[i] = 2;
							}
						}
					}
				}
				else // 如果是第三次及往后的迭代，则需要将单个LLH分数、LLH对分数及三LLH序列分数都纳入考虑
				{
					for (int i = 0; i < FoodNumber; i++)
					{
						if(is_active[i] > 0)
						{
							float* prob_temp1 = p_to_prob_float(LLH_single_scores[i], LLH_num);
							int LLH_ind1 = RouletteWheelSelection(prob_temp1, LLH_num); // 由单个LLH分数轮盘赌选择出的LLH
							float* prob_temp2 = p_to_prob_float(LLH_pair_scores[i][LLH_last_each_bee[i]], LLH_num);
							int LLH_ind2 = RouletteWheelSelection(prob_temp2, LLH_num); // 由LLH对分数轮盘赌选择出的LLH
							float* prob_temp3 = p_to_prob_float(LLH_triple_scores[i][LLH_last_2_each_bee[i]][LLH_last_each_bee[i]], LLH_num);
							int LLH_ind3 = RouletteWheelSelection(prob_temp3, LLH_num); // 由三LLH序列分数轮盘赌选择出的LLH
							// 根据三个轮盘赌选出来的LLH，如果某个LLH占多数，则其为胜出者LLH，否则将所有LLH放入当前迭代矿点LLH池中。
							if (LLH_ind1 == LLH_ind2)
							{
								LLH_winner_each_bee[i] = LLH_pool[LLH_ind1];
								LLH_this_iter_pool_len[i] = 1;
							}
							else if(LLH_ind1 == LLH_ind3)
							{
								LLH_winner_each_bee[i] = LLH_pool[LLH_ind1];
								LLH_this_iter_pool_len[i] = 1;
							}
							else if (LLH_ind2 == LLH_ind3)
							{
								LLH_winner_each_bee[i] = LLH_pool[LLH_ind2];
								LLH_this_iter_pool_len[i] = 1;
							}
							else
							{
								LLH_this_iter_pool[i][0] = LLH_pool[LLH_ind1]; LLH_this_iter_pool[i][1] = LLH_pool[LLH_ind2]; LLH_this_iter_pool[i][2] = LLH_pool[LLH_ind3];
								LLH_this_iter_pool_len[i] = 3;
							}
						}
					}
				}

				for (int i = 0; i < FoodNumber; i++)
				{
					for (int j = 0; j < LLH_this_iter_pool_len[i]; j++)
					{
						if (is_active[i] > 0)
						{
							for (int k = 0; k < LLH_num; k++)
							{
								if(LLH_pool[k] == LLH_this_iter_pool[i][j])
									LLH_each_bee_count[k]++;
							}
						}
					}
				}
				int max_ind = max(LLH_each_bee_count, LLH_num);
				int max_value = LLH_each_bee_count[max_ind];
				int max_count = max_num(LLH_each_bee_count, LLH_num);

				int global_best_LLH = LLH_pool[max_ind];
				if (max_count > 0){
					int rand_sel = my_rand_area(1, max_count);
					for (int i = 0; i < LLH_num; i++)
					{
						if (LLH_each_bee_count[i] == max_value)
						{
							global_best_LLH = LLH_pool[i];
							rand_sel--;
						}
						if (rand_sel == 0)
						{
							break;
						}
					}
				}

				for (int i = 0; i < FoodNumber; i++)
				{
					if (LLH_this_iter_pool_len[i] > 1)
					{
						LLH_winner_each_bee[i] = global_best_LLH;
					}
				}
				
				// 初始化各矿点LLH改善次数计数器、采蜜蜂矿点被观察蜂引用次数计数器
				for (int i = 0; i < FoodNumber; i++)
				{
					for (int j = 0; j < LLH_this_iter_pool_len[i]; j++)
						LLH_this_iter_improve[i][j] = 0;
					this_LLH_improve[i] = 0;
					// LLH_this_bee_count[i] = 0;
				}
					
					

				////////////采蜜蜂模式///////////
				for (int i = 0; i < FoodNumber; i++)
				{
					if(is_active[i] > 0)
					{
						// 将原路径寄存在route_next里便于进行操作
						int *route_next = (int *)malloc(sizeof(int) * (n_citys + 1));
						list_cpy(Foods[i], route_next, n_citys + 1);

						// 对route_next进行操作以探索有没有更好的路径
						int FitnessSol = Fitness[i];

						int temp_fit = selection_single_opt(LLH_winner_each_bee[i], route_next); // 计算出其新解的质量
						if(temp_fit < FitnessSol) // 如果有改善，则对改善次数加一
							LLH_this_iter_improve[i][0]++;
						FitnessSol = temp_fit;
						
						////// 若给出的新解相对原解有改善，则将其替换原解
						if (FitnessSol < Fitness[i])
						{
							list_cpy(route_next, Foods[i], n_citys + 1);
							Fitness[i] = FitnessSol; 
							trial[i] = 0;
							// not_improve_flag = 0; //如果当前这个矿点出现了改善情况，则将not_improve_flag置0
							this_LLH_improve[i]++;
						}
						else
						{
							trial[i]++;
							trial_temp[i]++;
						}

						free(route_next);
					}
				}


				//记录此时最好的解
				BestInd = min(Fitness, FoodNumber);
				if(GlobalMin == Fitness[BestInd]) //如果最优解未改善则将not_improve_count加1
					not_improve_count++;
				else
					not_improve_count=0;
				
				//wolf scout behavior
				for (int i = 0; i < FoodNumber; i++)
				{
					if (is_active[i] > 0)
					{
						Fitness[i] = wolf_scout_behavior(Foods[i]);
					}
				}
				

				GlobalMin = Fitness[BestInd];
				trial[BestInd] = 0;
				///////////侦察蜂模式///////////
				int* smallest_n_ind_temp = smallest_n_ind(Fitness, FoodNumber, int(FoodNumber / 10));
				for (int i = 0; i < FoodNumber; i++)
				{
					if (trial[i] > Limit)
					{
						is_active[i]--;
						if (is_active[i] > 0)
						{
							if(FoodNumber - inactive_count > FoodNumber / 2 && is_in(i, smallest_n_ind_temp, int(FoodNumber / 10)))
							{
								continue;
							}
							int *new_route = scout_bee_generate_new_route2(Foods[i]);
							printf("  bee%d: %d ---> %d\n", i, Fitness[i], calculateFitness(new_route));
							Fitness[i] = calculateFitness(new_route);
							list_cpy(new_route, Foods[i], n_citys+1);
							free(new_route);
							trial[i] = 0;
							++scout_count;
						}
						else
						{
							inactive_count ++;
							// is_active[i] = 1;
							trial[i] = 0;
							++scout_count;
						}
					}
				}
				free(smallest_n_ind_temp);


				///////////////////// 更新单个LLH分数
				// printf("\none LLH:  ");
				for (int i = 0; i < FoodNumber; i++)
				{	
					if(is_active[i] > 0)
					{
						int LLH_ind = LLH_winner_each_bee[i]; // 对于每个矿点，LLH_ind为本次迭代中该矿点所使用的LLH
						int improvement = 0;

						if (this_LLH_improve[i] > 0)
							improvement = (trial_temp[i] / Limit) * upValue / 2;

						// 对于该矿点使用的LLH分数进行更新
						// printf("%f->", LLH_single_scores[i][LLH_ind]);
						if(improvement > 0)
							LLH_single_scores[i][LLH_ind] += improvement;
						else
							LLH_single_scores[i][LLH_ind] *= score_decay;
						
						// 控制单个LLH分数在上限与下限之间
						if (LLH_single_scores[i][LLH_ind] < LLH_single_score_min)
							LLH_single_scores[i][LLH_ind] = LLH_single_score_min;
						else if(LLH_single_scores[i][LLH_ind] > LLH_single_score_max)
							LLH_single_scores[i][LLH_ind] = LLH_single_score_max;

					}
				}
				///////////////////// 更新LLH对分数
				// printf("\ntwo LLH:  ");
				if( iter >= 2 )
					for (int i = 0; i < FoodNumber; i++)
					{
						if (is_active[i] > 0)
						{
							int LLH_ind = LLH_winner_each_bee[i]; // 对于每个矿点，LLH_ind为本次迭代中该矿点所使用的LLH
							int improvement = 0;
							if (this_LLH_improve[i] > 0)
								improvement = (trial_temp[i] / Limit) * upValue / 2;

							// 对于该矿点使用的LLH分数进行更新	
							if(improvement > 0)
								LLH_pair_scores[i][LLH_last_each_bee[i]][LLH_ind] += improvement;
							else
								LLH_pair_scores[i][LLH_last_each_bee[i]][LLH_ind] *= score_decay;	

							// 控制LLH对分数在上限与下限之间
							if(LLH_pair_scores[i][LLH_last_each_bee[i]][LLH_ind] < LLH_pair_score_min)
								LLH_pair_scores[i][LLH_last_each_bee[i]][LLH_ind] = LLH_pair_score_min;
							else if(LLH_pair_scores[i][LLH_last_each_bee[i]][LLH_ind] > LLH_pair_score_max)
								LLH_pair_scores[i][LLH_last_each_bee[i]][LLH_ind] = LLH_pair_score_max;
						}
					}
				///////////////////// 更新三LLH序列分数
				// printf("\nthree LLH:  ");
				if( iter >= 3 )
					for (int i = 0; i < FoodNumber; i++)
					{
						if (is_active[i] > 0)
						{
								int LLH_ind = LLH_winner_each_bee[i]; // 对于每个矿点，LLH_ind为本次迭代中该矿点所使用的LLH
							int improvement = 0;
							if (this_LLH_improve[i] > 0)
								improvement = (trial_temp[i] / Limit) * upValue / 2;

							// 对于该矿点使用的LLH分数进行更新
							if(improvement > 0)
								LLH_triple_scores[i][LLH_last_2_each_bee[i]][LLH_last_each_bee[i]][LLH_ind] += improvement;
							else
								LLH_triple_scores[i][LLH_last_2_each_bee[i]][LLH_last_each_bee[i]][LLH_ind] *= score_decay;

							// 控制三LLH分数在上限与下限之间
							if(LLH_triple_scores[i][LLH_last_2_each_bee[i]][LLH_last_each_bee[i]][LLH_ind] < LLH_triple_score_min)
								LLH_triple_scores[i][LLH_last_2_each_bee[i]][LLH_last_each_bee[i]][LLH_ind] = LLH_triple_score_min;
							else if(LLH_triple_scores[i][LLH_last_2_each_bee[i]][LLH_last_each_bee[i]][LLH_ind] > LLH_triple_score_max)
								LLH_triple_scores[i][LLH_last_2_each_bee[i]][LLH_last_each_bee[i]][LLH_ind] = LLH_triple_score_max;
						}
					}
				//记录此时最好的解
				BestInd = min(Fitness, FoodNumber);
				GlobalMin = Fitness[BestInd];
				min_count = min_num(Fitness, FoodNumber);
				
				// print_list(this_LLH_improve, FoodNumber, 0);
				printf("%d iter:%d  best distance:%d FoodNumber:%d reset:%d\n  ",run_num, iter, GlobalMin, FoodNumber - inactive_count, is_active[max(is_active, FoodNumber)]);
				// print_list_float(LLH_single_scores[0], LLH_num, 0);
				if (iter > maxCycle/2)
				{
					// Sleep(500);
				}
				++iter;
			}
			//////////////////////////////////// 主循环结束 //////////////////////////////////////
			t_end = time(0);
			BestInd = min(Fitness, FoodNumber);
			GlobalMin = Fitness[BestInd];
			printf("best distance:%d\n", GlobalMin);
			print_list(Foods[BestInd], n_citys + 1, 0);
			printf("Use Time:%d\n", t_end - t_start);

			printf("scout bee:%d\n", scout_count);
			printf("2opt:%d  3opt:%d  discon_2opt:%d\n  2opt_swap:%d\n", two_opt_count, three_opt_count, discontinuous_two_opt_count, two_opt_swap_count);
			// print_list(Fitness, FoodNumber, 0);
			printf("min count:%d\n", min_count);
			printf("min value:%d max value:%d\n", Fitness[min(Fitness, FoodNumber)], Fitness[max(Fitness, FoodNumber)]);
			
			result[run_num] = GlobalMin;
			times[run_num] = t_end - t_start;
			//将数据写入csv
			FILE *fp = NULL;
			fp = fopen(filename, "a+");
			if (run_num == 0)
			{
				fprintf(fp, "%s\t\n%s\t%s\n", dataset_Name[dataset_ind], "result", "time");
			}
			fprintf(fp, "%d\t%d\n", GlobalMin, t_end - t_start);
			fclose(fp);

			two_opt_sum += two_opt_count; discontinuous_two_opt_sum += discontinuous_two_opt_count; two_opt_swap_sum += two_opt_swap_count;
			LLH_ss_sum += LLH_ss_count; LLH_rss_sum += LLH_rss_count; LLH_rs_sum += LLH_rs_count; LLH_ris_sum += LLH_ris_count; LLH_ri_sum += LLH_ri_count;
			LLH_rrs_sum += LLH_rrs_count; LLH_rris_sum += LLH_rris_count; LLH_rrss_sum += LLH_rrss_count; LLH_rsis_sum += LLH_rsis_count; LLH_rsss_sum += LLH_rsss_count;
		}	

		//将平均结果和平均运行时间数据写入csv
		FILE *fp = NULL;
		fp = fopen(filename, "a+");
		fprintf(fp, "average result\t average time\n %f\t%f\n", average(result, run_num_sum), average(times, run_num_sum));
		fclose(fp);


	}
	return 0;
} 

