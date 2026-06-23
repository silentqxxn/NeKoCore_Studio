// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EFasesBoss.h"
#include "EnemigoBase.h"
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
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(ReplicatedUsing = OnRep_FaseActual, BlueprintReadOnly, Category = "Boss | Fase")
	EFasesBoss FaseActual = EFasesBoss::Fase1;
	
	UFUNCTION()
	void OnRep_FaseActual();
	float VidaActual = 1000.f;
	float VidaMaxima = 1000.f;
	
	// FX
	// ataque mele
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX Boss")
	class UNiagaraSystem* FX_AtaqueMelee;

	// Proyectil
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX Boss")
	class UNiagaraSystem* FX_AtaqueRanged;

	// ataque fase 2
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX Boss")
	class UNiagaraSystem* FX_AtaqueEspecial;
	
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ReproducirFXMelee();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ReproducirFXRanged(AActor* Objetivo);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ReproducirFXEspecial();
	
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable, Category = "Boss - Combate")
	void RealizarAtaqueMelee();

	// El BT llamará a esta cuando esté lejos
	UFUNCTION(BlueprintCallable, Category = "Boss - Combate")
	void RealizarAtaqueRanged(AActor* Objetivo);

	// El BT llamará a esta en Fase 2
	UFUNCTION(BlueprintCallable, Category = "Boss - Combate")
	void RealizarAtaqueEspecial();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
 	// Si usas la interfaz de daño nativa de Unreal (AnyDamage)
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
