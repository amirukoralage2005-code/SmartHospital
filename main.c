#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void displayMenu() {
    printf("\n============================================\n");
    printf("   SMART HOSPITAL & RESOURCE ALLOCATION     \n");
    printf("============================================\n");
    printf("1.View Lookup Data & Bed Occupancy\n");
    printf("2.Patient Intake & Registration\n");
    printf("3.Display Patient Priority Waiting Queue\n");
    printf("4.Generate Performance & Analytics Report\n");
    printf("5.Exit System\n");
    printf("Enter your choice (1-5): ");
}

int main()
{
    int choice;

    do {
        displayMenu();
        if (scanf("%d", &choice) != 1) {
            continue;
        }

        switch (choice) {
            case 1:
                printf("\nLookup Data\n");
                break;
            case 2:
                printf("\nIntake & Registration\n");
                break;
            case 3:
                printf("\nPatient Priority Waiting Queue\n");
                break;
            case 4:
                printf("\nAnalytics Report\n");
                break;
            case 5:
                printf("Exiting system...\n");
                break;
            default:
                printf("\nInvalid selection! Choice must be between 1 and 5.\n");
        }
    } while (choice != 5);

    return 0;
}
