# MoxOptiTrack C++ Documentation

## Module and dependencies

MoxOptiTrack provides UMoxOptiTrackSubsystem, a game-instance subsystem that reads already registered Live Link OptiTrack subjects.

## Public API and data flow

The subsystem resolves a named subject through Live Link and exposes data for transform, animation, and camera roles. Data flows from the project-created source to Live Link, then to the subsystem; this plugin does not own source creation or network transport.

## Lifecycle and failures

Obtain the subsystem after the game instance is available. Missing sources, missing subjects, and incompatible roles result in unavailable data and must be handled by the caller.

## Configuration and extension

There is no source-creation schema in v3.0.0. Keep endpoint and source configuration project-specific; never embed site addresses in code or documentation.
