// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BGPlayerController.generated.h"

class UBGChatInput;
class UUserWidget;

UCLASS()
class BASEBALLGAME_API ABGPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ABGPlayerController();
	
	virtual void BeginPlay() override;

	void SetChatMessageString(const FString& InChatMessageString);

	void PrintChatMessageString(const FString& InChatMessageString);

	UFUNCTION(Client, Reliable)
	void ClientRPCPrintChatMessageString(const FString& CombinedMessageString, const FString& InChatMessageString);

	UFUNCTION(Server, Reliable)
	void ServerRPCPrintChatMessageString(const FString& CombinedMessageString, const FString& InChatMessageString);

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UBGChatInput> ChatInputWidgetClass;

	UPROPERTY()
	TObjectPtr<UBGChatInput> ChatInputWidgetInstance;

	FString ChatMessageString;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> NotificationTextWidgetClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> NotificationTextWidgetInstance;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> CountDownWidgetClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> CountDownWidgetInstance;

public:
	UPROPERTY(Replicated, BlueprintReadOnly)
	FText NotificationText;

	UPROPERTY(Replicated, BlueprintReadOnly)
	FText CountDownText;
	
};
