#include "FlagManager.h"

// Sets default values for this component's properties
UFlagManager::UFlagManager()
{
	PrimaryComponentTick.bCanEverTick = true;
}

//Permet de v�rifier si un flag existe bien
//A appeler � chaque beginplay des components qui utilisent des flags
bool UFlagManager::IsFlagValid(const FString& category, const FString& flagName) const
{
	FString key = GetFlagKey(category, flagName);
	return mFlagValues.Contains(key);
}

//Change la valeur d'un flag � partir de sa cat�gorie et son nom
void UFlagManager::EditFlag(const FString& category, const FString& flagName, const int& value)
{
	FString key = GetFlagKey(category, flagName);
	mFlagValues[key] = value;

	OnFlagChanged.Broadcast();
}

//Ajoute 1 � la valeur d'un flag
void UFlagManager::IncrementFlag(const FString& category, const FString& flagName)
{
	FString key = GetFlagKey(category, flagName);
	mFlagValues[key] += 1;

	OnFlagChanged.Broadcast();
}

//Soustrait 1 � la valeur d'un flag
void UFlagManager::DecrementFlag(const FString& category, const FString& flagName)
{
	FString key = GetFlagKey(category, flagName);
	mFlagValues[key] -= 1;

	OnFlagChanged.Broadcast();
}

//Repasse tous les flags d'une cat�gorie � 0
void UFlagManager::ResetFlagCategory(const FString& category)
{
	for (const auto& [key, value] : mFlagValues)
	{
		//Split entre les : pour r�cup�rer uniquement la cat�gorie du flag
		FString keyCategory = "";
		FString keyFlagName = "";
		key.Split(":", &keyCategory, &keyFlagName);

		if (category == keyCategory)
		{
			mFlagValues[key] = 0;
		}
	}

	OnFlagChanged.Broadcast();
}

//Repasse tous les flags � 0
void UFlagManager::ResetAllFlags()
{
	for (const auto& [key, value] : mFlagValues)
	{
		mFlagValues[key] = 0;
	}

	OnFlagChanged.Broadcast();
}

//V�rifie si un flag a la valeur d�sir�e
//Exact == true => la valeur du flag doit �tre exactement la m�me
//Exact == false => la valeur du flag doit �tre sup�rieur ou �gale
bool UFlagManager::IsFlagTrigger(const FString& category, const FString& flagName, const int& value, bool exact)
{
	FString key = GetFlagKey(category, flagName);
	int flagValue = mFlagValues[key];

	if (exact && flagValue == value)
	{
		return true;
	}
	else if (!exact && flagValue >= value)
	{
		return true;
	}
	return false;
}

void UFlagManager::BeginPlay()
{
	Super::BeginPlay();

	CreateFlags();
}

void UFlagManager::CreateFlags()
{
	//Cr�ation de la map flag name / flag value
	mFlagValues = TMap<FString, int>();

	//On boucle dans toutes les categories
	for(int categoryIndex = 0; categoryIndex < Flags.Num(); categoryIndex++)
	{
		//On boucle dans tous les flags de la categorie
		FFlagList flagList = Flags[categoryIndex];
		for (int flagIndex = 0; flagIndex < flagList.Flags.Num(); flagIndex++)
		{
			//On cr�e la cl� � partir de la categorie et du nom du flag
			FString key = GetFlagKey(flagList.Category, flagList.Flags[flagIndex]);
			//On initialise le flag � 0
			mFlagValues.Add(key, 0);
		}
	}
}

const FString UFlagManager::GetFlagKey(const FString& category, const FString& flagName) const
{
	return category + ":" + flagName;
}
