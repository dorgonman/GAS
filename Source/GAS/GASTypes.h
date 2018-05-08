// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "GameplayEffectTypes.h"
#include "GameplayEffect.h"
#include "GASTypes.generated.h"

UENUM()
enum class EGASActorFilterMatchType : uint8
{
	/** Skip this check completely. */
	Skip,	
	/** Actor must pass this check (true) */
	MustPass,	
	/** Actor must fail this check (not true) */
	MustFail,	
};

USTRUCT(BlueprintType)
struct FGASActorFilter
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere)
	EGASActorFilterMatchType AllowSelf;

	bool PassesFilter(UAbilitySystemComponent* Source, UAbilitySystemComponent* Target);
};

// ----------------------------------------

USTRUCT(BlueprintType)
struct FGASEffectItem
{
	GENERATED_USTRUCT_BODY()

	FGASEffectItem() : Level(1.f), StackCount(1)
	{
		
	}

	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> GameplayEffect;

	UPROPERTY(EditAnywhere)
	float Level;

	UPROPERTY(EditAnywhere)
	int32 StackCount;

	UPROPERTY()
	FGameplayEffectSpecHandle GeneratedSpec;
};

USTRUCT(BlueprintType)
struct FGASEffectApplicationItem
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	FGASActorFilter	Filter;

	UPROPERTY(EditAnywhere)
	TArray<FGASEffectItem>	Effects;
};

/** A generic utility struct that can hold a list of "Apply this GE (Spec, Level, StackCount) to actors matching this filter". */
USTRUCT(BlueprintType)
struct FGASEffectApplicationContainer
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	TArray<FGASEffectApplicationItem> Items;

	/** Cached source when specs are generated */
	UPROPERTY(BlueprintReadWrite)
	UAbilitySystemComponent* SourceAbilitySystemComponent;

	void GenerateEffectSpecs(AActor* Source, float LevelOverride=0.f, int32 StackOverride=0);

	void AddHitResult(FHitResult Result);

	TArray<FActiveGameplayEffectHandle> ApplyEffectApplicationContainerToTarget(AActor* Target);
};
