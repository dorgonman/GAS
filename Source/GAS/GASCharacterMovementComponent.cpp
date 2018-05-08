// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "GAS.h"
#include "GASCharacter.h"
#include "GASCharacterMovementComponent.h"

float UGASCharacterMovementComponent::GetMaxSpeed() const
{
	const AGASCharacter* GASChar = CastChecked<AGASCharacter>(PawnOwner, ECastCheckedType::NullAllowed);
	if (GASChar)
	{
		return GASChar->GetMaxSpeed();
	}

	return Super::GetMaxSpeed();
}