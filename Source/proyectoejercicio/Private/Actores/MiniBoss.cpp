// Fill out your copyright notice in the Description page of Project Settings.


#include "Actores/MiniBoss.h"

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

// Called when the game starts or when spawned
void AMiniBoss::BeginPlay()
{
	Super::BeginPlay();
	VidaActual = VidaMaxima;
}
void AMiniBoss::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMiniBoss, FaseActual);
}

float AMiniBoss::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DañoReal = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	VidaActual -= DañoReal;

	if (HasAuthority() && VidaActual <= (VidaMaxima * 0.5f) && FaseActual == EFasesBoss::Fase1)
	{
		FaseActual = EFasesBoss::Fase2;
		OnRep_FaseActual(); 
	}

	return DañoReal;
}

void AMiniBoss::OnRep_FaseActual()
{
	if (FaseActual == EFasesBoss::Fase2)
	{
	
	}
}

void AMiniBoss::RealizarAtaqueMelee()
{
	if (HasAuthority())
	{
		TArray<AActor*> IgnorarActores;
		IgnorarActores.Add(this);
        
		UGameplayStatics::ApplyRadialDamage(this, 30.f,GetActorLocation(), 400.f,UDamageType::StaticClass(),IgnorarActores, this, GetController(),true);

		Multicast_ReproducirFXMelee();
	}
}

void AMiniBoss::RealizarAtaqueRanged(AActor* Objetivo)
{
	if (HasAuthority() && Objetivo)
	{
		Multicast_ReproducirFXRanged(Objetivo);
	}
}

void AMiniBoss::RealizarAtaqueEspecial()
{
	if (HasAuthority() && FaseActual == EFasesBoss::Fase2)
	{
		Multicast_ReproducirFXEspecial();
	}
}

void AMiniBoss::Multicast_ReproducirFXMelee_Implementation()
{
	if (FX_AtaqueMelee)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), FX_AtaqueMelee, GetActorLocation() - FVector(0,0,90.f),FRotator::ZeroRotator);
	}
}

void AMiniBoss::Multicast_ReproducirFXRanged_Implementation(AActor* Objetivo)
{
	if (FX_AtaqueRanged && Objetivo)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), FX_AtaqueRanged, Objetivo->GetActorLocation(), FRotator::ZeroRotator);
	}
}

void AMiniBoss::Multicast_ReproducirFXEspecial_Implementation()
{
	if (FX_AtaqueEspecial)
	{
		UNiagaraFunctionLibrary::SpawnSystemAttached(FX_AtaqueEspecial,GetMesh(),NAME_None, FVector::ZeroVector, FRotator::ZeroRotator,EAttachLocation::SnapToTarget,true);
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

