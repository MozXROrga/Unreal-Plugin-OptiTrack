# MoxOptiTrack

**Status:** Beta · **Suite version:** 3.0.0 · **Unreal Engine:** 5.7

MoxOptiTrack exposes existing OptiTrack Live Link subjects to Blueprint. It does not create an OptiTrack source and does not hard-code network settings.

## DEPRECATED

Use case for plugin was superseded (originally for [Unity](https://github.com/MozXROrga/Unity-StarterKit) Endpoint project)

## Requirements and activation

Enable MoxOptiTrack and the project’s OptiTrack/Live Link integration. Configure source and network values in the owning project; use its configuration keys and environment-specific values rather than literal addresses from documentation.

## Blueprint entry point

Get the MoxOptiTrackSubsystem from the game instance, then query an existing Live Link subject for supported transform, animation, or camera roles.

## Included content

BP_MozXP_OptiTrackManager is included as a project-facing content example.

## Troubleshooting

- No subjects: create and connect the Live Link source in the project first.
- Unsupported role: query only transform, animation, or camera subjects.

See [Quickstart](QUICKSTART.md), [C++ documentation](DOCUMENTATION.md), and [changelog](CHANGELOG.md).
