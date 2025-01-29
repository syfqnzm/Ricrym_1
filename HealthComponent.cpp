// HealthComponent.cpp

#include "HealthComponent.h"
#include "Net/UnrealNetwork.h"

UHealthComponent::UHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    MaxHealth = 100.0f;
    CurrentHealth = MaxHealth;
    SetIsReplicatedByDefault(true);
}

void UHealthComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UHealthComponent::TakeDamage(float Damage)
{
    if (Damage <= 0.0f)
    {
        return;
    }

    CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.0f, MaxHealth);
    HandleHealthChanged();

    if (CurrentHealth <= 0.0f)
    {
        AActor* Owner = GetOwner();
        if (Owner)
        {
            Owner->Destroy();
            UE_LOG(LogTemp, Warning, TEXT("Current Health is = %f %s is Death"), CurrentHealth, *Owner->GetName());
            if (GEngine)
            {
                GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT(" %s is Death"), *Owner->GetName()));
            }
        }
    }
}

void UHealthComponent::HandleHealthChanged()
{
    float HealthPercent = CurrentHealth / MaxHealth;
    OnHealthChanged.Broadcast(HealthPercent);
}

void UHealthComponent::OnRep_CurrentHealth()
{
    HandleHealthChanged();
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(UHealthComponent, CurrentHealth);
}
