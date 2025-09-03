// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/BGPawn.h"
#include "BaseballGame/BaseballGame.h"


void ABGPawn::BeginPlay()
{
	Super::BeginPlay();
	/*
	FString NetRoleString = FunctionLibrary::GetRoleString(this);
	FString CombinedString = FString::Printf(TEXT("Pawn::BeginPlay() %s [%s]"), *FunctionLibrary::GetNetModeString(this), *NetRoleString);
	FunctionLibrary::MyPrintString(this, CombinedString, 10.f);
	*/
}

void ABGPawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	/*
	FString NetRoleString = FunctionLibrary::GetRoleString(this);
	FString CombinedString = FString::Printf(TEXT("Pawn::PossessedBy() %s [%s]"), *FunctionLibrary::GetNetModeString(this), *NetRoleString);
	FunctionLibrary::MyPrintString(this, CombinedString, 10.f);
	*/

}