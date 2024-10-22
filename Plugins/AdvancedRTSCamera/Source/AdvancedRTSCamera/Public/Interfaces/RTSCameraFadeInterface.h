// Copyright Two Neurons Studio, LLC. Year of Publication 2024.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RTSCameraFadeInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Category = "CameraRTS")
class URTSCameraFadeInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ADVANCEDRTSCAMERA_API IRTSCameraFadeInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|Fade")
	TArray<UMaterialInterface*> GetMaterials(UStaticMeshComponent* Target);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|Fade")
	UStaticMeshComponent* GetStaticMesh();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|Fade")
	void SetMaterial(UStaticMeshComponent* Target, int ElementIndex, UMaterialInterface* Material);
};
