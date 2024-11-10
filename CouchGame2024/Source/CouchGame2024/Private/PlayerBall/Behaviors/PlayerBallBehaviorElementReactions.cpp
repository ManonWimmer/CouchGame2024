// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBall/Behaviors/PlayerBallBehaviorElementReactions.h"

#include "Components/SphereComponent.h"
#include "PinballElements/PinballElement.h"
#include "PlayerBall/PlayerBall.h"
#include "PlayerBall/Datas/PlayerBallData.h"


UPlayerBallBehaviorElementReactions::UPlayerBallBehaviorElementReactions()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UPlayerBallBehaviorElementReactions::BeginPlay()
{
	Super::BeginPlay();
	
}


void UPlayerBallBehaviorElementReactions::TickComponent(float DeltaTime, ELevelTick TickType,
                                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UPlayerBallBehaviorElementReactions::InitBehavior()
{
	Super::InitBehavior();

	if (GetPlayerBall() == nullptr)	return;

	if (GetPlayerBall()->SphereCollision != nullptr)
	{
		if (GetPlayerBall()->SphereCollision != nullptr)
		{
			GetPlayerBall()->SphereCollision->OnComponentHit.AddDynamic(this, &UPlayerBallBehaviorElementReactions::OnCollisionHit);
		}
	}

	if (GetPlayerBall()->AttractionSphere != nullptr)
	{
		GetPlayerBall()->AttractionSphere->OnComponentBeginOverlap.AddDynamic(this, &UPlayerBallBehaviorElementReactions::OnAttractionBeginOverlap);
		GetPlayerBall()->AttractionSphere->OnComponentEndOverlap.AddDynamic(this, &UPlayerBallBehaviorElementReactions::OnAttractionEndOverlap);
	}
}

void UPlayerBallBehaviorElementReactions::BindBehaviorEventAction(APlayerBallController* InPlayerBallController)
{
	Super::BindBehaviorEventAction(InPlayerBallController);

	
}

void UPlayerBallBehaviorElementReactions::UnbindBehaviorEventAction(APlayerBallController* InPlayerBallController)
{
	Super::UnbindBehaviorEventAction(InPlayerBallController);

	
}

void UPlayerBallBehaviorElementReactions::SetupData()
{
	Super::SetupData();

	if (GetPlayerBall() == nullptr)	return;
	if (GetPlayerBall()->GetPlayerBallData() == nullptr)	return;
	
	// Impact
	ImpactForceMultiplier = GetPlayerBall()->GetPlayerBallData()->ImpactForceMultiplier;
	ImpactMinTotalForce = GetPlayerBall()->GetPlayerBallData()->ImpactMinTotalForce;
	ImpactStunCooldown = GetPlayerBall()->GetPlayerBallData()->ImpactStunCooldown;

	// Bumped
	BumpedForceMultiplier = GetPlayerBall()->GetPlayerBallData()->BumpedForceMultiplier;
	BumpedHitLagCooldown = GetPlayerBall()->GetPlayerBallData()->BumpedHitLagStunCooldown;

	// Snapping
	SnapAngularForce = GetPlayerBall()->GetPlayerBallData()->SnapAngularForce;
	SnapControlMoveRollDivider = GetPlayerBall()->GetPlayerBallData()->SnapControlMoveRollDivider;
	MinVelocityToSnap = GetPlayerBall()->GetPlayerBallData()->MinVelocityToSnap;
}

void UPlayerBallBehaviorElementReactions::OnCollisionHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor == nullptr) return;

	TObjectPtr<APlayerBall> OtherBall = Cast<APlayerBall>(OtherActor);

	if (OtherBall != nullptr)
	{
		ImpactedPlayerBall = OtherBall;
		ReceiveImpactAction(1.f, Hit.ImpactNormal);
	}
	else
	{
		TObjectPtr<APinballElement> OtherElement = Cast<APinballElement>(OtherActor);

		if (OtherElement != nullptr)
		{
			switch (OtherElement->GetElementID())
			{
			case EPinballElementID::Bumper:
				OtherElement->TriggerElement();
				ReceiveBumperReaction(OtherElement, Hit.ImpactNormal);
				return;
			case EPinballElementID::Flipper:
				return;
			case EPinballElementID::None:
				return;

			default:
				return;
			}
		}
	}
}

void UPlayerBallBehaviorElementReactions::OnAttractionBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (SnappingPlayerBall == nullptr)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "OnAttractionBeginOverlap");

		
		TObjectPtr<APlayerBall> OtherBall = Cast<APlayerBall>(OtherActor);

		if (OtherBall != nullptr)
		{
			SnappingPlayerBall = OtherBall;
			ReceiveSnappingAction(1.f);

			return;
		}
	}
}

void UPlayerBallBehaviorElementReactions::OnAttractionEndOverlap(UPrimitiveComponent* OverlappedComponent,
                                                                 AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (SnappingPlayerBall == nullptr)	return;

	TObjectPtr<APlayerBall> OtherBall = Cast<APlayerBall>(OtherActor);

	if (OtherBall != nullptr)
	{
		if (OtherBall ==  SnappingPlayerBall)
		{
			ReceiveSnappingAction(0.f);
		}
	}
}

void UPlayerBallBehaviorElementReactions::ReceiveStunnedAction(float InStunnedDurationValue)
{
	OnStunnedAction.Broadcast(InStunnedDurationValue);
}

void UPlayerBallBehaviorElementReactions::ReceiveImpactAction(float ImpactValue, const FVector& InNormalImpact)
{
	NormalImpact = InNormalImpact;

	NormalImpact.Z = 0.f;

	NormalImpact.Normalize();
	
	OnImpactAction.Broadcast(ImpactValue);
}

void UPlayerBallBehaviorElementReactions::ReceiveSnappingAction(float SnappingValue)
{
	OnReceiveSnappingAction.Broadcast(SnappingValue);
}

void UPlayerBallBehaviorElementReactions::ReceiveBumperReaction(APinballElement* Element, const FVector& InNormalBump)
{
	HitPinballElement = Element;

	NormalBump = InNormalBump;

	NormalBump.Z = 0.f;
	NormalBump.Normalize();

	OnBumperReaction.Broadcast(1.f);
}

