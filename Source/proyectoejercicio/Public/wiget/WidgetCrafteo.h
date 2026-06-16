// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WidgetCrafteo.generated.h"

class UWrapBox;
class UComponenteCrafteo;
class UCraftSlotWidget;

UCLASS()
class PROYECTOEJERCICIO_API UWidgetCrafteo : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category="Crafteo")
	void Inicializar(UComponenteCrafteo* InComponente);

	UFUNCTION(BlueprintCallable, Category="Crafteo")
	void Refrescar();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Crafteo")
	TSubclassOf<UCraftSlotWidget> ClaseSlot;

protected:
	// El WrapBox del WBP debe llamarse EXACTAMENTE "ContenedorRecetas"
	UPROPERTY(meta=(BindWidget))
	UWrapBox* ContenedorRecetas;

	UPROPERTY()
	UComponenteCrafteo* CompCrafteo;
};
