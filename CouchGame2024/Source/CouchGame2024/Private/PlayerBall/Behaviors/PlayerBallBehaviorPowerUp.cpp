// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBall/Behaviors/PlayerBallBehaviorPowerUp.h"

#include "Components/SphereComponent.h"
#include "PlayerBall/PlayerBall.h"
#include "PlayerBall/PlayerBallController.h"
#include "PlayerBall/Datas/PlayerPowerUpData.h"
#include "PowerUp/PowerUp.h"


UPlayerBallBehaviorPowerUp::UPlayerBallBehaviorPowerUp()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UPlayerBallBehaviorPowerUp::BeginPlay()
{
	Super::BeginPlay();
}


void UPlayerBallBehaviorPowerUp::TickComponent(float DeltaTime, ELevelTick TickType,
                                               FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	HandleStrengthDuration(DeltaTime);
	HandleSlipperyEffectDuration(DeltaTime);
}

void UPlayerBallBehaviorPowerUp::InitBehavior()
{
	Super::InitBehavior();

	if (GetPlayerBall() != nullptr)
	{
		GetPlayerBall()->SphereCollision->OnComponentBeginOverlap.AddDynamic(
			this, &UPlayerBallBehaviorPowerUp::OnPlayerSphereBeginOverlap);
	}
}

void UPlayerBallBehaviorPowerUp::BindBehaviorEventAction(APlayerBallController* InPlayerBallController)
{
	Super::BindBehaviorEventAction(InPlayerBallController);

	if (GetPlayerBallController() == nullptr) return;

	GetPlayerBallController()->OnUsePowerUpInput.AddDynamic(this, &UPlayerBallBehaviorPowerUp::UsePowerUpAction);
}

void UPlayerBallBehaviorPowerUp::UnbindBehaviorEventAction(APlayerBallController* InPlayerBallController)
{
	Super::UnbindBehaviorEventAction(InPlayerBallController);

	if (GetPlayerBallController() == nullptr) return;

	GetPlayerBallController()->OnUsePowerUpInput.RemoveDynamic(this, &UPlayerBallBehaviorPowerUp::UsePowerUpAction);
}

void UPlayerBallBehaviorPowerUp::SetupData()
{
	Super::SetupData();

	if (GetPlayerBall() == nullptr) return;
	if (GetPlayerBall()->GetPlayerPowerUpData() == nullptr) return;

	FreezeRange = GetPlayerBall()->GetPlayerPowerUpData()->FreezeRange;
	FreezeDuration = GetPlayerBall()->GetPlayerPowerUpData()->FreezeDuration;

	StrengthImpactForceDivider = GetPlayerBall()->GetPlayerPowerUpData()->StrengthImpactForceDivider;
	StrengthImpactStunDurationDivider = GetPlayerBall()->GetPlayerPowerUpData()->StrengthImpactStunDurationDivider;

	SlipperyRadius = GetPlayerBall()->GetPlayerPowerUpData()->SlipperyRadius;
	TotalSlipperyEffectDuration = GetPlayerBall()->GetPlayerPowerUpData()->TotalSlipperyEffectDuration;
}

void UPlayerBallBehaviorPowerUp::OnPlayerSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent,
                                                            AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                                            int32 OtherBodyIndex, bool bFromSweep,
                                                            const FHitResult& SweepResult)
{
	TObjectPtr<APowerUp> OtherPowerUp = Cast<APowerUp>(OtherActor);

	if (OtherPowerUp != nullptr && (GetCurrentPowerUpCarried() == EPowerUpID::None || OtherPowerUp->GetPowerUpID() == EPowerUpID::Duck || OtherPowerUp->GetPowerUpID() == EPowerUpID::Collectible))
	{
		AssignPowerUpCarried(OtherPowerUp->GetPowerUpID());

		switch (OtherPowerUp->GetPowerUpID())
		{
		case EPowerUpID::Dash:
			OtherPowerUp->TriggerPowerUp();
			break;

		case EPowerUpID::Collectible:
			OtherPowerUp->TriggerPowerUp(GetPlayerBall()->PlayerIndex);
			break;

		case EPowerUpID::Duck:
			//if (GEngine) GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Red, "overlap duck");
			OtherPowerUp->TriggerPowerUp(GetPlayerBall()->PlayerIndex);
			break;

		case EPowerUpID::Mole:
			OtherPowerUp->TriggerPowerUp(GetPlayerBall()->PlayerIndex);
			break;

		case EPowerUpID::Freeze:
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "Collect Freeze power up");
			OtherPowerUp->TriggerPowerUp();
			break;

		case EPowerUpID::Strength:
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "Collect Strength power up");
			OtherPowerUp->TriggerPowerUp();
			break;

		case EPowerUpID::Slippery:
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "Collect Slippery power up");
			OtherPowerUp->TriggerPowerUp();
			break;

		default:
			break;
		}

		return;
	}
}


EPowerUpID UPlayerBallBehaviorPowerUp::GetCurrentPowerUpCarried() const
{
	return CurrentPowerUpCarried;
}

void UPlayerBallBehaviorPowerUp::AssignPowerUpCarried(EPowerUpID PowerUpID)
{
	if (CurrentPowerUpCarried != EPowerUpID::None || PowerUpID == EPowerUpID::Collectible || PowerUpID ==
		EPowerUpID::Duck || PowerUpID == EPowerUpID::Mole) return;

	CurrentPowerUpCarried = PowerUpID;
}

void UPlayerBallBehaviorPowerUp::EmptyCurrentPowerUpCarried()
{
	CurrentPowerUpCarried = EPowerUpID::None;
}

void UPlayerBallBehaviorPowerUp::UsePowerUpAction(float UsePowerUpValue)
{
	UsePowerUpCarried();
}

void UPlayerBallBehaviorPowerUp::UsePowerUpCarried()
{
	if (CurrentPowerUpCarried == EPowerUpID::None)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, "You have no power up");
		return;
	}

	switch (CurrentPowerUpCarried)
	{
	case EPowerUpID::Freeze: // substate id -> 1.f
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, "Use Freeze PowerUp");
		OnUsePowerUpAction.Broadcast(1.f);
		break;

	case EPowerUpID::Strength: // substate id -> 2.f
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, "Use Strength PowerUp");
		OnUsePowerUpAction.Broadcast(2.f);
		break;

	case EPowerUpID::Slippery: // substate id -> 3.f
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, "Use Slippery PowerUp");
		OnUsePowerUpAction.Broadcast(3.f);
		break;

	default:
		break;
	}
}

void UPlayerBallBehaviorPowerUp::ActivateStrengthPowerUp()
{
	bUsingStrength = true;

	CurrentStrengthDuration = 0.f;
}

void UPlayerBallBehaviorPowerUp::DesactivateStrengthPowerUp()
{
	bUsingStrength = false;

	if (GetPlayerBall() != nullptr)
	{
		GetPlayerBall()->PlayStrengthDisapearGamefeelEffectsBlueprint();
	}
}

void UPlayerBallBehaviorPowerUp::HandleStrengthDuration(float DeltaTime)
{
	if (!bUsingStrength)
		return;

	if (CurrentStrengthDuration >= TotalStrengthDuration)
	{
		DesactivateStrengthPowerUp();
	}
	else
	{
		CurrentStrengthDuration += DeltaTime;
	}
}

bool UPlayerBallBehaviorPowerUp::GetIsUsingStrengthPowerUp()
{
	return bUsingStrength;
}

void UPlayerBallBehaviorPowerUp::ReceiveSlipperyEffect()
{
	if (GetPlayerBall() == nullptr) return;
	if (GetPlayerBall()->GetPlayerPowerUpData() == nullptr) return;

	bIsSlippery = true;

	CurrentSlipperyEffectDuration = 0.f;

	if (GetPlayerBall()->SphereCollision == nullptr) return;
	if (GetPlayerBall()->GetPlayerPowerUpData()->SlipperyPhysicsAsset == nullptr) return;

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, "Set slippery Material");
	GetPlayerBall()->SphereCollision->SetPhysMaterialOverride(
		GetPlayerBall()->GetPlayerPowerUpData()->SlipperyPhysicsAsset);

	GetPlayerBall()->PlaySlipperyEffectGamefeelEffectsBlueprint();
}

void UPlayerBallBehaviorPowerUp::EndSlipperyEffect()
{
	bIsSlippery = false;

	if (GetPlayerBall()->SphereCollision == nullptr) return;
	if (GetPlayerBall()->GetPlayerPowerUpData()->ClassicPhysicsAsset == nullptr) return;

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, "Set classic Material");
	GetPlayerBall()->SphereCollision->SetPhysMaterialOverride(
		GetPlayerBall()->GetPlayerPowerUpData()->ClassicPhysicsAsset);

	GetPlayerBall()->PlaySlipperySlipperyDisapearGamefeelEffectsBlueprint();
}

void UPlayerBallBehaviorPowerUp::HandleSlipperyEffectDuration(float DeltaTime)
{
	if (!bIsSlippery) return;

	if (CurrentSlipperyEffectDuration >= TotalSlipperyEffectDuration)
	{
		EndSlipperyEffect();
	}
	else
	{
		CurrentSlipperyEffectDuration += DeltaTime;
	}
}
