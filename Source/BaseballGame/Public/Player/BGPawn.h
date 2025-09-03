// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BGPawn.generated.h"

UCLASS()
class BASEBALLGAME_API ABGPawn : public APawn
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;

};
