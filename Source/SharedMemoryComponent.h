// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#define WIN32_LEAN_AND_MEAN
#if defined(TEXT)
#undef TEXT
#endif
#include <windows.h>

#if defined(TEXT)
#undef TEXT
#endif

// If we don't have a platform-specific define for the TEXT macro, define it now.
#if !defined(TEXT) && !UE_BUILD_DOCS
	#if PLATFORM_TCHAR_IS_UTF8CHAR
		#define TEXT_PASTE(x) UTF8TEXT(x)
	#else
		#define TEXT_PASTE(x) WIDETEXT(x)
	#endif
	#define TEXT(x) TEXT_PASTE(x)
#endif

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SharedMemoryComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(SharedMemoryComponentLog, Log, All);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CAMREC1_API USharedMemoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USharedMemoryComponent();
	virtual ~USharedMemoryComponent() override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category= "Shared Memory Component")
	void WriteFString(const FString& str);
	
	UFUNCTION(BlueprintCallable, Category= "Shared Memory Component")
	FString getSharedMemVal();
	
private:
	HANDLE hMapFile;
	LPCSTR pBuf;
	const int SHARED_MEM_SIZE = 1000;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Shared Memory Component", meta = (DisplayName = "sharedMemName"))
	FString sharedMemName = "webCamRec1";
};
