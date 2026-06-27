// Fill out your copyright notice in the Description page of Project Settings.


#include "Actores/ItemMasterAttach.h"

#include "Actores/Characterprincipal.h"
#include "interfaz/InterfazAttach.h"
#include "Components/SphereComponent.h"    
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AItemMasterAttach::AItemMasterAttach()
{
    PrimaryActorTick.bCanEverTick = false; 

    bReplicates = true;
    SetReplicatingMovement(true);

    ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>("ItemMesh");
    SetRootComponent(ItemMesh);
    
    ItemMesh->SetSimulatePhysics(false); 
    ItemMesh->SetEnableGravity(false);    
    ItemMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly); 
    ItemMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

    SphereCollision = CreateDefaultSubobject<USphereComponent>("SphereCollision");
    SphereCollision->SetupAttachment(ItemMesh);
    SphereCollision->SetSphereRadius(100.f);
    SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    SphereCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
    SphereCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void AItemMasterAttach::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(AItemMasterAttach, OwningCharacter);
    DOREPLIFETIME(AItemMasterAttach, bEquipado);
}

void AItemMasterAttach::BeginPlay()
{
    Super::BeginPlay();
}

void AItemMasterAttach::Equipar_Implementation(AActor* Interactor)
{
    ACharacterprincipal* Character = Cast<ACharacterprincipal>(Interactor);
    if (!Character) return;

    Character->Server_EquipItemGeneric(this);
}

void AItemMasterAttach::NotifyActorBeginOverlap(AActor* OtherActor)
{
    Super::NotifyActorBeginOverlap(OtherActor);

    if (bEquipado || !OtherActor) return;

    if (ACharacterprincipal* Character = Cast<ACharacterprincipal>(OtherActor))
        Character->SetItemInteractuable(this);
}

void AItemMasterAttach::NotifyActorEndOverlap(AActor* OtherActor)
{
    Super::NotifyActorEndOverlap(OtherActor);
    if (!OtherActor) return;

    if (ACharacterprincipal* Character = Cast<ACharacterprincipal>(OtherActor))
        Character->ClearItemInteractuable(this);
}



void AItemMasterAttach::OnRep_OwningCharacter()
{
    if (OwningCharacter)
    {
       SetReplicatingMovement(false);
       ItemMesh->SetSimulatePhysics(false);
       ItemMesh->SetEnableGravity(false);
       ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
       SphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

        FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget,EAttachmentRule::SnapToTarget,EAttachmentRule::KeepWorld,true
);       
        AttachToComponent(OwningCharacter->GetMesh(), Rules, SocketName);
    }
    else
    {
        DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        ItemMesh->SetSimulatePhysics(false);
        ItemMesh->SetEnableGravity(false);
        ItemMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
        SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
        SetReplicatingMovement(true);
    }
}

void AItemMasterAttach::EnablePickup()
{
    bEquipado = false;
    SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    ItemMesh->SetSimulatePhysics(false);
    ItemMesh->SetEnableGravity(false);
    ItemMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    SetReplicatingMovement(true);
}

void AItemMasterAttach::DisablePickup()
{
}

void AItemMasterAttach::EnablePickupDelayed(float Delay)
{
    GetWorldTimerManager().SetTimer(PickupTimerHandle, this, &AItemMasterAttach::EnablePickup, Delay, false);
}

