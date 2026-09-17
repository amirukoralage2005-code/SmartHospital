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

double calculateWaitTime(int specialtyIndex) {
    return specialtyQueueCount[specialtyIndex] * consultationDurations[specialtyIndex];
}

int findAvailableBed(int wardIndex) {
    int b;
    for (b = 0; b < wardBedLimits[wardIndex]; b++) {
        if (bedOccupancy[wardIndex][b] == 0) {
            return b;
        }
    }
    return -1;
}

double calculateSurcharge(int urgency, double baseFee) {
    if (urgency == 2) return baseFee * 0.20;
    if (urgency == 3) return baseFee * 0.50;
    return 0.0;
}

double calculateWardCost(int admitted, int wardIndex, int days) {
    if (admitted == 0) return 0.0;
    return wardDailyRates[wardIndex] * days;
}

double calculateDiscount(int age, double grossTotal) {
    if (age < 5 || age > 65) return grossTotal * 0.15;
    return 0.0;
}

void printBill(int idx) {
    int specialtyIdx = patientSpecialty[idx] - 1;
    int surchargePercent = (patientUrgency[idx] == 2) ? 20 : (patientUrgency[idx] == 3) ? 50 : 0;

    printf("\n====================================================\n");
    printf(" SMART HOSPITAL ADMISSION & BILL\n");
    printf("----------------------------------------------------\n");
    printf("Patient ID      : PAT-%d\n", 1000 + idx + 1);
    printf("Patient Name    : %s\n", patientNames[idx]);
    if (patientAges[idx] < 5 || patientAges[idx] > 65)
        printf("Age             : %d Years (15%% Subsidy Eligible)\n", patientAges[idx]);
    else
        printf("Age             : %d Years\n", patientAges[idx]);
    printf("Specialty       : %s\n", specialtyTitles[specialtyIdx]);

    if (patientAdmitted[idx] == 1)
        printf("Assigned Ward   : %s (Bed #%02d)\n", wardTitles[patientWard[idx] - 1], patientBed[idx]);
    else
        printf("Assigned Ward   : Outpatient (OPD)\n");

    printf("Urgency Level   : Level %d (%s)\n", patientUrgency[idx], urgencyText(patientUrgency[idx]));
    printf("----------------------------------------------------\n");
    printf("Base Consultation Fee   : LKR %.2f\n", patientBaseFee[idx]);
    printf("Emergency Surcharge     : LKR %.2f (%d%%)\n", patientSurcharge[idx], surchargePercent);
    if (patientAdmitted[idx] == 1)
        printf("Ward Stay Cost (%d Days) : LKR %.2f\n", patientDays[idx], patientWardCost[idx]);
    else
        printf("Ward Stay Cost          : LKR 0.00\n");
    printf("----------------------------------------------------\n");
    printf("Gross Total Bill        : LKR %.2f\n", patientGrossTotal[idx]);
    printf("Age Subsidy Discount    : LKR -%.2f (%s)\n", patientDiscount[idx], patientDiscount[idx] > 0 ? "15%" : "0%");
    printf("----------------------------------------------------\n");
    printf("Final Payable Amount    : LKR %.2f\n", patientFinalAmount[idx]);
    if (patientWaitTime[idx] == 0)
        printf("Estimated Waiting Time  : 0.00 mins (Immediate Attention)\n");
    else
        printf("Estimated Waiting Time  : %.2f mins\n", patientWaitTime[idx]);
    printf("====================================================\n");
}

void savePatientRecordToFile(int idx) {
    FILE *fp = fopen("patient_records.txt", "a");
    if (fp == NULL) {
        printf("\nWarning: could not write to patient_records.txt\n");
        return;
    }
    fprintf(fp, "PAT-%d | %s | Age:%d | Urgency:%s | Specialty:%s | Ward:%s | Days:%d | Final:LKR %.2f\n",
            1000 + idx + 1,
            patientNames[idx],
            patientAges[idx],
            urgencyText(patientUrgency[idx]),
            specialtyTitles[patientSpecialty[idx] - 1],
            patientAdmitted[idx] ? wardTitles[patientWard[idx] - 1] : "OPD",
            patientDays[idx],
            patientFinalAmount[idx]);
    fclose(fp);
}

void sortAndDisplayPriorityQueue() {
    int order[MAX_PATIENTS];
    int i, j, temp;

    if (patientCount == 0) {
        printf("\nNo patients registered yet.\n");
        return;
    }

    for (i = 0; i < patientCount; i++) order[i] = i;

    for (i = 0; i < patientCount - 1; i++) {
        for (j = 0; j < patientCount - 1 - i; j++) {
            if (patientUrgency[order[j]] < patientUrgency[order[j + 1]]) {
                temp = order[j];
                order[j] = order[j + 1];
                order[j + 1] = temp;
            }
        }
    }

    printf("\n------------- PATIENT PRIORITY WAITING QUEUE -------------\n");
    printf("%-4s %-10s %-20s %-10s %-20s %-12s\n", "Pos", "Pat.ID", "Name", "Urgency", "Specialty", "Final(LKR)");
    for (i = 0; i < patientCount; i++) {
        int p = order[i];
        printf("%-4d PAT-%-6d %-20s %-10s %-20s %-12.2f\n",
               i + 1, 1000 + p + 1, patientNames[p], urgencyText(patientUrgency[p]),
               specialtyTitles[patientSpecialty[p] - 1], patientFinalAmount[p]);
    }
}

void registerPatient() {
    int idx = patientCount;
    int specialtyIdx,wardIdx = -1, bedIdx = -1, i;

    if (patientCount >= MAX_PATIENTS) {
        printf("\nPatient records are full. Cannot register more patients.\n");
        return;
    }

    printf("\n---------------- PATIENT REGISTRATION ----------------\n");

    getValidatedString("Enter Patient Name: ", patientNames[idx], NAME_LEN);
    patientAges[idx] = getValidatedInt("Enter Patient Age: ", 0, 120);
    patientUrgency[idx] = getValidatedInt("Enter Urgency Level (1-Normal, 2-Urgent, 3-Critical): ", 1, 3);
    
    printf("\nAvailable Specialties:\n");
    for (i = 0; i < TOTAL_SPECIALTIES; i++)
        printf("  %d. %s\n", i + 1, specialtyTitles[i]);

    specialtyIdx = getValidatedInt("Select Specialty ID: ", 1, TOTAL_SPECIALTIES) - 1;
    patientSpecialty[idx] = specialtyIdx + 1;
    patientWaitTime[idx] = calculateWaitTime(specialtyIdx);
    specialtyQueueCount[specialtyIdx]++;
    
    patientAdmitted[idx] = getValidatedInt("Is Patient Admitted to a Ward? (1-Yes, 0-No): ", 0, 1);

    if (patientAdmitted[idx] == 1) {
        printf("\nAvailable Wards:\n");
        for (i = 0; i < TOTAL_WARDS; i++)
            printf("  %d. %s\n", i + 1, wardTitles[i]);

        wardIdx = getValidatedInt("Select Ward ID: ", 1, TOTAL_WARDS) - 1;
        patientDays[idx] = getValidatedInt("Enter Number of Days Admitted: ", 1, 365);

        bedIdx = findAvailableBed(wardIdx);
        if (bedIdx == -1) {
            printf("\nSorry, %s has no available beds right now.\n", wardTitles[wardIdx]);
            printf("Patient will be registered as Outpatient (OPD) instead.\n");
            patientAdmitted[idx] = 0;
            patientWard[idx] = 0;
            patientDays[idx] = 0;
            patientBed[idx] = 0;
        } else {
            bedOccupancy[wardIdx][bedIdx] = 1;
            patientWard[idx] = wardIdx + 1;
            patientBed[idx] = bedIdx + 1;
        }
    } else {
        patientWard[idx] = 0;
        patientDays[idx] = 0;
        patientBed[idx] = 0;
    }
    
    patientBaseFee[idx]    = specialtyRates[specialtyIdx];
    patientSurcharge[idx]  = calculateSurcharge(patientUrgency[idx], patientBaseFee[idx]);
    patientWardCost[idx]   = calculateWardCost(patientAdmitted[idx], patientWard[idx] - 1, patientDays[idx]);
    patientGrossTotal[idx] = patientBaseFee[idx] + patientSurcharge[idx] + patientWardCost[idx];
    patientDiscount[idx]   = calculateDiscount(patientAges[idx], patientGrossTotal[idx]);
    patientFinalAmount[idx]= patientGrossTotal[idx] - patientDiscount[idx];

    patientCount++;
    
    printBill(idx);
    savePatientRecordToFile(idx);
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
                registerPatient();
                break;
            case 3:
                sortAndDisplayPriorityQueue();
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

