// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "GAS.h"
#include "GASBlueprintLibrary.h"

UGASBlueprintLibrary::UGASBlueprintLibrary(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
}

void UGASBlueprintLibrary::GenerateEffectSpecs(UPARAM(ref) FGASEffectApplicationContainer& Container, AActor* Source, float LevelOverride, int32 StackOverride)
{
	Container.GenerateEffectSpecs(Source, LevelOverride, StackOverride);
}

void UGASBlueprintLibrary::AddHitResult(UPARAM(ref) FGASEffectApplicationContainer& Container, FHitResult HitResult)
{
	Container.AddHitResult(HitResult);
}

TArray<FActiveGameplayEffectHandle> UGASBlueprintLibrary::ApplyEffectApplicationContainerToTarget(UPARAM(ref) FGASEffectApplicationContainer& Container, AActor* Target)
{
	return Container.ApplyEffectApplicationContainerToTarget(Target);
}