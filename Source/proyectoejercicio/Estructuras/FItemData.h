#pragma once
#include "CoreMinimal.h"
#include "Engine/DataTable.h" 
#include "FItemData.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Material    UMETA(DisplayName="Material"),
	Consumible  UMETA(DisplayName="Consumible"),
	Arma        UMETA(DisplayName="Arma"),
	Equipable   UMETA(DisplayName="Equipable")
};

USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Descripcion;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemType Tipo = EItemType::Material;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Cantidad = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxStack = 99;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UTexture2D> Icono;
	
	bool operator==(const FItemData& Otro) const
	{
		return ItemID == Otro.ItemID;
	}
};

