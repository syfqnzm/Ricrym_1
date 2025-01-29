// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HealthComponent.h"
#include "CombatSystem.generated.h"

UCLASS()
class RICRYM_1_API ACombatSystem : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACombatSystem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Function for melee attack
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void MeleeAttack(float DamageAmount, float AttackRange);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void RangedAttack(ACharacter* Attacker, float DamageAmount, float ProjectileSpeed);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void AOEAttack(ACharacter* Attacker, float DamageAmount, float Radius);

private:
	ACharacter* OwnerCharacter; // Refer to the Owner of the Combat System
};

