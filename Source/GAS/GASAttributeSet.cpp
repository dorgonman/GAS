// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "GAS.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystemComponent.h"
#include "GASAttributeSet.h"
#include "GASCharacter.h"

UGASAttributeSet::UGASAttributeSet(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	MaxMovementSpeed = 450.f;
	JumpPower = 1.f;

	Health = 100.f;
	MaxHealth = 100.f;
}

void UGASAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == HealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, MaxHealth);
	}
}

void UGASAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{

}

void UGASAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData &Data)
{
	UAbilitySystemComponent* Source = Data.EffectSpec.GetContext().GetOriginalInstigatorAbilitySystemComponent();

	if (HealthAttribute() == Data.EvaluatedData.Attribute)
	{
		// Get the Target actor
		AActor* DamagedActor = nullptr;
		AController* DamagedController = nullptr;
		if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
		{
			DamagedActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
			DamagedController = Data.Target.AbilityActorInfo->PlayerController.Get();
		}

		// Get the Source actor
		AActor* AttackingActor = nullptr;
		AController* AttackingController = nullptr;
		AController* AttackingPlayerController = nullptr;
		if (Source && Source->AbilityActorInfo.IsValid() && Source->AbilityActorInfo->AvatarActor.IsValid())
		{
			AttackingActor = Source->AbilityActorInfo->AvatarActor.Get();
			AttackingController = Source->AbilityActorInfo->PlayerController.Get();
			AttackingPlayerController = Source->AbilityActorInfo->PlayerController.Get();
			if (AttackingController == nullptr && AttackingActor != nullptr)
			{
				if (APawn* Pawn = Cast<APawn>(AttackingActor))
				{
					AttackingController = Pawn->GetController();
				}
			}
		}

		// Clamp health
		Health = FMath::Clamp(Health, 0.0f, MaxHealth);
		if (Health <= 0)
		{

			// Handle death with GASCharacter. Note this is just one example of how this could be done.
			if (AGASCharacter* GASChar = Cast<AGASCharacter>(DamagedActor))
			{
				// Construct a gameplay cue event for this death
				FGameplayCueParameters Params(Data.EffectSpec.GetContext());
				Params.RawMagnitude = Data.EvaluatedData.Magnitude;;
				Params.NormalizedMagnitude = FMath::Abs(Data.EvaluatedData.Magnitude / MaxHealth);
				Params.AggregatedSourceTags = *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags();
				Params.AggregatedTargetTags = *Data.EffectSpec.CapturedTargetTags.GetAggregatedTags();

				GASChar->Die(DamagedController, DamagedActor,  Data.EffectSpec, Params.RawMagnitude, Params.Normal);
			}
		}
	}

}

// ----------------------------------------------------------------------------------

FGameplayAttribute UGASAttributeSet::HealthAttribute()
{
	static UProperty* Property = FindFieldChecked<UProperty>(UGASAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UGASAttributeSet, Health));
	return FGameplayAttribute(Property);
}

FGameplayAttribute UGASAttributeSet::MaxHealthAttribute()
{
	static UProperty* Property = FindFieldChecked<UProperty>(UGASAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UGASAttributeSet, MaxHealth));
	return FGameplayAttribute(Property);
}