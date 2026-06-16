#pragma once
#include "CoreMinimal.h"
#include "EEquipableSlot.generated.h"

UENUM(BlueprintType)
enum class EEquipableSlot: uint8
{
	Ninguno       UMETA(DisplayName = "Ninguno"),
	ManoDerecha   UMETA(DisplayName = "Mano Derecha"),   // Arma mele
	ManoIzquierda UMETA(DisplayName = "Mano Izquierda"), // Escudo
	Espalda       UMETA(DisplayName = "Espalda")         // Distancia	
};
