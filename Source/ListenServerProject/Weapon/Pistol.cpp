#include "Weapon/Pistol.h"

APistol::APistol()
{

}

void APistol::Equip()
{
	printf("자식 Equip 호출");

	UE_LOG(LogTemp, Warning, TEXT("자식 Equip 호출"));
}
