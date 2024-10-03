#pragma once

#include "CoreMinimal.h"
#include "Actors/WitnessActor.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "WitnessEnviromentManager/WitnessExtrude.h"
#include "WitnessExtrudeActor.generated.h"

UCLASS()
class WITNESSREPLAYER_API AWitnessExtrudeActor : public AWitnessActor
{
	GENERATED_BODY()

public:

	AWitnessExtrudeActor();

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	USplineComponent* SplineComponent;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	USplineMeshComponent* SplineMeshComponent;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UWitnessExtrude* WitnessExtrude;

protected:
	
	virtual void BeginPlay() override;

public:

	UFUNCTION(BlueprintCallable)
	void GenerateExtrude(UStaticMesh* Geometry, UMaterialInterface* Material);
	
};
