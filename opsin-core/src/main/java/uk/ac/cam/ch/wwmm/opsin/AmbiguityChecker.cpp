#include "AmbiguityChecker.h"
#include "Atom.h"
#include "StereoAnalyser.h"
#include "Bond.h"
#include "StructureBuildingMethods.h"
#include "ChemEl.h"

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
//						import static uk.ac.cam.ch.wwmm.opsin.XmlDeclarations.ELEMENTARYATOM_SUBTYPE_VAL;

						bool AmbiguityChecker::isSubstitutionAmbiguous(std::vector<Atom*> &substitutableAtoms, int numberToBeSubstituted)
						{
							if (substitutableAtoms.empty())
							{
								throw std::invalid_argument("OPSIN Bug: Must provide at least one substituable atom");
							}
							if (substitutableAtoms.size() < numberToBeSubstituted)
							{
								throw std::invalid_argument("OPSIN Bug: substitutableAtoms must be >= numberToBeSubstituted");
							}
							if (substitutableAtoms.size() == numberToBeSubstituted)
							{
								return false;
							}
							if (allAtomsConnectToDefaultInAtom(substitutableAtoms, numberToBeSubstituted))
							{
								return false;
							}
							Set<Atom*> *uniqueAtoms = std::unordered_set<Atom*>(substitutableAtoms);
							if (uniqueAtoms->size() == 1)
							{
								return false;
							}
							if (allAtomsEquivalent(uniqueAtoms) && (numberToBeSubstituted == 1 || numberToBeSubstituted == substitutableAtoms.size() - 1))
							{
								return false;
							}
							return true;
						}

						bool AmbiguityChecker::allAtomsEquivalent(Collection<Atom*> *atoms)
						{
							StereoAnalyser *analyser = analyseRelevantAtomsAndBonds(atoms);
							Set<std::wstring> *uniqueEnvironments = std::unordered_set<std::wstring>();
							for (auto a : atoms)
							{
								uniqueEnvironments->add(getAtomEnviron(analyser, a));
							}
							return uniqueEnvironments->size() == 1;
						}

						bool AmbiguityChecker::allBondsEquivalent(Collection<Bond*> *bonds)
						{
							Set<Atom*> *relevantAtoms = std::unordered_set<Atom*>();
							for (auto b : bonds)
							{
								relevantAtoms->add(b->getFromAtom());
								relevantAtoms->add(b->getToAtom());
							}
							StereoAnalyser *analyser = analyseRelevantAtomsAndBonds(relevantAtoms);
							Set<std::wstring> *uniqueBonds = std::unordered_set<std::wstring>();
							for (auto b : bonds)
							{
								uniqueBonds->add(bondToCanonicalEnvironString(analyser, b));
							}
							return uniqueBonds->size() == 1;
						}

						std::wstring AmbiguityChecker::bondToCanonicalEnvironString(StereoAnalyser *analyser, Bond *b)
						{
							std::wstring s1 = getAtomEnviron(analyser, b->getFromAtom());
							std::wstring s2 = getAtomEnviron(analyser, b->getToAtom());
							if (s1.compare(s2) > 0)
							{
								return s1 + s2;
							}
							else
							{
								return s2 + s1;
							}
						}

						std::wstring AmbiguityChecker::getAtomEnviron(StereoAnalyser *analyser, Atom *a)
						{
							boost::optional<int> env = analyser->getAtomEnvironmentNumber(a);
							if (!env)
							{
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("OPSIN Bug: Atom was not part of ambiguity analysis");
								throw std::exception();
							}
							//"identical" atoms may be distinguished by bonds yet to be formed, hence split by outvalency
							// e.g. [PH3] vs [PH3]=
							return env + std::wstring(L"\t") + std::to_wstring(a->getOutValency());
						}

						bool AmbiguityChecker::allAtomsConnectToDefaultInAtom(std::vector<Atom*> &substitutableAtoms, int numberToBeSubstituted)
						{
							Atom *defaultInAtom = substitutableAtoms[0]->getFrag().getDefaultInAtom();
							if (defaultInAtom != nullptr)
							{
								for (int i = 0; i < numberToBeSubstituted; i++)
								{
									if (!substitutableAtoms[i]->equals(defaultInAtom))
									{
										return false;
									}
								}
								return true;
							}
							return false;
						}

						StereoAnalyser *AmbiguityChecker::analyseRelevantAtomsAndBonds(Collection<Atom*> *startingAtoms)
						{
							Set<Atom*> *atoms = std::unordered_set<Atom*>();
							Set<Bond*> *bonds = std::unordered_set<Bond*>();
							Deque<Atom*> *stack = new ArrayDeque<Atom*>(startingAtoms);
							while (!stack->isEmpty())
							{
								Atom *a = stack->removeLast();
								if (!atoms->contains(a))
								{
									atoms->add(a);
									for (auto b : a->getBonds())
									{
										bonds->add(b);
										stack->add(b->getOtherAtom(a));
									}
								}
							}

							std::vector<Atom*> ghostHydrogens;
							for (auto atom : atoms)
							{
								if (atom->getFrag()->getSubType() == ELEMENTARYATOM_SUBTYPE_VAL)
								{ //these do not have implicit hydrogen e.g. phosphorus is literally just a phosphorus atom
									continue;
								}
								int explicitHydrogensToAdd = StructureBuildingMethods::calculateSubstitutableHydrogenAtoms(atom);
								for (int i = 0; i < explicitHydrogensToAdd; i++)
								{
									Atom *ghostHydrogen = new Atom(ChemEl::H);
									Bond *b = new Bond(ghostHydrogen, atom, 1);
									atom->addBond(b);
									ghostHydrogen->addBond(b);
									ghostHydrogens.push_back(ghostHydrogen);
								}
							}
							atoms->addAll(ghostHydrogens);
							StereoAnalyser *analyzer = new StereoAnalyser(atoms, bonds);
							for (auto ghostHydrogen : ghostHydrogens)
							{
								Bond *b = ghostHydrogen->getFirstBond();
								b->getOtherAtom(ghostHydrogen)->removeBond(b);
							}
							return analyzer;
						}

						std::vector<Atom*> AmbiguityChecker::useAtomEnvironmentsToGivePlausibleSubstitution(std::vector<Atom*> &substitutableAtoms, int numberToBeSubstituted)
						{
							if (substitutableAtoms.empty())
							{
								throw std::invalid_argument("OPSIN Bug: Must provide at least one substituable atom");
							}
							if (substitutableAtoms.size() < numberToBeSubstituted)
							{
								throw std::invalid_argument("OPSIN Bug: substitutableAtoms must be >= numberToBeSubstituted");
							}
							if (substitutableAtoms.size() == numberToBeSubstituted)
							{
								return substitutableAtoms;
							}

							std::vector<Atom*> preferredAtoms = findPlausibleSubstitutionPatternUsingSymmmetry(substitutableAtoms, numberToBeSubstituted);
							if (preferredAtoms.size() > 0)
							{
								return preferredAtoms;
							}
							return findPlausibleSubstitutionPatternUsingLocalEnvironment(substitutableAtoms, numberToBeSubstituted);
						}

						std::vector<Atom*> AmbiguityChecker::findPlausibleSubstitutionPatternUsingSymmmetry(std::vector<Atom*> &substitutableAtoms, int numberToBeSubstituted)
						{
							//cf. octaethylporphyrin (8 identical atoms capable of substitution)
							StereoAnalyser *analyser = analyseRelevantAtomsAndBonds(std::unordered_set<Atom*>(substitutableAtoms));
							std::unordered_map<std::wstring, std::vector<Atom*>> atomsInEachEnvironment;
							for (auto a : substitutableAtoms)
							{
								std::wstring env = getAtomEnviron(analyser, a);
								std::vector<Atom*> atomsInEnvironment = atomsInEachEnvironment[env];
								if (atomsInEnvironment.empty())
								{
									atomsInEnvironment = std::vector<Atom*>();
									atomsInEachEnvironment[env] = atomsInEnvironment;
								}
								atomsInEnvironment.push_back(a);
							}
							std::vector<Atom*> preferredAtoms;
							for (auto atoms : atomsInEachEnvironment)
							{
								if (atoms.second->size() == numberToBeSubstituted)
								{
									if (preferredAtoms.size() > 0)
									{
										return nullptr;
									}
									preferredAtoms = atoms.second;
								}
							}
							if (preferredAtoms.empty())
							{
								//check for environments with double the required atoms where this means each atom can support two substitutions c.f. cyclohexane
								for (auto atoms : atomsInEachEnvironment)
								{
									if (atoms.second->size() == (numberToBeSubstituted * 2))
									{
										Set<Atom*> *uniquified = new LinkedHashSet<Atom*>(atoms.second); //retain deterministic atom ordering
										if (uniquified->size() == numberToBeSubstituted)
										{
											if (preferredAtoms.size() > 0)
											{
												return nullptr;
											}
											preferredAtoms = std::vector<Atom*>(uniquified);
										}
									}
								}
							}
							return preferredAtoms;
						}

						std::vector<Atom*> AmbiguityChecker::findPlausibleSubstitutionPatternUsingLocalEnvironment(std::vector<Atom*> &substitutableAtoms, int numberToBeSubstituted)
						{
							//cf. pentachlorotoluene (5 sp2 carbons vs sp3 methyl)
							std::unordered_map<std::wstring, std::vector<Atom*>> atomsInEachLocalEnvironment;
							for (auto a : substitutableAtoms)
							{
								int valency = a->determineValency(true);
								int currentValency = a->getIncomingValency() + a->getOutValency();
								int numOfBonds = (valency - currentValency) + a->getBondCount(); //distinguish sp2 and sp3 atoms
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
								std::wstring s = a->getElement()::toString() + std::wstring(L"\t") + valency + std::wstring(L"\t") + numOfBonds + std::wstring(L"\t") + StringHelper::toString(a->hasSpareValency());
								std::vector<Atom*> atomsInEnvironment = atomsInEachLocalEnvironment[s];
								if (atomsInEnvironment.empty())
								{
									atomsInEnvironment = std::vector<Atom*>();
									atomsInEachLocalEnvironment[s] = atomsInEnvironment;
								}
								atomsInEnvironment.push_back(a);
							}
							std::vector<Atom*> preferredAtoms;
							for (auto atoms : atomsInEachLocalEnvironment)
							{
								if (atoms.second->size() == numberToBeSubstituted)
								{
									if (preferredAtoms.size() > 0)
									{
										return nullptr;
									}
									preferredAtoms = atoms.second;
								}
							}
							return preferredAtoms;
						}
					}
				}
			}
		}
	}
}
