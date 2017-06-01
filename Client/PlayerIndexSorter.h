#pragma once
#include <vector>
#include <iostream>

#define UNIQUE_ID_NOT_FOUND 9999
#define INDEX_NOT_FOUND -1

class PlayerIndexSorter
{
public:
	PlayerIndexSorter();
	void addCombo(unsigned int UniqueId, int index);
	void removeCombo(unsigned int UniqueId);
	void removeCombo(int index);
	int getIndex(unsigned int UniqueId);
	unsigned int getUniqueId(int index);
	int getSorterIndex(unsigned int UniqueId);
	int getSorterIndex(int index);
	int getCombosSize() const;
	
	

private:
	void subtractAfterIndex(int index);
	class PlayerIndexPackage
	{
	public:
		PlayerIndexPackage();
		PlayerIndexPackage(unsigned int UniqueId, int index);
		unsigned int getUniqueId() const;
		int getPlayerIndex() const;
		void backstep();
	private:
		unsigned int PlayerUniqueId;
		int PlayerIndex;
	};
	std::vector <PlayerIndexPackage> Combos;
};