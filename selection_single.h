#include <cstdio>

int selection_single_opt(int choice, int *food)
{
    int new_fit;
    switch (choice)
    {
    case 1: new_fit = two_opt_LLH(food); ++two_opt_count; break;

    case 2: new_fit = discontinuous_two_opt(food); ++discontinuous_two_opt_count; break;

    case 3: new_fit = two_opt_swap(food); ++two_opt_swap_count; break;

    case 4: new_fit = point_swap(food); ++three_opt_count; break; 

    case 5: new_fit = segment_insert(food); ++discontinuous_three_opt_count;break;

    case 6: new_fit = LLH_rss(food);break;

    case 7: new_fit = LLH_rs(food);break; 

    case 8: new_fit = LLH_ris(food);break;

    case 9: new_fit = LLH_ri(food);break;

    case 10: new_fit = LLH_rrs(food);break;

    case 11: new_fit = LLH_ss(food);break;

    case 12: new_fit = LLH_rris(food);break;

    case 13: new_fit = LLH_rrss(food);break;

    case 14: new_fit = LLH_rsis(food);break;

    case 15: new_fit = LLH_rsss(food);break;

    default:
        break;
    }
    return new_fit;
}
