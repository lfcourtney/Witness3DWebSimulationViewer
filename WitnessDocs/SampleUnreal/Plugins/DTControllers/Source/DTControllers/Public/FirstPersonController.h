#pragma once

#include "CoreMinimal.h"
#include "FirstPersonController.generated.h"

/**
 * A first person player controller.
 */
UCLASS()
class DTCONTROLLERS_API AFirstPersonController : public APlayerController
{
	GENERATED_BODY()

public:
	/** Sets default values for this controller's properties. */
	AFirstPersonController();

protected:
	/** Called when the game starts or when spawned. */
	virtual void BeginPlay() override;

	/** Allows the PlayerController to set up custom input bindings. */
	virtual void SetupInputComponent() override;

public:
	/** Called when the MoveForward input binding is triggered. */
	virtual void MoveForward(float Direction);

	/** Called when the MoveLeft input binding is triggered. */
	virtual void MoveLeft(float Direction);

	/** Called when the MousePitch input binding is triggered. */
	virtual void MousePitch(float Delta);

	/** Called when the MouseYaw input binding is triggered. */
	virtual void MouseYaw(float Delta);
};