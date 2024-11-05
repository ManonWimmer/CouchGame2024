// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBall/PlayerBallBehaviorComponent.h"
#include "PlayerBallBehaviorMovements.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class COUCHGAME2024_API UPlayerBallBehaviorMovements : public UPlayerBallBehaviorComponent
{
	GENERATED_BODY()

public:
	UPlayerBallBehaviorMovements();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
public:
	virtual void InitBehavior() override;

	virtual void BindBehaviorEventAction(APlayerBallController* InPlayerBallController) override;

	virtual void SetupData() override;

private:

	UFUNCTION()
	void MoveXAction(float XValue);
	UFUNCTION()	
	void MoveYAction(float YValue);

public:

	UFUNCTION(BlueprintCallable)
	bool IsGrounded();
	
	UPROPERTY()
	float MoveXValue = 0.f;
	UPROPERTY()
	float MoveYValue = 0.f;

	// roll Datas
	UPROPERTY()
	float AngularRollForce = 2000.f;
	UPROPERTY()
	float BraqueDirectionForceMultiplier = 1.f;
};
