#pragma once
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "FExperienciaData.generated.h"

USTRUCT(BlueprintType)

struct FExperienciaData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 NivelActual = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 XPActual = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 XPParaSiguienteNivel = 100;
};

USTRUCT(BlueprintType)
struct FProgresoNivelRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 SangNecesaria = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float PorcentajeHP = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float PorcentajeAtaque = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float PorcentajeVelocidad = 0.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float PorcentajeDefensa = 0.0f;
};
