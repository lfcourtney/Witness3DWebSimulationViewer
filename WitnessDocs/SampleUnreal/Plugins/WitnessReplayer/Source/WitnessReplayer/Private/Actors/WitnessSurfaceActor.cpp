#include "Actors/WitnessSurfaceActor.h"

AWitnessSurfaceActor::AWitnessSurfaceActor()
{
	PrimaryActorTick.bCanEverTick = false;

	ProceduralMeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMeshComponent"));
	SetRootComponent(ProceduralMeshComponent);
}

void AWitnessSurfaceActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWitnessSurfaceActor::GenerateSurface(UMaterialInstance* Material)
{
	UE_LOG(LogTemp, Error, TEXT("GenerateSurface"));
	ProceduralMeshComponent->ClearAllMeshSections();
	
	FVector TL = TransformLocation(WitnessSurface->BottomLeftCorner);
	FVector BR = TransformLocation(WitnessSurface->TopRightCorner);

	UE_LOG(LogTemp, Error, TEXT("Surface Corners: TL: %s, BR: %s"), *TL.ToString(), *BR.ToString());
	
	FVector Vert1 = TL;
	FVector Vert2 = FVector(BR.X, TL.Y, TL.Z);
	FVector Vert3 = FVector(TL.X, BR.Y, TL.Z);
	FVector Vert4 = BR;
	
	TArray<FVector> Verts = TArray<FVector>();
	Verts.Add(Vert1);
	Verts.Add(Vert2);
	Verts.Add(Vert3);
	Verts.Add(Vert4);
	
	TArray<int32> Tris = TArray<int32>();
	Tris.Add(2);
	Tris.Add(1);
	Tris.Add(0);
	Tris.Add(3);
	Tris.Add(1);
	Tris.Add(2);

	FVector Normal = TransformLocation(WitnessSurface->Normal);
	TArray<FVector> Normals = TArray<FVector>();
	Normals.Add(Normal);
	Normals.Add(Normal);
	Normals.Add(Normal);
	Normals.Add(Normal);
	
	TArray<FVector2D> UVs = TArray<FVector2D>();
	UVs.Add(FVector2D(0,0));
	UVs.Add(FVector2D(1,0));
	UVs.Add(FVector2D(0,1));
	UVs.Add(FVector2D(1,1));
	
	TArray<FColor> Colors = TArray<FColor>();
	Colors.Add(WitnessSurface->Color1);
	Colors.Add(WitnessSurface->Color1);
	Colors.Add(WitnessSurface->Color1);
	Colors.Add(WitnessSurface->Color1);
	
	TArray<FProcMeshTangent> Tangents = TArray<FProcMeshTangent>();
	Tangents.Add(FProcMeshTangent(1, 1, 1));
	Tangents.Add(FProcMeshTangent(1, 1, 1));
	Tangents.Add(FProcMeshTangent(1, 1, 1));
	Tangents.Add(FProcMeshTangent(1, 1, 1));

	ProceduralMeshComponent->CreateMeshSection(0, Verts, Tris, Normals, UVs, Colors, Tangents, false);
	ProceduralMeshComponent->SetMaterial(0, Material);
}
