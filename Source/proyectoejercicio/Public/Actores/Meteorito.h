// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Meteorito.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UProjectileMovementComponent;
class UNiagaraSystem;

UCLASS()
class PROYECTOEJERCICIO_API AMeteorito : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMeteorito();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes")
	USphereComponent* SphereCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes")
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditDefaultsOnly, Category = "Combate | Efectos")
	UNiagaraSystem* FX_Impacto;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combate | Stats")
	float DamageAmount = 25.f;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

};
