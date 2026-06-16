#pragma once
#include "CoreMinimal.h"
#include "FTiposObjetivosMision.h"
#include "Engine/DataTable.h"
#include "FMisiones.generated.h"


USTRUCT(BlueprintType)
struct FQuestStage 
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	FName ObjectiveID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	FText ObjectiveName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	ETiposDeObjetivo TipoObjetivo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	int32 CantidadRequerida;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	bool bIsOptional;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	float BonusXP;
};

// STRUCT DE MISIÓN PRINCIPAL 
USTRUCT(BlueprintType)
struct FQuestData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	FText NombreMision;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	FText LogDescripcionMision;
    
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	FText TrackingDescripcion;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	bool IsMainQuest;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	TArray<FQuestStage> Stages;
};

// STRUCT DE PROGRESO MISION
USTRUCT(BlueprintType)
struct FQuestProgress
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FName QuestID;

	UPROPERTY(BlueprintReadOnly)
	int32 Progress;

	FQuestProgress() : QuestID(NAME_None), Progress(0) {}
    
	FQuestProgress(FName InID, int32 InProgress) : QuestID(InID), Progress(InProgress) {}
};