#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeLib.h>
#include <Protocol/RealTimeClock.h>

UINT32 KEY = 123;

EFI_STATUS
EFIAPI
SampleRuntimeService (
  OUT UINT32               *key
)
{
  DEBUG ((DEBUG_INFO, "%a(): Hello!\n", __FUNCTION__));
  *key = KEY;
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
SampleRuntimeDriverEntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  DEBUG ((DEBUG_INFO, "%a(): XXX_init_sample_runtime_service!\n", __FUNCTION__));

  SystemTable->RuntimeServices->SampleRuntimeService       = SampleRuntimeService;

  return EFI_SUCCESS;
}