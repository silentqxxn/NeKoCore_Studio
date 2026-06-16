// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "FMisiones.h"
#include "GameStateCountlessBlood.generated.h"

/**
 * 
 */
UCLASS()
class PROYECTOEJERCICIO_API AGameStateCountlessBlood : public AGameStateBase
{
	GENERATED_BODY()
public:
	AGameStateCountlessBlood();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Quest")
	TArray<FQuestProgress> SharedQuestProgress;

	UFUNCTION(BlueprintCallable, Category = "Quest")
	void AddSharedProgress(FName QuestID, ETiposDeObjetivo Type, int32 Amount);
	
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Partida")
	bool bPartidaFinalizada;
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Quest")
	int32 GetSharedProgress(FName QuestID) const;
};
