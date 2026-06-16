#pragma once
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
