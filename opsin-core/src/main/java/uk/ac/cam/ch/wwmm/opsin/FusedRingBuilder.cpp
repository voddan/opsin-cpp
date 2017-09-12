#include "FusedRingBuilder.h"
#include "BuildState.h"
#include "Element.h"
#include "Fragment.h"
#include "Atom.h"
#include "StructureBuildingException.h"
#include "XmlDeclarations.h"
#include "OpsinTools.h"
#include "FragmentTools.h"
#include "StringTools.h"
#include "FusedRingNumberer.h"
#include "Bond.h"
#include "StructureBuildingMethods.h"
#include "CyclicAtomList.h"
#include "ChemEl.h"
#include "ComponentProcessor.h"

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
						using namespace uk::ac::cam::ch::wwmm::opsin;
//						import static uk.ac.cam.ch.wwmm.opsin.OpsinTools.*;
//						import static uk.ac.cam.ch.wwmm.opsin.XmlDeclarations.*;

						FusedRingBuilder::FusedRingBuilder(BuildState *state, std::vector<Element*> &groupsInFusedRing) : state(state), groupsInFusedRing(groupsInFusedRing), lastGroup(groupsInFusedRing[groupsInFusedRing.size() - 1]), parentRing(lastGroup->getFrag())
						{
							fragmentInScopeForEachFusionLevel[0] = parentRing;
						}

						void FusedRingBuilder::processFusedRings(BuildState *state, Element *subOrRoot) throw(StructureBuildingException)
						{
							std::vector<Element*> groups = subOrRoot->getChildElements(XmlDeclarations::GROUP_EL);
							if (groups.size() < 2)
							{
								return; //nothing to fuse
							}
							std::vector<Element*> groupsInFusedRing;
							for (int i = groups.size() - 1; i >= 0; i--)
							{ //group groups into fused rings
								Element *group = groups[i];
								groupsInFusedRing.push_back(0, group);
								if (i != 0)
								{
									Element *startingEl = group;
									if ((group->getValue() == L"benz" || group->getValue() == L"benzo") && XmlDeclarations::FUSIONRING_SUBTYPE_VAL == group->getAttributeValue(XmlDeclarations::SUBTYPE_ATR))
									{
										Element *beforeBenzo = OpsinTools::getPreviousSibling(group);
										if (beforeBenzo != nullptr && beforeBenzo->getName() == XmlDeclarations::LOCANT_EL)
										{
											startingEl = beforeBenzo;
										}
									}
									Element *possibleGroup = OpsinTools::getPreviousSiblingIgnoringCertainElements(startingEl, std::vector<std::wstring>{XmlDeclarations::MULTIPLIER_EL, XmlDeclarations::FUSION_EL});
									if (!groups[i - 1]->equals(possibleGroup))
									{ //end of fused ring system
										if (groupsInFusedRing.size() >= 2)
										{
											//This will be invoked in cases where there are multiple fused ring systems in the same subOrRoot such as some spiro systems
											FusedRingBuilder tempVar(state, groupsInFusedRing);
											(&tempVar)->buildFusedRing();
										}
										groupsInFusedRing.clear();
									}
								}
							}
							if (groupsInFusedRing.size() >= 2)
							{
								FusedRingBuilder tempVar2(state, groupsInFusedRing);
								(&tempVar2)->buildFusedRing();
							}
						}

						void FusedRingBuilder::buildFusedRing() throw(StructureBuildingException)
						{
							/*
							 * Apply any nonstandard ring numbering, sorts atomOrder by locant
							 * Aromatises appropriate cycloalkane rings, Rejects groups with acyclic atoms
							 */
							processRingNumberingAndIrregularities();
							processBenzoFusions(); //FR-2.2.8  e.g. in 2H-[1,3]benzodioxino[6',5',4':10,5,6]anthra[2,3-b]azepine  benzodioxino is one component
							std::vector<Element*> nameComponents = formNameComponentList();
//JAVA TO C++ CONVERTER TODO TASK: The Java Collection 'remove(Object)' method is not converted:
							nameComponents.remove(lastGroup);

							std::vector<Fragment*> componentFragments; //all the ring fragments (other than the parentRing). These will later be merged into the parentRing
							std::vector<Fragment*> parentFragments;
							parentFragments.push_back(parentRing);

							int numberOfParents = 1;
							Element *possibleMultiplier = OpsinTools::getPreviousSibling(lastGroup);
							if (nameComponents.size() > 0 && possibleMultiplier != nullptr && possibleMultiplier->getName() == XmlDeclarations::MULTIPLIER_EL)
							{
								numberOfParents = std::stoi(possibleMultiplier->getAttributeValue(XmlDeclarations::VALUE_ATR));
								possibleMultiplier->detach();
								for (int j = 1; j < numberOfParents; j++)
								{
									Fragment *copyOfParentRing = state->fragManager->copyFragment(parentRing);
									parentFragments.push_back(copyOfParentRing);
									componentFragments.push_back(copyOfParentRing);
								}
							}

							/*The indice from nameComponents to use next. Work from right to left i.e. starts at nameComponents.size()-1*/
							int ncIndice = processMultiParentSystem(parentFragments, nameComponents, componentFragments); //handle multiparent systems
							/*
							 * The number of primes on the component to be connected. 
							 * This is initially 0 indicating fusion of unprimed locants with the letter locants of the parentRing
							 * Subsequently it will switch to 1 indicating fusion of a second order component (primed locants) with a 
							 * first order component (unprimed locants)
							 * Next would be double primed fusing to single primed locants etc.
							 * 
							 */
							int fusionLevel = (nameComponents.size() - 1 - ncIndice) / 2;
							for (; ncIndice >= 0; ncIndice--)
							{
								Element *fusion = nullptr;
								if (nameComponents[ncIndice]->getName() == XmlDeclarations::FUSION_EL)
								{
									fusion = nameComponents[ncIndice--];
								}
								if (ncIndice < 0 || nameComponents[ncIndice]->getName() != XmlDeclarations::GROUP_EL)
								{
									throw StructureBuildingException(L"Group not found where group expected. This is probably a bug");
								}
								Fragment *nextComponent = nameComponents[ncIndice]->getFrag();
								int multiplier = 1;
								Element *possibleMultiplierEl = OpsinTools::getPreviousSibling(nameComponents[ncIndice]); //e.g. the di of difuro
								if (possibleMultiplierEl != nullptr && possibleMultiplierEl->getName() == XmlDeclarations::MULTIPLIER_EL)
								{
									multiplier = std::stoi(possibleMultiplierEl->getAttributeValue(XmlDeclarations::VALUE_ATR));
								}
								std::vector<std::wstring> fusionDescriptors = nullptr;
								if (fusion != nullptr)
								{
									std::wstring fusionDescriptorString = fusion->getValue().toLowerCase(Locale::ROOT)->substr(1, (fusion->getValue().length() - 1) - 1);
									if (multiplier == 1)
									{
										fusionDescriptors = std::vector<std::wstring>{fusionDescriptorString};
									}
									else
									{
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
										if (fusionDescriptorString.split(L";")->length > 1)
										{
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
											fusionDescriptors = fusionDescriptorString.split(L";");
										}
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
										else if (fusionDescriptorString.split(L":")->length > 1)
										{
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
											fusionDescriptors = fusionDescriptorString.split(L":");
										}
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
										else if (fusionDescriptorString.split(L",")->length > 1)
										{
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
											fusionDescriptors = fusionDescriptorString.split(L",");
										}
										else
										{ //multiplier does not appear to mean multiplied component. Could be indicating multiplication of the whole fused ring system
											if (ncIndice != 0)
											{
												throw StructureBuildingException(std::wstring(L"Unexpected multiplier: ") + possibleMultiplierEl->getValue() + std::wstring(L" or incorrect fusion descriptor: ") + fusionDescriptorString);
											}
											multiplier = 1;
											fusionDescriptors = std::vector<std::wstring>{fusionDescriptorString};
										}
									}
								}
								if (multiplier > 1)
								{
									possibleMultiplierEl->detach();
								}
								std::vector<Fragment*> fusionComponents(multiplier);
								for (int j = 0; j < multiplier; j++)
								{
									if (j > 0)
									{
										fusionComponents[j] = state->fragManager->copyAndRelabelFragment(nextComponent, j);
									}
									else
									{
										fusionComponents[j] = nextComponent;
									}
								}

								for (int j = 0; j < multiplier; j++)
								{
									Fragment *component = fusionComponents[j];
									componentFragments.push_back(component);
									if (fusion != nullptr)
									{
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
										if (fusionDescriptors[j].split(L":")->length == 1)
										{ //A fusion bracket without a colon is used when applying to the parent component (except in a special case where locants are ommitted)
											//check for case of omitted locant from a higher order fusion bracket e.g. cyclopenta[4,5]pyrrolo[2,3-c]pyridine
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
											if (fusionDescriptors[j].split(L"-")->length == 1 && fusionDescriptors[j].split(L",")->length > 1 && FragmentTools::allAtomsInRingAreIdentical(component) && ((StringTools::countTerminalPrimes(fusionDescriptors[j].split(L",")[0])) != fusionLevel))
											{ //Could be like cyclopenta[3,4]cyclobuta[1,2]benzene where the first fusion to occur has parent locants omitted not child locants
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
												int numberOfPrimes = StringTools::countTerminalPrimes(fusionDescriptors[j].split(L",")[0]);
												//note that this is the number of primes on the parent ring. So would expect the child ring and hence the fusionLevel to be 1 higher
												if (numberOfPrimes + 1 != fusionLevel)
												{
													if (numberOfPrimes + 2 == fusionLevel)
													{ //ring could be in previous fusion level e.g. the benzo in benzo[10,11]phenanthro[2',3',4',5',6':4,5,6,7]chryseno[1,2,3-bc]coronene
														fusionLevel--;
													}
													else
													{
														throw StructureBuildingException(std::wstring(L"Incorrect number of primes in fusion bracket: ") + fusionDescriptors[j]);
													}
												}
												relabelAccordingToFusionLevel(component, fusionLevel);
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
												std::vector<std::wstring> numericalLocantsOfParent = Arrays::asList(fusionDescriptors[j].split(L","));
												std::vector<std::wstring> numericalLocantsOfChild = findPossibleNumericalLocants(component, determineAtomsToFuse(fragmentInScopeForEachFusionLevel[fusionLevel], numericalLocantsOfParent, boost::none).size() - 1);
												processHigherOrderFusionDescriptors(component, fragmentInScopeForEachFusionLevel[fusionLevel], numericalLocantsOfChild, numericalLocantsOfParent);
											}
											else
											{
												fusionLevel = 0;
												relabelAccordingToFusionLevel(component, fusionLevel);
												std::wstring fusionDescriptor = fusionDescriptors[j];
												std::vector<std::wstring> fusionArray = determineNumericalAndLetterComponents(fusionDescriptor);
												int numberOfPrimes = 0;
												if (fusionArray[1] != L"")
												{
													numberOfPrimes = StringTools::countTerminalPrimes(fusionArray[1]);
													if (fusionArray[0] == L"")
													{
														fusionDescriptor = fusionArray[1].replaceAll(L"'", L"");
													}
													else
													{
														fusionDescriptor = fusionArray[0] + std::wstring(L"-") + fusionArray[1].replaceAll(L"'", L"");
													}
													if (numberOfPrimes >= parentFragments.size())
													{
														throw StructureBuildingException(L"Unexpected prime in fusion descriptor");
													}
												}
												performSimpleFusion(fusionDescriptor, component, parentFragments[numberOfPrimes]); //e.g. pyrano[3,2-b]imidazo[4,5-e]pyridine where both are level 0 fusions
											}
										}
										else
										{
											//determine number of primes in fusor and hence determine fusion level
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
											int numberOfPrimes = -j + StringTools::countTerminalPrimes(fusionDescriptors[j].split(L",")[0]);
											if (numberOfPrimes != fusionLevel)
											{
												if (fusionLevel == numberOfPrimes + 1)
												{
													fusionLevel--;
												}
												else
												{
													throw StructureBuildingException(std::wstring(L"Incorrect number of primes in fusion bracket: ") + fusionDescriptors[j]);
												}
											}
											relabelAccordingToFusionLevel(component, fusionLevel);
											performHigherOrderFusion(fusionDescriptors[j], component, fragmentInScopeForEachFusionLevel[fusionLevel]);
										}
									}
									else
									{
										relabelAccordingToFusionLevel(component, fusionLevel);
										performSimpleFusion(L"", component, fragmentInScopeForEachFusionLevel[fusionLevel]);
									}
								}
								fusionLevel++;
								if (multiplier == 1)
								{ //multiplied components may not be substituted onto
									fragmentInScopeForEachFusionLevel[fusionLevel] = fusionComponents[0];
								}
							}
							for (auto ring : componentFragments)
							{
								state->fragManager->incorporateFragment(ring, parentRing);
							}
							removeMergedAtoms();

							FusedRingNumberer::numberFusedRing(parentRing); //numbers the fused ring;

							StringBuilder *fusedRingName = new StringBuilder();
							for (auto element : nameComponents)
							{
								fusedRingName->append(element->getValue());
							}
							fusedRingName->append(lastGroup->getValue());

							Element *fusedRingEl = lastGroup; //reuse this element to save having to remap suffixes...
							fusedRingEl->getAttribute(XmlDeclarations::VALUE_ATR)->setValue(fusedRingName->toString());
							fusedRingEl->getAttribute(XmlDeclarations::TYPE_ATR)->setValue(XmlDeclarations::RING_TYPE_VAL);
							fusedRingEl->setValue(fusedRingName->toString());

							for (auto element : nameComponents)
							{
								element->detach();
							}
						}

						void FusedRingBuilder::removeMergedAtoms()
						{
							for (auto a : atomsToRemoveToReplacementAtom)
							{
								state->fragManager->removeAtomAndAssociatedBonds(a->first);
							}
							atomsToRemoveToReplacementAtom.clear();
						}

						std::vector<Element*> FusedRingBuilder::formNameComponentList()
						{
							std::vector<Element*> nameComponents;
							Element *currentEl = groupsInFusedRing[0];
							while (currentEl != lastGroup)
							{
								if (currentEl->getName() == XmlDeclarations::GROUP_EL || currentEl->getName() == XmlDeclarations::FUSION_EL)
								{
									nameComponents.push_back(currentEl);
								}
								currentEl = OpsinTools::getNextSibling(currentEl);
							}
							return nameComponents;
						}

						void FusedRingBuilder::processRingNumberingAndIrregularities() throw(StructureBuildingException)
						{
							for (auto group : groupsInFusedRing)
							{
								Fragment *ring = group->getFrag();
								if (XmlDeclarations::ALKANESTEM_SUBTYPE_VAL == group->getAttributeValue(XmlDeclarations::SUBTYPE_ATR))
								{
									aromatiseCyclicAlkane(group);
								}
								processPartiallyUnsaturatedHWSystems(group, ring);
								if (group == lastGroup)
								{
									//perform a quick check that every atom in this group is infact cyclic. Fusion components are enumerated and hence all guaranteed to be purely cyclic
									std::vector<Atom*> atomList = ring->getAtomList();
									for (auto atom : atomList)
									{
										if (!atom->getAtomIsInACycle())
										{
											throw StructureBuildingException(std::wstring(L"Inappropriate group used in fusion nomenclature. Only groups composed entirely of atoms in cycles may be used. i.e. not: ") + group->getValue());
										}
									}
									if (group->getAttribute(XmlDeclarations::FUSEDRINGNUMBERING_ATR) != nullptr)
									{
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
										std::vector<std::wstring> standardNumbering = group->getAttributeValue(XmlDeclarations::FUSEDRINGNUMBERING_ATR).split(L"/", -1);
										for (int j = 0; j < standardNumbering.size(); j++)
										{
											atomList[j]->replaceLocants(standardNumbering[j]);
										}
									}
									else
									{
										ring->sortAtomListByLocant(); //for those where the order the locants are in is sensible                    }
									}
									for (auto atom : atomList)
									{
										atom->clearLocants(); //the parentRing does not have locants, letters are used to indicate the edges
									}
								}
								else if (group->getAttribute(XmlDeclarations::FUSEDRINGNUMBERING_ATR) == nullptr)
								{
									ring->sortAtomListByLocant(); //for those where the order the locants are in is sensible
								}
							}
						}

						void FusedRingBuilder::processPartiallyUnsaturatedHWSystems(Element *group, Fragment *ring)
						{
							if (XmlDeclarations::HANTZSCHWIDMAN_SUBTYPE_VAL == group->getAttributeValue(XmlDeclarations::SUBTYPE_ATR) && group->getAttribute(XmlDeclarations::ADDBOND_ATR) != nullptr)
							{
								std::vector<Element*> unsaturators = OpsinTools::getNextAdjacentSiblingsOfType(group, XmlDeclarations::UNSATURATOR_EL);
								if (unsaturators.size() > 0)
								{
									Element *unsaturator = unsaturators[0];
									if (unsaturator->getAttribute(XmlDeclarations::LOCANT_ATR) == nullptr && unsaturator->getAttributeValue(XmlDeclarations::VALUE_ATR) == L"2")
									{
										unsaturator->detach();
										std::vector<Bond*> bondsToUnsaturate = StructureBuildingMethods::findBondsToUnSaturate(ring, 2, true);
										if (bondsToUnsaturate.empty())
										{
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("Failed to find bond to unsaturate on partially saturated HW ring");
											throw std::exception();
										}
										Bond *b = bondsToUnsaturate[0];
										b->getFromAtom()->setSpareValency(true);
										b->getToAtom()->setSpareValency(true);
									}
								}
							}
						}

						void FusedRingBuilder::aromatiseCyclicAlkane(Element *cyclicAlkaneGroup)
						{
							Element *next = OpsinTools::getNextSibling(cyclicAlkaneGroup);
							std::vector<Element*> unsaturators;
							while (next != nullptr && next->getName() == XmlDeclarations::UNSATURATOR_EL)
							{
								unsaturators.push_back(next);
								next = OpsinTools::getNextSibling(next);
							}
							bool conjugate = true;
							if (unsaturators.size() == 1)
							{
								int value = std::stoi(unsaturators[0]->getAttributeValue(XmlDeclarations::VALUE_ATR));
								if (value != 2)
								{
									conjugate = false;
								}
								else if (unsaturators[0]->getAttribute(XmlDeclarations::LOCANT_ATR) != nullptr)
								{
									conjugate = false;
								}
							}
							else if (unsaturators.size() == 2)
							{
								int value1 = std::stoi(unsaturators[0]->getAttributeValue(XmlDeclarations::VALUE_ATR));
								if (value1 != 1)
								{
									conjugate = false;
								}
								else
								{
									int value2 = std::stoi(unsaturators[1]->getAttributeValue(XmlDeclarations::VALUE_ATR));
									if (value2 != 2 || unsaturators[1]->getAttribute(XmlDeclarations::LOCANT_ATR) != nullptr)
									{
										conjugate = false;
									}
								}
							}
							else if (unsaturators.size() > 2)
							{
								conjugate = false;
							}
							if (conjugate)
							{
								for (auto unsaturator : unsaturators)
								{
									unsaturator->detach();
								}
								std::vector<Atom*> atomList = cyclicAlkaneGroup->getFrag()->getAtomList();
								for (auto atom : atomList)
								{
									atom->setSpareValency(true);
								}
							}
						}

						int FusedRingBuilder::processMultiParentSystem(std::vector<Fragment*> &parentFragments, std::vector<Element*> &nameComponents, std::vector<Fragment*> &componentFragments) throw(StructureBuildingException)
						{
							int i = nameComponents.size() - 1;
							int fusionLevel = 0;
							if (i >= 0 && parentFragments.size() > 1)
							{
								std::vector<Fragment*> &previousFusionLevelFragments = parentFragments;
								for (; i >= 0; i--)
								{
									if (previousFusionLevelFragments.size() == 1)
									{ //completed multi parent system
										fragmentInScopeForEachFusionLevel[fusionLevel] = previousFusionLevelFragments[0];
										break;
									}
									Element *fusion = nullptr;
									if (nameComponents[i]->getName() == XmlDeclarations::FUSION_EL)
									{
										fusion = nameComponents[i--];
									}
									else
									{
										throw StructureBuildingException(L"Fusion bracket not found where fusion bracket expected");
									}
									if (i < 0 || nameComponents[i]->getName() != XmlDeclarations::GROUP_EL)
									{
										throw StructureBuildingException(L"Group not found where group expected. This is probably a bug");
									}
									Fragment *nextComponent = nameComponents[i]->getFrag();
									relabelAccordingToFusionLevel(nextComponent, fusionLevel);
									int multiplier = 1;
									Element *possibleMultiplierEl = OpsinTools::getPreviousSibling(nameComponents[i]);
									if (possibleMultiplierEl != nullptr && possibleMultiplierEl->getName() == XmlDeclarations::MULTIPLIER_EL)
									{
										multiplier = std::stoi(possibleMultiplierEl->getAttributeValue(XmlDeclarations::VALUE_ATR));
										possibleMultiplierEl->detach();
									}
									std::vector<Fragment*> fusionComponents;
									for (int j = 0; j < multiplier; j++)
									{
										if (j > 0)
										{
											Fragment *clonedFrag = state->fragManager->copyFragment(nextComponent);
											relabelAccordingToFusionLevel(clonedFrag, j); //fusionLevels worth of primes already added
											fusionComponents.push_back(clonedFrag);
										}
										else
										{
											fusionComponents.push_back(nextComponent);
										}
									}
									fusionLevel += multiplier;
									if (multiplier > 1 && multiplier != previousFusionLevelFragments.size())
									{
										throw StructureBuildingException(L"Mismatch between number of components and number of parents in fused ring system");
									}
									std::wstring fusionDescriptorString = fusion->getValue().toLowerCase(Locale::ROOT)->substr(1, (fusion->getValue().length() - 1) - 1);
									std::vector<std::wstring> fusionDescriptors = nullptr;
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
									if (fusionDescriptorString.split(L";")->length > 1)
									{
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
										fusionDescriptors = fusionDescriptorString.split(L";");
									}
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
									else if (fusionDescriptorString.split(L":")->length > 1)
									{
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
										fusionDescriptors = fusionDescriptorString.split(L":");
									}
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
									else if (fusionDescriptorString.split(L",")->length > 1)
									{
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
										fusionDescriptors = fusionDescriptorString.split(L",");
									}
									else
									{
										throw StructureBuildingException(std::wstring(L"Invalid fusion descriptor: ") + fusionDescriptorString);
									}
									if (fusionDescriptors.size() != previousFusionLevelFragments.size())
									{
										throw StructureBuildingException(std::wstring(L"Invalid fusion descriptor: ") + fusionDescriptorString + std::wstring(L"(Number of locants disagrees with number of parents)"));
									}
									for (int j = 0; j < fusionDescriptors.size(); j++)
									{
										std::wstring fusionDescriptor = fusionDescriptors[j];
										Fragment *component = multiplier > 1 ? fusionComponents[j] : nextComponent;
										Fragment *parentToUse = previousFusionLevelFragments[j];
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
										bool simpleFusion = fusionDescriptor.split(L":")->length <= 1;
										if (simpleFusion)
										{
											std::vector<std::wstring> fusionArray = determineNumericalAndLetterComponents(fusionDescriptor);
											if (fusionArray[1].length() != 0)
											{
												int numberOfPrimes = StringTools::countTerminalPrimes(fusionArray[1]);
												if (fusionArray[0].length() == 0)
												{
													fusionDescriptor = fusionArray[1].replaceAll(L"'", L"");
												}
												else
												{
													fusionDescriptor = fusionArray[0] + std::wstring(L"-") + fusionArray[1].replaceAll(L"'", L"");
												}
												if (numberOfPrimes != j)
												{ //check the number of primes on the letter part agree with the parent to use e.g.[4,5-bcd:1,2-c']difuran
													throw StructureBuildingException(std::wstring(L"Incorrect number of primes in fusion descriptor: ") + fusionDescriptor);
												}
											}
											performSimpleFusion(fusionDescriptor, component, parentToUse);
										}
										else
										{
											performHigherOrderFusion(fusionDescriptor, component, parentToUse);
										}
									}
									previousFusionLevelFragments = fusionComponents;
									componentFragments.insert(componentFragments.end(), fusionComponents.begin(), fusionComponents.end());
								}
								if (previousFusionLevelFragments.size() != 1)
								{
									throw StructureBuildingException(L"Invalid fused ring system. Incomplete multiparent system");
								}
							}
							return i;
						}

						std::vector<std::wstring> FusedRingBuilder::determineNumericalAndLetterComponents(const std::wstring &fusionDescriptor)
						{
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
							std::vector<std::wstring> fusionArray = fusionDescriptor.split(L"-");
							if (fusionArray.size() == 2)
							{
								return fusionArray;
							}
							else
							{
								std::vector<std::wstring> components(2);
								if (fusionArray[0].find(L",") != std::wstring::npos)
								{ //the digit section
									components[0] = fusionArray[0];
									components[1] = L"";
								}
								else
								{
									components[0] = L"";
									components[1] = fusionArray[0];
								}
								return components;
							}
						}

						void FusedRingBuilder::processBenzoFusions() throw(StructureBuildingException)
						{
							for (int i = groupsInFusedRing.size() - 2; i >= 0; i--)
							{
								Element *group = groupsInFusedRing[i];
								if (group->getValue() == L"benz" || group->getValue() == L"benzo")
								{
									Element *possibleFusionbracket = OpsinTools::getNextSibling(group);
									if (possibleFusionbracket->getName() != XmlDeclarations::FUSION_EL)
									{
										Element *possibleMultiplier = OpsinTools::getPreviousSibling(group);
										if (possibleMultiplier == nullptr || possibleMultiplier->getName() != XmlDeclarations::MULTIPLIER_EL || possibleMultiplier->getAttributeValue(XmlDeclarations::TYPE_ATR) == XmlDeclarations::GROUP_TYPE_VAL)
										{
											//e.g. 2-benzofuran. Fused rings of this type are a special case treated as being a single component
											//and have a special convention for indicating the position of heteroatoms 
											benzoSpecificFusion(group, groupsInFusedRing[i + 1]);
											group->detach();
											groupsInFusedRing.erase(groupsInFusedRing.begin() + i);
										}
									}
								}
							}
						}

						void FusedRingBuilder::relabelAccordingToFusionLevel(Fragment *component, int fusionLevel)
						{
							if (fusionLevel > 0)
							{
								FragmentTools::relabelNumericLocants(component->getAtomList(), StringTools::multiplyString(L"'", fusionLevel));
							}
						}

						void FusedRingBuilder::performSimpleFusion(const std::wstring &fusionDescriptor, Fragment *childRing, Fragment *parentRing) throw(StructureBuildingException)
						{
							std::vector<std::wstring> numericalLocantsOfChild;
							std::vector<std::wstring> letterLocantsOfParent;
							if (fusionDescriptor != L"")
							{
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
								std::vector<std::wstring> fusionArray = fusionDescriptor.split(L"-");
								if (fusionArray.size() == 2)
								{
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
									numericalLocantsOfChild = Arrays::asList(fusionArray[0].split(L","));
									std::vector<wchar_t> tempLetterLocantsOfParent = fusionArray[1].toCharArray();
									letterLocantsOfParent = std::vector<std::wstring>();
									for (auto letterLocantOfParent : tempLetterLocantsOfParent)
									{
										letterLocantsOfParent.push_back(StringHelper::toString(letterLocantOfParent));
									}
								}
								else
								{
									if (fusionArray[0].find(L",") != std::wstring::npos)
									{ //only has digits
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
										std::vector<std::wstring> numericalLocantsOfChildTemp = fusionArray[0].split(L",");
										numericalLocantsOfChild = Arrays::asList(numericalLocantsOfChildTemp);
									}
									else
									{ //only has letters
										std::vector<wchar_t> tempLetterLocantsOfParentCharArray = fusionArray[0].toCharArray();
										letterLocantsOfParent = std::vector<std::wstring>();
										for (auto letterLocantOfParentCharArray : tempLetterLocantsOfParentCharArray)
										{
											letterLocantsOfParent.push_back(StringHelper::toString(letterLocantOfParentCharArray));
										}
									}
								}
							}

							int edgeLength = 1;
							if (numericalLocantsOfChild.size() > 0)
							{
								if (numericalLocantsOfChild.size() <= 1)
								{
									throw StructureBuildingException(L"At least two numerical locants must be provided to perform fusion!");
								}
								edgeLength = numericalLocantsOfChild.size() - 1;
							}
							else if (letterLocantsOfParent.size() > 0)
							{
								edgeLength = letterLocantsOfParent.size();
							}

							if (numericalLocantsOfChild.empty())
							{
								numericalLocantsOfChild = findPossibleNumericalLocants(childRing, edgeLength);
							}

							if (letterLocantsOfParent.empty())
							{
								letterLocantsOfParent = findPossibleLetterLocants(parentRing, edgeLength);
							}
							if (numericalLocantsOfChild.empty() || letterLocantsOfParent.empty())
							{
								throw StructureBuildingException(L"Unable to find bond to form fused ring system. Some information for forming fused ring system was only supplyed implicitly");
							}

							processFirstOrderFusionDescriptors(childRing, parentRing, numericalLocantsOfChild, letterLocantsOfParent); //fuse the rings
						}

						std::vector<std::wstring> FusedRingBuilder::findPossibleLetterLocants(Fragment *ring, int edgeLength)
						{
							std::vector<int> carbonAtomIndexes;
							int numberOfAtoms = ring->getAtomCount();
							CyclicAtomList *cyclicAtomList = new CyclicAtomList(ring->getAtomList());
							for (int i = 0; i <= numberOfAtoms; i++)
							{
								//iterate backwards in list to use highest locanted edge in preference.
								//this retains what is currently locant 1 on the parent ring as locant 1 if the first two atoms found match
								//the last atom in the list is potentially tested twice e.g. on a 6 membered ring, 6-5 and 1-6 are both possible
								Atom *atom = cyclicAtomList->previous();
								//want non-bridgehead carbon atoms. Double-check that these carbon atoms are actually bonded (e.g. von baeyer systems have non-consecutive atom numbering!)
								if (atom->getElement() == ChemEl::C && atom->getBondCount() == 2 && (carbonAtomIndexes.empty() || std::find(atom->getAtomNeighbours().begin(), atom->getAtomNeighbours().end(), cyclicAtomList->peekNext()) != atom->getAtomNeighbours().end())))
								{
									carbonAtomIndexes.push_back(cyclicAtomList->getIndex());
									if (carbonAtomIndexes.size() == edgeLength + 1)
									{ //as many carbons in a row as to give that edgelength ->use these side/s
										std::reverse(carbonAtomIndexes.begin(), carbonAtomIndexes.end());
										std::vector<std::wstring> letterLocantsOfParent;
										for (int j = 0; j < edgeLength; j++)
										{
											letterLocantsOfParent.push_back(StringHelper::toString(static_cast<wchar_t>(97 + carbonAtomIndexes[j]))); //97 is ascii for a
										}
										return letterLocantsOfParent;
									}
								}
								else
								{
									carbonAtomIndexes.clear();
								}
							}
							return nullptr;
						}

						std::vector<std::wstring> FusedRingBuilder::findPossibleNumericalLocants(Fragment *ring, int edgeLength)
						{
							std::vector<std::wstring> carbonLocants;
							int numberOfAtoms = ring->getAtomCount();
							CyclicAtomList *cyclicAtomList = new CyclicAtomList(ring->getAtomList());
							for (int i = 0; i <= numberOfAtoms; i++)
							{
								//the last atom in the list is potentially tested twice e.g. on a 6 membered ring, 1-2 and 6-1 are both possible
								Atom *atom = cyclicAtomList->next();
								//want non-bridgehead carbon atoms. Double-check that these carbon atoms are actually bonded (e.g. von baeyer systems have non-consecutive atom numbering!)
								if (atom->getElement() == ChemEl::C && atom->getBondCount() == 2 && (carbonLocants.empty() || std::find(atom->getAtomNeighbours().begin(), atom->getAtomNeighbours().end(), cyclicAtomList->peekPrevious()) != atom->getAtomNeighbours().end())))
								{
									carbonLocants.push_back(atom->getFirstLocant());
									if (carbonLocants.size() == edgeLength + 1)
									{ //as many carbons in a row as to give that edgelength ->use these side/s
										std::vector<std::wstring> numericalLocantsOfChild;
										for (auto locant : carbonLocants)
										{
											numericalLocantsOfChild.push_back(locant);
										}
										return numericalLocantsOfChild;
									}
								}
								else
								{
									carbonLocants.clear();
								}
							}
							return nullptr;
						}

						void FusedRingBuilder::processFirstOrderFusionDescriptors(Fragment *childRing, Fragment *parentRing, std::vector<std::wstring> &numericalLocantsOfChild, std::vector<std::wstring> &letterLocantsOfParent) throw(StructureBuildingException)
						{
							std::vector<Atom*> childAtoms = determineAtomsToFuse(childRing, numericalLocantsOfChild, letterLocantsOfParent.size() + 1);
							if (childAtoms.empty())
							{
								throw StructureBuildingException(L"Malformed fusion bracket!");
							}

							std::vector<Atom*> parentAtoms;
							std::vector<Atom*> parentPeripheralAtomList = getPeripheralAtoms(parentRing->getAtomList());
							CyclicAtomList *cyclicListAtomsOnSurfaceOfParent = new CyclicAtomList(parentPeripheralAtomList, static_cast<int>(letterLocantsOfParent[0][0]) - 97); //convert from lower case character through ascii to 0-23
							parentAtoms.push_back(cyclicListAtomsOnSurfaceOfParent->getCurrent());
							for (int i = 0; i < letterLocantsOfParent.size(); i++)
							{
								parentAtoms.push_back(cyclicListAtomsOnSurfaceOfParent->next());
							}
							fuseRings(childAtoms, parentAtoms);
						}

						std::vector<Atom*> FusedRingBuilder::getPeripheralAtoms(std::vector<Atom*> &atomList)
						{
							//find the indice of the last atom on the surface of the ring. This obviously connects to the first atom. The objective is to exclude any interior atoms.
							std::vector<Atom*> neighbours = atomList[0]->getAtomNeighbours();
							int indice = std::numeric_limits<int>::max();
							for (auto atom : neighbours)
							{
								int indexOfAtom = atomList.find(atom);
								if (indexOfAtom == 1)
								{ //not the next atom
									continue;
								}
								else if (indexOfAtom == -1)
								{ //not in parentRing
									continue;
								}
								if (atomList.find(atom) < indice)
								{
									indice = indexOfAtom;
								}
							}
							return atomList.subList(0, indice +1);
						}

						void FusedRingBuilder::performHigherOrderFusion(const std::wstring &fusionDescriptor, Fragment *nextComponent, Fragment *fusedRing) throw(StructureBuildingException)
						{
							std::vector<std::wstring> numericalLocantsOfChild;
							std::vector<std::wstring> numericalLocantsOfParent;
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
							std::vector<std::wstring> fusionArray = fusionDescriptor.split(L":");
							if (fusionArray.size() == 2)
							{
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
								numericalLocantsOfChild = Arrays::asList(fusionArray[0].split(L","));
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
								numericalLocantsOfParent = Arrays::asList(fusionArray[1].split(L","));
							}
							else
							{
								throw StructureBuildingException(L"Malformed fusion bracket: This is an OPSIN bug, check regexTokens.xml");
							}
							processHigherOrderFusionDescriptors(nextComponent, fusedRing, numericalLocantsOfChild, numericalLocantsOfParent); //fuse the rings
						}

						void FusedRingBuilder::processHigherOrderFusionDescriptors(Fragment *childRing, Fragment *parentRing, std::vector<std::wstring> &numericalLocantsOfChild, std::vector<std::wstring> &numericalLocantsOfParent) throw(StructureBuildingException)
						{
							std::vector<Atom*> childAtoms = determineAtomsToFuse(childRing, numericalLocantsOfChild, boost::none);
							if (childAtoms.empty())
							{
								throw StructureBuildingException(L"Malformed fusion bracket!");
							}

							std::vector<Atom*> parentAtoms = determineAtomsToFuse(parentRing, numericalLocantsOfParent, childAtoms.size());
							if (parentAtoms.empty())
							{
								throw StructureBuildingException(L"Malformed fusion bracket!");
							}
							fuseRings(childAtoms, parentAtoms);
						}

						std::vector<Atom*> FusedRingBuilder::determineAtomsToFuse(Fragment *ring, std::vector<std::wstring> &numericalLocantsOnRing, boost::optional<int> expectedNumberOfAtomsToBeUsedForFusion) throw(StructureBuildingException)
						{
							std::vector<Atom*> parentPeripheralAtomList = getPeripheralAtoms(ring->getAtomList());
							int indexfirst = parentPeripheralAtomList.find(ring->getAtomByLocantOrThrow(numericalLocantsOnRing[0]));
							int indexfinal = parentPeripheralAtomList.find(ring->getAtomByLocantOrThrow(numericalLocantsOnRing[numericalLocantsOnRing.size() - 1]));
							CyclicAtomList *cyclicRingAtomList = new CyclicAtomList(parentPeripheralAtomList, indexfirst);
							std::vector<Atom*> fusionAtoms;

							std::vector<Atom*> potentialFusionAtomsAscending;
							potentialFusionAtomsAscending.push_back(cyclicRingAtomList->getCurrent());
							while (cyclicRingAtomList->getIndex() != indexfinal)
							{ //assume numbers are ascending
								potentialFusionAtomsAscending.push_back(cyclicRingAtomList->next());
							}
							if (!expectedNumberOfAtomsToBeUsedForFusion || expectedNumberOfAtomsToBeUsedForFusion == potentialFusionAtomsAscending.size())
							{
								bool notInPotentialParentAtoms = false;
								for (int i = 1; i < numericalLocantsOnRing.size() - 1 ; i++)
								{
									if (!std::find(potentialFusionAtomsAscending.begin(), potentialFusionAtomsAscending.end(), ring->getAtomByLocantOrThrow(numericalLocantsOnRing[i])) != potentialFusionAtomsAscending.end()))
									{
										notInPotentialParentAtoms = true;
									}
								}
								if (!notInPotentialParentAtoms)
								{
									fusionAtoms = potentialFusionAtomsAscending;
								}
							}

							if (fusionAtoms.empty() || !expectedNumberOfAtomsToBeUsedForFusion)
							{ //that didn't work, so try assuming the numbers are descending
								cyclicRingAtomList->setIndex(indexfirst);
								std::vector<Atom*> potentialFusionAtomsDescending;
								potentialFusionAtomsDescending.push_back(cyclicRingAtomList->getCurrent());
								while (cyclicRingAtomList->getIndex() != indexfinal)
								{ //assume numbers are descending
									potentialFusionAtomsDescending.push_back(cyclicRingAtomList->previous());
								}
								if (!expectedNumberOfAtomsToBeUsedForFusion || expectedNumberOfAtomsToBeUsedForFusion == potentialFusionAtomsDescending.size())
								{
									bool notInPotentialParentAtoms = false;
									for (int i = 1; i < numericalLocantsOnRing.size() - 1 ; i++)
									{
										if (!std::find(potentialFusionAtomsDescending.begin(), potentialFusionAtomsDescending.end(), ring->getAtomByLocantOrThrow(numericalLocantsOnRing[i])) != potentialFusionAtomsDescending.end()))
										{
											notInPotentialParentAtoms = true;
										}
									}
									if (!notInPotentialParentAtoms)
									{
										if (fusionAtoms.size() > 0 && !expectedNumberOfAtomsToBeUsedForFusion)
										{
											//prefer less fusion atoms
											if (potentialFusionAtomsDescending.size() < fusionAtoms.size())
											{
												fusionAtoms = potentialFusionAtomsDescending;
											}
										}
										else
										{
											fusionAtoms = potentialFusionAtomsDescending;
										}
									}
								}
							}
							return fusionAtoms;
						}

						void FusedRingBuilder::fuseRings(std::vector<Atom*> &childAtoms, std::vector<Atom*> &parentAtoms) throw(StructureBuildingException)
						{
							if (parentAtoms.size() != childAtoms.size())
							{
								throw StructureBuildingException(std::wstring(L"Problem with fusion descriptors: Parent atoms specified: ") + parentAtoms.size() + std::wstring(L" Child atoms specified: ") + childAtoms.size() + std::wstring(L" These should have been identical!"));
							}
							//replace parent atoms if the atom has already been used in fusion with the original atom
							//This will occur if fusion has resulted in something resembling a spiro centre e.g. cyclopenta[1,2-b:5,1-b']bis[1,4]oxathiine
							for (int i = parentAtoms.size() - 1; i >= 0; i--)
							{
								if (atomsToRemoveToReplacementAtom[parentAtoms[i]] != nullptr)
								{
									parentAtoms[i] = atomsToRemoveToReplacementAtom[parentAtoms[i]];
								}
								if (atomsToRemoveToReplacementAtom[childAtoms[i]] != nullptr)
								{
									childAtoms[i] = atomsToRemoveToReplacementAtom[childAtoms[i]];
								}
							}

							//sync spareValency and check that element type matches
							for (int i = 0; i < childAtoms.size(); i++)
							{
								Atom *parentAtom = parentAtoms[i];
								Atom *childAtom = childAtoms[i];
								if (childAtom->hasSpareValency())
								{
									parentAtom->setSpareValency(true);
								}
								if (parentAtom->getElement() != childAtom->getElement())
								{
									throw StructureBuildingException(L"Invalid fusion descriptor: Heteroatom placement is ambiguous as it is not present in both components of the fusion");
								}
								atomsToRemoveToReplacementAtom[childAtom] = parentAtom;
							}

							Set<Bond*> *fusionEdgeBonds = std::unordered_set<Bond*>(); //these bonds already exist in both the child and parent atoms
							for (int i = 0; i < childAtoms.size() - 1; i++)
							{
								fusionEdgeBonds->add(childAtoms[i]->getBondToAtomOrThrow(childAtoms[i + 1]));
								fusionEdgeBonds->add(parentAtoms[i]->getBondToAtomOrThrow(parentAtoms[i + 1]));
							}

							Set<Bond*> *bondsToAddToParentAtoms = new LinkedHashSet<Bond*>();
							for (auto childAtom : childAtoms)
							{
								for (auto b : childAtom->getBonds())
								{
									if (!fusionEdgeBonds->contains(b))
									{
										bondsToAddToParentAtoms->add(b);
									}
								}
							}

							Set<Bond*> *bondsToAddToChildAtoms = new LinkedHashSet<Bond*>();
							for (auto parentAtom : parentAtoms)
							{
								for (auto b : parentAtom->getBonds())
								{
									if (!fusionEdgeBonds->contains(b))
									{
										bondsToAddToChildAtoms->add(b);
									}
								}
							}

							for (auto bond : bondsToAddToParentAtoms)
							{
								Atom *from = bond->getFromAtom();
								int indiceInChildAtoms = childAtoms.find(from);
								if (indiceInChildAtoms != -1)
								{
									from = parentAtoms[indiceInChildAtoms];
								}
								Atom *to = bond->getToAtom();
								indiceInChildAtoms = childAtoms.find(to);
								if (indiceInChildAtoms != -1)
								{
									to = parentAtoms[indiceInChildAtoms];
								}
								state->fragManager->createBond(from, to, 1);
							}

							for (auto bond : bondsToAddToChildAtoms)
							{
								Atom *from = bond->getFromAtom();
								int indiceInParentAtoms = parentAtoms.find(from);
								if (indiceInParentAtoms != -1)
								{
									from = childAtoms[indiceInParentAtoms];
								}
								Atom *to = bond->getToAtom();
								indiceInParentAtoms = parentAtoms.find(to);
								if (indiceInParentAtoms != -1)
								{
									to = childAtoms[indiceInParentAtoms];
								}
								Bond *newBond = new Bond(from, to, 1);
								if (std::find(childAtoms.begin(), childAtoms.end(), from) != childAtoms.end())
								{
									from->addBond(newBond);
								}
								else
								{
									to->addBond(newBond);
								}
							}
						}

						void FusedRingBuilder::benzoSpecificFusion(Element *benzoEl, Element *parentEl) throw(StructureBuildingException)
						{
							/*
							 * Perform the fusion, number it and associate it with the parentEl
							 */
							Fragment *benzoRing = benzoEl->getFrag();
							Fragment *parentRing = parentEl->getFrag();
							performSimpleFusion(L"", benzoRing, parentRing);
							state->fragManager->incorporateFragment(benzoRing, parentRing);
							removeMergedAtoms();
							FusedRingNumberer::numberFusedRing(parentRing); //numbers the fused ring;
							Fragment *fusedRing = parentRing;
							setBenzoHeteroatomPositioning(benzoEl, fusedRing);
						}

						void FusedRingBuilder::setBenzoHeteroatomPositioning(Element *benzoEl, Fragment *fusedRing) throw(StructureBuildingException)
						{
							Element *locantEl = OpsinTools::getPreviousSibling(benzoEl);
							if (locantEl != nullptr && locantEl->getName() == XmlDeclarations::LOCANT_EL)
							{
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
								std::vector<std::wstring> locants = locantEl->getValue().split(L",");
								if (locantsCouldApplyToHeteroatomPositions(locants, benzoEl))
								{
									std::vector<Atom*> atomList = fusedRing->getAtomList();
									std::vector<Atom*> heteroatoms;
									std::vector<ChemEl> elementOfHeteroAtom;
									for (auto atom : atomList)
									{ //this iterates in the same order as the numbering system
										if (atom->getElement() != ChemEl::C)
										{
											heteroatoms.push_back(atom);
											elementOfHeteroAtom.push_back(atom->getElement());
										}
									}
									if (locants.size() == heteroatoms.size())
									{ //as many locants as there are heteroatoms to assign
										//check for special case of a single locant indicating where the group substitutes e.g. 4-benzofuran-2-yl
										if (!(locants.size() == 1 && OpsinTools::getPreviousSibling(locantEl) == nullptr && ComponentProcessor::checkLocantPresentOnPotentialRoot(state, benzoEl->getParent(), locants[0])))
										{
											for (auto atom : heteroatoms)
											{
												atom->setElement(ChemEl::C);
											}
											for (int i = 0; i < heteroatoms.size(); i++)
											{
												fusedRing->getAtomByLocantOrThrow(locants[i])->setElement(elementOfHeteroAtom[i]);
											}
											locantEl->detach();
										}
									}
									else if (locants.size() > 1)
									{
										throw StructureBuildingException(L"Unable to assign all locants to benzo-fused ring or multiplier was mising");
									}
								}
							}
						}

						bool FusedRingBuilder::locantsCouldApplyToHeteroatomPositions(std::vector<std::wstring> &locants, Element *benzoEl)
						{
							if (!locantsAreAllNumeric(locants))
							{
								return false;
							}
							std::vector<Element*> suffixes = benzoEl->getParent()->getChildElements(XmlDeclarations::SUFFIX_EL);
							int suffixesWithoutLocants = 0;
							for (auto suffix : suffixes)
							{
								if (suffix->getAttribute(XmlDeclarations::LOCANT_ATR) == nullptr)
								{
									suffixesWithoutLocants++;
								}
							}
							if (locants.size() == suffixesWithoutLocants)
							{ //In preference locants will be assigned to suffixes rather than to this nomenclature
								return false;
							}
							return true;
						}

						bool FusedRingBuilder::locantsAreAllNumeric(std::vector<std::wstring> &locants)
						{
							for (auto locant : locants)
							{
								if (!OpsinTools::MATCH_NUMERIC_LOCANT->matcher(locant).matches())
								{
									return false;
								}
							}
							return true;
						}
					}
				}
			}
		}
	}
}
