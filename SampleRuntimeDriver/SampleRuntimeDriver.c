#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeLib.h>
#include <Protocol/RealTimeClock.h>

EFI_STATUS
EFIAPI
SampleRuntimeService (
  IN    CONST   CHAR8     *Cmd,
  IN    CONST   UINT32    Num,
  IN OUT        CHAR8     **Args
)
{
  DEBUG ((DEBUG_INFO, "%a(): Hello!\n", __FUNCTION__));

  // **args** params
  // Cmd        params
  // create     char* name
  // read       char* name, int byte_size, char* buffer
  // write      char* name, char* content, int byte_size
  // remove     char* name
  if(AsciiStrCmp(Cmd, "create") == 0) {
    CHAR8* name = Args[0];
    return EasyCreateFile(name);
  } else if (AsciiStrCmp(Cmd, "read") == 0) {
    CHAR8* filename = Args[0];
    UINTN byte_size = 0;
    AsciiStrDecimalToUintnS(Args[1], NULL, &byte_size);
    CHAR8* buffer = Args[2];
    return EasyReadFile(Cmd, byte_size, buffer);
    *Num = byte_size;
  } else if (AsciiStrCmp(Cmd, "write") == 0) {
    CHAR8* filename = Args[0];
    CHAR8* buffer = Args[1];
    UINTN byte_size = 0;
    AsciiStrDecimalToUintnS(Args[2], NULL, &byte_size);
    return EasyWriteFile(Cmd, byte_size, buffer);
  } else if (AsciiStrCmp(Cmd, "remove") == 0) {
    CHAR8* filename = Args[0];
    return EasyRemoveFile(filename);
  } else if (AsciiStrCmp(Cmd, "start") == 0) {
    return InitFileLayer();
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
  DEBUG ((DEBUG_INFO, "%a(): XXX_init_sample_runtime_service!\n", __FUNCTION__));

  SystemTable->RuntimeServices->SampleRuntimeService       = SampleRuntimeService;

  return EFI_SUCCESS;
}