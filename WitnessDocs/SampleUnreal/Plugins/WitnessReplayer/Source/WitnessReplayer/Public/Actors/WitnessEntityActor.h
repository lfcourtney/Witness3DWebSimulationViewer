#pragma once

#include "CoreMinimal.h"
#include "Actors/WitnessActor.h"
#include "WitnessEntityActor.generated.h"

UCLASS()
class WITNESSREPLAYER_API AWitnessEntityActor : public AWitnessActor
{
	GENERATED_BODY()

public:

	AWitnessEntityActor();

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UStaticMeshComponent* StaticMeshComponent;
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UStaticMesh* Mesh;

protected:
	
	virtual void BeginPlay() override;

private:

public:

	UFUNCTION(BlueprintCallable)
	void SetGeometry(UStaticMesh* _Mesh);
	
};
