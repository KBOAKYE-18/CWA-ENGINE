#ifndef ESTIMATER_H
#define ESTIMATER_H

#define MAX_BUF 1024
#define MAX_COURSE 100
typedef struct student *Student;



Student init_student(char*,char*,char*,char*,char*,char*,int,int);
void destroy_student(Student);
void print_details(Student,float,float);


#endif