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

	ZonaColision->OnComponentBeginOverlap.AddDynamic(this, &AZonaNocturna::AlEntrarEnZona);
	ZonaColision->OnComponentEndOverlap.AddDynamic(this, &AZonaNocturna::AlSalirDeZona);

	if (LuzDelSol)
	{
		//PitchSolDia = LuzDelSol->()->GetComponentRotation().Pitch;
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
    if (OtherActor && OtherActor->IsA(ACharacterprincipal::StaticClass()))
    {
        bHaciaLaNoche = true;
        GetWorldTimerManager().SetTimer(TimerTransicionEntorno, this, &AZonaNocturna::InterpolarEntorno, 0.01f, true);
    }
}

void AZonaNocturna::AlSalirDeZona(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor && OtherActor->IsA(ACharacterprincipal::StaticClass()))
    {
        bHaciaLaNoche = false; 
    	GetWorldTimerManager().SetTimer(TimerTransicionEntorno, this, &AZonaNocturna::InterpolarEntorno, 0.01f, true);
    }
}

void AZonaNocturna::InterpolarEntorno()
{
	bool bSolTerminado = true;
	bool bNieblaTerminada = true;

	float TargetPitch = bHaciaLaNoche ? PitchSolNoche : PitchSolDia;
	float TargetDensidad = bHaciaLaNoche ? DensidadNieblaNoche : DensidadNieblaDia;

	if (LuzDelSol)
	{
		// Usamos GetLightComponent() para acceder a la luz real
		auto* LightComp = LuzDelSol->GetLightComponent();
        
		if (LightComp)
		{
			float PitchActual = LightComp->GetComponentRotation().Pitch;
        
			if (!FMath::IsNearlyEqual(PitchActual, TargetPitch, 0.5f))
			{
				float NuevoPitch = FMath::FInterpTo(PitchActual, TargetPitch, 0.01f, VelocidadTransicion);
            
				FRotator NuevaRotacion = LightComp->GetComponentRotation();
				NuevaRotacion.Pitch = NuevoPitch;
				LightComp->SetWorldRotation(NuevaRotacion);
            
				bSolTerminado = false;
			}
		}
	}

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

	if (bSolTerminado && bNieblaTerminada)
	{
		GetWorldTimerManager().ClearTimer(TimerTransicionEntorno);
	}
}