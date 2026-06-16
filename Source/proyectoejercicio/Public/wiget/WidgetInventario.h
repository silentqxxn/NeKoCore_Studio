// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FItemData.h"
#include "WidgetInventario.generated.h"

class UItemSlotWidget;
class UWrapBox;

UCLASS()
class PROYECTOEJERCICIO_API UWidgetInventario : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category="Inventario")
	void RefrescarLista(const TArray<FItemData>& Items);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventario")
	TSubclassOf<UItemSlotWidget> ClaseSlot;

protected:

	UFUNCTION(BlueprintImplementableEvent, Category="Inventario")
	void OnInventarioActualizado(const TArray<FItemData>& Items);
	
	UPROPERTY(meta=(BindWidget))
	UWrapBox* ContenedorItems;
};
