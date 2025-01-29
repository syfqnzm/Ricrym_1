// PlayerCharacter.cpp

#include "PlayerCharacter.h"
#include "HealthComponent.h"
#include "CombatSystem.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "GameFramework/PlayerController.h"
#include "Components/ProgressBar.h"
#include "Blueprint/UserWidget.h"
#include "InputMappingContext.h"

APlayerCharacter::APlayerCharacter()
{
    PrimaryActorTick.bCanEverTick = true;
    bReplicates = true;

    HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

    // Create Spring Arm for Camera
    USpringArmComponent* SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(RootComponent);
    SpringArm->TargetArmLength = 300.0f;
    SpringArm->bUsePawnControlRotation = true;

    // Create Camera and attach it to Spring Arm
    UCameraComponent* Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm);
    Camera->bUsePawnControlRotation = false;

    // Create a Skeletal Mesh Component for the Spear
    SpearMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Spear"));

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

    static ConstructorHelpers::FObjectFinder<UInputAction> RangedAction(TEXT("/Game/ThirdPerson/Input/Actions/IA_RangedAttack.IA_RangedAttack"));
    if (RangedAction.Succeeded())
    {
        IA_RangedAttack = RangedAction.Object;
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to find IA_RangedAttack"));
    }

    static ConstructorHelpers::FObjectFinder<UInputAction> AOEAttack(TEXT("/Game/ThirdPerson/Input/Actions/IA_AOEAttack.IA_AOEAttack"));
    if (AOEAttack.Succeeded())
    {
        IA_AOEAttack = AOEAttack.Object;
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to find IA_AOEAttack"));
    }

    // Update the path to the correct location
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> SpearAsset(TEXT("/Game/SpearAnimationsV2/Mesh/SK_Spear.SK_Spear"));
    if (SpearAsset.Succeeded())
    {
        SpearMesh->SetSkeletalMesh(SpearAsset.Object);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Spear not found!"));
    }
}

// Called when the game starts
void APlayerCharacter::BeginPlay()
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

    // Ensure that GetMesh() and SpearMesh return valid pointers before attaching the SpearMesh
    if (USkeletalMeshComponent* MeshComponent = GetMesh())
    {
        if (SpearMesh)
        {
            SpearMesh->SetupAttachment(MeshComponent, TEXT("WeaponSocket"));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("SpearMesh is null"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to get mesh component for attachment"));
    }

    // Generate a unique name for the CombatSystem actor
    FString UniqueName = FString::Printf(TEXT("Combat   System_%s"), *FGuid::NewGuid().ToString());

    CombatSystem = GetWorld()->SpawnActor<ACombatSystem>(ACombatSystem::StaticClass());
    if (CombatSystem)
    {
        CombatSystem->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
        CombatSystem->SetOwner(this);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to spawn CombatSystem"));
    }

    if (HealthBarWidgetClass)
    {
        HealthBarWidget = CreateWidget<UUserWidget>(GetWorld(), HealthBarWidgetClass);
        if (HealthBarWidget)
        {
            HealthBarWidget->AddToViewport();

            // Bind health update function
            HealthComponent->OnHealthChanged.AddDynamic(this, &APlayerCharacter::UpdateHealthBar);
            UpdateHealthBar(HealthComponent->CurrentHealth / HealthComponent->MaxHealth);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to create HealthBarWidget"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("HealthBarWidgetClass is null"));
    }

}

void APlayerCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// Bind Inputs
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
    {
        if (PlayerController)
        {
            if (UEnhancedInputLocalPlayerSubsystem* Subsystems = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
            {
                Subsystems->AddMappingContext(IMC_Default, 0);
                if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent))
                {
                    if (Input)
                    {
                        Input->BindAction(IA_Move, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
                        Input->BindAction(IA_Look, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
                        Input->BindAction(IA_Jump, ETriggerEvent::Started, this, &APlayerCharacter::StartJump);
                        Input->BindAction(IA_Jump, ETriggerEvent::Completed, this, &APlayerCharacter::StopJump);
                        Input->BindAction(IA_Attack, ETriggerEvent::Triggered, this, &APlayerCharacter::PerformMeleeAttack);
                        Input->BindAction(IA_RangedAttack, ETriggerEvent::Triggered, this, &APlayerCharacter::PerformRangeAttack);
                        Input->BindAction(IA_AOEAttack, ETriggerEvent::Triggered, this, &APlayerCharacter::PerformAOEAttack);
                    }
                    else
                    {
                        UE_LOG(LogTemp, Error, TEXT("Failed to cast PlayerInputComponent to UEnhancedInputComponent"));
                    }
                }
            }
        }
    }
}

// Movement
void APlayerCharacter::Move(const FInputActionValue& Value)
{
    FVector2D MovementVector = Value.Get<FVector2D>();
    AddMovementInput(GetActorForwardVector(), MovementVector.Y);
    AddMovementInput(GetActorRightVector(), MovementVector.X);
}

// Look Around
void APlayerCharacter::Look(const FInputActionValue& Value)
{
    FVector2D LookAxisVector = Value.Get<FVector2D>();
    AddControllerYawInput(LookAxisVector.X);
    AddControllerPitchInput(LookAxisVector.Y);
}

// Jumping
void APlayerCharacter::StartJump() { Jump(); }
void APlayerCharacter::StopJump() { StopJumping(); }

// Melee Attack
void APlayerCharacter::PerformMeleeAttack()
{
    if (CombatSystem)
    {
        CombatSystem->MeleeAttack(20.0f, 200.0f); // 200.0f is the range of the attack
    }
}

// Range Attack
void APlayerCharacter::PerformRangeAttack()
{
    if (CombatSystem)
    {
        CombatSystem->RangedAttack(this, 20.0f, 200.0f); // 200.0f is the speed of the projectile
    }
}

// AOE Attack
void APlayerCharacter::PerformAOEAttack()
{
    if (CombatSystem)
    {
        CombatSystem->AOEAttack(this, 20.0f, 300.0f); // 300.0f is the radius of the AOE attack
    }
}

// Function to get lifetime replicated properties
void APlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(APlayerCharacter, HealthComponent);
}

void APlayerCharacter::UpdateHealthBar(float HealthPercent)
{
    if (HealthBarWidget)
    {
        UFunction* Func = HealthBarWidget->FindFunction(FName("UpdateHealthBar"));
        if (Func)
        {
            struct FUpdateHealthBarParams
            {
                float HealthPercent;
            };

            FUpdateHealthBarParams Params;
            Params.HealthPercent = HealthPercent;

            HealthBarWidget->ProcessEvent(Func, &Params);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to find UpdateHealthBar function in HealthBarWidget"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("HealthBarWidget is null"));
    }
}
