#pragma once
#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ETiposDeObjetivo :uint8
{
	Locacion UMETA(DisplayName="Locacion"),
	Matar UMETA(DisplayName="Matar"),
	Interactuar UMETA(DisplayName="Interactuar"),
	Recolectar UMETA(DisplayName="Recolectar"),
};


