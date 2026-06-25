// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ComponenteEstadisticas.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnVidaCambia, float, VidaActual, float, VidaMaxima);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPersonajeMuere);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPersonajeRevive);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROYECTOEJERCICIO_API UComponenteEstadisticas : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UComponenteEstadisticas();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Estadisticas")
	float VidaBase = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Estadisticas")
	float AtaqueBase = 15.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "Estadisticas")
	float AtaqueActual;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Estadisticas")
	float VelocidadBase = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Estadisticas")
	float DefensaBase = 5.f;
	
	UPROPERTY(ReplicatedUsing = OnRep_VidaActual, BlueprintReadOnly, Category = "Estadisticas")
	float VidaActual;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Estadisticas")
	float VidaMaximaActual;
	
	UPROPERTY(ReplicatedUsing=OnRep_VelocidadActual, BlueprintReadOnly, Category = "Estadisticas")
	float VelocidadActual;
	
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Estadisticas")
	float DefensaActual;

	UPROPERTY(BlueprintAssignable, Category = "Estadisticas")
	FOnVidaCambia OnVidaCambia;

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Estadisticas")
	void Server_AplicarMultiplicadoresNivel(float MultHP, float MultAtaque, float MultVel, float MultDefensa);
	
	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Estadisticas")
	void Server_RecibirDanio(float Cantidad);
	
	UPROPERTY(BlueprintAssignable, Category = "Estadisticas")
	FOnPersonajeMuere OnPersonajeMuere;
	
	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Estadisticas")
	void Server_Revivir();
	
	UPROPERTY(BlueprintAssignable, Category = "Estadisticas")
	FOnPersonajeRevive OnPersonajeRevive;
	
	UPROPERTY(ReplicatedUsing = OnRep_EstaMuerto, BlueprintReadOnly, Category = "Estadisticas")
	bool bEstaMuerto = false;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION()
	void OnRep_VidaActual();
	
	UFUNCTION()
	void OnRep_VelocidadActual();
	
	UFUNCTION()
	void OnRep_EstaMuerto();
};
