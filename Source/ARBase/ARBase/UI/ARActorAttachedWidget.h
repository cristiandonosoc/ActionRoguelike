#pragma once

#include <Blueprint/UserWidget.h>

#include "ARActorAttachedWidget.generated.h"

class USizeBox;

// UARActorAttachedWidget is a widget that is "attached" to an actor in the world and is placed
// "near" it.
// It handles the world to screen projection. It is meant to be sub-classed.
UCLASS()
class ARBASE_API UARActorAttachedWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeTick(const FGeometry& geometry, float delta) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<AActor> AttachedTo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector TranslationOffset = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector2D OnScreenTranslateOffset = FVector2D::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D OnScreenScale = {1.0f, 1.0f};

protected:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<USizeBox> ParentSizeBox;
};
