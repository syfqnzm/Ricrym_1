#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HealthComponent.h" // Header file for Health
#include "Net/UnrealNetwork.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "InputActionValue.h"
#include <CombatSystem.h>
#include "EnemyCharacter.generated.h"


UCLASS()
class RICRYM_1_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	AEnemyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

	void StartJump();

	void StopJump();
	void PerformMeleeAttack();

	// Function to get lifetime replicated properties
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	// Make this function as private to prevent other class to access it
	UPROPERTY(VisibleAnywhere, Replicated, Category = "Components")
	UHealthComponent* HealthComponent;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* IMC_Default; // Use default input mapping

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_Move; // Default Move Input

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_Look; // Default Look Input

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_Jump; // Default Jump Input	

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_Attack; // Default Attack Input

	UPROPERTY(VisibleAnywhere, Category = "Combat")
	ACombatSystem* CombatSystem;

};
