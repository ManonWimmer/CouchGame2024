﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBall/Behaviors/PlayerBallBehaviorElementReactions.h"

#include "Components/SphereComponent.h"
#include "PinballElements/PinballElement.h"
#include "PinballElements/Elements/RailElement.h"
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
			GetPlayerBall()->SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &UPlayerBallBehaviorElementReactions::OnCollisionBeginOverlap);
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


	// Punch stun
	PunchStunCooldown = GetPlayerBall()->GetPlayerBallData()->PunchStunCooldown;

	
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
			case EPinballElementID::Mole:
				OtherElement->TriggerElementWithPlayer(GetPlayerBall());
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

void UPlayerBallBehaviorElementReactions::OnCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (OtherActor == nullptr) return;

	TObjectPtr<APinballElement> OtherElement = Cast<APinballElement>(OtherActor);

	if (OtherElement != nullptr)
	{
		switch (OtherElement->GetElementID())
		{
			case EPinballElementID::DeathZone:
				// todo: check add score on death OtherElement - event push in player
				ReceiveDeathReaction();
				OtherElement->TriggerElement();
				break;
		case EPinballElementID::Rail:
				if (OtherActor->ActorHasTag(TEXT("RespawnRail")))
					break;
				if (OtherComp->ComponentHasTag(TEXT("DirectionInverse")))	// To define in which direction the ball will follow rail
				{
					ReceiveRailReaction(OtherElement, -1.f);
				}
				else
				{
					ReceiveRailReaction(OtherElement, 1.f);
				}
				break;
			case EPinballElementID::BoostPad:
				ReceiveBoostPadReaction();
				OtherElement->TriggerElement();
				break;
			case EPinballElementID::Tourniquet:
				OtherElement->TriggerElementWithPlayer(GetPlayerBall());
				break;
			default:
				break;
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

void UPlayerBallBehaviorElementReactions::ReceiveDeathReaction()
{
	if (GetPlayerBall() == nullptr) return;

	GetPlayerBall()->Kill();
}

void UPlayerBallBehaviorElementReactions::ReceiveRailReaction(APinballElement* PinballRailElement, float DirectionValue)
{
	if (PinballRailElement == nullptr) return;
	
	ARailElement* RailElement = Cast<ARailElement>(PinballRailElement);

	if (RailElement == nullptr)	return;
	
	if (!CheckRightDirectionForRail(RailElement, DirectionValue))
	{
		return;
	}
	
	CurrentRailElement = RailElement;

	if (CurrentRailElement == nullptr) return;

	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("Direction: %f"), DirectionValue));
	
	OnRailReaction.Broadcast(DirectionValue);
}

bool UPlayerBallBehaviorElementReactions::CheckRightDirectionForRail(ARailElement* InRailElement,
	float InEntryDirection)
{

	if (InRailElement == nullptr) return false;

	if (GetPlayerBall() == nullptr) return false;
	
	FVector Tangent = InRailElement->GetActorForwardVector();
	FVector EntryToPlayer = GetPlayerBall()->GetActorLocation();

	
	if (InEntryDirection == 1.f)
	{
		Tangent = InRailElement->GetTangentAtSplinePercent(0.95f);
		EntryToPlayer = GetPlayerBall()->GetActorLocation() - InRailElement->GetLocationAlongRailSpline(0.99f);
		//DrawDebugLine(GetWorld(), InRailElement->GetLocationAlongRailSpline(0.99f), InRailElement->GetLocationAlongRailSpline(0.99f) + (Tangent * 500.f), FColor::Red, false, 5.f);
		//DrawDebugLine(GetWorld(), InRailElement->GetLocationAlongRailSpline(0.99f), InRailElement->GetLocationAlongRailSpline(0.99f) + (EntryToPlayer * 500.f), FColor::Blue, false, 5.f);
	}
	else if (InEntryDirection == -1.f)
	{
		Tangent = InRailElement->GetTangentAtSplinePercent(0.01f);
		Tangent *= -1.f;
		EntryToPlayer = GetPlayerBall()->GetActorLocation() - InRailElement->GetLocationAlongRailSpline(0.01f);
		//DrawDebugLine(GetWorld(), InRailElement->GetLocationAlongRailSpline(0.01f), InRailElement->GetLocationAlongRailSpline(0.01f) + (Tangent * 500.f), FColor::Red, false, 5.f);
		//DrawDebugLine(GetWorld(), InRailElement->GetLocationAlongRailSpline(0.01f), InRailElement->GetLocationAlongRailSpline(0.01f) + (EntryToPlayer * 500.f), FColor::Blue, false, 5.f);
	}

	FVector2D Tangent2D = FVector2D(Tangent.X, Tangent.Y);
	FVector2D EntryToPlayer2D = FVector2D(EntryToPlayer.X, EntryToPlayer.Y);

	Tangent2D.Normalize();
	EntryToPlayer2D.Normalize();
	
	float DotProductEntryToPlayerAndTangent = FVector2D::DotProduct(EntryToPlayer2D, Tangent2D);

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("Dot product result : %f"), DotProductEntryToPlayerAndTangent));
	
	if (DotProductEntryToPlayerAndTangent < 0.f)	// Tangent and vector playerToEntry not same direction -> can't enter
	{
		return false;
	}
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "right direction");
	return true;
}

void UPlayerBallBehaviorElementReactions::ReceiveBoostPadReaction()
{
	OnBoostPadReaction.Broadcast(1.f);
}

void UPlayerBallBehaviorElementReactions::ReceiveTourniquetReaction()
{
	OnTourniquetReaction.Broadcast(1.f);
}

void UPlayerBallBehaviorElementReactions::ReceiveEndTourniquetReaction()
{
	OnEndTourniquetReaction.Broadcast(1.f);
}

