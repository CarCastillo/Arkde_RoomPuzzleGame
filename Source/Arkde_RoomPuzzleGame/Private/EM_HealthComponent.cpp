// Fill out your copyright notice in the Description page of Project Settings.


#include "EM_HealthComponent.h"

// Sets default values for this component's properties
UEM_HealthComponent::UEM_HealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	MaxHealth = 100.0f;
}

// Called when the game starts
void UEM_HealthComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;

	MyOwner = GetOwner();

	if (IsValid(MyOwner))
	{
		MyOwner->OnTakeAnyDamage.AddDynamic(this, &UEM_HealthComponent::TakingDamage);
	}

	GetWorld()->GetTimerManager().SetTimer(UpdateInitialHealthTimer, this, &UEM_HealthComponent::UpdateInitialHealth, 0.2f, false);
}

bool UEM_HealthComponent::IsDamaged()
{
	return Health < MaxHealth && Health > 0.0f;
}

void UEM_HealthComponent::RecoverHealth(float HealthPoints)
{
	if (bIsDead)
	{
		return;
	}

	Health = FMath::Clamp(Health + HealthPoints, 0.0f, MaxHealth);
	OnHealthUpdateDelegate.Broadcast(Health, MaxHealth);

	if (Health == MaxHealth)
	{
		OnHealthFullyRecoveredDelegate.Broadcast(this, true);
	}

	if (bDebug)
	{
		UE_LOG(LogTemp, Log, TEXT("My Health is: %s"), *FString::SanitizeFloat(Health));
	}
}

void UEM_HealthComponent::UpdateInitialHealth()
{
	OnHealthUpdateDelegate.Broadcast(Health, MaxHealth);
}

void UEM_HealthComponent::TakingDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0.0f || bIsDead)
	{
		return;
	}

	IsDamaged();

	Health = FMath::Clamp(Health - Damage, 0.0f, MaxHealth);

	if (Health == 0.0f)
	{
		bIsDead = true;
		OnDeadDelegate.Broadcast(DamageCauser);
	}

	if (Health <= 20.0f && Health > 0.0f)
	{
		bIsOnCriticalStatus = true;
	}

	OnHealthChangeDelegate.Broadcast(this, DamagedActor, Damage, DamageType, InstigatedBy, DamageCauser);
	OnHealthUpdateDelegate.Broadcast(Health, MaxHealth);

	if (bDebug)
	{
		UE_LOG(LogTemp, Log, TEXT("My Health is: %s"), *FString::SanitizeFloat(Health));
	}
}
