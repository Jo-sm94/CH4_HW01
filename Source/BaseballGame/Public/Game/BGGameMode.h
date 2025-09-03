// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BGGameMode.generated.h"

class ABGPlayerController;
class Tamp;
class ABGPlayerState;

UCLASS()
class BASEBALLGAME_API ABGGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	//새로운 플레이어가 서버에 접속 후 로그인 절차가 끝난후 호출
	virtual void OnPostLogin(AController* NewPlayer) override;
	
	virtual void BeginPlay() override;
	
	//숫자 3자리 생성
	FString GenerateSecretNumber();

	//그냥 채팅을 이용했는지 아니면 3자리 정답을 작성했는지 판별
	bool IsGuessNumberString(const FString& InNumberString);

	//정답인지 아닌지 판별
	FString JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString);
	
	void ResetGame();

	void JudgeGame(ABGPlayerController* InChattingPlayerController, int InStrikeCount);

	void PrintChatMessageString(ABGPlayerController* InChattingPlayerController, const FString& CombinedMessageString, const FString& InChatMessageString);

	void IncreaseGuessCount(ABGPlayerController* InChattingPlayerController);

	//
	void CountDownManager();

	void CountDown();

	void EndTurn();

protected:
	FString SecretNumberString;

	TArray<TObjectPtr<ABGPlayerController>> AllPlayerControllers;

	FTimerHandle CountDownTimeHandle;

	int32 RemainingTime;
	int32 CurrentTurn;
};
