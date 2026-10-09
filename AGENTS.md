# OvergrowthTactics

UE 5.7 C++ project following Alex Quevillon's "Tactical Combat" YouTube series (turn-based grid tactics), reimplementing his Blueprint logic in C++ where practical.

## Progress
- Episode 20 (Actions System) done: `AAction`, `AAction_SelectTile`, `APlayerActions::SetSelectedAction`, `OnSelectedActionsChanged` delegate, `W_ActionButton` widget.
- In progress: Episode 21 (More Grid Actions), partially implemented (e.g. `AAction_AddTile` exists).

## Working style
The user writes most code themselves to learn C++ and UE5. Default to explaining, reviewing, and pointing to the right APIs or files. Write or edit code only when asked.

## Planned
Move to Unreal 5.8 to use its Unreal MCP. Until the switch happens, paths and commands here still target 5.7.

## Build
Close the editor first (Live Coding blocks command-line builds):
```
"C:\Program Files\Epic Games\UE_5.7\Engine\Build\BatchFiles\Build.bat" OvergrowthTacticsEditor Win64 Development "-Project=C:\Users\PC\Documents\Unreal Projects\OvergrowthTactics\OvergrowthTactics.uproject" -WaitMutex
```

## Gotchas
- Live Coding patches are discarded on editor restart. Any header change (new UPROPERTY/UFUNCTION/delegate/class) needs a full build with the editor closed, otherwise the editor crashes or loads a stale DLL and Blueprints referencing new C++ members turn red.
- Don't save Blueprints/widgets that show "class not found" errors; rebuild C++ first.
- Logs: `Saved/Logs/OvergrowthTactics.log`, crashes: `Saved/Crashes/`.
