#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PATIENTS 100
#define TOTAL_SPECIALTIES 4
#define TOTAL_WARDS 4
#define MAX_BEDS 20
#define NAME_LEN 60

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

const int specialtyDailyCap[TOTAL_SPECIALTIES] = {30, 20, 12, 10};

int bedOccupancy[TOTAL_WARDS][MAX_BEDS];

char  patientNames[MAX_PATIENTS][NAME_LEN];
int   patientAges[MAX_PATIENTS];
int   patientUrgency[MAX_PATIENTS];
int   patientSpecialty[MAX_PATIENTS];
int   patientAdmitted[MAX_PATIENTS];
int   patientWard[MAX_PATIENTS];
int   patientDays[MAX_PATIENTS];
int   patientBed[MAX_PATIENTS];
double patientWaitTime[MAX_PATIENTS];
double patientBaseFee[MAX_PATIENTS];
double patientSurcharge[MAX_PATIENTS];
double patientWardCost[MAX_PATIENTS];
double patientGrossTotal[MAX_PATIENTS];
double patientDiscount[MAX_PATIENTS];
double patientFinalAmount[MAX_PATIENTS];

int patientCount = 0;
int specialtyQueueCount[TOTAL_SPECIALTIES] = {0, 0, 0, 0};

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

void initializeBedOccupancy() {
    int w, b;
    for (w = 0; w < TOTAL_WARDS; w++)
        for (b = 0; b < MAX_BEDS; b++)
            bedOccupancy[w][b] = 0;
}

int main()
{
    int choice;
    
    initializeBedOccupancy();

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
