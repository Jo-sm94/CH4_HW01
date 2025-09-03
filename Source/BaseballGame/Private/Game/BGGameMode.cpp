// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/BGGameMode.h"
#include "Game/BGGameState.h"
#include "Player/BGPlayerController.h"
#include "Player/BGPlayerState.h"
#include "EngineUtils.h"

void ABGGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	UE_LOG(LogTemp, Warning, TEXT("PostLogin"));

	ABGPlayerController* BGPlayerController = Cast<ABGPlayerController>(NewPlayer);
	if (IsValid(BGPlayerController) == true)
	{
		BGPlayerController->NotificationText = FText::FromString(TEXT("Connected to the game server."));
		
		AllPlayerControllers.Add(BGPlayerController);

		ABGPlayerState* BGPS = BGPlayerController->GetPlayerState<ABGPlayerState>();
		if (IsValid(BGPS) == true)
		{
			BGPS->PlayerNameString = TEXT("Player") + FString::FromInt(AllPlayerControllers.Num());
		}

		ABGGameState* BGGameState = GetGameState<ABGGameState>();
		if (IsValid(BGGameState) == true)
		{
			BGGameState->MulticastRPCBroadcastLoginMessage(BGPS->PlayerNameString);
		}
	}

	//최초 1회만 실행
	if (AllPlayerControllers[0] == BGPlayerController)
	{
		CountDownManager();
	}

}

void ABGGameMode::BeginPlay()
{
	Super::BeginPlay();

	SecretNumberString = GenerateSecretNumber();
	
	CurrentTurn = 0;
	
}

FString ABGGameMode::GenerateSecretNumber()
{
	TArray<int32> Numbers;
	for (int32 i = 1; i <= 9; ++i)
	{
		Numbers.Add(i);
	}

	FMath::RandInit(FDateTime::Now().GetTicks());
	Numbers = Numbers.FilterByPredicate([](int32 Num) { return Num > 0; });

	FString Result;
	for (int32 i = 0; i < 3; ++i)
	{
		int32 Index = FMath::RandRange(0, Numbers.Num() - 1);
		Result.Append(FString::FromInt(Numbers[Index]));
		Numbers.RemoveAt(Index);
	}

	UE_LOG(LogTemp, Error, TEXT("Generate Number : %s"), *Result);

	return Result;
}

bool ABGGameMode::IsGuessNumberString(const FString& InNumberString)
{
	bool bCanPlay = false;

	do {
		
		if (InNumberString.Len() != 3)
		{
			break;
		}

		bool bIsUnique = true;
		TSet<TCHAR> UniqueDigits;
		for (TCHAR C : InNumberString)
		{
			if (FChar::IsDigit(C) == false || C == '0')
			{
				bIsUnique = false;
				break;
			}

			UniqueDigits.Add(C);
		}

		if (bIsUnique == false)
		{
			break;
		}

		bCanPlay = true;

	} while (false);

	return bCanPlay;
}

FString ABGGameMode::JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString)
{
	int32 StrikeCount = 0, BallCount = 0;

	for (int32 i = 0; i < 3; ++i)
	{
		if (InSecretNumberString[i] == InGuessNumberString[i])
		{
			StrikeCount++;
		}
		else
		{
			FString PlayerGuessChar = FString::Printf(TEXT("%c"), InGuessNumberString[i]);
			if (InSecretNumberString.Contains(PlayerGuessChar))
			{
				BallCount++;
			}
		}
	}

	if (StrikeCount == 0 && BallCount == 0)
	{
		return TEXT("OUT");
	}

	return FString::Printf(TEXT("%dS%dB"), StrikeCount, BallCount);
}

void ABGGameMode::ResetGame()
{
	for (const auto& BGPlayerController : AllPlayerControllers)
	{
		ABGPlayerState* BGPS = BGPlayerController->GetPlayerState<ABGPlayerState>();
		if (IsValid(BGPS) == true)
		{
			BGPS->CurrentGuessCount = 0;
			BGPlayerController->CountDownText = FText::FromString(TEXT("0"));
		}
	}

	CountDownManager();
}

void ABGGameMode::JudgeGame(ABGPlayerController* InChattingPlayerController, int InStrikeCount)
{
	if (3 == InStrikeCount)
	{
		GetWorld()->GetTimerManager().ClearTimer(CountDownTimeHandle);
		ABGPlayerState* BGPS = InChattingPlayerController->GetPlayerState<ABGPlayerState>();
		for (const auto& BGPlayerController : AllPlayerControllers)
		{
			if (IsValid(BGPS) == true)
			{
				FString CombinedMessageString = BGPS->PlayerNameString + TEXT(" has won the game.");
				BGPlayerController->NotificationText = FText::FromString(CombinedMessageString);

				ResetGame();
			}
		}
		SecretNumberString = GenerateSecretNumber();
	}
	else
	{
		bool bIsDraw = true;
		for (const auto& CXPlayerController : AllPlayerControllers)
		{
			ABGPlayerState* BGPS = CXPlayerController->GetPlayerState<ABGPlayerState>();
			if (IsValid(BGPS) == true)
			{
				if (BGPS->CurrentGuessCount < BGPS->MaxGuessCount)
				{
					bIsDraw = false;
					break;
				}
			}
		}
		if (true == bIsDraw)
		{
			GetWorld()->GetTimerManager().ClearTimer(CountDownTimeHandle);
			for (const auto& BGPlayerController : AllPlayerControllers)
			{
				BGPlayerController->NotificationText = FText::FromString(TEXT("Draw..."));
			}
			ResetGame();
			SecretNumberString = GenerateSecretNumber();
		}
	}
}

void ABGGameMode::PrintChatMessageString(ABGPlayerController* InChattingPlayerController, const FString& CombinedMessageString, const FString& InChatMessageString)
{
	ABGPlayerState* BGPS = InChattingPlayerController->GetPlayerState<ABGPlayerState>();
	
	if (IsGuessNumberString(InChatMessageString) == true && BGPS->CurrentGuessCount < 3 && AllPlayerControllers[CurrentTurn] == InChattingPlayerController)
	{
		FString JudgeResultString = JudgeResult(SecretNumberString, InChatMessageString);
		//IncreaseGuessCount(InChattingPlayerController);
		EndTurn();
		InChattingPlayerController->CountDownText = FText::FromString(TEXT("0"));

		for (TActorIterator<ABGPlayerController> It(GetWorld()); It; ++It)
		{
			ABGPlayerController* BGPlayerController = *It;
			if (IsValid(BGPlayerController) == true)
			{
				FString JudgeMessageString = CombinedMessageString + TEXT(" -> ") + JudgeResultString;
				BGPlayerController->ClientRPCPrintChatMessageString(JudgeMessageString, InChatMessageString);

				int32 StrikeCount = FCString::Atoi(*JudgeResultString.Left(1));
				JudgeGame(InChattingPlayerController, StrikeCount);
			}
		}
	}
	else
	{
		for (TActorIterator<ABGPlayerController> It(GetWorld()); It; ++It)
		{
			ABGPlayerController* BGPlayerController = *It;
			if (IsValid(BGPlayerController) == true)
			{
				BGPlayerController->ClientRPCPrintChatMessageString(CombinedMessageString, InChatMessageString);
			}
		}
	}
}

void ABGGameMode::IncreaseGuessCount(ABGPlayerController* InChattingPlayerController)
{
	ABGPlayerState* BGPS = InChattingPlayerController->GetPlayerState<ABGPlayerState>();
	if (IsValid(BGPS) == true)
	{
		BGPS->CurrentGuessCount++;
	}
}

void ABGGameMode::CountDownManager()
{

	RemainingTime = 10;

	GetWorld()->GetTimerManager().SetTimer(
		CountDownTimeHandle,
		this,
		& ABGGameMode::CountDown,
		1.0f,
		true
	);

	ABGPlayerController* BGPC = AllPlayerControllers[CurrentTurn];
	ABGPlayerState* BGPS = BGPC->GetPlayerState<ABGPlayerState>();
	UE_LOG(LogTemp, Warning, TEXT("Turn Start: %s"), *BGPS->GetPlayerInfoString());
}

void ABGGameMode::CountDown()
{
	--RemainingTime;
	ABGPlayerController* BGPC = AllPlayerControllers[CurrentTurn];
	BGPC->CountDownText = FText::FromString(FString::FromInt(RemainingTime));

	if (RemainingTime <= 0)
	{
		EndTurn();
	}
}

void ABGGameMode::EndTurn()
{
	IncreaseGuessCount(AllPlayerControllers[CurrentTurn]);

	GetWorld()->GetTimerManager().ClearTimer(CountDownTimeHandle);

	CurrentTurn = (CurrentTurn + 1) % AllPlayerControllers.Num();

	CountDownManager();
}

