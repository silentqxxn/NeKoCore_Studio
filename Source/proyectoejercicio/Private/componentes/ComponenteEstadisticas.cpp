// Fill out your copyright notice in the Description page of Project Settings.


#include "componentes/ComponenteEstadisticas.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"

// Sets default values for this component's properties
UComponenteEstadisticas::UComponenteEstadisticas()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
	// ...
}

void UComponenteEstadisticas::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UComponenteEstadisticas, VidaActual);
	DOREPLIFETIME(UComponenteEstadisticas, VidaMaximaActual);
	DOREPLIFETIME(UComponenteEstadisticas, AtaqueActual);
	DOREPLIFETIME(UComponenteEstadisticas, VelocidadActual);
	DOREPLIFETIME(UComponenteEstadisticas, DefensaActual);
	DOREPLIFETIME(UComponenteEstadisticas, bEstaMuerto);

}

void UComponenteEstadisticas::Server_AplicarMultiplicadoresNivel_Implementation(float MultHP, float MultAtaque,	float MultVel, float MultDefensa)
{
	float NuevaVidaMaxima = VidaMaximaActual * MultHP;
	AtaqueActual = AtaqueActual * MultAtaque;
	VelocidadActual = VelocidadActual * MultVel;
	DefensaActual = DefensaActual * MultDefensa;

	if (NuevaVidaMaxima > VidaMaximaActual)
	{
		float Diferencia = NuevaVidaMaxima - VidaMaximaActual;
		VidaActual += Diferencia;
	}
    
	VidaMaximaActual = NuevaVidaMaxima;

	if (ACharacter* MiPersonaje = Cast<ACharacter>(GetOwner()))
	{
		if (UCharacterMovementComponent* Movimiento = MiPersonaje->GetCharacterMovement())
		{
			Movimiento->MaxWalkSpeed = VelocidadActual;
		}
	}

	OnRep_VidaActual();
}

void UComponenteEstadisticas::Server_Revivir_Implementation()
{
	VidaActual = VidaMaximaActual;
	bEstaMuerto = false;
	OnRep_VidaActual();
	
	UE_LOG(LogTemp, Warning, TEXT("Server_Revivir EJECUTADO. Owner=%s VidaActual=%.1f VidaMax=%.1f"),
		*GetNameSafe(GetOwner()), VidaActual, VidaMaximaActual);
}

// Called when the game starts
void UComponenteEstadisticas::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner()->HasAuthority())
	{
		VidaMaximaActual = VidaBase;
		VidaActual = VidaMaximaActual;
		AtaqueActual = AtaqueBase;
		VelocidadActual = VelocidadBase;
		DefensaActual = DefensaBase;
	}
	// ...
	
}


void UComponenteEstadisticas::Server_RecibirDanio_Implementation(float Cantidad)
{
	UE_LOG(LogTemp, Warning, TEXT("Server_RecibirDanio EJECUTADO. Owner=%s Cantidad=%.1f"),
	 *GetNameSafe(GetOwner()), Cantidad);
	
	if (bEstaMuerto) return;
	float MultiplicadorDanio = 100.f / (100.f + DefensaActual);
	float DanioFinal = Cantidad * MultiplicadorDanio;

	VidaActual = FMath::Clamp(VidaActual - DanioFinal, 0.f, VidaMaximaActual);
	OnRep_VidaActual(); 

	if (VidaActual <= 0.f && !bEstaMuerto)
	{
		bEstaMuerto = true;
		OnRep_EstaMuerto(); 
	}
}

void UComponenteEstadisticas::OnRep_VidaActual()
{
	OnVidaCambia.Broadcast(VidaActual, VidaMaximaActual);
	UE_LOG(LogTemp, Warning, TEXT("[%s] OnRep_VidaActual. Vida=%.1f"),
		   GetOwner()->HasAuthority() ? TEXT("SERVER") : TEXT("CLIENT"), VidaActual);
}

void UComponenteEstadisticas::OnRep_VelocidadActual()
{
	if (ACharacter* MiPersonaje = Cast<ACharacter>(GetOwner()))
	{
		if (UCharacterMovementComponent* Movimiento = MiPersonaje->GetCharacterMovement())
		{
			Movimiento->MaxWalkSpeed = VelocidadActual;
		}
	}
}

void UComponenteEstadisticas::OnRep_EstaMuerto()
{
	UE_LOG(LogTemp, Warning, TEXT("[%s] OnRep_EstaMuerto ejecutado. bEstaMuerto=%d"),
	   GetOwner()->HasAuthority() ? TEXT("SERVER") : TEXT("CLIENT"), bEstaMuerto);
	if (bEstaMuerto)
	{
		OnPersonajeMuere.Broadcast();
	}
}

// Called every frame
void UComponenteEstadisticas::TickComponent(float DeltaTime, ELevelTick TickType,
                                            FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

