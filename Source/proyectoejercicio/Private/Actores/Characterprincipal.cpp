// Fill out your copyright notice in the Description page of Project Settings.

#include "Actores/Characterprincipal.h"
#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Character.h"
#include "interfaz/interfazparahacerdanio.h"
#include "Kismet/GameplayStatics.h"              
#include "Components/SkeletalMeshComponent.h"    
#include "interfaz/InterfazAttach.h"             
#include "InputActionValue.h"
#include "Actores/ItemMasterAttach.h"
#include "Actores/ItemRecogible.h"
#include "Actores/WeaponMaster.h"
#include "componentes/ComponenteCrafteo.h"
#include "componentes/ComponenteExperiencia.h"
#include "componentes/ComponenteInventario.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "Actores/FogataNueva.h"
#include "Framework/PlayerControllerNuevo.h"


//class UEnhancedInputLocalPlayerSubsystem;

ACharacterprincipal::ACharacterprincipal()
{
	bReplicates = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = 400.f;
	GetCharacterMovement()->JumpZVelocity = 250.f;
	GetCharacterMovement()->GravityScale = 2.2f;
	JumpMaxCount = 1;
	
	CompInventario = CreateDefaultSubobject<UComponenteInventario>(TEXT("CompInventario"));
	CompExperiencia = CreateDefaultSubobject<UComponenteExperiencia>(TEXT("CompExperiencia"));
	
	CompCrafteo = CreateDefaultSubobject<UComponenteCrafteo>(TEXT("CompCrafteo"));

	CompArmas = CreateDefaultSubobject<UComponenteArmas>(TEXT("CompArmas"));
}

void ACharacterprincipal::OnRep_CurrentItemAttach()
{
	if (CurrentItemAttach)
	{
		// Los clientes replican la acción visual
		CurrentItemAttach->ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		CurrentItemAttach->SphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		CurrentItemAttach->SetReplicatingMovement(false);
		CurrentItemAttach->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, CurrentItemAttach->SocketName);
	}
	OnEquipoActualizado.Broadcast();

}

void ACharacterprincipal::Server_EquipItemGeneric_Implementation(AItemMasterAttach* NuevoItem)
{
	if (!NuevoItem || NuevoItem->bEquipado) return;
	if (FVector::Dist(GetActorLocation(), NuevoItem->GetActorLocation()) > 300.f) return;

	if (NuevoItem->SocketName == "ManoDerechaSocket")
	{
		DesequiparItemAttach(ItemManoDerecha);   
		ItemManoDerecha = NuevoItem;
	}
	else if (NuevoItem->SocketName == "ManoIzquierdaSocket")
	{
		DesequiparItemAttach(ItemManoIzquierda);
		ItemManoIzquierda = NuevoItem;
	}

	NuevoItem->SetReplicatingMovement(false);
	NuevoItem->ItemMesh->SetSimulatePhysics(false);
	NuevoItem->ItemMesh->SetEnableGravity(false);
	NuevoItem->ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	NuevoItem->SphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	FAttachmentTransformRules Rules(
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::KeepWorld,   
		true);
	NuevoItem->AttachToComponent(GetMesh(), Rules, NuevoItem->SocketName);

	NuevoItem->OwningCharacter = this;
	NuevoItem->bEquipado = true;
	NuevoItem->SetOwner(this);

	ForceNetUpdate();
	
}



void ACharacterprincipal::BeginPlay()
{
	Super::BeginPlay();
/*
	if (APlayerController* PC = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Sub =ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Sub->AddMappingContext(IMC_Player, 0);
		}
	}*/
}

void ACharacterprincipal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACharacterprincipal::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//DOREPLIFETIME(ACharacterprincipal, CurrentWeapon);
	
}


void ACharacterprincipal::OnRep_CurrentWeapon()
{
	
	OnEquipoActualizado.Broadcast();
}




USkeletalMeshComponent* ACharacterprincipal::GetSkeletalMesh_Implementation()
{
	return GetMesh();
}

// ── IInterfazRecogeItems ──────────────────────────────────────────
bool ACharacterprincipal::RecogerItem_Implementation(const FItemData& Item)
{
	return CompInventario?CompInventario->RecogerItem_Implementation(Item): false;
}


bool ACharacterprincipal::RecogerTodos_Implementation(const TArray<FItemData>& Items)
{
	return CompInventario? CompInventario->RecogerTodos_Implementation(Items): false;
}

bool ACharacterprincipal::PuedeRecoger_Implementation(const FItemData& Item) const
{
	return CompInventario? CompInventario->PuedeRecoger_Implementation(Item): false;
}

//Armas
void ACharacterprincipal::DropCurrentWeapon()
{

}

void ACharacterprincipal::DesequiparItemAttach(AItemMasterAttach* Item)
{
	if (!Item) return;

	Item->OwningCharacter = nullptr;
	Item->bEquipado = false;
	Item->SetOwner(nullptr);

	FVector DropLocation = GetActorLocation() + GetActorForwardVector() * 120.f;
	Item->SetActorLocation(DropLocation);
	Item->EnablePickupDelayed(1.0f);

	// El servidor también debe ejecutar el detach + restaurar física
	// (OnRep solo corre automático en clientes)
	Item->OnRep_OwningCharacter();
}




// ── IInterfazCrafteo ──────────────────────────────────────────────
bool ACharacterprincipal::CraftearItem_Implementation(FName RecetaID)
{
	return CompInventario? CompInventario->CraftearItem_Implementation(RecetaID): false;
}

bool ACharacterprincipal::PuedeCraftear_Implementation(FName RecetaID) const
{
	return CompInventario? CompInventario->PuedeCraftear_Implementation(RecetaID): false;
}

TArray<FName> ACharacterprincipal::ObtenerRecetasDisponibles_Implementation() const
{
	return CompInventario? CompInventario->ObtenerRecetasDisponibles_Implementation(): TArray<FName>();
}


//Input
void ACharacterprincipal::SetupPlayerInputComponent(
	UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EI =
		Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (IA_Movimiento)
			EI->BindAction(IA_Movimiento, ETriggerEvent::Triggered,this, &ACharacterprincipal::Move);

		if (IA_Interactuar)
			EI->BindAction(IA_Interactuar, ETriggerEvent::Started,this, &ACharacterprincipal::TryInteract);
	}
}

void ACharacterprincipal::Move(const FInputActionValue& Value)
{
	if (bMovimientoBloqueado) return;
	
	const FVector2D InputVec = Value.Get<FVector2D>();
	if (!Controller) return;

	if (InputVec.X != 0.f)
		AddControllerYawInput(InputVec.X);

	if (InputVec.Y != 0.f)
	{
		const FRotator Yaw(0, Controller->GetControlRotation().Yaw, 0);
		const FVector Forward =
			FRotationMatrix(Yaw).GetUnitAxis(EAxis::X);
		AddMovementInput(Forward, InputVec.Y);
	}
}

void ACharacterprincipal::OnRep_ItemManoIzquierda()
{
	OnEquipoActualizado.Broadcast();
}

void ACharacterprincipal::SetItemInteractuable(AActor* Item)
{
	ItemInteractuableCercano = Item;

}

void ACharacterprincipal::ClearItemInteractuable(AActor* Item)
{
	if (ItemInteractuableCercano == Item)
		ItemInteractuableCercano = nullptr;
}

void ACharacterprincipal::TryInteract()
{
	/*TArray<AActor*> Overlapping;
	GetOverlappingActors(Overlapping, AActor::StaticClass());
	
	bool bInteractuoConAlgo = false;
	
	for (AActor* Actor : Overlapping)
	{
		if (AWeaponMaster* Weapon = Cast<AWeaponMaster>(Actor))
		{
			if (Weapon != CurrentWeapon)
			{
				Server_EquipWeapon(Weapon);
				bInteractuoConAlgo = true;
				break;
			}
		}
		else if (AItemMasterAttach* ItemAttach = Cast<AItemMasterAttach>(Actor))
		{
			ItemAttach->EquiparItem(this);
			bInteractuoConAlgo = true;
			break;
		}
		else if (AItemRecogible* Item = Cast<AItemRecogible>(Actor))
		{
			Item->Interactuar(this);
			bInteractuoConAlgo = true;
			break;
		}
	
		*/
		/*
		else if (AActorMision* Mision = Cast<AActorMision>(Actor)) // Reemplazá por tu clase
		{
			Mision->Interactuar(); // O la función que uses para aceptar la misión
			bInteractuoConAlgo = true;
			break;
		}
		*/
	

	/*if (!bInteractuoConAlgo && CurrentWeapon)
	{
		Server_DropWeapon();
	}*/
	

	
	if (CompCrafteo && CompCrafteo->GetFogataCercana())
	{
		if (APlayerControllerNuevo* PC = Cast<APlayerControllerNuevo>(GetController()))
			PC->ToggleCrafteo();
		return;
	}
	
	if (ItemInteractuableCercano)
	{
		if (AWeaponMaster* Arma = Cast<AWeaponMaster>(ItemInteractuableCercano))
		{
			if (CompArmas) 
			{
				CompArmas->Server_RecogerArma(Arma);
			}
		}
		else if (ItemInteractuableCercano->Implements<UInterfazAttach>())
		{
			IInterfazAttach::Execute_Equipar(ItemInteractuableCercano, this);
		}
		return;
	}
	
}



void ACharacterprincipal::MostrarMensaje()
{
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,TEXT("Overlapeando"));
}

void ACharacterprincipal::SetMovimientoBloqueado(bool bBloqueado)
{
	bMovimientoBloqueado = bBloqueado;

	if (bBloqueado)
		GetCharacterMovement()->StopMovementImmediately();
}

void ACharacterprincipal::Server_EquipItemAttach_Implementation(AItemMasterAttach* Item)
{
	if (!Item) return;

	Item->ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Item->SphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Item->SetReplicatingMovement(false);

	Item->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, Item->SocketName);
	
	CurrentItemAttach = Item;
	Item->SetOwner(this);
}
