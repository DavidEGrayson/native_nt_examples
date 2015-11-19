// source: http://www.rohitab.com/discuss/topic/41379-running-native-applications-with-rtlcreateuserprocess/

#include <stdio.h>
#include <windows.h>
#include <winternl.h>
#include "missing.h"

int wmain(int argc, wchar_t ** argv)
{
    wchar_t Path[512], CmdLine[512];
    PRTL_USER_PROCESS_PARAMETERS UserProcessParam;

    UNICODE_STRING FileName,CommandLine;
    RTL_USER_PROCESS_INFORMATION ProcessInfo;

    if (argc < 2)
    {
        fprintf(stderr, "Usage: runnt.exe [filename] [command line]\n");
        return 1;
    }

    if (!SearchPath(NULL, argv[1], L".exe", 512, Path, NULL))
    {
        fprintf(stderr, "Error: File not found\n");
        return 1;
    }

    if (!RtlDosPathNameToNtPathName_U(Path, &FileName, NULL, NULL))
    {
        fprintf(stderr, "Error: Unable to convert path name.\n");
        return 1;
    }

    if (argc > 2)
    {
        swprintf(CmdLine, 512, L"\"%ws\" %ws", Path, argv[2]);
        RtlInitUnicodeString(&CommandLine,CmdLine);
    }

    NTSTATUS status = RtlCreateProcessParameters(
        &UserProcessParam, &FileName, NULL, NULL,
        argc > 2 ? &CommandLine : NULL,
        NULL, NULL, NULL, NULL, NULL);
    if(!NT_SUCCESS(status))
    {
        fprintf(stderr, "Error: Unable to create process parameters.\n");
        RtlFreeUnicodeString(&FileName);
        return 1;
    }

    status = RtlCreateUserProcess(
        &FileName, OBJ_CASE_INSENSITIVE, UserProcessParam, NULL, NULL,
        NULL, FALSE, NULL, NULL, &ProcessInfo);
    if(!NT_SUCCESS(status))
    {
        fprintf(stderr, "Error: Unable to create process.\n");

        RtlDestroyProcessParameters(UserProcessParam);
        RtlFreeUnicodeString(&FileName);
        return 1;
    }

    printf("Process created.\n");

    NtResumeThread(ProcessInfo.ThreadHandle, NULL);

    printf("Waiting for new process to terminate.\n");
    NtWaitForSingleObject(ProcessInfo.ProcessHandle, FALSE, NULL);

    printf("Process terminated.\n");
    RtlDestroyProcessParameters(UserProcessParam);

    NtClose(ProcessInfo.ThreadHandle);
    NtClose(ProcessInfo.ProcessHandle);

    return 0;
}
