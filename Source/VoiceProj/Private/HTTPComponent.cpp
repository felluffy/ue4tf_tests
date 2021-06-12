// Fill out your copyright notice in the Description page of Project Settings.


#include "HTTPComponent.h"
#include "GameFramework/Actor.h"
#include "Runtime/JsonUtilities/Public/JsonUtilities.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "Runtime/Online/HTTP/Public/HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <Engine/TextureRenderTarget2D.h>
using std::vector;
using std::ofstream;

// Sets default values for this component's properties
UHTTPComponent::UHTTPComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	Http = &FHttpModule::Get();
	URL = TEXT("http://127.0.0.1:6502/"); //Generate url from a file or something
	FileLocation = TEXT("");
	OAuthToken = TEXT("");
	VerbType = "POST";
}

// ...


// Called when the game starts
void UHTTPComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}


// Called every frame
void UHTTPComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHTTPComponent::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		auto res = abs(CurrentTime - GetWorld()->GetTimeSeconds());
		GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Green, FString(JsonObject->GetStringField("svalue")) + FString::SanitizeFloat(res));

		//UE_LOG(LogTemp, Warning, TEXT("TEXT VALS: %s"), JsonObject->GetStringField("svalue"));
	}

}

void UHTTPComponent::OnResponseReceivedMNIST(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Green, FString("Returned"));
	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		auto res = abs(CurrentTime - GetWorld()->GetTimeSeconds());
		if(FCString::Atof(*(JsonObject->GetStringField("accuracy"))) > .6)
			GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Green, FString("Predicted value is: ") + FString(JsonObject->GetStringField("value")) + FString(" with accuracy: ") + FString(JsonObject->GetStringField("accuracy")) + FString(" in time: ") + FString::SanitizeFloat(res));
		else
			GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Red, FString("Accuracy was below .6"));

		//UE_LOG(LogTemp, Warning, TEXT("TEXT VALS: %s"), JsonObject->GetStringField("svalue"));
	}
}

void UHTTPComponent::SendAudioRequest(FString Path, bool UseHeaderMap /*= false*/)
{

}

void UHTTPComponent::SendTextRequest(FString Sentence, bool UseHeaderMap /*= false*/)
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
	Request->SetURL(URL + Sentence);
	Request->SetVerb(VerbType);
	if (!UseHeaderMap)
	{
		Request->OnProcessRequestComplete().BindUObject(this, &UHTTPComponent::OnResponseReceived);
		Request->SetHeader("Content-Type", TEXT("application/json"));
		Request->SetVerb("GET");
		/*Request->SetHeader("Content-Type", TEXT("audio/wav"));
		Request->AppendToHeader("Content-Type", TEXT("codecs=audio/pcm"));
		Request->AppendToHeader("Content-Type", TEXT("samplerate=16000"));
		Request->SetHeader(TEXT("Ocp-Apim-Subscription-Key"), OAuthToken);
		Request->SetHeader("Accept", "application/json; text/xml");
		Request->SetHeader("Expect", "100-continue");
		Request->SetHeader("Host", "centralindia.speech.microsoft.com");*/
	}
	else
	{
		for (auto &it : HeaderMap)
		{
			Request->SetHeader(it.Key, it.Value);
		}
	}
	CurrentTime = GetWorld()->GetTimeSeconds();
	Request->ProcessRequest();
}

void UHTTPComponent::SendImageRequest(UTextureRenderTarget2D* InTexture, bool UseHeaderMap /*= false*/)
{
	TArray<FColor> SurfData;
	UTexture2D* Texture = InTexture->ConstructTexture2D(this, FString("Image"), EObjectFlags::RF_NoFlags);
	Texture->SRGB = false;
	Texture->CompressionSettings = TextureCompressionSettings::TC_EditorIcon;
	Texture->MipGenSettings = TextureMipGenSettings::TMGS_NoMipmaps;
	Texture->UpdateResource();
	FColor* ImageData = static_cast<FColor*>(Texture->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_ONLY));
	int sizeY = Texture->PlatformData->Mips[0].SizeY;
	int sizeX = Texture->PlatformData->Mips[0].SizeX;
	UE_LOG(LogTemp, Warning, TEXT("Size: x:y %d : %d : -> size of %d"), sizeY, sizeX, sizeof(ImageData) / sizeof(ImageData[0]));
	vector<vector<uint8>> GSImageData(sizeX, vector<uint8>(sizeY, 0));
	TArray<uint8> vals;//(sizeX * sizeY, 0);
	//int i = -1;
	//ofstream out("C://Projects//Unreal Projects//VoiceProj//Source//Values1.txt", std::ofstream::out);
	for (int y = 0; y < sizeY; ++y)
	{
		for (int x = 0; x < sizeX; ++x)
		{
			FColor Pixel = ImageData[(y * sizeX) + x];
			//GSImageData[y][x] = .3*Pixel.R + .59*Pixel.G + .11*Pixel.B;
			vals.Add((.3*Pixel.R + .59*Pixel.G + .11*Pixel.B));
			//out << unsigned(GSImageData[y][x]) << (x + 1 < sizeX ? ", " : "");
			//std::string s = "[pos: " + std::to_string(((y * sizeX) + x)) + " " + std::to_string(Pixel.R) + " " + std::to_string(Pixel.G) + " " + std::to_string(Pixel.B) + " " + std::to_string(Pixel.A) + "]" + (x + 1 < sizeX ? ", " : "");
			//out << s;// << "[pos: " << int << " " << float(Pixel.R) << " " << float(Pixel.G)  << " " << float(Pixel.B) << " " << Pixel.A << "]" << (x + 1 < sizeX ? ", " : "");
		}
		//out << std::endl;
	}
	
	/*for (int x = 0; x < sizeX; ++x)
	{
		for (int y = 0; y < sizeY; ++y)
		{
			out << unsigned(GSImageData[x][y]) << (y + 1 < sizeY ? ", " : "");
		}
		out << std::endl;
	}*/
	//auto CachedRenderTexture = UTexture2D::CreateTransient(InTexture->SizeX, InTexture->SizeY, PF_B8G8R8A8);

	//out.close();
	Texture->PlatformData->Mips[0].BulkData.Unlock();
	
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
	Request->SetURL(URL + "image");
	Request->SetVerb("POST");
	if (!UseHeaderMap)
	{
		Request->OnProcessRequestComplete().BindUObject(this, &UHTTPComponent::OnResponseReceivedMNIST);
		Request->SetHeader("Content-Type", TEXT("application/json"));
		Request->SetVerb("POST");
	}
	else
	{
		for (auto &it : HeaderMap)
		{
			Request->SetHeader(it.Key, it.Value);
		}
	}
	Request->SetContent(vals);
	CurrentTime = GetWorld()->GetTimeSeconds();
	if (!Request->ProcessRequest())
	{
		UE_LOG(LogTemp, Warning, TEXT("FAILED TO SEND REQUEST"));
	}
// 	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
// 	TArray< TSharedPtr<FJsonValue> > ObjArray;
// 	ObjArray.Add()
// 	JsonObject->SetArrayField("Image Data", ObjArray);
}

