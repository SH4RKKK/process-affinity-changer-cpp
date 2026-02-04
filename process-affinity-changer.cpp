#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <thread>
#include <fstream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <sstream>

const char* Filename = "AffinityNumber.txt";
int Affinity = NULL;
HANDLE Process = NULL;
DWORD proc_id;

void ExitApp()
{
    std::chrono::milliseconds timespan(3000);
    std::cout << "The executable will exit in 3 seconds." << std::endl;
    std::this_thread::sleep_for(timespan);
    exit(0);
}

bool attach_to_proc(char* procName)
{
    PROCESSENTRY32 procEntry32;
    procEntry32.dwSize = sizeof(PROCESSENTRY32);
    auto hProcSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (hProcSnap == INVALID_HANDLE_VALUE)
    {
        std::cout << "Snapshot Failed" << std::endl;
        ExitApp();
    }

    while (Process32Next(hProcSnap, &procEntry32))
    {
        if (!strcmp(procName, procEntry32.szExeFile))
        {
            Process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procEntry32.th32ProcessID);
            proc_id = procEntry32.th32ProcessID;

            if (Process == NULL)
            {
                std::cout << "Failed to get handle to process" << std::endl;
                ExitApp();
                return false;
            }
            return true;
        }
    }
    std::cout << "Didnt find " << procName << std::endl;
    ExitApp();
    return false;

}

int main()
{
    std::cout << "The executable will close if user gives an invalid entry." << std::endl;

    char* TargetProcess;
    std::fstream file;
    file.open(Filename);
    if (file.is_open())
    {
        std::string line;
        for (int i = 0; getline(file, line) && i < 2; i++)
        {
            if (i == 0)
            {
                Affinity = std::stoi(line);
            }

            if (i == 1)
            {
                TargetProcess = &line[0];
                if (NULL < Affinity and Affinity < 65535 and TargetProcess != NULL)
                {
                    attach_to_proc((char*)TargetProcess);
                    BOOL Success = SetProcessAffinityMask(Process, Affinity);
                    if (Success == 0)
                    {
                        std::cout << "Failed to set affinity." << std::endl;
                        ExitApp();
                    }
                    std::cout << "Affinity has been set successfully." << std::endl;
                    ExitApp();
                }

            std::cout << "Starting configuration:" << std::endl;
            }
        }
    }
    else
    {
        std::cout << Filename << " has not been found." << std::endl;
    }
    file.close();

    int Affinity = NULL;
    std::string TargetInput;
    char* TargetProcess2 = NULL;
    std::cout << "Give the name of the process, Example = Client.exe" << std::endl;
    std::cin >> TargetInput;
    TargetProcess2 = &TargetInput[0];
    attach_to_proc((char*)TargetProcess2);
    if (Process != NULL)
    {
        std::cout << "Process found" << std::endl;
    }

    int CPU_0 = 1, CPU_1 = 2, CPU_2 = 4, CPU_3 = 8, CPU_4 = 16, CPU_5 = 32, CPU_6 = 64, CPU_7 = 128, CPU_8 = 256, CPU_9 = 512, CPU_10 = 1024, CPU_11 = 2048, CPU_12 = 4096, CPU_13 = 8192, CPU_14 = 16384, CPU_15 = 32768;
    int AllCores[16]{ CPU_0, CPU_1, CPU_2, CPU_3, CPU_4, CPU_5, CPU_6, CPU_7, CPU_8, CPU_9, CPU_10, CPU_11, CPU_12, CPU_13, CPU_14, CPU_15, };

    const int Thread_count = std::thread::hardware_concurrency();
    std::cout << "Thread count = " << Thread_count << std::endl;

    std::cout << "How many threads are you setting the affinity for " << TargetProcess2 << std::endl;
    int UserInput;
    std::cin >> UserInput;
    if (UserInput > Thread_count)
    {
        std::cout << "You dont have that many threads." << std::endl;
        ExitApp();
    }

    const int ArraySize = 16;
    int SelectedCoresPrint[ArraySize];

    std::cout << "Which threads do you want to use? Press Enter after every entry. Range is 0-" << Thread_count - 1 << "." << std::endl;
    for (int i = 0; i < UserInput; i++)
    {
        if (std::cin >> SelectedCoresPrint[i])
        {
            if (SelectedCoresPrint[i] > (Thread_count - 1) or SelectedCoresPrint[i] < 0)
            {
                std::cout << "You have selected cores out of range." << std::endl;
                ExitApp();
            }
            Affinity += Affinity + AllCores[SelectedCoresPrint[i]];
        }
        else
        {
            std::cout << "This is not a number." << std::endl;
            ExitApp();
        }
    }

    std::cout << "Affinity number = " << Affinity << std::endl;
    const char* SelectedCores2[16]{ "CPU_0", "CPU_1", "CPU_2", "CPU_3", "CPU_4", "CPU_5", "CPU_6", "CPU_7", "CPU_8", "CPU_9", "CPU_10", "CPU_11", "CPU_12", "CPU_13", "CPU_14", "CPU_15" };
    std::cout << "Set Affinity to following cores?" << std::endl;
    for (int j = 0; j < UserInput; j++)
    {
        std::cout << SelectedCores2[SelectedCoresPrint[j]] << std::endl;
    }
    system("Pause");
    BOOL Success = SetProcessAffinityMask(Process, Affinity);
    if (Success == 1)
    {
        file.open(Filename);
        if (file.is_open())
        {
            file << Affinity << "\n";
            file << TargetProcess2;
            std::cout << "Affinity has been set successfully and configuration has been saved in " << Filename << std::endl;
            file.close();
            ExitApp();
        }
        else
        {
            std::cout << "Affinity has been set successfully" << std::endl;
            ExitApp();
        }
    }
    else
    {
        std::cout << "Failed to set affinity." << std::endl;
        ExitApp();
    }
}