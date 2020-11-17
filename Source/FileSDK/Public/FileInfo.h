// Copyright Incanta Games 2020. All Rights Reserved.

#pragma once

#include "FileInfo.generated.h"

USTRUCT(BlueprintType)
struct FFileInfo {
  GENERATED_USTRUCT_BODY();

  /** The time that the file or directory was originally created, or FDateTime::MinValue if the creation time is unknown */
  UPROPERTY(BlueprintReadOnly)
  FDateTime CreationTime;

  /** The time that the file or directory was last accessed, or FDateTime::MinValue if the access time is unknown */
  UPROPERTY(BlueprintReadOnly)
  FDateTime AccessTime;

  /** The time the the file or directory was last modified, or FDateTime::MinValue if the modification time is unknown */
  UPROPERTY(BlueprintReadOnly)
  FDateTime ModificationTime;

  /** Size of the file (in bytes), or -1 if the file size is unknown */
  UPROPERTY(BlueprintReadOnly)
  int64 FileSize;

  /** True if this data is for a directory, false if it's for a file */
  UPROPERTY(BlueprintReadOnly)
  bool bIsDirectory;

  /** True if this file is read-only */
  UPROPERTY(BlueprintReadOnly)
  bool bIsReadOnly;

  /** True if file or directory was found, false otherwise. Note that this value being true does not ensure that the other members are filled in with meaningful data, as not all file systems have access to all of this data */
  UPROPERTY(BlueprintReadOnly)
  bool bIsValid;
};
