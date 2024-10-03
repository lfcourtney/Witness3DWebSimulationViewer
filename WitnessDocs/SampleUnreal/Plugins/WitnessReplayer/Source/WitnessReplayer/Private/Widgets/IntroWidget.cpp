// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/IntroWidget.h"

#include "Kismet/GameplayStatics.h"

void UIntroWidget::SaveShowIntroSaveGame()
{
	ShowIntroSaveGame->bShowIntro = !ShowIntroSaveGame->bShowIntro;
	CheckImage->SetOpacity(ShowIntroSaveGame->bShowIntro ? 1 : 0);
	UGameplayStatics::SaveGameToSlot(ShowIntroSaveGame, SaveSlotName, 0);
}

void UIntroWidget::StartReplayer()
{
	OnStart.Broadcast();
	RemoveFromParent();
}

void UIntroWidget::NativeOnInitialized()
{
	USaveGame* SaveGame = UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0);
	SaveGame = SaveGame ? SaveGame : UGameplayStatics::CreateSaveGameObject(UShowIntroSaveGame::StaticClass());
	ShowIntroSaveGame = Cast<UShowIntroSaveGame>(SaveGame);

	CheckBoxButton->OnClicked.AddDynamic(this, &UIntroWidget::SaveShowIntroSaveGame);
	StartReplayerButton->OnClicked.AddDynamic(this, &UIntroWidget::StartReplayer);
	
	Super::NativeOnInitialized();
	
	if(!ShowIntroSaveGame->bShowIntro)
	{
		StartReplayer();
	}
}
