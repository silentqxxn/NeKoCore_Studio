// Fill out your copyright notice in the Description page of Project Settings.

#include "Actores/Characterprincipal.h"
#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Character.h"
#include "interfaz/interfazparahacerdanio.h"
#include "Kismet/GameplayStatics.h"      
#include "componentes/ComponenteEstadisticas.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SkeletalMeshComponent.h"    
#include "interfaz/InterfazAttach.h"             
#include "InputActionValue.h"
#include "NiagaraComponent.h"
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
	CompEstadisticas = CreateDefaultSubobject<UComponenteEstadisticas>(TEXT("ComponenteEstadisticasNat"));	
	
	IndicadorRanged = CreateDefaultSubobject<UDecalComponent>(TEXT("IndicadorRanged"));
    
	IndicadorRanged->SetupAttachment(GetMesh());

	IndicadorRanged->SetRelativeLocationAndRotation(FVector(100.f, 0.f, 0.f), FRotator(-90.f, 0.f, 0.f));

	IndicadorRanged->SetVisibility(false);
	
	EfectoCorrerNiagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("EfectoCorrerNiagara"));
	EfectoCorrerNiagara->SetupAttachment(GetMesh());
	EfectoCorrerNiagara->bAutoActivate = false;
	
	TargetFOV = FOVNormal;

}

void ACharacterprincipal::OnRep_CurrentItemAttach()
{
	if (CurrentItemAttach)
	{
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
	UE_LOG(LogTemp, Warning, TEXT("[%s] BeginPlay - Owner actual: %s | Controller: %s"),
		HasAuthority() ? TEXT("SERVER") : TEXT("CLIENT"),
		*GetNameSafe(GetOwner()),
		*GetNameSafe(GetController()));
	
	if (USpringArmComponent* SpringArm = FindComponentByClass<USpringArmComponent>())
	{
		DistanciaNormal = SpringArm->TargetArmLength;
		DistanciaSalto = DistanciaNormal + 150.0f;
	}

	TargetFOV = FOVNormal;
	TargetDistancia = DistanciaNormal;

}

void ACharacterprincipal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
		
}


void ACharacterprincipal::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
}


void ACharacterprincipal::OnRep_CurrentWeapon()
{
	
	OnEquipoActualizado.Broadcast();
}




USkeletalMeshComponent* ACharacterprincipal::GetSkeletalMesh_Implementation()
{
	return GetMesh();
}

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


void ACharacterprincipal::Server_SetEfectoCorrer_Implementation(bool bActivo)
{
	Multicast_SetEfectoCorrer(bActivo);
}

void ACharacterprincipal::Multicast_SetEfectoCorrer_Implementation(bool bActivo)
{
	if (EfectoCorrerNiagara)
	{
		if (bActivo)
		{
			EfectoCorrerNiagara->Activate(true); 
		}
		else
		{
			EfectoCorrerNiagara->Deactivate();
		}
	}
}

void ACharacterprincipal::IniciarPunteriaRanged()
{
	if (CompArmas && CompArmas->ArmaEspalda && IndicadorRanged)
	{
		if (MaterialFlechaPunteria && !IndicadorRanged->GetDecalMaterial())
		{
			IndicadorRanged->SetDecalMaterial(MaterialFlechaPunteria);
		}
		IndicadorRanged->SetVisibility(true);
	}
}

void ACharacterprincipal::IntentarSaltar()
{
	if (!GetCharacterMovement()) return;

	bool bEnElSuelo = GetCharacterMovement()->IsMovingOnGround();

	if (bEnElSuelo || bCoyoteTimeActivo)
	{
		SaltosRealizados = 1; 
		bCoyoteTimeActivo = false;
		GetWorldTimerManager().ClearTimer(TimerCoyote);
        
		Jump();
	}
	else if (SaltosRealizados < MaximoDeSaltos)
	{
		SaltosRealizados++;
        
		FVector VelocidadActual = GetCharacterMovement()->Velocity;
		VelocidadActual.Z = GetCharacterMovement()->JumpZVelocity;
		GetCharacterMovement()->Velocity = VelocidadActual;
        
		Jump();
	}
	TargetDistancia = DistanciaSalto;
	TargetFOV = FOVSalto;
	GetWorldTimerManager().SetTimer(TimerTransicionCamara, this, &ACharacterprincipal::ActualizarCamara, 0.01f, true);
}

void ACharacterprincipal::IntentarDetenerSalto()
{
	StopJumping();

}

void ACharacterprincipal::FinalizarCoyoteTime()
{
	bCoyoteTimeActivo = false;
	if (SaltosRealizados == 0)
	{
		SaltosRealizados = 1; 
	}
}


void ACharacterprincipal::Server_EjecutarDash_Implementation(FVector FuerzaEmpuje)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->GroundFriction = 0.f;
		LaunchCharacter(FuerzaEmpuje, true, false);
		GetWorldTimerManager().SetTimer(TimerDuracionDash, this, &ACharacterprincipal::FinalizarDash, DuracionDash, false);
	}
}

void ACharacterprincipal::FinalizarDash()
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->GroundFriction = FriccionOriginal > 0.f ? FriccionOriginal : 8.0f;

		FVector VelocidadFrenada = GetCharacterMovement()->Velocity * 0.1f; 
		VelocidadFrenada.Z = GetCharacterMovement()->Velocity.Z;         
		GetCharacterMovement()->Velocity = VelocidadFrenada;
		TargetFOV = GetCharacterMovement()->IsFalling() ? FOVSalto : FOVNormal;
		GetWorldTimerManager().SetTimer(TimerTransicionCamara, this, &ACharacterprincipal::ActualizarCamara, 0.01f, true);
	}
}

void ACharacterprincipal::EjecutarDash()
{
	if (!bPuedeDashear || bMovimientoBloqueado || !GetCharacterMovement() || !GetCharacterMovement()->IsMovingOnGround()) return;
	bPuedeDashear = false;

	FVector DireccionDash = GetCharacterMovement()->GetLastInputVector();
	if (DireccionDash.IsNearlyZero())
	{
		DireccionDash = GetActorForwardVector();
	}
	DireccionDash.Normalize();

	FVector EmpujeDash = DireccionDash * FuerzaDash;
	EmpujeDash.Z = 0.f; 

	FriccionOriginal = GetCharacterMovement()->GroundFriction;
	GetCharacterMovement()->GroundFriction = 0.f;

	LaunchCharacter(EmpujeDash, true, false);
	Server_EjecutarDash(EmpujeDash);
	TargetFOV = FOVDash;
	GetWorldTimerManager().SetTimer(TimerTransicionCamara, this, &ACharacterprincipal::ActualizarCamara, 0.01f, true);
	
	GetWorldTimerManager().SetTimer(TimerDuracionDash, this, &ACharacterprincipal::FinalizarDash, DuracionDash, false);
	GetWorldTimerManager().SetTimer(TimerCooldownDash, this, &ACharacterprincipal::ResetearDash, CooldownDash, false);
}

void ACharacterprincipal::ResetearDash()
{
	bPuedeDashear = true;
}

void ACharacterprincipal::PresionTeclaCuracion()
{
	FName IDPocion = TEXT("PocionDeVida");
	float PoderCuracion = 30.f;

	if (this->Implements<UInterfazConsumible>())
	{
		IInterfazConsumible::Execute_ConsumirItem(this, IDPocion, PoderCuracion);
	}
}

bool ACharacterprincipal::ConsumirItem_Implementation(FName ItemID, float CantidadEfecto)
{
	if (!CompInventario || !CompEstadisticas) return false;

	FItemData DatosItem;
	if (CompInventario->QueryInventory(ItemID, DatosItem) && DatosItem.Cantidad > 0)
	{
		if (CompEstadisticas->VidaActual < CompEstadisticas->VidaMaximaActual && !CompEstadisticas->bEstaMuerto)
		{
			Server_ProcesarConsumoInterface(ItemID, CantidadEfecto);
			return true;
		}
	}
	return false;
}

void ACharacterprincipal::Server_ProcesarConsumoInterface_Implementation(FName ItemID, float CantidadEfecto)
{
	if (!CompInventario || !CompEstadisticas) return;

	FItemData DatosItem;
	if (CompInventario->QueryInventory(ItemID, DatosItem) && DatosItem.Cantidad > 0)
	{
		CompInventario->Server_QuitarItem_Implementation(ItemID, 1);

		float NuevaVida = FMath::Clamp(CompEstadisticas->VidaActual + CantidadEfecto, 0.f, CompEstadisticas->VidaMaximaActual);
		CompEstadisticas->VidaActual = NuevaVida;
        
		CompEstadisticas->OnRep_VidaActual();
	}
}

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

	
	Item->OnRep_OwningCharacter();
}


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


void ACharacterprincipal::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EI =
		Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (IA_Movimiento)
			EI->BindAction(IA_Movimiento, ETriggerEvent::Triggered,this, &ACharacterprincipal::Move);

		if (IA_Interactuar)
			EI->BindAction(IA_Interactuar, ETriggerEvent::Started,this, &ACharacterprincipal::TryInteract);
		
		if (ActionAtaqueRanged)
		{
			EI->BindAction(ActionAtaqueRanged, ETriggerEvent::Started, this, &ACharacterprincipal::IniciarPunteriaRanged);
           
			EI->BindAction(ActionAtaqueRanged, ETriggerEvent::Completed, this, &ACharacterprincipal::FinalizarYAtaqueRanged);
		}
		if (IA_Correr)
		{
			EI->BindAction(IA_Correr, ETriggerEvent::Started, this, &ACharacterprincipal::EmpezarACorrer);
			EI->BindAction(IA_Correr, ETriggerEvent::Completed, this, &ACharacterprincipal::DejarDeCorrer);
		}
		if (IA_Saltar)
		{
			EI->BindAction(IA_Saltar, ETriggerEvent::Started, this, &ACharacterprincipal::IntentarSaltar);
			EI->BindAction(IA_Saltar, ETriggerEvent::Completed, this, &ACharacterprincipal::IntentarDetenerSalto);
		}
		if (IA_Dash)
		{
			EI->BindAction(IA_Dash, ETriggerEvent::Started, this, &ACharacterprincipal::EjecutarDash);
		}
		if (IA_Curar)
		{
			EI->BindAction(IA_Curar, ETriggerEvent::Started, this, &ACharacterprincipal::PresionTeclaCuracion);
		}
	}
}

void ACharacterprincipal::Server_SetVelocidadMax_Implementation(float NuevaVelocidad)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = NuevaVelocidad;
	}
}

void ACharacterprincipal::EmpezarACorrer()
{
	if (CompEstadisticas && GetCharacterMovement())
	{
		float VelocidadBaseSegura = (CompEstadisticas->VelocidadActual > 0.f) ? CompEstadisticas->VelocidadActual : CompEstadisticas->VelocidadBase;
		float VelocidadCorrer = VelocidadBaseSegura * MultiplicadorCorrer;

		GetCharacterMovement()->MaxWalkSpeed = VelocidadCorrer;
       	Server_SetVelocidadMax(VelocidadCorrer);
		Server_SetEfectoCorrer(true);
		TargetFOV = FOVCorrer;
		GetWorldTimerManager().SetTimer(TimerTransicionCamara, this, &ACharacterprincipal::ActualizarCamara, 0.01f, true);
		
	}
}

void ACharacterprincipal::DejarDeCorrer()
{
	if (CompEstadisticas && GetCharacterMovement())
	{
		float VelocidadBaseSegura = (CompEstadisticas->VelocidadActual > 0.f) ? CompEstadisticas->VelocidadActual : CompEstadisticas->VelocidadBase;
        
		GetCharacterMovement()->MaxWalkSpeed = VelocidadBaseSegura;
		Server_SetVelocidadMax(VelocidadBaseSegura);
		Server_SetEfectoCorrer(false);
		TargetFOV = FOVNormal;
		GetWorldTimerManager().SetTimer(TimerTransicionCamara, this, &ACharacterprincipal::ActualizarCamara, 0.01f, true);
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

void ACharacterprincipal::FinalizarYAtaqueRanged()
{
	if (IndicadorRanged && IndicadorRanged->IsVisible())
	{
		IndicadorRanged->SetVisibility(false);

		FRotator DireccionDisparo = GetActorRotation(); 

		EjecutarDisparoRanged(DireccionDisparo);
	}
}

void ACharacterprincipal::ActualizarCamara()
{
	UCameraComponent* Camara = FindComponentByClass<UCameraComponent>();
	USpringArmComponent* SpringArm = FindComponentByClass<USpringArmComponent>();

	bool bFOVTerminado = true;
	bool bDistanciaTerminada = true;

	if (Camara)
	{
		if (FMath::IsNearlyEqual(Camara->FieldOfView, TargetFOV, 0.1f))
		{
			Camara->SetFieldOfView(TargetFOV);
		}
		else
		{
			Camara->SetFieldOfView(FMath::FInterpTo(Camara->FieldOfView, TargetFOV, 0.01f, VelocidadTransicionFOV));
			bFOVTerminado = false;
		}
	}

	if (SpringArm)
	{
		if (FMath::IsNearlyEqual(SpringArm->TargetArmLength, TargetDistancia, 0.5f))
		{
			SpringArm->TargetArmLength = TargetDistancia;
		}
		else
		{
			SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, TargetDistancia, 0.01f, VelocidadTransicionDistancia);
			bDistanciaTerminada = false;
		}
	}

	if (bFOVTerminado && bDistanciaTerminada)
	{
		GetWorldTimerManager().ClearTimer(TimerTransicionCamara);
	}
}

void ACharacterprincipal::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);
	
	if (!GetCharacterMovement()) return;

	if (PrevMovementMode == MOVE_Walking && GetCharacterMovement()->MovementMode == MOVE_Falling)
	{
		if (SaltosRealizados == 0)
		{
			bCoyoteTimeActivo = true;
			GetWorldTimerManager().SetTimer(TimerCoyote, this, &ACharacterprincipal::FinalizarCoyoteTime, TiempoCoyote, false);
		}
	}

	if (GetCharacterMovement()->MovementMode == MOVE_Walking)
	{
		SaltosRealizados = 0;
		bCoyoteTimeActivo = false;
		GetWorldTimerManager().ClearTimer(TimerCoyote);

		TargetDistancia = DistanciaNormal;
		TargetFOV = FOVNormal;
		GetWorldTimerManager().SetTimer(TimerTransicionCamara, this, &ACharacterprincipal::ActualizarCamara, 0.01f, true);
	}
}
