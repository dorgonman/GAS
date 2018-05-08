// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "GAS.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemComponent.h"




bool FGASActorFilter::PassesFilter(UAbilitySystemComponent* Source, UAbilitySystemComponent* Target)
{
	if (AllowSelf != EGASActorFilterMatchType::Skip && ( (Source == Target) != (AllowSelf == EGASActorFilterMatchType::MustPass)))
	{
		return false;
	}

	return true;
}

void FGASEffectApplicationContainer::GenerateEffectSpecs(AActor* Source, float LevelOverride, int32 StackOverride)
{
	if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Source))
	{
		SourceAbilitySystemComponent = ASC;

		for (FGASEffectApplicationItem& Item : Items)
		{
			for (FGASEffectItem& EffectItem : Item.Effects)
			{
				if (EffectItem.GeneratedSpec.IsValid())
				{
					UE_LOG(LogTemp, Warning, TEXT("GenerateEffectSpecs called on ApplicationContainer that already has generated specs!"));
					continue;
				}
				if (*EffectItem.GameplayEffect == nullptr)
				{
					UE_LOG(LogTemp, Warning, TEXT("GenerateEffectSpecs called on ApplicationContainer with invalid GAmeplayEffect!"));
					continue;
				}

				float Level = LevelOverride > 0.f ? LevelOverride : EffectItem.Level;
				int32 StackCount = StackOverride > 0 ? StackOverride : EffectItem.StackCount;

				EffectItem.GeneratedSpec = FGameplayEffectSpecHandle( new FGameplayEffectSpec(EffectItem.GameplayEffect.GetDefaultObject(), ASC->MakeEffectContext(), Level ) );
				EffectItem.GeneratedSpec.Data->StackCount = StackCount;
			}
		}

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("GenerateEffectSpecs called on Source %s with no ASC"), *GetNameSafe(Source));
	}
}

void FGASEffectApplicationContainer::AddHitResult(FHitResult Result)
{
	for (FGASEffectApplicationItem& Item : Items)
	{
		for (FGASEffectItem& EffectItem : Item.Effects)
		{
			if (EffectItem.GeneratedSpec.Data.IsValid())
			{
				EffectItem.GeneratedSpec.Data->GetContext().AddHitResult(Result, true);
			}
		}
	}
}

TArray<FActiveGameplayEffectHandle> FGASEffectApplicationContainer::ApplyEffectApplicationContainerToTarget(AActor* Target)
{
	TArray<FActiveGameplayEffectHandle> Handles;
	if (SourceAbilitySystemComponent == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("ApplyEffectApplicationContainerToTarget called on container with no SourceAbilitySystemComponent. call GeneratEffectSpecs first"));
		return Handles;
	}

	if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Target))
	{
		for (FGASEffectApplicationItem& Item : Items)
		{
			if (Item.Filter.PassesFilter(SourceAbilitySystemComponent , ASC) == false)
			{
				continue;
			}

			for (FGASEffectItem& EffectItem : Item.Effects)
			{
				if (EffectItem.GeneratedSpec.IsValid() == false)
				{
					UE_LOG(LogTemp, Warning, TEXT("ApplyEffectApplicationContainerToTarget called on ApplicationContainer that has NO generated specs!"));
					continue;
				}
				
				FActiveGameplayEffectHandle NewHandle = ASC->ApplyGameplayEffectSpecToSelf(*EffectItem.GeneratedSpec.Data.Get(), SourceAbilitySystemComponent->ScopedPredictionKey);
				if (NewHandle.IsValid())
				{
					Handles.Add(NewHandle);
				}
			}
		}
	}

	return Handles;
}