#include <cstdio>  

int* start_used;
int two_opt_count = 0; int three_opt_count = 0; int discontinuous_two_opt_count = 0; int discontinuous_three_opt_count = 0; int two_opt_swap_count = 0;
int LLH_ss_count = 0; int LLH_rss_count = 0; int LLH_rs_count = 0; int LLH_ris_count = 0; int LLH_ri_count = 0; int LLH_rrs_count = 0; int LLH_rris_count = 0;
int LLH_rrss_count = 0; int LLH_rsis_count = 0; int LLH_rsss_count = 0;

void initialize_params()
{
   

    start_used = (int *)malloc(sizeof(int) * (n_citys + 1));
    for (int i = 0; i < n_citys + 1; i++)
    {
        start_used[i] = 0;
    }
}