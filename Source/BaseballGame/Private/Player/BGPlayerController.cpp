// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/BGPlayerController.h"
#include "Player/BGPlayerState.h"
#include "Game/BGGameMode.h"
#include "UI/BGChatInput.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "BaseballGame/BaseballGame.h"
#include "Net/UnrealNetwork.h"


ABGPlayerController::ABGPlayerController()
{
	bReplicates = true;
}

void ABGPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController() == false)
	{
		return;
	}

	FInputModeUIOnly InputModeUIOnly;
	SetInputMode(InputModeUIOnly);

	if (IsValid(ChatInputWidgetClass) == true)
	{
		ChatInputWidgetInstance = CreateWidget<UBGChatInput>(this, ChatInputWidgetClass);
		if (IsValid(ChatInputWidgetInstance) == true)
		{
			ChatInputWidgetInstance->AddToViewport();
		}
	}

	if (IsValid(NotificationTextWidgetClass) == true)
	{
		NotificationTextWidgetInstance = CreateWidget<UUserWidget>(this, NotificationTextWidgetClass);
		if (IsValid(NotificationTextWidgetInstance) == true)
		{
			NotificationTextWidgetInstance->AddToViewport();
		}
	}

	if (IsValid(CountDownWidgetClass) == true)
	{
		CountDownWidgetInstance = CreateWidget<UUserWidget>(this, CountDownWidgetClass);
		if (IsValid(CountDownWidgetInstance) == true)
		{
			CountDownWidgetInstance->AddToViewport();
		}
	}
}

void ABGPlayerController::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, NotificationText);
	DOREPLIFETIME(ThisClass, CountDownText);
}

void ABGPlayerController::SetChatMessageString(const FString& InChatMessageString)
{
	ChatMessageString = InChatMessageString;

	if (IsLocalController() == true)
	{
		ABGPlayerState* BGPS = GetPlayerState<ABGPlayerState>();
		if (IsValid(BGPS) == true)
		{
			FString CombinedMessageString = BGPS->GetPlayerInfoString() + TEXT(": ") + InChatMessageString;

			ServerRPCPrintChatMessageString(CombinedMessageString, InChatMessageString);
		}
	}
}

void ABGPlayerController::PrintChatMessageString(const FString& InChatMessageString)
{

	FunctionLibrary::MyPrintString(this, InChatMessageString, 10.f);
}

void ABGPlayerController::ServerRPCPrintChatMessageString_Implementation(const FString& CombinedMessageString, const FString& InChatMessageString)
{
	AGameModeBase* GM = UGameplayStatics::GetGameMode(this);
	if (IsValid(GM) == true)
	{
		ABGGameMode* BGGM = Cast<ABGGameMode>(GM);
		if (IsValid(BGGM) == true)
		{
			BGGM->PrintChatMessageString(this, CombinedMessageString, InChatMessageString);
		}
	}
}

void ABGPlayerController::ClientRPCPrintChatMessageString_Implementation(const FString& CombinedMessageString, const FString& InChatMessageString)
{
	PrintChatMessageString(CombinedMessageString);
}
