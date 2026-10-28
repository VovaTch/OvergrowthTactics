# OvergrowthTactics

UE 5.8 C++ project following Alex Quevillon's "Tactical Combat" YouTube series (turn-based grid tactics), reimplementing his Blueprint logic in C++ where practical.

## Progress
- Episode 20 (Actions System) done: `AAction`, `AAction_SelectTile`, `APlayerActions::SetSelectedAction`, `OnSelectedActionsChanged` delegate, `W_ActionButton` widget.
- Episode 21 (More Grid Actions) done: `AAction_AddTile`, `AAction_RemoveTile`, hold-to-drag for left/right click (`APlayerActions` re-executes the action when the hovered tile changes while a button is held), `AAction_IncreaseTileHeight` / `AAction_DecreaseTileHeight` (read the stored tile with `AGrid::GetTileData`, shift Z by `TileSize.Z`, write back with `AddGridTile`), `AAction_SelectTileType` (the user's own design, differs from the video; Blueprint actions in `Content/Blueprints/Debug/Actions`). Buttons live in `W_Tab_Grid`.
- Episode 22 (Adding Text On The Tiles) done: `ADebugTextOnTiles` (Blueprint child `BP_DebugTextOnTiles`, a child actor of `BP_DebugMenu`) keeps one `ATextRenderActor` per walkable tile showing its index, toggled by `bShowTileIndices` / `SetShowTileIndices`. It binds `UpdateTextOnTile` to `AGrid::OnTileDataUpdated` (broadcast from `AddGridTile` / `RemoveGridTile` / tile state changes) and `ClearAllTextActors` to `AGrid::OnGridDestroyed` (broadcast from `DestroyGrid`, so stale text is cleared when the grid shape changes). Both delegates are public.
- Episode 23 (Tile Neighbors) done: `AGrid::GetNeighbors(FIntPoint)` switches on `GridShape` over per-shape offsets (hex parity on `Index.Y`, triangle third neighbor on `(X+Y)&1`) and filters with `IsIndexValid`. New `ETileState::IsNeighbor` with its own color/opacity in `AGridMeshInstance` (hierarchy Selected > Hovered > IsNeighbor > None). `APlayerActions` broadcasts `OnHoveredTileChanged(FIntPoint)` from `UpdateTileUnderCursor`; `AAction_ShowTileNeighbors` binds to it in `BeginPlay` (owner cast, since `SpawnActor` runs `BeginPlay` before `PlayerActions` is assigned), so neighbors show on hover. It tracks highlighted tiles in `SavedNeighbors` and clears them on hover change and in `EndPlay`. Button lives in `W_Tab_Pathfinding`.
- Next: Episode 24.
- Known loose ends:
  - `AGrid::GetTileDataFromIndex` uses an identity rotation for all tiles; alternating triangle tiles need a 180° yaw. Keep it consistent with the triangle neighbor parity in `GetNeighbors`.
  - `UDataTable::FindRow` warns about row `None` in `T_GridShapeData` (lookup runs while `GridShape` is `None`).

## Working style
The user writes most code themselves to learn C++ and UE5. Default to explaining, reviewing, and pointing to the right APIs or files. Write or edit code only when asked.

## Build
Close the editor first (Live Coding blocks command-line builds):
```
"C:\Program Files\Epic Games\UE_5.8\Engine\Build\BatchFiles\Build.bat" OvergrowthTacticsEditor Win64 Development "-Project=C:\Users\PC\Documents\Unreal Projects\OvergrowthTactics\OvergrowthTactics.uproject" -WaitMutex
```

## Gotchas
- Live Coding patches are discarded on editor restart. Any header change (new UPROPERTY/UFUNCTION/delegate/class) needs a full build with the editor closed, otherwise the editor crashes or loads a stale DLL and Blueprints referencing new C++ members turn red.
- New `UCLASS`es added only through Live Coding are unreliable: widget/Blueprint references to them can silently drop (this unhooked the height buttons in `W_Tab_Grid`). Do a full build after adding a class.
- Don't save Blueprints/widgets that show "class not found" errors; rebuild C++ first.
- Logs: `Saved/Logs/OvergrowthTactics.log`, crashes: `Saved/Crashes/`.
