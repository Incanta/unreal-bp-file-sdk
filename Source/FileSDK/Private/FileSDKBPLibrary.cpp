// Copyright Incanta Games. All Rights Reserved.

#include "FileSDKBPLibrary.h"
#include "FileSDK.h"

#include "HAL/FileManagerGeneric.h"

UFileSDKBPLibrary::UFileSDKBPLibrary(
  const FObjectInitializer& ObjectInitializer
) : Super(ObjectInitializer) {
  //
}

void UFileSDKBPLibrary::CreateFile(
  FString FileName,
  bool ClearContentsIfExists,
  bool CreateDirectoryTree
) {
  IPlatformFile & PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

  FString directoryName = FPaths::GetPath(FileName);
  bool directoryExists = PlatformFile.DirectoryExists(*directoryName);

  if (!directoryExists && CreateDirectoryTree) {
    FFileManagerGeneric::Get().MakeDirectory(*directoryName, true);
  } else if (!directoryExists) {
    UE_LOG(
      LogFileSDK,
      Fatal,
      TEXT("Cannot create file %s because directory %s doesn't exist. Ensure the directory exists before or enable 'CreateDirectoryTree'"),
      *FileName,
      *directoryName
    );
  }

  bool fileExists = PlatformFile.FileExists(*FileName);

  if ((ClearContentsIfExists && fileExists) || !fileExists) {
    FFileHelper::SaveStringToFile(FString(), *FileName);
  }
}

bool UFileSDKBPLibrary::DeleteFile(FString FileName) {
  IPlatformFile & PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

  if (PlatformFile.FileExists(*FileName)) {
    return FFileManagerGeneric::Get().Delete(*FileName);
  } else {
    return false;
  }
}

bool UFileSDKBPLibrary::CreateDirectory(
  FString DirectoryName,
  bool CreateDirectoryTree
) {
  return FFileManagerGeneric::Get().MakeDirectory(*DirectoryName, CreateDirectoryTree);
}

bool UFileSDKBPLibrary::DeleteDirectory(
  FString DirectoryName,
  bool Recursive
) {
  return FFileManagerGeneric::Get().DeleteDirectory(*DirectoryName, false, Recursive);
}

bool UFileSDKBPLibrary::RenameFileOrDirectory(
  FString Source,
  FString Destination
) {
  IPlatformFile & PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

  return PlatformFile.MoveFile(*Destination, *Source);
}

bool UFileSDKBPLibrary::CopyFile(
  FString Source,
  FString Destination
) {
  IPlatformFile & PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

  return PlatformFile.CopyFile(*Destination, *Source);
}

bool UFileSDKBPLibrary::CopyDirectory(
  FString Source,
  FString Destination,
  bool OverwriteDestination
) {
  IPlatformFile & PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

  return PlatformFile.CopyDirectoryTree(*Destination, *Source, OverwriteDestination);
}

bool UFileSDKBPLibrary::ReadStringFromFile(FString FileName, FString & Content) {
  return FFileHelper::LoadFileToString(Content, *FileName);
}

bool UFileSDKBPLibrary::WriteStringToFile(FString FileName, FString Content, bool Append) {
  if (Append) {
    return FFileHelper::SaveStringToFile(
      Content,
      *FileName,
      FFileHelper::EEncodingOptions::AutoDetect,
      &IFileManager::Get(),
      std::ios_base::app
    );
  } else {
    return FFileHelper::SaveStringToFile(Content, *FileName);
  }
}

bool UFileSDKBPLibrary::WriteBytesToFile(FString FileName, TArray<uint8> Content) {
  return FFileHelper::SaveArrayToFile(Content, *FileName);
}

bool UFileSDKBPLibrary::ReadBytesFromFile(FString FileName, TArray<uint8> & Content) {
  return FFileHelper::LoadFileToArray(Content, *FileName);
}

TArray<FString> UFileSDKBPLibrary::GetFilesFromDirectory(
  FString DirectoryToSearch,
  FString FilterFilesWithExtension,
  bool SearchSubfolders
) {
  TArray<FString> FileNames;
  IPlatformFile & PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

  if (SearchSubfolders) {
    PlatformFile.FindFilesRecursively(
      FileNames,
      *DirectoryToSearch,
      *FilterFilesWithExtension
    );
  } else {
    PlatformFile.FindFiles(
      FileNames,
      *DirectoryToSearch,
      *FilterFilesWithExtension
    );
  }

  return FileNames;
}

void UFileSDKBPLibrary::GetFileOrDirectoryInfo(FString Path, FFileInfo & Info) {
  IPlatformFile & PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

  FFileStatData data = PlatformFile.GetStatData(*Path);

  Info.CreationTime = data.CreationTime;
  Info.AccessTime = data.AccessTime;
  Info.ModificationTime = data.ModificationTime;
  Info.FileSize = data.FileSize;
  Info.bIsDirectory = data.bIsDirectory;
  Info.bIsReadOnly = data.bIsReadOnly;
  Info.bIsValid = data.bIsValid;
}
