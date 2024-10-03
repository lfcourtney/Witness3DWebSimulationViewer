#pragma once

#include "CoreMinimal.h"
#include "Actors/WitnessActor.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "WitnessManagers/Public/WitnessPathManager/WitnessPath.h"
#include "WitnessPathActor.generated.h"

class ULine;
class UExtrude;
UCLASS()
class WITNESSREPLAYER_API AWitnessPathActor : public AWitnessActor
{
	GENERATED_BODY()
	
public:

	AWitnessPathActor();
	
    UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	USplineComponent* SplineComponent;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	TArray<USplineMeshComponent*> SplineMeshComponents;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UWitnessPath* WitnessPath;
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UStaticMesh* Mesh;

protected:
	
	virtual void BeginPlay() override;

private:

	UPROPERTY(VisibleAnywhere)
	TArray<FVector> SplinePoints;
	
	UPROPERTY(VisibleAnywhere)
	TArray<FVector> SplineTangents;

public:

	UFUNCTION(BlueprintCallable)
	void CreateSpline(float MeshLength, ESplineMeshAxis::Type MeshForward);

	UFUNCTION(BlueprintCallable)
	void AddMeshToSpline(float MeshLength, ESplineMeshAxis::Type MeshForward);
	
};
