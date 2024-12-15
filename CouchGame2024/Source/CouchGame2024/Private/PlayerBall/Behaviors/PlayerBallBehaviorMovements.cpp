// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBall/Behaviors/PlayerBallBehaviorMovements.h"

#include "Components/SphereComponent.h"
#include "PlayerBall/PlayerBall.h"
#include "PlayerBall/PlayerBallController.h"
#include "PlayerBall/Behaviors/PlayerBallBehaviorElementReactions.h"
#include "PlayerBall/Datas/PlayerBallData.h"


UPlayerBallBehaviorMovements::UPlayerBallBehaviorMovements()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UPlayerBallBehaviorMovements::BeginPlay()
{
	Super::BeginPlay();

	
}


void UPlayerBallBehaviorMovements::TickComponent(float DeltaTime, ELevelTick TickType,
                                                 FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	HandleRollBoostDuration(DeltaTime);
}

void UPlayerBallBehaviorMovements::InitBehavior()
{
	Super::InitBehavior();

	if (GetPlayerBall() != nullptr)
	{
		if (GetPlayerBall()->BehaviorElementReactions != nullptr)
		{
			GetPlayerBall()->BehaviorElementReactions->OnBoostPadReaction.AddDynamic(this, &UPlayerBallBehaviorMovements::ReceiveRollBoost);
		}
	}
}

void UPlayerBallBehaviorMovements::BindBehaviorEventAction(APlayerBallController* InPlayerBallController)
{
	Super::BindBehaviorEventAction(InPlayerBallController);

	if (GetPlayerBallController() == nullptr)	return;

	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, "Bind");
	GetPlayerBallController()->OnPlayerMoveXInput.AddDynamic(this, &UPlayerBallBehaviorMovements::MoveXAction);
	GetPlayerBallController()->OnPlayerMoveYInput.AddDynamic(this, &UPlayerBallBehaviorMovements::MoveYAction);
}

void UPlayerBallBehaviorMovements::UnbindBehaviorEventAction(APlayerBallController* InPlayerBallController)
{
	Super::UnbindBehaviorEventAction(InPlayerBallController);

	if (GetPlayerBallController() == nullptr)	return;
	
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, "UnBind");
	GetPlayerBallController()->OnPlayerMoveXInput.RemoveDynamic(this, &UPlayerBallBehaviorMovements::MoveXAction);
	GetPlayerBallController()->OnPlayerMoveYInput.RemoveDynamic(this, &UPlayerBallBehaviorMovements::MoveYAction);
}

void UPlayerBallBehaviorMovements::SetupData()
{
	Super::SetupData();

	if (GetPlayerBall() == nullptr)	return;
	if (GetPlayerBall()->GetPlayerBallData() == nullptr)	return;

	
	AngularRollForce = GetPlayerBall()->GetPlayerBallData()->AngularRollForce;
	BraqueDirectionForceMultiplier = GetPlayerBall()->GetPlayerBallData()->BraqueDirectionForceMultiplier;

	NewBraqueDirectionImpulseForce = GetPlayerBall()->GetPlayerBallData()->NewBraqueDirectionImpulseForce;
	
	RollBoostForce = GetPlayerBall()->GetPlayerBallData()->RollBoostForce;
	TotalRollBoostDuration = GetPlayerBall()->GetPlayerBallData()->TotalRollBoostDuration;

	
	GetPlayerBall()->SphereCollision->SetAngularDamping(GetPlayerBall()->GetPlayerBallData()->AngularRollDamping);
	GetPlayerBall()->SphereCollision->SetPhysicsMaxAngularVelocityInDegrees(GetPlayerBall()->GetPlayerBallData()->MaxAngularRollVelocity);
}

void UPlayerBallBehaviorMovements::MoveXAction(float XValue)
{
	MoveXValue = XValue;
}

void UPlayerBallBehaviorMovements::MoveYAction(float YValue)
{
	MoveYValue = YValue;
}

void UPlayerBallBehaviorMovements::ReceiveRollBoost(float InBoostValue)
{
	CurrentRollDuration = 0.f;
	bUseBoostRollForce = true;
}

void UPlayerBallBehaviorMovements::StopRollBoost()
{
	bUseBoostRollForce = false;
}


void UPlayerBallBehaviorMovements::HandleRollBoostDuration(float DeltaTime)
{
	if (!bUseBoostRollForce)	return;

	if (CurrentRollDuration >= TotalRollBoostDuration)
	{
		StopRollBoost();
	}
	else
	{
		CurrentRollDuration += DeltaTime;
	}
}

bool UPlayerBallBehaviorMovements::IsGrounded()
{
	if (GetPlayerBall() == nullptr)	return false;
	
	// Line trace Pos
	FVector Start = GetPlayerBall()->GetActorLocation();
	FVector End = Start + FVector(0, 0, -50.f);

	// LineTrace Parameters
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetPlayerBall());

	// Line Trace Hit
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECC_WorldStatic,
		QueryParams
	);

	//DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1.0f, 0, 1.0f);

	return bHit;
}

float UPlayerBallBehaviorMovements::GetContextRollForce()
{
	if (bUseBoostRollForce)
	{
		return RollBoostForce;
	}

	return AngularRollForce;
}

