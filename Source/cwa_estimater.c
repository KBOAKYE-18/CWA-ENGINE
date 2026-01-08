#include<stdio.h>
#include<stdlib.h>
#include "cwa_estimater.h"

/*
 * Error Codes:
 * -1: NULL pointer passed
 * -2: Impossible/unrealistic score
 * -3: Computation error (division by zero, invalid input)
 * -4: Function type mismatch (CWA vs CGPA)
 */


#define PUBLIC 
#define PRIVATE static


struct student{
    int credit_completed;
    int credit_remaining;
    union {
        struct {
            float weighted_average_remain;
            float target_CWA;
            float current_CWA;
        } CWA;
        struct {
            float gradePoints_needed;
            float target_CGPA;
            float current_CGPA;
        } CGPA;
    } academic_system;
    enum {CGPA_KIND,CWA_KIND} kind;
};

PUBLIC Student init_student(int credits_com,int credits_remain){
    //Allocate memory for student struct
    Student student = malloc(sizeof(struct student));
    if(!student){
        fprintf(stderr,"Allocation error\n");
        return NULL;
    }

    //Set Struct fields
    student->credit_completed = credits_com;
    student->credit_remaining = credits_remain;
    
    return student;

}

PUBLIC float calculate_dist_CWA(const Student student,float target_cwa,float current_cwa){
    //Check if student is null
    if(!student){
        fprintf(stderr,"CWA dist Error:Student type was NULL\n");
        return -1;
    }
    
    //Check for division by zero
    if(target_cwa == 0 || current_cwa == 0 || target_cwa < current_cwa){
        fprintf(stderr,"Computation error\n");
        return -3;
    }


    //Calculate score distribution for CWA
    int total_credits = student->credit_completed + student->credit_remaining;
    float total_WA = target_cwa * total_credits;
    float current_WA = current_cwa * student->credit_completed;
    float expected_WA = total_WA - current_WA;
    float score_dist = (expected_WA)/(student->credit_remaining);

    //Check if score is realistic
    if(score_dist > 100 || score_dist < 0){
        fprintf(stderr,"Impossible score to obtain\n");
        return -2;
    }

    //Set CWA specifics together with the kind
    student->academic_system.CWA.target_CWA = target_cwa;
    student->academic_system.CWA.current_CWA = current_cwa;
    student->academic_system.CWA.weighted_average_remain = expected_WA;

    student->kind = CWA_KIND;

    return score_dist;

}

PUBLIC float calculate_dist_CGPA(const Student student,float target_cgpa,float current_cgpa){
    //Check if student is null
    if(!student){
        fprintf(stderr,"CGPA dist Error:Student type was NULL\n");
        return -1;
    }

    //Check for division by zero
    if(target_cgpa == 0 || current_cgpa == 0 || target_cgpa < current_cgpa){
        fprintf(stderr,"Computation error\n");
        return -3;
    }


    //Calculate distribution for CGPA
    int total_credits = student->credit_completed + student->credit_remaining;
    float total_pts = target_cgpa * total_credits;
    float current_pts = current_cgpa * student->credit_completed;
    float expected_pts = total_pts  -  current_pts;
    float grade_dist = (expected_pts )/ (student->credit_remaining);


    //Check if score is realistic
    if(grade_dist < 0){
        fprintf(stderr,"Impossible score to obtain\n");
        return -2;
    }


    //Set CGPA specifics
    student->academic_system.CGPA.target_CGPA = target_cgpa;
    student->academic_system.CGPA.current_CGPA = current_cgpa;
    student->academic_system.CGPA.gradePoints_needed = expected_pts;

    student->kind = CGPA_KIND;

    return grade_dist;

}


PUBLIC float recalculate_dist_CWA(const Student student,float total_achievable_WA,int total_achievable_Credits){
    if(!student){
        fprintf(stderr,"CWA recalc_dist Error:Student type was NULL\n");
        return -1;
    }
    

    if(total_achievable_Credits <= 0 && total_achievable_WA > 0 || total_achievable_Credits > 0 && total_achievable_WA <= 0 ){
        fprintf(stderr,"Computation error\n");
        return -3;
    }

    
    if(student->kind != CWA_KIND){
        fprintf(stderr,"Function call mismatch\n");
        return -4;
    }

    float WA_remain = student->academic_system.CWA.weighted_average_remain;
    float remain_credit = student->credit_remaining;
    float score_redist = (WA_remain - total_achievable_WA)/(remain_credit - total_achievable_Credits);

    if(score_redist > 100 || score_redist < 0){
        fprintf(stderr,"Impossible score to obtain\n");
        return -2;
    }

    return score_redist;  
    
}

PUBLIC float recalculate_dist_CGPA(const Student student,float total_achievable_gradePts,int total_achievable_Credits){
    if(!student){
        fprintf(stderr,"CGPA recalc_dist Error:Student type was NULL\n");
        return -1;
    }

    if(total_achievable_Credits <= 0 && total_achievable_gradePts > 0 || total_achievable_Credits > 0 && total_achievable_gradePts <= 0 ){
        fprintf(stderr,"Computation error\n");
        return -3;
    }

    
    if(student->kind != CGPA_KIND){
        fprintf(stderr,"Function call mismatch\n");
        return -4;
    }

    float gradePts_remain = student->academic_system.CGPA.gradePoints_needed;
    float remain_credit = student->credit_remaining;
    float grade_redist = (gradePts_remain- total_achievable_gradePts)/(remain_credit - total_achievable_Credits);

    if(grade_redist < 0){
        fprintf(stderr,"Impossible score to obtain\n");
        return -2;
    }

    return grade_redist;  
    
}

PUBLIC void destroy_object(const Student student){
    free(student);
}