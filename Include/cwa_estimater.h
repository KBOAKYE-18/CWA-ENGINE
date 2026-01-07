#ifndef ESTIMATER_H
#define ESTIMATER_H

typedef struct student *Student;


Student init_student(int credits_com,int credits_remain);
float calculate_dist_CWA(const Student student,float target_cwa,float current_cwa);
float calculate_dist_CGPA(const Student student,float target_cgpa,int current_cgpa);
float recalculate_dist_CWA(const Student student,float total_achievable_WA,int total_achievable_Credits);
float recalculate_dist_CGPA(const Student student,float total_achievable_gradePts,int total_achievable_Credits);
void destroy_object(const Student student);

#endif