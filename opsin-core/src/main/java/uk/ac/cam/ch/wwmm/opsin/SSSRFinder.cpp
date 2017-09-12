#include "SSSRFinder.h"
#include "Fragment.h"
#include "Ring.h"
#include "Atom.h"
#include "Bond.h"

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

						std::vector<Ring*> SSSRFinder::getSetOfSmallestRings(Fragment *frag)
						{
							std::vector<Atom*> atomSet = frag->getAtomList();

							std::vector<Ring*> ringList = getRings(atomSet);

							if (ringList.size() > 1)
							{
								bool change = true;
								while (change)
								{
									for (int i = 0; i < ringList.size(); i++)
									{
										Ring *ring = ringList[i];
										change = reduceRingSizes(ring, ringList);
									}
								}
							}
							return ringList;
						}

						std::vector<Ring*> SSSRFinder::getRings(std::vector<Atom*> &atomSet)
						{
							std::vector<Ring*> ringList;
							Set<Atom*> *usedAtoms = std::unordered_set<Atom*>();

							Atom *root = atomSet[0];
							Atom *parentAtom = nullptr;
							std::unordered_map<Atom*, Atom*> atomToParentMap;
							Set<Bond*> *linkBondSet = new LinkedHashSet<Bond*>();

							expand(root, parentAtom, usedAtoms, atomToParentMap, linkBondSet);

							for (auto bond : linkBondSet)
							{
								Ring *ring = getRing(bond, atomToParentMap);
								ringList.push_back(ring);
							}

							return ringList;
						}

						Ring *SSSRFinder::getRing(Bond *bond, std::unordered_map<Atom*, Atom*> &atomToParentMap)
						{
							Atom *atomFrom = bond->getFromAtom();
							Atom *atomTo = bond->getToAtom();
							std::vector<Bond*> bondSet0 = getAncestors1(atomFrom, atomToParentMap);
							std::vector<Bond*> bondSet1 = getAncestors1(atomTo, atomToParentMap);
							std::vector<Bond*> mergedBondSet = symmetricDifference(bondSet0, bondSet1);

							mergedBondSet.push_back(bond);
							return new Ring(mergedBondSet);
						}

						std::vector<Bond*> SSSRFinder::getAncestors1(Atom *atom, std::unordered_map<Atom*, Atom*> &atomToParentMap)
						{
							std::vector<Bond*> newBondSet;
							while (true)
							{
								Atom *atom1 = atomToParentMap[atom];
								if (atom1 == nullptr)
								{
									break;
								}
								Bond *bond = atom->getBondToAtom(atom1);
								if (std::find(newBondSet.begin(), newBondSet.end(), bond) != newBondSet.end())
								{
									break;
								}
								newBondSet.push_back(bond);
								atom = atom1;
							}
							return newBondSet;
						}

						void SSSRFinder::expand(Atom *atom, Atom *parentAtom, Set<Atom*> *usedAtoms, std::unordered_map<Atom*, Atom*> &atomToParentMap, Set<Bond*> *linkBondSet)
						{

							usedAtoms->add(atom);
							atomToParentMap[atom] = parentAtom;
							std::vector<Atom*> ligandAtomList = atom->getAtomNeighbours();

							for (auto ligandAtom : ligandAtomList)
							{
								if (ligandAtom->equals(parentAtom))
								{
									// skip existing bond
								}
								else if (usedAtoms->contains(ligandAtom))
								{
									Bond *linkBond = atom->getBondToAtom(ligandAtom);
									linkBondSet->add(linkBond);
									// already treated
								}
								else
								{
									expand(ligandAtom, atom, usedAtoms, atomToParentMap, linkBondSet);
								}
							}
						}

						bool SSSRFinder::reduceRingSizes(Ring *ring, std::vector<Ring*> &newList)
						{
							bool change = false;
							for (int i = 0; i < newList.size(); i++)
							{
								Ring *target = newList[i];
								if (target == ring)
								{
									continue;
								}

								std::vector<Bond*> newBondSet = symmetricDifference(target->getBondList(), ring->getBondList());
								if (newBondSet.size() < target->size())
								{
									Ring *newRing = new Ring(newBondSet);
									newList[i] = newRing;
									change = true;
								}
							}
							return change;
						}

						std::vector<Bond*> SSSRFinder::symmetricDifference(std::vector<Bond*> &bondSet1, std::vector<Bond*> &bondSet2)
						{
							std::vector<Bond*> newBondSet;

							for (auto bond1 : bondSet1)
							{
								if (!std::find(bondSet2.begin(), bondSet2.end(), bond1) != bondSet2.end())
								{
									newBondSet.push_back(bond1);
								}
							}
							for (auto bond2 : bondSet2)
							{
								if (!std::find(bondSet1.begin(), bondSet1.end(), bond2) != bondSet1.end())
								{
									newBondSet.push_back(bond2);
								}
							}
							return newBondSet;
						}
					}
				}
			}
		}
	}
}
