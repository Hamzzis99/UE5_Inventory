// Gihyeon's Inventory Project

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inv_SlottedItem.generated.h"

class UInv_InventoryItem;
class UImage;

UCLASS()
class INVENTORY_API UInv_SlottedItem : public UUserWidget
{
	GENERATED_BODY()

public:
	bool IsStackable() const { return bIsStackable; } // 스택 가능 여부 가져오기
	void SetIsStackable(bool bStackable) { bIsStackable = bStackable; } // 스택 가능 여부 설정
	UImage* GetImageIcon() const {return Image_Icon;} // 아이콘 이미지 가져오기
	void SetGridIndex(int32 Index) { GridIndex = Index; } // 그리드 인덱스 설정
	int32 GetGridIndex() const { return GridIndex; } // 그리드 인덱스 가져오기
	void SetGridDimensions(const FIntPoint& Dimensions) { GridDimensions = Dimensions; } // 그리드 크기 설정
	FIntPoint GetGridDimensions() const { return GridDimensions; } // 그리드 크기 가져오기
	void SetInventoryItem(UInv_InventoryItem* Item) { InventoryItem = Item; } // 인벤토리 아이템 설정
	UInv_InventoryItem* GetInventoryItem() const { return InventoryItem.Get(); } // 인벤토리 아이템 가져오기
	void SetImageBrush(const FSlateBrush& Brush) const; // 이미지 브러시 설정

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_Icon;

	int32 GridIndex;
	FIntPoint GridDimensions;
	TWeakObjectPtr<UInv_InventoryItem> InventoryItem;
	bool bIsStackable{ false };
};
