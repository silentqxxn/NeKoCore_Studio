// Fill out your copyright notice in the Description page of Project Settings.


#include "componentes/ComponenteMisiones.h"

#include "FMisiones.h"
#include "componentes/ComponenteExperiencia.h"
#include "Framework/GameStateCountlessBlood.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"


UComponenteMisiones::UComponenteMisiones()
{
	
	PrimaryComponentTick.bCanEverTick = false;

    SetIsReplicatedByDefault(true);
	
}

void UComponenteMisiones::AcceptQuest_Implementation(FName QuestID)
{
    if (!QuestDataTable) return;
    
    if (ActiveQuests.Contains(QuestID)) return;

    FQuestData* QuestData = QuestDataTable->FindRow<FQuestData>(QuestID, TEXT("Context_AcceptQuest"));
    
    if (QuestData)
    {
        ActiveQuests.Add(QuestID);
        
        OnQuestListUpdated.Broadcast();
    }
}

void UComponenteMisiones::ProgressObjective(FName QuestID, ETiposDeObjetivo Type, int32 Amount)
{
    if (!GetOwner()->HasAuthority()) return;

    FName OwningQuestID;
    FQuestStage Stage;
    if (!FindStageForObjective(QuestID, OwningQuestID, Stage)) return; 

    if (IsObjectiveStageComplete(QuestID)) return; 

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
            
            OnRep_IndividualProgress(); 

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

void UComponenteMisiones::Server_TurnInQuest_Implementation(FName QuestID)
{
    if (!ActiveQuests.Contains(QuestID)) return;

    if (!IsQuestCompleted(QuestID))
    {
        UE_LOG(LogTemp, Error, TEXT("¡ALERTA! El servidor dice que la misión %s NO está completa. El progreso no coincide."), *QuestID.ToString());
        return; 
    }

    if (QuestDataTable)
    {
        FQuestData* QuestData = QuestDataTable->FindRow<FQuestData>(QuestID, TEXT("TurnIn"));
        if (QuestData)
        {
            float TotalXPFloat = 0.f;
            for (const FQuestStage& Stage : QuestData->Stages)
            {
                TotalXPFloat += Stage.BonusXP;
            }
            
            int32 TotalXP = FMath::RoundToInt(TotalXPFloat);

            UComponenteExperiencia* CompXP = GetOwner()->FindComponentByClass<UComponenteExperiencia>();
            
            if (!CompXP)
            {
                if (APlayerState* PS = Cast<APlayerState>(GetOwner()))
                {
                    if (APawn* MiPersonaje = PS->GetPawn())
                    {
                        CompXP = MiPersonaje->FindComponentByClass<UComponenteExperiencia>();
                    }
                }
            }

            if (CompXP && TotalXP > 0)
            {
                CompXP->Server_AgregarXP(TotalXP);
            }
        }
    }

    ActiveQuests.Remove(QuestID);
    IndividualProgress.RemoveAll([&](const FQuestProgress& Item) { return Item.QuestID == QuestID; });
   
    OnQuestCompleted.Broadcast(QuestID);
    OnQuestListUpdated.Broadcast();
    
}

void UComponenteMisiones::Client_MisionCompletada_Implementation(FName QuestID)
{
    OnQuestCompleted.Broadcast(QuestID);
}

void UComponenteMisiones::OnRep_IndividualProgress()
{
    OnQuestListUpdated.Broadcast();
}

void UComponenteMisiones::TurnInQuest(FName QuestID)
{
    if (!GetOwner()->HasAuthority()) return;
    if (!ActiveQuests.Contains(QuestID)) return;

    if (IsQuestCompleted(QuestID))
    {
        ActiveQuests.Remove(QuestID);
        IndividualProgress.RemoveAll([&](const FQuestProgress& Item) { return Item.QuestID == QuestID; }); // (Recuerda revisar ese Item.QuestID vs ObjectiveID)
       
        OnQuestCompleted.Broadcast(QuestID);
        OnQuestListUpdated.Broadcast();

        Client_MisionCompletada(QuestID); 
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

void UComponenteMisiones::Server_AcceptQuest_Implementation(FName QuestID)
{
    if (!QuestDataTable) return;

    // 2. Seguridad: Evitamos que el jugador acepte la misma misión dos veces
    if (ActiveQuests.Contains(QuestID)) return;

    // 3. Verificamos que la misión exista en la base de datos
    FQuestData* QuestData = QuestDataTable->FindRow<FQuestData>(QuestID, TEXT("Context_AcceptQuest"));

    if (QuestData)
    {
        // 4. ¡AQUÍ OCURRE LA MAGIA! El Servidor anota la misión en su registro
        ActiveQuests.Add(QuestID);
        
        // 5. Actualizamos la interfaz del servidor (El cliente se actualizará solo gracias a tu OnRep_ActiveQuests)
        OnQuestListUpdated.Broadcast();
        
        // Un log verde para que festejes cuando lo veas funcionar
        UE_LOG(LogTemp, Warning, TEXT("¡ÉXITO! El Servidor le dio la misión %s al jugador correctamente."), *QuestID.ToString());
    }
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
    
    if (GetOwner()->HasAuthority()) {
        UE_LOG(LogTemp, Warning, TEXT("Componente existe en SERVER"));
    } else {
        UE_LOG(LogTemp, Warning, TEXT("Componente existe en CLIENTE"));
    }
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
