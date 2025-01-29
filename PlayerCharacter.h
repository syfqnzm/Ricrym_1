// PlayerCharacter.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <CombatSystem.h>
#include <EnhancedInputLibrary.h>
#include "PlayerCharacter.generated.h"


class UHealthComponent;
class UInputMappingContext;
class UInputAction;
class UUserWidget;

UCLASS()
class RICRYM_1_API APlayerCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    APlayerCharacter();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    void Move(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);
    void StartJump();
    void StopJump(); // Add this declaration

    UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadOnly, Category = "Components")
    UHealthComponent* HealthComponent;

    // Combat system
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class ACombatSystem* CombatSystem;

    // Skeletal Mesh Component for the Spear
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class USkeletalMeshComponent* SpearMesh;

    // Input Mapping Context
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    class UInputMappingContext* IMC_Default;

    // Input Actions
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    class UInputAction* IA_Move;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    class UInputAction* IA_Look;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    class UInputAction* IA_Jump;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    class UInputAction* IA_Attack;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    class UInputAction* IA_RangedAttack;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    class UInputAction* IA_AOEAttack;

    // Health Bar Widget Class
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
    TSubclassOf<class UUserWidget> HealthBarWidgetClass;

    // Health Bar Widget
    UPROPERTY()
    class UUserWidget* HealthBarWidget;

    // Function to update health bar
    UFUNCTION()
    void UpdateHealthBar(float HealthPercent);

    // Attack Functions
    void PerformMeleeAttack();
    void PerformRangeAttack();
    void PerformAOEAttack();

    // Replication
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
