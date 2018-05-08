// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "GAS.h"
#include "GASProjectile.h"
#include "UnrealNetwork.h"
#include "GameFramework/ProjectileMovementComponent.h"

//////////////////////////////////////////////////////////////////////////
// AGASCharacter

AGASProjectile::AGASProjectile(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionBlockComp0"));
	CollisionComponent->InitSphereRadius(0.0f);
	CollisionComponent->bAbsoluteScale = true;
	CollisionComponent->SetCanEverAffectNavigation(false);
	//CollisionComponent->BodyInstance.SetCollisionProfileName("Projectile");
	RootComponent = CollisionComponent;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp0"));
	MeshComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetCanEverAffectNavigation(false);
	MeshComponent->CastShadow = false;
	MeshComponent->bGenerateOverlapEvents = false;

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp0"));
	MovementComponent->UpdatedComponent = RootComponent;	

	bReplicates = true;
}

void AGASProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (Role == ROLE_Authority)
	{
		if (Instigator)
		{
			SetActorTransform( Instigator->GetActorTransform(), false);
		}

		SpawnOrigin = GetActorLocation();
		SpawnRotation = GetActorRotation().GetNormalized();
	}

	const FVector Direction = SpawnRotation.Vector();
	const float Speed = GetLaunchSpeed();

	Launch(Direction, Speed);
}

void AGASProjectile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AGASProjectile, SpawnOrigin, COND_InitialOnly);
	DOREPLIFETIME_CONDITION(AGASProjectile, SpawnRotation, COND_InitialOnly);
	DOREPLIFETIME_CONDITION(AGASProjectile, LaunchSpeed, COND_InitialOnly);
}

float AGASProjectile::GetLaunchSpeed() const
{
	return LaunchSpeed;
}

void AGASProjectile::Launch(const FVector& Dir, float Speed)
{
	FVector const DirNorm = Dir.GetSafeNormal();	

	MovementComponent->Velocity = Speed * DirNorm;
}