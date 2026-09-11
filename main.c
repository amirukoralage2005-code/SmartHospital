#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PATIENTS 100
#define TOTAL_SPECIALTIES 4
#define TOTAL_WARDS 4

const char *specialtyTitles[TOTAL_SPECIALTIES] = {
    "General Practice (OPD)",
    "Paediatrics",
    "Cardiology",
    "Neurology"
};

const double specialtyRates[TOTAL_SPECIALTIES] = {1500.00,2500.00,4500.00,5000.00};

const int consultationDurations[TOTAL_SPECIALTIES] = {15,20,30,30};

const char *wardTitles[TOTAL_WARDS] = {
    "General Ward",
    "Paediatric Ward",
    "Surgical Ward",
    "ICU (Intensive Care Unit)"
};

const double wardDailyRates[TOTAL_WARDS] = {3000.00,6000.00,12000.00,25000.00};

const int wardBedLimits[TOTAL_WARDS] = {20,10,10,5};

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
