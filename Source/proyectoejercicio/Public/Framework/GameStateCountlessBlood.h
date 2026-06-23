// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "FMisiones.h"
#include "GameStateCountlessBlood.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSharedProgressUpdated);
UCLASS()
class PROYECTOEJERCICIO_API AGameStateCountlessBlood : public AGameStateBase
{
	GENERATED_BODY()
public:
	AGameStateCountlessBlood();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION(BlueprintCallable, Category = "Quest")
	void AddSharedProgress(FName QuestID, ETiposDeObjetivo Type, int32 Amount);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Quest")
	int32 GetSharedProgress(FName QuestID) const;
	
	UPROPERTY(BlueprintAssignable, Category = "Misiones Compartidas")
	FOnSharedProgressUpdated OnSharedProgressUpdated;
	
protected:

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Partida")
	bool bPartidaFinalizada;
	
	UPROPERTY(ReplicatedUsing = OnRep_SharedQuestProgress)
	TArray<FQuestProgress> SharedQuestProgress;
	
	UFUNCTION()
	void OnRep_SharedQuestProgress();
	

};
