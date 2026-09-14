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


void displayLookupData() {
    int i;
    printf("\n---------------- DOCTOR SPECIALTIES ----------------\n");
    printf("%-4s %-25s %-15s %-15s %-10s\n", "ID", "Specialty Name", "Fee (LKR)", "Time/Patient", "Daily Cap");
    for (i = 0; i < TOTAL_SPECIALTIES; i++) {
        printf("%-4d %-25s %-15.2f %-15d %-10d\n",
               i + 1, specialtyTitles[i], specialtyRates[i], consultationDurations[i], specialtyDailyCap[i]);
    }

    printf("\n------------------ HOSPITAL WARDS ------------------\n");
    printf("%-4s %-25s %-18s %-12s\n", "ID", "Ward Name", "Daily Rate (LKR)", "Capacity");
    for (i = 0; i < TOTAL_WARDS; i++) {
        printf("%-4d %-25s %-18.2f %-12d\n",
               i + 1, wardTitles[i], wardDailyRates[i], wardBedLimits[i]);
    }
}

void displayBedOccupancy(void) {
    int w, b, occupied;
    printf("\n------------------ BED OCCUPANCY --------------------\n");
    for (w = 0; w < TOTAL_WARDS; w++) {
        occupied = 0;
        printf("\n%s (Capacity: %d)\n[ ", wardTitles[w], wardBedLimits[w]);
        for (b = 0; b < wardBedLimits[w]; b++) {
            printf("%s ", bedOccupancy[w][b] == 1 ? "O" : "A");
            if (bedOccupancy[w][b] == 1) occupied++;
        }
        printf("]\n");
        printf("Occupied: %d / %d  |  Available: %d\n", occupied, wardBedLimits[w], wardBedLimits[w] - occupied);
    }
    printf("(A = Available, O = Occupied)\n");
}

void loadBedStatus() {
    FILE *fp = fopen("beds_status.txt", "r");
    int w, b;

    if (fp == NULL) {
        return;
    }

    for (w = 0; w < TOTAL_WARDS; w++) {
        for (b = 0; b < MAX_BEDS; b++) {
            fscanf(fp, "%d", &bedOccupancy[w][b]);
        }
    }
    fclose(fp);
    printf("\n[Bed occupancy status loaded from beds_status.txt]\n");
}

void saveBedStatus() {
    FILE *fp = fopen("beds_status.txt", "w");
    int w, b;

    if (fp == NULL) {
        printf("\nError: could not save bed status.\n");
        return;
    }

    for (w = 0; w < TOTAL_WARDS; w++) {
        for (b = 0; b < MAX_BEDS; b++) {
            fprintf(fp, "%d ", bedOccupancy[w][b]);
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
}

int getValidatedInt(const char *prompt, int min, int max) {
    int value;

    while (1) {
        printf("%s", prompt);

        if (scanf("%d", &value) == 1) {
            if (value >= min && value <= max)
                return value;

            printf("Please enter a value between %d and %d.\n", min, max);
        } else {
            printf("Invalid input! Please enter a whole number.\n");
        }

        while (getchar() != '\n');
    }
}

void getValidatedString(const char *prompt, char *buffer, int size) {
    size_t len;

    while (getchar() != '\n');

    do {
        printf("%s", prompt);
        fgets(buffer, size, stdin);

        len = strlen(buffer);

        if (buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
            len--;
        }

        if (len == 0)
            printf("Input cannot be empty.\n");

    } while (len == 0);
}

const char *urgencyText(int level) {
    switch (level) {
        case 1: return "Normal";
        case 2: return "Urgent";
        default: return "Critical";
    }
}

int main()
{
    int choice;
    
    initializeBedOccupancy();
    loadBedStatus();

    do {
        displayMenu();
        if (scanf("%d", &choice) != 1) {
            continue;
        }

        switch (choice) {
            case 1:
                displayLookupData();
                displayBedOccupancy();
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
                saveBedStatus();
                printf("Bed status saved. Exiting system...\n");
                break;
            default:
                printf("\nInvalid selection! Choice must be between 1 and 5.\n");
        }
    } while (choice != 5);

    return 0;
}

