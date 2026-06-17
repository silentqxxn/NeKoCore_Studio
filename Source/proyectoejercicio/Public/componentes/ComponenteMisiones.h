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
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	UDataTable* QuestDataTable;

	UPROPERTY(ReplicatedUsing = OnRep_ActiveQuests, BlueprintReadOnly, Category = "Quest")
	TArray<FName> ActiveQuests;
	
	UFUNCTION()
	void OnRep_ActiveQuests();

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Quest")
	void AcceptQuest(FName QuestID);

	UFUNCTION(BlueprintCallable, Category = "Quest")
	void ProgressObjective(FName QuestID, ETiposDeObjetivo Type, int32 Amount);

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Quest")
	TArray<FQuestProgress> IndividualProgress;
	
	UFUNCTION(BlueprintCallable, Category = "Quest")
	void TurnInQuest(FName QuestID);
	
	UPROPERTY(BlueprintAssignable, Category = "Quest")
	FOnQuestCompletedSignature OnQuestCompleted;
	
	UFUNCTION(BlueprintPure, Category = "Quest")
	bool IsQuestCompleted(FName QuestID) const;
	
	UPROPERTY(BlueprintAssignable, Category = "Quest")
	FOnQuestListUpdated OnQuestListUpdated;
	
	//////////////
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Quest")
	bool IsObjectiveStageComplete(FName ObjectiveID) const;

	bool FindStageForObjective(FName ObjectiveID, FName& OutQuestID, FQuestStage& OutStage) const;
	int32 GetCurrentObjectiveProgress(FName ObjectiveID, ETiposDeObjetivo Type) const;
	

	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
