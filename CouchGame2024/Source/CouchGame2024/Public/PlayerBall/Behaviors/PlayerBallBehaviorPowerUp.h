// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBall/PlayerBallBehaviorComponent.h"
#include "PowerUp/PowerUpID.h"
#include "PlayerBallBehaviorPowerUp.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class COUCHGAME2024_API UPlayerBallBehaviorPowerUp : public UPlayerBallBehaviorComponent
{
	GENERATED_BODY()

public:
	UPlayerBallBehaviorPowerUp();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

public:
	virtual void InitBehavior() override;

	virtual void BindBehaviorEventAction(APlayerBallController* InPlayerBallController) override;

	virtual void UnbindBehaviorEventAction(APlayerBallController* InPlayerBallController) override;

	virtual void SetupData() override;


	
#pragma region PowerUp

public:
	UFUNCTION()
	void OnPlayerSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                                const FHitResult& SweepResult);
	
	EPowerUpID GetCurrentPowerUpCarried() const;

	void AssignPowerUpCarried(EPowerUpID PowerUpID);

	void EmptyCurrentPowerUpCarried();
	
	
	UFUNCTION()
	void UsePowerUpAction(float UsePowerUpValue);
	
	UFUNCTION()
	void UsePowerUpCarried();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUsePowerUpAction, float, PowerUpValueId);

	FOnUsePowerUpAction OnUsePowerUpAction;

	
private:
	UPROPERTY(VisibleAnywhere)
	EPowerUpID CurrentPowerUpCarried;
	
#pragma endregion



#pragma region FreezePowerUp

public:
	float FreezeRange = 40.f;

	float FreezeDuration = 3.f;
	

#pragma endregion

#pragma region StrengthPowerUp

public:
	void ActivateStrengthPowerUp();

	void DesactivateStrengthPowerUp();
	
	void HandleStrengthDuration(float DeltaTime);

	bool GetIsUsingStrengthPowerUp();
	
	float StrengthImpactForceDivider = 3.f;

	float StrengthImpactStunDurationDivider = 2.f;

private:
	bool bUsingStrength = false;

	float TotalStrengthDuration = 5.f;

	float CurrentStrengthDuration = 0.f;

#pragma endregion

#pragma region SlipperyPowerUp

public:
	float SlipperyRadius = 100.f;

	void ReceiveSlipperyEffect();

	void EndSlipperyEffect();

	void HandleSlipperyEffectDuration(float DeltaTime);
	
private:
	bool bIsSlippery = false;

	float CurrentSlipperyEffectDuration = 0.f;
	
	float TotalSlipperyEffectDuration = 5.f;

#pragma endregion
	
};
