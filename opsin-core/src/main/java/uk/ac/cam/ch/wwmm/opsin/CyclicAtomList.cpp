#include "CyclicAtomList.h"
#include "Atom.h"

namespace uk
{
	namespace ac
	{
		namespace cam
		{
			namespace ch
			{
				namespace wwmm
				{
					namespace opsin
					{

						CyclicAtomList::CyclicAtomList(std::vector<Atom*> &atomList) : atomList(atomList)
						{
						}

						CyclicAtomList::CyclicAtomList(std::vector<Atom*> &atomList, int index) : atomList(atomList)
						{
							setIndex(index);
						}

						int CyclicAtomList::size()
						{
							return atomList.size();
						}

						Atom *CyclicAtomList::get(int index) throw(std::out_of_range)
						{
							return atomList[index];
						}

						int CyclicAtomList::getIndex()
						{
							return index;
						}

						void CyclicAtomList::setIndex(int index)
						{
							if (index >= atomList.size())
							{
								throw std::invalid_argument("Specified index is not within ringAtom list");
							}
							this->index = index;
						}

						Atom *CyclicAtomList::next()
						{
							int tempIndex = index + 1;
							if (tempIndex >= atomList.size())
							{
								tempIndex = 0;
							}
							index = tempIndex;
							return atomList[index];
						}

						Atom *CyclicAtomList::previous()
						{
							int tempIndex = index - 1;
							if (tempIndex < 0)
							{
								tempIndex = atomList.size() - 1;
							}
							index = tempIndex;
							return atomList[index];
						}

						Atom *CyclicAtomList::peekNext()
						{
							int tempIndex = index + 1;
							if (tempIndex >= atomList.size())
							{
								tempIndex = 0;
							}
							return atomList[tempIndex];
						}

						Atom *CyclicAtomList::peekPrevious()
						{
							int tempIndex = index - 1;
							if (tempIndex < 0)
							{
								tempIndex = atomList.size() - 1;
							}
							return atomList[tempIndex];
						}

						Atom *CyclicAtomList::getCurrent()
						{
							return atomList[index];
						}
					}
				}
			}
		}
	}
}
