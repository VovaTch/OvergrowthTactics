#include "GridUtils.h"

FGridShapeData *GetGridShapeRow(UDataTable *GridShapeData, EGridShape Shape) {
    FName RowName = FName(UEnum::GetDisplayValueAsText(Shape).ToString());
    const FString ContextString(TEXT("Grid Shape Data Context"));
    return GridShapeData->FindRow<FGridShapeData>(RowName, ContextString);
}
