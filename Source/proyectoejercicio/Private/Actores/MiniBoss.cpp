// Fill out your copyright notice in the Description page of Project Settings.


#include "Actores/MiniBoss.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


// Sets default values
AMiniBoss::AMiniBoss()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

void AMiniBoss::BeginPlay()
{
	Super::BeginPlay();
	VidaActual = VidaMaxima;
}
void AMiniBoss::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMiniBoss, VidaActual);
	DOREPLIFETIME(AMiniBoss, VidaMaxima);
}

float AMiniBoss::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DañoReal = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
    
	if (HasAuthority() && DañoReal > 0.f)
	{
		RecibirDanioInterfaz(DañoReal);
	}
    
	return DañoReal;
}

void AMiniBoss::RecibirDanioInterfaz(float CantidadDanio)
{
	if (HasAuthority() && CantidadDanio > 0.f && VidaActual > 0.f)
	{
		VidaActual = FMath::Clamp(VidaActual - CantidadDanio, 0.f, VidaMaxima);

		OnRep_VidaActual();	
	}
}

void AMiniBoss::OnRep_VidaActual()
{
	OnVidaBossCambiada.Broadcast(VidaActual, VidaMaxima);
}

void AMiniBoss::Multicast_ReproducirAnimacion_Implementation(UAnimMontage* MontageToPlay)
{
	if (MontageToPlay)
	{
		PlayAnimMontage(MontageToPlay);
	}
}

// Called every frame
void AMiniBoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMiniBoss::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AMiniBoss::IniciarFaseAtaqueRanged()
{
	if (HasAuthority() && !bEstaBombardeando)
	{
		bEstaBombardeando = true;
		FVector SpawnLocation = GetActorLocation() + (GetActorForwardVector() * DistanciaAdelanteNube) + FVector(0.f, 0.f, AlturaNube);
        
		Multicast_CrearNube(SpawnLocation);
		Multicast_ReproducirAnimacion(Montage_InvocarNube);
	}
}

void AMiniBoss::LanzarProyectilDesdeNube(AActor* Objetivo)
{
	if (HasAuthority() && bEstaBombardeando && NubeSpawnActiva && Objetivo && ClaseMeteorito)
	{
		FVector SpawnLocation = NubeSpawnActiva->GetComponentLocation();
		FVector DireccionAObjetivo = Objetivo->GetActorLocation() - SpawnLocation;
		FRotator SpawnRotation = DireccionAObjetivo.Rotation();

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		GetWorld()->SpawnActor<AActor>(ClaseMeteorito, SpawnLocation, SpawnRotation, SpawnParams);
	}
}

void AMiniBoss::FinalizarFaseAtaqueRanged()
{
	if (HasAuthority() && bEstaBombardeando)
	{
		bEstaBombardeando = false;
		Multicast_DestruirNube();
	}
}



void AMiniBoss::Multicast_DestruirNube_Implementation()
{
	if (NubeSpawnActiva)
	{
		NubeSpawnActiva->DestroyComponent();
		NubeSpawnActiva = nullptr;
	}
	
}

void AMiniBoss::Multicast_CrearNube_Implementation(FVector Ubicacion)
{
	if (FX_NubeSpawn)
	{
		NubeSpawnActiva = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), FX_NubeSpawn, Ubicacion, FRotator::ZeroRotator);
	}
}
