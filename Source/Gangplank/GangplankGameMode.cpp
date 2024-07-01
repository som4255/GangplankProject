// Copyright Epic Games, Inc. All Rights Reserved.

#include "GangplankGameMode.h"
#include "GangplankCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGangplankGameMode::AGangplankGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
