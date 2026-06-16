// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FItemData.h"
#include "InterfazRecogeItems.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInterfazRecogeItems : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROYECTOEJERCICIO_API IInterfazRecogeItems
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Inventario")
	bool RecogerItem(const FItemData& Item);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Inventario")
	bool RecogerTodos(const TArray<FItemData>& Items);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Inventario")
	bool PuedeRecoger(const FItemData& Item) const;

};
