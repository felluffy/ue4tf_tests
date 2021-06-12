// Copyright Epic Games, Inc. All Rights Reserved.

#include "VoiceProjGameMode.h"
#include "VoiceProjCharacter.h"
#include "UObject/ConstructorHelpers.h"

AVoiceProjGameMode::AVoiceProjGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
