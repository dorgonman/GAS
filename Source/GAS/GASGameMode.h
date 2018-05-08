// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameMode.h"
#include "GASGameMode.generated.h"

UCLASS(minimalapi)
class AGASGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AGASGameMode();

	virtual void RestartPlayer(class AController* NewPlayer) override;
	
	// -------------------------------------------------------------------------------------------
	//			GameplayAbility System Sample
	// -------------------------------------------------------------------------------------------

	void KilledBy(AController* Killer, AActor* DamageCauser, AController* KilledContoller, AActor* KilledActor, const struct FGameplayEffectSpec& KillingEffectSpec);
};



