// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZonaNocturna.generated.h"

class UBoxComponent;
class ADirectionalLight;
class AExponentialHeightFog;
UCLASS()
class PROYECTOEJERCICIO_API AZonaNocturna : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AZonaNocturna();

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes")
	UBoxComponent* ZonaColision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Entorno")
	ADirectionalLight* LuzDelSol;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Entorno")
	AExponentialHeightFog* Niebla;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Entorno | Configuracion")
	float PitchSolNoche = -90.0f; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Entorno | Configuracion")
	float DensidadNieblaNoche = 0.15f; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Entorno | Configuracion")
	float VelocidadTransicion = 1.5f;

	float PitchSolDia;
	float DensidadNieblaDia;
	bool bHaciaLaNoche = false;

	FTimerHandle TimerTransicionEntorno;

	UFUNCTION()
	void AlEntrarEnZona(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void AlSalirDeZona(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void InterpolarEntorno();
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
