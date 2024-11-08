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

	virtual void LockBehavior() override;

	virtual void UnlockBehavior() override;
	
#pragma region PowerUp

public:
	UFUNCTION()
	void OnPlayerSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                                const FHitResult& SweepResult);
	
	EPowerUpID GetCurrentPowerUpCarried() const;

	void AssignPowerUpCarried(EPowerUpID PowerUpID);

	UFUNCTION()
	void UsePowerUpAction(float UsePowerUpValue);
	
	UFUNCTION()
	void UsePowerUpCarried();
	
private:
	UPROPERTY(VisibleAnywhere)
	EPowerUpID CurrentPowerUpCarried;
	
#pragma endregion 
};
