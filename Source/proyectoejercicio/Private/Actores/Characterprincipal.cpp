#include "Actores/Characterprincipal.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"

ACharacterprincipal::ACharacterprincipal()
{
	PrimaryActorTick.bCanEverTick = true;

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

void ACharacterprincipal::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (IA_Movimiento)
		{
			EnhancedInput->BindAction(IA_Movimiento, ETriggerEvent::Triggered, this, &ACharacterprincipal::Move);
		}
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
