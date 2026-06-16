#pragma once
#include "FItemData.h"
#include "FRecetaData.generated.h"

USTRUCT(BlueprintType)
struct FIngredienteReceta
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Cantidad = 1;
};

USTRUCT(BlueprintType)
struct FRecetaData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName RecetaID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FIngredienteReceta> Ingredientes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemResultadoID;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CantidadResultado = 1;
};