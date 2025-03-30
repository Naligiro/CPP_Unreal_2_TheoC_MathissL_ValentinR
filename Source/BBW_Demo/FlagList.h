#pragma once

#include "CoreMinimal.h"
#include "FlagList.generated.h"

USTRUCT(BlueprintType)
struct BBW_DEMO_API FFlagList
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Flag")
	FString Category;
	UPROPERTY(EditDefaultsOnly, Category = "Flag")
	TArray<FString> Flags;
};
