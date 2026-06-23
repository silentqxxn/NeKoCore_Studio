#pragma once
#include "EFasesBoss.generated.h"

UENUM(BlueprintType)
enum class EFasesBoss :uint8
{
	Fase1       UMETA(DisplayName = "Fase 1"),
	Fase2       UMETA(DisplayName = "Fase 2 - Furioso"),
	Derrotado   UMETA(DisplayName = "Derrotado")	
};
