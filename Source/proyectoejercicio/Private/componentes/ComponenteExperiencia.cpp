// Fill out your copyright notice in the Description page of Project Settings.


#include "componentes/ComponenteExperiencia.h"

#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
UComponenteExperiencia::UComponenteExperiencia()
{
	
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);

}

void UComponenteExperiencia::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UComponenteExperiencia, Datos);
}


// Called when the game starts
void UComponenteExperiencia::BeginPlay()
{
	Super::BeginPlay();

	
	Datos.XPParaSiguienteNivel = CalcularXPRequerida(1);

}


// Called every frame
void UComponenteExperiencia::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
}

void UComponenteExperiencia::Server_AgregarXP_Implementation(int32 Cantidad)
{
	if (Cantidad <= 0) return;
	FExperienciaData OldDatos = Datos;
	
	Datos.XPActual += Cantidad;
	ProcesarSubidaNivel();
	
	OnRep_Datos(OldDatos);
}

void UComponenteExperiencia::ProcesarSubidaNivel()
{
	while (Datos.XPActual >= Datos.XPParaSiguienteNivel)
	{
		Datos.XPActual -= Datos.XPParaSiguienteNivel;
		Datos.NivelActual++;
		Datos.XPParaSiguienteNivel = CalcularXPRequerida(Datos.NivelActual);

	}
}

int32 UComponenteExperiencia::CalcularXPRequerida(int32 Nivel) const
{
	
	return 100 * Nivel + 50 * (Nivel * Nivel);
}

float UComponenteExperiencia::GetPorcentajeXP() const
{
	if (Datos.XPParaSiguienteNivel <= 0) return 1.f;
	return static_cast<float>(Datos.XPActual) /
		   static_cast<float>(Datos.XPParaSiguienteNivel);
}

void UComponenteExperiencia::OnRep_Datos(FExperienciaData OldDatos)
{
	OnXPCambia.Broadcast(Datos.XPActual, Datos.XPParaSiguienteNivel);
	if (Datos.NivelActual > OldDatos.NivelActual)
	{
		OnSubeNivel.Broadcast(OldDatos.NivelActual, Datos.NivelActual);
	}
	
}