// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "HardlyInputInfoSubsystem.generated.h"

UENUM(BlueprintType)
enum class EHardlyInputType : uint8
{
	MouseAndKeyboard,
	Gamepad
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHardlyInputTypeChangedDelegate, EHardlyInputType, NewInputType);

/**
 * 
 */
UCLASS()
class ZOMBIESURVIVOR_API UHardlyInputInfoSubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	void OnInputKey(const FInputKeyEventArgs& InputKeyEventArgs);
	void OnInputAxis(FViewport* Viewport, int32 ControllerId, FKey Key, float Delta, float DeltaTime, int32 NumSamples, bool bGamepad);
	void OnControllerConnectionChanged(EInputDeviceConnectionState InputDeviceConnectionState, FPlatformUserId PlatformUserId, FInputDeviceId InputDeviceId);

	UPROPERTY(BlueprintAssignable, Category = "Input")
		FHardlyInputTypeChangedDelegate OnInputTypeChanged;

	UFUNCTION(BlueprintPure, Category = "Input")
		EHardlyInputType GetCurrentInputType() const { return CurrentInputType; }

private:
	void SetCurrentInputType(EHardlyInputType NewInputType);
	void BroadcastInputEvent() const;
	
	UPROPERTY()
		EHardlyInputType CurrentInputType = EHardlyInputType::MouseAndKeyboard;
};
