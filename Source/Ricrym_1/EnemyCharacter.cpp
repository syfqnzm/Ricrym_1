#include "EnemyCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "CombatSystem.h"
#include "InputActionValue.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "GameFramework/PlayerController.h"

AEnemyCharacter::AEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

	// Get the Default Input Mapping Context
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> DefaultMapping(TEXT("/Game/ThirdPerson/Input/IMC_Default.IMC_Default"));
	if (DefaultMapping.Succeeded())
	{
		IMC_Default = DefaultMapping.Object;
	}

	// Get the Default Input Actions
	static ConstructorHelpers::FObjectFinder<UInputAction> MoveAction(TEXT("/Game/ThirdPerson/Input/Actions/IA_Move.IA_Move"));
	if (MoveAction.Succeeded())
	{
		IA_Move = MoveAction.Object;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to find IA_Move"));
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> LookAction(TEXT("/Game/ThirdPerson/Input/Actions/IA_Look.IA_Look"));
	if (LookAction.Succeeded())
	{
		IA_Look = LookAction.Object;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to find IA_Look"));
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> JumpAction(TEXT("/Game/ThirdPerson/Input/Actions/IA_Jump.IA_Jump"));
	if (JumpAction.Succeeded())
	{
		IA_Jump = JumpAction.Object;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to find IA_Jump"));
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> AttackAction(TEXT("/Game/ThirdPerson/Input/Actions/IA_Attack.IA_Attack"));
	if (AttackAction.Succeeded())
	{
		IA_Attack = AttackAction.Object;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to find IA_Attack"));
	}
}

// Called when the game starts
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		PlayerController->Possess(this);
	}

	if (PlayerController)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystems = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystems->AddMappingContext(IMC_Default, 0);
		}
	}

	// Generate a unique name for the CombatSystem actor
	FString UniqueName = FString::Printf(TEXT("CombatSystem_%s"), *FGuid::NewGuid().ToString());

	CombatSystem = GetWorld()->SpawnActor<ACombatSystem>(ACombatSystem::StaticClass());
	if (CombatSystem)
	{
		CombatSystem->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
		CombatSystem->SetOwner(this);
	}
}

void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Bind Inputs
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (PlayerController)
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystems = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				Subsystems->AddMappingContext(IMC_Default, 0);
				if (UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {

					Input->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AEnemyCharacter::Move);
					Input->BindAction(IA_Look, ETriggerEvent::Triggered, this, &AEnemyCharacter::Look);
					Input->BindAction(IA_Jump, ETriggerEvent::Started, this, &AEnemyCharacter::StartJump);
					Input->BindAction(IA_Jump, ETriggerEvent::Completed, this, &AEnemyCharacter::StopJump);
					Input->BindAction(IA_Attack, ETriggerEvent::Triggered, this, &AEnemyCharacter::PerformMeleeAttack);
				}
			}
		}
	}
}

// Movement
void AEnemyCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	AddMovementInput(GetActorForwardVector(), MovementVector.Y);
	AddMovementInput(GetActorRightVector(), MovementVector.X);
}

// Look Around
void AEnemyCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

// Jumping
void AEnemyCharacter::StartJump() { Jump(); }
void AEnemyCharacter::StopJump() { StopJumping(); }

// Melee Attack
void AEnemyCharacter::PerformMeleeAttack()
{
	if (CombatSystem) {
		CombatSystem->MeleeAttack(20.0f, 200.0f); // 200.0f is the range of the attack
	}
}

// Function to get lifetime replicated properties
void AEnemyCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AEnemyCharacter, HealthComponent);
}
