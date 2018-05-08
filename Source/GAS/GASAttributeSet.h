// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "AttributeSet.h"
#include "GASAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class GAS_API UGASAttributeSet : public UAttributeSet
{
	GENERATED_UCLASS_BODY()
	
	UPROPERTY()
	float	MaxMovementSpeed;

	UPROPERTY()
	float	JumpPower;
	
	UPROPERTY()
	float	Health;

	UPROPERTY()
	float	MaxHealth;

	UPROPERTY()
	float	AttackPower;

	void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData &Data) override;

	// -------------------------------------------------

	static FGameplayAttribute HealthAttribute();
	static FGameplayAttribute MaxHealthAttribute();
};
