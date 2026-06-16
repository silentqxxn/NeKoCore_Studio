// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FItemData.h"
#include "Blueprint/UserWidget.h"
#include "ItemSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROYECTOEJERCICIO_API UItemSlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category="Inventario")
	void SetItemData(const FItemData& NuevoItem);

	UPROPERTY(BlueprintReadOnly, Category="Inventario")
	FItemData ItemData;

protected:
	UFUNCTION(BlueprintImplementableEvent, Category="Inventario")
	void OnItemDataActualizado(const FItemData& Item);
	
	
};
