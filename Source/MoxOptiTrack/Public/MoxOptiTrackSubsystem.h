#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LiveLinkTypes.h"
#include "MoxOptiTrackSubsystem.generated.h"

USTRUCT(BlueprintType)
struct FMoxTrackerData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "MozXR|OptiTrack")
	FName SubjectName;

	UPROPERTY(BlueprintReadOnly, Category = "MozXR|OptiTrack")
	FTransform Transform;
};

/**
 * Subsystem to access OptiTrack tracker data via LiveLink.
 */
UCLASS()
class MOXOPTITRACK_API UMoxOptiTrackSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	/**
	 * Get all active OptiTrack trackers.
	 * @param NameFilter Optional filter to include only trackers with names containing this string. Empty string returns all.
	 * @return Array of tracker data.
	 */
	UFUNCTION(BlueprintCallable, Category = "MozXR|OptiTrack")
	TArray<FMoxTrackerData> GetAllTrackers(FString NameFilter = TEXT(""));

	/**
	 * Get data for a specific tracker by name.
	 * @param SubjectName The name of the tracker subject.
	 * @param bFound True if the tracker was found and data is valid.
	 * @return The transform of the tracker.
	 */
	UFUNCTION(BlueprintCallable, Category = "MozXR|OptiTrack")
	FTransform GetTrackerData(FName SubjectName, bool& bFound);

private:
	bool IsOptiTrackSubject(const FLiveLinkSubjectKey& SubjectKey) const;
};
