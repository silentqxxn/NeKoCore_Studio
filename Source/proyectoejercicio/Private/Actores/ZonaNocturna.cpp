// Fill out your copyright notice in the Description page of Project Settings.


#include "Actores/ZonaNocturna.h"
#include "Components/BoxComponent.h"
#include "Engine/DirectionalLight.h"
#include "Components/DirectionalLightComponent.h"
#include "Engine/ExponentialHeightFog.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Actores/Characterprincipal.h"


// Sets default values
AZonaNocturna::AZonaNocturna()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	ZonaColision = CreateDefaultSubobject<UBoxComponent>(TEXT("ZonaColision"));
	RootComponent = ZonaColision;
    
	ZonaColision->SetBoxExtent(FVector(1000.f, 1000.f, 500.f)); 
	ZonaColision->SetCollisionProfileName(TEXT("Trigger"));
}

// Called when the game starts or when spawned
void AZonaNocturna::BeginPlay()
{
	Super::BeginPlay();

	// Vinculamos los eventos de colisión
	ZonaColision->OnComponentBeginOverlap.AddDynamic(this, &AZonaNocturna::AlEntrarEnZona);
	ZonaColision->OnComponentEndOverlap.AddDynamic(this, &AZonaNocturna::AlSalirDeZona);

	// Guardamos cómo estaba el clima de forma natural en el nivel
	if (LuzDelSol && LuzDelSol->GetComponent())
	{
		PitchSolDia = LuzDelSol->GetComponent()->GetComponentRotation().Pitch;
	}
	if (Niebla && Niebla->GetComponent())
	{
		DensidadNieblaDia = Niebla->GetComponent()->FogDensity;
	}
}

// Called every frame
void AZonaNocturna::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AZonaNocturna::AlEntrarEnZona(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // Verificamos que sea el jugador principal y no un enemigo o proyectil
    if (OtherActor && OtherActor->IsA(ACharacterprincipal::StaticClass()))
    {
        bHaciaLaNoche = true;
        // Encendemos el Timer de transición
        GetWorldTimerManager().SetTimer(TimerTransicionEntorno, this, &AZonaNocturna::InterpolarEntorno, 0.01f, true);
    }
}

void AZonaNocturna::AlSalirDeZona(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor && OtherActor->IsA(ACharacterprincipal::StaticClass()))
    {
        bHaciaLaNoche = false; // El objetivo vuelve a ser el Día
        GetWorldTimerManager().SetTimer(TimerTransicionEntorno, this, &AZonaNocturna::InterpolarEntorno, 0.01f, true);
    }
}

void AZonaNocturna::InterpolarEntorno()
{
    bool bSolTerminado = true;
    bool bNieblaTerminada = true;

    // Definimos hacia dónde vamos dependiendo de si el jugador está adentro o afuera
    float TargetPitch = bHaciaLaNoche ? PitchSolNoche : PitchSolDia;
    float TargetDensidad = bHaciaLaNoche ? DensidadNieblaNoche : DensidadNieblaDia;

    // 1. Mover el Sol
    if (LuzDelSol && LuzDelSol->GetComponent())
    {
        float PitchActual = LuzDelSol->GetComponent()->GetComponentRotation().Pitch;
        
        if (!FMath::IsNearlyEqual(PitchActual, TargetPitch, 0.5f))
        {
            float NuevoPitch = FMath::FInterpTo(PitchActual, TargetPitch, 0.01f, VelocidadTransicion);
            
            FRotator NuevaRotacion = LuzDelSol->GetComponent()->GetComponentRotation();
            NuevaRotacion.Pitch = NuevoPitch;
            LuzDelSol->GetComponent()->SetWorldRotation(NuevaRotacion);
            
            bSolTerminado = false;
        }
    }

    // 2. Espesar la Niebla
    if (Niebla && Niebla->GetComponent())
    {
        float DensidadActual = Niebla->GetComponent()->FogDensity;
        
        if (!FMath::IsNearlyEqual(DensidadActual, TargetDensidad, 0.005f))
        {
            float NuevaDensidad = FMath::FInterpTo(DensidadActual, TargetDensidad, 0.01f, VelocidadTransicion);
            Niebla->GetComponent()->SetFogDensity(NuevaDensidad);
            
            bNieblaTerminada = false;
        }
    }

    // 3. Apagar el Timer si ambos elementos llegaron a su objetivo
    if (bSolTerminado && bNieblaTerminada)
    {
        GetWorldTimerManager().ClearTimer(TimerTransicionEntorno);
    }
}