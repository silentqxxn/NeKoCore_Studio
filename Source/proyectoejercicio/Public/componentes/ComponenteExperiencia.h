// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FExperienciaData.h"
#include "Components/ActorComponent.h"
#include "ComponenteExperiencia.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnXPCambia, int32, XPActual, int32, XPMaximo);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSubeNivel, int32, NivelAnterior, int32, NivelNuevo);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROYECTOEJERCICIO_API UComponenteExperiencia : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UComponenteExperiencia();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(ReplicatedUsing=OnRep_Datos, BlueprintReadOnly, Category="Experiencia")
	FExperienciaData Datos;
	
	UFUNCTION(BlueprintCallable, Server, Reliable, Category="Experiencia")
	void Server_AgregarXP(int32 Cantidad);

	UFUNCTION(BlueprintPure, Category="Experiencia")
	int32 GetNivel() const { return Datos.NivelActual; }

	UFUNCTION(BlueprintPure, Category="Experiencia")
	int32 GetXPActual() const { return Datos.XPActual; }

	UFUNCTION(BlueprintPure, Category="Experiencia")
	int32 GetXPRequerida() const { return Datos.XPParaSiguienteNivel; }

	UFUNCTION(BlueprintPure, Category="Experiencia")
	float GetPorcentajeXP() const;

	UPROPERTY(BlueprintAssignable, Category="Experiencia")
	FOnXPCambia OnXPCambia;

	UPROPERTY(BlueprintAssignable, Category="Experiencia")
	FOnSubeNivel OnSubeNivel;
	



protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
private:
	
	
	
	UFUNCTION()
	void OnRep_Datos(FExperienciaData OldDatos);
	
	int32 CalcularXPRequerida(int32 Nivel) const;
	
	void ProcesarSubidaNivel();
};
