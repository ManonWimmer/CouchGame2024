// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerBallBehaviorComponent.generated.h"


class APlayerBallController;
class APlayerBall;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class COUCHGAME2024_API UPlayerBallBehaviorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPlayerBallBehaviorComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

public:
	virtual void InitBehavior();

	virtual void BindBehaviorEventAction(APlayerBallController* InPlayerBallController);

	virtual void UnbindBehaviorEventAction(APlayerBallController* InPlayerBallController);

	virtual void LockBehavior();

	virtual void UnlockBehavior();
	
	virtual void SetupData();
	
protected:
	virtual APlayerBall* GetPlayerBall() const;
	
	virtual APlayerBallController* GetPlayerBallController() const;

private:
	UPROPERTY()
	TObjectPtr<APlayerBall> PlayerBall;

	UPROPERTY()
	TObjectPtr<APlayerBallController> PlayerBallController;
};
