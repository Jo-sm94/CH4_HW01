// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "BGPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class BASEBALLGAME_API ABGPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	ABGPlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	FString GetPlayerInfoString();

public:
	UPROPERTY(Replicated)
	FString PlayerNameString;

	UPROPERTY(Replicated, BlueprintReadOnly)
	int32 CurrentGuessCount;

	UPROPERTY(Replicated, BlueprintReadOnly)
	int32 MaxGuessCount;

};
