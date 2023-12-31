#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Protocol/SimpleFileSystem.h>
#include <Protocol/RealTimeClock.h>
#include <Library/MemoryAllocationLib.h>

UINT32 KEY = 123;
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
  OUT UINT32               *key
)
{
  char *ch;
  *key = *(UINT32 *)gMemoryPool;
  ch = (char *)key;
  DEBUG ((DEBUG_INFO, "%c\n", ch));

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
  *(UINT32 *)gMemoryPool = 777;

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