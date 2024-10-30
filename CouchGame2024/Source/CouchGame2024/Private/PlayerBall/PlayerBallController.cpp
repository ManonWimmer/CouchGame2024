// Fill out your copyright notice in the Description page of Project Settings.


#include "CouchGame2024/Public/PlayerBall/PlayerBallController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "LocalMultiplayerSettings.h"
#include "PlayerBall/Datas/PlayerActionsData.h"


void APlayerBallController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);

	if (EnhancedInputComponent == nullptr) return;

	BindMoveXInput(EnhancedInputComponent);
	BindMoveYInput(EnhancedInputComponent);
	BindGrapplingInput(EnhancedInputComponent);
	BindPunchInput(EnhancedInputComponent);
	BindMoreLessGrapplingInput(EnhancedInputComponent);
}

void APlayerBallController::SetupMappingContextIntoController() const
{
	ULocalPlayer* LocalPlayer = GetLocalPlayer();

	if (LocalPlayer == nullptr) return;

	UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
}

#pragma region MoveInput
void APlayerBallController::MoveXInput(const FInputActionValue& XInput)
{
	OnPlayerMoveXInput.Broadcast(XInput.Get<float>());
}

void APlayerBallController::MoveYInput(const FInputActionValue& YInput)
{
	OnPlayerMoveYInput.Broadcast(YInput.Get<float>());
}

void APlayerBallController::BindMoveXInput(UEnhancedInputComponent* EnhancedInputComponent)
{
	if (EnhancedInputComponent == nullptr) return;

	if (PlayerInputsData == nullptr) return;

	EnhancedInputComponent->BindAction(
		PlayerInputsData->MoveXAction,
		ETriggerEvent::Started,
		this,
		&APlayerBallController::MoveXInput
	);
	EnhancedInputComponent->BindAction(
		PlayerInputsData->MoveXAction,
		ETriggerEvent::Triggered,
		this,
		&APlayerBallController::MoveXInput
	);
	EnhancedInputComponent->BindAction(
		PlayerInputsData->MoveXAction,
		ETriggerEvent::Completed,
		this,
		&APlayerBallController::MoveXInput
	);
}
void APlayerBallController::BindMoveYInput(UEnhancedInputComponent* EnhancedInputComponent)
{
	if (EnhancedInputComponent == nullptr) return;

	if (PlayerInputsData == nullptr) return;

	EnhancedInputComponent->BindAction(
		PlayerInputsData->MoveYAction,
		ETriggerEvent::Started,
		this,
		&APlayerBallController::MoveYInput
	);
	EnhancedInputComponent->BindAction(
		PlayerInputsData->MoveYAction,
		ETriggerEvent::Triggered,
		this,
		&APlayerBallController::MoveYInput
	);
	EnhancedInputComponent->BindAction(
		PlayerInputsData->MoveYAction,
		ETriggerEvent::Completed,
		this,
		&APlayerBallController::MoveYInput
	);
}
#pragma endregion

#pragma region Grappling

void APlayerBallController::GrapplingInputStarted(const FInputActionValue& GrapplingInput)
{
	OnPlayerGrapplingInputStarted.Broadcast(GrapplingInput.Get<float>());
}

void APlayerBallController::GrapplingInputEnded(const FInputActionValue& GrapplingInput)
{
	OnPlayerGrapplingInputEnded.Broadcast(GrapplingInput.Get<float>());
}

void APlayerBallController::BindGrapplingInput(UEnhancedInputComponent* EnhancedInputComponent)
{
	if (EnhancedInputComponent == nullptr) return;

	if (PlayerInputsData == nullptr) return;

	EnhancedInputComponent->BindAction(
		PlayerInputsData->GrapplingAction,
		ETriggerEvent::Started,
		this,
		&APlayerBallController::GrapplingInputStarted
	);
	EnhancedInputComponent->BindAction(
		PlayerInputsData->GrapplingAction,
		ETriggerEvent::Completed,
		this,
		&APlayerBallController::GrapplingInputEnded
	);
}
#pragma endregion

#pragma region PunchInput
void APlayerBallController::PunchInput(const FInputActionValue& PunchInput)
{
	OnPlayerPunchInput.Broadcast(PunchInput.Get<float>());
}

void APlayerBallController::BindPunchInput(UEnhancedInputComponent* EnhancedInputComponent)
{
	if (EnhancedInputComponent == nullptr) return;

	if (PlayerInputsData == nullptr) return;

	EnhancedInputComponent->BindAction(
		PlayerInputsData->PunchAction,
		ETriggerEvent::Started,
		this,
		&APlayerBallController::PunchInput
	);
}
#pragma endregion

#pragma region MoreLessGrapplingInput

void APlayerBallController::MoreLessGrapplingInput(const FInputActionValue& InputActionValue)
{
	OnPlayerMoreLessGrapplingInput.Broadcast(InputActionValue.Get<float>());
}

void APlayerBallController::BindMoreLessGrapplingInput(UEnhancedInputComponent* EnhancedInputComponent)
{
	if (EnhancedInputComponent == nullptr) return;

	if (PlayerInputsData == nullptr) return;

	EnhancedInputComponent->BindAction(
		PlayerInputsData->MoreLessGrappling,
		ETriggerEvent::Started,
		this,
		&APlayerBallController::MoreLessGrapplingInput
		);

	EnhancedInputComponent->BindAction(
		PlayerInputsData->MoreLessGrappling,
		ETriggerEvent::Completed,
		this,
		&APlayerBallController::MoreLessGrapplingInput
		);
}

#pragma endregion 
