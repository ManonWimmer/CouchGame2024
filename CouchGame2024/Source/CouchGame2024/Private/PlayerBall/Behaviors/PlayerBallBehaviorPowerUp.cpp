// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBall/Behaviors/PlayerBallBehaviorPowerUp.h"

#include "Components/SphereComponent.h"
#include "PlayerBall/PlayerBall.h"
#include "PlayerBall/PlayerBallController.h"
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

	
}

void UPlayerBallBehaviorPowerUp::InitBehavior()
{
	Super::InitBehavior();

	if (GetPlayerBall() != nullptr)
	{
		GetPlayerBall()->SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &UPlayerBallBehaviorPowerUp::OnPlayerSphereBeginOverlap);
	}
}

void UPlayerBallBehaviorPowerUp::BindBehaviorEventAction(APlayerBallController* InPlayerBallController)
{
	Super::BindBehaviorEventAction(InPlayerBallController);

	if (GetPlayerBallController() == nullptr)	return;
	
	GetPlayerBallController()->OnUsePowerUpInput.AddDynamic(this, &UPlayerBallBehaviorPowerUp::UsePowerUpAction);
}

void UPlayerBallBehaviorPowerUp::OnPlayerSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	TObjectPtr<APowerUp> OtherPowerUp = Cast<APowerUp>(OtherActor);

	if (OtherPowerUp != nullptr && GetCurrentPowerUpCarried() == EPowerUpID::None)
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
				
		case EPowerUpID::Freeze:
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "Collect Freeze power up");
			OtherPowerUp->TriggerPowerUp();
			break;
				
		case EPowerUpID::Strength:
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "Collect Strength power up");
			OtherPowerUp->TriggerPowerUp();
			break;
				
		case EPowerUpID::Heavy:
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "Collect Heavy power up");
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
	if (CurrentPowerUpCarried != EPowerUpID::None || PowerUpID == EPowerUpID::Collectible)	return;

	CurrentPowerUpCarried = PowerUpID;
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
	case EPowerUpID::Freeze:
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, "Use Freeze PowerUp");
		break;
			
	case EPowerUpID::Strength:
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, "Use Strength PowerUp");
		break;
			
	case EPowerUpID::Heavy:
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, "Use Heavy PowerUp");
		break;
			
	default:
		break;
	}

	CurrentPowerUpCarried = EPowerUpID::None;
}

