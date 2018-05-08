// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "GAS.h"
#include "GASGameMode.h"
#include "GASCharacter.h"
#include "GameplayEffect.h"

AGASGameMode::AGASGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/BP_Hero"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}


void AGASGameMode::RestartPlayer(class AController* NewPlayer)
{
	Super::RestartPlayer(NewPlayer);
}

void AGASGameMode::KilledBy(AController* Killer, AActor* DamageCauser, AController* KilledController, AActor* KilledActor, const struct FGameplayEffectSpec& KillingEffectSpec)
{
	if (KilledController)
	{
		KilledController->ChangeState(NAME_Spectating);
		RestartPlayer(KilledController);
	}
}