#pragma once

#include "CoreMinimal.h"
#include "SayuCharacterBase.h"
#include "ProjectGASEnemyCharacter.generated.h"

class USayuAttributeSet_Enemy;

UCLASS()
class PROJECTGAS_API AProjectGASEnemyCharacter : public ASayuCharacterBase
{
	GENERATED_BODY()

public:
	AProjectGASEnemyCharacter();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<USayuAttributeSet_Enemy> AttributeSet;
};