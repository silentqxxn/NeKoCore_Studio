// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FMisiones.h"
#include "FTiposObjetivosMision.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "ComponenteMisiones.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestCompletedSignature, FName, QuestID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnQuestListUpdated);
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROYECTOEJERCICIO_API UComponenteMisiones : public UActorComponent
{
	GENERATED_BODY()


public:
	
	// Sets default values for this component's properties
	UComponenteMisiones();
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Misiones")
	UDataTable* QuestDataTable;

	UPROPERTY(ReplicatedUsing = OnRep_ActiveQuests, BlueprintReadOnly, Category = "Misiones")
	TArray<FName> ActiveQuests;
	
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Misiones")
	TArray<FName> CompletedQuests;
	
	UFUNCTION()
	void OnRep_ActiveQuests();

	//UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Misiones")
	//void AcceptQuest(FName QuestID);

	UFUNCTION(BlueprintCallable, Category = "Misiones")
	void ProgressObjective(FName QuestID, ETiposDeObjetivo Type, int32 Amount);

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Misiones")
	void Server_TurnInQuest(FName QuestID);
		
		
	UPROPERTY(ReplicatedUsing = OnRep_IndividualProgress, BlueprintReadOnly, Category = "Misiones")
	TArray<FQuestProgress> IndividualProgress;

	UFUNCTION(Client, Reliable)
	void Client_MisionCompletada(FName QuestID);
	
	UFUNCTION()
	void OnRep_IndividualProgress();
	
	UFUNCTION()
	void OnRep_Progress();
	
	//UFUNCTION(BlueprintCallable, Category = "Misiones")
	//void TurnInQuest(FName QuestID);
	
	UPROPERTY(BlueprintAssignable, Category = "Misiones")
	FOnQuestCompletedSignature OnQuestCompleted;
	
	UFUNCTION(BlueprintPure, Category = "Misiones")
	bool IsQuestCompleted(FName QuestID) const;
	
	UPROPERTY(BlueprintAssignable, Category = "Misiones")
	FOnQuestListUpdated OnQuestListUpdated;
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Misiones")
	bool IsObjectiveStageComplete(FName ObjectiveID) const;

	bool FindStageForObjective(FName ObjectiveID, FName& OutQuestID, FQuestStage& OutStage) const;
	
	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Misiones")
	void Server_AcceptQuest(FName QuestID);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Misiones")
	int32 GetCurrentObjectiveProgress(FName ObjectiveID, ETiposDeObjetivo Type) const;
	
	
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
