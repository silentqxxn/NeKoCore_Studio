// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FItemData.h"
#include "InterfazCrafteo.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UInterfazCrafteo : public UInterface
{
	GENERATED_BODY()
	
};


class PROYECTOEJERCICIO_API IInterfazCrafteo
{
	GENERATED_BODY()
	
	public:
		UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Crafteo")
		bool CraftearItem(FName RecetaID);

		UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Crafteo")
		bool PuedeCraftear(FName RecetaID) const;

		UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Crafteo")
		TArray<FName> ObtenerRecetasDisponibles() const;

};
