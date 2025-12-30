#include<stdio.h>
#include<stdlib.h>
#include "cwa_estimater.h"


#define PUBLIC 
#define PRIVATE static

struct student{
    char*name;
    int completed_credits;
    int remaining_credits;
    float current_cwa;
    float target_cwa;
    float wa_remain;
};

PUBLIC Student init_student(char*name,int com_credit,int rem_credit,float curr_cwa,float target_cwa){
    printf("Initializing Object..........\n");
    Student student = malloc(sizeof(struct student));
    if(!student){
        fprintf(stderr,"Allocation error\n");
        return NULL;
    }

    printf("Setting internal state............\n");
    student->name = name;
    student->completed_credits = com_credit;
    student->remaining_credits = rem_credit;
    student->current_cwa = curr_cwa;
    student->target_cwa = target_cwa;

    printf("Object initialized successfully\n");
    return student;

}

PUBLIC float calculate_fair_distribution(const Student student){
    int total_credit_hrs = student->completed_credits + student->remaining_credits;
    float weighted_average_com = student->current_cwa * student->completed_credits;
    float final_weighted_average = (student->target_cwa) * total_credit_hrs;
    float weighted_average_remain = final_weighted_average - weighted_average_com;

    float score_dist = (weighted_average_remain )/ (student->remaining_credits);
    student->wa_remain = weighted_average_remain;

    if(score_dist > 100 || score_dist < 0){
        if(score_dist > 100){
            return 0;
        }else{
            return -1;
        }
    }

    return score_dist;
}

PUBLIC float recalculate_fair_distribution(const Student student,float priority_score,int priority_credit){
    float local_wa_remain = student->wa_remain;
    float priority_wa = priority_credit * priority_score;

    float wa_diff = local_wa_remain - priority_wa;
    float credit_hr_diff = student->remaining_credits - priority_credit;
    float score_dist = (wa_diff)/ (credit_hr_diff);

    //Return 0 if score exceeds 100
    //Return -1 if score is less than 0
    if(score_dist > 100 || score_dist < 0){
        if(score_dist > 100){
            return 0;
        }else{
            return -1;
        }
    }


    
    return score_dist;
    
}

PUBLIC void destroy_object(const Student student){
    free(student);
    printf("Object has been destryed\n");
}