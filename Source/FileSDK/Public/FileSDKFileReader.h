// Copyright Incanta Games 2021. All Rights Reserved.

#pragma once

#include "UObject/Object.h"
#include "FileAnchor.h"

#include <fstream>

#include "FileSDKFileReader.generated.h"

class UFileSDKBPLibrary;

UCLASS(BlueprintType, Blueprintable)
class UFileSDKFileReader : public UObject {
  GENERATED_UCLASS_BODY()

  void OpenFile(FString fileName, bool OpenInBinaryMode);

  UFUNCTION(
    BlueprintCallable,
    meta = (
      DisplayName = "Seek File Position",
      Keywords = "FileSDK seek change file position pointer"
    ),
    Category = "FileSDK | File Reader"
  )
  bool SeekFilePosition(EFileSDKFileAnchor Anchor, int Offset);

  UFUNCTION(
    BlueprintCallable,
    meta = (
      DisplayName = "Read Bytes",
      Keywords = "FileSDK read bytes"
    ),
    Category = "FileSDK | File Reader"
  )
  int ReadBytes(int Num, TArray<uint8> & Content);

  UFUNCTION(
    BlueprintCallable,
    meta = (
      DisplayName = "Read Bytes To End",
      Keywords = "FileSDK read bytes to end"
    ),
    Category = "FileSDK | File Reader"
  )
  int ReadBytesToEnd(TArray<uint8> & Content);

  UFUNCTION(
    BlueprintCallable,
    meta = (
      DisplayName = "Read String",
      Keywords = "FileSDK read string chars characters"
    ),
    Category = "FileSDK | File Reader"
  )
  int ReadString(int Num, FString & Content);

  UFUNCTION(
    BlueprintCallable,
    meta = (
      DisplayName = "Read String to End",
      Keywords = "FileSDK read string chars characters to end"
    ),
    Category = "FileSDK | File Reader"
  )
  int ReadStringToEnd(FString & Content);

  UFUNCTION(
    BlueprintCallable,
    meta = (
      DisplayName = "File Is Good",
      Keywords = "FileSDK file reader is good open"
    ),
    Category = "FileSDK | File Reader"
  )
  bool IsGood();

  UFUNCTION(
    BlueprintCallable,
    meta = (
      DisplayName = "Close File Reader",
      Keywords = "FileSDK file reader close"
    ),
    Category = "FileSDK | File Reader"
  )
  void Close();

  UPROPERTY(BlueprintReadOnly, Category = "Details")
  FString FileName;

  UPROPERTY(BlueprintReadOnly, Category = "Details")
  bool BinaryMode;

private:
  std::ifstream * fileReader;

  friend class UFileSDKBPLibrary;
};
