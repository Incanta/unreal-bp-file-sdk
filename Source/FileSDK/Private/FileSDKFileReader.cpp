// Copyright Incanta Games 2020. All Rights Reserved.

#include "FileSDKFileReader.h"
#include "FileSDKBPLibrary.h"

UFileSDKFileReader::UFileSDKFileReader(
  const FObjectInitializer& ObjectInitializer
) : Super(ObjectInitializer) {
  //
}

void UFileSDKFileReader::OpenFile(
  FString fileName,
  bool OpenInBinaryMode
) {
  this->FileName = fileName;
  this->BinaryMode = OpenInBinaryMode;
  this->fileReader = new std::ifstream();
  if (OpenInBinaryMode) {
    this->fileReader->open(TCHAR_TO_UTF8(*fileName), std::ios_base::in | std::ios_base::binary);
  } else {
    this->fileReader->open(TCHAR_TO_UTF8(*fileName), std::ios_base::in);
  }
}

bool UFileSDKFileReader::IsGood() {
  return this->fileReader && this->fileReader->good();
}

bool UFileSDKFileReader::SeekFilePosition(
  EFileSDKFileAnchor Anchor,
  int Offset
) {
  if (this->fileReader && this->fileReader->good()) {
    this->fileReader->seekg(
      Offset,
      UFileSDKBPLibrary::FileAnchorToSeekDir(Anchor)
    );
    return true;
  } else {
    return false;
  }
}

int UFileSDKFileReader::ReadBytes(int Num, TArray<uint8> & Content) {
  if (this->fileReader && this->fileReader->good()) {
    char * buffer = new char[Num];
    Content.Reserve(Num);
    memset(buffer, 0, Num);
    this->fileReader->read(buffer, Num);
    int numRead = this->fileReader->gcount();
    Content.Append((uint8*) buffer, numRead);
    return numRead;
  } else {
    return 0;
  }
}

int UFileSDKFileReader::ReadBytesToEnd(TArray<uint8> & Content) {
  if (this->fileReader && this->fileReader->good()) {
    int currentPosition = this->fileReader->tellg();
    this->fileReader->seekg(0, std::ios_base::end);
    int endPosition = this->fileReader->tellg();
    this->fileReader->seekg(currentPosition, std::ios_base::beg);

    return this->ReadBytes(endPosition - currentPosition + 1, Content);
  } else {
    return 0;
  }
}

int UFileSDKFileReader::ReadString(int Num, FString & Content) {
  if (this->fileReader && this->fileReader->good()) {
    char * buffer = new char[Num + 1]; // one more for string termination
    Content.Reset(Num);
    memset(buffer, 0, Num + 1);
    this->fileReader->read(buffer, Num);
    Content.Append(buffer);
    return this->fileReader->gcount();
  } else {
    return 0;
  }
}

int UFileSDKFileReader::ReadStringToEnd(FString & Content) {
  if (this->fileReader && this->fileReader->good()) {
    int currentPosition = this->fileReader->tellg();
    this->fileReader->seekg(0, std::ios_base::end);
    int endPosition = this->fileReader->tellg();
    this->fileReader->seekg(currentPosition, std::ios_base::beg);

    return this->ReadString(endPosition - currentPosition + 1, Content);
  } else {
    return 0;
  }
}

void UFileSDKFileReader::Close() {
  if (this->fileReader) {
    this->fileReader->close();
  }
}
