// HealthComponent.h

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, float, HealthPercent);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class RICRYM_1_API UHealthComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UHealthComponent();

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health")
    float MaxHealth;

    UPROPERTY(ReplicatedUsing = OnRep_CurrentHealth, BlueprintReadOnly, Category = "Health")
    float CurrentHealth;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnHealthChangedSignature OnHealthChanged;

    void TakeDamage(float Damage);

protected:
    virtual void BeginPlay() override;

private:
    UFUNCTION()
    void OnRep_CurrentHealth();

    void HandleHealthChanged();
};
