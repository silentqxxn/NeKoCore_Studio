// Fill out your copyright notice in the Description page of Project Settings.
#include "Actores/WeaponMaster.h"
#include "Actores/Characterprincipal.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"

AWeaponMaster::AWeaponMaster()
{
	bReplicates = true;
	SetReplicatingMovement(true);

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	SetRootComponent(WeaponMesh);
	WeaponMesh->SetSimulatePhysics(false);
	WeaponMesh->SetEnableGravity(false);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	PickupCollision = CreateDefaultSubobject<USphereComponent>("PickupCollision");
	PickupCollision->SetupAttachment(WeaponMesh);
	PickupCollision->SetSphereRadius(80.f);
	PickupCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AWeaponMaster::BeginPlay()
{
	Super::BeginPlay();
}

void AWeaponMaster::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (!HasAuthority() || !OtherActor) return;

	ACharacterprincipal* Character = Cast<ACharacterprincipal>(OtherActor);
	if (!Character) return;


	PickupCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Character->Server_EquipWeapon(this);
}

void AWeaponMaster::EnablePickup()
{
	PickupCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	FVector Loc = GetActorLocation();
	SetReplicatingMovement(true);
	SetActorLocation(FVector(Loc.X, Loc.Y, Loc.Z - 50.f));
}

void AWeaponMaster::DisablePickup()
{
	PickupCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetReplicatingMovement(false);
}

void AWeaponMaster::EnablePickupDelayed(float Delay)
{
	GetWorldTimerManager().SetTimer(PickupTimerHandle,this,&AWeaponMaster::EnablePickup,Delay,false);
}