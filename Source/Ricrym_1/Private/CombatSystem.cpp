// CombatSystem.cpp

#include "CombatSystem.h"
#include "HealthComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "DrawDebugHelpers.h"

ACombatSystem::ACombatSystem()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ACombatSystem::BeginPlay()
{
    Super::BeginPlay();
    OwnerCharacter = Cast<ACharacter>(GetOwner());
}

void ACombatSystem::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ACombatSystem::MeleeAttack(float DamageAmount, float AttackRange)
{
    if (!OwnerCharacter) return;

    UE_LOG(LogTemp, Warning, TEXT("%s is attacking!"), *OwnerCharacter->GetName());

    FVector Start = OwnerCharacter->GetActorLocation();
    FVector ForwardVector = OwnerCharacter->GetActorForwardVector();
    FVector End = Start + (ForwardVector * AttackRange);

    FHitResult HitResult;
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(OwnerCharacter);

    if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Pawn, CollisionParams))
    {
        AActor* HitActor = HitResult.GetActor();
        if (HitActor)
        {
            if (UHealthComponent* EnemyHealth = HitActor->FindComponentByClass<UHealthComponent>())
            {
                EnemyHealth->TakeDamage(DamageAmount);
                UE_LOG(LogTemp, Warning, TEXT("%s hit %s for %.2f damage!"), *OwnerCharacter->GetName(), *HitActor->GetName(), DamageAmount);
                if (GEngine)
                {
                    GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("MeleeAttack attack for %.2f damage!"), DamageAmount));
                }
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("%s hit %s but no HealthComponent found!"), *OwnerCharacter->GetName(), *HitActor->GetName());
            }
        }
    }

    // Debug line to visualize the melee attack range
    DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.0f, 0, 1.0f);
}

void ACombatSystem::RangedAttack(ACharacter* Attacker, float DamageAmount, float ProjectileSpeed)
{
    if (!Attacker) return;

    FVector Start = Attacker->GetActorLocation();
    FVector ForwardVector = Attacker->GetActorForwardVector();
    FVector End = Start + (ForwardVector * 1000.0f);  // Adjust projectile travel distance

    FHitResult HitResult;
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(Attacker);

    if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Pawn, CollisionParams))
    {
        AActor* HitActor = HitResult.GetActor();
        if (HitActor)
        {
            UE_LOG(LogTemp, Warning, TEXT("%s hit %s with a projectile!"), *Attacker->GetName(), *HitActor->GetName());

            UHealthComponent* EnemyHealth = HitActor->FindComponentByClass<UHealthComponent>();
            if (EnemyHealth)
            {
                EnemyHealth->TakeDamage(DamageAmount);
                UE_LOG(LogTemp, Warning, TEXT("%s hit %s with a projectile for %.2f damage!"), *Attacker->GetName(), *HitActor->GetName(), DamageAmount);
                if (GEngine)
                {
                    GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("RangedAttack attack for %.2f damage!"), DamageAmount));
                }
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("Failed to find HealthComponent on %s"), *HitActor->GetName());
            }
        }
    }

    // Debug line to visualize the projectile path
    DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1.0f, 0, 1.0f);
}

void ACombatSystem::AOEAttack(ACharacter* Attacker, float DamageAmount, float Radius)
{
    if (!Attacker) return;

    FVector Origin = Attacker->GetActorLocation();
    TArray<FHitResult> HitResults;
    FCollisionShape Sphere = FCollisionShape::MakeSphere(Radius);

    if (GetWorld()->SweepMultiByChannel(HitResults, Origin, Origin, FQuat::Identity, ECC_Pawn, Sphere))
    {
        for (FHitResult Hit : HitResults)
        {
            AActor* HitActor = Hit.GetActor();
            if (HitActor && HitActor != Attacker)  // Ensure we don’t damage the attacker
            {
                UE_LOG(LogTemp, Warning, TEXT("%s hit %s with an AOE attack!"), *Attacker->GetName(), *HitActor->GetName());

                UHealthComponent* EnemyHealth = HitActor->FindComponentByClass<UHealthComponent>();
                if (EnemyHealth)
                {
                    EnemyHealth->TakeDamage(DamageAmount);
                    UE_LOG(LogTemp, Warning, TEXT("%s hit %s with an AOE attack for %.2f damage!"), *Attacker->GetName(), *HitActor->GetName(), DamageAmount);
                    if (GEngine)
                    {
                        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("AOE attack for %.2f damage!"), DamageAmount));
                    }
                }
                else
                {
                    UE_LOG(LogTemp, Error, TEXT("Failed to find HealthComponent on %s"), *HitActor->GetName());
                }
            }
        }
    }

    // Debug sphere to visualize the AOE attack radius
    DrawDebugSphere(GetWorld(), Origin, Radius, 32, FColor::Blue, false, 1.0f, 0, 1.0f);
}

