// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "CameraWorldSubsystem.generated.h"


class UCameraData;
class UCameraComponent;
/**
 * 
 */
UCLASS()
class COUCHGAME2024_API UCameraWorldSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

#pragma region Subsystem Overrides
	
public:
	virtual void PostInitialize() override;

	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

	virtual void Tick(float DeltaTime) override;

	virtual TStatId GetStatId() const override { return TStatId(); };

#pragma endregion


public:
	void SetupData();

private:
	
	UPROPERTY()
	TObjectPtr<UCameraData> CameraData;
	
#pragma region MainCamera

public:
	UFUNCTION(BlueprintPure)
	AActor* GetCameraMainActor() const;
	
protected:
	UPROPERTY()
	TObjectPtr<UCameraComponent> CameraMain;

	UCameraComponent* FindCameraByTag(const FName& Tag) const;

	void TickUpdateCameraZoom(float DeltaTime);

	void TickUpdateCameraPosition(float DeltaTime);

#pragma endregion
	
#pragma region Follow Target
	
public:	
	void AddFollowTarget(UObject* FollowTarget);

	void RemoveFollowTarget(UObject* FollowTarget);
	
protected:
	UPROPERTY()
	TArray<UObject*> FollowTargets;

	FVector CalculateAveragePositionBetweenTargets();

	float CalculateGreatestDistanceBetweenTargets();

	float CameraStartPosX = 0.f;
	
	float CameraStartPosY = 0.f;

#pragma endregion

#pragma region Misc
	

#pragma endregion

#pragma region Bounds

protected:
	UPROPERTY()
	FVector2D CameraBoundsMin;

	UPROPERTY()
	FVector2D CameraBoundsMax;

	UPROPERTY()
	float CameraBoundsZProjectionCenter;

	AActor* FindCameraBoundsActor();

	void InitCameraBounds(AActor* CameraBoundsActor);

	void ClampPositionIntoCameraBounds(FVector& Position);

	void GetViewportBounds(FVector2D& OutViewportBoundsMin, FVector2D& OutViewportBoundsMax);

	FVector CalculateWorldPositionFromViewportPosition(const FVector2D& ViewportPosition);

#pragma endregion

#pragma region Zoom

protected:
	UPROPERTY()
	float CameraZoomZMin = 0.f;

	UPROPERTY()
	float CameraZoomZMax = 0.f;

	UPROPERTY()
	float CameraZoomDistanceBetweenTargetsMin = 1000.f;

	UPROPERTY()
	float CameraZoomDistanceBetweenTargetsMax = 4000.f;

	UFUNCTION()
	void InitCameraZoomParameters();


#pragma endregion

#pragma region Smooth
private:
	float CameraSmoothSpeed = 2.f;
	
	float CameraOffsetUp = 0.f;

	float CameraPercentTreshold = 0.6f;
	
#pragma endregion

#pragma region Camera Shakes

public:

	UFUNCTION(BlueprintCallable)
	void PlayPunchCameraShake();

	UFUNCTION(BlueprintCallable)
	void PlayImpactCameraShake();
	
	UFUNCTION(BlueprintCallable)
	void PlayDeathZoneCameraShake();
	
private:
	void InitCameraShake();

	UPROPERTY()
	TObjectPtr<APlayerController> CameraShakePlayerController;


#pragma endregion 

};
