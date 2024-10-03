#pragma once

#include "CoreMinimal.h"
#include "Actors/WitnessActor.h"
#include "WitnessStaticManager/WitnessStatic.h"
#include "WitnessStaticActor.generated.h"

UCLASS()
class WITNESSREPLAYER_API AWitnessStaticActor : public AWitnessActor
{
	GENERATED_BODY()

public:

	AWitnessStaticActor();

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UStaticMeshComponent* StaticMeshComponent;
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UStaticMesh* Mesh;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UWitnessStatic* WitnessStatic;

protected:
	
	virtual void BeginPlay() override;

private:

public:

	UFUNCTION(BlueprintCallable)
	void SetGeometry(UStaticMesh* _Mesh);
	
};
