// Fill out your copyright notice in the Description page of Project Settings.


#include "Skins/SkinsSubsystem.h"

#include "Skins/SkinsData.h"
#include "Skins/SkinsSettings.h"

void USkinsSubsystem::InitSkinsSubsystem()
{
	if (bIsInitialized)	return;
	
	const USkinsSettings* SkinsSettings = GetDefault<USkinsSettings>();

	if (SkinsSettings == nullptr)	return;
	
	TObjectPtr<USkinsData> OutSkinsDataP1 = SkinsSettings->SkinsDataP1.LoadSynchronous();
	if (OutSkinsDataP1 != nullptr)
	{
		SkinsDataP1 = OutSkinsDataP1;
	}

	TObjectPtr<USkinsData> OutSkinsDataP2 = SkinsSettings->SkinsDataP2.LoadSynchronous();
	if (OutSkinsDataP2 != nullptr)
	{
		SkinsDataP2 = OutSkinsDataP2;
	}

	TObjectPtr<USkinsData> OutSkinsDataP3 = SkinsSettings->SkinsDataP3.LoadSynchronous();
	if (OutSkinsDataP3 != nullptr)
	{
		SkinsDataP3 = OutSkinsDataP3;
	}

	TObjectPtr<USkinsData> OutSkinsDataP4 = SkinsSettings->SkinsDataP4.LoadSynchronous();
	if (OutSkinsDataP4 != nullptr)
	{
		SkinsDataP4 = OutSkinsDataP4;
	}

	bIsInitialized = true;
}

UMaterialInstance* USkinsSubsystem::GetSkinFromIndexAndPlayerIndex(int InPlayerIndex, int InSkinIndex)
{
	switch (InPlayerIndex)
	{
	case 0:
		return GetSkinPlayer1FromIndex(InSkinIndex);
		break;
	case 1:
		return GetSkinPlayer2FromIndex(InSkinIndex);
		break;
	case 2:
		return GetSkinPlayer3FromIndex(InSkinIndex);
		break;
	case 3:
		return GetSkinPlayer4FromIndex(InSkinIndex);
		break;
	default:
		break;
	}
	return nullptr;
}

UMaterialInstance* USkinsSubsystem::GetCurrentSkinFromPlayerIndex(int InPlayerIndex)
{
	switch (InPlayerIndex)
	{
	case 0:
		return GetSkinPlayer1FromIndex(P1SkinIndex);
		break;
	case 1:
		return GetSkinPlayer2FromIndex(P2SkinIndex);
		break;
	case 2:
		return GetSkinPlayer3FromIndex(P3SkinIndex);
		break;
	case 3:
		return GetSkinPlayer4FromIndex(P4SkinIndex);
		break;
	default:
		break;
	}
	return nullptr;
}

UMaterialInstance* USkinsSubsystem::GetNextSkinFromPlayerIndex(int InPlayerIndex)
{
	switch (InPlayerIndex)
	{
		case 0:
			return GetSkinPlayer1FromIndex(P1SkinIndex + 1);
			break;
		case 1:
			return GetSkinPlayer2FromIndex(P2SkinIndex + 1);
			break;
		case 2:
			return GetSkinPlayer3FromIndex(P3SkinIndex + 1);
			break;
		case 3:
			return GetSkinPlayer4FromIndex(P4SkinIndex + 1);
			break;
		default:
			break;
	}
	return nullptr;
}

UMaterialInstance* USkinsSubsystem::GetSkinPlayer1FromIndex(int InSkinIndex)
{
	if (SkinsDataP1 == nullptr)	return nullptr;
	if (SkinsDataP1->SkinsMaterials.Num() > 0)
	{
		InSkinIndex = InSkinIndex % SkinsDataP1->SkinsMaterials.Num();
	}
	P1SkinIndex = InSkinIndex;
	
	if (P1SkinIndex >= SkinsDataP1->SkinsMaterials.Num())	return nullptr;

	return  SkinsDataP1->SkinsMaterials[P1SkinIndex];
}

UMaterialInstance* USkinsSubsystem::GetSkinPlayer2FromIndex(int InSkinIndex)
{
	if (SkinsDataP2 == nullptr)	return nullptr;
	if (SkinsDataP2->SkinsMaterials.Num() > 0)
	{
		InSkinIndex = InSkinIndex % SkinsDataP2->SkinsMaterials.Num();
	}
	P2SkinIndex = InSkinIndex;
	
	if (P2SkinIndex >= SkinsDataP2->SkinsMaterials.Num())	return nullptr;

	return  SkinsDataP2->SkinsMaterials[P2SkinIndex];
}

UMaterialInstance* USkinsSubsystem::GetSkinPlayer3FromIndex(int InSkinIndex)
{
	if (SkinsDataP3 == nullptr)	return nullptr;
	if (SkinsDataP3->SkinsMaterials.Num() > 0)
	{
		InSkinIndex = InSkinIndex % SkinsDataP3->SkinsMaterials.Num();
	}
	P3SkinIndex = InSkinIndex;
	
	if (P3SkinIndex >= SkinsDataP3->SkinsMaterials.Num())	return nullptr;

	return  SkinsDataP3->SkinsMaterials[P3SkinIndex];
}

UMaterialInstance* USkinsSubsystem::GetSkinPlayer4FromIndex(int InSkinIndex)
{
	if (SkinsDataP4 == nullptr)	return nullptr;
	if (SkinsDataP4->SkinsMaterials.Num() > 0)
	{
		InSkinIndex = InSkinIndex % SkinsDataP4->SkinsMaterials.Num();
	}
	P4SkinIndex = InSkinIndex;
	
	if (P4SkinIndex >= SkinsDataP4->SkinsMaterials.Num())	return nullptr;

	return  SkinsDataP4->SkinsMaterials[P4SkinIndex];
}
