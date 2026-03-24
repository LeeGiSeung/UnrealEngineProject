#pragma once

#include "CoreMinimal.h"
#include "MinimapType.generated.h"

UENUM(Blueprinttype)
enum class EnumMinimapType : uint8
{
	Road, //길
	Shop, //상점
	Boss, //보스
	Mineral, //광물
	NPC,
	End
};
