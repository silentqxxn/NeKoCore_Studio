// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WidgetEquipo.generated.h"

/**
 * 
 */
class ACharacterprincipal;
class UItemSlotWidget;
UCLASS()
class PROYECTOEJERCICIO_API UWidgetEquipo : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category="Equipo")
	void Inicializar(ACharacterprincipal* InCharacter);

	UFUNCTION(BlueprintCallable, Category="Equipo")
	void Refrescar();

protected:
	UPROPERTY(meta=(BindWidget))
	UItemSlotWidget* SlotArma;

	UPROPERTY(meta=(BindWidget))
	UItemSlotWidget* SlotManoDerecha;

	UPROPERTY(meta=(BindWidget))
	UItemSlotWidget* SlotManoIzquierda;

private:
	UPROPERTY()
	ACharacterprincipal* Character;
};
