#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "Actors/WitnessActor.h"
#include "WitnessEnviromentManager/WitnessSurface.h"
#include "WitnessSurfaceActor.generated.h"

UCLASS()
class WITNESSREPLAYER_API AWitnessSurfaceActor : public AWitnessActor
{
	GENERATED_BODY()

public:

	AWitnessSurfaceActor();

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UProceduralMeshComponent* ProceduralMeshComponent;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UWitnessSurface* WitnessSurface;

protected:
	
	virtual void BeginPlay() override;

private:

public:

	UFUNCTION(BlueprintCallable)
	void GenerateSurface(UMaterialInstance* Material);
	
};
