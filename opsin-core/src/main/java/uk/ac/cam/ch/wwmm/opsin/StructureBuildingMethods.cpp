#include "StructureBuildingMethods.h"
#include "BuildState.h"
#include "Element.h"
#include "StructureBuildingException.h"
#include "XmlDeclarations.h"
#include "OpsinTools.h"
#include "Fragment.h"
#include "Attribute.h"
#include "Atom.h"
#include "Bond.h"
#include "FragmentTools.h"
#include "AmbiguityChecker.h"
#include "StringTools.h"
#include "OutAtom.h"
#include "ValencyChecker.h"
#include "IsotopeSpecificationParser.h"
#include "WordRule.h"
#include "BuildResults.h"
#include "CycleDetector.h"

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
						using org::apache::log4j::Logger;
						using IsotopeSpecification = uk::ac::cam::ch::wwmm::opsin::IsotopeSpecificationParser::IsotopeSpecification;
						using namespace uk::ac::cam::ch::wwmm::opsin;
//						import static uk.ac.cam.ch.wwmm.opsin.XmlDeclarations.*;
//						import static uk.ac.cam.ch.wwmm.opsin.OpsinTools.*;

						StructureBuildingMethods::StructureBuildingMethods()
						{
						}

						void StructureBuildingMethods::resolveWordOrBracket(BuildState *state, Element *word) throw(StructureBuildingException)
						{
							if (word->getName() == XmlDeclarations::WORDRULE_EL)
							{ //already been resolved
								return;
							}
							if (word->getName() != XmlDeclarations::WORD_EL && word->getName() != XmlDeclarations::BRACKET_EL)
							{
								throw StructureBuildingException(L"A word or bracket is the expected input");
							}
							recursivelyResolveLocantedFeatures(state, word);
							recursivelyResolveUnLocantedFeatures(state, word);
							//TODO check all things that can substitute have outAtoms
							//TOOD think whether you can avoid the need to have a cansubstitute function by only using appropriate group
							std::vector<Element*> subsBracketsAndRoots = OpsinTools::getDescendantElementsWithTagNames(word, std::vector<std::wstring>{XmlDeclarations::BRACKET_EL, XmlDeclarations::SUBSTITUENT_EL, XmlDeclarations::ROOT_EL});
							for (auto subsBracketsAndRoot : subsBracketsAndRoots)
							{
								if (subsBracketsAndRoot->getAttribute(XmlDeclarations::MULTIPLIER_ATR) != nullptr)
								{
									throw StructureBuildingException(std::wstring(L"Structure building problem: multiplier on :") + subsBracketsAndRoot->getName() + std::wstring(L" was never used"));
								}
							}
							std::vector<Element*> groups = OpsinTools::getDescendantElementsWithTagName(word, XmlDeclarations::GROUP_EL);
							for (int i = 0; i < groups.size(); i++)
							{
								Element *group = groups[i];
								if (group->getAttribute(XmlDeclarations::RESOLVED_ATR) == nullptr && i != groups.size() - 1)
								{
									throw StructureBuildingException(std::wstring(L"Structure building problem: Bond was not made from :") + group->getValue() + std::wstring(L" but one should of been"));
								}
							}
						}

						void StructureBuildingMethods::recursivelyResolveLocantedFeatures(BuildState *state, Element *word) throw(StructureBuildingException)
						{
							if (word->getName() != XmlDeclarations::WORD_EL && word->getName() != XmlDeclarations::BRACKET_EL)
							{
								throw StructureBuildingException(L"A word or bracket is the expected input");
							}
							std::vector<Element*> subsBracketsAndRoots = OpsinTools::getChildElementsWithTagNames(word, std::vector<std::wstring>{XmlDeclarations::BRACKET_EL, XmlDeclarations::SUBSTITUENT_EL, XmlDeclarations::ROOT_EL});
							//substitution occurs left to right so by doing this right to left you ensure that any groups that will come into existence
							//due to multipliers being expanded will be in existence
							for (int i = subsBracketsAndRoots.size() - 1; i >= 0; i--)
							{
								Element *subBracketOrRoot = subsBracketsAndRoots[i];
								if (subBracketOrRoot->getName() == XmlDeclarations::BRACKET_EL)
								{
									recursivelyResolveLocantedFeatures(state,subBracketOrRoot);
									if (potentiallyCanSubstitute(subBracketOrRoot))
									{
										performAdditiveOperations(state, subBracketOrRoot);
										performLocantedSubstitutiveOperations(state, subBracketOrRoot);
									}
								}
								else
								{
									resolveRootOrSubstituentLocanted(state, subBracketOrRoot);
								}
							}
						}

						void StructureBuildingMethods::recursivelyResolveUnLocantedFeatures(BuildState *state, Element *word) throw(StructureBuildingException)
						{
							if (word->getName() != XmlDeclarations::WORD_EL && word->getName() != XmlDeclarations::BRACKET_EL)
							{
								throw StructureBuildingException(L"A word or bracket is the expected input");
							}
							std::vector<Element*> subsBracketsAndRoots = OpsinTools::getChildElementsWithTagNames(word, std::vector<std::wstring>{XmlDeclarations::BRACKET_EL, XmlDeclarations::SUBSTITUENT_EL, XmlDeclarations::ROOT_EL});
							//substitution occurs left to right so by doing this right to left you ensure that any groups that will come into existence
							//due to multipliers being expanded will be in existence
							for (int i = subsBracketsAndRoots.size() - 1; i >= 0; i--)
							{
								Element *subBracketOrRoot = subsBracketsAndRoots[i];
								if (subBracketOrRoot->getName() == XmlDeclarations::BRACKET_EL)
								{
									recursivelyResolveUnLocantedFeatures(state,subBracketOrRoot);
									if (potentiallyCanSubstitute(subBracketOrRoot))
									{
										performUnLocantedSubstitutiveOperations(state, subBracketOrRoot);
									}
								}
								else
								{
									resolveRootOrSubstituentUnLocanted(state, subBracketOrRoot);
								}
							}
						}

						void StructureBuildingMethods::resolveRootOrSubstituentLocanted(BuildState *state, Element *subOrRoot) throw(StructureBuildingException)
						{

							resolveLocantedFeatures(state, subOrRoot); //e.g. unsaturators, hydro groups and heteroatom replacement

							bool foundSomethingToSubstitute = potentiallyCanSubstitute(subOrRoot);

							if (foundSomethingToSubstitute)
							{
								performAdditiveOperations(state, subOrRoot); //e.g. ethylenediimino, oxyethylene (operations where two outAtoms are used to produce the bond and no locant is required as groups)
								performLocantedSubstitutiveOperations(state, subOrRoot); //e.g. 2-methyltoluene
							}
						}

						void StructureBuildingMethods::resolveRootOrSubstituentUnLocanted(BuildState *state, Element *subOrRoot) throw(StructureBuildingException)
						{

							bool foundSomethingToSubstitute = potentiallyCanSubstitute(subOrRoot);

							resolveUnLocantedFeatures(state, subOrRoot); //e.g. unsaturators, hydro groups and heteroatom replacement

							if (foundSomethingToSubstitute)
							{
								performUnLocantedSubstitutiveOperations(state, subOrRoot); //e.g. tetramethylfuran
							}
						}

						void StructureBuildingMethods::performLocantedSubstitutiveOperations(BuildState *state, Element *subBracketOrRoot) throw(StructureBuildingException)
						{
							Element *group;
							if (subBracketOrRoot->getName() == XmlDeclarations::BRACKET_EL)
							{
								group = findRightMostGroupInBracket(subBracketOrRoot);
							}
							else
							{
								group = subBracketOrRoot->getFirstChildElement(XmlDeclarations::GROUP_EL);
							}
							if (group->getAttribute(XmlDeclarations::RESOLVED_ATR) != nullptr)
							{
								return;
							}
							Fragment *frag = group->getFrag();
							if (frag->getOutAtomCount() >= 1 && subBracketOrRoot->getAttribute(XmlDeclarations::LOCANT_ATR) != nullptr)
							{
								std::wstring locantString = subBracketOrRoot->getAttributeValue(XmlDeclarations::LOCANT_ATR);
								if (frag->getOutAtomCount() > 1)
								{
									checkAndApplySpecialCaseWhereOutAtomsCanBeCombinedOrThrow(frag, group);
								}
								if (subBracketOrRoot->getAttribute(XmlDeclarations::MULTIPLIER_ATR) != nullptr)
								{ //e.g. 1,2-diethyl
									multiplyOutAndSubstitute(state, subBracketOrRoot);
								}
								else
								{
									Fragment *parentFrag = findFragmentWithLocant(subBracketOrRoot, locantString);
									if (parentFrag == nullptr)
									{
										std::wstring modifiedLocant = checkForBracketedPrimedLocantSpecialCase(subBracketOrRoot, locantString);
										if (modifiedLocant != L"")
										{
											parentFrag = findFragmentWithLocant(subBracketOrRoot, modifiedLocant);
											if (parentFrag != nullptr)
											{
												locantString = modifiedLocant;
											}
										}
									}
									if (parentFrag == nullptr)
									{
										throw StructureBuildingException(std::wstring(L"Cannot find in scope fragment with atom with locant ") + locantString + std::wstring(L"."));
									}
									Attribute tempVar(XmlDeclarations::RESOLVED_ATR, L"yes");
									group->addAttribute(&tempVar);
									Element *groupToAttachTo = parentFrag->getTokenEl();
									if (groupToAttachTo->getAttribute(XmlDeclarations::ACCEPTSADDITIVEBONDS_ATR) != nullptr && parentFrag->getOutAtomCount() > 0 && groupToAttachTo->getAttribute(XmlDeclarations::ISAMULTIRADICAL_ATR) != nullptr && parentFrag->getAtomByLocantOrThrow(locantString)->getOutValency() > 0 && frag->getOutAtom(0)->getValency() == 1 && parentFrag->getFirstAtom()->equals(parentFrag->getAtomByLocantOrThrow(locantString)))
									{
										//horrible special case to allow C-hydroxycarbonimidoyl and the like
										//If additive nomenclature the first atom should be an out atom
										joinFragmentsAdditively(state, frag, parentFrag);
									}
									else
									{
										Atom *atomToSubstituteAt = parentFrag->getAtomByLocantOrThrow(locantString);
										if (XmlDeclarations::PHOSPHO_SUBTYPE_VAL == group->getAttributeValue(XmlDeclarations::SUBTYPE_ATR) && frag->getOutAtom(0)->getValency() == 1)
										{
											if (atomToSubstituteAt->getElement() != ChemEl::O)
											{
												for (auto neighbour : atomToSubstituteAt->getAtomNeighbours())
												{
													if (neighbour->getElement() == ChemEl::O && neighbour->getBondCount() == 1 && neighbour->getFirstBond()->getOrder() == 1 && neighbour->getOutValency() == 0 && neighbour->getCharge() == 0)
													{
														atomToSubstituteAt = neighbour;
														break;
													}
												}
											}
										}
										joinFragmentsSubstitutively(state, frag, atomToSubstituteAt);
									}
								}
							}
						}

						void StructureBuildingMethods::performUnLocantedSubstitutiveOperations(BuildState *state, Element *subBracketOrRoot) throw(StructureBuildingException)
						{
							Element *group;
							if (subBracketOrRoot->getName() == XmlDeclarations::BRACKET_EL)
							{
								group = findRightMostGroupInBracket(subBracketOrRoot);
							}
							else
							{
								group = subBracketOrRoot->getFirstChildElement(XmlDeclarations::GROUP_EL);
							}
							if (group->getAttribute(XmlDeclarations::RESOLVED_ATR) != nullptr)
							{
								return;
							}
							Fragment *frag = group->getFrag();
							if (frag->getOutAtomCount() >= 1)
							{
								if (subBracketOrRoot->getAttribute(XmlDeclarations::LOCANT_ATR) != nullptr)
								{
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("Substituent has an unused outAtom and has a locant but locanted substitution should already have been performed!");
									throw std::exception();
								}
								if (frag->getOutAtomCount() > 1)
								{
									checkAndApplySpecialCaseWhereOutAtomsCanBeCombinedOrThrow(frag, group);
								}
								if (subBracketOrRoot->getAttribute(XmlDeclarations::MULTIPLIER_ATR) != nullptr)
								{ //e.g. diethyl
									multiplyOutAndSubstitute(state, subBracketOrRoot);
								}
								else
								{
									if (XmlDeclarations::PERHALOGENO_SUBTYPE_VAL == group->getAttributeValue(XmlDeclarations::SUBTYPE_ATR))
									{
										performPerHalogenoSubstitution(state, frag, subBracketOrRoot);
									}
									else
									{
										std::vector<Atom*> atomsToJoinTo;
										if (XmlDeclarations::PHOSPHO_SUBTYPE_VAL == group->getAttributeValue(XmlDeclarations::SUBTYPE_ATR) && frag->getOutAtom(0)->getValency() == 1)
										{
											std::vector<Fragment*> possibleParents = findAlternativeFragments(subBracketOrRoot);
											for (auto fragment : possibleParents)
											{
												std::vector<Atom*> hydroxyAtoms = FragmentTools::findHydroxyGroups(fragment);
												if (hydroxyAtoms.size() >= 1)
												{
													atomsToJoinTo = hydroxyAtoms;
												}
												break;
											}
										}
										if (atomsToJoinTo.empty())
										{
											atomsToJoinTo = findAtomsForSubstitution(subBracketOrRoot, 1, frag->getOutAtom(0)->getValency());
										}
										if (atomsToJoinTo.empty())
										{
											throw StructureBuildingException(std::wstring(L"Unlocanted substitution failed: unable to find suitable atom to bond atom with id:") + frag->getOutAtom(0)->getAtom()->getID() + std::wstring(L" to!"));
										}
										if (AmbiguityChecker::isSubstitutionAmbiguous(atomsToJoinTo, 1))
										{
											state->addIsAmbiguous(std::wstring(L"Connection of ") + group->getValue() + std::wstring(L" to ") + atomsToJoinTo[0]->getFrag().getTokenEl().getValue());
										}
										joinFragmentsSubstitutively(state, frag, atomsToJoinTo[0]);
									}
									Attribute tempVar(XmlDeclarations::RESOLVED_ATR, L"yes");
									group->addAttribute(&tempVar);
								}
							}
						}

						void StructureBuildingMethods::performPerHalogenoSubstitution(BuildState *state, Fragment *perhalogenFrag, Element *subBracketOrRoot) throw(StructureBuildingException)
						{
							std::vector<Fragment*> fragmentsToAttachTo = findAlternativeFragments(subBracketOrRoot);
							std::vector<Atom*> atomsToHalogenate;
							for (auto fragment : fragmentsToAttachTo)
							{
								FragmentTools::convertSpareValenciesToDoubleBonds(fragment);
								for (auto atom : fragment->getAtomList())
								{
									int substitutableHydrogen = calculateSubstitutableHydrogenAtoms(atom);
									if (substitutableHydrogen > 0 && FragmentTools::isCharacteristicAtom(atom))
									{
										continue;
									}
									for (int i = 0; i < substitutableHydrogen; i++)
									{
										atomsToHalogenate.push_back(atom);
									}
								}
							}
							if (atomsToHalogenate.empty())
							{
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("Failed to find any substitutable hydrogen to apply " + perhalogenFrag.getTokenEl().getValue() + " to!");
								throw std::exception();
							}
							std::vector<Fragment*> halogens;
							halogens.push_back(perhalogenFrag);
							for (int i = 0; i < atomsToHalogenate.size() - 1; i++)
							{
								halogens.push_back(state->fragManager->copyFragment(perhalogenFrag));
							}
							for (int i = 0; i < atomsToHalogenate.size(); i++)
							{
								Fragment *halogen = halogens[i];
								Atom *from = halogen->getOutAtom(0)->getAtom();
								halogen->removeOutAtom(0);
								state->fragManager->createBond(from, atomsToHalogenate[i], 1);
							}
							for (int i = 1; i < atomsToHalogenate.size(); i++)
							{
								state->fragManager->incorporateFragment(halogens[i], perhalogenFrag);
							}
						}

						void StructureBuildingMethods::multiplyOutAndSubstitute(BuildState *state, Element *subOrBracket) throw(StructureBuildingException)
						{
							Attribute *multiplierAtr = subOrBracket->getAttribute(XmlDeclarations::MULTIPLIER_ATR);
							int multiplier = std::stoi(multiplierAtr->getValue());
							subOrBracket->removeAttribute(multiplierAtr);
							std::vector<std::wstring> locants;
							std::wstring locantsAtrValue = subOrBracket->getAttributeValue(XmlDeclarations::LOCANT_ATR);
							if (locantsAtrValue != L"")
							{
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
								locants = locantsAtrValue.split(L",");
							}
							Element *parentWordOrBracket = subOrBracket->getParent();
							int indexOfSubOrBracket = parentWordOrBracket->find(subOrBracket);
							subOrBracket->detach();

							std::vector<Element*> elementsNotToBeMultiplied; //anything before the multiplier in the sub/bracket
							Element *multiplierEl = subOrBracket->getFirstChildElement(XmlDeclarations::MULTIPLIER_EL);
							if (multiplierEl == nullptr)
							{
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("Multiplier not found where multiplier expected");
								throw std::exception();
							}
							for (int i = subOrBracket->find(multiplierEl) - 1 ; i >= 0 ; i--)
							{
								Element *el = subOrBracket->getChild(i);
								el->detach();
								elementsNotToBeMultiplied.push_back(el);
							}
							multiplierEl->detach();

							std::vector<Element*> multipliedElements;
							for (int i = multiplier - 1; i >= 0; i--)
							{
								Element *currentElement;
								if (i != 0)
								{
									currentElement = state->fragManager->cloneElement(state, subOrBracket, i);
									addPrimesToLocantedStereochemistryElements(currentElement, StringTools::multiplyString(L"'", i)); //Stereochemistry elements with locants will need to have their locants primed (stereochemistry is only processed after structure building)
								}
								else
								{
									currentElement = subOrBracket;
								}
								multipliedElements.push_back(currentElement);
								if (locants.size() > 0)
								{
									parentWordOrBracket->insertChild(currentElement, indexOfSubOrBracket);
									currentElement->getAttribute(XmlDeclarations::LOCANT_ATR)->setValue(locants[i]);
									performLocantedSubstitutiveOperations(state, currentElement);
									currentElement->detach();
								}
							}
							if (locants.empty())
							{
								parentWordOrBracket->insertChild(multipliedElements[0], indexOfSubOrBracket);
								performUnlocantedSubstitutiveOperations(state, multipliedElements);
								multipliedElements[0]->detach();
							}
							for (auto multipliedElement : multipliedElements)
							{ //attach all the multiplied subs/brackets
								parentWordOrBracket->insertChild(multipliedElement, indexOfSubOrBracket);
							}
							for (auto el : elementsNotToBeMultiplied)
							{ //re-add anything before multiplier to original subOrBracket
								subOrBracket->insertChild(el, 0);
							}
						}

						void StructureBuildingMethods::performUnlocantedSubstitutiveOperations(BuildState *state, std::vector<Element*> &multipliedElements) throw(StructureBuildingException)
						{
							int numOfSubstituents = multipliedElements.size();
							Element *subBracketOrRoot = multipliedElements[0];
							Element *group;
							if (subBracketOrRoot->getName() == XmlDeclarations::BRACKET_EL)
							{
								group = findRightMostGroupInBracket(subBracketOrRoot);
							}
							else
							{
								group = subBracketOrRoot->getFirstChildElement(XmlDeclarations::GROUP_EL);
							}
							Fragment *frag = group->getFrag();
							if (frag->getOutAtomCount() >= 1)
							{
								if (subBracketOrRoot->getAttribute(XmlDeclarations::LOCANT_ATR) != nullptr)
								{
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("Substituent has an unused outAtom and has a locant but locanted substitution should already been been performed!");
									throw std::exception();
								}
								if (XmlDeclarations::PERHALOGENO_SUBTYPE_VAL == group->getAttributeValue(XmlDeclarations::SUBTYPE_ATR))
								{
									throw StructureBuildingException(group->getValue() + std::wstring(L" cannot be multiplied"));
								}
								if (frag->getOutAtomCount() > 1)
								{
									checkAndApplySpecialCaseWhereOutAtomsCanBeCombinedOrThrow(frag, group);
								}
								std::vector<Atom*> atomsToJoinTo;
								if (XmlDeclarations::PHOSPHO_SUBTYPE_VAL == group->getAttributeValue(XmlDeclarations::SUBTYPE_ATR) && frag->getOutAtom(0)->getValency() == 1)
								{
									std::vector<Fragment*> possibleParents = findAlternativeFragments(subBracketOrRoot);
									for (auto fragment : possibleParents)
									{
										std::vector<Atom*> hydroxyAtoms = FragmentTools::findHydroxyGroups(fragment);
										if (hydroxyAtoms.size() >= numOfSubstituents)
										{
											atomsToJoinTo = hydroxyAtoms;
										}
										break;
									}
								}
								if (atomsToJoinTo.empty())
								{
									atomsToJoinTo = findAtomsForSubstitution(subBracketOrRoot, numOfSubstituents, frag->getOutAtom(0)->getValency());
								}
								if (atomsToJoinTo.empty())
								{
									throw StructureBuildingException(std::wstring(L"Unlocanted substitution failed: unable to find suitable atom to bond atom with id:") + frag->getOutAtom(0)->getAtom()->getID() + std::wstring(L" to!"));
								}
								if (AmbiguityChecker::isSubstitutionAmbiguous(atomsToJoinTo, numOfSubstituents))
								{
									state->addIsAmbiguous(std::wstring(L"Connection of ") + group->getValue() + std::wstring(L" to ") + atomsToJoinTo[0]->getFrag().getTokenEl().getValue());
									std::vector<Atom*> atomsPreferredByEnvironment = AmbiguityChecker::useAtomEnvironmentsToGivePlausibleSubstitution(atomsToJoinTo, numOfSubstituents);
									if (atomsPreferredByEnvironment.size() > 0)
									{
										atomsToJoinTo = atomsPreferredByEnvironment;
									}
								}

								joinFragmentsSubstitutively(state, frag, atomsToJoinTo[0]);
								Attribute tempVar(XmlDeclarations::RESOLVED_ATR, L"yes");
								group->addAttribute(&tempVar);

								for (int i = 1; i < numOfSubstituents; i++)
								{
									subBracketOrRoot = multipliedElements[i];
									if (subBracketOrRoot->getName() == XmlDeclarations::BRACKET_EL)
									{
										group = findRightMostGroupInBracket(subBracketOrRoot);
									}
									else
									{
										group = subBracketOrRoot->getFirstChildElement(XmlDeclarations::GROUP_EL);
									}
									frag = group->getFrag();
									if (frag->getOutAtomCount() > 1)
									{ //TODO do this prior to multiplication?
										checkAndApplySpecialCaseWhereOutAtomsCanBeCombinedOrThrow(frag, group);
									}

									joinFragmentsSubstitutively(state, frag, atomsToJoinTo[i]);
									Attribute tempVar2(XmlDeclarations::RESOLVED_ATR, L"yes");
									group->addAttribute(&tempVar2);
								}
							}
						}

						void StructureBuildingMethods::resolveLocantedFeatures(BuildState *state, Element *subOrRoot) throw(StructureBuildingException)
						{
							std::vector<Element*> groups = subOrRoot->getChildElements(XmlDeclarations::GROUP_EL);
							if (groups.size() != 1)
							{
								throw StructureBuildingException(L"Each sub or root should only have one group element. This indicates a bug in OPSIN");
							}
							Element *group = groups[0];
							Fragment *thisFrag = group->getFrag();

							std::vector<Element*> unsaturators;
							std::vector<Element*> heteroatoms;
							std::vector<Element*> hydrogenElements;
							std::vector<Element*> subtractivePrefixElements;
							std::vector<Element*> isotopeSpecifications;

							std::vector<Element*> children = subOrRoot->getChildElements();
							for (auto currentEl : children)
							{
								std::wstring elName = currentEl->getName();
								if (elName == XmlDeclarations::UNSATURATOR_EL)
								{
									unsaturators.push_back(currentEl);
								}
								else if (elName == XmlDeclarations::HETEROATOM_EL)
								{
									heteroatoms.push_back(currentEl);
								}
								else if (elName == XmlDeclarations::SUBTRACTIVEPREFIX_EL)
								{
									subtractivePrefixElements.push_back(currentEl);
								}
								else if (elName == XmlDeclarations::HYDRO_EL)
								{
									hydrogenElements.push_back(currentEl);
								}
								else if (elName == XmlDeclarations::INDICATEDHYDROGEN_EL)
								{
									hydrogenElements.push_back(currentEl);
								}
								else if (elName == XmlDeclarations::ADDEDHYDROGEN_EL)
								{
									hydrogenElements.push_back(currentEl);
								}
								else if (elName == XmlDeclarations::ISOTOPESPECIFICATION_EL)
								{
									isotopeSpecifications.push_back(currentEl);
								}
							}
							/*
							 * Add locanted functionality
							 */

							std::vector<Atom*> atomsToDehydro;
							//locanted substitution can be assumed to be irrelevant to subtractive operations hence perform all subtractive operations now
							std::unordered_map<ChemEl, int> unlocantedSubtractivePrefixes;

							for (int i = subtractivePrefixElements.size() - 1; i >= 0; i--)
							{
								Element *subtractivePrefix = subtractivePrefixElements[i];
								std::wstring type = subtractivePrefix->getAttributeValue(XmlDeclarations::TYPE_ATR);
								if (type == XmlDeclarations::DEOXY_TYPE_VAL)
								{
									std::wstring locant = subtractivePrefix->getAttributeValue(XmlDeclarations::LOCANT_ATR);
									ChemEl chemEl = StringHelper::fromString<ChemEl>(subtractivePrefix->getAttributeValue(XmlDeclarations::VALUE_ATR));
									if (locant == L"")
									{
										boost::optional<int> count = unlocantedSubtractivePrefixes[chemEl];
										unlocantedSubtractivePrefixes[chemEl] = count ? count + 1 : 1;
									}
									else
									{
										applySubtractivePrefix(state, thisFrag, chemEl, locant);
									}
								}
								else if (type == XmlDeclarations::ANHYDRO_TYPE_VAL)
								{
									applyAnhydroPrefix(state, thisFrag, subtractivePrefix);
								}
								else if (type == XmlDeclarations::DEHYDRO_TYPE_VAL)
								{
									std::wstring locant = subtractivePrefix->getAttributeValue(XmlDeclarations::LOCANT_ATR);
									if (locant != L"")
									{
										atomsToDehydro.push_back(thisFrag->getAtomByLocantOrThrow(locant));
									}
									else
									{
										throw StructureBuildingException(L"locants are assumed to be required for the use of dehydro to be unambiguous");
									}
								}
								else
								{
									throw StructureBuildingException(std::wstring(L"OPSIN bug: Unexpected subtractive prefix type: ") + type);
								}
								subtractivePrefix->detach();
							}
							for (auto entry : unlocantedSubtractivePrefixes)
							{
								applyUnlocantedSubtractivePrefixes(state, thisFrag, entry.first, entry.second);
							}

							if (atomsToDehydro.size() > 0)
							{
								bool isCarbohydrateDehydro = false;
								if (group->getAttributeValue(XmlDeclarations::TYPE_ATR) == XmlDeclarations::CARBOHYDRATE_TYPE_VAL)
								{
									Set<Atom*> *uniquifiedDehydroAtoms = std::unordered_set<Atom*>(atomsToDehydro);
									if (uniquifiedDehydroAtoms->size() == atomsToDehydro.size())
									{ //need to rule out case where dehydro is being used to form triple bonds on carbohydrates
										isCarbohydrateDehydro = true;
									}
								}
								if (isCarbohydrateDehydro)
								{
									for (auto a : atomsToDehydro)
									{
										std::vector<Atom*> hydroxyAtoms = FragmentTools::findHydroxyLikeTerminalAtoms(a->getAtomNeighbours(), ChemEl::O);
										if (hydroxyAtoms.size() > 0)
										{
											hydroxyAtoms[0]->getFirstBond().setOrder(2);
										}
										else
										{
											throw StructureBuildingException(std::wstring(L"atom with locant ") + a->getFirstLocant() + std::wstring(L" did not have a hydroxy group to convert to a ketose"));
										}
									}
								}
								else
								{
									std::vector<Atom*> atomsToFormDoubleBonds;
									std::vector<Atom*> atomsToFormTripleBondsBetween; //dehydro on a double/aromatic bond forms a triple bond

									for (auto a : atomsToDehydro)
									{
										if (!a->hasSpareValency())
										{
											a->setSpareValency(true);
											atomsToFormDoubleBonds.push_back(a);
										}
										else
										{
											atomsToFormTripleBondsBetween.push_back(a);
										}
									}

									for (auto atom : atomsToFormDoubleBonds)
									{ //check that all the dehydro-ed atoms are next to another atom with spare valency
										bool hasSpareValency = false;
										for (auto neighbour : atom->getAtomNeighbours())
										{
											if (neighbour->hasSpareValency())
											{
												hasSpareValency = true;
												break;
											}
										}
										if (!hasSpareValency)
										{
											throw StructureBuildingException(L"Unexpected use of dehydro; two adjacent atoms were not unsaturated such as to form a double bond");
										}
									}
									addDehydroInducedTripleBonds(atomsToFormTripleBondsBetween);
								}
							}

							for (int i = hydrogenElements.size() - 1;i >= 0;i--)
							{
								Element *hydrogen = hydrogenElements[i];
								std::wstring locant = hydrogen->getAttributeValue(XmlDeclarations::LOCANT_ATR);
								if (locant != L"")
								{
									Atom *a = thisFrag->getAtomByLocantOrThrow(locant);
									if (a->hasSpareValency())
									{
										a->setSpareValency(false);
									}
									else
									{
										if (!acdNameSpiroIndicatedHydrogenBug(group, locant))
										{
											throw StructureBuildingException(std::wstring(L"hydrogen addition at locant: ") + locant + std::wstring(L" was requested, but this atom is not unsaturated"));
										}
									}
									hydrogenElements.erase(hydrogenElements.begin() + i);
									hydrogen->detach();
								}
							}

							for (int i = unsaturators.size() - 1;i >= 0;i--)
							{
								Element *unsaturator = unsaturators[i];
								std::wstring locant = unsaturator->getAttributeValue(XmlDeclarations::LOCANT_ATR);
								int bondOrder = std::stoi(unsaturator->getAttributeValue(XmlDeclarations::VALUE_ATR));
								if (bondOrder <= 1)
								{
									unsaturator->detach();
									continue;
								}
								if (locant != L"")
								{
//JAVA TO C++ CONVERTER TODO TASK: The Java Collection 'remove(Object)' method is not converted:
									unsaturators.remove(unsaturator);
									/*
									 * Is the locant a compound locant e.g. 1(6) 
									 * This would indicate unsaturation between the atoms with locants 1 and 6
									 */
									Matcher *matcher = matchCompoundLocant->matcher(locant);
									if (matcher->find())
									{
										std::wstring compoundLocant = matcher->group(1);
										locant = matcher->replaceAll(L"");
										FragmentTools::unsaturate(thisFrag->getAtomByLocantOrThrow(locant), compoundLocant, bondOrder, thisFrag);
									}
									else
									{
										FragmentTools::unsaturate(thisFrag->getAtomByLocantOrThrow(locant), bondOrder, thisFrag);
									}
									unsaturator->detach();
								}
							}

							for (int i = heteroatoms.size() - 1;i >= 0;i--)
							{
								Element *heteroatomEl = heteroatoms[i];
								std::wstring locant = heteroatomEl->getAttributeValue(XmlDeclarations::LOCANT_ATR);
								if (locant != L"")
								{
									Atom *heteroatom = state->fragManager->getHeteroatom(heteroatomEl->getAttributeValue(XmlDeclarations::VALUE_ATR));
									Atom *atomToBeReplaced = thisFrag->getAtomByLocantOrThrow(locant);
									if (heteroatom->getElement() == atomToBeReplaced->getElement() && heteroatom->getCharge() == atomToBeReplaced->getCharge())
									{
										throw StructureBuildingException(std::wstring(L"The replacement term ") + heteroatomEl->getValue() + std::wstring(L" was used on an atom that already is a ") + heteroatom->getElement());
									}
									state->fragManager->replaceAtomWithAtom(thisFrag->getAtomByLocantOrThrow(locant), heteroatom, true);
									if (heteroatomEl->getAttribute(XmlDeclarations::LAMBDA_ATR) != nullptr)
									{
										thisFrag->getAtomByLocantOrThrow(locant)->setLambdaConventionValency(std::stoi(heteroatomEl->getAttributeValue(XmlDeclarations::LAMBDA_ATR)));
									}
//JAVA TO C++ CONVERTER TODO TASK: The Java Collection 'remove(Object)' method is not converted:
									heteroatoms.remove(heteroatomEl);
									heteroatomEl->detach();
								}
							}

							if (isotopeSpecifications.size() > 0)
							{
								applyIsotopeSpecifications(state, thisFrag, isotopeSpecifications, true);
							}
						}

						bool StructureBuildingMethods::acdNameSpiroIndicatedHydrogenBug(Element *group, const std::wstring &indicatedHydrogenLocant)
						{
							if (boost::starts_with(group->getValue(), L"spiro"))
							{
								for (auto suffix : group->getParent()->getChildElements(XmlDeclarations::SUFFIX_EL))
								{
									std::wstring suffixLocant = suffix->getAttributeValue(XmlDeclarations::LOCANT_ATR);
									if (suffixLocant != L"" && suffixLocant == indicatedHydrogenLocant)
									{
										LOG->debug(std::wstring(L"Indicated hydrogen at ") + indicatedHydrogenLocant + std::wstring(L" ignored. Known bug in generated IUPAC name"));
										return true;
									}
								}
							}
							return false;
						}

						void StructureBuildingMethods::applySubtractivePrefix(BuildState *state, Fragment *fragment, ChemEl chemEl, const std::wstring &locant) throw(StructureBuildingException)
						{
							Atom *adjacentAtom = fragment->getAtomByLocantOrThrow(locant);
							std::vector<Atom*> applicableTerminalAtoms = FragmentTools::findHydroxyLikeTerminalAtoms(adjacentAtom->getAtomNeighbours(), chemEl);
							if (applicableTerminalAtoms.empty())
							{
								throw StructureBuildingException(std::wstring(L"Unable to find terminal atom of type: ") + chemEl + std::wstring(L" at locant ") + locant + std::wstring(L" for subtractive nomenclature"));
							}
							Atom *atomToRemove = applicableTerminalAtoms[0];
							if (FragmentTools::isFunctionalAtom(atomToRemove))
							{ //This can occur with aminoglycosides where the anomeric OH is removed by deoxy
								for (int i = 0, len = fragment->getFunctionalAtomCount(); i < len; i++)
								{
									if (atomToRemove->equals(fragment->getFunctionalAtom(i)->getAtom()))
									{
										fragment->removeFunctionalAtom(i);
										break;
									}
								}
								fragment->addFunctionalAtom(atomToRemove->getFirstBond()->getOtherAtom(atomToRemove));
							}
							FragmentTools::removeTerminalAtom(state, atomToRemove);
						}

						void StructureBuildingMethods::applyUnlocantedSubtractivePrefixes(BuildState *state, Fragment *fragment, ChemEl chemEl, int count) throw(StructureBuildingException)
						{
							std::vector<Atom*> applicableTerminalAtoms = FragmentTools::findHydroxyLikeTerminalAtoms(fragment->getAtomList(), chemEl);
							if (applicableTerminalAtoms.empty() || applicableTerminalAtoms.size() < count)
							{
								throw StructureBuildingException(std::wstring(L"Unable to find terminal atom of type: ") + chemEl + std::wstring(L" for subtractive nomenclature"));
							}
							if (AmbiguityChecker::isSubstitutionAmbiguous(applicableTerminalAtoms, count))
							{
								state->addIsAmbiguous(L"Group to remove with subtractive prefix");
							}
							for (int i = 0; i < count; i++)
							{
								Atom *atomToRemove = applicableTerminalAtoms[i];
								if (FragmentTools::isFunctionalAtom(atomToRemove))
								{ //This can occur with aminoglycosides where the anomeric OH is removed by deoxy
									for (int j = 0, len = fragment->getFunctionalAtomCount(); j < len; j++)
									{
										if (atomToRemove->equals(fragment->getFunctionalAtom(j)->getAtom()))
										{
											fragment->removeFunctionalAtom(j);
											break;
										}
									}
									fragment->addFunctionalAtom(atomToRemove->getFirstBond()->getOtherAtom(atomToRemove));
								}
								FragmentTools::removeTerminalAtom(state, atomToRemove);
							}
						}

						void StructureBuildingMethods::applyAnhydroPrefix(BuildState *state, Fragment *frag, Element *subtractivePrefix) throw(StructureBuildingException)
						{
							ChemEl chemEl = StringHelper::fromString<ChemEl>(subtractivePrefix->getAttributeValue(XmlDeclarations::VALUE_ATR));
							std::wstring locantStr = subtractivePrefix->getAttributeValue(XmlDeclarations::LOCANT_ATR);
							if (locantStr == L"")
							{
								throw StructureBuildingException(L"Two locants are required before an anhydro prefix");
							}
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
							std::vector<std::wstring> locants = locantStr.split(L",");
							Atom *backBoneAtom1 = frag->getAtomByLocantOrThrow(locants[0]);
							Atom *backBoneAtom2 = frag->getAtomByLocantOrThrow(locants[1]);
							std::vector<Atom*> applicableTerminalAtoms = FragmentTools::findHydroxyLikeTerminalAtoms(backBoneAtom1->getAtomNeighbours(), chemEl);
							if (applicableTerminalAtoms.empty())
							{
								throw StructureBuildingException(std::wstring(L"Unable to find terminal atom of type: ") + chemEl + std::wstring(L" for subtractive nomenclature"));
							}
							FragmentTools::removeTerminalAtom(state, applicableTerminalAtoms[0]);

							applicableTerminalAtoms = FragmentTools::findHydroxyLikeTerminalAtoms(backBoneAtom2->getAtomNeighbours(), chemEl);
							if (applicableTerminalAtoms.empty())
							{
								throw StructureBuildingException(std::wstring(L"Unable to find terminal atom of type: ") + chemEl + std::wstring(L" for subtractive nomenclature"));
							}
							state->fragManager->createBond(backBoneAtom1, applicableTerminalAtoms[0], 1);
						}

						void StructureBuildingMethods::addDehydroInducedTripleBonds(std::vector<Atom*> &atomsToFormTripleBondsBetween) throw(StructureBuildingException)
						{
							if (atomsToFormTripleBondsBetween.size() > 0)
							{
								Set<Atom*> *atoms = std::unordered_set<Atom*>(atomsToFormTripleBondsBetween);
								if (atomsToFormTripleBondsBetween.size() != atoms->size())
								{
									throw StructureBuildingException(L"locants specified for dehydro specify the same atom too many times");
								}
								for (int i = atomsToFormTripleBondsBetween.size() - 1; i >= 0; i = i - 2)
								{ //two atoms will have a triple bond formed betwen them
									Atom *a = atomsToFormTripleBondsBetween[i];
									std::vector<Atom*> neighbours = a->getAtomNeighbours();
									for (auto neighbour : neighbours)
									{
										if (std::find(atomsToFormTripleBondsBetween.begin(), atomsToFormTripleBondsBetween.end(), neighbour) != atomsToFormTripleBondsBetween.end())
										{
											atomsToFormTripleBondsBetween.erase(atomsToFormTripleBondsBetween.begin() + i);
//JAVA TO C++ CONVERTER TODO TASK: The Java Collection 'remove(Object)' method is not converted:
											atomsToFormTripleBondsBetween.remove(neighbour);
											Bond *b = a->getBondToAtomOrThrow(neighbour);
											b->setOrder(3);
											a->setSpareValency(false);
											neighbour->setSpareValency(false);
											goto atomLoopContinue;
										}
									}
									throw StructureBuildingException(L"dehydro indicated atom should form a triple bond but no adjacent atoms also had hydrogen removed!");
									atomLoopContinue:
								}
								atomLoopBreak:
							}
						}

						void StructureBuildingMethods::resolveUnLocantedFeatures(BuildState *state, Element *subOrRoot) throw(StructureBuildingException)
						{
							std::vector<Element*> groups = subOrRoot->getChildElements(XmlDeclarations::GROUP_EL);
							if (groups.size() != 1)
							{
								throw StructureBuildingException(L"Each sub or root should only have one group element. This indicates a bug in OPSIN");
							}
							Fragment *frag = groups[0]->getFrag();

							std::vector<int> unsaturationBondOrders;
							std::vector<Element*> heteroatoms;
							std::vector<Element*> hydrogenElements;
							std::vector<Element*> isotopeSpecifications;

							std::vector<Element*> children = subOrRoot->getChildElements();
							for (auto currentEl : children)
							{
								std::wstring elName = currentEl->getName();
								if (elName == XmlDeclarations::UNSATURATOR_EL)
								{
									int bondOrder = std::stoi(currentEl->getAttributeValue(XmlDeclarations::VALUE_ATR));
									if (bondOrder > 1)
									{
										unsaturationBondOrders.push_back(bondOrder);
									}
									currentEl->detach();
								}
								else if (elName == XmlDeclarations::HETEROATOM_EL)
								{
									heteroatoms.push_back(currentEl);
									currentEl->detach();
								}
								else if (elName == XmlDeclarations::HYDRO_EL || elName == XmlDeclarations::INDICATEDHYDROGEN_EL || elName == XmlDeclarations::ADDEDHYDROGEN_EL)
								{
									hydrogenElements.push_back(currentEl);
									currentEl->detach();
								}
								else if (elName == XmlDeclarations::ISOTOPESPECIFICATION_EL)
								{
									isotopeSpecifications.push_back(currentEl);
								}
							}

							if (hydrogenElements.size() > 0)
							{
								applyUnlocantedHydro(state, frag, hydrogenElements);
							}

							if (unsaturationBondOrders.size() > 0)
							{
								unsaturateBonds(state, frag, unsaturationBondOrders);
							}

							if (heteroatoms.size() > 0)
							{
								applyUnlocantedHeteroatoms(state, frag, heteroatoms);
							}

							if (isotopeSpecifications.size() > 0)
							{
								applyIsotopeSpecifications(state, frag, isotopeSpecifications, false);
							}

							if (frag->getOutAtomCount() > 0)
							{ //assign any outAtoms that have not been set to a specific atom to a specific atom
								for (int i = 0, l = frag->getOutAtomCount(); i < l; i++)
								{
									OutAtom *outAtom = frag->getOutAtom(i);
									if (!outAtom->isSetExplicitly())
									{
										outAtom->setAtom(findAtomForUnlocantedRadical(state, frag, outAtom));
										outAtom->setSetExplicitly(true);
									}
								}
							}
						}

						void StructureBuildingMethods::applyUnlocantedHydro(BuildState *state, Fragment *frag, std::vector<Element*> &hydrogenElements) throw(StructureBuildingException)
						{
							/*
							 * This function is not entirely straightforward as certain atoms definitely should have their spare valency reduced
							 * However names are not consistent as to whether they bother having the hydro tags do this!
							 * The atoms in atomsWithSV are in atom order those that can take a hydro element and then those that shouldn't really take a hydro element as its absence is unambiguous
							 */
							std::vector<Atom*> atomsAcceptingHydroPrefix;
							Set<Atom*> *atomsWhichImplicitlyHadTheirSVRemoved = std::unordered_set<Atom*>();
							std::vector<Atom*> atomList = frag->getAtomList();
							for (auto atom : atomList)
							{
								if (atom->getType().equals(XmlDeclarations::SUFFIX_TYPE_VAL))
								{
									continue;
								}
								atom->ensureSVIsConsistantWithValency(false); //doesn't take into account suffixes
								if (atom->hasSpareValency())
								{
									atomsAcceptingHydroPrefix.push_back(atom);
									//if we take into account suffixes is the SV removed
									atom->ensureSVIsConsistantWithValency(true);
									if (!atom->hasSpareValency())
									{
										atomsWhichImplicitlyHadTheirSVRemoved->add(atom);
									}
								}
							}

							int hydrogenElsCount = hydrogenElements.size();
							for (auto hydrogenElement : hydrogenElements)
							{
								if (hydrogenElement->getValue().equals(L"perhydro"))
								{
									if (hydrogenElsCount != 1)
									{
										throw StructureBuildingException(L"Unexpected indication of hydrogen when perhydro makes such indication redundnant");
									}
									for (auto atom : atomsAcceptingHydroPrefix)
									{
										atom->setSpareValency(false);
									}
									return;
								}
							}

							std::vector<Atom*> atomsWithDefiniteSV;
							std::vector<Atom*> otherAtomsThatCanHaveHydro;
							for (auto a : atomsAcceptingHydroPrefix)
							{
								if (atomsWhichImplicitlyHadTheirSVRemoved->contains(a))
								{
									otherAtomsThatCanHaveHydro.push_back(a);
								}
								else
								{
									bool canFormDoubleBond = false;
									for (auto aa : frag->getIntraFragmentAtomNeighbours(a))
									{
										if (aa->hasSpareValency())
										{
											canFormDoubleBond = true;
											break;
										}
									}
									if (canFormDoubleBond)
									{
										atomsWithDefiniteSV.push_back(a);
									}
									else
									{
										otherAtomsThatCanHaveHydro.push_back(a);
									}
								}
							}
							std::vector<Atom*> prioritisedAtomsAcceptingHydro(atomsWithDefiniteSV);
							prioritisedAtomsAcceptingHydro.insert(prioritisedAtomsAcceptingHydro.end(), otherAtomsThatCanHaveHydro.begin(), otherAtomsThatCanHaveHydro.end()); //these end up at the end of the list

							if (hydrogenElsCount > prioritisedAtomsAcceptingHydro.size())
							{
								throw StructureBuildingException(std::wstring(L"Cannot find atom to add hydrogen to (") + hydrogenElsCount + std::wstring(L" hydrogens requested but only ") + prioritisedAtomsAcceptingHydro.size() + std::wstring(L" positions that can be hydrogenated)"));
							}

							int svCountAfterRemoval = atomsWithDefiniteSV.size() - hydrogenElsCount;
							if (svCountAfterRemoval > 1)
							{ //ambiguity likely. If it's 1 then an atom will be implicitly hydrogenated
								//NOTE: as hydrogens as added in pairs the unambiguous if one hydrogen is added and allow atoms are identical condition is unlikely to be ever satisfied
								if (!(AmbiguityChecker::allAtomsEquivalent(atomsWithDefiniteSV) && (hydrogenElsCount == 1 || hydrogenElsCount == atomsWithDefiniteSV.size() - 1)))
								{
									state->addIsAmbiguous(std::wstring(L"Ambiguous choice of positions to add hydrogen to on ") + frag->getTokenEl()->getValue());
								}
							}

							for (int i = 0; i < hydrogenElsCount; i++)
							{
								prioritisedAtomsAcceptingHydro[i]->setSpareValency(false);
							}
						}

						void StructureBuildingMethods::unsaturateBonds(BuildState *state, Fragment *frag, std::vector<int> &unsaturationBondOrders) throw(StructureBuildingException)
						{
							int tripleBonds = 0;
							int doublebonds = 0;
							for (* : :optional<int> bondOrder : unsaturationBondOrders)
							{
								if (bondOrder == 3)
								{
									tripleBonds++;
								}
								else if (bondOrder == 2)
								{
									doublebonds++;
								}
								else
								{
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("Unexpected unsaturation bon order: " + bondOrder);
									throw std::exception();
								}
							}

							if (tripleBonds > 0)
							{
								unsaturateBonds(state, frag, 3, tripleBonds);
							}
							if (doublebonds > 0)
							{
								unsaturateBonds(state, frag, 2, doublebonds);
							}
						}

						void StructureBuildingMethods::unsaturateBonds(BuildState *state, Fragment *frag, int bondOrder, int numToUnsaturate) throw(StructureBuildingException)
						{
							std::vector<Bond*> bondsThatCouldBeUnsaturated = findBondsToUnSaturate(frag, bondOrder, false);
							std::vector<Bond*> alternativeBondsThatCouldBeUnsaturated = Collections::emptyList();
							if (bondsThatCouldBeUnsaturated.size() < numToUnsaturate)
							{
								bondsThatCouldBeUnsaturated = findBondsToUnSaturate(frag, bondOrder, true);
							}
							else
							{
								alternativeBondsThatCouldBeUnsaturated = findAlternativeBondsToUnSaturate(frag, bondOrder, bondsThatCouldBeUnsaturated);
							}
							if (bondsThatCouldBeUnsaturated.size() < numToUnsaturate)
							{
								throw StructureBuildingException(std::wstring(L"Failed to find bond to change to a bond of order: ") + std::to_wstring(bondOrder));
							}
							if (bondsThatCouldBeUnsaturated.size() > numToUnsaturate)
							{
								//by convention cycloalkanes can have one unsaturation implicitly at the 1 locant
								//terms like oxazoline are formally ambiguous but in practice the lowest locant is the one that will be intended (in this case 2-oxazoline)
								if (!isCycloAlkaneSpecialCase(frag, numToUnsaturate, bondsThatCouldBeUnsaturated) && XmlDeclarations::HANTZSCHWIDMAN_SUBTYPE_VAL != frag->getSubType())
								{
									if (alternativeBondsThatCouldBeUnsaturated.size() >= numToUnsaturate)
									{
										std::vector<Bond*> allBonds(bondsThatCouldBeUnsaturated);
										allBonds.insert(allBonds.end(), alternativeBondsThatCouldBeUnsaturated.begin(), alternativeBondsThatCouldBeUnsaturated.end());
										if (!(AmbiguityChecker::allBondsEquivalent(allBonds) && numToUnsaturate == 1))
										{
											state->addIsAmbiguous(std::wstring(L"Unsaturation of bonds of ") + frag->getTokenEl()->getValue());
										}
									}
									else
									{
										if (!(AmbiguityChecker::allBondsEquivalent(bondsThatCouldBeUnsaturated) && (numToUnsaturate == 1 || numToUnsaturate == bondsThatCouldBeUnsaturated.size() - 1)))
										{
											state->addIsAmbiguous(std::wstring(L"Unsaturation of bonds of ") + frag->getTokenEl()->getValue());
										}
									}
								}
							}
							for (int i = 0; i < numToUnsaturate; i++)
							{
								bondsThatCouldBeUnsaturated[i]->setOrder(bondOrder);
							}
						}

						bool StructureBuildingMethods::isCycloAlkaneSpecialCase(Fragment *frag, int numToUnsaturate, std::vector<Bond*> &bondsThatCouldBeUnsaturated)
						{
							if (numToUnsaturate == 1)
							{
								Bond *b = bondsThatCouldBeUnsaturated[0];
								Atom *a1 = b->getFromAtom();
								Atom *a2 = b->getToAtom();
								if ((XmlDeclarations::ALKANESTEM_SUBTYPE_VAL == frag->getSubType() || XmlDeclarations::HETEROSTEM_SUBTYPE_VAL == frag->getSubType()) && a1->getAtomIsInACycle() && a2->getAtomIsInACycle() && (a1->equals(frag->getFirstAtom()) || a2->equals(frag->getFirstAtom())))
								{
									//mono unsaturated cyclo alkanes are unambiguous e.g. cyclohexene
									return true;
								}
							}
							return false;
						}

						bool StructureBuildingMethods::isCycloAlkaneHeteroatomSpecialCase(Fragment *frag, int numHeteroatoms, std::vector<Atom*> &atomsThatCouldBeReplaced)
						{
							if (numHeteroatoms == 1)
							{
								if ((XmlDeclarations::ALKANESTEM_SUBTYPE_VAL == frag->getSubType() || XmlDeclarations::HETEROSTEM_SUBTYPE_VAL == frag->getSubType()) && frag->getFirstAtom()->getAtomIsInACycle() && atomsThatCouldBeReplaced[0]->equals(frag->getFirstAtom()))
								{
									//single heteroatom implicitly goes to 1 position
									return true;
								}
							}
							return false;
						}

						StructureBuildingMethods::HeteroAtomSmilesAndLambda::HeteroAtomSmilesAndLambda(const std::wstring &smiles, const std::wstring &lambdaConvention) : smiles(smiles), lambdaConvention(lambdaConvention)
						{
						}

						int StructureBuildingMethods::HeteroAtomSmilesAndLambda::hashCode()
						{
							constexpr int prime = 31;
							int result = 1;
							result = prime * result + ((lambdaConvention == L"") ? 0 : lambdaConvention.hashCode());
							result = prime * result + ((smiles == L"") ? 0 : smiles.hashCode());
							return result;
						}

						bool StructureBuildingMethods::HeteroAtomSmilesAndLambda::equals(void *obj)
						{
							if (this == obj)
							{
								return true;
							}
							if (obj == nullptr)
							{
								return false;
							}
							if (getClass() != obj->getClass())
							{
								return false;
							}
							HeteroAtomSmilesAndLambda *other = static_cast<HeteroAtomSmilesAndLambda*>(obj);
							if (lambdaConvention == L"")
							{
								if (other->lambdaConvention != L"")
								{
									return false;
								}
							}
							else if (lambdaConvention != other->lambdaConvention)
							{
								return false;
							}
							if (smiles == L"")
							{
								if (other->smiles != L"")
								{
									return false;
								}
							}
							else if (smiles != other->smiles)
							{
								return false;
							}
							return true;
						}

						void StructureBuildingMethods::applyUnlocantedHeteroatoms(BuildState *state, Fragment *frag, std::vector<Element*> &heteroatoms) throw(StructureBuildingException)
						{
							std::unordered_map<HeteroAtomSmilesAndLambda*, int> heteroatomDescriptionToCount;
							for (auto heteroatomEl : heteroatoms)
							{
								std::wstring smiles = heteroatomEl->getAttributeValue(XmlDeclarations::VALUE_ATR);
								std::wstring lambdaConvention = heteroatomEl->getAttributeValue(XmlDeclarations::LAMBDA_ATR);
								HeteroAtomSmilesAndLambda *desc = new HeteroAtomSmilesAndLambda(smiles, lambdaConvention);
								boost::optional<int> count = heteroatomDescriptionToCount[desc];
								heteroatomDescriptionToCount[desc] = count ? count + 1 : 1;
							}
							std::vector<Atom*> atomlist = frag->getAtomList();
							for (auto entry : heteroatomDescriptionToCount)
							{
								HeteroAtomSmilesAndLambda *desc = entry.first;
								int replacementsRequired = entry.second;
								Atom *heteroatom = state->fragManager->getHeteroatom(desc->smiles);
								ChemEl heteroatomChemEl = heteroatom->getElement();
								//finds an atom for which changing it to the specified heteroatom will not cause valency to be violated
								std::vector<Atom*> atomsThatCouldBeReplaced;
								for (auto atom : atomlist)
								{
									if (atom->getType().equals(XmlDeclarations::SUFFIX_TYPE_VAL))
									{
										continue;
									}
									if ((heteroatomChemEl::equals(atom->getElement()) && heteroatom->getCharge() == atom->getCharge()))
									{
										continue; //replacement would do nothing
									}
									if (atom->getElement() != ChemEl::C && heteroatomChemEl != ChemEl::C)
									{
										if (atom->getElement() == ChemEl::O && (heteroatomChemEl == ChemEl::S || heteroatomChemEl == ChemEl::Se || heteroatomChemEl == ChemEl::Te))
										{
											//by special case allow replacement of oxygen by chalcogen
										}
										else
										{
											//replacement of heteroatom by another heteroatom
											continue;
										}
									}
									if (ValencyChecker::checkValencyAvailableForReplacementByHeteroatom(atom, heteroatom))
									{
										atomsThatCouldBeReplaced.push_back(atom);
									}
								}
								if (atomsThatCouldBeReplaced.size() < replacementsRequired)
								{
									throw StructureBuildingException(L"Cannot find suitable atom for heteroatom replacement");
								}

								if (atomsThatCouldBeReplaced.size() > replacementsRequired && !isCycloAlkaneHeteroatomSpecialCase(frag, replacementsRequired, atomsThatCouldBeReplaced))
								{
									if (!(AmbiguityChecker::allAtomsEquivalent(atomsThatCouldBeReplaced) && (replacementsRequired == 1 || replacementsRequired == atomsThatCouldBeReplaced.size() - 1)))
									{
										//by convention cycloalkanes can have one unsaturation implicitly at the 1 locant
										state->addIsAmbiguous(std::wstring(L"Heteroatom replacement on ") + frag->getTokenEl()->getValue());
									}
								}

								for (int i = 0; i < replacementsRequired; i++)
								{
									Atom *atomToReplaceWithHeteroAtom = atomsThatCouldBeReplaced[i];
									state->fragManager->replaceAtomWithAtom(atomToReplaceWithHeteroAtom, heteroatom, true);
									if (desc->lambdaConvention != nullptr)
									{
										atomToReplaceWithHeteroAtom->setLambdaConventionValency(static_cast<Integer>(desc->lambdaConvention));
									}
								}
							}
						}

						void StructureBuildingMethods::applyIsotopeSpecifications(BuildState *state, Fragment *frag, std::vector<Element*> &isotopeSpecifications, bool applyLocanted) throw(StructureBuildingException)
						{
							for (int i = isotopeSpecifications.size() - 1; i >= 0; i--)
							{
								Element *isotopeSpecification = isotopeSpecifications[i];
								IsotopeSpecification *isotopeSpec = IsotopeSpecificationParser::parseIsotopeSpecification(isotopeSpecification);
								std::vector<std::wstring> locants = isotopeSpec->getLocants();
								if (locants.size() > 0)
								{
									if (!applyLocanted)
									{
										continue;
									}
								}
								else if (applyLocanted)
								{
									continue;
								}

								ChemEl chemEl = isotopeSpec->getChemEl();
								int isotope = isotopeSpec->getIsotope();
								if (locants.size() > 0)
								{
									if (chemEl == ChemEl::H)
									{
										for (int j = 0; j < locants.size(); j++)
										{
											Atom *atomWithHydrogenIsotope = frag->getAtomByLocantOrThrow(locants[j]);
											Atom *hydrogen = state->fragManager->createAtom(isotopeSpec->getChemEl(), frag);
											hydrogen->setIsotope(isotope);
											state->fragManager->createBond(atomWithHydrogenIsotope, hydrogen, 1);
										}
									}
									else
									{
										for (int j = 0; j < locants.size(); j++)
										{
											Atom *atom = frag->getAtomByLocantOrThrow(locants[j]);
											if (chemEl != atom->getElement())
											{
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
												throw StructureBuildingException(std::wstring(L"The atom at locant: ") + locants[j] + std::wstring(L" was not a ") + chemEl::toString());
											}
											atom->setIsotope(isotope);
										}
									}
								}
								else
								{
									int multiplier = isotopeSpec->getMultiplier();
									if (chemEl == ChemEl::H)
									{
										std::vector<Atom*> parentAtomsToApplyTo = FragmentTools::findnAtomsForSubstitution(frag, multiplier, 1);
										if (parentAtomsToApplyTo.empty())
										{
											throw StructureBuildingException(L"Failed to find sufficient hydrogen atoms for unlocanted hydrogen isotope replacement");
										}
										if (AmbiguityChecker::isSubstitutionAmbiguous(parentAtomsToApplyTo, multiplier))
										{
											if (!casIsotopeAmbiguitySpecialCase(frag, parentAtomsToApplyTo, multiplier))
											{
												state->addIsAmbiguous(std::wstring(L"Position of hydrogen isotope on ") + frag->getTokenEl()->getValue());
											}
										}
										for (int j = 0; j < multiplier; j++)
										{
											Atom *atomWithHydrogenIsotope = parentAtomsToApplyTo[j];
											Atom *hydrogen = state->fragManager->createAtom(isotopeSpec->getChemEl(), frag);
											hydrogen->setIsotope(isotope);
											state->fragManager->createBond(atomWithHydrogenIsotope, hydrogen, 1);
										}
									}
									else
									{
										std::vector<Atom*> parentAtomsToApplyTo;
										for (auto atom : frag->getAtomList())
										{
											if (atom->getElement() == chemEl)
											{
												parentAtomsToApplyTo.push_back(atom);
											}
										}
										if (parentAtomsToApplyTo.size() < multiplier)
										{
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
											throw StructureBuildingException(std::wstring(L"Failed to find sufficient atoms for ") + chemEl::toString() + std::wstring(L" isotope replacement"));
										}
										if (AmbiguityChecker::isSubstitutionAmbiguous(parentAtomsToApplyTo, multiplier))
										{
											state->addIsAmbiguous(std::wstring(L"Position of isotope on ") + frag->getTokenEl()->getValue());
										}
										for (int j = 0; j < multiplier; j++)
										{
											parentAtomsToApplyTo[j]->setIsotope(isotope);
										}
									}
								}
								isotopeSpecification->detach();
							}
						}

						bool StructureBuildingMethods::casIsotopeAmbiguitySpecialCase(Fragment *frag, std::vector<Atom*> &parentAtomsToApplyTo, int multiplier) throw(StructureBuildingException)
						{
							if (multiplier != 1)
							{
								return false;
							}
							std::vector<Atom*> atoms = frag->getAtomList();
							Atom *firstAtom = atoms[0];
							if (!parentAtomsToApplyTo[0]->equals(firstAtom))
							{
								return false;
							}
							ChemEl firstAtomEl = firstAtom->getElement();
							if (atoms.size() == 2)
							{
								if (firstAtomEl == atoms[1]->getElement())
								{
									//e.g. ethane
									return true;
								}
							}
							else
							{
								int intraFragValency = frag->getIntraFragmentIncomingValency(firstAtom);
								bool spareValency = firstAtom->hasSpareValency();
								if (firstAtom->getAtomIsInACycle())
								{
									for (int i = 1; i < atoms.size(); i++)
									{
										Atom *atom = atoms[i];
										if (atom->getElement() != firstAtomEl)
										{
											return false;
										}
										if (frag->getIntraFragmentIncomingValency(atom) != intraFragValency)
										{
											return false;
										}
										if (atom->hasSpareValency() != spareValency)
										{
											return false;
										}
									}
									//e.g. benzene
									return true;
								}
							}
							return false;
						}

						Atom *StructureBuildingMethods::findAtomForUnlocantedRadical(BuildState *state, Fragment *frag, OutAtom *outAtom) throw(StructureBuildingException)
						{
							std::vector<Atom*> possibleAtoms = FragmentTools::findnAtomsForSubstitution(frag, outAtom->getAtom(), 1, outAtom->getValency(), true);
							if (possibleAtoms.empty())
							{
								throw StructureBuildingException(L"Failed to assign all unlocanted radicals to actual atoms without violating valency");
							}
							if (!((XmlDeclarations::ALKANESTEM_SUBTYPE_VAL == frag->getSubType() || XmlDeclarations::HETEROSTEM_SUBTYPE_VAL == frag->getSubType()) && possibleAtoms[0]->equals(frag->getFirstAtom())))
							{
								if (AmbiguityChecker::isSubstitutionAmbiguous(possibleAtoms, 1))
								{
									state->addIsAmbiguous(std::wstring(L"Positioning of radical on: ") + frag->getTokenEl()->getValue());
								}
							}
							return possibleAtoms[0];
						}

						std::vector<Bond*> StructureBuildingMethods::findAlternativeBondsToUnSaturate(Fragment *frag, int bondOrder, Collection<Bond*> *bondsToIgnore)
						{
							return findBondsToUnSaturate(frag, bondOrder, false, std::unordered_set<Bond*>(bondsToIgnore));
						}

						std::vector<Bond*> StructureBuildingMethods::findBondsToUnSaturate(Fragment *frag, int bondOrder, bool allowAdjacentUnsaturatedBonds)
						{
							return findBondsToUnSaturate(frag, bondOrder, allowAdjacentUnsaturatedBonds, Collections::emptySet<Bond*>());
						}

						std::vector<Bond*> StructureBuildingMethods::findBondsToUnSaturate(Fragment *frag, int bondOrder, bool allowAdjacentUnsaturatedBonds, Set<Bond*> *bondsToIgnore)
						{
							std::vector<Bond*> bondsToUnsaturate;
							for (auto atom1 : frag->getAtomList())
							{
								if (atom1->hasSpareValency() || XmlDeclarations::SUFFIX_TYPE_VAL == atom1->getType() || atom1->getProperty(Atom::ISALDEHYDE) != nullptr)
								{
									continue;
								}
								std::vector<Bond*> bonds = atom1->getBonds();
								int incomingValency = 0;
								for (auto bond : bonds)
								{
									//don't place implicitly unsaturated bonds next to each other
									if (bond->getOrder() != 1 && !allowAdjacentUnsaturatedBonds)
									{
										goto mainLoopContinue;
									}
									if (std::find(bondsToUnsaturate.begin(), bondsToUnsaturate.end(), bond) != bondsToUnsaturate.end())
									{
										if (!allowAdjacentUnsaturatedBonds)
										{
											goto mainLoopContinue;
										}
										incomingValency += bondOrder;
									}
									else
									{
										incomingValency += bond->getOrder();
									}
								}

								boost::optional<int> maxVal = getLambdaValencyOrHwValencyOrMaxValIfCharged(atom1);
								if (maxVal && (incomingValency + (bondOrder - 1) + atom1->getOutValency()) > maxVal)
								{
									continue;
								}
								for (auto bond : bonds)
								{
									if (bond->getOrder() == 1 && !std::find(bondsToUnsaturate.begin(), bondsToUnsaturate.end(), bond) != bondsToUnsaturate.end() && !bondsToIgnore->contains(bond))
									{
										Atom *atom2 = bond->getOtherAtom(atom1);
										if (frag->getAtomByID(atom2->getID()) != nullptr)
										{ //check other atom is actually in the fragment!
											if (atom2->hasSpareValency() || XmlDeclarations::SUFFIX_TYPE_VAL == atom2->getType() || atom2->getProperty(Atom::ISALDEHYDE) != nullptr)
											{
												continue;
											}
											int incomingValency2 = 0;
											for (auto bond2 : atom2->getBonds())
											{
												//don't place implicitly unsaturated bonds next to each other
												if (bond2->getOrder() != 1 && !allowAdjacentUnsaturatedBonds)
												{
													goto bondLoopContinue;
												}
												if (std::find(bondsToUnsaturate.begin(), bondsToUnsaturate.end(), bond2) != bondsToUnsaturate.end())
												{
													if (!allowAdjacentUnsaturatedBonds)
													{
														goto bondLoopContinue;
													}
													incomingValency2 += bondOrder;
												}
												else
												{
													incomingValency2 += bond2->getOrder();
												}
											}

											boost::optional<int> maxVal2 = getLambdaValencyOrHwValencyOrMaxValIfCharged(atom2);
											if (maxVal2 && (incomingValency2 + (bondOrder - 1) + atom2->getOutValency()) > maxVal2)
											{
												continue;
											}
											bondsToUnsaturate.push_back(bond);
											goto bondLoopBreak;
										}
									}
									bondLoopContinue:
								}
								bondLoopBreak:
								mainLoopContinue:
							}
							mainLoopBreak:
							return bondsToUnsaturate;
						}

						boost::optional<int> StructureBuildingMethods::getLambdaValencyOrHwValencyOrMaxValIfCharged(Atom *a)
						{
							if (a->getLambdaConventionValency())
							{
								return a->getLambdaConventionValency() + a->getProtonsExplicitlyAddedOrRemoved();
							}
							else if (a->getCharge() == 0)
							{
								return ValencyChecker::getHWValency(a->getElement());
							}
							else
							{
								return ValencyChecker::getMaximumValency(a->getElement(), a->getCharge());
							}
						}

						void StructureBuildingMethods::performAdditiveOperations(BuildState *state, Element *subBracketOrRoot) throw(StructureBuildingException)
						{
							if (subBracketOrRoot->getAttribute(XmlDeclarations::LOCANT_ATR) != nullptr)
							{ //additive nomenclature does not employ locants
								return;
							}
							Element *group;
							if (subBracketOrRoot->getName() == XmlDeclarations::BRACKET_EL)
							{
								group = findRightMostGroupInBracket(subBracketOrRoot);
							}
							else
							{
								group = subBracketOrRoot->getFirstChildElement(XmlDeclarations::GROUP_EL);
							}
							if (group->getAttribute(XmlDeclarations::RESOLVED_ATR) != nullptr)
							{
								return;
							}
							Fragment *frag = group->getFrag();
							int outAtomCount = frag->getOutAtomCount();
							if (outAtomCount >= 1)
							{
								if (subBracketOrRoot->getAttribute(XmlDeclarations::MULTIPLIER_ATR) == nullptr)
								{
									Element *nextSiblingEl = OpsinTools::getNextSibling(subBracketOrRoot);
									if (nextSiblingEl->getAttribute(XmlDeclarations::MULTIPLIER_ATR) != nullptr && (outAtomCount >= std::stoi(nextSiblingEl->getAttributeValue(XmlDeclarations::MULTIPLIER_ATR)) || outAtomCount == 1 && frag->getOutAtom(0)->getValency() == std::stoi(nextSiblingEl->getAttributeValue(XmlDeclarations::MULTIPLIER_ATR))) && hasRootLikeOrMultiRadicalGroup(nextSiblingEl))
									{ //probably multiplicative nomenclature, should be as many outAtoms as the multiplier
										if (outAtomCount == 1)
										{ //special case e.g. 4,4'-(benzylidene)dianiline
											FragmentTools::splitOutAtomIntoValency1OutAtoms(frag->getOutAtom(0));
											//special case where something like benzylidene is being used as if it meant benzdiyl for multiplicative nomenclature
											//this is allowed in the IUPAC 79 recommendations but not recommended in the current recommendations
										}
										performMultiplicativeOperations(state, group, nextSiblingEl);
									}
									else if (group->getAttribute(XmlDeclarations::ISAMULTIRADICAL_ATR) != nullptr)
									{ //additive nomenclature e.g. ethyleneoxy
										Fragment *nextFrag = getNextInScopeMultiValentFragment(subBracketOrRoot);
										if (nextFrag != nullptr)
										{
											Element *nextMultiRadicalGroup = nextFrag->getTokenEl();
											Element *parentSubOrRoot = nextMultiRadicalGroup->getParent();
											if (state->currentWordRule != WordRule::polymer)
											{ //imino does not behave like a substituent in polymers only as a linker
												if (nextMultiRadicalGroup->getAttribute(XmlDeclarations::IMINOLIKE_ATR) != nullptr)
												{ //imino/methylene can just act as normal substituents, should an additive bond really be made???
													Fragment *adjacentFrag = OpsinTools::getNextGroup(subBracketOrRoot)->getFrag();

													if (nextFrag != adjacentFrag)
													{ //imino is not the absolute next frag
														if (potentiallyCanSubstitute(nextMultiRadicalGroup->getParent()) || potentiallyCanSubstitute(nextMultiRadicalGroup->getParent()->getParent()))
														{
															return;
														}
													}
												}
												if (group->getAttribute(XmlDeclarations::IMINOLIKE_ATR) != nullptr && levelsToWordEl(group) > levelsToWordEl(nextMultiRadicalGroup))
												{
													return; //e.g. imino substitutes ((chloroimino)ethylene)dibenzene
												}
											}
											if (parentSubOrRoot->getAttribute(XmlDeclarations::MULTIPLIER_ATR) != nullptr)
											{
												throw StructureBuildingException(L"Attempted to form additive bond to a multiplied component");
											}
											Attribute tempVar(XmlDeclarations::RESOLVED_ATR, L"yes");
											group->addAttribute(&tempVar);
											joinFragmentsAdditively(state, frag, nextFrag);
										}
									}
									else
									{ //e.g. chlorocarbonyl or hydroxy(sulfanyl)phosphoryl
										std::vector<Fragment*> siblingFragments = findAlternativeFragments(subBracketOrRoot);
										if (siblingFragments.size() > 0)
										{
											Fragment *nextFrag = siblingFragments[siblingFragments.size() - 1];
											Element *nextGroup = nextFrag->getTokenEl();
											if (nextGroup->getAttribute(XmlDeclarations::ACCEPTSADDITIVEBONDS_ATR) != nullptr && nextGroup->getAttribute(XmlDeclarations::ISAMULTIRADICAL_ATR) != nullptr && (nextFrag->getOutAtomCount() > 1 || nextGroup->getAttribute(XmlDeclarations::RESOLVED_ATR) != nullptr && nextFrag->getOutAtomCount() >= 1))
											{
												Atom *toAtom = nextFrag->getOutAtom(0)->getAtom();
												if (calculateSubstitutableHydrogenAtoms(toAtom) == 0)
												{
													Attribute tempVar2(XmlDeclarations::RESOLVED_ATR, L"yes");
													group->addAttribute(&tempVar2);
													joinFragmentsAdditively(state, frag, nextFrag); //e.g. aminocarbonyl or aminothio
												}
											}
											if (group->getAttribute(XmlDeclarations::RESOLVED_ATR) == nullptr && siblingFragments.size() > 1)
											{
												for (int i = 0; i < siblingFragments.size() - 1; i++)
												{
													Fragment *lastFrag = siblingFragments[i];
													Element *lastGroup = lastFrag->getTokenEl();
													if (lastGroup->getAttribute(XmlDeclarations::ACCEPTSADDITIVEBONDS_ATR) != nullptr && lastGroup->getAttribute(XmlDeclarations::ISAMULTIRADICAL_ATR) != nullptr && (lastFrag->getOutAtomCount() > 1 || lastGroup->getAttribute(XmlDeclarations::RESOLVED_ATR) != nullptr && lastFrag->getOutAtomCount() >= 1))
													{
														Atom *toAtom = lastFrag->getOutAtom(0)->getAtom();
														if (calculateSubstitutableHydrogenAtoms(toAtom) == 0)
														{
															Attribute tempVar3(XmlDeclarations::RESOLVED_ATR, L"yes");
															group->addAttribute(&tempVar3);
															joinFragmentsAdditively(state, frag, lastFrag); //e.g. hydroxy(sulfanyl)phosphoryl
														}
														break;
													}

													//loop may continue if lastFrag was in fact completely unsubstitutable e.g. hydroxy...phosphoryloxy. The oxy is unsubstituable as the phosphoryl will already have bonded to it
													if (FragmentTools::findSubstituableAtoms(lastFrag, frag->getOutAtom(outAtomCount - 1)->getValency()).size() > 0)
													{
														break;
													}
												}
											}
										}
									}
								}
								else
								{ // e.g. dimethoxyphosphoryl or bis(methylamino)phosphoryl
									std::vector<Fragment*> siblingFragments = findAlternativeFragments(subBracketOrRoot);
									if (siblingFragments.size() > 0)
									{
										int multiplier = std::stoi(subBracketOrRoot->getAttributeValue(XmlDeclarations::MULTIPLIER_ATR));
										Fragment *nextFrag = siblingFragments[siblingFragments.size() - 1];
										Element *nextGroup = nextFrag->getTokenEl();
										if (nextGroup->getAttribute(XmlDeclarations::ACCEPTSADDITIVEBONDS_ATR) != nullptr && nextGroup->getAttribute(XmlDeclarations::ISAMULTIRADICAL_ATR) != nullptr && (nextFrag->getOutAtomCount() >= multiplier || nextGroup->getAttribute(XmlDeclarations::RESOLVED_ATR) != nullptr && nextFrag->getOutAtomCount() >= multiplier + 1))
										{
											Atom *toAtom = nextFrag->getOutAtom(0)->getAtom();
											if (calculateSubstitutableHydrogenAtoms(toAtom) == 0)
											{
												Attribute tempVar4(XmlDeclarations::RESOLVED_ATR, L"yes");
												group->addAttribute(&tempVar4);
												multiplyOutAndAdditivelyBond(state, subBracketOrRoot, nextFrag); //e.g.dihydroxyphosphoryl
											}
										}
										if (group->getAttribute(XmlDeclarations::RESOLVED_ATR) == nullptr && siblingFragments.size() > 1)
										{
											for (int i = 0; i < siblingFragments.size() - 1; i++)
											{
												Fragment *lastFrag = siblingFragments[i];
												Element *lastGroup = lastFrag->getTokenEl();
												if (lastGroup->getAttribute(XmlDeclarations::ACCEPTSADDITIVEBONDS_ATR) != nullptr && lastGroup->getAttribute(XmlDeclarations::ISAMULTIRADICAL_ATR) != nullptr && (lastFrag->getOutAtomCount() >= multiplier || lastGroup->getAttribute(XmlDeclarations::RESOLVED_ATR) != nullptr && lastFrag->getOutAtomCount() >= multiplier + 1))
												{
													Atom *toAtom = lastFrag->getOutAtom(0)->getAtom();
													if (calculateSubstitutableHydrogenAtoms(toAtom) == 0)
													{
														Attribute tempVar5(XmlDeclarations::RESOLVED_ATR, L"yes");
														group->addAttribute(&tempVar5);
														multiplyOutAndAdditivelyBond(state, subBracketOrRoot, lastFrag); //e.g. dihydroxyphosphoryloxy
													}
													break;
												}

												//loop may continue if lastFrag was in fact completely unsubstitutable e.g. hydroxy...phosphoryloxy. The oxy is unsubstituable as the phosphoryl will already have bonded to it
												if (FragmentTools::findSubstituableAtoms(lastFrag, frag->getOutAtom(outAtomCount - 1)->getValency()).size() > 0)
												{
													break;
												}
											}
										}
									}
								}
							}
						}

						bool StructureBuildingMethods::hasRootLikeOrMultiRadicalGroup(Element *subBracketOrRoot)
						{
							std::vector<Element*> groups = OpsinTools::getDescendantElementsWithTagName(subBracketOrRoot, XmlDeclarations::GROUP_EL);
							if (subBracketOrRoot->getAttribute(XmlDeclarations::INLOCANTS_ATR) != nullptr)
							{
								return true; // a terminus with specified inLocants
							}
							for (auto group : groups)
							{
								Fragment *frag = group->getFrag();
								int outAtomCount = frag->getOutAtomCount();
								if (group->getAttribute(XmlDeclarations::ISAMULTIRADICAL_ATR) != nullptr)
								{
									if (outAtomCount >= 1)
									{
										return true; //a multi radical
									}
								}
								else if (outAtomCount == 0 && group->getAttribute(XmlDeclarations::RESOLVED_ATR) == nullptr)
								{
									return true; // a terminus
								}
							}
							return false;
						}

						void StructureBuildingMethods::multiplyOutAndAdditivelyBond(BuildState *state, Element *subOrBracket, Fragment *fragToAdditivelyBondTo) throw(StructureBuildingException)
						{
							int multiplier = std::stoi(subOrBracket->getAttributeValue(XmlDeclarations::MULTIPLIER_ATR));
							subOrBracket->removeAttribute(subOrBracket->getAttribute(XmlDeclarations::MULTIPLIER_ATR));
							std::vector<Element*> clonedElements;
							std::vector<Element*> elementsNotToBeMultiplied; //anything before the multiplier in the sub/bracket
							for (int i = multiplier - 1; i >= 0; i--)
							{
								Element *currentElement;
								if (i != 0)
								{
									currentElement = state->fragManager->cloneElement(state, subOrBracket, i);
									addPrimesToLocantedStereochemistryElements(currentElement, StringTools::multiplyString(L"'", i)); //Stereochemistry elements with locants will need to have their locants primed (stereochemistry is only processed after structure building)
									clonedElements.push_back(currentElement);
								}
								else
								{
									currentElement = subOrBracket;
									Element *multiplierEl = subOrBracket->getFirstChildElement(XmlDeclarations::MULTIPLIER_EL);
									if (multiplierEl == nullptr)
									{
										throw StructureBuildingException(L"Multiplier not found where multiplier expected");
									}
									for (int j = subOrBracket->find(multiplierEl) - 1 ; j >= 0 ; j--)
									{
										Element *el = subOrBracket->getChild(j);
										el->detach();
										elementsNotToBeMultiplied.push_back(el);
									}
									multiplierEl->detach();
								}
								Element *group;
								if (currentElement->getName() == XmlDeclarations::BRACKET_EL)
								{
									group = findRightMostGroupInBracket(currentElement);
								}
								else
								{
									group = currentElement->getFirstChildElement(XmlDeclarations::GROUP_EL);
								}
								Fragment *frag = group->getFrag();
								if (frag->getOutAtomCount() != 1)
								{
									throw StructureBuildingException(std::wstring(L"Additive bond formation failure: Fragment expected to have one OutAtom in this case but had: ") + std::to_wstring(frag->getOutAtomCount()));
								}
								joinFragmentsAdditively(state, frag, fragToAdditivelyBondTo);
							}
							for (auto clone : clonedElements)
							{ //make sure cloned substituents don't substitute onto each other!
								OpsinTools::insertAfter(subOrBracket, clone);
							}
							for (auto el : elementsNotToBeMultiplied)
							{ //re-add anything before multiplier to original subOrBracket
								subOrBracket->insertChild(el, 0);
							}
						}

						void StructureBuildingMethods::performMultiplicativeOperations(BuildState *state, Element *group, Element *multipliedParent) throw(StructureBuildingException)
						{
							BuildResults *multiRadicalBR = new BuildResults(group->getParent());
							performMultiplicativeOperations(state, multiRadicalBR, multipliedParent);
						}

						void StructureBuildingMethods::performMultiplicativeOperations(BuildState *state, BuildResults *multiRadicalBR, Element *multipliedParent) throw(StructureBuildingException)
						{
							int multiplier = std::stoi(multipliedParent->getAttributeValue(XmlDeclarations::MULTIPLIER_ATR));
							if (multiplier != multiRadicalBR->getOutAtomCount())
							{
								if (multiRadicalBR->getOutAtomCount() == multiplier*2)
								{
									//TODO substituents like nitrilo can have their outatoms combined
								}
								if (multiplier != multiRadicalBR->getOutAtomCount())
								{
									throw StructureBuildingException(std::wstring(L"Multiplication bond formation failure: number of outAtoms disagree with multiplier(multiplier: ") + multiplier + std::wstring(L", outAtom count: ") + multiRadicalBR->getOutAtomCount() + std::wstring(L")"));
								}
							}
							if (LOG->isTraceEnabled())
							{
								LOG->trace(std::to_wstring(multiplier) + L" multiplicative bonds to be formed");
							}
							multipliedParent->removeAttribute(multipliedParent->getAttribute(XmlDeclarations::MULTIPLIER_ATR));
							std::vector<std::wstring> inLocants;
							std::wstring inLocantsString = multipliedParent->getAttributeValue(XmlDeclarations::INLOCANTS_ATR);
							if (inLocantsString != L"")
							{ //true for the root of a multiplicative name
								if (inLocantsString == XmlDeclarations::INLOCANTS_DEFAULT)
								{
									inLocants = std::vector<std::wstring>(multiplier);
									for (int i = 0; i < multiplier; i++)
									{
										inLocants.push_back(XmlDeclarations::INLOCANTS_DEFAULT);
									}
								}
								else
								{
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
									inLocants = StringTools::arrayToList(inLocantsString.split(L","));
									if (inLocants.size() != multiplier)
									{
										throw StructureBuildingException(L"Mismatch between multiplier and number of inLocants in multiplicative nomenclature");
									}
								}
							}
							std::vector<Element*> clonedElements;
							BuildResults *newBr = new BuildResults();
							for (int i = multiplier - 1; i >= 0; i--)
							{
								Element *multipliedElement;
								if (i != 0)
								{
									multipliedElement = state->fragManager->cloneElement(state, multipliedParent, i);
									addPrimesToLocantedStereochemistryElements(multipliedElement, StringTools::multiplyString(L"'", i)); //Stereochemistry elements with locants will need to have their locants primed (stereochemistry is only processed after structure building)
									clonedElements.push_back(multipliedElement);
								}
								else
								{
									multipliedElement = multipliedParent;
								}

								//determine group that will be additively bonded to
								Element *multipliedGroup;
								if (multipliedElement->getName() == XmlDeclarations::BRACKET_EL)
								{
									multipliedGroup = getFirstMultiValentGroup(multipliedElement);
									if (multipliedGroup == nullptr)
									{ //root will not have a multivalent group
										std::vector<Element*> groups = OpsinTools::getDescendantElementsWithTagName(multipliedElement, XmlDeclarations::GROUP_EL);
										if (inLocants.empty())
										{
											throw StructureBuildingException(L"OPSIN Bug? in locants must be specified for a multiplied root in multiplicative nomenclature");
										}
										if (inLocants[0] == XmlDeclarations::INLOCANTS_DEFAULT)
										{
											multipliedGroup = groups[groups.size() - 1];
										}
										else
										{
											for (int j = groups.size() - 1; j >= 0; j--)
											{
												Fragment *possibleFrag = groups[j]->getFrag();
												for (auto locant : inLocants)
												{
													if (possibleFrag->hasLocant(locant))
													{
														multipliedGroup = groups[j];
														goto groupLoopBreak;
													}
												}
												groupLoopContinue:
											}
											groupLoopBreak:
										}
										if (multipliedGroup == nullptr)
										{
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
											throw StructureBuildingException(std::wstring(L"Locants for inAtoms on the root were either misassigned to the root or were invalid: ") + inLocants.toString() + std::wstring(L" could not be assigned!"));
										}
									}
								}
								else
								{
									multipliedGroup = multipliedElement->getFirstChildElement(XmlDeclarations::GROUP_EL);
								}
								Fragment *multipliedFrag = multipliedGroup->getFrag();

								OutAtom *multiRadicalOutAtom = multiRadicalBR->getOutAtom(i);
								Fragment *multiRadicalFrag = multiRadicalOutAtom->getAtom()->getFrag();
								Element *multiRadicalGroup = multiRadicalFrag->getTokenEl();
								if (multiRadicalGroup->getAttribute(XmlDeclarations::RESOLVED_ATR) == nullptr)
								{
									resolveUnLocantedFeatures(state, multiRadicalGroup->getParent()); //the addition of unlocanted unsaturators can effect the position of radicals e.g. diazenyl
									Attribute tempVar(XmlDeclarations::RESOLVED_ATR, L"yes");
									multiRadicalGroup->addAttribute(&tempVar);
								}

								bool substitutivelyBondedToRoot = false;
								if (inLocants.size() > 0)
								{
									Element *rightMostGroup;
									if (multipliedElement->getName() == XmlDeclarations::BRACKET_EL)
									{
										rightMostGroup = findRightMostGroupInBracket(multipliedElement);
									}
									else
									{
										rightMostGroup = multipliedElement->getFirstChildElement(XmlDeclarations::GROUP_EL);
									}
									Attribute tempVar2(XmlDeclarations::RESOLVED_ATR, L"yes");
									rightMostGroup->addAttribute(&tempVar2); //this group will not be used further within this word but can in principle be a substituent e.g. methylenedisulfonyl dichloride
									if (multipliedGroup->getAttribute(XmlDeclarations::ISAMULTIRADICAL_ATR) != nullptr)
									{ //e.g. methylenedisulfonyl dichloride
										if (multipliedParent->getAttributeValue(XmlDeclarations::INLOCANTS_ATR) != XmlDeclarations::INLOCANTS_DEFAULT)
										{
											throw StructureBuildingException(L"inLocants should not be specified for a multiradical parent in multiplicative nomenclature");
										}
									}
									else
									{
										Atom *from = multiRadicalOutAtom->getAtom();
										int bondOrder = multiRadicalOutAtom->getValency();
										//bonding will be substitutive rather additive as this is bonding to a root
										Atom *atomToJoinTo = nullptr;
										for (int j = inLocants.size() - 1; j >= 0; j--)
										{
											std::wstring locant = inLocants[j];
											if (locant == XmlDeclarations::INLOCANTS_DEFAULT)
											{ //note that if one entry in inLocantArray is default then they all are "default"
												std::vector<Atom*> possibleAtoms = getPossibleAtomsForUnlocantedConnectionToMultipliedRoot(multipliedGroup, bondOrder, i);
												if (possibleAtoms.empty())
												{
													throw StructureBuildingException(L"No suitable atom found for multiplicative operation");
												}
												if (AmbiguityChecker::isSubstitutionAmbiguous(possibleAtoms, 1))
												{
													state->addIsAmbiguous(std::wstring(L"Connection to multiplied group: ") + multipliedGroup->getValue());
												}
												atomToJoinTo = possibleAtoms[0];
												inLocants.erase(inLocants.begin() + j);
												break;
											}
											else
											{
												Atom *inAtom = multipliedFrag->getAtomByLocant(locant);
												if (inAtom != nullptr)
												{
													atomToJoinTo = inAtom;
													inLocants.erase(inLocants.begin() + j);
													break;
												}
											}
										}
										if (atomToJoinTo == nullptr)
										{
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
											throw StructureBuildingException(std::wstring(L"Locants for inAtoms on the root were either misassigned to the root or were invalid: ") + inLocants.toString() + std::wstring(L" could not be assigned!"));
										}

										if (!multiRadicalOutAtom->isSetExplicitly())
										{ //not set explicitly so may be an inappropriate atom
											from = findAtomForUnlocantedRadical(state, from->getFrag(), multiRadicalOutAtom);
										}
										multiRadicalFrag->removeOutAtom(multiRadicalOutAtom);

										state->fragManager->createBond(from, atomToJoinTo, bondOrder);
										if (LOG->isTraceEnabled())
										{
											LOG->trace(std::wstring(L"Substitutively bonded (multiplicative to root) ") + from->getID() + std::wstring(L" (") + from->getFrag()->getTokenEl()->getValue() + std::wstring(L") ") + atomToJoinTo->getID() + std::wstring(L" (") + atomToJoinTo->getFrag()->getTokenEl()->getValue() + std::wstring(L")"));
										}
										substitutivelyBondedToRoot = true;
									}
								}
								if (!substitutivelyBondedToRoot)
								{
									joinFragmentsAdditively(state, multiRadicalFrag, multipliedFrag);
								}
								if (multipliedElement->getName() == XmlDeclarations::BRACKET_EL)
								{
									recursivelyResolveUnLocantedFeatures(state, multipliedElement); //there may be outAtoms that are involved in unlocanted substitution, these can be safely used now e.g. ...bis((3-hydroxy-4-methoxyphenyl)methylene) where (3-hydroxy-4-methoxyphenyl)methylene is the currentElement
								}

								if (inLocants.empty())
								{
									//currentElement is not a root element. Need to build up a new BuildResults so as to call performMultiplicativeOperations again
									//at this stage an outAtom has been removed from the fragment within currentElement through an additive bond
									BuildResults tempVar3(multipliedElement);
									newBr->mergeBuildResults(&tempVar3);
								}
							}

							if (newBr->getFragmentCount() == 1)
							{
								throw StructureBuildingException(L"Multiplicative nomenclature cannot yield only one temporary terminal fragment");
							}
							if (newBr->getFragmentCount() >= 2)
							{
								std::vector<Element*> siblings = OpsinTools::getNextSiblingsOfTypes(multipliedParent, std::vector<std::wstring>{XmlDeclarations::SUBSTITUENT_EL, XmlDeclarations::BRACKET_EL, XmlDeclarations::ROOT_EL});
								if (siblings.empty())
								{
									Element *parentOfMultipliedEl = multipliedParent->getParent();
									if (parentOfMultipliedEl->getName() == XmlDeclarations::BRACKET_EL)
									{ //brackets are allowed
										siblings = OpsinTools::getNextSiblingsOfTypes(parentOfMultipliedEl, std::vector<std::wstring>{XmlDeclarations::SUBSTITUENT_EL, XmlDeclarations::BRACKET_EL, XmlDeclarations::ROOT_EL});
										if (siblings[0]->getAttribute(XmlDeclarations::MULTIPLIER_ATR) == nullptr)
										{
											throw StructureBuildingException(L"Multiplier not found where multiplier was expected for succesful multiplicative nomenclature");
										}
										performMultiplicativeOperations(state, newBr, siblings[0]);
									}
									else
									{
										throw StructureBuildingException(L"Could not find suitable element to continue multiplicative nomenclature");
									}
								}
								else
								{
									if (siblings[0]->getAttribute(XmlDeclarations::MULTIPLIER_ATR) == nullptr)
									{
										throw StructureBuildingException(L"Multiplier not found where multiplier was expected for successful multiplicative nomenclature");
									}
									performMultiplicativeOperations(state, newBr, siblings[0]);
								}
							}

							for (auto clone : clonedElements)
							{ //only insert cloned substituents now so they don't substitute onto each other!
								OpsinTools::insertAfter(multipliedParent, clone);
							}
						}

						std::vector<Atom*> StructureBuildingMethods::getPossibleAtomsForUnlocantedConnectionToMultipliedRoot(Element *multipliedGroup, int bondOrder, int primesAdded) throw(StructureBuildingException)
						{
							Fragment *multipliedFrag = multipliedGroup->getFrag();
							if ((std::wstring(L"yes")) == multipliedGroup->getAttributeValue(XmlDeclarations::USABLEASJOINER_ATR) && multipliedFrag->getDefaultInAtom() == nullptr)
							{
								Element *previous = OpsinTools::getPrevious(multipliedGroup);
								if (previous != nullptr && previous->getName() == XmlDeclarations::MULTIPLIER_EL)
								{
									std::wstring locant = getLocantOfEndOfChainIfGreaterThan1(multipliedFrag, primesAdded);
									if (locant != L"")
									{
										Atom *preferredAtom = multipliedFrag->getAtomByLocantOrThrow(locant);
										std::vector<Atom*> possibleAtoms = FragmentTools::findnAtomsForSubstitution(multipliedFrag->getAtomList(), preferredAtom, 1, bondOrder, true);
										if (possibleAtoms.empty())
										{
											possibleAtoms = Collections::emptyList();
										}
										return possibleAtoms;
									}
								}
							}
							return FragmentTools::findSubstituableAtoms(multipliedFrag, bondOrder);
						}

						std::wstring StructureBuildingMethods::getLocantOfEndOfChainIfGreaterThan1(Fragment *frag, int primes)
						{
							std::wstring primesStr = StringTools::multiplyString(L"'", primes);
							int length = 0;
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
							Atom *next = frag->getAtomByLocant(Integer::toString(length + 1) + primesStr);
							Atom *previous = nullptr;
							while (next != nullptr)
							{
								if (previous != nullptr && previous->getBondToAtom(next) == nullptr)
								{
									break;
								}
								length++;
								previous = next;
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
								next = frag->getAtomByLocant(Integer::toString(length + 1) + primesStr);
							}
							if (length > 1)
							{
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
								return Integer::toString(length) + primesStr;
							}
							return L"";
						}

						Fragment *StructureBuildingMethods::getNextInScopeMultiValentFragment(Element *substituentOrBracket) throw(StructureBuildingException)
						{
							if (substituentOrBracket->getName() != XmlDeclarations::SUBSTITUENT_EL && substituentOrBracket->getName() != XmlDeclarations::BRACKET_EL)
							{
								throw StructureBuildingException(L"Input to this function should be a substituent or bracket");
							}
							if (substituentOrBracket->getParent() == nullptr)
							{
								throw StructureBuildingException(L"substituent did not have a parent!");
							}
							Element *parent = substituentOrBracket->getParent();

							std::vector<Element*> children = OpsinTools::getChildElementsWithTagNames(parent, std::vector<std::wstring>{XmlDeclarations::SUBSTITUENT_EL, XmlDeclarations::BRACKET_EL, XmlDeclarations::ROOT_EL}); //will be returned in index order
							int indexOfSubstituent = parent->find(substituentOrBracket);
							for (auto child : children)
							{
								if (parent->find(child) <= indexOfSubstituent)
								{ //only want things after the input
									continue;
								}
								if (child->getAttribute(XmlDeclarations::MULTIPLIER_ATR) != nullptr)
								{
									continue;
								}
								std::vector<Element*> childDescendants;
								if (child->getName().equals(XmlDeclarations::BRACKET_EL))
								{
									childDescendants = OpsinTools::getDescendantElementsWithTagNames(child, std::vector<std::wstring>{XmlDeclarations::SUBSTITUENT_EL, XmlDeclarations::ROOT_EL}); //will be returned in depth-first order
								}
								else
								{
									childDescendants = std::vector<Element*>();
									childDescendants.push_back(child);
								}
								for (auto descendantChild : childDescendants)
								{
									Element *group = descendantChild->getFirstChildElement(XmlDeclarations::GROUP_EL);
									if (group == nullptr)
									{
										throw StructureBuildingException(L"substituent/root is missing its group");
									}
									Fragment *possibleFrag = group->getFrag();
									if (group->getAttribute(XmlDeclarations::ISAMULTIRADICAL_ATR) != nullptr && (possibleFrag->getOutAtomCount() >= 2 || (possibleFrag->getOutAtomCount() >= 1 && group->getAttribute(XmlDeclarations::RESOLVED_ATR) != nullptr)))
									{
										return possibleFrag;
									}
								}
							}
							return nullptr;
						}

						Element *StructureBuildingMethods::getFirstMultiValentGroup(Element *bracket) throw(StructureBuildingException)
						{
							if (bracket->getName() != XmlDeclarations::BRACKET_EL)
							{
								throw StructureBuildingException(L"Input to this function should be a bracket");
							}

							std::vector<Element*> groups = OpsinTools::getDescendantElementsWithTagName(bracket, XmlDeclarations::GROUP_EL); //will be returned in index order
							for (auto group : groups)
							{
								Fragment *possibleFrag = group->getFrag();
								if (group->getAttribute(XmlDeclarations::ISAMULTIRADICAL_ATR) != nullptr && (possibleFrag->getOutAtomCount() >= 2 || (possibleFrag->getOutAtomCount() >= 1 && group->getAttribute(XmlDeclarations::RESOLVED_ATR) != nullptr)))
								{
									return group;
								}
							}
							return nullptr;
						}

						void StructureBuildingMethods::joinFragmentsAdditively(BuildState *state, Fragment *fragToBeJoined, Fragment *parentFrag) throw(StructureBuildingException)
						{
							Element *elOfFragToBeJoined = fragToBeJoined->getTokenEl();
							if (XmlDeclarations::EPOXYLIKE_SUBTYPE_VAL == elOfFragToBeJoined->getAttributeValue(XmlDeclarations::SUBTYPE_ATR))
							{
								for (int i = 0, l = fragToBeJoined->getOutAtomCount(); i < l; i++)
								{
									OutAtom *outAtom = fragToBeJoined->getOutAtom(i);
									if (outAtom->getLocant() != L"")
									{
										throw StructureBuildingException(std::wstring(L"Inappropriate use of ") + elOfFragToBeJoined->getValue());
									}
								}
							}
							int outAtomCountOnFragToBeJoined = fragToBeJoined->getOutAtomCount();
							if (outAtomCountOnFragToBeJoined == 0)
							{
								throw StructureBuildingException(L"Additive bond formation failure: Fragment expected to have at least one OutAtom but had none");
							}

							if (parentFrag->getOutAtomCount() == 0)
							{
								throw StructureBuildingException(L"Additive bond formation failure: Fragment expected to have at least one OutAtom but had none");
							}
							OutAtom *in_Renamed = nullptr;
							if (parentFrag->getOutAtomCount() > 1)
							{
								int firstOutAtomOrder = parentFrag->getOutAtom(0)->getValency();
								bool unresolvedAmbiguity = false;
								for (int i = 1, l = parentFrag->getOutAtomCount(); i < l; i++)
								{
									OutAtom *outAtom = parentFrag->getOutAtom(i);
									if (outAtom->getValency() != firstOutAtomOrder)
									{
										unresolvedAmbiguity = true;
									}
								}
								if (unresolvedAmbiguity)
								{ //not all outAtoms on parent equivalent
									firstOutAtomOrder = fragToBeJoined->getOutAtom(0)->getValency();
									unresolvedAmbiguity = false;
									for (int i = 1, l = fragToBeJoined->getOutAtomCount(); i < l; i++)
									{
										OutAtom *outAtom = fragToBeJoined->getOutAtom(i);
										if (outAtom->getValency() != firstOutAtomOrder)
										{
											unresolvedAmbiguity = true;
										}
									}
									if (unresolvedAmbiguity && outAtomCountOnFragToBeJoined == 2)
									{ //not all outAtoms on frag to be joined are equivalent either!
										//Solves the specific case of 2,2'-[ethane-1,2-diylbis(azanylylidenemethanylylidene)]diphenol vs 2,2'-[ethane-1,2-diylidenebis(azanylylidenemethanylylidene)]bis(cyclohexan-1-ol)
										//but does not solve the general case as only a single look behind is performed.
										Element *previousGroup = OpsinTools::getPreviousGroup(elOfFragToBeJoined);
										if (previousGroup != nullptr)
										{
											Fragment *previousFrag = previousGroup->getFrag();
											if (previousFrag->getOutAtomCount() > 1)
											{
												int previousGroupFirstOutAtomOrder = previousFrag->getOutAtom(0)->getValency();
												unresolvedAmbiguity = false;
												for (int i = 1, l = previousFrag->getOutAtomCount(); i < l; i++)
												{
													OutAtom *outAtom = previousFrag->getOutAtom(i);
													if (outAtom->getValency() != previousGroupFirstOutAtomOrder)
													{
														unresolvedAmbiguity = true;
													}
												}
												if (!unresolvedAmbiguity && previousGroupFirstOutAtomOrder == parentFrag->getOutAtom(0)->getValency())
												{
													for (int i = 1, l = parentFrag->getOutAtomCount(); i < l; i++)
													{
														OutAtom *outAtom = parentFrag->getOutAtom(i);
														if (outAtom->getValency() != previousGroupFirstOutAtomOrder)
														{
															in_Renamed = outAtom;
															break;
														}
													}
												}
											}
										}
									}
									else
									{
										for (int i = 0, l = parentFrag->getOutAtomCount(); i < l; i++)
										{
											OutAtom *outAtom = parentFrag->getOutAtom(i);
											if (outAtom->getValency() == firstOutAtomOrder)
											{
												in_Renamed = outAtom;
												break;
											}
										}
									}
								}
							}
							if (in_Renamed == nullptr)
							{
								in_Renamed = parentFrag->getOutAtom(0);
							}
							Atom *to = in_Renamed->getAtom();
							int bondOrder = in_Renamed->getValency();
							if (!in_Renamed->isSetExplicitly())
							{ //not set explicitly so may be an inappropriate atom
								to = findAtomForUnlocantedRadical(state, to->getFrag(), in_Renamed);
							}
							parentFrag->removeOutAtom(in_Renamed);

							OutAtom *out = nullptr;

							for (int i = outAtomCountOnFragToBeJoined - 1; i >= 0; i--)
							{
								if (fragToBeJoined->getOutAtom(i)->getValency() == bondOrder)
								{
									out = fragToBeJoined->getOutAtom(i);
									break;
								}
							}

							if (out == nullptr)
							{
								if (outAtomCountOnFragToBeJoined >= bondOrder)
								{ //handles cases like nitrilo needing to be -N= (remove later outAtoms first as per usual)
									int valency = 0;
									Atom *lastOutAtom = fragToBeJoined->getOutAtom(outAtomCountOnFragToBeJoined - 1)->getAtom();
									for (int i = outAtomCountOnFragToBeJoined - 1; i >= 0; i--)
									{
										OutAtom *nextOutAtom = fragToBeJoined->getOutAtom(i);
										if (nextOutAtom->getAtom() != lastOutAtom)
										{
											throw StructureBuildingException(L"Additive bond formation failure: bond order disagreement");
										}
										valency += nextOutAtom->getValency();
										if (valency == bondOrder)
										{
											nextOutAtom->setValency(valency);
											out = nextOutAtom;
											break;
										}
										fragToBeJoined->removeOutAtom(nextOutAtom);
									}
									if (out == nullptr)
									{
										throw StructureBuildingException(L"Additive bond formation failure: bond order disagreement");
									}
								}
								else
								{
									throw StructureBuildingException(L"Additive bond formation failure: bond order disagreement");
								}
							}

							Atom *from = out->getAtom();
							if (!out->isSetExplicitly())
							{ //not set explicitly so may be an inappropriate atom
								from = findAtomForUnlocantedRadical(state, from->getFrag(), out);
							}
							fragToBeJoined->removeOutAtom(out);

							state->fragManager->createBond(from, to, bondOrder);
							if (LOG->isTraceEnabled())
							{
								LOG->trace(std::wstring(L"Additively bonded ") + from->getID() + std::wstring(L" (") + from->getFrag()->getTokenEl()->getValue() + std::wstring(L") ") + to->getID() + std::wstring(L" (") + to->getFrag()->getTokenEl()->getValue() + std::wstring(L")"));
							}
						}

						void StructureBuildingMethods::joinFragmentsSubstitutively(BuildState *state, Fragment *fragToBeJoined, Atom *atomToJoinTo) throw(StructureBuildingException)
						{
							Element *elOfFragToBeJoined = fragToBeJoined->getTokenEl();
							if (XmlDeclarations::EPOXYLIKE_SUBTYPE_VAL == elOfFragToBeJoined->getAttributeValue(XmlDeclarations::SUBTYPE_ATR))
							{
								formEpoxide(state, fragToBeJoined, atomToJoinTo);
								return;
							}
							int outAtomCount = fragToBeJoined->getOutAtomCount();
							if (outAtomCount > 1)
							{
								throw StructureBuildingException(std::wstring(L"Substitutive bond formation failure: Fragment expected to have one OutAtom but had: ") + std::to_wstring(outAtomCount));
							}
							if (outAtomCount == 0)
							{
								throw StructureBuildingException(L"Substitutive bond formation failure: Fragment expected to have one OutAtom but had none");
							}
							if (elOfFragToBeJoined->getAttribute(XmlDeclarations::IMINOLIKE_ATR) != nullptr)
							{ //special case for methylene/imino
								if (fragToBeJoined->getOutAtomCount() == 1 && fragToBeJoined->getOutAtom(0)->getValency() == 1)
								{
									fragToBeJoined->getOutAtom(0)->setValency(2);
								}
							}
							OutAtom *out = fragToBeJoined->getOutAtom(0);
							Atom *from = out->getAtom();
							int bondOrder = out->getValency();
							if (!out->isSetExplicitly())
							{ //not set explicitly so may be an inappropriate atom
								std::vector<Atom*> possibleAtoms = FragmentTools::findnAtomsForSubstitution(fragToBeJoined->getAtomList(), from, 1, bondOrder, false);
								if (possibleAtoms.empty())
								{
									throw StructureBuildingException(L"Failed to assign all unlocanted radicals to actual atoms without violating valency");
								}
								if (!((XmlDeclarations::ALKANESTEM_SUBTYPE_VAL == fragToBeJoined->getSubType() || XmlDeclarations::HETEROSTEM_SUBTYPE_VAL == fragToBeJoined->getSubType()) && possibleAtoms[0]->equals(fragToBeJoined->getFirstAtom())))
								{
									if (AmbiguityChecker::isSubstitutionAmbiguous(possibleAtoms, 1))
									{
										state->addIsAmbiguous(std::wstring(L"Positioning of radical on: ") + fragToBeJoined->getTokenEl()->getValue());
									}
								}
								from = possibleAtoms[0];
							}
							fragToBeJoined->removeOutAtom(out);

							state->fragManager->createBond(from, atomToJoinTo, bondOrder);
							if (LOG->isTraceEnabled())
							{
								LOG->trace(std::wstring(L"Substitutively bonded ") + from->getID() + std::wstring(L" (") + from->getFrag()->getTokenEl()->getValue() + std::wstring(L") ") + atomToJoinTo->getID() + std::wstring(L" (") + atomToJoinTo->getFrag()->getTokenEl()->getValue() + std::wstring(L")"));
							}
						}

						std::vector<Atom*> StructureBuildingMethods::formEpoxide(BuildState *state, Fragment *bridgingFragment, Atom *atomToJoinTo) throw(StructureBuildingException)
						{
							Fragment *fragToJoinTo = atomToJoinTo->getFrag();
							std::vector<Atom*> atomList = fragToJoinTo->getAtomList();
							if (atomList.size() == 1)
							{
								throw StructureBuildingException(L"Epoxides must be formed between two different atoms");
							}
							Atom *firstAtomToJoinTo;
							if (bridgingFragment->getOutAtom(0)->getLocant() != L"")
							{
								firstAtomToJoinTo = fragToJoinTo->getAtomByLocantOrThrow(bridgingFragment->getOutAtom(0)->getLocant());
							}
							else
							{
								firstAtomToJoinTo = atomToJoinTo;
							}
							Atom *chalcogenAtom1 = bridgingFragment->getOutAtom(0)->getAtom();
							bridgingFragment->removeOutAtom(0);

							//In epoxy chalcogenAtom1 will be chalcogenAtom2. Methylenedioxy is also handled by this method
							state->fragManager->createBond(chalcogenAtom1, firstAtomToJoinTo, 1);

							Atom *secondAtomToJoinTo;
							if (bridgingFragment->getOutAtom(0)->getLocant() != L"")
							{
								secondAtomToJoinTo = fragToJoinTo->getAtomByLocantOrThrow(bridgingFragment->getOutAtom(0)->getLocant());
							}
							else
							{
								int index = atomList.find(firstAtomToJoinTo);
								Atom *preferredAtom = (index + 1 >= atomList.size()) ? atomList[index - 1] : atomList[index + 1];
								std::vector<Atom*> possibleSecondAtom = FragmentTools::findnAtomsForSubstitution(fragToJoinTo->getAtomList(), preferredAtom, 1, 1, true);
								if (possibleSecondAtom.size() > 0)
								{
									possibleSecondAtom.removeAll(Collections::singleton(firstAtomToJoinTo));
								}
								if (possibleSecondAtom.empty() || possibleSecondAtom.empty())
								{
									throw StructureBuildingException(L"Unable to find suitable atom to form bridge");
								}
								if (AmbiguityChecker::isSubstitutionAmbiguous(possibleSecondAtom, 1))
								{
									state->addIsAmbiguous(std::wstring(L"Addition of bridge to: ") + fragToJoinTo->getTokenEl()->getValue());
								}
								secondAtomToJoinTo = possibleSecondAtom[0];
							}
							Atom *chalcogenAtom2 = bridgingFragment->getOutAtom(0)->getAtom();
							bridgingFragment->removeOutAtom(0);
							if (chalcogenAtom1->equals(chalcogenAtom2) && firstAtomToJoinTo == secondAtomToJoinTo)
							{
								throw StructureBuildingException(L"Epoxides must be formed between two different atoms");
							}
							state->fragManager->createBond(chalcogenAtom2, secondAtomToJoinTo, 1);
							CycleDetector::assignWhetherAtomsAreInCycles(bridgingFragment);
							return std::vector<Atom*>{firstAtomToJoinTo, secondAtomToJoinTo};
						}

						std::vector<Atom*> StructureBuildingMethods::findAtomsForSubstitution(Element *subOrBracket, int numberOfSubstitutions, int bondOrder)
						{
							bool rootHandled = false;
							std::vector<Element*> possibleParents = findAlternativeGroups(subOrBracket);
							for (int i = 0, l = possibleParents.size(); i < l; i++)
							{
								Element *possibleParent = possibleParents[i];
								Fragment *frag = possibleParent->getFrag();
								std::vector<Atom*> substitutableAtoms;
								if (possibleParent->getParent()->getName() == XmlDeclarations::ROOT_EL)
								{ //consider all root groups as if they were one
									if (rootHandled)
									{
										continue;
									}
									std::vector<Atom*> atoms = frag->getAtomList();
									for (int j = i + 1; j < l; j++)
									{
										Element *possibleOtherRoot = possibleParents[j];
										if (possibleOtherRoot->getParent()->getName() == XmlDeclarations::ROOT_EL)
										{
											atoms.addAll(possibleOtherRoot->getFrag()->getAtomList());
										}
									}
									rootHandled = true;
									substitutableAtoms = FragmentTools::findnAtomsForSubstitution(atoms, frag->getDefaultInAtom(), numberOfSubstitutions, bondOrder, true);
								}
								else
								{
									substitutableAtoms = FragmentTools::findnAtomsForSubstitution(frag, numberOfSubstitutions, bondOrder);
								}
								if (substitutableAtoms.size() > 0)
								{
									return substitutableAtoms;
								}
							}
							return nullptr;
						}

						std::vector<Fragment*> StructureBuildingMethods::findAlternativeFragments(Element *startingElement)
						{
							std::vector<Fragment*> foundFragments;
							for (auto group : findAlternativeGroups(startingElement))
							{
								foundFragments.push_back(group->getFrag());
							}
							return foundFragments;
						}

						std::vector<Element*> StructureBuildingMethods::findAlternativeGroups(Element *startingElement)
						{
							Deque<Element*> *stack = new ArrayDeque<Element*>();
							stack->add(startingElement->getParent());
							std::vector<Element*> foundGroups;
							bool doneFirstIteration = false; //check on index only done on first iteration to only get elements with an index greater than the starting element
							while (stack->size() > 0)
							{
								Element *currentElement = stack->removeLast();
								if (currentElement->getName() == XmlDeclarations::GROUP_EL)
								{
									foundGroups.push_back(currentElement);
									continue;
								}
								std::vector<Element*> siblings = OpsinTools::getChildElementsWithTagNames(currentElement, std::vector<std::wstring>{XmlDeclarations::BRACKET_EL, XmlDeclarations::SUBSTITUENT_EL, XmlDeclarations::ROOT_EL});

								std::vector<Element*> bracketted;
								for (auto bracketOrSubOrRoot : siblings)
								{
									if (!doneFirstIteration && currentElement->find(bracketOrSubOrRoot) <= currentElement->find(startingElement))
									{
										continue;
									}
									if (bracketOrSubOrRoot->getAttribute(XmlDeclarations::MULTIPLIER_ATR) != nullptr)
									{
										continue;
									}
									if (bracketOrSubOrRoot->getName().equals(XmlDeclarations::BRACKET_EL))
									{
										if (XmlDeclarations::IMPLICIT_TYPE_VAL == bracketOrSubOrRoot->getAttributeValue(XmlDeclarations::TYPE_ATR))
										{
											stack->add(bracketOrSubOrRoot);
										}
										else
										{
											bracketted.push_back(bracketOrSubOrRoot);
										}
									}
									else
									{
										Element *group = bracketOrSubOrRoot->getFirstChildElement(XmlDeclarations::GROUP_EL);
										stack->add(group);
									}
								}
								//locanting into brackets is rarely the desired answer so place at the bottom of the stack
								for (int i = bracketted.size() - 1; i >= 0; i--)
								{
									stack->addFirst(bracketted[i]);
								}
								doneFirstIteration = true;
							}
							return foundGroups;
						}

						Fragment *StructureBuildingMethods::findFragmentWithLocant(Element *startingElement, const std::wstring &locant) throw(StructureBuildingException)
						{
							Deque<Element*> *stack = new ArrayDeque<Element*>();
							stack->add(startingElement->getParent());
							bool doneFirstIteration = false; //check on index only done on first iteration to only get elements with an index greater than the starting element
							Fragment *monoNuclearHydride = nullptr; //e.g. methyl/methane - In this case no locant would be expected as unlocanted substitution is always unambiguous. Hence deprioritise
							while (stack->size() > 0)
							{
								Element *currentElement = stack->removeLast();
								if (currentElement->getName() == XmlDeclarations::SUBSTITUENT_EL || currentElement->getName() == XmlDeclarations::ROOT_EL)
								{
									Fragment *groupFrag = currentElement->getFirstChildElement(XmlDeclarations::GROUP_EL)->getFrag();
									if (monoNuclearHydride != nullptr && currentElement->getAttribute(XmlDeclarations::LOCANT_ATR) != nullptr)
									{ //It looks like all groups are locanting onto the monoNuclearHydride e.g. 1-oxo-1-phenyl-sulfanylidene
										return monoNuclearHydride;
									}
									if (groupFrag->hasLocant(locant))
									{
										if (locant == L"1" && groupFrag->getAtomCount() == 1)
										{
											if (monoNuclearHydride == nullptr)
											{
												monoNuclearHydride = groupFrag;
											}
										}
										else
										{
											return groupFrag;
										}
									}
									continue;
								}
								else if (monoNuclearHydride != nullptr)
								{
									return monoNuclearHydride;
								}
								std::vector<Element*> siblings = OpsinTools::getChildElementsWithTagNames(currentElement, std::vector<std::wstring>{XmlDeclarations::BRACKET_EL, XmlDeclarations::SUBSTITUENT_EL, XmlDeclarations::ROOT_EL});

								std::vector<Element*> bracketted;
								if (!doneFirstIteration)
								{ //on the first iteration, ignore elements before the starting element and favour the element directly after the starting element (conditions apply)
									int indexOfStartingEl = currentElement->find(startingElement);
									Element *substituentToTryFirst = nullptr;
									for (auto bracketOrSubOrRoot : siblings)
									{
										int indexOfCurrentEl = currentElement->find(bracketOrSubOrRoot);
										if (indexOfCurrentEl <= indexOfStartingEl)
										{
											continue;
										}
										if (bracketOrSubOrRoot->getAttribute(XmlDeclarations::MULTIPLIER_ATR) != nullptr)
										{
											continue;
										}
										if (bracketOrSubOrRoot->getName().equals(XmlDeclarations::BRACKET_EL))
										{
											if (XmlDeclarations::IMPLICIT_TYPE_VAL == bracketOrSubOrRoot->getAttributeValue(XmlDeclarations::TYPE_ATR))
											{
												stack->add(bracketOrSubOrRoot);
											}
											else
											{
												bracketted.push_back(bracketOrSubOrRoot);
											}
										}
										else
										{
											if (substituentToTryFirst == nullptr && bracketOrSubOrRoot->getAttribute(XmlDeclarations::LOCANT_EL) == nullptr && OpsinTools::MATCH_NUMERIC_LOCANT->matcher(locant).matches())
											{
												substituentToTryFirst = bracketOrSubOrRoot;
											}
											else
											{
												stack->add(bracketOrSubOrRoot);
											}
										}
									}
									if (substituentToTryFirst != nullptr)
									{
										stack->add(substituentToTryFirst);
									}
									doneFirstIteration = true;
								}
								else
								{
									for (auto bracketOrSubOrRoot : siblings)
									{
										if (bracketOrSubOrRoot->getAttribute(XmlDeclarations::MULTIPLIER_ATR) != nullptr)
										{
											continue;
										}
										if (bracketOrSubOrRoot->getName().equals(XmlDeclarations::BRACKET_EL))
										{
											if (XmlDeclarations::IMPLICIT_TYPE_VAL == bracketOrSubOrRoot->getAttributeValue(XmlDeclarations::TYPE_ATR))
											{
												stack->add(bracketOrSubOrRoot);
											}
											else
											{
												bracketted.push_back(bracketOrSubOrRoot);
											}
										}
										else
										{
											stack->add(bracketOrSubOrRoot);
										}
									}
								}
								//locanting into brackets is rarely the desired answer so place at the bottom of the stack
								for (int i = bracketted.size() - 1; i >= 0; i--)
								{
									stack->addFirst(bracketted[i]);
								}
							}
							return monoNuclearHydride;
						}

						Element *StructureBuildingMethods::findRightMostGroupInBracket(Element *bracket)
						{
							std::vector<Element*> subsBracketsAndRoots = OpsinTools::getChildElementsWithTagNames(bracket, std::vector<std::wstring>{XmlDeclarations::BRACKET_EL, XmlDeclarations::SUBSTITUENT_EL, XmlDeclarations::ROOT_EL});
							Element *lastSubsBracketOrRoot = subsBracketsAndRoots[subsBracketsAndRoots.size() - 1];
							while (lastSubsBracketOrRoot->getName() == XmlDeclarations::BRACKET_EL)
							{
								subsBracketsAndRoots = OpsinTools::getChildElementsWithTagNames(lastSubsBracketOrRoot, std::vector<std::wstring>{XmlDeclarations::BRACKET_EL, XmlDeclarations::SUBSTITUENT_EL, XmlDeclarations::ROOT_EL});
								lastSubsBracketOrRoot = subsBracketsAndRoots[subsBracketsAndRoots.size() - 1];
							}
							return findRightMostGroupInSubOrRoot(lastSubsBracketOrRoot);
						}

						Element *StructureBuildingMethods::findRightMostGroupInSubBracketOrRoot(Element *subBracketOrRoot)
						{
							if (subBracketOrRoot->getName() == XmlDeclarations::BRACKET_EL)
							{
								return findRightMostGroupInBracket(subBracketOrRoot);
							}
							else
							{
								return findRightMostGroupInSubOrRoot(subBracketOrRoot);
							}
						}

						Element *StructureBuildingMethods::findRightMostGroupInSubOrRoot(Element *subOrRoot)
						{
							for (int i = subOrRoot->getChildCount() - 1; i >= 0; i--)
							{
								Element *el = subOrRoot->getChild(i);
								if (el->getName() == XmlDeclarations::GROUP_EL)
								{
									return el;
								}
							}
							return nullptr;
						}

						bool StructureBuildingMethods::potentiallyCanSubstitute(Element *subBracketOrRoot)
						{
							Element *parent = subBracketOrRoot->getParent();
							std::vector<Element*> children = parent->getChildElements();
							for (int i = parent->find(subBracketOrRoot) + 1 ; i < children.size(); i++)
							{
								if (children[i]->getName() != XmlDeclarations::HYPHEN_EL)
								{
									return true;
								}
							}
							return false;
						}

						std::wstring StructureBuildingMethods::checkForBracketedPrimedLocantSpecialCase(Element *subBracketOrRoot, const std::wstring &locantString)
						{
							int terminalPrimes = StringTools::countTerminalPrimes(locantString);
							if (terminalPrimes > 0)
							{
								int brackettingDepth = 0;
								Element *parent = subBracketOrRoot->getParent();
								while (parent != nullptr && parent->getName() == XmlDeclarations::BRACKET_EL)
								{
									if (XmlDeclarations::IMPLICIT_TYPE_VAL != parent->getAttributeValue(XmlDeclarations::TYPE_ATR))
									{
										brackettingDepth++;
									}
									parent = parent->getParent();
								}
								if (terminalPrimes == brackettingDepth)
								{
									return locantString.substr(0, locantString.length() - terminalPrimes);
								}
							}
							return L"";
						}

						void StructureBuildingMethods::checkAndApplySpecialCaseWhereOutAtomsCanBeCombinedOrThrow(Fragment *frag, Element *group) throw(StructureBuildingException)
						{
							int outAtomCount = frag->getOutAtomCount();
							if (outAtomCount <= 1)
							{
								return;
							}
							if (XmlDeclarations::EPOXYLIKE_SUBTYPE_VAL == group->getAttributeValue(XmlDeclarations::SUBTYPE_ATR))
							{
								return;
							}
							std::wstring groupValue = group->getValue();
							if (groupValue == L"oxy" || groupValue == L"thio" || groupValue == L"seleno" || groupValue == L"telluro")
							{ //always bivalent
								return;
							}
							//special case: all outAtoms on same atom e.g. methylenecyclohexane
							Atom *firstOutAtom = frag->getOutAtom(0)->getAtom();
							int valencyOfOutAtom = 0;
							for (int i = outAtomCount - 1; i >= 0 ; i--)
							{ //remove all outAtoms and add one with the total valency of all those that have been removed
								OutAtom *out = frag->getOutAtom(i);
								if (!out->getAtom()->equals(firstOutAtom))
								{
									throw StructureBuildingException(std::wstring(L"Substitutive bond formation failure: Fragment expected to have one OutAtom but had: ") + std::to_wstring(outAtomCount));
								}
								valencyOfOutAtom += out->getValency();
								frag->removeOutAtom(i);
							}
							frag->addOutAtom(firstOutAtom, valencyOfOutAtom, true);
						}

						int StructureBuildingMethods::calculateSubstitutableHydrogenAtoms(Atom *atom)
						{
							int valency = atom->determineValency(true);
							int currentValency = atom->getIncomingValency() + atom->getOutValency();
							int substitutableHydrogen = valency - currentValency;
							return substitutableHydrogen >= 0 ? substitutableHydrogen : 0;
						}

						void StructureBuildingMethods::addPrimesToLocantedStereochemistryElements(Element *subOrBracket, const std::wstring &primesString)
						{
							std::vector<Element*> stereoChemistryElements = OpsinTools::getDescendantElementsWithTagName(subOrBracket, XmlDeclarations::STEREOCHEMISTRY_EL);
							for (auto stereoChemistryElement : stereoChemistryElements)
							{
								if (stereoChemistryElement->getAttribute(XmlDeclarations::LOCANT_ATR) != nullptr)
								{
									stereoChemistryElement->getAttribute(XmlDeclarations::LOCANT_ATR)->setValue(stereoChemistryElement->getAttributeValue(XmlDeclarations::LOCANT_ATR) + primesString);
								}
							}
						}

						boost::optional<int> StructureBuildingMethods::levelsToWordEl(Element *element)
						{
							int count = 0;
							while (element->getName() != XmlDeclarations::WORD_EL)
							{
								element = element->getParent();
								if (element == nullptr)
								{
									return boost::none;
								}
								count++;
							}
							return count;
						}
					}
				}
			}
		}
	}
}
