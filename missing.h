#pragma once

#include <sal.h>

// source: http://www.rohitab.com/discuss/topic/39956-my-first-native-application/
NTSTATUS NTAPI NtDisplayString(PUNICODE_STRING);
NTSTATUS NTAPI NtDelayExecution(BOOLEAN,PLARGE_INTEGER);
NTSTATUS NTAPI NtTerminateProcess(HANDLE,NTSTATUS);

// copied from reactos/include/reactos/wine/winternl.h
typedef struct _SECTION_IMAGE_INFORMATION {
  PVOID TransferAddress;
  ULONG ZeroBits;
  SIZE_T MaximumStackSize;
  SIZE_T CommittedStackSize;
  ULONG SubSystemType;
  WORD SubsystemVersionLow;
  WORD SubsystemVersionHigh;
  ULONG GpValue;
  USHORT ImageCharacteristics;
  USHORT DllCharacteristics;
  USHORT Machine;
  BOOLEAN ImageContainsCode;
  UCHAR ImageFlags;
  ULONG LoaderFlags;
  ULONG ImageFileSize;
  ULONG CheckSum;
} SECTION_IMAGE_INFORMATION, *PSECTION_IMAGE_INFORMATION;

// copied from reactos/include/ndk/rtltypes.h
typedef struct _RTL_USER_PROCESS_INFORMATION
{
    ULONG Size;
    HANDLE ProcessHandle;
    HANDLE ThreadHandle;
    CLIENT_ID ClientId;
    SECTION_IMAGE_INFORMATION ImageInformation;
} RTL_USER_PROCESS_INFORMATION, *PRTL_USER_PROCESS_INFORMATION;

// copied from: reactos/include/ndk/rtlfuncs.h
NTSYSAPI NTSTATUS NTAPI
RtlCreateProcessParameters(
    _Out_ PRTL_USER_PROCESS_PARAMETERS *ProcessParameters,
    _In_ PUNICODE_STRING ImagePathName,
    _In_opt_ PUNICODE_STRING DllPath,
    _In_opt_ PUNICODE_STRING CurrentDirectory,
    _In_opt_ PUNICODE_STRING CommandLine,
    _In_opt_ PWSTR Environment,
    _In_opt_ PUNICODE_STRING WindowTitle,
    _In_opt_ PUNICODE_STRING DesktopInfo,
    _In_opt_ PUNICODE_STRING ShellInfo,
    _In_opt_ PUNICODE_STRING RuntimeInfo);
NTSYSAPI NTSTATUS NTAPI
RtlCreateUserProcess(
    _In_ PUNICODE_STRING ImageFileName,
    _In_ ULONG Attributes,
    _In_ PRTL_USER_PROCESS_PARAMETERS ProcessParameters,
    _In_opt_ PSECURITY_DESCRIPTOR ProcessSecutityDescriptor,
    _In_opt_ PSECURITY_DESCRIPTOR ThreadSecurityDescriptor,
    _In_opt_ HANDLE ParentProcess,
    _In_ BOOLEAN CurrentDirectory,
    _In_opt_ HANDLE DebugPort,
    _In_opt_ HANDLE ExceptionPort,
    _Out_ PRTL_USER_PROCESS_INFORMATION ProcessInfo);
NTSYSAPI NTSTATUS NTAPI
RtlDestroyProcessParameters(
    _In_ PRTL_USER_PROCESS_PARAMETERS ProcessParameters);

// copied from reactos/include/reactos/wine/winternl.h
NTSYSAPI NTSTATUS WINAPI NtResumeThread(HANDLE, PULONG);



// from http://stackoverflow.com/a/4638530/28128 and/or ReactOS
typedef enum _HARDERROR_RESPONSE_OPTION
{
    OptionAbortRetryIgnore,
    OptionOk,
    OptionOkCancel,
    OptionRetryCancel,
    OptionYesNo,
    OptionYesNoCancel,
    OptionShutdownSystem
} HARDERROR_RESPONSE_OPTION, *PHARDERROR_RESPONSE_OPTION;

typedef enum _HARDERROR_RESPONSE
{
    ResponseReturnToCaller,
    ResponseNotHandled,
    ResponseAbort,
    ResponseCancel,
    ResponseIgnore,
    ResponseNo,
    ResponseOk,
    ResponseRetry,
    ResponseYes,
    ResponseTryAgain,
    ResponseContinue
} HARDERROR_RESPONSE, *PHARDERROR_RESPONSE;

NTSYSAPI VOID NTAPI RtlInitUnicodeString(IN OUT PUNICODE_STRING DestinationString,
    IN PCWSTR SourceString);

NTSYSAPI NTSTATUS NTAPI NtRaiseHardError(IN NTSTATUS ErrorStatus,
    IN ULONG NumberOfParameters, IN ULONG UnicodeStringParameterMask,
    IN PULONG_PTR Parameters,
    IN HARDERROR_RESPONSE_OPTION ValidResponseOptions,
    OUT PHARDERROR_RESPONSE Response);

#define STATUS_SERVICE_NOTIFICATION_2 0x50000018

