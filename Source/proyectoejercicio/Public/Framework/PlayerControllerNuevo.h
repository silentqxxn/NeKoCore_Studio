// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FItemData.h" 
#include "InputMappingContext.h"
#include "PlayerControllerNuevo.generated.h"

//class UInputMappingContext;

UCLASS()
class PROYECTOEJERCICIO_API APlayerControllerNuevo : public APlayerController
{
	GENERATED_BODY()
	
public:
	APlayerControllerNuevo();
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* IMC_CountlessBlood;
	
	
	UPROPERTY(EditDefaultsOnly, Category="UI-Inventario")
	TSubclassOf<UUserWidget> ClaseWidgetInventario;

	UPROPERTY(EditDefaultsOnly, Category="UI-Crafteo")
	TSubclassOf<UUserWidget> ClaseWidgetCrafteo;

	UPROPERTY(EditDefaultsOnly, Category="UI-Stats")
	TSubclassOf<UUserWidget> ClaseWidgetStats;

	UFUNCTION(BlueprintCallable, Category="UI")
	void MostrarInventario();

	UFUNCTION(BlueprintCallable, Category="UI")
	void OcultarInventario();

	UFUNCTION(BlueprintCallable, Category="UI")
	void ActualizarInventario(const TArray<FItemData>& Items);


	UFUNCTION(BlueprintCallable, Category="UI")
	void MostrarCrafteo();

	UFUNCTION(BlueprintCallable, Category="UI")
	void OcultarCrafteo();

	UFUNCTION(BlueprintCallable, Category="UI")
	void ActualizarStats();

	UFUNCTION()
	void ActualizarBarraXP(int32 XPActual, int32 XPMaximo);

	UFUNCTION()
	void MostrarSubidaNivel(int32 NivelAnterior, int32 NivelNuevo);

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* IA_Inventario;

	UFUNCTION(BlueprintCallable, Category="UI")
	void ToggleInventario();
	
	UFUNCTION(BlueprintCallable, Category="UI")
	void ToggleCrafteo();
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	UPROPERTY() UUserWidget* WidgetInventario = nullptr;
	UPROPERTY() UUserWidget* WidgetCrafteo    = nullptr;
	UPROPERTY() UUserWidget* WidgetStats      = nullptr;

	UUserWidget* ObtenerOCrearWidget(TSubclassOf<UUserWidget> Clase,UUserWidget*& Cache);
	
	void AbrirUI();
	void CerrarUI();
	void SuscribirEventosInventario();

};
