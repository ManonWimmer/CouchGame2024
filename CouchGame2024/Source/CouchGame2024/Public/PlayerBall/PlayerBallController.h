// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerBallController.generated.h"

class UPlayerActionsData;
/**
 * 
 */
UCLASS()
class COUCHGAME2024_API APlayerBallController : public APlayerController
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UPlayerActionsData> PlayerInputsData;
	
	UFUNCTION()
	virtual void SetupInputComponent() override;

	UFUNCTION()
	void SetupMappingContextIntoController() const;

#pragma region Movements Input
public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerMoveXInput, float, XInput);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerMoveYInput, float, YInput);

	UPROPERTY(BlueprintCallable)
	FOnPlayerMoveXInput OnPlayerMoveXInput;
	UPROPERTY(BlueprintCallable)
	FOnPlayerMoveYInput OnPlayerMoveYInput;
	
protected:
	UFUNCTION()
	void MoveXInput(const FInputActionValue& XInput);
	UFUNCTION()
	void MoveYInput(const FInputActionValue& YInput);

	UFUNCTION()
	void BindMoveXInput(UEnhancedInputComponent* EnhancedInputComponent);
	UFUNCTION()
	void BindMoveYInput(UEnhancedInputComponent* EnhancedInputComponent);
	
#pragma endregion

#pragma region Punch

public:

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerPunchInput, float, PunchInput);

	UPROPERTY(BlueprintCallable)
	FOnPlayerPunchInput OnPlayerPunchInput;

	UFUNCTION()
	void PunchInput(const FInputActionValue& PunchInput);

	UFUNCTION()
	void BindPunchInput(UEnhancedInputComponent* EnhancedInputComponent);

# pragma  endregion

#pragma region Grappling Input
public:

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerGrapplingInputStarted, float, GrapplingInput);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerGrapplingInputEnded, float, GrapplingInput);
	
	
	UPROPERTY(BlueprintCallable)
	FOnPlayerGrapplingInputStarted OnPlayerGrapplingInputStarted;
	UPROPERTY(BlueprintCallable)
	FOnPlayerGrapplingInputEnded OnPlayerGrapplingInputEnded;

	UFUNCTION()
	void GrapplingInputStarted(const FInputActionValue& GrapplingInput);
	UFUNCTION()
	void GrapplingInputEnded(const FInputActionValue& GrapplingInput);

	UFUNCTION()
	void BindGrapplingInput(UEnhancedInputComponent* EnhancedInputComponent);

#pragma endregion

#pragma region MoreLessGrapplingInput
public:

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerMoreLessGrapplingInput, float, MoreLessGrapplingInput);

	UPROPERTY()
	FOnPlayerMoreLessGrapplingInput OnPlayerMoreLessGrapplingInput;
	
	UFUNCTION()
	void MoreLessGrapplingInput(const FInputActionValue& InputActionValue);

	UFUNCTION()
	void BindMoreLessGrapplingInput(UEnhancedInputComponent* EnhancedInputComponent);
	
#pragma endregion 
	
};
