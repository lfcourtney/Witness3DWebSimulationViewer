#include "Actors/WitnessPathActor.h"

AWitnessPathActor::AWitnessPathActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
	SplineComponent->ClearSplinePoints();
}

void AWitnessPathActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWitnessPathActor::CreateSpline(float MeshLength, ESplineMeshAxis::Type MeshForward)
{
	if(WitnessPath == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get WitnessPath*"));
		return;
	}
	
	const int PathNum = (int)(WitnessPath->Length * 4.0f) + 1;
	for(int i = 0; i <= PathNum; i++)
	{
		//FVector Temp = WitnessPath->GetPointAt(static_cast<float>(i) / PathNum);
		FVector Temp = WitnessPath->GetPointAtExact(static_cast<float>(i) / PathNum);
		Temp = TransformLocation(Temp);
		SplinePoints.Add(Temp);
	}
	if(SplinePoints.Num() > 0)
	{
		//UE_LOG(LogTemp, Warning, TEXT("SplinePoints.Num() = %d"), SplinePoints.Num());
		for(int i = 0; i < SplinePoints.Num(); i++)
		{
			SplineComponent->AddSplinePoint(SplinePoints[i], ESplineCoordinateSpace::World, true);
		}
	}
	
	for(int i = 0; i <= PathNum; i++)
	{
		//FVector Temp = WitnessPath->GetTangentAt(static_cast<float>(i) / PathNum);
		FVector Temp = WitnessPath->GetTangentAtExact(static_cast<float>(i) / PathNum);
		Temp = TransformLocation(Temp);
		Temp.Normalize();
		Temp *= -1;
		SplineTangents.Add(Temp);
	}
	if(SplineTangents.Num() > 0)
	{
		//UE_LOG(LogTemp, Warning, TEXT("SplineTangents.Num() = %d"), SplineTangents.Num());
		for(int i = 0; i < SplineTangents.Num(); i++)
		{
			SplineComponent->SetTangentAtSplinePoint(i, SplineTangents[i], ESplineCoordinateSpace::World, true);
		}
	}
	
	SplineComponent->UpdateSpline();
	SplineComponent->CastShadow = 0;

	if(Mesh != nullptr)
	{
		AddMeshToSpline(MeshLength, MeshForward);
	}
	
}

void AWitnessPathActor::AddMeshToSpline(float MeshLength, ESplineMeshAxis::Type MeshForward)
{
	SplineComponent->SetMobility(EComponentMobility::Movable);
	
	int NumberOfMeshes = FMath::TruncToInt(SplineComponent->GetSplineLength() / MeshLength);

	for (int i = 0; i <= NumberOfMeshes; i++)
	{
		FName name = *(InstanceName + FString(" SplineMesh: ") + FString::FromInt(i));
		//UE_LOG(LogTemp, Warning, TEXT("%s"), *name.ToString());
		USplineMeshComponent* SplineMeshComp = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass(), name);
		if(SplineMeshComp != nullptr)
		{
			SplineMeshComp->RegisterComponent(); 
			SplineMeshComp->SetMobility(EComponentMobility::Movable);
			SplineMeshComp->CastShadow = 0;
			SplineMeshComp->SetStaticMesh(Mesh);
			SplineMeshComp->SetForwardAxis(MeshForward, true);

			float StartDistance = i * MeshLength;
			float EndDistance = (i+1) * MeshLength;
			
			FVector StartPos = SplineComponent->GetLocationAtDistanceAlongSpline(StartDistance, ESplineCoordinateSpace::Local);
			FVector EndPos = SplineComponent->GetLocationAtDistanceAlongSpline(EndDistance, ESplineCoordinateSpace::Local);
			FVector StartTang = SplineComponent->GetTangentAtDistanceAlongSpline(StartDistance, ESplineCoordinateSpace::Local);
			FVector EndTang = SplineComponent->GetTangentAtDistanceAlongSpline(EndDistance, ESplineCoordinateSpace::Local);

			StartTang.Normalize();
			EndTang.Normalize();

			StartTang *= MeshLength;
			EndTang *= MeshLength;
			
			SplineMeshComp->SetStartAndEnd(StartPos, StartTang, EndPos, EndTang ,true);

			SplineMeshComp->AttachToComponent(SplineComponent->GetAttachParent(),FAttachmentTransformRules::KeepRelativeTransform);
			SplineMeshComponents.Add(SplineMeshComp);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Could not Create splineMeshComponent"));
		}

		for (USplineMeshComponent* SplineMeshComponent : SplineMeshComponents)
		{
			SplineMeshComponent->UpdateMesh_Concurrent();
		}
	}
}
