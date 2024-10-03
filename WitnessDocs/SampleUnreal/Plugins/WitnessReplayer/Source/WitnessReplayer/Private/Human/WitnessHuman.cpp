// Fill out your copyright notice in the Description page of Project Settings.


#include "Human/WitnessHuman.h"

#include "GeometryTypes.h"
#include "DSP/Chorus.h"


// Sets default values
AWitnessHuman::AWitnessHuman()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AWitnessHuman::BeginPlay()
{
	Super::BeginPlay();
	Load();
}

void AWitnessHuman::Load()
{
	USkeletalMeshComponent* Mesh = FindComponentByClass<USkeletalMeshComponent>();
	AnimationInstance = Cast<UHumanAnimation>(Mesh->GetAnimInstance());
	EffectPoint = FindComponentByClass<UChildActorComponent>();

	AnimationInstance->State = EHumanStates::Idle;
}

// Called every frame
void AWitnessHuman::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AnimationInstance->WalkingAt = AnimationAt;

	SetEffector();
		CheckState();
}

void AWitnessHuman::SetEffector()
{
	EffectorPosition = EffectPoint->GetComponentLocation();
}

void AWitnessHuman::CheckState()
{
	
	bool moving = AnimationAt > 0;

	if (moving)
	{
		AnimationInstance->State = EHumanStates::Moving;
		AnimationInstance->LeftAlpha = 1;
		AnimationInstance->RightAlpha = 1;

		CaculateHandles();
	}
	else
	{
		AnimationInstance->State = EHumanStates::Idle;
		AnimationInstance->LeftAlpha = 0;
		AnimationInstance->RightAlpha = 0;
	}

	OldPosition = GetActorLocation();
}

void AWitnessHuman::CaculateHandles()
{
	// Get nessacry values
	FVector Orgin;
	FVector BoxExtent;
	AttachedActor->GetActorBounds(false, Orgin, BoxExtent, false);

	Orgin.Z = Orgin.Z - BoxExtent.Z;

	// Caculate Right Handle
	RightHandle = Orgin + (GetActorRightVector() * BoxExtent.Y) - (GetActorForwardVector() * BoxExtent.X);
	LeftHandle = Orgin - (GetActorRightVector() * BoxExtent.Y) - (GetActorForwardVector() * BoxExtent.X);

	AnimationInstance->RightHandle = RightHandle;
	AnimationInstance->LeftHandle = LeftHandle;
}
