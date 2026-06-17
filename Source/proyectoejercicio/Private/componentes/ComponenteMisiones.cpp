// Fill out your copyright notice in the Description page of Project Settings.


#include "componentes/ComponenteMisiones.h"

#include "FMisiones.h"
#include "Framework/GameStateCountlessBlood.h"
#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
UComponenteMisiones::UComponenteMisiones()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UComponenteMisiones::AcceptQuest_Implementation(FName QuestID)
{
    if (!QuestDataTable) return;
    
    // Evitar aceptar misiones duplicadas
    if (ActiveQuests.Contains(QuestID)) return;

    FQuestData* QuestData = QuestDataTable->FindRow<FQuestData>(QuestID, TEXT("Context_AcceptQuest"));
    
    if (QuestData)
    {
        ActiveQuests.Add(QuestID);
        
        // Disparamos el delegado para que la UI se refresque automáticamente
        OnQuestListUpdated.Broadcast();
        
        UE_LOG(LogTemp, Log, TEXT("Misión aceptada con éxito: %s"), *QuestID.ToString());
    }
}

void UComponenteMisiones::ProgressObjective(FName QuestID, ETiposDeObjetivo Type, int32 Amount)
{
    if (!GetOwner()->HasAuthority()) return;

    switch (Type)
    {
        case ETiposDeObjetivo::Locacion:
        case ETiposDeObjetivo::Interactuar:
        {
            bool bFound = false;
            for (FQuestProgress& QuestProg : IndividualProgress)
            {
                if (QuestProg.QuestID == QuestID)
                {
                    QuestProg.Progress += Amount;
                    bFound = true;
                    break;
                }
            }

            if (!bFound)
            {
                IndividualProgress.Add(FQuestProgress(QuestID, Amount));
            }
            break;
        }

        case ETiposDeObjetivo::Matar:
        case ETiposDeObjetivo::Recolectar:
        {
            if (AGameStateCountlessBlood* GS = GetWorld()->GetGameState<AGameStateCountlessBlood>())
            {
                GS->AddSharedProgress(QuestID, Type, Amount);
            }
            break;
        }
    }
}

void UComponenteMisiones::TurnInQuest(FName QuestID)
{
    if (!GetOwner()->HasAuthority()) return;
    if (!ActiveQuests.Contains(QuestID)) return;

    if (IsQuestCompleted(QuestID))
    {
        ActiveQuests.Remove(QuestID);
        // Limpiamos progreso individual residual
        IndividualProgress.RemoveAll([&](const FQuestProgress& Item) { return Item.QuestID == QuestID; });
        
        OnQuestCompleted.Broadcast(QuestID);
        OnQuestListUpdated.Broadcast(); // Refrescamos la lista al completar
    }
}

bool UComponenteMisiones::IsQuestCompleted(FName QuestID) const
{
    if (!QuestDataTable) return false;

    const FQuestData* QuestData = QuestDataTable->FindRow<FQuestData>(QuestID, TEXT("Context_IsQuestCompleted"));
    if (!QuestData) return false;

    for (const FQuestStage& Stage : QuestData->Stages)
    {
        if (Stage.bIsOptional) continue;

        if (GetCurrentObjectiveProgress(Stage.ObjectiveID, Stage.TipoObjetivo) < Stage.CantidadRequerida)
        {
            return false;
        }
    }
    return true;
}

// Funciones auxiliares
bool UComponenteMisiones::FindStageForObjective(FName ObjectiveID, FName& OutQuestID, FQuestStage& OutStage) const
{
    if (!QuestDataTable) return false;

    for (const FName& ActiveQuestID : ActiveQuests)
    {
        if (const FQuestData* QuestData = QuestDataTable->FindRow<FQuestData>(ActiveQuestID, TEXT("Context_FindStage")))
        {
            for (const FQuestStage& Stage : QuestData->Stages)
            {
                if (Stage.ObjectiveID == ObjectiveID)
                {
                    OutQuestID = ActiveQuestID;
                    OutStage = Stage;
                    return true;
                }
            }
        }
    }
    return false;
}

int32 UComponenteMisiones::GetCurrentObjectiveProgress(FName ObjectiveID, ETiposDeObjetivo Type) const
{
    if (Type == ETiposDeObjetivo::Locacion || Type == ETiposDeObjetivo::Interactuar)
    {
        for (const FQuestProgress& QuestProg : IndividualProgress)
        {
            if (QuestProg.QuestID == ObjectiveID) return QuestProg.Progress;
        }
        return 0;
    }

    if (AGameStateCountlessBlood* GS = GetWorld()->GetGameState<AGameStateCountlessBlood>())
    {
        return GS->GetSharedProgress(ObjectiveID);
    }
    return 0;
}

void UComponenteMisiones::BeginPlay()
{
    Super::BeginPlay();
}

void UComponenteMisiones::TickComponent(float DeltaTime, ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UComponenteMisiones::IsObjectiveStageComplete(FName ObjectiveID) const
{
    FName OwningQuestID;
    FQuestStage Stage;

    if (!FindStageForObjective(ObjectiveID, OwningQuestID, Stage)) return false;

    return GetCurrentObjectiveProgress(ObjectiveID, Stage.TipoObjetivo) >= Stage.CantidadRequerida;
}

void UComponenteMisiones::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(UComponenteMisiones, ActiveQuests);
    DOREPLIFETIME(UComponenteMisiones, IndividualProgress);
}

void UComponenteMisiones::OnRep_ActiveQuests()
{
    OnQuestListUpdated.Broadcast();
}
