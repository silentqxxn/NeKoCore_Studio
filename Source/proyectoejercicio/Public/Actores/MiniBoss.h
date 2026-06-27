// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EFasesBoss.h"
#include "EnemigoBase.h"
#include "NiagaraComponent.h"
#include "MiniBoss.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnVidaBossCambiada, float, VidaActual, float, VidaMaxima);
UCLASS()
class PROYECTOEJERCICIO_API AMiniBoss : public AEnemigoBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMiniBoss();

	UPROPERTY(BlueprintAssignable, Category = "Boss UI")
	FOnVidaBossCambiada OnVidaBossCambiada;
	
	UPROPERTY(ReplicatedUsing = OnRep_VidaActual, EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float VidaActual = 800.f;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float VidaMaxima = 800.f;
	
	UFUNCTION(BlueprintCallable, Category = "Combate")
	void RecibirDanioInterfaz(float CantidadDanio);
	UFUNCTION()
	void OnRep_VidaActual();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Eventos")
	virtual void Morir();
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_BossMuere();
	UFUNCTION(BlueprintImplementableEvent, Category = "Boss | Eventos")
	void BP_AlMorir();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animaciones")
	UAnimMontage* Montage_InvocarNube;
	
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ReproducirAnimacion(UAnimMontage* MontageToPlay);
	
	UPROPERTY(EditDefaultsOnly, Category = "Ranged")
	TSubclassOf<AActor> ClaseMeteorito;
	
	
	UPROPERTY(EditDefaultsOnly, Category = "Combate")
	UNiagaraSystem* FX_NubeSpawn;

	UPROPERTY()
	UNiagaraComponent* NubeSpawnActiva;

	UPROPERTY(EditDefaultsOnly, Category = "Combate")
	float AlturaNube = 500.f;

	UPROPERTY(EditDefaultsOnly, Category = "Combate")
	float DistanciaAdelanteNube = 200.f;

	bool bEstaBombardeando = false;
	
	
	UPROPERTY(EditDefaultsOnly, Category = "Ranged")
	FName SocketLanzamiento = "";
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION(BlueprintCallable, Category = "Combate")
	void IniciarFaseAtaqueRanged();

	UFUNCTION(BlueprintCallable, Category = "Combate")
	void LanzarProyectilDesdeNube(AActor* Objetivo);

	UFUNCTION(BlueprintCallable, Category = "Combate")
	void FinalizarFaseAtaqueRanged();
	
	
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_CrearNube(FVector Ubicacion);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_DestruirNube();
};
