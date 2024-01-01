#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Protocol/SimpleFileSystem.h>
#include <Protocol/RealTimeClock.h>
#include <Library/MemoryAllocationLib.h>

#include "EasyDefs.h"
#include "EasyFile.h"

EFI_EVENT mSetVirtualAddressMapEvent = NULL;
VOID *gMemoryPool;
#define TOTAL_BYTE 102400

VOID
EFIAPI
NotifySetVirtualAddressMap (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  EFI_STATUS  Status;
  Status = EfiConvertPointer (
             EFI_OPTIONAL_PTR,
             (VOID **)&gMemoryPool
             );
  if (EFI_ERROR (Status)) {
  }
}

VOID
InitFileSystem(
  VOID
  )
{
  InitBlockLayer(gMemoryPool, TOTAL_BYTE);
  InitFileLayer();
}

EFI_STATUS
EFIAPI
SampleRuntimeService (
  IN    CONST   CHAR8     *Cmd,
  IN    CONST   UINT32    Num,
  IN OUT        CHAR8     **Args
)
{
  // **args** params
  // Cmd        params
  // create     char* name
  // read       char* name, int byte_size, char* buffer
  // write      char* name, char* content, int byte_size
  // remove     char* name
  // start
  // ls         char* buffer
  // pwd        char* buffer
  // mkdir      char* name
  // cat        char* name, char* buffer
  // touch      char* name
  if(AsciiStrCmp(Cmd, "create") == 0) {
    CHAR8* name = Args[0];
    return (EFI_STATUS)EasyCreateFile(name);
  } else if (AsciiStrCmp(Cmd, "read") == 0) {
    CHAR8* filename = Args[0];
    UINTN byte_size = 0;
    AsciiStrDecimalToUintnS(Args[1], NULL, &byte_size);
    CHAR8* buffer = Args[2];
    return (EFI_STATUS)EasyReadFile(filename, byte_size, buffer);
  } else if (AsciiStrCmp(Cmd, "write") == 0) {
    CHAR8* filename = Args[0];
    CHAR8* buffer = Args[1];
    UINTN byte_size = 0;
    AsciiStrDecimalToUintnS(Args[2], NULL, &byte_size);
    return (EFI_STATUS)EasyWriteFile(filename, byte_size, buffer);
  } else if (AsciiStrCmp(Cmd, "remove") == 0) {
    CHAR8* filename = Args[0];
    return (EFI_STATUS)EasyRemoveFile(filename);
  } else if (AsciiStrCmp(Cmd, "start") == 0) {
    InitFileSystem();
    return EFI_SUCCESS;
  } else if (AsciiStrCmp(Cmd, "ls") == 0) {
    if (Num == 0) {
      CHAR8* buffer = Args[0];
      return (EFI_STATUS)EasyLs(buffer);
    }
    CHAR8 *DirName = Args[0];
    CHAR8 *buffer = Args[1];
    return EasyDirListFiles(DirName, buffer);
  } else if (AsciiStrCmp(Cmd, "pwd") == 0) {
    CHAR8* buffer = Args[0];
    return (EFI_STATUS)EasyPwd(buffer);
  } else if (AsciiStrCmp(Cmd, "mkdir") == 0) {
    CHAR8* name = Args[0];
    return (EFI_STATUS)EasyMkdir(name);
  } else if (AsciiStrCmp(Cmd, "cat") == 0) {
    CHAR8* filename = Args[0];
    CHAR8* buffer = Args[1];
    return (EFI_STATUS)EasyCat(filename, buffer);
  } else if (AsciiStrCmp(Cmd, "touch") == 0) {
    CHAR8* filename = Args[0];
    return (EFI_STATUS)EasyTouch(filename);
  } else if (AsciiStrCmp(Cmd, "cd") == 0) {
    CHAR8* DirName = Args[0];
    return (EFI_STATUS)EasyCd(DirName);
  } else {
    return EASY_CMD_NOT_FOUND_ERROR;
  }
}

EFI_STATUS
EFIAPI
SampleRuntimeDriverEntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS  Status;
  DEBUG ((DEBUG_INFO, "%a(): XXX_init_sample_runtime_service!\n", __FUNCTION__));

  SystemTable->RuntimeServices->SampleRuntimeService       = SampleRuntimeService;
  gMemoryPool = AllocateRuntimePool (sizeof(char) * 102400);

  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,                   // Type
                  TPL_NOTIFY,                          // NotifyTpl
                  NotifySetVirtualAddressMap,          // NotifyFunction
                  NULL,                                // NotifyContext
                  &gEfiEventVirtualAddressChangeGuid,  // EventGroup
                  &mSetVirtualAddressMapEvent          // Event
                  );

  if (EFI_ERROR (Status)) {
    return Status;
  }

  return EFI_SUCCESS;
}