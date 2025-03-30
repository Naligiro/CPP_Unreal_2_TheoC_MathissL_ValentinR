#pragma once

#include "CoreMinimal.h"
#include "Flag.generated.h"

USTRUCT(BlueprintType)
struct BBW_DEMO_API FFlag
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flag")
	FString Category;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flag")
	FString FlagName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flag")
	int Value;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flag")
	bool Exact;
};
