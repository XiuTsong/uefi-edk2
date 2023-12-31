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
    return (EFI_STATUS)InitFileLayer();
  } else {
    return EFI_SUCCESS;
  }

  return EFI_SUCCESS;
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