// Copyright Incanta Games 2020. All Rights Reserved.

#include "FileSDKBPLibrary.h"
#include "FileSDK.h"
#include "FileSDKFileReader.h"

UFileSDKBPLibrary::UFileSDKBPLibrary(
  const FObjectInitializer& ObjectInitializer
) : Super(ObjectInitializer) {
  //
}

UFileSDKFileReader * UFileSDKBPLibrary::OpenFileReader(
  FString FileName,
  bool OpenInBinaryMode
) {
  UFileSDKFileReader * fileReader = NewObject<UFileSDKFileReader>();
  fileReader->OpenFile(FileName, OpenInBinaryMode);
  return fileReader;
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

bool UFileSDKBPLibrary::WriteStringToFile(
  FString FileName,
  FString Content,
  bool Append,
  EFileSDKEncodingOptions Encoding
) {
  FFileHelper::EEncodingOptions internalEncoding;
  switch (Encoding) {
    case EFileSDKEncodingOptions::AutoDetect: {
      internalEncoding = FFileHelper::EEncodingOptions::AutoDetect;
      break;
    }
    case EFileSDKEncodingOptions::ForceAnsi: {
      internalEncoding = FFileHelper::EEncodingOptions::ForceAnsi;
      break;
    }
    case EFileSDKEncodingOptions::ForceUnicode: {
      internalEncoding = FFileHelper::EEncodingOptions::ForceUnicode;
      break;
    }
    case EFileSDKEncodingOptions::ForceUTF8: {
      internalEncoding = FFileHelper::EEncodingOptions::ForceUTF8;
      break;
    }
    case EFileSDKEncodingOptions::ForceUTF8WithoutBOM: {
      internalEncoding = FFileHelper::EEncodingOptions::ForceUTF8WithoutBOM;
      break;
    }
    default: {
      internalEncoding = FFileHelper::EEncodingOptions::AutoDetect;
      break;
    }
  }

  if (Append) {
    return FFileHelper::SaveStringToFile(
      Content,
      *FileName,
      internalEncoding,
      &IFileManager::Get(),
      std::ios_base::app
    );
  } else {
    return FFileHelper::SaveStringToFile(Content, *FileName, internalEncoding);
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
  auto buf = new char[512];
#if PLATFORM_WINDOWS
  FWindowsPlatformMisc::GetEnvironmentVariable(ANSI_TO_TCHAR("USERNAME"), ANSI_TO_TCHAR(buf), 512);
  auto str = FString(buf);
  delete[] buf;
  return str;
#elif PLATFORM_LINUX
  FUnixPlatformMisc::GetEnvironmentVariable(ANSI_TO_TCHAR("USER"), ANSI_TO_TCHAR(buf), 512);
  auto str = FString(buf);
  delete[] buf;
  return str;
#elif PLATFORM_MAC
  FApplePlatformMisc::GetEnvironmentVariable(ANSI_TO_TCHAR("USER"), ANSI_TO_TCHAR(buf), 512);
  auto str = FString(buf);
  delete[] buf;
  return str;
#else
  return "PLATFORM_NOT_SUPPORTED";
#endif
}

FString UFileSDKBPLibrary::GetCurrentUserHomeDirectory() {
  auto buf = new char[512];
#if PLATFORM_WINDOWS
  auto buf2 = new char[512];
  FWindowsPlatformMisc::GetEnvironmentVariable(ANSI_TO_TCHAR("HOMEDRIVE"), ANSI_TO_TCHAR(buf), 512);
  FWindowsPlatformMisc::GetEnvironmentVariable(ANSI_TO_TCHAR("HOMEPATH"), ANSI_TO_TCHAR(buf2), 512);
  auto str = FString(buf) + FString(buf2);
  delete[] buf;
  delete[] buf2;
  return str;
#elif PLATFORM_LINUX
  FUnixPlatformMisc::GetEnvironmentVariable(ANSI_TO_TCHAR("HOME"), ANSI_TO_TCHAR(buf), 512);
  auto str = FString(buf);
  delete[] buf;
  return str;
#elif PLATFORM_MAC
  FApplePlatformMisc::GetEnvironmentVariable(ANSI_TO_TCHAR("HOME"), ANSI_TO_TCHAR(buf), 512);
  auto str = FString(buf);
  delete[] buf;
  return str;
#else
  return "PLATFORM_NOT_SUPPORTED";
#endif
}

FString UFileSDKBPLibrary::GetEnvironmentVariable(FString VariableName) {
  auto buf = new char[512];
#if PLATFORM_WINDOWS
  FWindowsPlatformMisc::GetEnvironmentVariable(*VariableName, ANSI_TO_TCHAR(buf), 512);
  auto str = FString(buf);
  delete[] buf;
  return str;
#elif PLATFORM_LINUX
  FUnixPlatformMisc::GetEnvironmentVariable(*VariableName, ANSI_TO_TCHAR(buf), 512);
  auto str = FString(buf);
  delete[] buf;
  return str;
#elif PLATFORM_MAC
  FApplePlatformMisc::GetEnvironmentVariable(*VariableName, ANSI_TO_TCHAR(buf), 512);
  auto str = FString(buf);
  delete[] buf;
  return str;
#else
  return "PLATFORM_NOT_SUPPORTED";
#endif
}

std::ios_base::seekdir UFileSDKBPLibrary::FileAnchorToSeekDir(
  EFileSDKFileAnchor Anchor
) {
  switch (Anchor) {
    case EFileSDKFileAnchor::Beginning: {
      return std::ios_base::beg;
    }
    case EFileSDKFileAnchor::Current: {
      return std::ios_base::cur;
    }
    case EFileSDKFileAnchor::End:
    default: {
      return std::ios_base::end;
    }
  }
}
