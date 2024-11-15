// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBall/PlayerBallBehaviorComponent.h"
#include "PlayerBallBehaviorElementReactions.generated.h"


class ARailElement;
class APinballElement;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class COUCHGAME2024_API UPlayerBallBehaviorElementReactions : public UPlayerBallBehaviorComponent
{
	GENERATED_BODY()

public:
	UPlayerBallBehaviorElementReactions();

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


	
	UFUNCTION()
	void OnCollisionHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	UFUNCTION()
	void OnCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                             const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnAttractionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                              const FHitResult& SweepResult);
	UFUNCTION()
	void OnAttractionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

#pragma region Stun

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStunedAction, float, StunValue);
	
	UPROPERTY()
	FOnStunedAction OnStunnedAction;

	UFUNCTION(BlueprintCallable)
	void ReceiveStunnedAction(float InStunnedDurationValue);
	
	UPROPERTY()
	float PunchStunCooldown = 3.f;

#pragma	endregion

#pragma region Impact
public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnImpactAction, float, ImpactValue);
	
	UPROPERTY()
	FOnImpactAction OnImpactAction;

	UPROPERTY()
	TObjectPtr<APlayerBall> ImpactedPlayerBall;

	UPROPERTY()
	float ImpactForceMultiplier = 30000.f;

	UPROPERTY()
	float ImpactMinTotalForce = 50.f;

	UPROPERTY()
	float ImpactStunCooldown = 1.f;
	
	UFUNCTION()
	void ReceiveImpactAction(float ImpactValue, const FVector &InNormalImpact);

	
	UPROPERTY()
	FVector NormalImpact = FVector(0, 0, 0);

#pragma endregion

		
#pragma region Snapping
public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnReceiveSnappingAction, float, SnappingValue);

	UPROPERTY()
	FOnReceiveSnappingAction OnReceiveSnappingAction;

	UFUNCTION()
	void ReceiveSnappingAction(float SnappingValue);
	
	UPROPERTY()
	TObjectPtr<APlayerBall> SnappingPlayerBall;

	float SnapAngularForce = 3000.f;

	float SnapControlMoveRollDivider = 2.f;

	float MinVelocityToSnap = 200.f;

#pragma endregion 

#pragma region Reaction Pinball Elements

public:
	UPROPERTY()
	TObjectPtr<APinballElement> HitPinballElement;

	
#pragma region Bumper Reaction
public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBumperReaction, float, BumperReactionValue);

	FOnBumperReaction OnBumperReaction;

	UPROPERTY()
	float BumpedForceMultiplier = 30000.f;

	UPROPERTY()
	float BumpedHitLagCooldown = 0.2f;
	
	UPROPERTY()
	FVector NormalBump = FVector(0, 0, 0);
	
private:
	UFUNCTION()
	void ReceiveBumperReaction(APinballElement* Element, const FVector &InNormalBump);

#pragma endregion

#pragma region Death Zones reaction

private:
	UFUNCTION()
	void ReceiveDeathReaction();

#pragma endregion

#pragma region Rail Reactions

public:

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRailReaction, float, ReactionValue);

	FOnRailReaction OnRailReaction;
	
	UPROPERTY()
	TObjectPtr<ARailElement> CurrentRailElement;

	UFUNCTION()
	void ReceiveRailReaction(APinballElement* PinballRailElement, float DirectionValue);

	
	
#pragma endregion
	
#pragma endregion
	
};
