#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

#include "Inv_ItemFragment.generated.h"

USTRUCT(BlueprintType)
struct FInv_ItemFragment
{
	GENERATED_BODY()

	//스페셜 함수 이동 연산자 및 이동 할당 연산자 윤정현 교수님 도와줘!!

	FInv_ItemFragment() {} //기본생성자

	//복사연산자들
	FInv_ItemFragment(const FInv_ItemFragment&) = default; // 복사생성자
	FInv_ItemFragment& operator=(const FInv_ItemFragment&) = default; // 복사 할당 연산자

	//이동연산자들
	FInv_ItemFragment(FInv_ItemFragment&&) = default; // 이동 생성자
	FInv_ItemFragment& operator=(FInv_ItemFragment&&) = default; // 이동 할당 연산자

	virtual ~FInv_ItemFragment() {} // 소멸자인데 뭐 어차피 다 필요하잖아 (다형성 소멸자)

	FGameplayTag GetFragmentTag() const { return FragmentTag; } // 조각 태그 가져오기
	void SetFragmentTag(FGameplayTag Tag) { FragmentTag = Tag; } // 조각 태그 설정
private:

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FGameplayTag FragmentTag = FGameplayTag::EmptyTag; //조각 태그
};

USTRUCT(BlueprintType)
struct FInv_GridFragment : public FInv_ItemFragment
{
	GENERATED_BODY()

	FIntPoint GetGridSize() const { return GridSize; } //그리드 크기 얻기
	void SetGridSize(const FIntPoint& Size) { GridSize = Size; } //그리드 크기 설정
	float GetGridPadding() const { return GridPadding; } //그리드 패딩 얻기
	void SetGridPadding(float Padding) { GridPadding = Padding; } //그리드 패딩 설정

private:
	UPROPERTY(EditAnywhere, Category = "Inventory")
	FIntPoint GridSize{1, 1}; //그리드 크기

	UPROPERTY(EditAnywhere, Category = "Inventory")
	float GridPadding{ 0.f }; //그리드 패딩
};

