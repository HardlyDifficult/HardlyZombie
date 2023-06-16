// Fill out your copyright notice in the Description page of Project Settings.


#include "HardlyInputInfoSubsystem.h"

void UHardlyInputInfoSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if(UWorld* World = GetWorld())
	{
		if(UGameViewportClient* ViewportClient = World->GetGameViewport())
		{
			ViewportClient->OnInputKey().AddUObject(this, &ThisClass::OnInputKey);
			ViewportClient->OnInputAxis().AddUObject(this, &ThisClass::OnInputAxis);
		}
	}
	IPlatformInputDeviceMapper::Get().GetOnInputDeviceConnectionChange().AddUObject(this, &ThisClass::OnControllerConnectionChanged);
}

void UHardlyInputInfoSubsystem::OnInputKey(const FInputKeyEventArgs& InputKeyEventArgs)
{
	if(GetLocalPlayer()->GetControllerId() == InputKeyEventArgs.ControllerId)
	{
		SetCurrentInputType(InputKeyEventArgs.IsGamepad() ? EHardlyInputType::Gamepad : EHardlyInputType::MouseAndKeyboard);
	}
}

void UHardlyInputInfoSubsystem::OnInputAxis(FViewport* /*Viewport*/, int32 ControllerId, FKey /*Key*/, float /*Delta*/,
	float /*DeltaTime*/, int32 /*NumSamples*/, bool bGamepad)
{
	if(GetLocalPlayer()->GetControllerId() == ControllerId)
	{
		SetCurrentInputType(bGamepad ? EHardlyInputType::Gamepad : EHardlyInputType::MouseAndKeyboard);
	}
}

void UHardlyInputInfoSubsystem::OnControllerConnectionChanged(EInputDeviceConnectionState InputDeviceConnectionState,
	FPlatformUserId PlatformUserId, FInputDeviceId InputDeviceId)
{
	if(GetLocalPlayer()->GetPlatformUserId() == PlatformUserId)
	{
		if(InputDeviceConnectionState != EInputDeviceConnectionState::Connected)
		{
			SetCurrentInputType(EHardlyInputType::MouseAndKeyboard);
		}
	}
}

void UHardlyInputInfoSubsystem::SetCurrentInputType(EHardlyInputType NewInputType)
{
	if(CurrentInputType != NewInputType)
	{
		CurrentInputType = NewInputType;
		BroadcastInputEvent();
	}
}

void UHardlyInputInfoSubsystem::BroadcastInputEvent() const
{
	const UWorld* World = GetWorld();
	if(World && !World->bIsTearingDown)
	{
		OnInputTypeChanged.Broadcast(CurrentInputType);
	}
}
