// Fill out your copyright notice in the Description page of Project Settings.

#include "SharedMemoryComponent.h"
#include <string>

DEFINE_LOG_CATEGORY(SharedMemoryComponentLog);

USharedMemoryComponent::USharedMemoryComponent() {
	PrimaryComponentTick.bCanEverTick = true;
	
	hMapFile = CreateFileMapping(
		INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0,
		SHARED_MEM_SIZE,
		ToCStr(sharedMemName)
	);

	if (!hMapFile) {
		UE_LOG(SharedMemoryComponentLog, Error, TEXT("Failed to create shared memory"));
		return;
	}

	pBuf = (LPCSTR) MapViewOfFile(
		hMapFile,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		SHARED_MEM_SIZE
	);

	if (!pBuf) {
		UE_LOG(SharedMemoryComponentLog, Error, TEXT("Failed to create shared memory: failed to do the map view of file"));
		CloseHandle(hMapFile);
		return;
	}

	std::memset((void*)pBuf, 0, SHARED_MEM_SIZE);
	
	// UnmapViewOfFile(pBuf);
	// CloseHandle(hMapFile);
}

USharedMemoryComponent::~USharedMemoryComponent() {
	if (pBuf) {
		UnmapViewOfFile(pBuf);
	}
	if (hMapFile) {
		CloseHandle(hMapFile);
	}
}

void USharedMemoryComponent::BeginPlay() {
	Super::BeginPlay();
}

void USharedMemoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

FString USharedMemoryComponent::getSharedMemVal() {
	if (!pBuf) {
		return FString();
	}
	return pBuf;
}

void USharedMemoryComponent::WriteFString(const FString& str) {
	if (str.Len() > SHARED_MEM_SIZE) {
		UE_LOG(SharedMemoryComponentLog, Warning, TEXT("Couldn't write string to shared memory: string too long"));
	}

	std::wstring wstr1(ToCStr(str));
	std::string str1(wstr1.begin(), wstr1.end());
	
	std::memset((void*)pBuf, 0, SHARED_MEM_SIZE);
	std::memcpy((void*)pBuf, str1.c_str(), str1.length() * sizeof(char));
}