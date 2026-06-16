// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FRecetaData.h"
#include "Actores/FogataNueva.h"
#include "Components/ActorComponent.h"
#include "ComponenteCrafteo.generated.h"

class UComponenteInventario;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROYECTOEJERCICIO_API UComponenteCrafteo : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UComponenteCrafteo();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Crafteo")
	UDataTable* TablaRecetas;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Crafteo")
	UDataTable* TablaItems; 

	UFUNCTION(BlueprintCallable, Server, Reliable, Category="Crafteo")
	void Server_Craftear(FName RecetaID);

	UFUNCTION(BlueprintPure, Category="Crafteo")
	bool PuedeCraftear(FName RecetaID) const;

	UFUNCTION(BlueprintPure, Category="Crafteo")
	TArray<FName> ObtenerRecetasDisponibles() const;

	UFUNCTION(BlueprintPure, Category="Crafteo")
	TArray<FRecetaData> ObtenerTodasLasRecetas() const;

	UFUNCTION(BlueprintPure, Category="Crafteo")
	UComponenteInventario* GetInventario() const;
	
	void SetFogataCercana(AFogataNueva* Fogata);
	
	UFUNCTION(BlueprintPure, Category="Crafteo")
	AFogataNueva* GetFogataCercana() const { return FogataActual; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	bool BuscarReceta(FName RecetaID, FRecetaData& OutReceta) const;
	//bool CercaDeFogata() const;
	
	UPROPERTY()
	AFogataNueva* FogataActual = nullptr;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
