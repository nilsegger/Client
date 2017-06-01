#include "PlayerIndexSorter.h"

PlayerIndexSorter::PlayerIndexSorter()
{
}

void PlayerIndexSorter::addCombo(unsigned int UniqueId, int index)
{
	Combos.push_back(PlayerIndexPackage(UniqueId, index));
}

void PlayerIndexSorter::removeCombo(unsigned int UniqueId)
{
	int i = getSorterIndex(UniqueId);

	if (i != INDEX_NOT_FOUND) {
		Combos.erase(Combos.begin() + i);
		subtractAfterIndex(i);
	}
	else {
		std::cout << "Package searched by Unique Id not found..." << std::endl;
	}
}

void PlayerIndexSorter::removeCombo(int index)
{
	int i = getSorterIndex(index);

	if (i != INDEX_NOT_FOUND) {
		Combos.erase(Combos.begin() + i);
		subtractAfterIndex(i);
	}
	else {
		std::cout << "Package searched by Unique Id not found..." << std::endl;
	}
}

int PlayerIndexSorter::getIndex(unsigned int UniqueId)
{
	for (int i = 0; i < int(Combos.size()); i++) {
		if (Combos[i].getUniqueId() == UniqueId)
			return Combos[i].getPlayerIndex();
	}
	//std::cout << "Index not found" << std::endl;
	return INDEX_NOT_FOUND;
}

unsigned int PlayerIndexSorter::getUniqueId(int index)
{
	for (int i = 0; i < int(Combos.size()); i++) {
		if (Combos[i].getPlayerIndex() == index)
			return Combos[i].getUniqueId();
	}
	//std::cout << "Unique Id not found" << std::endl;
	return UNIQUE_ID_NOT_FOUND;
}

int PlayerIndexSorter::getSorterIndex(unsigned int UniqueId)
{
	for (int i = 0; i < int(Combos.size()); i++) {
		if (Combos[i].getUniqueId() == UniqueId)
			return i;
	}
	return INDEX_NOT_FOUND;
}

int PlayerIndexSorter::getSorterIndex(int index)
{
	for (int i = 0; i < int(Combos.size()); i++) {
		if (Combos[i].getPlayerIndex() == index)
			return i;
	}
	return INDEX_NOT_FOUND;
}

int PlayerIndexSorter::getCombosSize() const
{
	return int(Combos.size());
}

void PlayerIndexSorter::subtractAfterIndex(int index)
{
	for (int i = 0; i < int(Combos.size()); i++) {
		if (Combos[i].getPlayerIndex() > i) {
			Combos[i].backstep();
		}
	}

}

PlayerIndexSorter::PlayerIndexPackage::PlayerIndexPackage()
{
}

PlayerIndexSorter::PlayerIndexPackage::PlayerIndexPackage(unsigned int UniqueId, int index)
	:PlayerUniqueId(UniqueId),
	PlayerIndex(index)
{
}

unsigned int PlayerIndexSorter::PlayerIndexPackage::getUniqueId() const
{
	return PlayerUniqueId;
}

int PlayerIndexSorter::PlayerIndexPackage::getPlayerIndex() const
{
	return PlayerIndex;
}

void PlayerIndexSorter::PlayerIndexPackage::backstep()
{
	PlayerIndex--;
}
