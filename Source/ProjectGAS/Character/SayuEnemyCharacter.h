#pragma once

#include "CoreMinimal.h"
#include "SayuCharacterBase.h"
#include "SayuEnemyCharacter.generated.h"

class USayuAttributeSet_Enemy;

UCLASS()
class PROJECTGAS_API ASayuEnemyCharacter : public ASayuCharacterBase
{
	GENERATED_BODY()

public:
	ASayuEnemyCharacter();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<USayuAttributeSet_Enemy> AttributeSet;
};