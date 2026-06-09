#include "Actores/Characterprincipal.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "Actores/WeaponMaster.h"


ACharacterprincipal::ACharacterprincipal()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 400.0f, 0.0f);
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->MaxWalkSpeed = 400.0f;
	GetCharacterMovement()->JumpZVelocity = 250.0f;
	GetCharacterMovement()->GravityScale = 2.2f;
	JumpMaxCount = 1;
}

void ACharacterprincipal::BeginPlay()
{
	Super::BeginPlay();
}

void ACharacterprincipal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
void ACharacterprincipal::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACharacterprincipal, CurrentWeapon);
}
void ACharacterprincipal::OnRep_CurrentWeapon()
{
}

void ACharacterprincipal::Server_EquipWeapon_Implementation(AWeaponMaster* Weapon)
{
	if (!Weapon) return;

	DropCurrentWeapon(); 
	CurrentWeapon = Weapon;
	CurrentWeapon->DisablePickup();
	CurrentWeapon->AttachToComponent(GetMesh(),FAttachmentTransformRules::SnapToTargetNotIncludingScale,Weapon->SocketName
	);
}

void ACharacterprincipal::Server_DropWeapon_Implementation()
{
	DropCurrentWeapon();
}

void ACharacterprincipal::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (IA_Movimiento)
		{
			EnhancedInput->BindAction(IA_Movimiento, ETriggerEvent::Triggered, this, &ACharacterprincipal::Move);
		}
		if (IA_Interactuar)
			EnhancedInput->BindAction(IA_Interactuar, ETriggerEvent::Triggered,	this, &ACharacterprincipal::TryInteract);
	}
}

void ACharacterprincipal::Move(const FInputActionValue& Value)
{
	const FVector2D InputVec = Value.Get<FVector2D>();

	if (!Controller) return;

	if (InputVec.X != 0.0f)
	{
		AddControllerYawInput(InputVec.X);
	}

	if (InputVec.Y != 0.0f)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		AddMovementInput(ForwardDirection, InputVec.Y);
	}
}

void ACharacterprincipal::AddMonedas_Implementation(int Moneda)
{
	Monedas += Moneda;

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Monedas: %d"), Monedas));
	}
}

USkeletalMeshComponent* ACharacterprincipal::GetSkeletalMesh_Implementation()
{
	return GetMesh();
}

void ACharacterprincipal::MostrarMensaje()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Overlapeando"));
	}
}

void ACharacterprincipal::TryInteract()
{
	if (CurrentWeapon)
	{
		TArray<AActor*> Overlapping;
		GetOverlappingActors(Overlapping, AWeaponMaster::StaticClass());

		bool bNearPickup = false;
		for (AActor* Actor : Overlapping)
		{
			AWeaponMaster* Weapon = Cast<AWeaponMaster>(Actor);
			if (Weapon && Weapon != CurrentWeapon)
			{
				bNearPickup = true;
				break;
			}
		}

		if (!bNearPickup)
			Server_DropWeapon();
	}
}

void ACharacterprincipal::DropCurrentWeapon()
{
	if (!CurrentWeapon) return;

	FVector DropLocation = GetActorLocation() + GetActorForwardVector() * 150.f;  // más lejos

	CurrentWeapon->DetachFromActor(
		FDetachmentTransformRules::KeepWorldTransform
	);
	CurrentWeapon->SetActorLocationAndRotation(DropLocation, FRotator::ZeroRotator);

	CurrentWeapon->EnablePickupDelayed(1.0f); 
	CurrentWeapon = nullptr;
}
