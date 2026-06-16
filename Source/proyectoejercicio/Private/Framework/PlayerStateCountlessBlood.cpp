// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/PlayerStateCountlessBlood.h"
#include "componentes/ComponenteMisiones.h"
#include "Net/UnrealNetwork.h"

APlayerStateCountlessBlood::APlayerStateCountlessBlood()
{
	bReplicates = true;
	SangreTotal = 0;
	ComponenteMisiones = CreateDefaultSubobject<UComponenteMisiones>(TEXT("ComponenteMisiones"));
    
	if (ComponenteMisiones)
	{
		ComponenteMisiones->SetIsReplicated(true);
	}
}

void APlayerStateCountlessBlood::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerStateCountlessBlood, SangreTotal);
}

void APlayerStateCountlessBlood::OnRep_Sangre()
{
}