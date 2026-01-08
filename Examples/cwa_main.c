#include <stdio.h>
#include "cwa_estimater.h"

/*
 * Simple demonstration of CWA Estimater library
 * Shows typical usage for both CWA and CGPA systems
 */

void print_separator() {
    printf("\n================================================\n\n");
}

int main(void) {
    printf("\n=== CWA ESTIMATER LIBRARY DEMO ===\n");
    
    // ============================================
    // EXAMPLE 1: CWA System
    // ============================================
    print_separator();
    printf("EXAMPLE 1: Using CWA System\n");
    printf("---------------------------\n\n");
    
    // Student has completed 70 credits with 78.0 CWA
    // Has 80 credits remaining
    // Wants to achieve 79.0 CWA overall
    
    Student student1 = init_student(70, 80);
    
    printf("Student Profile:\n");
    printf("  Credits completed: 70\n");
    printf("  Credits remaining: 80\n");
    printf("  Current CWA: 78.0\n");
    printf("  Target CWA: 79.0\n\n");
    
    float required_cwa = calculate_dist_CWA(student1, 79.0, 78.0);
    
    if (required_cwa > 0) {
        printf("Result: You need to score %.2f average in remaining courses\n", required_cwa);
        printf("        to achieve your target CWA of 79.0\n\n");
        
        // After completing some courses, recalculate
        printf("Update: After completing 18 credits with total score of 1530:\n");
        float new_required = recalculate_dist_CWA(student1, 1530.0, 18);
        
        if (new_required > 0) {
            printf("  New required average: %.2f for remaining courses\n", new_required);
        }
    }
    
    destroy_object(student1);
    
    // ============================================
    // EXAMPLE 2: CGPA System
    // ============================================
    print_separator();
    printf("EXAMPLE 2: Using CGPA System\n");
    printf("----------------------------\n\n");
    
    // Student has completed 48 credits with 2.7 CGPA
    // Has 48 credits remaining
    // Wants to achieve 3.0 CGPA overall
    
    Student student2 = init_student(48, 48);
    
    printf("Student Profile:\n");
    printf("  Credits completed: 48\n");
    printf("  Credits remaining: 48\n");
    printf("  Current CGPA: 2.7\n");
    printf("  Target CGPA: 3.0\n\n");
    
    float required_gpa = calculate_dist_CGPA(student2, 3.0, 2.7);
    
    if (required_gpa > 0) {
        printf("Result: You need %.2f grade point average in remaining courses\n", required_gpa);
        printf("        to achieve your target CGPA of 3.0\n\n");
        
        // After completing some courses, recalculate
        printf("Update: After completing 6 credits earning 24 grade points (4.0 avg):\n");
        float new_required_gpa = recalculate_dist_CGPA(student2, 24.0, 6);
        
        if (new_required_gpa > 0) {
            printf("  New required grade point: %.2f for remaining courses\n", new_required_gpa);
        }
    }
    
    destroy_object(student2);
    
    // ============================================
    // EXAMPLE 3: Checking if goal is achievable
    // ============================================
    print_separator();
    printf("EXAMPLE 3: Validating Goal Feasibility\n");
    printf("---------------------------------------\n\n");
    
    Student student3 = init_student(80, 20);
    
    printf("Student Profile:\n");
    printf("  Credits completed: 80\n");
    printf("  Credits remaining: 20\n");
    printf("  Current CWA: 65.0\n");
    printf("  Target CWA: 85.0\n\n");
    
    float result = calculate_dist_CWA(student3, 85.0, 65.0);
    
    if (result == -2) {
        printf("Result: Target is impossible to achieve!\n");
        printf("  (Would require score > 100 in remaining courses)\n\n");
    } else if (result > 0) {
        printf("Required average: %.2f\n", result);
    }
    
    destroy_object(student3);
    
    // ============================================
    // EXAMPLE 4: Maintaining current performance
    // ============================================
    print_separator();
    printf("EXAMPLE 4: Maintaining Current GPA\n");
    printf("-----------------------------------\n\n");
    
    Student student4 = init_student(60, 36);
    
    printf("Student Profile:\n");
    printf("  Credits completed: 60\n");
    printf("  Credits remaining: 36\n");
    printf("  Current CGPA: 3.5\n");
    printf("  Target CGPA: 3.5 (maintain)\n\n");
    
    float maintain = calculate_dist_CGPA(student4, 3.5, 3.5);
    
    if (maintain == 0.0) {
        printf("Result: Just maintain your current performance!\n");
        printf("  Continue scoring around 3.5 grade points per credit\n");
    }
    
    destroy_object(student4);
    
    print_separator();
    printf("Demo completed successfully!\n\n");
    
    return 0;
}