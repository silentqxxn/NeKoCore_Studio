// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FRecetaData.h"
#include "Blueprint/UserWidget.h"
#include "CraftSlotWidget.generated.h"

class UComponenteCrafteo;

UCLASS()
class PROYECTOEJERCICIO_API UCraftSlotWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category="Crafteo")
	void Configurar(const FRecetaData& Receta, UComponenteCrafteo* InComponente);

	UFUNCTION(BlueprintCallable, Category="Crafteo")
	void OnCraftearPresionado();

	UPROPERTY(BlueprintReadOnly, Category="Crafteo")
	FRecetaData RecetaData;

	// Texto "Madera: 1/2\nTela: 0/1" — armado en C++ para no loopear en BP
	UFUNCTION(BlueprintPure, Category="Crafteo")
	FText GetIngredientesTexto() const;

protected:
	UPROPERTY(BlueprintReadOnly, Category="Crafteo")
	UComponenteCrafteo* Componente;

	UFUNCTION(BlueprintImplementableEvent, Category="Crafteo")
	void OnRecetaActualizada(const FRecetaData& Receta, bool bPuedeCraftear);
};
