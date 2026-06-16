// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/GameStateCountlessBlood.h"

#include "componentes/ComponenteMisiones.h"
#include "Net/UnrealNetwork.h"


AGameStateCountlessBlood::AGameStateCountlessBlood()
{
	bPartidaFinalizada = false;
}

void AGameStateCountlessBlood::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGameStateCountlessBlood, bPartidaFinalizada);
	DOREPLIFETIME(AGameStateCountlessBlood, SharedQuestProgress);
}

void AGameStateCountlessBlood::AddSharedProgress(FName QuestID, ETiposDeObjetivo Type, int32 Amount)
{
	if (!HasAuthority()) return;

	bool bFound = false;
    
	for (FQuestProgress& Prog : SharedQuestProgress)
	{
		if (Prog.QuestID == QuestID)
		{
			Prog.Progress += Amount; 
			bFound = true;
			break;
		}
	}

	if (!bFound)
	{
		SharedQuestProgress.Add(FQuestProgress(QuestID, Amount));
	}
}

int32 AGameStateCountlessBlood::GetSharedProgress(FName QuestID) const
{
	for (const FQuestProgress& Prog : SharedQuestProgress)
	{
		if (Prog.QuestID == QuestID)
		{
			return Prog.Progress;
		}
	}
	return 0;
}
