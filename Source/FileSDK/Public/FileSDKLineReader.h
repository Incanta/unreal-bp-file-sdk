// Copyright Incanta Games 2021. All Rights Reserved.

#pragma once

#include "FileSDKLineReader.generated.h"

UCLASS(Blueprintable)
class FILESDK_API UFileSDKLineReader : public UObject {
  GENERATED_BODY()

public:
  UFUNCTION(BlueprintNativeEvent, Category = "FileSDK")
  bool FilterLine(const FString & line);

  virtual bool FilterLine_Implementation(const FString & line) {
    return true;
  }
};
