#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "cwa_estimater.h"

#define PUBLIC
#define PRIVATE static
#define DELIMITER " \t\n\r"

typedef struct course *Course;


struct course{
    char* name;
    int credit_hour;
    int score;
};


struct student{
    char* name;
    Course course_completed;
    Course course_remain;
};

PRIVATE Course init_course_rem(char* buf_r_course,char* buf_r_credit,int num_courses_rem){
    Course course_arr_rem = (Course) calloc(MAX_COURSE,sizeof(struct course)); //Allocate memory for course
    if(!course_arr_rem){
        fprintf(stderr,"Allocation Error\n");
        exit(EXIT_FAILURE);
    }

    int i = 0;
    int n,offset;
    int credit_hrs;
    char* ptr_credit = buf_r_credit;
    char* token;
    int limit_rem = num_courses_rem;

    token = strtok(buf_r_course,DELIMITER);

    //For completed Course
    while((i < limit_rem) && (token  != NULL ) && ((n = sscanf(ptr_credit,"%d%n",&credit_hrs,&offset)) == 1)){
        struct course rem_courses ; //Create comp_course object

        rem_courses.name = token; //set course name before calling subsequent token
        token = strtok(NULL,DELIMITER);
        rem_courses.credit_hour = credit_hrs;
        rem_courses.score = -1;

        //Set comp_course into comp_course array
        course_arr_rem[i] = rem_courses;

        ptr_credit += offset;
        ++i;
    
    };

    //Check to see if credits and scores were read
    if(n < 1){
        fprintf(stderr,"Failed to read credits");
        exit(EXIT_FAILURE);
    }

    return course_arr_rem;

}


PRIVATE Course init_course_com(char* buf_c_course,char* buf_c_credit,char* buf_c_scores,int num_courses_comp){
    Course course_arr_com = (Course) calloc(MAX_COURSE,sizeof(struct course)); //Allocate memory for course
    if(!course_arr_com){
        fprintf(stderr,"Allocation Error\n");
        exit(EXIT_FAILURE);
    }

    int i = 0;
    int n,j,offset_1,offset_2;
    int credit_hrs,scores;
    char* ptr_credit = buf_c_credit;
    char* ptr_score = buf_c_scores;
    char* token;
    int limit_com = num_courses_comp;

    token = strtok(buf_c_course,DELIMITER);

    //For completed Course
    while((i < limit_com) && (token  != NULL ) && ((n = sscanf(ptr_credit,"%d%n",&credit_hrs,&offset_1)) == 1) && ((j = sscanf(ptr_score,"%d%n",&scores,&offset_2)) == 1)){
        struct course comp_course ; //Create comp_course object

        comp_course.name = token; //set course name before calling subsequent token
        token = strtok(NULL,DELIMITER);
        comp_course.credit_hour = credit_hrs;
        comp_course.score = scores;

        //Set comp_course into comp_course array
        course_arr_com[i] = comp_course;

        ptr_credit += offset_1;
        ptr_score += offset_2;

        ++i;
    
    };

    //Check to see if credits and scores were read
    if(n < 1 || j < 1){
        fprintf(stderr,"Failed to read credits or scores");
        exit(EXIT_FAILURE);
    }

    return course_arr_com;

}
    

PUBLIC Student init_student(char* student_name,char* buf_c_course,char* buf_r_course,char* buf_c_credit,char* buf_r_credit,char* buf_c_scores,int num_courses_rem,int num_courses_com){
    printf("Initializing Student Object...............\n");
    Student student = (Student) malloc(sizeof(struct student));

    if(!student){
        fprintf(stderr,"Allocation Error\n");
        exit(EXIT_FAILURE);
    }

    student->name = student_name; //Set student name

    //Set course completed
    Course ptr_completed = init_course_com(buf_c_course,buf_c_credit,buf_c_scores,num_courses_com);
    student->course_completed = ptr_completed;

    //Set course remaining
    Course ptr_remain = init_course_rem(buf_r_course,buf_r_credit,num_courses_rem);
    student->course_remain = ptr_remain;

    printf("Student Object initialized successfully\n");
    return student;
}



PUBLIC float cwa_needed(Student student,float curr_cwa,float cwa_target){
    
};


void terminate(char* message){
    printf("%s",message);
}



PUBLIC void destroy_student(Student student){
    free(student->course_completed);
    terminate("First stage done....\n");

    free(student->course_remain);
    terminate("Second stage done....\n");

    free(student);
    terminate("Student object destroyed successfully\n");
};




PUBLIC void print_details(Student student,float exp_cwa){
    
}


