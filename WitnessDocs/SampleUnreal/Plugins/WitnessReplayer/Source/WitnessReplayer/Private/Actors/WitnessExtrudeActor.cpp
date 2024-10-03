#include "Actors/WitnessExtrudeActor.h"

AWitnessExtrudeActor::AWitnessExtrudeActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
	SetRootComponent(SplineComponent);

	SplineMeshComponent = CreateDefaultSubobject<USplineMeshComponent>(TEXT("SplineMeshComponent"));
	SplineMeshComponent->AttachToComponent(SplineComponent->GetAttachParent(),FAttachmentTransformRules::KeepRelativeTransform);
}

void AWitnessExtrudeActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWitnessExtrudeActor::GenerateExtrude(UStaticMesh* Geometry, UMaterialInterface* Material)
{
	//Spline
    SplineComponent->SetMobility(EComponentMobility::Movable);
	SplineComponent->ClearSplinePoints(true);
	
	FVector Start = TransformLocation(WitnessExtrude->StartLocation);
	FVector End = TransformLocation(WitnessExtrude->EndLocation);

	SetActorLocation(Start);
			
	SplineComponent->AddSplinePoint(Start, ESplineCoordinateSpace::World, false);
	SplineComponent->AddSplinePoint(End, ESplineCoordinateSpace::World, false);

	SplineComponent->UpdateSpline();
	SplineComponent->CastShadow = 0;
	
	//SplineMesh
	SplineMeshComponent->SetMobility(EComponentMobility::Movable);
	SplineMeshComponent->CastShadow = 0;
	SplineMeshComponent->SetStaticMesh(Geometry);
	SplineMeshComponent->SetMaterial(0, Material);
	SplineMeshComponent->SetForwardAxis(ESplineMeshAxis::X, true);

	FVector StartPos = SplineComponent->GetLocationAtSplinePoint(0, ESplineCoordinateSpace::World);
	FVector StartTang = SplineComponent->GetTangentAtSplinePoint(0, ESplineCoordinateSpace::World);
	FVector EndPos = SplineComponent->GetLocationAtSplinePoint(1, ESplineCoordinateSpace::World);
	FVector EndTang = SplineComponent->GetTangentAtSplinePoint(1, ESplineCoordinateSpace::World);
	
	SplineMeshComponent->SetStartAndEnd(StartPos, StartTang, EndPos, EndTang ,true);
}
