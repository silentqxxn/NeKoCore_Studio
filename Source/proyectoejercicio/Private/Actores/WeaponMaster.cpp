// Fill out your copyright notice in the Description page of Project Settings.
#include "Actores/WeaponMaster.h"
#include "Actores/Characterprincipal.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Net/UnrealNetwork.h"

AWeaponMaster::AWeaponMaster()
{
	
}

void AWeaponMaster::BeginPlay()
{
	Super::BeginPlay();
}

void AWeaponMaster::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	if (bEquipado) return;
	if (!OtherActor) return;
	if (ACharacterprincipal* Character = Cast<ACharacterprincipal>(OtherActor))
		Character->SetItemInteractuable(this);
}

void AWeaponMaster::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);
	if (!OtherActor) return;

	if (ACharacterprincipal* Character = Cast<ACharacterprincipal>(OtherActor))
		Character->ClearItemInteractuable(this);
}


