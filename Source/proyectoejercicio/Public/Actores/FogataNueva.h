// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FogataNueva.generated.h"

class UStaticMeshComponent;
class UParticleSystemComponent;
class USphereComponent;

UCLASS()
class PROYECTOEJERCICIO_API AFogataNueva : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFogataNueva();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Fogata")
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Fogata")
	UParticleSystemComponent* FuegoFX;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Fogata")
	USphereComponent* RangoCrafteo;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
