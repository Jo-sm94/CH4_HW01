// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BGChatInput.generated.h"

class UEditableTextBox;

UCLASS()
class BASEBALLGAME_API UBGChatInput : public UUserWidget
{
	GENERATED_BODY()

public:
	//AddToViewport시 호출(viewport에 add될때마다 호출)
	virtual void NativeConstruct() override;
	//RemoveFromParent시 호출(viewport에 remove될때마다 호출)
	virtual void NativeDestruct() override;

protected:
	UFUNCTION()
	void OnChatInputTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableTextBox> EditableTextBox_ChatInput;
	
};
