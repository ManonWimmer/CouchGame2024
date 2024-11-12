// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBall/PlayerBallBehaviorComponent.h"
#include "PlayerBallBehaviorGrapple.generated.h"


class IHookable;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class COUCHGAME2024_API UPlayerBallBehaviorGrapple : public UPlayerBallBehaviorComponent
{
	GENERATED_BODY()

public:
	UPlayerBallBehaviorGrapple();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

public:
	virtual void InitBehavior() override;

	virtual void BindBehaviorEventAction(APlayerBallController* InPlayerBallController) override;

	virtual void SetupData() override;


#pragma region Grappling

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGrapplingActionStarted, float, GrapplingValue);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGrapplingActionEnded, float, GrapplingValue);
	
	UPROPERTY()
	FOnGrapplingActionStarted OnGrapplingActionStarted;
	UPROPERTY()
	FOnGrapplingActionEnded OnGrapplingActionEnded;

	UFUNCTION()
	void ReceiveGrapplingActionStarted(float InGrapplingValue);
	UFUNCTION()
	void ReceiveGrapplingActionEnded(float InGrapplingValue);

	UPROPERTY(VisibleAnywhere)
	bool CanGrappling = false;
	
	UPROPERTY()
	float GrapplingValue = 0.f;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<APlayerBall> GrappledPlayerBall = nullptr;	// Ball grappled to this playerBall

	UPROPERTY()
	float CableLength = 0.f;

	UPROPERTY()
	FVector HookPoint = FVector(0.f, 0.f, 0.f);

	UPROPERTY()
	float CurrentGrapplingAngularVelocity = 0.f;

	UPROPERTY()
	float CurrentGrapplingAngle = 0.f;

	UPROPERTY()
	bool IsGrappling = false;

	UPROPERTY()
	float LastAngle = 0.f;

	UPROPERTY()
	float AngleRotate = 0.f;

	UPROPERTY()
	FVector ReleaseDirection = FVector(0.f, 0.f, 0.f);

	UPROPERTY()
	FVector GrapplingOffset = FVector(0.f, 0.f, 0.f);

	UPROPERTY()
	float GrapplingDamping = 0.99f;

	UPROPERTY()
	float GrapplingPillarForce = 0.1f;

	UPROPERTY()
	float GrapplingReleaseForce = 250.f;

	UPROPERTY()
	float MinCableDistance = 200.f;

	UPROPERTY()
	float MaxCableDistance = 500.f;

	UPROPERTY()
	float MoreOrLessCablePerFrame = 10.f;

	UPROPERTY()
	float StartGrapplingForceFactorWhenAlreadyMoving = 0.001f;

	UPROPERTY()
	float GrapplingNotPillarForce = 5.f;

	// ----- NEW VERSION - GRAPPLING BETWEEN PLAYER AND HOOK POINT ----- //
	UPROPERTY()
	TObjectPtr<UObject> HookObject;

	UPROPERTY()
	TScriptInterface<IHookable> HookInterface;

	UPROPERTY()
	bool IsHookingPillar = false;
	// ----- NEW VERSION - GRAPPLING BETWEEN PLAYER AND HOOK POINT ----- //

	UPROPERTY()
	float GrapplingCooldown = 3.f;

	UPROPERTY(VisibleAnywhere)
	float CurrentGrapplingCooldown = 0.f;

	UFUNCTION()
	void HandleGrapplingCooldown(float DeltaTime);

	UFUNCTION()
	void ResetGrapplingCooldown();

	UFUNCTION()
	void StartGrapplingCooldown();

#pragma endregion

#pragma region Grappled

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGrappledActionStarted, float, GrappledValue);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGrappledActionEnded, float, GrappledValue);

	
	UPROPERTY()
	FOnGrappledActionStarted OnGrappledActionStarted;
	FOnGrappledActionEnded OnGrappledActionEnded;

	UPROPERTY(VisibleAnywhere)
	bool CanBeGrappled = false;
	
	UFUNCTION()
	void ReceiveGrappledActionStarted(float InGrappledValue);
	void ReceiveGrappledActionEnded(float InGrappledValue);

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<APlayerBall> GrapplingPlayerBall = nullptr;	// Ball grappling to this playerBall

#pragma endregion

#pragma region MoreLessGrappling
public:
	UPROPERTY()
	float MoreLessValue;

private:
	UFUNCTION()
	void MoreLessAction(float InMoreLessValue);
	

#pragma endregion
	
};
