// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PlayerBallData.generated.h"

/**
 * 
 */
UCLASS()
class COUCHGAME2024_API UPlayerBallData : public UDataAsset
{
	GENERATED_BODY()

public:

#pragma region Roll Movements
	UPROPERTY(EditAnywhere, Category = "Roll Movements")
	float AngularRollForce = 2000.f;

	UPROPERTY(EditAnywhere, Category = "Roll Movements")
	float AngularRollDamping = 2.f;

	UPROPERTY(EditAnywhere, Category = "Roll Movements")
	float MaxAngularRollVelocity = 12000.f;

	UPROPERTY(EditAnywhere, Category = "Roll Movements")
	float BraqueDirectionForceMultiplier = 2.f;
#pragma endregion

#pragma region Falling Movements (Obsolete)
	/*
	UPROPERTY(EditAnywhere, Category = "Falling Movements Obsolete")
	float SlowFallForce = 50.f;

	UPROPERTY(EditAnywhere, Category = "Falling Movements Obsolete")
	float AccelerateFallForce = 50.f;

	UPROPERTY(EditAnywhere, Category = "Falling Movements Obsolete")
	float AirControlSideAcceleration = 200.f;

	UPROPERTY(EditAnywhere, Category = "Falling Movements Obsolete")
	float AirControlSideMaxSpeed = 600.f;

	UPROPERTY(EditAnywhere, Category = "Falling Movements Obsolete")
	float AirControlSideDeceleration = 1.f;
*/
#pragma endregion

#pragma region Stun
	UPROPERTY(EditAnywhere, Category = "Stun")
	
	float PunchStunCooldown = 3.f;
	UPROPERTY(EditAnywhere, Category = "Stun")
	float ImpactStunCooldown = 1.f;
	
	UPROPERTY(EditAnywhere, Category = "Stun")
	float BumpedHitLagStunCooldown = 0.2f;

#pragma endregion

#pragma region Punch

	UPROPERTY(EditAnywhere, Category = "Punch")
	float PunchRadius = 50.f;

	UPROPERTY(EditAnywhere, Category = "Punch")
	float PunchCooldown = 3.f;
	
	UPROPERTY(EditAnywhere, Category = "Punch")
	float PunchForceMultiplier = 50000.f;
#pragma endregion

#pragma region Impact

	UPROPERTY(EditAnywhere, Category = "Impact")
	float ImpactForceMultiplier = 30000.f;

	UPROPERTY(EditAnywhere, Category = "Impact")
	float ImpactMinTotalForce = 50.f;

#pragma endregion 

#pragma region Bumped

	UPROPERTY(EditAnywhere, Category = "Bumped")
	float BumpedForceMultiplier = 30000.f;

#pragma endregion



#pragma region SnapMagnetic

	UPROPERTY(EditAnywhere, Category = "SnapMagnetic")
	float SnapAngularForce = 3000.f;

	UPROPERTY(EditAnywhere, Category = "SnapMagnetic", meta = (ClampMin = "1.0", ClampMax = "100.0", SliderMin = "1.0", SliderMax = "100.0"))
	float SnapControlMoveRollDivider = 2.f;
	
	UPROPERTY(EditAnywhere, Category = "SnapMagnetic")
	float MinVelocityToSnap = 200.f;

	UPROPERTY(EditAnywhere, Category = "SnapMagnetic")
	float SnapTriggerRadius = 100.f;
	
#pragma endregion

#pragma region Grappling
	UPROPERTY(EditAnywhere, Category = "Grappling")
	float GrapplingDamping = 0.99f;

	UPROPERTY(EditAnywhere, Category = "Grappling")
	float GrapplingForce = 0.1f;

	UPROPERTY(EditAnywhere, Category = "Grappling")
	float GrapplingReleaseForce = 250.f;

	UPROPERTY(EditAnywhere, Category = "Grappling")
	float MinCableDistance = 200.f;

	UPROPERTY(EditAnywhere, Category = "Grappling")
	float MaxCableDistance = 500.f;

	UPROPERTY(EditAnywhere, Category = "Grappling")
	float MoreOrLessCablePerFrame = 10.f;

	UPROPERTY(EditAnywhere, Category = "Grappling")
	float StartGrapplingForceFactorWhenAlreadyMoving = 0.001f;
	
#pragma endregion


#pragma region Starting State

	UPROPERTY(EditAnywhere, Category = "Starting State")
	float StartingStateMinForce = 30000.f;

	UPROPERTY(EditAnywhere, Category = "Starting State")
	float StartingStateMaxForce = 70000.f;

	UPROPERTY(EditAnywhere, Category = "Starting State")
	float PercentAddByStartingSpam = 0.05f;

#pragma endregion 
	
};
