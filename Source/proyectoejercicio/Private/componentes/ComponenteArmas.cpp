// Fill out your copyright notice in the Description page of Project Settings.


#include "componentes/ComponenteArmas.h"
#include "Actores/WeaponMaster.h"
#include "GameFramework/Character.h"
#include "EWeaponType.h"
#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
UComponenteArmas::UComponenteArmas()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UComponenteArmas::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UComponenteArmas, ArmaManoDerecha);
	DOREPLIFETIME(UComponenteArmas, ArmaManoIzquierda);
	DOREPLIFETIME(UComponenteArmas, ArmaEspalda);
	/*DOREPLIFETIME(UComponenteArmas, ArmaManoDerechaGuardada);
	DOREPLIFETIME(UComponenteArmas, ArmaManoIzquierdaGuardada);
	DOREPLIFETIME(UComponenteArmas, ArmaEspaldaGuardada);
*/
}

void UComponenteArmas::Server_RecogerArma_Implementation(AWeaponMaster* Arma)
{
	if (!Arma || Arma->bEquipado) return;
	if (Arma->SlotEquipable == EEquipableSlot::Ninguno) return;

	AWeaponMaster*& SlotEquipado = GetSlotEquipado(Arma->SlotEquipable);

	if (SlotEquipado)
	{
		Server_SoltarEquipada_Implementation(Arma->SlotEquipable); 
	}

	AttacharArma(Arma, Arma->SocketName);
	SlotEquipado = Arma;

	if (GetOwner()) GetOwner()->ForceNetUpdate();

}
/*
void UComponenteArmas::Server_IntercambiarArma_Implementation(EEquipableSlot Slot)
{
	if (Slot == EEquipableSlot::Ninguno) return;

	AWeaponMaster*& SlotEquipado = GetSlotEquipado(Slot);
	//AWeaponMaster*& SlotGuardado = GetSlotGuardado(Slot);

	if (!SlotGuardado) return;

	AWeaponMaster* Anterior = SlotEquipado;

	AttacharArma(SlotGuardado, SlotGuardado->SocketName);
	SlotEquipado = SlotGuardado;

	if (Anterior)
	{
		AttacharArma(Anterior, Anterior->SocketGuardado);
		SlotGuardado = Anterior;
	}
	else
	{
		SlotGuardado = nullptr;
	}

	if (GetOwner()) GetOwner()->ForceNetUpdate();
}
*/
void UComponenteArmas::Server_SoltarEquipada_Implementation(EEquipableSlot Slot)
{
	if (Slot == EEquipableSlot::Ninguno) return;

	AWeaponMaster*& SlotEquipado = GetSlotEquipado(Slot);
	if (!SlotEquipado) return;

	ACharacter* Owner = Cast<ACharacter>(GetOwner());
	if (!Owner) return;

	AWeaponMaster* ArmaASoltar = SlotEquipado;

	ArmaASoltar->bEquipado = false;
	ArmaASoltar->SetOwner(nullptr);

	Multicast_DetachVisual(ArmaASoltar);

	FVector DropLocation = Owner->GetActorLocation() + Owner->GetActorForwardVector() * 120.f;
	ArmaASoltar->SetActorLocation(DropLocation);
	ArmaASoltar->SetReplicatingMovement(true);
    
	ArmaASoltar->ItemMesh->SetSimulatePhysics(false);
	ArmaASoltar->ItemMesh->SetEnableGravity(false);
	ArmaASoltar->ItemMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);    
	ArmaASoltar->ItemMesh->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore); 
	ArmaASoltar->ItemMesh->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Ignore);
    
	ArmaASoltar->SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ArmaASoltar->EnablePickupDelayed(1.0f);

	SlotEquipado = nullptr;

	Owner->ForceNetUpdate();
}


void UComponenteArmas::Multicast_DetachVisual_Implementation(AWeaponMaster* Arma)
{
	if (!Arma) return;
    
	Arma->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}

void UComponenteArmas::Multicast_AttachVisual_Implementation(AWeaponMaster* Arma, FName Socket)
{
	if (!Arma) return;
	ACharacter* OwnerChar = Cast<ACharacter>(GetOwner());
	if (!OwnerChar) return;

	Arma->ItemMesh->SetSimulatePhysics(false);
	Arma->ItemMesh->SetEnableGravity(false);
	Arma->ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Arma->SphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    
	Arma->SetReplicatingMovement(false);

	FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, true);
	Arma->AttachToComponent(OwnerChar->GetMesh(), Rules, Socket);
}

AWeaponMaster*& UComponenteArmas::GetSlotEquipado(EEquipableSlot Slot)
{
	switch (Slot)
	{
	case EEquipableSlot::ManoIzquierda: return ArmaManoIzquierda;
	case EEquipableSlot::Espalda:       return ArmaEspalda;
	default:                            return ArmaManoDerecha;
	}
}
/*
AWeaponMaster*& UComponenteArmas::GetSlotGuardado(EEquipableSlot Slot)
{
	switch (Slot)
	{
	case EEquipableSlot::ManoIzquierda: return ArmaManoIzquierdaGuardada;
	case EEquipableSlot::Espalda:       return ArmaEspaldaGuardada;
	default:                            return ArmaManoDerechaGuardada;
	}
}*/

void UComponenteArmas::AttacharArma(AWeaponMaster* Arma, FName Socket)
{
	ACharacter* Owner = Cast<ACharacter>(GetOwner());
	if (!Arma || !Owner) return;

	Arma->bEquipado = true;
	Arma->SetOwner(Owner);
	Arma->SetReplicatingMovement(false); 

	Arma->ItemMesh->SetSimulatePhysics(false);
	Arma->ItemMesh->SetEnableGravity(false);
	Arma->ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Arma->SphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, true);
	Arma->AttachToComponent(Owner->GetMesh(), Rules, Socket);

	Multicast_AttachVisual(Arma, Socket);

}

// Called when the game starts
void UComponenteArmas::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UComponenteArmas::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

