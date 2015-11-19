#include <windows.h>
#include <winternl.h>
#include <ntdef.h>
#include "missing.h"

#if 1

// source: http://www.rohitab.com/discuss/topic/39956-my-first-native-application/
void NtProcessStartup()
{
    LARGE_INTEGER delay;
    delay.QuadPart = -10000000;

    //UNICODE_STRING us;
    //RtlInitUnicodeString(&us, L"Message from native application.");
    //NtDisplayString(&us);

    HANDLE file;
    OBJECT_ATTRIBUTES attrs;
    UNICODE_STRING name;
    RtlInitUnicodeString(&name, L"\\Device\\HarddiskVolume7\\foobar.txt");
    InitializeObjectAttributes(&attrs, &name, OBJ_INHERIT, NULL, NULL);
    IO_STATUS_BLOCK ioStatusBlock;
    NTSTATUS status = NtCreateFile(&file,
        FILE_WRITE_DATA,
        &attrs,
        &ioStatusBlock,
        NULL,
        FILE_ATTRIBUTE_NORMAL,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        FILE_SUPERSEDE,
        FILE_NON_DIRECTORY_FILE,
        NULL, 0);

    NtDelayExecution(FALSE, &delay);
    NtTerminateProcess((HANDLE)-1,0);
}

#else

// source: http://stackoverflow.com/a/4638530/28128

void NtProcessStartup()
{
    HARDERROR_RESPONSE response;
    ULONG_PTR items[4] = {0};
    UNICODE_STRING text, title;
    RtlInitUnicodeString(&text,
        L"Hello, NT!\r\nDo you like this?\r\n"
        L"This is just about as pretty as the GUI will get.\r\n"
        L"This message will self-destruct in 5 seconds...");
    RtlInitUnicodeString(&title, L"Native Message Box!");
    items[0] = (ULONG_PTR)&text;
    items[1] = (ULONG_PTR)&title;
    items[2] = (ULONG_PTR)OptionYesNo;
    items[3] = (ULONG_PTR)5000;
    NtRaiseHardError(STATUS_SERVICE_NOTIFICATION_2, ARRAYSIZE(items),
        0x1 | 0x2 /*First two parameters are UNICODE_STRINGs*/, items,
        OptionOk  /* This is ignored, since we have a custom message box. */,
        &response);
    NtTerminateProcess((HANDLE)-1, 0);
}

#endif
