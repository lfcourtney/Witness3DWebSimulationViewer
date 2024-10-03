#include "Actors/WitnessStaticActor.h"

AWitnessStaticActor::AWitnessStaticActor()
{
	PrimaryActorTick.bCanEverTick = false;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SplineComponent"));
	SetRootComponent(StaticMeshComponent);
}

void AWitnessStaticActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWitnessStaticActor::SetGeometry(UStaticMesh* _Mesh)
{
	if(_Mesh != nullptr)
	{
		StaticMeshComponent->SetStaticMesh(_Mesh);
	}
}