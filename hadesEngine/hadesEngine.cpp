#include <iostream>
#include<vector>
#include "syscalls_mem.h"

struct Region {
    PVOID regionAddress;
    DWORD regionSize;
};
int main(int argc, char* argv[])
{
    std::vector<Region> regions;

    DWORD PID = 8008;
    HANDLE hProc;
    OBJECT_ATTRIBUTES objAtt;
    CLIENT_ID clientId;
    NTSTATUS status;
    clientId.UniqueProcess = (HANDLE)PID;
    clientId.UniqueThread = 0;
    InitializeObjectAttributes(&objAtt, NULL, 0, NULL, NULL);
    status = Sw3NtOpenProcess(
        &hProc,
        PROCESS_ALL_ACCESS,
        &objAtt,
        &clientId
    );
    
    PVOID addr = NULL;
    SIZE_T szLen = 0;
    MEMORY_BASIC_INFORMATION mbi = { 0 };
    while (Sw3NtQueryVirtualMemory(
        hProc,
        addr,
        MemoryBasicInformation,
        &mbi,
        sizeof(mbi),
        &szLen
    ) == 0) {
        addr = reinterpret_cast<PVOID>((DWORD_PTR)mbi.BaseAddress + mbi.RegionSize);
        if (mbi.BaseAddress == NULL)
            continue;
        if (mbi.RegionSize == 0)
            continue;
        Region reg = { 0 };
        reg.regionAddress = mbi.BaseAddress;
        reg.regionSize = mbi.RegionSize;
        regions.insert(regions.begin() , reg);

    }
    
    for (auto region = regions.begin(); region < regions.end(); region++)
    {
        printf("0x%p\n", region->regionAddress);
    }
}
