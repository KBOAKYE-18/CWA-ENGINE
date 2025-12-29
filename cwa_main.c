#include<stdio.h>
#include<stdlib.h>
#include "cwa_estimater.h"



int main(void){
    int num_com_courses;
    int num_rem_courses;

    printf("This is CWA Estimater\n");

    char buf_com_courses[MAX_BUF];
    char buf_rem_courses[MAX_BUF];
    char buf_com_credits[MAX_BUF];
    char buf_rem_credits[MAX_BUF];
    char buf_com_scores[MAX_BUF];
    char student_name[MAX_BUF];

    

    printf("Enter name:");
    if(fgets(student_name,MAX_BUF,stdin) != NULL){
        printf("Name read successfully\n");
    }else{
        if(feof(stdin)){
            fprintf(stderr,"End of File while reading name\n");
            exit(EXIT_FAILURE);
        }else{
            fprintf(stderr,"Read error happened while reading name\n");
            exit(EXIT_FAILURE);
        }
    }


    
    printf("Enter completed courses:");
    if(fgets(buf_com_courses,MAX_BUF,stdin) != NULL){
        printf("Completed courses read successfully\n");
    }else{
        if(feof(stdin)){
            fprintf(stderr,"End of File for completed courses\n");
            exit(EXIT_FAILURE);
        }else{
            fprintf(stderr,"Read error for completed courses\n");
            exit(EXIT_FAILURE);
        }
    }


    printf("Enter remaining courses:");
    if(fgets(buf_rem_courses,MAX_BUF,stdin) != NULL){
        printf("Remaining courses  read successfully\n");
    }else{
        if(feof(stdin)){
            fprintf(stderr,"End of File for remaining courses\n");
            exit(EXIT_FAILURE);
        }else{
            fprintf(stderr,"Read error for remaining courses\n");
            exit(EXIT_FAILURE);
        }
    }


    printf("Enter completed credits for each course:");
    if(fgets(buf_com_credits,MAX_BUF,stdin) != NULL){
        printf("Completed course credits  read successfully\n");
    }else{
        if(feof(stdin)){
            fprintf(stderr,"End of File for completed credits\n");
            exit(EXIT_FAILURE);
        }else{
            fprintf(stderr,"Read error for completed credits\n");
            exit(EXIT_FAILURE);
        }
    }

    printf("Enter remaining course credits:");
    if(fgets(buf_rem_credits,MAX_BUF,stdin) != NULL){
        printf("Completed course credits read successfully\n");
    }else{
        if(feof(stdin)){
            fprintf(stderr,"End of File for remianing credits\n");
            exit(EXIT_FAILURE);
        }else{
            fprintf(stderr,"Read error for remaining credits\n");
            exit(EXIT_FAILURE);
        }
    }

    printf("Enter scores for each course(completed):");
    if(fgets(buf_com_scores,MAX_BUF,stdin) != NULL){
        printf("Completed courses read successfully\n");
    }else{
        if(feof(stdin)){
            fprintf(stderr,"End of File for completed score\n");
            exit(EXIT_FAILURE);
        }else{
            fprintf(stderr,"Read error for completed scores\n");
            exit(EXIT_FAILURE);
        }
    }

    printf("Enter number of completed courses:");
    scanf("%d",&num_com_courses);

    printf("Enter number of remaining courses:");
    scanf("%d",&num_rem_courses);

    Student student = init_student(student_name,buf_com_courses,buf_rem_courses,buf_com_credits,buf_rem_credits,buf_com_scores,num_rem_courses,num_com_courses);
    
    float current_cwa ;
    float target_cwa;

    printf("Enter current cwa:");
    scanf("%f",&current_cwa);

    printf("Enter target cwa:");
    scanf("%f",&target_cwa);

    print_details(student,current_cwa,target_cwa);
    
    destroy_student(student);

    return 0;
}