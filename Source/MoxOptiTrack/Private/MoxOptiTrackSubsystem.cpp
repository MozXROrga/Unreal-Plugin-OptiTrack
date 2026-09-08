#include "MoxOptiTrackSubsystem.h"
#include "ILiveLinkClient.h"
#include "LiveLinkClientReference.h"
#include "Features/IModularFeatures.h"
#include "Roles/LiveLinkTransformRole.h"
#include "Roles/LiveLinkTransformTypes.h"
#include "Roles/LiveLinkAnimationRole.h"
#include "Roles/LiveLinkCameraRole.h"
#include "Roles/LiveLinkCameraTypes.h"

TArray<FMoxTrackerData> UMoxOptiTrackSubsystem::GetAllTrackers(FString NameFilter)
{
	TArray<FMoxTrackerData> Result;

	if (!IModularFeatures::Get().IsModularFeatureAvailable(ILiveLinkClient::ModularFeatureName))
	{
		return Result;
	}

	ILiveLinkClient& LiveLinkClient = IModularFeatures::Get().GetModularFeature<ILiveLinkClient>(ILiveLinkClient::ModularFeatureName);
	TArray<FLiveLinkSubjectKey> Subjects = LiveLinkClient.GetSubjects(true, true);

	for (const FLiveLinkSubjectKey& Subject : Subjects)
	{
		if (IsOptiTrackSubject(Subject))
		{
			if (!NameFilter.IsEmpty() && !Subject.SubjectName.ToString().Contains(NameFilter))
			{
				continue;
			}

			bool bFound = false;
			FTransform Transform = GetTrackerData(Subject.SubjectName, bFound);
			if (bFound)
			{
				FMoxTrackerData Data;
				Data.SubjectName = Subject.SubjectName;
				Data.Transform = Transform;
				Result.Add(Data);
			}
		}
	}

	return Result;
}

FTransform UMoxOptiTrackSubsystem::GetTrackerData(FName SubjectName, bool& bFound)
{
	bFound = false;
	FTransform ResultTransform = FTransform::Identity;

	if (!IModularFeatures::Get().IsModularFeatureAvailable(ILiveLinkClient::ModularFeatureName))
	{
		return ResultTransform;
	}

	ILiveLinkClient& LiveLinkClient = IModularFeatures::Get().GetModularFeature<ILiveLinkClient>(ILiveLinkClient::ModularFeatureName);
	
	FLiveLinkSubjectKey SubjectKey;
	TArray<FLiveLinkSubjectKey> Subjects = LiveLinkClient.GetSubjects(true, true);
	bool bSubjectFound = false;
	for (const FLiveLinkSubjectKey& S : Subjects)
	{
		if (S.SubjectName == SubjectName)
		{
			SubjectKey = S;
			bSubjectFound = true;
			break;
		}
	}

	if (!bSubjectFound || !IsOptiTrackSubject(SubjectKey))
	{
		return ResultTransform;
	}

	FLiveLinkSubjectFrameData SubjectFrameData;
	if (LiveLinkClient.EvaluateFrame_AnyThread(SubjectKey.SubjectName, ULiveLinkTransformRole::StaticClass(), SubjectFrameData))
	{
		FLiveLinkTransformFrameData* TransformData = SubjectFrameData.FrameData.Cast<FLiveLinkTransformFrameData>();
		if (TransformData)
		{
			ResultTransform = TransformData->Transform;
			bFound = true;
		}
	}
	else if (LiveLinkClient.EvaluateFrame_AnyThread(SubjectKey.SubjectName, ULiveLinkAnimationRole::StaticClass(), SubjectFrameData))
	{
		FLiveLinkAnimationFrameData* AnimData = SubjectFrameData.FrameData.Cast<FLiveLinkAnimationFrameData>();
		if (AnimData && AnimData->Transforms.Num() > 0)
		{
			// OptiTrack LiveLink often sets Transform[0] to Identity (Origin) for Skeletons.
			// Transform[1] is usually the Root/Hips.
			if (AnimData->Transforms.Num() > 1)
			{
				ResultTransform = AnimData->Transforms[1];
			}
			else
			{
				ResultTransform = AnimData->Transforms[0];
			}
			bFound = true;
		}
	}
    else if (LiveLinkClient.EvaluateFrame_AnyThread(SubjectKey.SubjectName, ULiveLinkCameraRole::StaticClass(), SubjectFrameData))
    {
        FLiveLinkCameraFrameData* CameraData = SubjectFrameData.FrameData.Cast<FLiveLinkCameraFrameData>();
        if (CameraData)
        {
            ResultTransform = CameraData->Transform;
            bFound = true;
        }
    }

	return ResultTransform;
}

bool UMoxOptiTrackSubsystem::IsOptiTrackSubject(const FLiveLinkSubjectKey& SubjectKey) const
{
	if (!IModularFeatures::Get().IsModularFeatureAvailable(ILiveLinkClient::ModularFeatureName))
	{
		return false;
	}

	ILiveLinkClient& LiveLinkClient = IModularFeatures::Get().GetModularFeature<ILiveLinkClient>(ILiveLinkClient::ModularFeatureName);
	
    // Check Source Type
    FText SourceType = LiveLinkClient.GetSourceType(SubjectKey.Source);
    // The OptiTrack plugin defines its source type as "OptiTrack"
    if (SourceType.ToString().Equals(TEXT("OptiTrack"), ESearchCase::IgnoreCase))
    {
        return true;
    }

	return false;
}
