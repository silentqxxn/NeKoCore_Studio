// Fill out your copyright notice in the Description page of Project Settings.


#include "componentes/ComponenteArmas.h"
#include "Actores/WeaponMaster.h"
#include "GameFramework/Character.h"
#include "componentes/ComponenteEstadisticas.h"
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

	if (UComponenteEstadisticas* Stats = GetOwner()->FindComponentByClass<UComponenteEstadisticas>())
	{
		Stats->AtaqueActual += Arma->DanioBase;
       
		if (GetOwner()->HasAuthority())
		{
			Stats->OnRep_AtaqueActual(); 
		}
	}
	if (GetOwner()) GetOwner()->ForceNetUpdate();

}

void UComponenteArmas::Server_SoltarEquipada_Implementation(EEquipableSlot Slot)
{
	if (Slot == EEquipableSlot::Ninguno) return;

	AWeaponMaster*& SlotEquipado = GetSlotEquipado(Slot);
	if (!SlotEquipado) return;

	ACharacter* Owner = Cast<ACharacter>(GetOwner());
	if (!Owner) return;

	AWeaponMaster* ArmaASoltar = SlotEquipado;

	if (UComponenteEstadisticas* Stats = Owner->FindComponentByClass<UComponenteEstadisticas>())
	{
		Stats->AtaqueActual -= ArmaASoltar->DanioBase;

		if (Owner->HasAuthority())
		{
			Stats->OnRep_AtaqueActual();
		}
	}

	ArmaASoltar->bEquipado = false;
	ArmaASoltar->SetOwner(nullptr);

	// Desacoplar antes de reposicionar
	ArmaASoltar->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

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

	// Bug 1 fix: llamar el OnRep correcto según el slot
	switch (Slot)
	{
	case EEquipableSlot::ManoIzquierda: OnRep_ArmaManoIzquierda(ArmaASoltar); break;
	case EEquipableSlot::Espalda:       OnRep_ArmaEspalda(ArmaASoltar);       break;
	default:                            OnRep_ArmaManoDerecha(ArmaASoltar);    break;
	}

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

void UComponenteArmas::OnRep_ArmaManoDerecha(AWeaponMaster* ArmaVieja)
{
	
	ACharacter* OwnerChar = Cast<ACharacter>(GetOwner());
	if (!OwnerChar) return;

	if (ArmaVieja && ArmaVieja != ArmaManoDerecha)
	{
		SetupArmaDropeada(ArmaVieja);
		ArmaVieja->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	}

	if (ArmaManoDerecha)
	{
		SetupArmaAdjuntada(ArmaManoDerecha);
		FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, true);
		ArmaManoDerecha->AttachToComponent(OwnerChar->GetMesh(), Rules, ArmaManoDerecha->SocketName);
	}
}
void UComponenteArmas::OnRep_ArmaManoIzquierda(AWeaponMaster* ArmaVieja)
{
	ACharacter* OwnerChar = Cast<ACharacter>(GetOwner());
	if (!OwnerChar) return;

	if (ArmaVieja && ArmaVieja != ArmaManoIzquierda)
	{
		SetupArmaDropeada(ArmaVieja);
		ArmaVieja->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	}

	if (ArmaManoIzquierda)
	{
		SetupArmaAdjuntada(ArmaManoIzquierda);
		FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, true);
		ArmaManoIzquierda->AttachToComponent(OwnerChar->GetMesh(), Rules, ArmaManoIzquierda->SocketName);
	}
}

void UComponenteArmas::OnRep_ArmaEspalda(AWeaponMaster* ArmaVieja)
{
	ACharacter* OwnerChar = Cast<ACharacter>(GetOwner());
	if (!OwnerChar) return;

	if (ArmaVieja && ArmaVieja != ArmaEspalda)
	{
		SetupArmaDropeada(ArmaVieja);
		ArmaVieja->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	}

	if (ArmaEspalda)
	{
		SetupArmaAdjuntada(ArmaEspalda);
		FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, true);
		ArmaEspalda->AttachToComponent(OwnerChar->GetMesh(), Rules, ArmaEspalda->SocketName);
	}

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

void UComponenteArmas::SetupArmaAdjuntada(AWeaponMaster* Arma)
{
	if (!Arma) return;
	Arma->SetReplicatingMovement(false);
	Arma->ItemMesh->SetSimulatePhysics(false);
	Arma->ItemMesh->SetEnableGravity(false);
	Arma->ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Arma->SphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void UComponenteArmas::SetupArmaDropeada(AWeaponMaster* Arma)
{
	if (!Arma) return;
	Arma->SetReplicatingMovement(true);
	Arma->ItemMesh->SetSimulatePhysics(false);
	Arma->ItemMesh->SetEnableGravity(false);
	Arma->ItemMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Arma->ItemMesh->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
	Arma->ItemMesh->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Ignore);
	Arma->SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}


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

	//Multicast_AttachVisual(Arma, Socket);

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

