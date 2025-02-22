// Fill out your copyright notice in the Description page of Project Settings.


#include "EM_Weapon.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

// Sets default values
AEM_Weapon::AEM_Weapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Damage = 20.0f;
}

// Called when the game starts or when spawned
void AEM_Weapon::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AEM_Weapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEM_Weapon::StartAction()
{
	BP_StartAction();
	PlaySound(MagicProjectileSound);
}

void AEM_Weapon::StopAction()
{
	BP_StopAction();
}

void AEM_Weapon::SetCharacterOwner(ACharacter* NewOwner)
{
	if (IsValid(NewOwner))
	{
		SetOwner(NewOwner);
		CurrentOwnerCharacter = NewOwner;
	}
}

void AEM_Weapon::PlaySound(USoundCue* SoundCue, bool bIs3D, FVector SoundLocation)
{
	if (!IsValid(SoundCue))
	{
		return;
	}

	if (bIs3D)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), SoundCue, SoundLocation);
	}
	else
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundCue);
	}
}

