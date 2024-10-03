// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/WitnessTextBillboard.h"

AWitnessTextBillboard::AWitnessTextBillboard()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AWitnessTextBillboard::BeginPlay()
{
	Super::BeginPlay();
	
	// Getting the Camera Manager
	playerCameraManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;


	// Set up all spawn parameters
	FVector location = FVector(0,0,0);
	FRotator rotation = FRotator(0, 0, 0);

	FActorSpawnParameters spawnParameters;
	spawnParameters.Owner = this;

	//Create "Text Render Actor" and make it a child this object
	// if (actorToSpawn) {
	// 	textRender = GetWorld()->SpawnActor<ATextRenderActor>(actorToSpawn, location, rotation, spawnParameters);
	// 	textRender->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
	// }
}

void AWitnessTextBillboard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Get Camera location
	const FVector cameraLocation = playerCameraManager->GetCameraLocation();
	
	// Find rotation between Camera and Object
	const FRotator lookRotation = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), cameraLocation);

	// Rotate this object to the Camera
	this->SetActorRelativeRotation(lookRotation);

}