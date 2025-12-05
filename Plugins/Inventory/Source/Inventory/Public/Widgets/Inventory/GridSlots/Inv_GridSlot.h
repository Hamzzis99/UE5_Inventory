// Gihyeon's Inventory Project


#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inv_GridSlot.generated.h"

class UImage;

UENUM(BlueprintType)
enum class EInv_GridSlotState : uint8
{
	Unoccupied,
	Occupied,
	Selected,
	GrayedOut
}; // 4가지의 이미지

UCLASS()
class INVENTORY_API UInv_GridSlot : public UUserWidget
{
	GENERATED_BODY()
public:
	// Getter Setter 부분들.
	void SetTileIndex(int32 Index) { TileIndex = Index; } // 타일 인덱스 설정
	int32 GetTileIndex() const { return TileIndex; } //	타일 인덱스 반환
	EInv_GridSlotState GetGridSlotState() const { return GridSlotState; } // 현재 상태 반환
	TWeakObjectPtr<UInv_InventoryItem> GetInventoryItem() const { return InventoryItem; } // 인벤토리 아이템 반환
	void SetInventoryItem(UInv_InventoryItem* Item); // 인벤토리 아이템 설정
	int32 GetStackCount() const { return StackCount; } // 스택 카운트 반환
	void SetStackCount(int32 Count) { StackCount = Count; } // 스택 카운트 설정
	int32 GetIndex() const { return TileIndex; } // 인덱스 반환
	void SetIndex(int32 Index) { TileIndex = Index; } // 인덱스 설정

	//아이템 먹을수록 왼쪽부터 채워주는 함수들 만들기
	int32 GetUpperLeftIndex() const { return UpperLeftIndex; } // 왼쪽 위 인덱스 반환
	void SetUpperLeftIndex(int32 Index) { UpperLeftIndex = Index; } // 왼쪽 위 인덱스 설정
	bool IsAvailable() const { return bAvailable; } // 사용 가능 여부 반환
	void SetAvailable(bool bIsAvailable) { bAvailable = bIsAvailable; } // 사용 가능 여부 설정

	void SetOccupiedTexture();
	void SetUnoccupiedTexture();
	void SetSelectedTexture();
	void SetGrayedOutTexture();

private:
	int32 TileIndex{ INDEX_NONE };
	int32 StackCount{ 0 };
	int32 UpperLeftIndex{ INDEX_NONE }; // 정사각형인지 확인해주는 것인가?
	TWeakObjectPtr<UInv_InventoryItem> InventoryItem;
	bool bAvailable{ true };

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_GridSlot;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FSlateBrush Brush_Unoccupied;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FSlateBrush Brush_Occupied;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FSlateBrush Brush_Selected;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FSlateBrush Brush_GrayedOut;

	EInv_GridSlotState GridSlotState;

};