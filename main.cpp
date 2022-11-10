#include <iostream>
#include <windows.h>
#include <tlhelp32.h>
#include <psapi.h>
#include <iomanip>
#include <vector>
#include <algorithm>
using namespace std;

class Process {
    private: 
    // getting the processes & memory variables 
    PROCESSENTRY32 pe32;
    PROCESS_MEMORY_COUNTERS pmc;

    public:
    Process(){}
    Process(PROCESSENTRY32 _pe32, PROCESS_MEMORY_COUNTERS _pmc) {
        pe32 = _pe32;
        pmc = _pmc;
    }

    int getPID() {
        // storing PID into string 
        string pIdStr = to_string(pe32.th32ProcessID);
        try {
            int pId = stoi(pIdStr);
            return pId;
        } catch(exception &e) {
            return 0;
        }
        
    }
    string getImageName() {
        // storing image name into string 
        string name = pe32.szExeFile;
       return name;
    }
    int getMemUsage() {
        // storing memory usage into string
        string memUsageStr = to_string((pmc.WorkingSetSize) / 1024);
        try {
            int memUsage = stoi(memUsageStr);
        return memUsage;
        } catch(exception &e) {
            return 0;
        }
        

    }
};

class ProcessList {
    private:
    Process list[1024];
    vector<Process> listVec;
    int size;
    void makeList(){
        // handle process snapshot
        HANDLE hPSnap;
        // handle process 
        HANDLE hP;
        // process and memory vars
        PROCESSENTRY32 pe32;
        PROCESS_MEMORY_COUNTERS pmc;
        //counter
        int i = 0;

        // creating snap and seting the size of process
        hPSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        pe32.dwSize = sizeof(PROCESSENTRY32);

        do
        {
            // storing the handle process
            hP = OpenProcess( PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID );
            // storing memory usage info into pmc var 
            GetProcessMemoryInfo( hP, &pmc, sizeof(pmc));
            // storing the process into the list
            listVec.push_back(Process(pe32, pmc));

            i++;

        } while( Process32Next( hPSnap, &pe32 ) );

        size = i;

        CloseHandle( hPSnap );
    }
    
    public:
    ProcessList(){};
    void printList(int choice) {
        makeList();
        if(choice == 1) {
            sortImageName();
        } else if (choice == 2) {
            sortPID();
        } else if (choice == 3) {
            sortMemUsage();
        } else {
            cout << "Invalid number" << endl;
            exit(0);
        }
        
        cout << setw(15) << left << "Process ID" << "|" << setw(50) << left << "Image Name" << "| " <<  setw(20) << right << " memory usage" << endl;
        cout << "--------------------------------------------------------------------" << endl;

        for(int j = 0; j < size; j++) {
            cout << setw(15) << left << listVec[j].getPID() << "|";
            cout << setw(50) << left << listVec[j].getImageName() << "|";
            cout << setw(20) << right << listVec[j].getMemUsage() << " kb";
            cout << endl;
        }
    }
    void sortPID() {
        sort(listVec.begin(), listVec.end(), [](Process &l, Process &r){ return l.getPID() < r.getPID();});
    }
    void sortMemUsage() {
        sort(listVec.begin(), listVec.end(), [](Process &l, Process &r){ return l.getMemUsage() < r.getMemUsage();});
    }
    void sortImageName() {
        sort(listVec.begin(), listVec.end(), [](Process &l, Process &r){ return l.getImageName() < r.getImageName();});
    }
};

int main()
{
    cout << "you want to sort by 1) name 2) PID 3) Memory usage" << endl;
    int choice;
    cin >> choice;
    ProcessList processList;
    processList.printList(choice);
    return 0;
}