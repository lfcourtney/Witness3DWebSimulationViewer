// Fill out your copyright notice in the Description page of Project Settings.


#include "WitnessObject.h"

#include "Action.h"
#include "DrawDebugHelpers.h"
#include "XmlFile.h"
#include "Engine/StaticMeshSocket.h"


// Sets default values
AWitnessObject::AWitnessObject()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AWitnessObject::StartInterpolate(const FVector Start, const FVector End, const float Speed)
{
	SetActorLocation(Start);
	//UE_LOG(LogTemp, Log, TEXT("Timer Start!"));
	GetWorld()->GetTimerManager().SetTimer(NavigationTimer, this
	                                       , &AWitnessObject::SimulateNavigation, GetWorld()->DeltaTimeSeconds, true);

	const float Dist = FVector::Distance(Start, End);
	const float DistFactor = GetDistanceFactor(Dist);
	// debug code for distance: GEngine->AddOnScreenDebugMessage(-1,1,FColor::Green,FString::SanitizeFloat(Dist), true);

	NavigationStart = Start;
	NavigationEnd = End;
	NavigationProcess = 0;
	NavigationSpeed = (Speed * GetWorld()->DeltaTimeSeconds) / (Dist / DistFactor);
}

float AWitnessObject::GetDistanceFactor(const float Dist)
{
	return FMath::RoundToInt(Dist / 10);

	if (Dist > 80)
		return 10;
	if (Dist > 50)
		return 5;
	if (Dist > 20)
		return 2.5;
	return 1;
}

void AWitnessObject::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	/*UStaticMeshComponent* MeshComp = FindComponentByClass<UStaticMeshComponent>();
	if(MeshComp != nullptr)
	{
		UStaticMesh* mesh = MeshComp->GetStaticMesh();
		if (mesh)
		{
			if (mesh->Sockets.Num() > 0)
			{
				DrawDebugSphere(GetWorld(), GetActorLocation() + mesh->Sockets[0]->RelativeLocation, 100, 50, FColor::Red);
			}
		}
	}*/
}

void AWitnessObject::SimulateNavigation()
{
	NavigationProcess = NavigationProcess + NavigationSpeed;
	FVector CaculatedLocation = FMath::LerpStable(NavigationStart, NavigationEnd, NavigationProcess);

	SetActorLocation(CaculatedLocation);
	if (NavigationProcess >= 1)
	{
		GetWorld()->GetTimerManager().ClearTimer(NavigationTimer);
		//UE_LOG(LogTemp, Log, TEXT("Navigation Done"));
	}
}

AMachineObject::AMachineObject()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	SetRootComponent(MeshComp);
	if (Geometry != nullptr)
	{
		UStaticMeshSocket* NewSocket = NewObject<UStaticMeshSocket>();
		NewSocket->SocketName = FName("Queue");
		NewSocket->RelativeLocation = QueueInfo->Position;
		Geometry->AddSocket(NewSocket);
		MeshComp->SetStaticMesh(Geometry);
	}
}

AMachineObject::AMachineObject(UStaticMesh* _Geometry)
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	SetRootComponent(MeshComp);
	Geometry = _Geometry;
	UStaticMeshSocket* NewSocket = NewObject<UStaticMeshSocket>();
	NewSocket->SocketName = FName("Queue");
	NewSocket->RelativeLocation = QueueInfo->Position;
	Geometry->AddSocket(NewSocket);
	MeshComp->SetStaticMesh(Geometry);
}

void AMachineObject::SetGeometry(UStaticMesh* _Geometry)
{
	MeshComp->SetStaticMesh(_Geometry);
	Geometry = _Geometry;
	MeshComp->SetMobility(EComponentMobility::Movable);
}

APathObject::APathObject()
{
	SplineComp = CreateDefaultSubobject<USplineComponent>("SplineComp");
	SetRootComponent(SplineComp);
	if (GetWorld())
	{
		SplineComp->SetMobility(EComponentMobility::Movable);
		SplineComp->RegisterComponent();
	}
	//SplineMeshComp->SetupAttachment(RootComponent);
	const ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("/Game/Temp/TempConveyor"));
	Geometry = MeshObj.Object;
	if (Geometry == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Cant Get Geometry"));
	}
	/*else
	{
		if (Path != nullptr)
		{
			UMaterial* ParentMaterial = Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(),
																	 GetWorld(),
																	 TEXT(
																		 "/Game/Temp/WallMat")));
			UMaterialInstanceDynamic* Material = UMaterialInstanceDynamic::Create(ParentMaterial, ParentMaterial->GetWorld(), FName("MaterialName"));
					
			CreateSpline(this, Path->PathData, Geometry, 100.0f, ESplineMeshAxis::X, Material);
		}
	}*/
}

APathObject::APathObject(UStaticMesh* _Geometry, float MeshLength)
{
	SplineComp = CreateDefaultSubobject<USplineComponent>("SplineComp");
	//SplineMeshComp = CreateDefaultSubobject<USplineMeshComponent>("SplineMeshComp");
	SetRootComponent(SplineComp);
	SplineComp->Mobility = EComponentMobility::Movable;
	SplineComp->RegisterComponent();

	//SplineComp->SetupAttachment(RootComponent);
	Geometry = _Geometry;
	UMaterial* ParentMaterial = Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(),
	                                                             GetWorld(),
	                                                             TEXT(
		                                                             "/Game/Temp/WallMat")));
	Material = UMaterialInstanceDynamic::Create(ParentMaterial, ParentMaterial->GetWorld(), FName("MaterialName"));

	CreateSpline(this, Path->PathData, Geometry, 100.0f, ESplineMeshAxis::X, Material);
}

void APathObject::SetGeometry(UStaticMesh* _Geometry, float MeshLength = 100.0f)
{
	SplineComp->SetMobility(EComponentMobility::Movable);
	SplineComp->RegisterComponent();
	for (auto splineComps : SplineMeshComponents)
	{
		splineComps->SetMobility(EComponentMobility::Movable);
		splineComps->RegisterComponent();
	}

	Geometry = _Geometry;
	UMaterial* ParentMaterial = Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(),
	                                                             GetWorld(),
	                                                             TEXT(
		                                                             "/Game/Temp/WallMat")));
	Material = UMaterialInstanceDynamic::Create(ParentMaterial, ParentMaterial->GetWorld(), FName("MaterialName"));
	//CreateSpline(this, Path->PathData, Geometry, 100.0f, ESplineMeshAxis::X, Material);
}

void APathObject::SetMaterial(UMaterial* _Material)
{
	Material = UMaterialInstanceDynamic::Create(_Material, _Material->GetWorld(), FName("MaterialName"));
	SplineComp->SetMobility(EComponentMobility::Movable);
}

/*void APathObject::SetMaterial(UMaterialInstanceDynamic* _Material)
{
	UE_LOG(LogTemp, Warning, TEXT("Set Material for Path"));
	for (auto SplineMesh : SplineMeshComponents)
	{
		SplineMesh->SetMaterial(0, _Material);
		//SplineMesh->SetMaterial(1, _Material);
	}
}*/

AExtrudeObject::AExtrudeObject()
{
	SplineComp = CreateDefaultSubobject<USplineComponent>("SplineComp");
	SplineMeshComp = CreateDefaultSubobject<USplineMeshComponent>("SplineMeshComp");
	SplineComp->SetupAttachment(RootComponent);
	SplineMeshComp->SetupAttachment(RootComponent);

	if (GetWorld())
	{
		SplineMeshComp->SetMobility(EComponentMobility::Movable);
		SplineMeshComp->RegisterComponent();
		SplineComp->SetMobility(EComponentMobility::Movable);
		SplineComp->RegisterComponent();
	}
	
	
	const ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("/Game/Temp/Wall"));
	Geometry = MeshObj.Object;
	if (Geometry == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Cant Get Geometry"));
	}
}

AExtrudeObject::AExtrudeObject(UStaticMesh* _Geometry)
{
	SplineComp = CreateDefaultSubobject<USplineComponent>("SplineComp");
	SplineMeshComp = CreateDefaultSubobject<USplineMeshComponent>("SplineMeshComp");
	SplineComp->SetupAttachment(RootComponent);
	SplineMeshComp->SetupAttachment(RootComponent);
	SplineMeshComp->Mobility = EComponentMobility::Movable;
	SplineMeshComp->RegisterComponent();
	SplineComp->Mobility = EComponentMobility::Movable;
	SplineComp->RegisterComponent();
	Geometry = _Geometry;
	CreateSpline(this, Extrude, Geometry);
}


void AExtrudeObject::SetGeometry(UStaticMesh* _Geometry)
{
	Geometry = _Geometry;
	CreateSpline(this, Extrude, Geometry);
}

void AExtrudeObject::SetMaterial(UMaterialInstanceDynamic* _Material)
{
	UE_LOG(LogTemp, Display, TEXT("Setting Material for extrude!"));
	SplineMeshComp->SetMaterial(1, _Material);
}

ASurfaceObject::ASurfaceObject()
{
	//MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	//SetRootComponent(MeshComp);

	ProcMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("MeshComp"));
	SetRootComponent(ProcMesh);
	

	//UFUNCTION(BlueprintCallable, Category = "Components|ProceduralMesh", meta = (AutoCreateRefTerm = "Normals,UV0,VertexColors,Tangents"))
	// void CreateMeshSection(int32 SectionIndex, const TArray& Vertices, const TArray& Triangles, const TArray& Normals, // const TArray& UV0, const TArray& VertexColors, const TArray& Tangents, bool bCreateCollision);

	TArray<FVector> vertices;
	vertices.Add(FVector(0, 0, 0));
	vertices.Add(FVector(0, 100, 0));
	vertices.Add(FVector(0, 0, 100));
	vertices.Add(FVector(0, 100, 100));
	
	TArray<int32> Triangles;
	Triangles.Add(1);
	Triangles.Add(0);
	Triangles.Add(2);
	Triangles.Add(1);
	Triangles.Add(2);
	Triangles.Add(3);
	
	TArray<FVector> normals;
	normals.Add(FVector(1, 0, 0));
	normals.Add(FVector(1, 0, 0));
	normals.Add(FVector(1, 0, 0));
	normals.Add(FVector(1, 0, 0));
	
	TArray<FVector2D> UV0;
	UV0.Add(FVector2D(0, 0));
	UV0.Add(FVector2D(10, 0));
	UV0.Add(FVector2D(0, 10));
	UV0.Add(FVector2D(10 ,10));

	TArray<FColor> vertexColors;
	vertexColors.Add(FColor(100, 100, 100, 100));
	vertexColors.Add(FColor(100, 100, 100, 100));
	vertexColors.Add(FColor(100, 100, 100, 100));
	vertexColors.Add(FColor(100, 100, 100, 100));
	TArray<FProcMeshTangent> tangents;
	tangents.Add(FProcMeshTangent(1, 1, 1));
	tangents.Add(FProcMeshTangent(1, 1, 1));
	tangents.Add(FProcMeshTangent(1, 1, 1));
	tangents.Add(FProcMeshTangent(1, 1, 1));
	ProcMesh->CreateMeshSection(0, vertices, Triangles, normals, UV0, vertexColors, tangents, false);
	ProcMesh->CastShadow = 0;
	UMaterial* ParentMaterial = Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(),
	                                                             GetWorld(),
	                                                             TEXT(
		                                                             "/Game/Temp/FloorMat")));
	Material = UMaterialInstanceDynamic::Create(ParentMaterial, ParentMaterial->GetWorld(), FName("MaterialName"));
}

void ASurfaceObject::SetMaterial(UMaterialInstanceDynamic* _Material)
{
	UE_LOG(LogTemp, Display, TEXT("Setting Material for surface!"));
	if (_Material == nullptr && Material != nullptr)
	{
		ProcMesh->SetMaterial(0, Material);
	}
	else
	{
		ProcMesh->SetMaterial(0, _Material);
	}
}
