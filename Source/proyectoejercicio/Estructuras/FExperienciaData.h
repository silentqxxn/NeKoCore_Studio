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
	int32 SangNecesaria;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float PorcentajeHP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float PorcentajeAtaque;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float PorcentajeVelocidad;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float PorcentajeDefensa;
};
