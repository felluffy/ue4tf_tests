// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RunFlaskGameMode.generated.h"

/**
 * 
 */
UCLASS()
class VOICEPROJ_API ARunFlaskGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	virtual void StartPlay() override;
	ARunFlaskGameMode();
	virtual void RunFlaskOnStartUp();
	//virtual void StopThread();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Startup)
	TArray<FString> ConsoleCommands;
/*
	FRunnableThread Thread;
	FThreadSafeCounter StopTaskCounter;
	void EnsureCompletion();
	static void Shutdown();
	static bool IsThreadFinished();*/
};
