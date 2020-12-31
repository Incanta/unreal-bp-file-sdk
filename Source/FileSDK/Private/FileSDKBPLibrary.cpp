// Copyright Incanta Games 2020. All Rights Reserved.

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
  bool SearchSubfolders,
  EFileSDKFileType FileType
) {
  TArray<FString> FileNames;
  IFileManager & FileManager = IFileManager::Get();

  if (SearchSubfolders) {
    if (FileType == EFileSDKFileType::File) {
      FileManager.FindFilesRecursive(
        FileNames,
        *DirectoryToSearch,
        *(TEXT("*") + FilterFilesWithExtension),
        true,
        false
      );
    } else {
      FileManager.FindFilesRecursive(
        FileNames,
        *DirectoryToSearch,
        TEXT("*"),
        false,
        true
      );
    }
  } else {
    TArray<FString> relativeFileNames;

    if (FileType == EFileSDKFileType::File) {
      FileManager.FindFiles(
        relativeFileNames,
        *DirectoryToSearch,
        *FilterFilesWithExtension
      );
    } else {
      FileManager.FindFiles(
        relativeFileNames,
        *(DirectoryToSearch + FGenericPlatformMisc::GetDefaultPathSeparator() + TEXT("*")),
        false,
        true
      );
    }

    for (FString fileName : relativeFileNames) {
      FileNames.Add(
        DirectoryToSearch +
        FGenericPlatformMisc::GetDefaultPathSeparator() +
        fileName
      );
    }
  }

  return FileNames;
}

TArray<FFileSDKFileInfo> UFileSDKBPLibrary::GetDirectoryContentsWithFileInfo(
  FString Directory,
  bool SearchSubfolders
) {
  TArray<FFileSDKFileInfo> contents;
  IFileManager & FileManager = IFileManager::Get();

  TArray<FString> filePaths;
  if (SearchSubfolders) {
    FileManager.FindFilesRecursive(
      filePaths,
      *Directory,
      TEXT("*"),
      true,
      true
    );
  } else {
    TArray<FString> relativeFileNames;

    FileManager.FindFiles(
      relativeFileNames,
      *(Directory + FGenericPlatformMisc::GetDefaultPathSeparator() + TEXT("*")),
      true,
      true
    );

    for (FString fileName : relativeFileNames) {
      filePaths.Add(
        Directory +
        FGenericPlatformMisc::GetDefaultPathSeparator() +
        fileName
      );
    }
  }

  for (FString path : filePaths) {
    FFileSDKFileInfo info;
    info.AbsolutePath = path;
    info.Filename = FPaths::GetCleanFilename(path);
    UFileSDKBPLibrary::GetFileOrDirectoryInfo(path, info);
    contents.Add(info);
  }

  return contents;
}

void UFileSDKBPLibrary::GetFileOrDirectoryInfo(FString Path, FFileSDKFileInfo & Info) {
  IPlatformFile & PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

  FFileStatData data = PlatformFile.GetStatData(*Path);

  Info.AbsolutePath = Path;
  Info.Filename = FPaths::GetCleanFilename(Path);
  Info.CreationTime = data.CreationTime;
  Info.AccessTime = data.AccessTime;
  Info.ModificationTime = data.ModificationTime;
  Info.FileSize = data.FileSize;
  Info.bIsDirectory = data.bIsDirectory;
  Info.bIsReadOnly = data.bIsReadOnly;
  Info.bIsValid = data.bIsValid;
}

FString UFileSDKBPLibrary::GetCurrentUsername() {
#if PLATFORM_WINDOWS
  return FGenericPlatformMisc::GetEnvironmentVariable(ANSI_TO_TCHAR("USERNAME"));
#elif PLATFORM_LINUX
  return FGenericPlatformMisc::GetEnvironmentVariable(ANSI_TO_TCHAR("USER"));
#elif PLATFORM_MAC
  return FGenericPlatformMisc::GetEnvironmentVariable(ANSI_TO_TCHAR("USER")));
#else
  return "PLATFORM_NOT_SUPPORTED";
#endif
}

FString UFileSDKBPLibrary::GetEnvironmentVariable(FString VariableName) {
  return FGenericPlatformMisc::GetEnvironmentVariable(*VariableName);
}
