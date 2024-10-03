// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <string>

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SplineData.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Microsoft/AllowMicrosoftPlatformTypes.h"
#include "ProceduralMeshComponent.h"
#include "WitnessObject.generated.h"

class UQueueInfo;

UCLASS(Blueprintable)
class XML_INTERPETER_API AWitnessObject : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWitnessObject();
	virtual ~AWitnessObject(){};

	UPROPERTY(BlueprintReadWrite)
	FString ObjectName; //_instanceName
	
	UPROPERTY(BlueprintReadWrite)
	int32 Time;

	UPROPERTY(BlueprintReadWrite)
	FString ContentPath;

	UPROPERTY(BlueprintReadWrite)
	FString GeometryName;

	UPROPERTY(BlueprintReadOnly)
	FTimerHandle NavigationTimer;

	UPROPERTY(BlueprintReadOnly)
	FVector NavigationStart;

	UPROPERTY(BlueprintReadOnly)
	FVector NavigationEnd;

	UPROPERTY(BlueprintReadOnly)
	float NavigationProcess;

	UPROPERTY(BlueprintReadOnly)
	float NavigationSpeed;

	UFUNCTION(BlueprintCallable, Category = "Functions")
	void StartInterpolate(const FVector Start, const FVector End, const float Speed);

	static float GetDistanceFactor(const float Dist);

	virtual void BeginPlay() override { Super::BeginPlay(); }

	virtual void Tick(float DeltaSeconds) override;
	
	void SimulateNavigation();
};

UCLASS(Blueprintable)
class XML_INTERPETER_API AMachineObject : public AWitnessObject
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMachineObject();
	AMachineObject(UStaticMesh* _Geometry);
	virtual ~AMachineObject(){};

	UPROPERTY(BlueprintReadWrite)
	UStaticMeshComponent* MeshComp;

	UPROPERTY(BlueprintReadWrite)
	UQueueInfo* QueueInfo;

	UPROPERTY(BlueprintReadWrite)
	UStaticMesh* Geometry;

	UFUNCTION(BlueprintCallable)
	void SetGeometry(UStaticMesh* _Geometry);
	
};

UCLASS(Blueprintable)
class XML_INTERPETER_API APathObject : public AWitnessObject
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APathObject();
	APathObject(UStaticMesh* _Geometry, float MeshLength = 100.0f);
	virtual ~APathObject(){};

	UPROPERTY(BlueprintReadWrite)
	USplineComponent* SplineComp;

	//UPROPERTY(BlueprintReadWrite)
	//USplineMeshComponent* SplineMeshComp;
	
	UPROPERTY(BlueprintReadWrite)
	TArray<USplineMeshComponent*> SplineMeshComponents = TArray<USplineMeshComponent*>();

	UPROPERTY(BlueprintReadWrite)
	UQueueInfo* QueueInfo;

	UPROPERTY(BlueprintReadWrite)
	UStaticMesh* Geometry;

	UPROPERTY(BlueprintReadWrite)
	UMaterialInstanceDynamic* Material;

	UPROPERTY(BlueprintReadWrite)
	UPath* Path;

	UFUNCTION(BlueprintCallable)
	void SetGeometry(UStaticMesh* _Geometry, float MeshLength);

	UFUNCTION(BlueprintCallable)
	void SetMaterial(UMaterial* _Material);

	//UFUNCTION(BlueprintCallable)
	//void SetMaterial(UMaterialInstanceDynamic* _Material);

	UFUNCTION(BlueprintCallable)
	static int CheckIfLocationExists(USplineComponent* _SplineComp, FVector _Location)// maybe just an array of FPathData
	{
		_SplineComp->SetMobility(EComponentMobility::Movable);
		int returnValue = 0;
		for (int i = 0; i < _SplineComp->GetNumberOfSplinePoints(); i++)
		{
			FVector SplineLoc = _SplineComp->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World);
			//UE_LOG(LogTemp, Error, TEXT("Z Values: SL: %f, L: %f"), SplineLoc.Z, _Location.Z);
			
			if(SplineLoc.X == _Location.X && SplineLoc.Y == _Location.Y)
			{
				if(SplineLoc.Z < _Location.Z)
				{
					_SplineComp->SetLocationAtSplinePoint(i, _Location, ESplineCoordinateSpace::World);
					return returnValue = i + 1;
				}
				return returnValue = i + 1;
			}
		}
		return returnValue;
	}

	UFUNCTION(BlueprintCallable)
	static void CheckDuplicates(USplineComponent* _SplineComp)// maybe just an array of FPathData
	{
		_SplineComp->SetMobility(EComponentMobility::Movable);
		for (int i = 0; i < _SplineComp->GetNumberOfSplinePoints(); i++)
		{
			for (int j = 0; j < _SplineComp->GetNumberOfSplinePoints(); j++)
			{
				if(j != i)
				{
					FVector PointOne = _SplineComp->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World);
					FVector PointTwo = _SplineComp->GetLocationAtSplinePoint(j, ESplineCoordinateSpace::World);

					FVector2D VecOne = FVector2D(PointOne.X, PointOne.Y);
					FVector2D VecTwo = FVector2D(PointTwo.X, PointTwo.Y);
			
					if(VecOne == VecTwo)
					{
						FVector Location = PointOne;
						float ZValue = (PointOne.Z > PointTwo.Z) ? PointOne.Z : PointTwo.Z;
						Location.Z = ZValue;

						FVector TangentOne = _SplineComp->GetTangentAtSplinePoint(i, ESplineCoordinateSpace::World);
						FVector TangentTwo = _SplineComp->GetTangentAtSplinePoint(j, ESplineCoordinateSpace::World);
				
						FVector NewTangent = (TangentOne + TangentTwo) / 2;
						//FVector NewTangent = (TangentOne + TangentTwo);

						_SplineComp->RemoveSplinePoint(i);
						_SplineComp->SetLocationAtSplinePoint(j, Location, ESplineCoordinateSpace::World);
						_SplineComp->SetTangentsAtSplinePoint(j, TangentOne, TangentTwo, ESplineCoordinateSpace::World);
					}
				}
			}
		}
	}

	//static void CreateSpline(USplineComponent* _SplineComp, TArray<UPathData*> _PathData)// maybe just an array of FPathData
	UFUNCTION(BlueprintCallable)
	static void CreateSpline(AActor* SplineActor, TArray<UPathData*> _PathData, UStaticMesh* _Mesh, float MeshLength, ESplineMeshAxis::Type MeshForward, UMaterialInstanceDynamic* _Material)
	{
		USplineComponent* _SplineComp = SplineActor->FindComponentByClass<USplineComponent>();
		_SplineComp->SetMobility(EComponentMobility::Movable);
		int PositionMultiplier = 100;
		_SplineComp->ClearSplinePoints(true);
		for (UPathData* PathData : _PathData)
		{
			if(PathData->IsA(ULine::StaticClass()))
			{
				ULine* Line = StaticCast<ULine*>(PathData);
				
				FVector Start = FVector(Line->Start.X, Line->Start.Z, Line->Start.Y);
				FVector End = FVector(Line->End.X, Line->End.Z, Line->End.Y);

				Start *= PositionMultiplier;
				End *= PositionMultiplier;

				if(CheckIfLocationExists(_SplineComp, Start) == 0)
				{
					_SplineComp->AddSplinePoint(Start, ESplineCoordinateSpace::World, false);
				}

				if(CheckIfLocationExists(_SplineComp, End) == 0)
				{
					_SplineComp->AddSplinePoint(End, ESplineCoordinateSpace::World, false);
				}
				
			}
			else if(PathData->IsA(UArc::StaticClass()))
			{
				UArc* Arc = StaticCast<UArc*>(PathData);

				bool bIsClockwise = Arc->bIsClockwise;
				float Angle = Arc->Angle;

				FVector Start = FVector(Arc->Start.X, Arc->Start.Z, Arc->Start.Y);
				FVector Centre = FVector(Arc->Centre.X, Arc->Centre.Z, Arc->Centre.Y);
				FVector End = FVector(Arc->End.X, Arc->End.Z, Arc->End.Y);

				Start *= PositionMultiplier;
				Centre *= PositionMultiplier;
				End *= PositionMultiplier;
				
				FVector StartTangents = FVector(0,0,0);
				FVector CentreTangents = FVector(0,0,0);
				FVector EndTangents = FVector(0,0,0);

				float DistX = Start.X - End.X;
				float DistY = Start.Y - End.Y;

				if(Angle == 180.0f)
				{
					if(DistX != 0)
					{
						Centre.Y += bIsClockwise ? DistX / 2 : -DistX / 2;
						
						CentreTangents.X = bIsClockwise ? DistX * 0.75f : -DistX * 0.75f;
						StartTangents.Y = bIsClockwise ? DistX * 0.75f : -DistX * 0.75f;
						EndTangents.Y = bIsClockwise ? -DistX * 0.75f : DistX * 0.75f;
					}
					else
					{
						Centre.X += bIsClockwise ? -DistY / 2 : DistY / 2;
						
						CentreTangents.Y = bIsClockwise ? -DistY * 0.75f : -DistY * 0.75f;
						StartTangents.X = bIsClockwise ? -DistY * 0.75f : DistY * 0.75f;
						EndTangents.X = bIsClockwise ? DistY * 0.75f : -DistY * 0.75f;
					}
				}
				else
				{
					//UE_LOG(LogTemp, Error, TEXT("Angle Is not 180 but is: %f"), Angle);
					
					DistX *= -1;
					DistY *= -1;
					
					CentreTangents.X = (Angle == 90.0f) ? DistX * 0.8 : DistX;
					CentreTangents.Y = (Angle == 90.0f) ? DistY * 0.8 : DistY;

					if((DistX > 0 && DistY > 0) || (DistX < 0 && DistY < 0)) // both positive or negative
					{
						if(bIsClockwise == true)
						{
							Centre.X = (Angle == 90.0f) ? Start.X + DistX * 0.8f : Start.X + DistX * 0.65f;
							Centre.Y = (Angle == 90.0f) ? Start.Y + DistY * 0.2f : Start.Y + DistY * 0.35f;
							StartTangents.X = (Angle == 90.0f) ? DistX: 0.0f;
							EndTangents.Y = (Angle == 90.0f) ? DistY: 0.0f;
						}
						else
						{
							Centre.X = (Angle == 90.0f) ? Start.X + DistX * 0.2f : Start.X + DistX * 0.35f;
							Centre.Y = (Angle == 90.0f) ? Start.Y + DistY * 0.8f : Start.Y + DistY * 0.65f;
							StartTangents.Y = (Angle == 90.0f) ? DistY: 0.0f;
							EndTangents.X = (Angle == 90.0f) ? DistX: 0.0f;
						}
						
					}
					else if(DistX > 0 && DistY < 0 || DistX < 0 && DistY > 0) // X positive and Y negative And Reversed
					{
						if(bIsClockwise == true)
						{
							Centre.X = (Angle == 90) ? Start.X + DistX * 0.2f : Start.X + DistX * 0.35f;
							Centre.Y = (Angle == 90) ? Start.Y + DistY * 0.8f : Start.Y + DistY * 0.65f;
							StartTangents.Y = (Angle == 90) ? DistY: 0.0f;
							EndTangents.X = (Angle == 90) ? DistX: 0.0f;
						}
						else
						{
							Centre.X = (Angle == 90) ? Start.X + DistX * 0.8f : Start.X + DistX * 0.65f;
							Centre.Y = (Angle == 90) ? Start.Y + DistY * 0.2f : Start.Y + DistY * 0.35f;
							StartTangents.X = (Angle == 90) ? DistX: 0.0f;
							EndTangents.Y = (Angle == 90) ? DistY: 0.0f;
						}
					}
				}
				
				
				if(CheckIfLocationExists(_SplineComp, Start) != 0)
				{
					_SplineComp->SetTangentsAtSplinePoint(_SplineComp->GetNumberOfSplinePoints() - 1, StartTangents, StartTangents, ESplineCoordinateSpace::World, false);
				}
				else
				{
					_SplineComp->AddSplinePoint(Start, ESplineCoordinateSpace::World, false);
					_SplineComp->SetTangentsAtSplinePoint(_SplineComp->GetNumberOfSplinePoints() - 1, StartTangents, StartTangents, ESplineCoordinateSpace::World, false);
				}

				_SplineComp->AddSplinePoint(Centre, ESplineCoordinateSpace::World, false);
				_SplineComp->SetTangentsAtSplinePoint(_SplineComp->GetNumberOfSplinePoints() - 1, CentreTangents, CentreTangents, ESplineCoordinateSpace::World, false);

				if(CheckIfLocationExists(_SplineComp, End) != 0)
				{
					_SplineComp->SetTangentsAtSplinePoint(_SplineComp->GetNumberOfSplinePoints() - 1, EndTangents, EndTangents, ESplineCoordinateSpace::World, false);
				}
				else
				{
					_SplineComp->AddSplinePoint(End, ESplineCoordinateSpace::World, false);
					_SplineComp->SetTangentsAtSplinePoint(_SplineComp->GetNumberOfSplinePoints() - 1, EndTangents, EndTangents, ESplineCoordinateSpace::World, false);
				}
				
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Couldnt Convert Path data while making spline!"));
			}
		}

		//CheckDuplicates(_SplineComp);
		
		_SplineComp->UpdateSpline();
		AddMeshToSplineActor(SplineActor, _Mesh, MeshLength, MeshForward, _Material);
	}

	UFUNCTION(BlueprintCallable)
	static void AddMeshToSplineActor(AActor* SplineActor, UStaticMesh* _Mesh, float MeshLength, ESplineMeshAxis::Type MeshForward, UMaterialInstanceDynamic* _Material)
	{
		USplineComponent* _SplineComp = SplineActor->FindComponentByClass<USplineComponent>();
		_SplineComp->SetMobility(EComponentMobility::Movable);
		APathObject* temp = static_cast<APathObject*>(SplineActor);
		if(temp->Material != nullptr)
		{
			_Material = temp->Material;
		}
		
		if(_SplineComp != nullptr)
		{
			APathObject* PathObject = Cast<APathObject>(SplineActor);
			int SplinePoints = _SplineComp->GetNumberOfSplinePoints() - 1;//Old Crooked Spline
			int SplinePointsOnLength = (int)_SplineComp->GetSplineLength() / MeshLength;

			for(int i = 0; i < SplinePoints; i++)// use SplinePointsOnLength for better version when its fixed
			{
				FName name = *FString("SplineMesh: ") + i;
				USplineMeshComponent* SplineMeshComp = NewObject<USplineMeshComponent>(SplineActor, USplineMeshComponent::StaticClass(), name);
				SplineMeshComp->SetMobility(EComponentMobility::Movable);
					
				if(SplineMeshComp != nullptr)
				{
					SplineMeshComp->RegisterComponent(); 
					SplineMeshComp->SetStaticMesh(_Mesh);
					SplineMeshComp->SetForwardAxis(MeshForward, true);

					//Old Crooked Spline
					FVector StartPos = _SplineComp->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local);
					FVector EndPos = _SplineComp->GetLocationAtSplinePoint(i+1, ESplineCoordinateSpace::Local);
					FVector StartTang = _SplineComp->GetTangentAtSplinePoint(i, ESplineCoordinateSpace::Local);
					FVector EndTang = _SplineComp->GetTangentAtSplinePoint(i+1, ESplineCoordinateSpace::Local);

					float StartDistance = i * MeshLength;
					float EndDistance = (i + 1) * MeshLength;
					
					//new more screwed spline
					/*FVector StartPos = _SplineComp->GetLocationAtDistanceAlongSpline(StartDistance, ESplineCoordinateSpace::Local);
					FVector EndPos = _SplineComp->GetLocationAtDistanceAlongSpline(EndDistance, ESplineCoordinateSpace::Local);
					FVector StartTang = _SplineComp->GetLocationAtDistanceAlongSpline(StartDistance, ESplineCoordinateSpace::Local);
					FVector EndTang = _SplineComp->GetLocationAtDistanceAlongSpline(EndDistance, ESplineCoordinateSpace::Local);
					StartTang = StartTang.GetClampedToSize(-MeshLength, MeshLength * 0.5f);
					EndTang = EndTang.GetClampedToSize(-MeshLength, MeshLength * 0.5f);*/
			
					SplineMeshComp->SetStartAndEnd(StartPos, StartTang, EndPos, EndTang ,true);
					SplineMeshComp->SetMaterial(0, _Material);
					SplineMeshComp->AttachToComponent(_SplineComp->GetAttachParent(),FAttachmentTransformRules::KeepRelativeTransform);
					
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("Could not Create splineMeshComponent"));
				}
			
				
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Could not get splineComponent"));
		}
	}
};

UCLASS(Blueprintable)
class XML_INTERPETER_API AExtrudeObject : public AWitnessObject
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AExtrudeObject();
	AExtrudeObject(UStaticMesh* _Geometry);
	virtual ~AExtrudeObject(){};

	UPROPERTY(BlueprintReadWrite)
	USplineComponent* SplineComp;

	UPROPERTY(BlueprintReadWrite)
	USplineMeshComponent* SplineMeshComp;

	UPROPERTY(BlueprintReadWrite)
	UStaticMesh* Geometry;

	UFUNCTION(BlueprintCallable)
	void SetGeometry(UStaticMesh* _Geometry);
	
	UFUNCTION(BlueprintCallable)
	void SetMaterial(UMaterialInstanceDynamic* _Material);

	UPROPERTY(BlueprintReadWrite)
	UExtrude* Extrude;

	UFUNCTION(BlueprintCallable)
	static void CreateSpline(AActor* SplineActor, UExtrude* _Extrude, UStaticMesh* _Geometry)// maybe just an array of FPathData
	{
		USplineComponent* _SplineComp = SplineActor->FindComponentByClass<USplineComponent>();
        _SplineComp->SetMobility(EComponentMobility::Movable);
		int PositionMultiplier = 100;
		_SplineComp->ClearSplinePoints(true);
		ULine* Line = _Extrude->Line;
		FVector Start = FVector(Line->Start.X, Line->Start.Z, Line->Start.Y);
		FVector End = FVector(Line->End.X, Line->End.Z, Line->End.Y);

		Start *= PositionMultiplier;
		End *= PositionMultiplier;

		SplineActor->SetActorLocation(Start);
				
		_SplineComp->AddSplinePoint(Start, ESplineCoordinateSpace::World, false);
		_SplineComp->AddSplinePoint(End, ESplineCoordinateSpace::World, false);

		_SplineComp->UpdateSpline();
		_SplineComp->CastShadow = 0;
		AddMeshToSpline(SplineActor, _Geometry, 100.0f, ESplineMeshAxis::X);
		
	}

	UFUNCTION(BlueprintCallable)
	static void AddMeshToSpline(AActor* SplineActor, UStaticMesh* _Mesh, float MeshLength, ESplineMeshAxis::Type MeshForward)
	{
		USplineComponent* _SplineComp = SplineActor->FindComponentByClass<USplineComponent>();
		_SplineComp->SetMobility(EComponentMobility::Movable);
		if(_SplineComp != nullptr)
		{
			int SplinePoints = _SplineComp->GetNumberOfSplinePoints() - 1;//Old Crooked Spline
			int SplinePointsOnLength = (int)_SplineComp->GetSplineLength() / MeshLength;
			_SplineComp->CastShadow = 0;

			for(int i = 0; i < SplinePoints; i++)// use SplinePointsOnLength for better version when its fixed
			{
				FName name = *FString("SplineMesh: ") + i;
				USplineMeshComponent* SplineMeshComp = NewObject<USplineMeshComponent>(SplineActor, USplineMeshComponent::StaticClass(), name);
				SplineMeshComp->SetMobility(EComponentMobility::Movable);
				SplineMeshComp->CastShadow = 0;
				if(SplineMeshComp != nullptr)
				{
					SplineMeshComp->RegisterComponent(); 
					SplineMeshComp->SetStaticMesh(_Mesh);
					SplineMeshComp->SetForwardAxis(MeshForward, true);

					//Old Crooked Spline
					FVector StartPos = _SplineComp->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World);
					FVector EndPos = _SplineComp->GetLocationAtSplinePoint(i+1, ESplineCoordinateSpace::World);
					FVector StartTang = _SplineComp->GetTangentAtSplinePoint(i, ESplineCoordinateSpace::World);
					FVector EndTang = _SplineComp->GetTangentAtSplinePoint(i+1, ESplineCoordinateSpace::World);

					float StartDistance = i * MeshLength;
					float EndDistance = (i + 1) * MeshLength;
					
					SplineMeshComp->SetStartAndEnd(StartPos, StartTang, EndPos, EndTang ,true);

					SplineMeshComp->AttachToComponent(_SplineComp->GetAttachParent(),FAttachmentTransformRules::KeepRelativeTransform);
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("Could not Create splineMeshComponent"));
				}
			
				
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Could not get splineComponent"));
		}
	}
	
};

UCLASS(Blueprintable)
class XML_INTERPETER_API ASurfaceObject : public AWitnessObject
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASurfaceObject();
	virtual ~ASurfaceObject(){};

	UPROPERTY(BlueprintReadWrite)
	UStaticMeshComponent* MeshComp; // size is distance between min and max. position is center of min and max

	UPROPERTY(BlueprintReadWrite)
	UProceduralMeshComponent* ProcMesh;
	
	UPROPERTY(BlueprintReadWrite)
	USurface* Surface;

	UPROPERTY(BlueprintReadWrite)
	UMaterialInstanceDynamic* Material = nullptr;

	UFUNCTION(BlueprintCallable)
	void SetMaterial(UMaterialInstanceDynamic* _Material);
	
};
