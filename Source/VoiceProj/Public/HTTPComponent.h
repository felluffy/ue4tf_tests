// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/IHttpRequest.h"
#include "HTTPComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VOICEPROJ_API UHTTPComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UHTTPComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	class FHttpModule* Http;
	UPROPERTY(EditDefaultsOnly, Category = "Request Properties")
		FString URL;
	UPROPERTY(EditDefaultsOnly, Category = "Request Properties")
		FString ContentType;
	UPROPERTY(EditDefaultsOnly, Category = "Request Properties")
		FString VerbType;
	UPROPERTY(EditDefaultsOnly, Category = "Request Properties")
		FString FileLocation;
	UPROPERTY(EditDefaultsOnly, Category = "Request Properties")
		FString	OAuthToken;
	UPROPERTY(EditDefaultsOnly, Category = "Request Properties")
		TMap<FString, FString> HeaderMap;

	float CurrentTime;
public:
	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void OnResponseReceivedMNIST(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	//void SendAudioRequest(const FString& Url, const FString& RequestContent);
	UFUNCTION(BlueprintCallable)
		void SendAudioRequest(FString Path, bool UseHeaderMap = false);
	UFUNCTION(BlueprintCallable)
		void SendTextRequest(FString Sentence, bool UseHeaderMap = false);
	UFUNCTION(BlueprintCallable)
		void SendImageRequest(class UTextureRenderTarget2D* InTexture, bool UseHeaderMap = false);

};
