// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameFramework/CharacterMovementComponent.h"
#include "GASCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class GAS_API UGASCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
	
	virtual float GetMaxSpeed() const override;
	
	
};
