#include <stdio.h> 
#include <stdlib.h> 
#include <time.h>
#define SORT_BY_ARRIVAL 0 
#define SORT_BY_PID 1 
#define SORT_BY_BURST 2

#define SORT_BY_START 3 
 
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

typedef struct{ 
    int iPID; 
    int iArrival, iBurst; 
    int iRemaining;
    int iStart, iFinish, iWaiting, iResponse, iTaT; 
} PCB; 
 
void inputProcess(int n, PCB P[]) {
    for (int i=0; i < n; i++) {
        P[i].iPID = i+1;
        // printf("PID %d\n", P[i].iPID);
        // printf("Arrival time: ");
        // scanf("%d", &P[i].iArrival);
        // printf("Burst time: ");
        // scanf("%d", &P[i].iBurst);
        P[i].iArrival = rand() % 21;
        P[i].iBurst = rand() % 11 + 2;
        P[i].iStart = 0;
        P[i].iFinish = 0;
        P[i].iWaiting = 0;
        P[i].iResponse = 0;
        P[i].iTaT = 0;
        P[i].iRemaining = P[i].iBurst;
    }
}
 
void printProcess(int n, PCB P[]) {
    printf("-------------------------------------------------------------\n");
    printf("| %-5s | %-8s | %-6s | %-6s | %-6s | %-9s | %-8s | %-8s |\n", 
           "PID", "Arrival", "Burst", "Start", "Finish", "Waiting", "Response", "Turnaround");
    printf("-------------------------------------------------------------\n");

    for (int i = 0; i < n; i++) {
        printf("| %-5d | %-8d | %-6d | %-6d | %-6d | %-9d | %-8d | %-8d |\n", 
               P[i].iPID, P[i].iArrival, P[i].iBurst, P[i].iStart, 
               P[i].iFinish, P[i].iWaiting, P[i].iResponse, P[i].iTaT);
    }
    printf("-------------------------------------------------------------\n");
}
 
void exportGanttChart(int n, PCB P[]) {
    printf("Gantt Chart:\n");
    for (int i = 0; i < n; i++) {
        printf("| P%d ", P[i].iPID);

        if (i < n-1) {
            int flag = 0;
            for (int j = i+1; j<n; j++)
                if (P[j].iPID != P[i].iPID) 
                {
                    flag = 1;
                    break;
                }
            if (flag == 0)
                break;

            if (P[i].iFinish < P[i+1].iArrival)
                printf("|    ");
        }
    }
    printf("|\n");
    
    for (int i = 0; i < n; i++) {
        if (i > 0)
            if (P[i-1].iFinish < P[i].iArrival) {
                printf("%d    ", P[i-1].iFinish);
                
                if (P[i-1].iFinish > 9)
                    printf("\b");
            }

        printf("%d    ", P[i].iStart);
        
        if (P[i+1].iStart > 9)
            printf("\b");
        int flag = 0;
        for (int j = i+1; j<n; j++)
            if (P[j].iPID != P[i].iPID) 
            {
                flag = 1;
                break;
            }
        if (flag == 0)
            break;
    }

    printf("%d\n", P[n-1].iFinish);
}

void pushProcess(int *n, PCB P[], PCB Q) {
    P[*n] = Q;
    (*n)++;
}

void removeProcess(int *n, int index, PCB P[]) {
    for (int i = index; i < *n - 1; i++) {
        P[i] = P[i + 1];
    }
    (*n)--;
}

int swapProcess(PCB *P, PCB *Q) {
    PCB temp = *P;
    *P = *Q;
    *Q = temp;
    return 0;
}

int partition(PCB P[], int low, int high, int iCriteria) {
    PCB pivot = P[high];
    int i = (low - 1);
    for (int j = low; j <= high - 1; j++) {
        int condition = 0;
        if (iCriteria == SORT_BY_ARRIVAL) {
            condition = (P[j].iArrival < pivot.iArrival) ||
                        (P[j].iArrival == pivot.iArrival && P[j].iBurst < pivot.iBurst);
        } else if (iCriteria == SORT_BY_PID) {
            condition = (P[j].iPID < pivot.iPID);
        } else if (iCriteria == SORT_BY_BURST) {
            condition = (P[j].iBurst < pivot.iBurst) ||
                        (P[j].iBurst == pivot.iBurst && P[j].iArrival < pivot.iArrival);
        } else if (iCriteria == SORT_BY_START) {
            condition = (P[j].iStart < pivot.iStart);
        }
        if (condition) {
            i++;
            swapProcess(&P[i], &P[j]);
        }
    }
    swapProcess(&P[i + 1], &P[high]);
    return (i + 1);
}

void quickSort(PCB P[], int low, int high, int iCriteria) {
    if (low < high) {
        int pi = partition(P, low, high, iCriteria);
        quickSort(P, low, pi - 1, iCriteria);
        quickSort(P, pi + 1, high, iCriteria);
    }
}

void calculateAWT(int n, PCB P[]) {
    int totalWaitingTime = 0;
    for (int i = 0; i < n-1; i++) {
        if (P[i].iPID < P[i+1].iPID)
            totalWaitingTime += P[i].iWaiting;
        if (P[i].iPID == n-1)
            break;
    }
    totalWaitingTime += P[n-1].iWaiting;
    double averageWaitingTime = (double)totalWaitingTime / n;
    printf("Average Waiting Time: %.2f\n", averageWaitingTime);
}

void calculateATaT(int n, PCB P[]) {
    int totalTaT = 0;
    for (int i = 0; i < n-1; i++) {
        if (P[i].iPID < P[i+1].iPID)
            totalTaT += P[i].iTaT;
        if (P[i].iPID == n-1)
            break;
    }
    totalTaT += P[n-1].iTaT;
    double averageTaT = (double)totalTaT / n;
    printf("Average Turnaround Time: %.2f\n", averageTaT);
}
 
int main() 
{
    srand(time(NULL));

    int iNumberOfProcess; 
    printf("Please input number of Process: "); 
    scanf("%d", &iNumberOfProcess); 
    int iRemain = iNumberOfProcess, iReady = 0, iTerminated = 0, iGantt = 0;
    int iCurrentTime, iQuantumTime;

    PCB Input[iNumberOfProcess]; 
    PCB ReadyQueue[iNumberOfProcess]; 
    PCB TerminatedArray[iNumberOfProcess]; 
    PCB GanttArray[1000];

    printf("Please input Quantum time: ");
    scanf("%d", &iQuantumTime);
    

    inputProcess(iNumberOfProcess, Input); 
    quickSort(Input, 0, iNumberOfProcess - 1, SORT_BY_ARRIVAL); 

   
    printProcess(iNumberOfProcess, Input);

    pushProcess(&iReady, ReadyQueue, Input[0]); 
    removeProcess(&iRemain, 0, Input); 
 
    ReadyQueue[0].iStart = ReadyQueue[0].iArrival; 
    ReadyQueue[0].iFinish = MIN(ReadyQueue[0].iArrival+iQuantumTime, ReadyQueue[0].iArrival+ReadyQueue[0].iBurst); 
    ReadyQueue[0].iResponse = ReadyQueue[0].iStart - ReadyQueue[0].iArrival; 
    ReadyQueue[0].iWaiting = ReadyQueue[0].iResponse; 
    ReadyQueue[0].iTaT = ReadyQueue[0].iFinish - ReadyQueue[0].iArrival; 
    iCurrentTime = MIN(ReadyQueue[0].iArrival+iQuantumTime, ReadyQueue[0].iArrival+ReadyQueue[0].iBurst); 
    ReadyQueue[0].iRemaining -= MIN(iQuantumTime, ReadyQueue[0].iRemaining);

    printf("\nReady Queue: "); 
    printProcess(1, ReadyQueue); 
 
    while (iTerminated < iNumberOfProcess) 
    { 
        while (iRemain > 0) 
        { 
            if (Input[0].iArrival <= ReadyQueue[0].iFinish) 
            { 
                pushProcess(&iReady, ReadyQueue, Input[0]); 
                removeProcess(&iRemain, 0, Input);   
                continue; 
            } 
            else 
            { 
                if (iReady == 1) 
                {
                    pushProcess(&iReady, ReadyQueue, Input[0]); 
                    removeProcess(&iRemain, 0, Input);   
                }
                break; 
            }
        } 
 
        if (iReady > 0) 
        { 
            while ((iReady > 1 && ReadyQueue[0].iFinish < ReadyQueue[1].iArrival || iReady == 1) && 
                    ReadyQueue[0].iRemaining > 0) 
            {   
                ReadyQueue[0].iFinish += MIN(iQuantumTime, ReadyQueue[0].iRemaining);
                iCurrentTime += MIN(iQuantumTime, ReadyQueue[0].iRemaining);
                ReadyQueue[0].iRemaining -= MIN(iQuantumTime, ReadyQueue[0].iRemaining);
                ReadyQueue[0].iTaT = ReadyQueue[0].iFinish - ReadyQueue[0].iArrival; 
                ReadyQueue[0].iWaiting = ReadyQueue[0].iTaT - ReadyQueue[0].iBurst; 
            }

            if (ReadyQueue[0].iRemaining > 0) {
                pushProcess(&iReady, ReadyQueue, ReadyQueue[0]);
            }
            else 
            {
                pushProcess(&iTerminated, TerminatedArray, ReadyQueue[0]); 
            }

            pushProcess(&iGantt, GanttArray, ReadyQueue[0]);
            removeProcess(&iReady, 0, ReadyQueue); 


            if (ReadyQueue[0].iArrival <= GanttArray[iGantt - 1].iFinish)
            {
                ReadyQueue[0].iStart = GanttArray[iGantt - 1].iFinish; 
            }
            else 
            {
                ReadyQueue[0].iStart = ReadyQueue[0].iArrival; 
                iCurrentTime = ReadyQueue[0].iStart;
            }

            ReadyQueue[0].iFinish = MIN(iCurrentTime + ReadyQueue[0].iRemaining, iCurrentTime + iQuantumTime); 
            iCurrentTime += MIN(iQuantumTime, ReadyQueue[0].iRemaining);
            ReadyQueue[0].iRemaining -= MIN(iQuantumTime, ReadyQueue[0].iRemaining);
            ReadyQueue[0].iResponse = ReadyQueue[0].iStart - ReadyQueue[0].iArrival; 
            ReadyQueue[0].iTaT = ReadyQueue[0].iFinish - ReadyQueue[0].iArrival; 
            ReadyQueue[0].iWaiting = ReadyQueue[0].iTaT - ReadyQueue[0].iBurst; 
        } 
    } 
 
    printf("\n===== RR Scheduling =====\n"); 
    exportGanttChart(iGantt, GanttArray); 
    
    quickSort(TerminatedArray, 0, iTerminated - 1, SORT_BY_PID); 
 
    calculateAWT(iTerminated, TerminatedArray); 
    calculateATaT(iTerminated, TerminatedArray); 
     
    return 0; 
} 