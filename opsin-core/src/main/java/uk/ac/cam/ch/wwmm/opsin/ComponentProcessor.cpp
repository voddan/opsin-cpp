#include "ComponentProcessor.h"
#include "FunctionalReplacement.h"
#include "SuffixApplier.h"
#include "BuildState.h"
#include "ComponentGenerationException.h"
#include "Element.h"
#include "StructureBuildingException.h"
#include "OpsinTools.h"
#include "XmlDeclarations.h"
#include "WordRule.h"
#include "WordType.h"
#include "Fragment.h"
#include "FusedRingBuilder.h"
#include "WordRulesOmittedSpaceCorrector.h"
#include "StringTools.h"
#include "Atom.h"
#include "OutAtom.h"
#include "Bond.h"
#include "FragmentTools.h"
#include "ChemEl.h"
#include "Attribute.h"
#include "TokenEl.h"
#include "CycleDetector.h"
#include "AtomParity.h"
#include "StereochemistryHandler.h"
#include "SuffixRule.h"
#include "SuffixRuleType.h"
#include "StructureBuildingMethods.h"
#include "GroupingEl.h"
#include "AmbiguityChecker.h"

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
//						import static uk.ac.cam.ch.wwmm.opsin.XmlDeclarations.*;
//						import static uk.ac.cam.ch.wwmm.opsin.OpsinTools.*;
std::vector<std::wstring> const ComponentProcessor::traditionalAlkanePositionNames = std::vector<std::wstring>{L"alpha", L"beta", L"gamma", L"delta", L"epsilon", L"zeta"};
const std::unordered_map<std::wstring, std::vector<std::wstring>> ComponentProcessor::specialHWRings = std::unordered_map<std::wstring, std::vector<std::wstring>>();

						ComponentProcessor::StaticConstructor::StaticConstructor()
						{
															//The first entry of the array is a special instruction e.g. blocked or saturated. The correct order of the heteroatoms follows
															//terminal e is ignored from all of the keys as it is optional in the input name
															specialHWRings[L"oxin"] = std::vector<std::wstring>{L"blocked"};
															specialHWRings[L"azin"] = std::vector<std::wstring>{L"blocked"};
                                                    
															specialHWRings[L"selenin"] = std::vector<std::wstring>{L"not_icacid", L"Se",L"C",L"C",L"C",L"C",L"C"};
															specialHWRings[L"tellurin"] = std::vector<std::wstring>{L"not_icacid", L"Te",L"C",L"C",L"C",L"C",L"C"};
                                                    
															specialHWRings[L"thiol"] = std::vector<std::wstring>{L"not_nothingOrOlate", L"S",L"C",L"C",L"C",L"C"};
															specialHWRings[L"selenol"] = std::vector<std::wstring>{L"not_nothingOrOlate", L"Se",L"C",L"C",L"C",L"C"};
															specialHWRings[L"tellurol"] = std::vector<std::wstring>{L"not_nothingOrOlate", L"Te",L"C",L"C",L"C",L"C"};
                                                    
															specialHWRings[L"oxazol"] = std::vector<std::wstring>{L"",L"O",L"C",L"N",L"C",L"C"};
															specialHWRings[L"thiazol"] = std::vector<std::wstring>{L"",L"S",L"C",L"N",L"C",L"C"};
															specialHWRings[L"selenazol"] = std::vector<std::wstring>{L"",L"Se",L"C",L"N",L"C",L"C"};
															specialHWRings[L"tellurazol"] = std::vector<std::wstring>{L"",L"Te",L"C",L"N",L"C",L"C"};
															specialHWRings[L"oxazolidin"] = std::vector<std::wstring>{L"",L"O",L"C",L"N",L"C",L"C"};
															specialHWRings[L"thiazolidin"] = std::vector<std::wstring>{L"",L"S",L"C",L"N",L"C",L"C"};
															specialHWRings[L"selenazolidin"] = std::vector<std::wstring>{L"",L"Se",L"C",L"N",L"C",L"C"};
															specialHWRings[L"tellurazolidin"] = std::vector<std::wstring>{L"",L"Te",L"C",L"N",L"C",L"C"};
															specialHWRings[L"oxazolid"] = std::vector<std::wstring>{L"",L"O",L"C",L"N",L"C",L"C"};
															specialHWRings[L"thiazolid"] = std::vector<std::wstring>{L"",L"S",L"C",L"N",L"C",L"C"};
															specialHWRings[L"selenazolid"] = std::vector<std::wstring>{L"",L"Se",L"C",L"N",L"C",L"C"};
															specialHWRings[L"tellurazolid"] = std::vector<std::wstring>{L"",L"Te",L"C",L"N",L"C",L"C"};
															specialHWRings[L"oxazolin"] = std::vector<std::wstring>{L"",L"O",L"C",L"N",L"C",L"C"};
															specialHWRings[L"thiazolin"] = std::vector<std::wstring>{L"",L"S",L"C",L"N",L"C",L"C"};
															specialHWRings[L"selenazolin"] = std::vector<std::wstring>{L"",L"Se",L"C",L"N",L"C",L"C"};
															specialHWRings[L"tellurazolin"] = std::vector<std::wstring>{L"",L"Te",L"C",L"N",L"C",L"C"};
                                                    
															specialHWRings[L"oxoxolan"] = std::vector<std::wstring>{L"",L"O",L"C",L"O",L"C",L"C"};
															specialHWRings[L"oxoxol"] = std::vector<std::wstring>{L"",L"O",L"C",L"O",L"C",L"C"};
															specialHWRings[L"oxoxan"] = std::vector<std::wstring>{L"",L"O",L"C",L"C",L"O",L"C",L"C"};
															specialHWRings[L"oxoxin"] = std::vector<std::wstring>{L"",L"O",L"C",L"C",L"O",L"C",L"C"};
                                                    
															specialHWRings[L"boroxin"] = std::vector<std::wstring>{L"saturated",L"O",L"B",L"O",L"B",L"O",L"B"};
															specialHWRings[L"borazin"] = std::vector<std::wstring>{L"saturated",L"N",L"B",L"N",L"B",L"N",L"B"};
															specialHWRings[L"borthiin"] = std::vector<std::wstring>{L"saturated",L"S",L"B",L"S",L"B",L"S",L"B"};
						}

ComponentProcessor::StaticConstructor ComponentProcessor::staticConstructor;

						ComponentProcessor::ComponentProcessor(BuildState *state, SuffixApplier *suffixApplier) : functionalReplacement(new FunctionalReplacement(state)), suffixApplier(suffixApplier), state(state)
						{
						}

						void ComponentProcessor::processParse(Element *parse) throw(ComponentGenerationException, StructureBuildingException)
						{
							std::vector<Element*> words = OpsinTools::getDescendantElementsWithTagName(parse, XmlDeclarations::WORD_EL);
							int wordCount = words.size();
							for (int i = wordCount - 1; i >= 0; i--)
							{
								Element *word = words[i];
								std::wstring wordRule = OpsinTools::getParentWordRule(word)->getAttributeValue(XmlDeclarations::WORDRULE_EL);
								state->currentWordRule = StringHelper::fromString<WordRule>(wordRule);
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
								if (word->getAttributeValue(XmlDeclarations::TYPE_ATR) == WordType::functionalTerm->toString())
								{
									continue; //functionalTerms are handled on a case by case basis by wordRules
								}

								std::vector<Element*> roots = OpsinTools::getDescendantElementsWithTagName(word, XmlDeclarations::ROOT_EL);
								if (roots.size() > 1)
								{
									throw ComponentGenerationException(std::wstring(L"Multiple roots, but only 0 or 1 were expected. Found: ") + roots.size());
								}
								std::vector<Element*> substituents = OpsinTools::getDescendantElementsWithTagName(word, XmlDeclarations::SUBSTITUENT_EL);
								std::vector<Element*> substituentsAndRoot = OpsinTools::combineElementLists(substituents, roots);
								std::vector<Element*> brackets = OpsinTools::getDescendantElementsWithTagName(word, XmlDeclarations::BRACKET_EL);
								std::vector<Element*> substituentsAndRootAndBrackets = OpsinTools::combineElementLists(substituentsAndRoot, brackets);
								std::vector<Element*> groups = OpsinTools::getDescendantElementsWithTagName(word, XmlDeclarations::GROUP_EL);

								for (auto group : groups)
								{
									Fragment *thisFrag = resolveGroup(state, group);
									processChargeAndOxidationNumberSpecification(group, thisFrag); //e.g. mercury(2+) or mercury(II)
								}

								for (auto subOrRoot : substituentsAndRoot)
								{
									applyDLPrefixes(subOrRoot);
									processCarbohydrates(subOrRoot); //e.g. glucopyranose (needs to be done before determineLocantMeaning to cope with alpha,beta for undefined anomer stereochemistry)
								}

								Element *finalSubOrRootInWord = word->getChild(word->getChildCount() - 1);
								while (finalSubOrRootInWord->getName() != XmlDeclarations::ROOT_EL && finalSubOrRootInWord->getName() != XmlDeclarations::SUBSTITUENT_EL)
								{
									std::vector<Element*> children = OpsinTools::getChildElementsWithTagNames(finalSubOrRootInWord, std::vector<std::wstring>{XmlDeclarations::ROOT_EL, XmlDeclarations::SUBSTITUENT_EL, XmlDeclarations::BRACKET_EL});
									if (children.empty())
									{
										throw ComponentGenerationException(L"Unable to find finalSubOrRootInWord");
									}
									finalSubOrRootInWord = children[children.size() - 1];
								}

								for (auto subOrRootOrBracket : substituentsAndRootAndBrackets)
								{
									determineLocantMeaning(subOrRootOrBracket, finalSubOrRootInWord);
								}

								for (auto subOrRoot : substituentsAndRoot)
								{
									processMultipliers(subOrRoot);
									detectConjunctiveSuffixGroups(subOrRoot, groups);
									matchLocantsToDirectFeatures(subOrRoot);

									std::vector<Element*> groupsOfSubOrRoot = subOrRoot->getChildElements(XmlDeclarations::GROUP_EL);
									if (groupsOfSubOrRoot.size() > 0)
									{
										Element *lastGroupInSubOrRoot = groupsOfSubOrRoot[groupsOfSubOrRoot.size() - 1];
										preliminaryProcessSuffixes(lastGroupInSubOrRoot, subOrRoot->getChildElements(XmlDeclarations::SUFFIX_EL));
									}
								}
								for (int j = substituents.size() - 1; j >= 0; j--)
								{
									Element *substituent = substituents[j];
									if (substituent->getChildElements(XmlDeclarations::GROUP_EL).empty())
									{
										bool removed = removeAndMoveToAppropriateGroupIfHydroSubstituent(substituent); //this REMOVES a substituent just containing hydro/perhydro elements and moves these elements in front of an appropriate ring
										if (!removed)
										{
											removed = removeAndMoveToAppropriateGroupIfSubtractivePrefix(substituent);
										}
										if (!removed)
										{
											removed = removeAndMoveToAppropriateGroupIfRingBridge(substituent);
										}
										if (!removed)
										{
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("OPSIN Bug: Encountered substituent with no group!: " + substituent.toXML());
											throw std::exception();
										}
										substituents.erase(substituents.begin() + j);
//JAVA TO C++ CONVERTER TODO TASK: The Java Collection 'remove(Object)' method is not converted:
										substituentsAndRoot.remove(substituent);
//JAVA TO C++ CONVERTER TODO TASK: The Java Collection 'remove(Object)' method is not converted:
										substituentsAndRootAndBrackets.remove(substituent);
									}
								}

								functionalReplacement->processAcidReplacingFunctionalClassNomenclature(finalSubOrRootInWord, word);

								if (functionalReplacement->processPrefixFunctionalReplacementNomenclature(groups, substituents))
								{ //true if functional replacement performed, 1 or more substituents will have been removed
									substituentsAndRoot = OpsinTools::combineElementLists(substituents, roots);
									substituentsAndRootAndBrackets = OpsinTools::combineElementLists(substituentsAndRoot, brackets);
								}

								handleGroupIrregularities(groups);

								for (auto subOrRoot : substituentsAndRoot)
								{
									processHW(subOrRoot); //hantzch-widman rings
									FusedRingBuilder::processFusedRings(state, subOrRoot);
									processFusedRingBridges(subOrRoot);
									assignElementSymbolLocants(subOrRoot);
									processRingAssemblies(subOrRoot);
									processPolyCyclicSpiroNomenclature(subOrRoot);
								}

								for (auto subOrRoot : substituentsAndRoot)
								{
									applyLambdaConvention(subOrRoot);
									handleMultiRadicals(subOrRoot);
								}

								addImplicitBracketsToAminoAcids(groups, brackets);
								for (auto substituent : substituents)
								{
									matchLocantsToIndirectFeatures(substituent);
									addImplicitBracketsWhenSubstituentHasTwoLocants(substituent, brackets);
									implicitlyBracketToPreviousSubstituentIfAppropriate(substituent, brackets);
								}
								for (auto root : roots)
								{
									matchLocantsToIndirectFeatures(root);
								}

								for (auto subOrRoot : substituentsAndRoot)
								{
									assignImplicitLocantsToDiTerminalSuffixes(subOrRoot);
									processConjunctiveNomenclature(subOrRoot);
									suffixApplier->resolveSuffixes(subOrRoot->getFirstChildElement(XmlDeclarations::GROUP_EL), subOrRoot->getChildElements(XmlDeclarations::SUFFIX_EL));
								}

								moveErroneouslyPositionedLocantsAndMultipliers(brackets); //e.g. (tetramethyl)azanium == tetra(methyl)azanium
								std::vector<Element*> children = OpsinTools::getChildElementsWithTagNames(word, std::vector<std::wstring>{XmlDeclarations::ROOT_EL, XmlDeclarations::SUBSTITUENT_EL, XmlDeclarations::BRACKET_EL});
								addImplicitBracketsWhenFirstSubstituentHasTwoMultipliers(children[0], brackets); //e.g. ditrifluoroacetic acid --> di(trifluoroacetic acid)
								while (children.size() == 1)
								{
									children = OpsinTools::getChildElementsWithTagNames(children[0], std::vector<std::wstring>{XmlDeclarations::ROOT_EL, XmlDeclarations::SUBSTITUENT_EL, XmlDeclarations::BRACKET_EL});
								}
								if (children.size() > 0)
								{
									assignLocantsToMultipliedRootIfPresent(children[children.size() - 1]); //multiplicative nomenclature e.g. methylenedibenzene or 3,4'-oxydipyridine
								}
								substituentsAndRootAndBrackets = OpsinTools::combineElementLists(substituentsAndRoot, brackets); //implicit brackets may have been created
								for (auto subBracketOrRoot : substituentsAndRootAndBrackets)
								{
									assignLocantsAndMultipliers(subBracketOrRoot);
								}
								processBiochemicalLinkageDescriptors(substituents, brackets);
								processWordLevelMultiplierIfApplicable(word, roots, wordCount);
							}
							WordRulesOmittedSpaceCorrector tempVar(state, parse);
							(&tempVar)->correctOmittedSpaces(); //TODO where should this go?
						}

						Fragment *ComponentProcessor::resolveGroup(BuildState *state, Element *group) throw(StructureBuildingException, ComponentGenerationException)
						{
							std::wstring groupValue = group->getAttributeValue(XmlDeclarations::VALUE_ATR);
							std::wstring labelsValue = group->getAttributeValue(XmlDeclarations::LABELS_ATR);
							Fragment *thisFrag = state->fragManager->buildSMILES(groupValue, group, labelsValue != L"" ? labelsValue : XmlDeclarations::NONE_LABELS_VAL);
							group->setFrag(thisFrag);

							//processes groups like cymene and xylene whose structure is determined by the presence of a locant in front e.g. p-xylene
							processXyleneLikeNomenclature(state, group, thisFrag);

							setFragmentDefaultInAtomIfSpecified(thisFrag, group);
							setFragmentFunctionalAtomsIfSpecified(group, thisFrag);
							applyTraditionalAlkaneNumberingIfAppropriate(group, thisFrag);
							applyHomologyGroupLabelsIfSpecified(group, thisFrag);
							return thisFrag;
						}

						ComponentProcessor::AtomReference::AtomReference(AtomReferenceType referenceType, const std::wstring &reference) : referenceType(referenceType), reference(reference)
						{
						}

						ComponentProcessor::AddGroup::AddGroup(Fragment *frag, AtomReference *atomReference) : frag(frag)
						{
							this->atomReference = atomReference;
						}

						ComponentProcessor::AddHeteroatom::AddHeteroatom(const std::wstring &heteroAtomSmiles, AtomReference *atomReference) : heteroAtomSmiles(heteroAtomSmiles)
						{
							this->atomReference = atomReference;
						}

						ComponentProcessor::AddBond::AddBond(int bondOrder, AtomReference *atomReference) : bondOrder(bondOrder)
						{
							this->atomReference = atomReference;
						}

						void ComponentProcessor::processXyleneLikeNomenclature(BuildState *state, Element *group, Fragment *parentFrag) throw(StructureBuildingException, ComponentGenerationException)
						{
							bool ambiguous = false;

							if (group->getAttribute(XmlDeclarations::ADDGROUP_ATR) != nullptr)
							{
								std::wstring addGroupInformation = group->getAttributeValue(XmlDeclarations::ADDGROUP_ATR);
								std::vector<AddGroup*> groupsToBeAdded;
								////typically only one, but 2 in the case of xylene and quinones
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
								for (std::wstring groupToBeAdded : addGroupInformation.split(L";"))
								{
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
									std::vector<std::wstring> description = groupToBeAdded.split(L" ");
									if (description.size() < 3 || description.size() > 4)
									{
										throw ComponentGenerationException(L"malformed addGroup tag");
									}
									std::wstring smiles = description[0];
									AtomReferenceType referenceType = static_cast<AtomReferenceType>(description[1].toUpperCase(Locale::ROOT));
									std::wstring reference = description[2];
									Fragment *fragToAdd;
									if (description.size() == 4)
									{ //labels may optionally be specified for the group to be added
										fragToAdd = state->fragManager->buildSMILES(smiles, group, description[3]);
									}
									else
									{
										fragToAdd = state->fragManager->buildSMILES(smiles, group, XmlDeclarations::NONE_LABELS_VAL);
									}
									AddGroup tempVar(fragToAdd, new AtomReference(referenceType, reference));
									groupsToBeAdded.push_back(&tempVar);
								}
								Element *previousEl = OpsinTools::getPreviousSibling(group);
								if (previousEl != nullptr && previousEl->getName() == XmlDeclarations::LOCANT_EL)
								{ //has the name got specified locants to override the default ones
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
									std::vector<std::wstring> locantValues = StringTools::arrayToList(previousEl->getValue().split(L","));
									if ((locantValues.size() == groupsToBeAdded.size() || locantValues.size() + 1 == groupsToBeAdded.size()) && locantAreAcceptableForXyleneLikeNomenclatures(locantValues, group))
									{ //one locant can be implicit in some cases
										bool assignlocants = true;
										if (locantValues.size() != groupsToBeAdded.size())
										{
											//check that the firstGroup by default will be added to the atom with locant 1. If this is not the case then as many locants as there were groups should of been specified
											//or no locants should have been specified, which is what will be assumed (i.e. the locants will be left unassigned)
											AddGroup *groupInformation = groupsToBeAdded[0];
											std::wstring locant;
											switch (groupInformation->atomReference.referenceType)
											{
											case DEFAULTLOCANT:
											case LOCANT:
												locant = parentFrag->getAtomByLocantOrThrow(groupInformation->atomReference.reference)->getFirstLocant();
												break;
											case DEFAULTID:
											case ID:
												locant = parentFrag->getAtomByIDOrThrow(parentFrag->getIdOfFirstAtom() + static_cast<Integer>(groupInformation->atomReference.reference) - 1)->getFirstLocant();
												break;
											default:
												throw ComponentGenerationException(L"malformed addGroup tag");
											}
											if (locant == L"" || locant != L"1")
											{
												assignlocants = false;
											}
										}
										if (assignlocants)
										{
											for (int i = groupsToBeAdded.size() - 1; i >= 0 ; i--)
											{
												//if less locants than expected are specified the locants of only the later groups will be changed
												//e.g. 4-xylene will transform 1,2-xylene to 1,4-xylene
												AddGroup *groupInformation = groupsToBeAdded[i];
												if (locantValues.size() > 0)
												{
													groupInformation->atomReference = new AtomReference(AtomReferenceType::LOCANT, locantValues[locantValues.size() - 1]);
													locantValues.pop_back();
												}
												else
												{
													break;
												}
											}
											group->removeAttribute(group->getAttribute(XmlDeclarations::FRONTLOCANTSEXPECTED_ATR));
											previousEl->detach();
										}
									}
								}

								for (int i = 0; i < groupsToBeAdded.size(); i++)
								{
									AddGroup *groupInformation = groupsToBeAdded[i];
									Fragment *newFrag = groupInformation->frag;

									Atom *atomOnParentFrag;
									switch (groupInformation->atomReference.referenceType)
									{
									case DEFAULTLOCANT:
										ambiguous = true;
									case LOCANT:
										if (groupInformation->atomReference.reference.equals(L"required"))
										{
											throw ComponentGenerationException(group->getValue() + std::wstring(L" requires an allowed locant"));
										}
										atomOnParentFrag = parentFrag->getAtomByLocantOrThrow(groupInformation->atomReference.reference);
										break;
									case DEFAULTID:
										ambiguous = true;
									case ID:
										atomOnParentFrag = parentFrag->getAtomByIDOrThrow(parentFrag->getIdOfFirstAtom() + static_cast<Integer>(groupInformation->atomReference.reference) - 1);
										break;
									default:
										throw ComponentGenerationException(L"malformed addGroup tag");
									}

									if (newFrag->getOutAtomCount() > 1)
									{
										throw ComponentGenerationException(L"too many outAtoms on group to be added");
									}
									if (newFrag->getOutAtomCount() == 1)
									{
										OutAtom *newFragOutAtom = newFrag->getOutAtom(0);
										newFrag->removeOutAtom(newFragOutAtom);
										state->fragManager->incorporateFragment(newFrag, newFragOutAtom->getAtom(), parentFrag, atomOnParentFrag, newFragOutAtom->getValency());
									}
									else
									{
										Atom *atomOnNewFrag = newFrag->getDefaultInAtomOrFirstAtom();
										state->fragManager->incorporateFragment(newFrag, atomOnNewFrag, parentFrag, atomOnParentFrag, 1);
									}
								}
							}

							if (group->getAttributeValue(XmlDeclarations::ADDHETEROATOM_ATR) != L"")
							{
								std::wstring addHeteroAtomInformation = group->getAttributeValue(XmlDeclarations::ADDHETEROATOM_ATR);
								std::vector<AddHeteroatom*> heteroAtomsToBeAdded;
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
								for (std::wstring heteroAtomToBeAdded : addHeteroAtomInformation.split(L";"))
								{
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
									std::vector<std::wstring> description = heteroAtomToBeAdded.split(L" ");
									if (description.size() != 3)
									{
										throw ComponentGenerationException(L"malformed addHeteroAtom tag");
									}
									std::wstring heteroAtomSmiles = description[0];
									AtomReferenceType referenceType = static_cast<AtomReferenceType>(description[1].toUpperCase(Locale::ROOT));
									std::wstring reference = description[2];
									AddHeteroatom tempVar2(heteroAtomSmiles, new AtomReference(referenceType, reference));
									heteroAtomsToBeAdded.push_back(&tempVar2);
								}
								Element *previousEl = OpsinTools::getPreviousSibling(group);
								if (previousEl != nullptr && previousEl->getName() == XmlDeclarations::LOCANT_EL)
								{ //has the name got specified locants to override the default ones
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
									std::vector<std::wstring> locantValues = StringTools::arrayToList(previousEl->getValue().split(L","));
									if (locantValues.size() == heteroAtomsToBeAdded.size() && locantAreAcceptableForXyleneLikeNomenclatures(locantValues, group))
									{
										for (int i = heteroAtomsToBeAdded.size() - 1; i >= 0 ; i--)
										{ //all heteroatoms must have a locant or default locants will be used
											AddHeteroatom *groupInformation = heteroAtomsToBeAdded[i];
											groupInformation->atomReference = new AtomReference(AtomReferenceType::LOCANT, locantValues[locantValues.size() - 1]);
											locantValues.pop_back();
										}
										group->removeAttribute(group->getAttribute(XmlDeclarations::FRONTLOCANTSEXPECTED_ATR));
										previousEl->detach();
									}
								}

								for (int i = 0; i < heteroAtomsToBeAdded.size(); i++)
								{
									AddHeteroatom *heteroAtomInformation = heteroAtomsToBeAdded[i];
									Atom *atomOnParentFrag = nullptr;
									switch (heteroAtomInformation->atomReference.referenceType)
									{
									case DEFAULTLOCANT:
										ambiguous = true;
									case LOCANT:
										if (heteroAtomInformation->atomReference.reference.equals(L"required"))
										{
											throw ComponentGenerationException(group->getValue() + std::wstring(L" requires an allowed locant"));
										}
										atomOnParentFrag = parentFrag->getAtomByLocantOrThrow(heteroAtomInformation->atomReference.reference);
										break;
									case DEFAULTID:
										ambiguous = true;
									case ID:
										atomOnParentFrag = parentFrag->getAtomByIDOrThrow(parentFrag->getIdOfFirstAtom() + static_cast<Integer>(heteroAtomInformation->atomReference.reference) - 1);
										break;
									default:
										throw ComponentGenerationException(L"malformed addHeteroAtom tag");
									}
									state->fragManager->replaceAtomWithSmiles(atomOnParentFrag, heteroAtomInformation->heteroAtomSmiles);
								}
							}

							if (group->getAttributeValue(XmlDeclarations::ADDBOND_ATR) != L"" && XmlDeclarations::HANTZSCHWIDMAN_SUBTYPE_VAL != group->getAttributeValue(XmlDeclarations::SUBTYPE_ATR))
							{ //HW add bond is handled later
								std::wstring addBondInformation = group->getAttributeValue(XmlDeclarations::ADDBOND_ATR);
								std::vector<AddBond*> bondsToBeAdded;
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
								for (std::wstring bondToBeAdded : addBondInformation.split(L";"))
								{
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
									std::vector<std::wstring> description = bondToBeAdded.split(L" ");
									if (description.size() != 3)
									{
										throw ComponentGenerationException(L"malformed addBond tag");
									}
									int bondOrder = std::stoi(description[0]);
									AtomReferenceType referenceType = static_cast<AtomReferenceType>(description[1].toUpperCase(Locale::ROOT));
									std::wstring reference = description[2];
									AddBond tempVar3(bondOrder, new AtomReference(referenceType, reference));
									bondsToBeAdded.push_back(&tempVar3);
								}
								bool locanted = false;
								Element *previousEl = OpsinTools::getPreviousSibling(group);
								if (previousEl != nullptr && previousEl->getName() == XmlDeclarations::LOCANT_EL)
								{ //has the name got specified locants to override the default ones
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
									std::vector<std::wstring> locantValues = StringTools::arrayToList(previousEl->getValue().split(L","));
									if (locantValues.size() == bondsToBeAdded.size() && locantAreAcceptableForXyleneLikeNomenclatures(locantValues, group))
									{
										for (int i = bondsToBeAdded.size() - 1; i >= 0 ; i--)
										{ //all bond order changes must have a locant or default locants will be used
											AddBond *bondInformation = bondsToBeAdded[i];
											bondInformation->atomReference = new AtomReference(AtomReferenceType::LOCANT, locantValues[locantValues.size() - 1]);
											locantValues.pop_back();
										}
										group->removeAttribute(group->getAttribute(XmlDeclarations::FRONTLOCANTSEXPECTED_ATR));
										previousEl->detach();
										locanted = true;
									}
								}

								for (int i = 0; i < bondsToBeAdded.size(); i++)
								{
									AddBond *bondInformation = bondsToBeAdded[i];
									Atom *atomOnParentFrag;
									switch (bondInformation->atomReference.referenceType)
									{
									case DEFAULTLOCANT:
										ambiguous = true;
									case LOCANT:
										if (bondInformation->atomReference.reference.equals(L"required"))
										{
											throw ComponentGenerationException(group->getValue() + std::wstring(L" requires an allowed locant"));
										}
										atomOnParentFrag = parentFrag->getAtomByLocantOrThrow(bondInformation->atomReference.reference);
										break;
									case DEFAULTID:
										ambiguous = true;
									case ID:
										atomOnParentFrag = parentFrag->getAtomByIDOrThrow(parentFrag->getIdOfFirstAtom() + static_cast<Integer>(bondInformation->atomReference.reference) - 1);
										break;
									default:
										throw ComponentGenerationException(L"malformed addBond tag");
									}

									Bond *b = FragmentTools::unsaturate(atomOnParentFrag, bondInformation->bondOrder, parentFrag);
									if (!locanted && b->getOrder() == 2 && parentFrag->getAtomCount() == 5 && b->getFromAtom()->getAtomIsInACycle() && b->getToAtom()->getAtomIsInACycle())
									{
										//special case just that substitution of groups like imidazoline may actually remove the double bond...
										b->setOrder(1);
										b->getFromAtom()->setSpareValency(true);
										b->getToAtom()->setSpareValency(true);
									}
								}
							}
							if (ambiguous)
							{
								state->addIsAmbiguous(group->getValue() + std::wstring(L" describes multiple structures"));
							}
						}

						bool ComponentProcessor::locantAreAcceptableForXyleneLikeNomenclatures(std::vector<std::wstring> &locantValues, Element *group)
						{
							if (group->getAttribute(XmlDeclarations::FRONTLOCANTSEXPECTED_ATR) == nullptr)
							{
								throw std::invalid_argument("Group must have frontLocantsExpected to implement xylene-like nomenclature");
							}
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
							std::vector<std::wstring> allowedLocants = Arrays::asList(group->getAttributeValue(XmlDeclarations::FRONTLOCANTSEXPECTED_ATR).split(L","));
							for (auto locant : locantValues)
							{
								if (!std::find(allowedLocants.begin(), allowedLocants.end(), locant) != allowedLocants.end())
								{
									return false;
								}
							}
							return true;
						}

						void ComponentProcessor::setFragmentDefaultInAtomIfSpecified(Fragment *thisFrag, Element *group) throw(StructureBuildingException)
						{
							std::wstring defaultInLocant = group->getAttributeValue(XmlDeclarations::DEFAULTINLOCANT_ATR);
							std::wstring defaultInId = group->getAttributeValue(XmlDeclarations::DEFAULTINID_ATR);
							if (defaultInLocant != L"")
							{ //sets the atom at which substitution will occur to by default
								thisFrag->setDefaultInAtom(thisFrag->getAtomByLocantOrThrow(defaultInLocant));
							}
							else if (defaultInId != L"")
							{
								thisFrag->setDefaultInAtom(thisFrag->getAtomByIDOrThrow(thisFrag->getIdOfFirstAtom() + std::stoi(defaultInId) - 1));
							}
						}

						void ComponentProcessor::setFragmentFunctionalAtomsIfSpecified(Element *group, Fragment *thisFrag) throw(StructureBuildingException)
						{
							if (group->getAttribute(XmlDeclarations::FUNCTIONALIDS_ATR) != nullptr)
							{
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
								std::vector<std::wstring> functionalIDs = group->getAttributeValue(XmlDeclarations::FUNCTIONALIDS_ATR).split(L",");
								for (auto functionalID : functionalIDs)
								{
									thisFrag->addFunctionalAtom(thisFrag->getAtomByIDOrThrow(thisFrag->getIdOfFirstAtom() + std::stoi(functionalID) - 1));
								}
							}
						}

						void ComponentProcessor::applyTraditionalAlkaneNumberingIfAppropriate(Element *group, Fragment *thisFrag)
						{
							std::wstring groupType = group->getAttributeValue(XmlDeclarations::TYPE_ATR);
							if (groupType == XmlDeclarations::ACIDSTEM_TYPE_VAL)
							{
								std::vector<Atom*> atomList = thisFrag->getAtomList();
								Atom *startingAtom = thisFrag->getFirstAtom();
								if (group->getAttribute(XmlDeclarations::SUFFIXAPPLIESTO_ATR) != nullptr)
								{
									std::wstring suffixAppliesTo = group->getAttributeValue(XmlDeclarations::SUFFIXAPPLIESTO_ATR);
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
									std::vector<std::wstring> suffixAppliesToArr = suffixAppliesTo.split(L",");
									if (suffixAppliesToArr.size() != 1)
									{
										return;
									}
									startingAtom = atomList[std::stoi(suffixAppliesToArr[0]) - 1];
								}
								std::vector<Atom*> neighbours = startingAtom->getAtomNeighbours();
								int counter = -1;
								Atom *previousAtom = startingAtom;
								for (int i = neighbours.size() - 1; i >= 0; i--)
								{ //only consider carbon atoms
									if (neighbours[i]->getElement() != ChemEl::C)
									{
										neighbours.erase(neighbours.begin() + i);
									}
								}
								while (neighbours.size() == 1)
								{
									counter++;
									if (counter > 5)
									{
										break;
									}
									Atom *nextAtom = neighbours[0];
									if (nextAtom->getAtomIsInACycle())
									{
										break;
									}
									std::wstring traditionalLocant = traditionalAlkanePositionNames[counter];
									if (!nextAtom->hasLocant(traditionalLocant))
									{
										nextAtom->addLocant(traditionalLocant);
									}
									neighbours = nextAtom->getAtomNeighbours();
//JAVA TO C++ CONVERTER TODO TASK: The Java Collection 'remove(Object)' method is not converted:
									neighbours.remove(previousAtom);
									for (int i = neighbours.size() - 1; i >= 0; i--)
									{ //only consider carbon atoms
										if (neighbours[i]->getElement() != ChemEl::C)
										{
											neighbours.erase(neighbours.begin() + i);
										}
									}
									previousAtom = nextAtom;
								}
							}
							else if (groupType == XmlDeclarations::CHAIN_TYPE_VAL && XmlDeclarations::ALKANESTEM_SUBTYPE_VAL == group->getAttributeValue(XmlDeclarations::SUBTYPE_ATR))
							{
								std::vector<Atom*> atomList = thisFrag->getAtomList();
								if (atomList.size() == 1)
								{
									return;
								}
								Element *possibleSuffix = OpsinTools::getNextSibling(group, XmlDeclarations::SUFFIX_EL);
								boost::optional<bool> terminalSuffixWithNoSuffixPrefixPresent = false;
								if (possibleSuffix != nullptr && XmlDeclarations::TERMINAL_SUBTYPE_VAL == possibleSuffix->getAttributeValue(XmlDeclarations::SUBTYPE_ATR) && possibleSuffix->getAttribute(XmlDeclarations::SUFFIXPREFIX_ATR) == nullptr)
								{
									terminalSuffixWithNoSuffixPrefixPresent = true;
								}
								for (auto atom : atomList)
								{
									std::wstring firstLocant = atom->getFirstLocant();
									if (!atom->getAtomIsInACycle() && firstLocant != L"" && firstLocant.length() == 1 && std::isdigit(firstLocant[0]))
									{
										int locantNumber = std::stoi(firstLocant);
										if (terminalSuffixWithNoSuffixPrefixPresent)
										{
											if (locantNumber > 1 && locantNumber <= 7)
											{
												atom->addLocant(traditionalAlkanePositionNames[locantNumber - 2]);
											}
										}
										else
										{
											if (locantNumber > 0 && locantNumber <= 6)
											{
												atom->addLocant(traditionalAlkanePositionNames[locantNumber - 1]);
											}
										}
									}
								}
							}
						}

						void ComponentProcessor::applyHomologyGroupLabelsIfSpecified(Element *group, Fragment *frag)
						{
							std::wstring homologyValsStr = group->getAttributeValue(XmlDeclarations::HOMOLOGY_ATR);
							if (homologyValsStr != L"")
							{
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
								std::vector<std::wstring> vals = homologyValsStr.split(L";");

								std::vector<Atom*> homologyAtoms;
								for (auto a : frag->getAtomList())
								{
									if (a->getElement() == ChemEl::R)
									{
										homologyAtoms.push_back(a);
									}
								}
								int count = vals.size();
								if (count != homologyAtoms.size())
								{
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("OPSIN Bug: Number of homology atoms should match number of homology labels! for: " + group.getValue());
									throw std::exception();
								}
								for (int i = 0; i < count; i++)
								{
									homologyAtoms[i]->setProperty(Atom::HOMOLOGY_GROUP, vals[i]);
								}
							}
						}

						void ComponentProcessor::processChargeAndOxidationNumberSpecification(Element *group, Fragment *frag)
						{
							Element *nextEl = OpsinTools::getNextSibling(group);
							if (nextEl != nullptr)
							{
								if (nextEl->getName() == XmlDeclarations::CHARGESPECIFIER_EL)
								{
									frag->getFirstAtom()->setCharge(std::stoi(nextEl->getAttributeValue(XmlDeclarations::VALUE_ATR)));
									nextEl->detach();
								}
								if (nextEl->getName() == XmlDeclarations::OXIDATIONNUMBERSPECIFIER_EL)
								{
									frag->getFirstAtom()->setProperty(Atom::OXIDATION_NUMBER, std::stoi(nextEl->getAttributeValue(XmlDeclarations::VALUE_ATR)));
									nextEl->detach();
								}
							}
						}

						bool ComponentProcessor::removeAndMoveToAppropriateGroupIfHydroSubstituent(Element *substituent) throw(ComponentGenerationException)
						{
							std::vector<Element*> hydroElements = substituent->getChildElements(XmlDeclarations::HYDRO_EL);
							if (hydroElements.size() > 0)
							{
								Element *targetRing = nullptr;
								Element * const adjacentSubOrRootOrBracket = OpsinTools::getNextSibling(substituent);
								if (adjacentSubOrRootOrBracket == nullptr)
								{
									throw ComponentGenerationException(L"Cannot find ring for hydro substituent to apply to");
								}
								//first check adjacent substituent/root. If the hydro element has one locant or the ring is locantless then we can assume the hydro is acting as a nondetachable prefix
								Element *potentialRing = adjacentSubOrRootOrBracket->getFirstChildElement(XmlDeclarations::GROUP_EL);
								if (potentialRing != nullptr && containsCyclicAtoms(potentialRing))
								{
									Element *possibleLocantInFrontOfHydro = OpsinTools::getPreviousSibling(hydroElements[0]);
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
									if (possibleLocantInFrontOfHydro != nullptr && possibleLocantInFrontOfHydro->getName() == XmlDeclarations::LOCANT_EL && possibleLocantInFrontOfHydro->getValue().split(L",")->length == 1)
									{
										//e.g.4-decahydro-1-naphthalenyl
										targetRing = potentialRing;
									}
									else
									{
										Element *possibleLocantInFrontOfRing = OpsinTools::getPreviousSibling(potentialRing, XmlDeclarations::LOCANT_EL);
										if (possibleLocantInFrontOfRing != nullptr)
										{
											if (potentialRing->getAttribute(XmlDeclarations::FRONTLOCANTSEXPECTED_ATR) != nullptr)
											{ //check whether the group was expecting a locant e.g. 2-furyl
												std::wstring locantValue = possibleLocantInFrontOfRing->getValue();
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
												std::vector<std::wstring> expectedLocants = potentialRing->getAttributeValue(XmlDeclarations::FRONTLOCANTSEXPECTED_ATR).split(L",");
												for (auto expectedLocant : expectedLocants)
												{
													if (locantValue == expectedLocant)
													{
														targetRing = potentialRing;
														break;
													}
												}
											}
											//check whether the group is a benzofused ring e.g. 1,4-benzodioxin
											if (XmlDeclarations::FUSIONRING_SUBTYPE_VAL == potentialRing->getAttributeValue(XmlDeclarations::SUBTYPE_ATR) && (potentialRing->getValue() == L"benzo" || potentialRing->getValue() == L"benz") && OpsinTools::getNextSibling(potentialRing)->getName() != XmlDeclarations::FUSION_EL)
											{
												targetRing = potentialRing;
											}
										}
										else
										{
											targetRing = potentialRing;
										}
									}
								}

								//that didn't match so the hydro appears to be a detachable prefix. detachable prefixes attach in preference to the rightmost applicable group so search any remaining substituents/roots from right to left
								if (targetRing == nullptr)
								{
									Element *nextSubOrRootOrBracketFromLast = substituent->getParent()->getChild(substituent->getParent()->getChildCount() - 1); //the last sibling
									while (!nextSubOrRootOrBracketFromLast->equals(substituent))
									{
										potentialRing = nextSubOrRootOrBracketFromLast->getFirstChildElement(XmlDeclarations::GROUP_EL);
										if (potentialRing != nullptr && containsCyclicAtoms(potentialRing))
										{
											targetRing = potentialRing;
											break;
										}
										else
										{
											nextSubOrRootOrBracketFromLast = OpsinTools::getPreviousSibling(nextSubOrRootOrBracketFromLast);
										}
									}
								}
								if (targetRing == nullptr)
								{
									throw ComponentGenerationException(L"Cannot find ring for hydro substituent to apply to");
								}
								//move the children of the hydro substituent
								std::vector<Element*> children = substituent->getChildElements();
								Element *targetSubstituent = targetRing->getParent();
								if (targetSubstituent->equals(adjacentSubOrRootOrBracket))
								{
									for (int i = children.size() - 1; i >= 0 ; i--)
									{
										Element *child = children[i];
										if (child->getName() == XmlDeclarations::HYPHEN_EL)
										{
											continue;
										}
										child->detach();
										targetSubstituent->insertChild(child, 0);
									}
								}
								else
								{
									bool inDetachablePrefix = true;
									for (int i = children.size() - 1; i >= 0 ; i--)
									{
										Element *child = children[i];
										std::wstring elName = child->getName();
										if (elName == XmlDeclarations::HYPHEN_EL)
										{
											continue;
										}
										else if (inDetachablePrefix && elName == XmlDeclarations::HYDRO_EL)
										{
											child->detach();
											targetSubstituent->insertChild(child, 0);
										}
										else if (elName == XmlDeclarations::STEREOCHEMISTRY_EL)
										{
											inDetachablePrefix = false;
											child->detach();
											adjacentSubOrRootOrBracket->insertChild(child, 0);
										}
										else
										{
											throw ComponentGenerationException(std::wstring(L"Unexpected term found before detachable hydro prefix: ") + child->getValue());
										}
									}
								}
								substituent->detach();
								return true;
							}
							return false;
						}

						bool ComponentProcessor::removeAndMoveToAppropriateGroupIfSubtractivePrefix(Element *substituent) throw(ComponentGenerationException)
						{
							std::vector<Element*> subtractivePrefixes = substituent->getChildElements(XmlDeclarations::SUBTRACTIVEPREFIX_EL);
							if (subtractivePrefixes.size() > 0)
							{
								Element *biochemicalGroup = nullptr; //preferred
								Element *standardGroup = nullptr;
								Element * const adjacentSubOrRootOrBracket = OpsinTools::getNextSibling(substituent);
								Element *nextSubOrRootOrBracket = adjacentSubOrRootOrBracket;
								if (nextSubOrRootOrBracket == nullptr)
								{
									throw ComponentGenerationException(std::wstring(L"Unable to find group for: ") + subtractivePrefixes[0]->getValue() + std::wstring(L" to apply to!"));
								}
								//prefer the nearest (unlocanted) biochemical group or the rightmost standard group
								while (nextSubOrRootOrBracket != nullptr)
								{
									Element *groupToConsider = nextSubOrRootOrBracket->getFirstChildElement(XmlDeclarations::GROUP_EL);
									if (groupToConsider != nullptr)
									{
										std::wstring type = groupToConsider->getAttributeValue(XmlDeclarations::TYPE_ATR);
										std::wstring subType = groupToConsider->getAttributeValue(XmlDeclarations::SUBTYPE_ATR);
										if (XmlDeclarations::BIOCHEMICAL_SUBTYPE_VAL == subType || XmlDeclarations::CARBOHYDRATE_TYPE_VAL == type || XmlDeclarations::AMINOACID_TYPE_VAL == type)
										{
											biochemicalGroup = groupToConsider;
											if (OpsinTools::getPreviousSiblingsOfType(biochemicalGroup, XmlDeclarations::LOCANT_EL).empty())
											{
												break;
											}
										}
										else
										{
											standardGroup = groupToConsider;
										}
									}
									nextSubOrRootOrBracket = OpsinTools::getNextSibling(nextSubOrRootOrBracket);
								}

								Element *targetGroup = biochemicalGroup != nullptr ? biochemicalGroup : standardGroup;
								if (targetGroup == nullptr)
								{
									throw ComponentGenerationException(std::wstring(L"Unable to find group for: ") + subtractivePrefixes[0]->getValue() + std::wstring(L" to apply to!"));
								}

								//move the children of the subtractivePrefix substituent
								std::vector<Element*> children = substituent->getChildElements();
								Element *targetSubstituent = targetGroup->getParent();
								if (targetSubstituent->equals(adjacentSubOrRootOrBracket))
								{
									for (int i = children.size() - 1; i >= 0 ; i--)
									{
										Element *child = children[i];
										if (child->getName() != XmlDeclarations::HYPHEN_EL)
										{
											child->detach();
											targetSubstituent->insertChild(child, 0);
										}
									}
								}
								else
								{
									bool inDetachablePrefix = true;
									for (int i = children.size() - 1; i >= 0 ; i--)
									{
										Element *child = children[i];
										std::wstring elName = child->getName();
										if (elName == XmlDeclarations::HYPHEN_EL)
										{
											continue;
										}
										else if (inDetachablePrefix && elName == XmlDeclarations::SUBTRACTIVEPREFIX_EL)
										{
											child->detach();
											targetSubstituent->insertChild(child, 0);
										}
										else if (elName == XmlDeclarations::STEREOCHEMISTRY_EL)
										{
											inDetachablePrefix = false;
											child->detach();
											adjacentSubOrRootOrBracket->insertChild(child, 0);
										}
										else
										{
											throw ComponentGenerationException(std::wstring(L"Unexpected term found before detachable substractive prefix: ") + child->getValue());
										}
									}
								}
								substituent->detach();
								return true;
							}
							return false;
						}

						bool ComponentProcessor::removeAndMoveToAppropriateGroupIfRingBridge(Element *substituent) throw(ComponentGenerationException)
						{
							std::vector<Element*> ringBridges = substituent->getChildElements(XmlDeclarations::FUSEDRINGBRIDGE_EL);
							if (ringBridges.size() > 0)
							{
								Element * const adjacentSubOrRootOrBracket = OpsinTools::getNextSibling(substituent);
								Element *nextSubOrRootOrBracket = adjacentSubOrRootOrBracket;
								if (nextSubOrRootOrBracket == nullptr)
								{
									throw ComponentGenerationException(std::wstring(L"Unable to find group for: ") + ringBridges[0]->getValue() + std::wstring(L" to apply to!"));
								}
								Element *targetGroup = nullptr;
								Element *standardGroup = nullptr;
								//prefer the nearest (unlocanted) ring group or the rightmost standard group
								while (nextSubOrRootOrBracket != nullptr)
								{
									Element *groupToConsider = nextSubOrRootOrBracket->getFirstChildElement(XmlDeclarations::GROUP_EL);
									if (groupToConsider != nullptr)
									{
										if (containsCyclicAtoms(groupToConsider) && OpsinTools::getPreviousSiblingsOfType(groupToConsider, XmlDeclarations::LOCANT_EL).empty())
										{
											targetGroup = groupToConsider;
											break;
										}
										else
										{
											standardGroup = groupToConsider;
										}
									}
									nextSubOrRootOrBracket = OpsinTools::getNextSibling(nextSubOrRootOrBracket);
								}
								if (targetGroup == nullptr)
								{
									targetGroup = standardGroup;
								}
								if (targetGroup == nullptr)
								{
									throw ComponentGenerationException(std::wstring(L"Unable to find group for: ") + ringBridges[0]->getValue() + std::wstring(L" to apply to!"));
								}

								//move the children of the fusedRingBridge substituent
								std::vector<Element*> children = substituent->getChildElements();
								Element *targetSubstituent = targetGroup->getParent();
								if (targetSubstituent->equals(adjacentSubOrRootOrBracket))
								{
									for (int i = children.size() - 1; i >= 0 ; i--)
									{
										Element *child = children[i];
										if (child->getName() != XmlDeclarations::HYPHEN_EL)
										{
											child->detach();
											targetSubstituent->insertChild(child, 0);
										}
									}
								}
								else
								{
									bool inDetachablePrefix = true;
									for (int i = children.size() - 1; i >= 0 ; i--)
									{
										Element *child = children[i];
										std::wstring elName = child->getName();
										if (elName == XmlDeclarations::HYPHEN_EL)
										{
											continue;
										}
										else if (inDetachablePrefix && (elName == XmlDeclarations::FUSEDRINGBRIDGE_EL || elName == XmlDeclarations::COLONORSEMICOLONDELIMITEDLOCANT_EL || elName == XmlDeclarations::LOCANT_EL))
										{
											child->detach();
											targetSubstituent->insertChild(child, 0);
										}
										else if (elName == XmlDeclarations::STEREOCHEMISTRY_EL)
										{
											inDetachablePrefix = false;
											child->detach();
											adjacentSubOrRootOrBracket->insertChild(child, 0);
										}
										else
										{
											throw ComponentGenerationException(std::wstring(L"Unexpected term found before detachable ring bridge: ") + child->getValue());
										}
									}
								}
								substituent->detach();
								return true;
							}
							return false;
						}

						bool ComponentProcessor::containsCyclicAtoms(Element *potentialRing)
						{
							Fragment *potentialRingFrag = potentialRing->getFrag();
							std::vector<Atom*> atomList = potentialRingFrag->getAtomList();
							for (auto atom : atomList)
							{
								if (atom->getAtomIsInACycle())
								{
									return true;
								}
							}
							return false;
						}

						void ComponentProcessor::determineLocantMeaning(Element *subOrBracketOrRoot, Element *finalSubOrRootInWord) throw(StructureBuildingException, ComponentGenerationException)
						{
							std::vector<Element*> locants = subOrBracketOrRoot->getChildElements(XmlDeclarations::LOCANT_EL);
							Element *group = subOrBracketOrRoot->getFirstChildElement(XmlDeclarations::GROUP_EL); //will be null if element is a bracket
							for (auto locant : locants)
							{
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
								std::vector<std::wstring> locantValues = locant->getValue().split(L",");
								if (locantValues.size() > 1)
								{
									Element *afterLocant = OpsinTools::getNextSibling(locant);
									int structuralBracketDepth = 0;
									Element *multiplierEl = nullptr;
									while (afterLocant != nullptr)
									{
										std::wstring elName = afterLocant->getName();
										if (elName == XmlDeclarations::STRUCTURALOPENBRACKET_EL)
										{
											structuralBracketDepth++;
										}
										else if (elName == XmlDeclarations::STRUCTURALCLOSEBRACKET_EL)
										{
											structuralBracketDepth--;
										}
										if (structuralBracketDepth != 0)
										{
											afterLocant = OpsinTools::getNextSibling(afterLocant);
											continue;
										}
										if (elName == XmlDeclarations::LOCANT_EL)
										{
											break;
										}
										else if (elName == XmlDeclarations::MULTIPLIER_EL)
										{
											if (locantValues.size() == std::stoi(afterLocant->getAttributeValue(XmlDeclarations::VALUE_ATR)))
											{
												if (afterLocant->equals(OpsinTools::getNextSiblingIgnoringCertainElements(locant, std::vector<std::wstring>{XmlDeclarations::INDICATEDHYDROGEN_EL})))
												{
													//direct locant, typical case. An exception is made for indicated hydrogen e.g. 1,2,4-1H-triazole
													multiplierEl = afterLocant;
													break;
												}
												else
												{
													Element *afterMultiplier = OpsinTools::getNextSibling(afterLocant);
													if (afterMultiplier != nullptr && (afterMultiplier->getName() == XmlDeclarations::SUFFIX_EL || afterMultiplier->getName() == XmlDeclarations::INFIX_EL || afterMultiplier->getName() == XmlDeclarations::UNSATURATOR_EL || afterMultiplier->getName() == XmlDeclarations::GROUP_EL))
													{
														multiplierEl = afterLocant; //indirect locant
														break;
													}
												}
											}
											if (afterLocant->equals(OpsinTools::getNextSibling(locant)))
											{ //if nothing better can be found report this as a locant/multiplier mismatch
												multiplierEl = afterLocant;
											}
										}
										else if (elName == XmlDeclarations::RINGASSEMBLYMULTIPLIER_EL && afterLocant->equals(OpsinTools::getNextSibling(locant)))
										{ //e.g. 1,1'-biphenyl
											multiplierEl = afterLocant;
											if (!FragmentTools::allAtomsInRingAreIdentical(group->getFrag()))
											{ //if all atoms are identical then the locant may refer to suffixes
												break;
											}
										}
										else if (elName == XmlDeclarations::FUSEDRINGBRIDGE_EL && locantValues.size() == 2 && afterLocant->equals(OpsinTools::getNextSibling(locant)))
										{ //e.g. 1,8-methano
											break;
										}
										afterLocant = OpsinTools::getNextSibling(afterLocant);
									}
									if (multiplierEl != nullptr)
									{
										if (std::stoi(multiplierEl->getAttributeValue(XmlDeclarations::VALUE_ATR)) == locantValues.size())
										{
											// number of locants and multiplier agree
											bool locantModified = false; //did determineLocantMeaning do something?
											if (boost::ends_with(locantValues[locantValues.size() - 1], L"'") && group != nullptr && subOrBracketOrRoot->find(group) > subOrBracketOrRoot->find(locant))
											{ //quite possible that this is referring to a multiplied root
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
												if (group->getAttribute(XmlDeclarations::OUTIDS_ATR) != nullptr && group->getAttributeValue(XmlDeclarations::OUTIDS_ATR).split(L",")->length > 1)
												{
													locantModified = checkSpecialLocantUses(locant, locantValues, finalSubOrRootInWord);
												}
												else
												{
													Element *afterGroup = OpsinTools::getNextSibling(group);
													int inlineSuffixCount = 0;
													int multiplier = 1;
													while (afterGroup != nullptr)
													{
														if (afterGroup->getName() == XmlDeclarations::MULTIPLIER_EL)
														{
															multiplier = std::stoi(afterGroup->getAttributeValue(XmlDeclarations::VALUE_ATR));
														}
														else if (afterGroup->getName() == XmlDeclarations::SUFFIX_EL && afterGroup->getAttributeValue(XmlDeclarations::TYPE_ATR) == XmlDeclarations::INLINE_TYPE_VAL)
														{
															inlineSuffixCount += (multiplier);
															multiplier = 1;
														}
														afterGroup = OpsinTools::getNextSibling(afterGroup);
													}
													if (inlineSuffixCount >= 2)
													{
														locantModified = checkSpecialLocantUses(locant, locantValues, finalSubOrRootInWord);
													}
												}
											}
											if (!locantModified && !OpsinTools::getNextSibling(locant)->equals(multiplierEl))
											{ //the locants apply indirectly the multiplier e.g. 2,3-butandiol
												//move the locant to be next to the multiplier.
												locant->detach();
												OpsinTools::insertBefore(multiplierEl, locant);
											}
										}
										else
										{
											if (!checkSpecialLocantUses(locant, locantValues, finalSubOrRootInWord))
											{
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
												throw ComponentGenerationException(std::wstring(L"Mismatch between locant and multiplier counts (") + Integer::toString(locantValues.size()) + std::wstring(L" and ") + multiplierEl->getAttributeValue(XmlDeclarations::VALUE_ATR) + std::wstring(L"):") + locant->getValue());
											}
										}
									}
									else
									{
										/* Multiple locants without a multiplier */
										if (!checkSpecialLocantUses(locant, locantValues, finalSubOrRootInWord))
										{
											throw ComponentGenerationException(std::wstring(L"Multiple locants without a multiplier: ") + locant->toXML());
										}
									}
								}
							}
						}

						bool ComponentProcessor::checkSpecialLocantUses(Element *locant, std::vector<std::wstring> &locantValues, Element *finalSubOrRootInWord) throw(ComponentGenerationException)
						{
							int count = locantValues.size();
							Element *currentElem = OpsinTools::getNextSibling(locant);
							int heteroCount = 0;
							int multiplierValue = 1;
							while (currentElem != nullptr && currentElem->getName() != XmlDeclarations::GROUP_EL)
							{
								if (currentElem->getName() == XmlDeclarations::HETEROATOM_EL)
								{
									heteroCount += multiplierValue;
									multiplierValue = 1;
								}
								else if (currentElem->getName() == XmlDeclarations::MULTIPLIER_EL)
								{
									multiplierValue = std::stoi(currentElem->getAttributeValue(XmlDeclarations::VALUE_ATR));
								}
								else
								{
									break;
								}
								currentElem = OpsinTools::getNextSibling(currentElem);
							}
							if (currentElem != nullptr && currentElem->getName() == XmlDeclarations::GROUP_EL)
							{
								if (currentElem->getAttributeValue(XmlDeclarations::SUBTYPE_ATR) == XmlDeclarations::HANTZSCHWIDMAN_SUBTYPE_VAL)
								{
									if (heteroCount == count)
									{
										return true;
									}
									else if (heteroCount > 1)
									{
										return false; //there is a case where locants don't apply to heteroatoms in a HW system, but in that case only one locant is expected so this function would not be called
									}
								}
								if (heteroCount == 0 && currentElem->getAttribute(XmlDeclarations::OUTIDS_ATR) != nullptr)
								{ //e.g. 1,4-phenylene
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
									std::vector<std::wstring> outIDs = currentElem->getAttributeValue(XmlDeclarations::OUTIDS_ATR).split(L",", -1);
									Fragment *groupFragment = currentElem->getFrag();
									if (count == outIDs.size() && groupFragment->getAtomCount() > 1)
									{ //things like oxy do not need to have their outIDs specified
										int idOfFirstAtomInFrag = groupFragment->getIdOfFirstAtom();
										bool foundLocantNotPresentOnFragment = false;
										for (int i = outIDs.size() - 1; i >= 0; i--)
										{
											Atom *a = groupFragment->getAtomByLocant(locantValues[i]);
											if (a == nullptr)
											{
												foundLocantNotPresentOnFragment = true;
												break;
											}
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
											outIDs[i] = Integer::toString(a->getID() - idOfFirstAtomInFrag + 1); //convert to relative id
										}
										if (!foundLocantNotPresentOnFragment)
										{
											currentElem->getAttribute(XmlDeclarations::OUTIDS_ATR)->setValue(StringTools::arrayToString(outIDs, L","));
											locant->detach();
											return true;
										}
									}
								}
								else if (currentElem->getValue() == L"benz" || currentElem->getValue() == L"benzo")
								{
									Element *potentialGroupAfterBenzo = OpsinTools::getNextSibling(currentElem, XmlDeclarations::GROUP_EL); //need to make sure this isn't benzyl
									if (potentialGroupAfterBenzo != nullptr)
									{
										return true; //e.g. 1,2-benzothiazole
									}
								}
							}
							if (currentElem != nullptr)
							{
								std::wstring name = currentElem->getName();
								if (name == XmlDeclarations::POLYCYCLICSPIRO_EL)
								{
									return true;
								}
								else if (name == XmlDeclarations::FUSEDRINGBRIDGE_EL && count == 2)
								{
									return true;
								}
								else if (name == XmlDeclarations::SUFFIX_EL && XmlDeclarations::CYCLEFORMER_SUBTYPE_VAL == currentElem->getAttributeValue(XmlDeclarations::SUBTYPE_ATR) && count == 2)
								{
									Attribute tempVar(XmlDeclarations::LOCANT_ATR, locant->getValue());
									currentElem->addAttribute(&tempVar);
									locant->detach();
									return true;
								}
								else if (name == XmlDeclarations::SUBTRACTIVEPREFIX_EL && XmlDeclarations::ANHYDRO_TYPE_VAL == currentElem->getAttributeValue(XmlDeclarations::TYPE_ATR))
								{
									if (count != 2)
									{
										throw ComponentGenerationException(std::wstring(L"Two locants are required before an anhydro prefix, but found: ") + locant->getValue());
									}
									Attribute tempVar2(XmlDeclarations::LOCANT_ATR, locant->getValue());
									currentElem->addAttribute(&tempVar2);
									locant->detach();
									return true;
								}
							}
							bool detectedMultiplicativeNomenclature = detectMultiplicativeNomenclature(locant, locantValues, finalSubOrRootInWord);
							if (detectedMultiplicativeNomenclature)
							{
								return true;
							}
							if (currentElem != nullptr && count == 2 && currentElem->getName() == XmlDeclarations::GROUP_EL)
							{
								if (XmlDeclarations::EPOXYLIKE_SUBTYPE_VAL == currentElem->getAttributeValue(XmlDeclarations::SUBTYPE_ATR))
								{
									return true;
								}
								if ((std::wstring(L"yes")) == currentElem->getAttributeValue(XmlDeclarations::IMINOLIKE_ATR))
								{
									currentElem->getAttribute(XmlDeclarations::SUBTYPE_ATR)->setValue(XmlDeclarations::EPOXYLIKE_SUBTYPE_VAL);
									return true;
								}
							}
							Element *parentElem = locant->getParent();
							if (count == 2 && parentElem->getName() == XmlDeclarations::BRACKET_EL)
							{ //e.g. 3,4-(dichloromethylenedioxy) this is changed to (dichloro3,4-methylenedioxy)
								std::vector<Element*> substituents = parentElem->getChildElements(XmlDeclarations::SUBSTITUENT_EL);
								if (substituents.size() > 0)
								{
									Element *finalSub = substituents[substituents.size() - 1];
									Element *group = finalSub->getFirstChildElement(XmlDeclarations::GROUP_EL);
									if (XmlDeclarations::EPOXYLIKE_SUBTYPE_VAL == group->getAttributeValue(XmlDeclarations::SUBTYPE_ATR))
									{
										locant->detach();
										OpsinTools::insertBefore(group, locant);
										return true;
									}
								}
							}

							return false;
						}

						bool ComponentProcessor::detectMultiplicativeNomenclature(Element *locant, std::vector<std::wstring> &locantValues, Element *finalSubOrRootInWord)
						{
							int count = locantValues.size();
							Element *multiplier = finalSubOrRootInWord->getChild(0);
							if (finalSubOrRootInWord->getParent()->getName() == XmlDeclarations::BRACKET_EL)
							{ //e.g. 1,1'-ethynediylbis(1-cyclopentanol)
								if (multiplier->getName() != XmlDeclarations::MULTIPLIER_EL)
								{
									multiplier = finalSubOrRootInWord->getParent()->getChild(0);
								}
								else
								{
									Element *elAfterMultiplier = OpsinTools::getNextSibling(multiplier);
									std::wstring elName = elAfterMultiplier->getName();
									if (elName == XmlDeclarations::HETEROATOM_EL || elName == XmlDeclarations::SUBTRACTIVEPREFIX_EL || (elName == XmlDeclarations::HYDRO_EL && !boost::starts_with(elAfterMultiplier->getValue(), L"per")) || elName == XmlDeclarations::FUSEDRINGBRIDGE_EL)
									{
										multiplier = finalSubOrRootInWord->getParent()->getChild(0);
									}
								}
							}
							Element *commonParent = locant->getParent()->getParent(); //this should be a common parent of the multiplier in front of the root. If it is not, then this locant is in a different scope
							Element *parentOfMultiplier = multiplier->getParent();
							while (parentOfMultiplier != nullptr)
							{
								if (commonParent->equals(parentOfMultiplier))
								{
									if (boost::ends_with(locantValues[count - 1], L"'") && multiplier->getName() == XmlDeclarations::MULTIPLIER_EL && OpsinTools::getNextSibling(multiplier)->getName() != XmlDeclarations::MULTIPLICATIVELOCANT_EL && std::stoi(multiplier->getAttributeValue(XmlDeclarations::VALUE_ATR)) == count)
									{ //multiplicative nomenclature
										locant->setName(XmlDeclarations::MULTIPLICATIVELOCANT_EL);
										locant->detach();
										OpsinTools::insertAfter(multiplier, locant);
										return true;
									}
								}
								parentOfMultiplier = parentOfMultiplier->getParent();
							}
							return false;
						}

						void ComponentProcessor::applyDLPrefixes(Element *subOrRoot) throw(ComponentGenerationException)
						{
							std::vector<Element*> dlStereochemistryEls = OpsinTools::getChildElementsWithTagNameAndAttribute(subOrRoot, XmlDeclarations::STEREOCHEMISTRY_EL, XmlDeclarations::TYPE_ATR, XmlDeclarations::DLSTEREOCHEMISTRY_TYPE_VAL);
							for (auto dlStereochemistry : dlStereochemistryEls)
							{
								std::wstring dlStereochemistryValue = dlStereochemistry->getAttributeValue(XmlDeclarations::VALUE_ATR);
								Element *elementToApplyTo = OpsinTools::getNextSibling(dlStereochemistry);
								if (elementToApplyTo == nullptr)
								{
									continue;
								}
								std::wstring type = elementToApplyTo->getAttributeValue(XmlDeclarations::TYPE_ATR);
								if (XmlDeclarations::OPTICALROTATION_TYPE_VAL == type)
								{
									elementToApplyTo = OpsinTools::getNextSibling(elementToApplyTo);
									if (elementToApplyTo == nullptr)
									{
										continue;
									}
									type = elementToApplyTo->getAttributeValue(XmlDeclarations::TYPE_ATR);
								}
								if (XmlDeclarations::AMINOACID_TYPE_VAL == type)
								{
									if (!applyDlStereochemistryToAminoAcid(elementToApplyTo, dlStereochemistryValue))
									{
										continue;
									}
								}
								else if (XmlDeclarations::CARBOHYDRATE_TYPE_VAL == type)
								{
									applyDlStereochemistryToCarbohydrate(elementToApplyTo, dlStereochemistryValue);
								}
								else if (XmlDeclarations::CARBOHYDRATECONFIGURATIONPREFIX_TYPE_VAL == type)
								{
									applyDlStereochemistryToCarbohydrateConfigurationalPrefix(elementToApplyTo, dlStereochemistryValue);
								}
								else
								{
									continue;
								}
								dlStereochemistry->detach();
							}
						}

						bool ComponentProcessor::applyDlStereochemistryToAminoAcid(Element *aminoAcidEl, const std::wstring &dlStereochemistryValue) throw(ComponentGenerationException)
						{
							Fragment *aminoAcid = aminoAcidEl->getFrag();
							std::vector<Atom*> atomList = aminoAcid->getAtomList();
							std::vector<Atom*> atomsWithParities;
							for (auto atom : atomList)
							{
								if (atom->getAtomParity() != nullptr)
								{
									atomsWithParities.push_back(atom);
								}
							}
							if (atomsWithParities.empty())
							{
								//achiral amino acid... but may become chiral after substitution
								return false;
							}
							if (dlStereochemistryValue == L"dl")
							{
								for (auto atom : atomsWithParities)
								{
									atom->setAtomParity(nullptr);
								}
							}
							else
							{
								bool invert;
								if (dlStereochemistryValue == L"l" || dlStereochemistryValue == L"ls")
								{
									invert = false;
								}
								else if (dlStereochemistryValue == L"d" || dlStereochemistryValue == L"ds")
								{
									invert = true;
								}
								else
								{
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("OPSIN bug: Unexpected value for D/L stereochemistry found before amino acid: " + dlStereochemistryValue);
									throw std::exception();
								}
								if ((std::wstring(L"yes")) == aminoAcidEl->getAttributeValue(XmlDeclarations::NATURALENTISOPPOSITE_ATR))
								{
									invert = !invert;
								}

								if (invert)
								{
									for (auto atom : atomsWithParities)
									{
										atom->getAtomParity()->setParity(-atom->getAtomParity()->getParity());
									}
								}
							}
							return true;
						}

						void ComponentProcessor::applyDlStereochemistryToCarbohydrate(Element *carbohydrateEl, const std::wstring &dlStereochemistryValue) throw(ComponentGenerationException)
						{
							Fragment *carbohydrate = carbohydrateEl->getFrag();
							std::vector<Atom*> atomList = carbohydrate->getAtomList();
							std::vector<Atom*> atomsWithParities;
							for (auto atom : atomList)
							{
								if (atom->getAtomParity() != nullptr)
								{
									atomsWithParities.push_back(atom);
								}
							}
							if (atomsWithParities.empty())
							{
								throw ComponentGenerationException(std::wstring(L"D/L stereochemistry :") + dlStereochemistryValue + std::wstring(L" found before achiral carbohydrate")); //sounds like a vocab bug...
							}

							if (dlStereochemistryValue == L"dl")
							{
								for (auto atom : atomsWithParities)
								{
									atom->setAtomParity(nullptr);
								}
							}
							else
							{
								bool invert;
								if (dlStereochemistryValue == L"d" || dlStereochemistryValue == L"dg")
								{
									invert = false;
								}
								else if (dlStereochemistryValue == L"l" || dlStereochemistryValue == L"lg")
								{
									invert = true;
								}
								else
								{
									throw ComponentGenerationException(std::wstring(L"Unexpected value for D/L stereochemistry found before carbohydrate: ") + dlStereochemistryValue);
								}
								if ((std::wstring(L"yes")) == carbohydrateEl->getAttributeValue(XmlDeclarations::NATURALENTISOPPOSITE_ATR))
								{
									invert = !invert;
								}

								if (invert)
								{
									for (auto atom : atomsWithParities)
									{
										atom->getAtomParity()->setParity(-atom->getAtomParity()->getParity());
									}
								}
							}
						}

						void ComponentProcessor::applyDlStereochemistryToCarbohydrateConfigurationalPrefix(Element *elementToApplyTo, const std::wstring &dlStereochemistryValue) throw(ComponentGenerationException)
						{
							if (dlStereochemistryValue == L"d" || dlStereochemistryValue == L"dg")
							{
								//do nothing, D- is implicit
							}
							else if (dlStereochemistryValue == L"l" || dlStereochemistryValue == L"lg")
							{
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
								std::vector<std::wstring> values = elementToApplyTo->getAttributeValue(XmlDeclarations::VALUE_ATR).split(L"/", -1);
								StringBuilder *sb = new StringBuilder();
								for (auto value : values)
								{
									if (value.equals(L"r"))
									{
										sb->append(L"l");
									}
									else if (value.equals(L"l"))
									{
										sb->append(L"r");
									}
									else
									{
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("OPSIN Bug: Invalid carbohydrate prefix value: " + elementToApplyTo.getAttributeValue(VALUE_ATR));
										throw std::exception();
									}
									sb->append(L"/");
								}
								std::wstring newVal = sb->toString()->substr(0, sb->length() - 1);
								elementToApplyTo->getAttribute(XmlDeclarations::VALUE_ATR)->setValue(newVal);
							}
							else if (dlStereochemistryValue == L"dl")
							{
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
								std::vector<std::wstring> values = elementToApplyTo->getAttributeValue(XmlDeclarations::VALUE_ATR).split(L"/");
								std::wstring newVal = std::wstring(L"?") + StringTools::multiplyString(L"/?", values.size() - 1);
								elementToApplyTo->getAttribute(XmlDeclarations::VALUE_ATR)->setValue(newVal);
							}
							else
							{
								throw ComponentGenerationException(std::wstring(L"Unexpected value for D/L stereochemistry found before carbohydrate prefix: ") + dlStereochemistryValue);
							}
						}

						void ComponentProcessor::processCarbohydrates(Element *subOrRoot) throw(StructureBuildingException)
						{
							std::vector<Element*> carbohydrates = OpsinTools::getChildElementsWithTagNameAndAttribute(subOrRoot, XmlDeclarations::GROUP_EL, XmlDeclarations::TYPE_ATR, XmlDeclarations::CARBOHYDRATE_TYPE_VAL);
							for (auto carbohydrate : carbohydrates)
							{
								Fragment *carbohydrateFrag = carbohydrate->getFrag();
								std::wstring subtype = carbohydrate->getAttributeValue(XmlDeclarations::SUBTYPE_ATR);
								bool isAldose;
								if (XmlDeclarations::CARBOHYDRATESTEMKETOSE_SUBTYPE_VAL == subtype)
								{
									isAldose = false;
								}
								else if (XmlDeclarations::CARBOHYDRATESTEMALDOSE_SUBTYPE_VAL == subtype || XmlDeclarations::SYSTEMATICCARBOHYDRATESTEMALDOSE_SUBTYPE_VAL == subtype)
								{
									isAldose = true;
								}
								else
								{
									Attribute *anomericId = carbohydrate->getAttribute(XmlDeclarations::SUFFIXAPPLIESTO_ATR);
									if (anomericId != nullptr)
									{
										Atom *anomericCarbon = carbohydrateFrag->getAtomByID(carbohydrateFrag->getIdOfFirstAtom() + std::stoi(anomericId->getValue()) - 1);
										applyAlphaBetaStereoToCyclisedCarbohydrate(carbohydrate, anomericCarbon);
										carbohydrate->removeAttribute(anomericId);
									}
									//trivial carbohydrates don't have suffixes
									continue;
								}
								bool cyclisationPerformed = false;
								Attribute *anomericId = carbohydrate->getAttribute(XmlDeclarations::SUFFIXAPPLIESTO_ATR);
								if (anomericId == nullptr)
								{
									throw StructureBuildingException(std::wstring(L"OPSIN bug: Missing suffixAppliesTo on: ") + carbohydrate->getValue());
								}
								Atom *potentialCarbonyl = carbohydrateFrag->getAtomByID(carbohydrateFrag->getIdOfFirstAtom() + std::stoi(anomericId->getValue()) - 1);
								if (potentialCarbonyl == nullptr)
								{
									throw StructureBuildingException(std::wstring(L"OPSIN bug: ") + anomericId->getValue() + std::wstring(L" did not point to an atom on: ") + carbohydrate->getValue());
								}
								carbohydrate->removeAttribute(anomericId);

								Element *nextSibling = OpsinTools::getNextSibling(carbohydrate);
								while (nextSibling != nullptr)
								{
									Element *nextNextSibling = OpsinTools::getNextSibling(nextSibling);
									std::wstring elName = nextSibling->getName();
									if (elName == XmlDeclarations::SUFFIX_EL)
									{
										Element *suffix = nextSibling;
										std::wstring value = suffix->getAttributeValue(XmlDeclarations::VALUE_ATR);
										if (value == L"dialdose" || value == L"aric acid" || value == L"arate")
										{
											if (!isAldose)
											{
												throw StructureBuildingException(value + std::wstring(L" may only be used with aldoses"));
											}
											if (cyclisationPerformed)
											{
												throw StructureBuildingException(std::wstring(L"OPSIN bug: ") + value + std::wstring(L" not expected after carbohydrate cycliser"));
											}
											processAldoseDiSuffix(value, carbohydrate, potentialCarbonyl);
											suffix->detach();
										}
										else if (boost::starts_with(value, L"uron"))
										{
											//strictly these are also aldose di suffixes but in practice they are also used on ketoses
											Attribute tempVar(XmlDeclarations::LOCANT_ATR, std::to_wstring(carbohydrateFrag->getChainLength()));
											suffix->addAttribute(&tempVar);
										}
										else if (!cyclisationPerformed && (value == L"ulose" || value == L"osulose"))
										{
											if (value == L"ulose")
											{
												isAldose = false;
												if (XmlDeclarations::SYSTEMATICCARBOHYDRATESTEMALDOSE_SUBTYPE_VAL == subtype)
												{
													carbohydrate->getAttribute(XmlDeclarations::SUBTYPE_ATR)->setValue(XmlDeclarations::SYSTEMATICCARBOHYDRATESTEMKETOSE_SUBTYPE_VAL);
												}
											}
											potentialCarbonyl = processUloseSuffix(carbohydrate, suffix, potentialCarbonyl);
											suffix->detach();
										}
										else if (value == L"itol" || value == L"yl" || value == L"glycoside")
										{
											Attribute tempVar2(XmlDeclarations::LOCANT_ATR, potentialCarbonyl->getFirstLocant());
											suffix->addAttribute(&tempVar2);
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
											if (value == L"glycoside" && OpsinTools::getParentWordRule(subOrRoot)->getAttributeValue(XmlDeclarations::WORDRULE_ATR) == WordRule::simple->toString())
											{
												throw StructureBuildingException(L"A glycoside requires a space-separated substituent e.g. methyl alpha-D-glucopyranoside");
											}
										}
									}
									else if (elName == XmlDeclarations::CARBOHYDRATERINGSIZE_EL)
									{
										if (cyclisationPerformed)
										{
											throw StructureBuildingException(L"OPSIN bug: Carbohydate cyclised twice!");
										}
										Element *ringSize = nextSibling;
										cycliseCarbohydrateAndApplyAlphaBetaStereo(carbohydrate, ringSize, potentialCarbonyl);
										ringSize->detach();
										cyclisationPerformed = true;
									}
									else if (elName != XmlDeclarations::LOCANT_EL && elName != XmlDeclarations::MULTIPLIER_ATR && elName != XmlDeclarations::UNSATURATOR_EL && elName != XmlDeclarations::COLONORSEMICOLONDELIMITEDLOCANT_EL)
									{
										break;
									}
									nextSibling = nextNextSibling;
								}
								if (!cyclisationPerformed)
								{
									applyUnspecifiedRingSizeCyclisationIfPresent(carbohydrate, potentialCarbonyl);
								}
							}
						}

						void ComponentProcessor::applyUnspecifiedRingSizeCyclisationIfPresent(Element *group, Atom *potentialCarbonyl) throw(StructureBuildingException)
						{
							bool cyclise = false;
							Element *possibleYl = OpsinTools::getNextSibling(group);
							if (possibleYl != nullptr && possibleYl->getName() == XmlDeclarations::SUFFIX_EL)
							{
								if (possibleYl->getAttributeValue(XmlDeclarations::VALUE_ATR) == L"yl")
								{
									cyclise = true;
								}
								else
								{
									//(on|uron)osyl
									possibleYl = OpsinTools::getNextSibling(possibleYl);
									if (possibleYl != nullptr && possibleYl->getName() == XmlDeclarations::SUFFIX_EL && possibleYl->getAttributeValue(XmlDeclarations::VALUE_ATR) == L"yl")
									{
										cyclise = true;
									}
								}
							}
							if (!cyclise)
							{
								Element *alphaOrBetaLocantEl = OpsinTools::getPreviousSiblingIgnoringCertainElements(group, std::vector<std::wstring>{XmlDeclarations::STEREOCHEMISTRY_EL});
								if (alphaOrBetaLocantEl != nullptr && alphaOrBetaLocantEl->getName() == XmlDeclarations::LOCANT_EL)
								{
									std::wstring value = alphaOrBetaLocantEl->getValue();
									if (value == L"alpha" || value == L"beta" || value == L"alpha,beta" || value == L"beta,alpha")
									{
										cyclise = true;
									}
								}
							}
							if (cyclise)
							{
								Element *ringSize = new TokenEl(XmlDeclarations::CARBOHYDRATERINGSIZE_EL);
								std::wstring sugarStem = group->getValue();
								if (group->getFrag()->hasLocant(L"5") && sugarStem != L"rib" && sugarStem != L"fruct")
								{
									Attribute tempVar(XmlDeclarations::VALUE_ATR, L"6");
									ringSize->addAttribute(&tempVar);
								}
								else
								{
									Attribute tempVar2(XmlDeclarations::VALUE_ATR, L"5");
									ringSize->addAttribute(&tempVar2);
								}
								OpsinTools::insertAfter(group, ringSize);
								cycliseCarbohydrateAndApplyAlphaBetaStereo(group, ringSize, potentialCarbonyl);
								ringSize->detach();
							}
						}

						Atom *ComponentProcessor::processUloseSuffix(Element *group, Element *suffix, Atom *potentialCarbonyl) throw(StructureBuildingException)
						{
							std::vector<std::wstring> locantsToConvertToKetones;
							Element *potentialLocantOrMultiplier = OpsinTools::getPreviousSibling(suffix);
							if (potentialLocantOrMultiplier->getName() == XmlDeclarations::MULTIPLIER_ATR)
							{
								int multVal = std::stoi(potentialLocantOrMultiplier->getAttributeValue(XmlDeclarations::VALUE_ATR));
								Element *locant = OpsinTools::getPreviousSibling(potentialLocantOrMultiplier);
								if (locant != nullptr && locant->getName() == XmlDeclarations::LOCANT_EL)
								{
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
									std::vector<std::wstring> locantStrs = locant->getValue().split(L",");
									if (locantStrs.size() != multVal)
									{
										throw StructureBuildingException(std::wstring(L"Mismatch between locant and multiplier counts (") + locantStrs.size() + std::wstring(L" and ") + multVal + std::wstring(L"):") + locant->getValue());
									}
									Collections::addAll(locantsToConvertToKetones, locantStrs);
									locant->detach();
								}
								else
								{
									for (int i = 0; i < multVal; i++)
									{
										locantsToConvertToKetones.push_back(std::to_wstring(i + 2));
									}
								}
								potentialLocantOrMultiplier->detach();
							}
							else
							{
								Element *locant = potentialLocantOrMultiplier;
								if (locant->getName() != XmlDeclarations::LOCANT_EL)
								{
									locant = OpsinTools::getPreviousSibling(group);
								}
								if (locant != nullptr && locant->getName() == XmlDeclarations::LOCANT_EL)
								{
									std::wstring locantStr = locant->getValue();
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
									if (locantStr.split(L",")->length == 1)
									{
										locantsToConvertToKetones.push_back(locantStr);
									}
									else
									{
										throw StructureBuildingException(std::wstring(L"Incorrect number of locants for ul suffix: ") + locantStr);
									}
									locant->detach();
								}
								else
								{
									locantsToConvertToKetones.push_back(L"2");
								}
							}
							Fragment *frag = group->getFrag();
							if (suffix->getAttributeValue(XmlDeclarations::VALUE_ATR) == L"ulose")
							{ //convert aldose to ketose
								Atom *aldehydeAtom = potentialCarbonyl;
								bool foundBond = false;
								for (auto bond : aldehydeAtom->getBonds())
								{
									if (bond->getOrder() == 2)
									{
										Atom *otherAtom = bond->getOtherAtom(aldehydeAtom);
										if (otherAtom->getElement() == ChemEl::O && otherAtom->getCharge() == 0 && otherAtom->getBondCount() == 1)
										{
											bond->setOrder(1);
											foundBond = true;
											break;
										}
									}
								}
								if (!foundBond)
								{
									throw StructureBuildingException(L"OPSIN bug: Unable to convert aldose to ketose");
								}
								Atom *backboneAtom = frag->getAtomByLocantOrThrow(locantsToConvertToKetones[0]);
								potentialCarbonyl = backboneAtom;
							}
							for (auto locantStr : locantsToConvertToKetones)
							{
								Atom *backboneAtom = frag->getAtomByLocantOrThrow(locantStr);
								bool foundBond = false;
								for (auto bond : backboneAtom->getBonds())
								{
									if (bond->getOrder() == 1)
									{
										Atom *otherAtom = bond->getOtherAtom(backboneAtom);
										if (otherAtom->getElement() == ChemEl::O && otherAtom->getCharge() == 0 && otherAtom->getBondCount() == 1)
										{
											bond->setOrder(2);
											foundBond = true;
											break;
										}
									}
								}
								if (!foundBond)
								{
									throw StructureBuildingException(std::wstring(L"Failed to find hydroxy group at position:") + locantStr);
								}
								backboneAtom->setAtomParity(nullptr);
							}
							return potentialCarbonyl;
						}

						void ComponentProcessor::cycliseCarbohydrateAndApplyAlphaBetaStereo(Element *carbohydrateGroup, Element *ringSize, Atom *potentialCarbonyl) throw(StructureBuildingException)
						{
							Fragment *frag = carbohydrateGroup->getFrag();
							std::wstring ringSizeVal = ringSize->getAttributeValue(XmlDeclarations::VALUE_ATR);
							Element *potentialLocant = OpsinTools::getPreviousSibling(ringSize);
							Atom *carbonylCarbon = nullptr;
							Atom *atomToJoinWith = nullptr;
							if (potentialLocant->getName() == XmlDeclarations::LOCANT_EL)
							{
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
								std::vector<std::wstring> locants = potentialLocant->getValue().split(L",");
								if (locants.size() != 2)
								{
									throw StructureBuildingException(std::wstring(L"Expected 2 locants in front of sugar ring size specifier but found: ") + potentialLocant->getValue());
								}
								try
								{
									int firstLocant = std::stoi(locants[0]);
									int secondLocant = std::stoi(locants[1]);
									if (std::abs(secondLocant - firstLocant) != (std::stoi(ringSizeVal) - 2))
									{
										throw StructureBuildingException(std::wstring(L"Mismatch between ring size: ") + ringSizeVal + std::wstring(L" and ring size specified by locants: ") + (std::abs(secondLocant - firstLocant) + 2));
									}
								}
								catch (const NumberFormatException &e)
								{
									throw StructureBuildingException(std::wstring(L"Locants for ring should be numeric but were: ") + potentialLocant->getValue());
								}
								carbonylCarbon = frag->getAtomByLocantOrThrow(locants[0]);
								atomToJoinWith = frag->getAtomByLocantOrThrow(std::wstring(L"O") + locants[1]);
								potentialLocant->detach();
							}

							if (carbonylCarbon == nullptr)
							{
								carbonylCarbon = potentialCarbonyl;
								if (carbonylCarbon == nullptr)
								{
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("OPSIN bug: Could not find carbonyl carbon in carbohydrate");
									throw std::exception();
								}
							}

							for (auto b : carbonylCarbon->getBonds())
							{
								if (b->getOrder() == 2)
								{
									b->setOrder(1);
									break;
								}
							}
							int locantOfCarbonyl;
							try
							{
								locantOfCarbonyl = std::stoi(carbonylCarbon->getFirstLocant());
							}
							catch (const std::exception &e)
							{
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("OPSIN bug: Could not determine locant of carbonyl carbon in carbohydrate", e);
								throw std::exception();
							}
							if (atomToJoinWith == nullptr)
							{
								std::wstring locantToJoinWith = std::to_wstring(locantOfCarbonyl + std::stoi(ringSizeVal) - 2);
								atomToJoinWith = frag->getAtomByLocant(std::wstring(L"O") + locantToJoinWith);
								if (atomToJoinWith == nullptr)
								{
									throw StructureBuildingException(std::wstring(L"Carbohydrate was not an inappropriate length to form a ring of size: ") + ringSizeVal);
								}
							}
							state->fragManager->createBond(carbonylCarbon, atomToJoinWith, 1);
							CycleDetector::assignWhetherAtomsAreInCycles(frag);
							applyAlphaBetaStereoToCyclisedCarbohydrate(carbohydrateGroup, carbonylCarbon);
						}

						void ComponentProcessor::applyAlphaBetaStereoToCyclisedCarbohydrate(Element *carbohydrateGroup, Atom *carbonylCarbon)
						{
							Fragment *frag = carbohydrateGroup->getFrag();
							Element *alphaOrBetaLocantEl = OpsinTools::getPreviousSiblingIgnoringCertainElements(carbohydrateGroup, std::vector<std::wstring>{XmlDeclarations::STEREOCHEMISTRY_EL});
							if (alphaOrBetaLocantEl != nullptr && alphaOrBetaLocantEl->getName() == XmlDeclarations::LOCANT_EL)
							{
								Element *stereoPrefixAfterAlphaBeta = OpsinTools::getNextSibling(alphaOrBetaLocantEl);
								Atom *anomericReferenceAtom = getAnomericReferenceAtom(frag);
								if (anomericReferenceAtom == nullptr)
								{
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("OPSIN bug: Unable to determine anomeric reference atom in: " +carbohydrateGroup.getValue());
									throw std::exception();
								}
								applyAnomerStereochemistryIfPresent(alphaOrBetaLocantEl, carbonylCarbon, anomericReferenceAtom);
								if (carbonylCarbon->getAtomParity() != nullptr && (XmlDeclarations::SYSTEMATICCARBOHYDRATESTEMALDOSE_SUBTYPE_VAL == carbohydrateGroup->getAttributeValue(XmlDeclarations::SUBTYPE_ATR) || XmlDeclarations::SYSTEMATICCARBOHYDRATESTEMKETOSE_SUBTYPE_VAL == carbohydrateGroup->getAttributeValue(XmlDeclarations::SUBTYPE_ATR)))
								{
									//systematic chains only have their stereochemistry defined after structure building to account for the fact that some stereocentres may be removed
									//hence inspect the stereoprefix to see if it is L and flip if so
									std::wstring val = stereoPrefixAfterAlphaBeta->getAttributeValue(XmlDeclarations::VALUE_ATR);
									if (val.substr(val.length() - 1, val.length() - (val.length() - 1))->equals(L"l"))
									{ //"r" if D, "l" if L
										//flip if L
										AtomParity *atomParity = carbonylCarbon->getAtomParity();
										atomParity->setParity(-atomParity->getParity());
									}
								}
							}
							carbonylCarbon->setProperty(Atom::ISANOMERIC, true);
						}

						void ComponentProcessor::processAldoseDiSuffix(const std::wstring &suffixValue, Element *group, Atom *aldehydeAtom) throw(StructureBuildingException)
						{
							Fragment *frag = group->getFrag();
							Atom *alcoholAtom = frag->getAtomByLocantOrThrow(std::to_wstring(frag->getChainLength()));

							if (suffixValue == L"aric acid" || suffixValue == L"arate")
							{
								FragmentTools::removeTerminalOxygen(state, alcoholAtom, 1);
								Fragment *f = state->fragManager->buildSMILES(L"O", group, XmlDeclarations::NONE_LABELS_VAL);
								state->fragManager->incorporateFragment(f, f->getFirstAtom(), frag, alcoholAtom, 2);

								f = state->fragManager->buildSMILES(L"O", group, XmlDeclarations::NONE_LABELS_VAL);
								Atom *hydroxyAtom = f->getFirstAtom();
								if (suffixValue == L"arate")
								{
									hydroxyAtom->addChargeAndProtons(-1, -1);
								}
								state->fragManager->incorporateFragment(f, f->getFirstAtom(), frag, alcoholAtom, 1);
								frag->addFunctionalAtom(hydroxyAtom);

								f = state->fragManager->buildSMILES(L"O", group, XmlDeclarations::NONE_LABELS_VAL);
								hydroxyAtom = f->getFirstAtom();
								if (suffixValue == L"arate")
								{
									hydroxyAtom->addChargeAndProtons(-1, -1);
								}
								state->fragManager->incorporateFragment(f, f->getFirstAtom(), frag, aldehydeAtom, 1);
								frag->addFunctionalAtom(hydroxyAtom);
							}

							else if (suffixValue == L"dialdose")
							{
								FragmentTools::removeTerminalOxygen(state, alcoholAtom, 1);
								Fragment *f = state->fragManager->buildSMILES(L"O", group, XmlDeclarations::NONE_LABELS_VAL);
								state->fragManager->incorporateFragment(f, f->getFirstAtom(), frag, alcoholAtom, 2);
							}
							else
							{
								throw std::invalid_argument(std::wstring(L"OPSIN Bug: Unexpected suffix value: ") + suffixValue);
							}
						}

						Atom *ComponentProcessor::getAnomericReferenceAtom(Fragment *frag)
						{
							std::vector<Atom*> atomList = frag->getAtomList();
							int highestLocantfound = std::numeric_limits<int>::min();
							Atom *configurationalAtom = nullptr;
							for (auto a : atomList)
							{
								if (a->getAtomParity() == nullptr)
								{
									continue;
								}
								try
								{
									std::wstring locant = a->getFirstLocant();
									if (locant != L"")
									{
										int intVal = std::stoi(locant);
										if (intVal > highestLocantfound)
										{
											highestLocantfound = intVal;
											configurationalAtom = a;
										}
									}

								}
								catch (const NumberFormatException &e)
								{
									//may throw number format exceptions
								}
							}
							return configurationalAtom;
						}

						void ComponentProcessor::applyAnomerStereochemistryIfPresent(Element *alphaOrBetaLocantEl, Atom *anomericAtom, Atom *anomericReferenceAtom)
						{
							std::wstring value = alphaOrBetaLocantEl->getValue();
							if (value == L"alpha" || value == L"beta")
							{
								std::vector<Atom*> referenceAtomRefs4 = getDeterministicAtomRefs4ForReferenceAtom(anomericReferenceAtom);
								bool flip = StereochemistryHandler::checkEquivalencyOfAtomsRefs4AndParity(referenceAtomRefs4, 1, anomericReferenceAtom->getAtomParity()->getAtomRefs4(), anomericReferenceAtom->getAtomParity()->getParity());
								std::vector<Atom*> atomRefs4 = getDeterministicAtomRefs4ForAnomericAtom(anomericAtom);
								if (flip)
								{
									if (value == L"alpha")
									{
										anomericAtom->setAtomParity(atomRefs4, 1);
									}
									else
									{
										anomericAtom->setAtomParity(atomRefs4, -1);
									}
								}
								else
								{
									if (value == L"alpha")
									{
										anomericAtom->setAtomParity(atomRefs4, -1);
									}
									else
									{
										anomericAtom->setAtomParity(atomRefs4, 1);
									}
								}
								alphaOrBetaLocantEl->detach();
							}
							else if (value == L"alpha,beta" || value == L"beta,alpha")
							{
								//unspecified stereochemistry
								alphaOrBetaLocantEl->detach();
							}
						}

						std::vector<Atom*> ComponentProcessor::getDeterministicAtomRefs4ForReferenceAtom(Atom *referenceAtom)
						{
							std::vector<Atom*> neighbours = referenceAtom->getAtomNeighbours();
							if (neighbours.size() != 3)
							{
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("OPSIN bug: Unexpected number of atoms connected to anomeric reference atom of carbohydrate");
								throw std::exception();
							}
							std::wstring nextLowestLocant = std::to_wstring(std::stoi(referenceAtom->getFirstLocant()) - 1);
							std::vector<Atom*> atomRefs4(4);
							for (auto neighbour : neighbours)
							{
								if (neighbour->getElement() == ChemEl::O)
								{
									atomRefs4[0] = neighbour;
								}
								else if (neighbour->getElement() == ChemEl::C)
								{
									if (neighbour->getFirstLocant().equals(nextLowestLocant))
									{
										atomRefs4[1] = neighbour;
									}
									else
									{
										atomRefs4[2] = neighbour;
									}
								}
								else
								{
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("OPSIN bug: Unexpected atom element type connected to for anomeric reference atom");
									throw std::exception();
								}
							}
							atomRefs4[3] = AtomParity::hydrogen;
							for (auto atom : atomRefs4)
							{
								if (atom == nullptr)
								{
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("OPSIN bug: Unable to determine atomRefs4 for anomeric reference atom");
									throw std::exception();
								}
							}
							return atomRefs4;
						}

						std::vector<Atom*> ComponentProcessor::getDeterministicAtomRefs4ForAnomericAtom(Atom *anomericAtom)
						{
							std::vector<Atom*> neighbours = anomericAtom->getAtomNeighbours();
							std::vector<Atom*> atomRefs4(4);
							if (neighbours.size() == 3 || neighbours.size() == 4)
							{
								//normal case
							}
							else if (neighbours.size() == 2 && anomericAtom->getOutValency() == 1)
							{
								//trivial glycosyl
								atomRefs4[1] = AtomParity::deoxyHydrogen;
							}
							else
							{
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("OPSIN bug: Unexpected number of atoms connected to anomeric atom of carbohydrate");
								throw std::exception();
							}

							for (auto neighbour : neighbours)
							{
								if (neighbour->getElement() == ChemEl::C)
								{
									if (neighbour->getAtomIsInACycle())
									{
										atomRefs4[0] = neighbour;
									}
									else
									{
										atomRefs4[3] = neighbour;
									}
								}
								else if (neighbour->getElement() == ChemEl::O)
								{
									int incomingVal = neighbour->getIncomingValency();
									if (incomingVal == 1)
									{
										atomRefs4[1] = neighbour;
									}
									else if (incomingVal == 2)
									{
										atomRefs4[2] = neighbour;
									}
									else
									{
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("OPSIN bug: Unexpected valency on oxygen in carbohydrate");
										throw std::exception();
									}
								}
								else
								{
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("OPSIN bug: Unexpected atom element type connected to anomeric atom of carbohydrate");
									throw std::exception();
								}
							}
							if (atomRefs4[3] == nullptr)
							{
								atomRefs4[3] = AtomParity::hydrogen;
							}
							for (auto atom : atomRefs4)
							{
								if (atom == nullptr)
								{
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("OPSIN bug: Unable to assign anomeric carbon stereochemistry on carbohydrate");
									throw std::exception();
								}
							}
							return atomRefs4;
						}

						void ComponentProcessor::processMultipliers(Element *subOrRoot)
						{
							std::vector<Element*> multipliers = subOrRoot->getChildElements(XmlDeclarations::MULTIPLIER_EL);
							for (auto multiplier : multipliers)
							{
								Element *possibleLocant = OpsinTools::getPreviousSibling(multiplier);
								std::vector<std::wstring> locants;
								if (possibleLocant != nullptr)
								{
									std::wstring possibleLocantElName = possibleLocant->getName();
									if (possibleLocantElName == XmlDeclarations::LOCANT_EL)
									{
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
										locants = possibleLocant->getValue().split(L",");
									}
									else if (possibleLocantElName == XmlDeclarations::COLONORSEMICOLONDELIMITEDLOCANT_EL)
									{
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
										locants = StringTools::removeDashIfPresent(possibleLocant->getValue()).split(L":");
									}
								}
								Element *featureToMultiply = OpsinTools::getNextSibling(multiplier);
								std::wstring nextName = featureToMultiply->getName();
								if (nextName == XmlDeclarations::UNSATURATOR_EL || nextName == XmlDeclarations::SUFFIX_EL || nextName == XmlDeclarations::SUBTRACTIVEPREFIX_EL || (nextName == XmlDeclarations::HETEROATOM_EL && XmlDeclarations::GROUP_TYPE_VAL != multiplier->getAttributeValue(XmlDeclarations::TYPE_ATR)) || nextName == XmlDeclarations::HYDRO_EL)
								{
									int mvalue = std::stoi(multiplier->getAttributeValue(XmlDeclarations::VALUE_ATR));
									if (mvalue > 1)
									{
										Attribute tempVar(XmlDeclarations::MULTIPLIED_ATR, L"multiplied");
										featureToMultiply->addAttribute(&tempVar);
									}
									for (int i = mvalue -1; i >= 1; i--)
									{
										Element *newElement = featureToMultiply->copy();
										if (locants.size() > 0 && locants.size() == mvalue)
										{
											Attribute tempVar2(XmlDeclarations::LOCANT_ATR, locants[i]);
											newElement->addAttribute(&tempVar2);
										}
										OpsinTools::insertAfter(featureToMultiply, newElement);
									}
									multiplier->detach();
									if (locants.size() > 0 && locants.size() == mvalue)
									{
										Attribute tempVar3(XmlDeclarations::LOCANT_ATR, locants[0]);
										featureToMultiply->addAttribute(&tempVar3);
										possibleLocant->detach();
									}
								}
							}
						}

						void ComponentProcessor::detectConjunctiveSuffixGroups(Element *subOrRoot, std::vector<Element*> &allGroups) throw(ComponentGenerationException, StructureBuildingException)
						{
							std::vector<Element*> groups = subOrRoot->getChildElements(XmlDeclarations::GROUP_EL);
							if (groups.size() > 1)
							{
								std::vector<Element*> conjunctiveGroups;
								Element *ringGroup = nullptr;
								for (int i = groups.size() - 1 ; i >= 0; i--)
								{
									Element *group = groups[i];
									if (group->getAttributeValue(XmlDeclarations::TYPE_ATR) != XmlDeclarations::RING_TYPE_VAL)
									{ //e.g. the methanol in benzenemethanol.
										conjunctiveGroups.push_back(group);
									}
									else
									{
										ringGroup = group;
										break;
									}
								}
								if (conjunctiveGroups.empty())
								{
									return;
								}
								if (ringGroup == nullptr)
								{
									throw ComponentGenerationException(L"OPSIN bug: unable to find ring associated with conjunctive suffix group");
								}
								if (conjunctiveGroups.size() != 1)
								{
									throw ComponentGenerationException(L"OPSIN Bug: Two groups exactly should be present at this point when processing conjunctive nomenclature");
								}
								Element *primaryConjunctiveGroup = conjunctiveGroups[0];
								Fragment *primaryConjunctiveFrag = primaryConjunctiveGroup->getFrag();
								//remove all locants
								std::vector<Atom*> atomList = primaryConjunctiveFrag->getAtomList();
								for (auto atom : atomList)
								{
									atom->clearLocants();
								}
								std::vector<Element*> suffixes;
								Element *possibleSuffix = OpsinTools::getNextSibling(primaryConjunctiveGroup);
								while (possibleSuffix != nullptr)
								{
									if (possibleSuffix->getName() == XmlDeclarations::SUFFIX_EL)
									{
										suffixes.push_back(possibleSuffix);
									}
									possibleSuffix = OpsinTools::getNextSibling(possibleSuffix);
								}
								preliminaryProcessSuffixes(primaryConjunctiveGroup, suffixes);
								suffixApplier->resolveSuffixes(primaryConjunctiveGroup, suffixes);
								for (auto suffix : suffixes)
								{
									suffix->detach();
								}
								primaryConjunctiveGroup->setName(XmlDeclarations::CONJUNCTIVESUFFIXGROUP_EL);
//JAVA TO C++ CONVERTER TODO TASK: The Java Collection 'remove(Object)' method is not converted:
								allGroups.remove(primaryConjunctiveGroup);

								Element *possibleMultiplier = OpsinTools::getPreviousSibling(primaryConjunctiveGroup);
								//label atoms appropriately
								bool alphaIsPosition1 = atomList[0]->getIncomingValency() < 3;
								int counter = 0;
								for (int i = (alphaIsPosition1 ? 0 : 1); i < atomList.size(); i++)
								{
									Atom *a = atomList[i];
									if (counter == 0)
									{
										a->addLocant(L"alpha");
									}
									else if (counter == 1)
									{
										a->addLocant(L"beta");
									}
									else if (counter == 2)
									{
										a->addLocant(L"gamma");
									}
									else if (counter == 3)
									{
										a->addLocant(L"delta");
									}
									else if (counter == 4)
									{
										a->addLocant(L"epsilon");
									}
									else if (counter == 5)
									{
										a->addLocant(L"zeta");
									}
									else if (counter == 6)
									{
										a->addLocant(L"eta");
									}
									counter++;
								}
								if (XmlDeclarations::MULTIPLIER_EL == possibleMultiplier->getName())
								{
									int multiplier = std::stoi(possibleMultiplier->getAttributeValue(XmlDeclarations::VALUE_ATR));
									for (int i = 1; i < multiplier; i++)
									{
										Element *conjunctiveSuffixGroup = primaryConjunctiveGroup->copy();
										Fragment *newFragment = state->fragManager->copyAndRelabelFragment(primaryConjunctiveFrag, i);
										newFragment->setTokenEl(conjunctiveSuffixGroup);
										conjunctiveSuffixGroup->setFrag(newFragment);
										conjunctiveGroups.push_back(conjunctiveSuffixGroup);
										OpsinTools::insertAfter(primaryConjunctiveGroup, conjunctiveSuffixGroup);
									}
									Element *possibleLocant = OpsinTools::getPreviousSibling(possibleMultiplier);
									possibleMultiplier->detach();
									if (possibleLocant->getName() == XmlDeclarations::LOCANT_EL)
									{
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
										std::vector<std::wstring> locants = possibleLocant->getValue().split(L",");
										if (locants.size() != multiplier)
										{
											throw ComponentGenerationException(L"mismatch between number of locants and multiplier in conjunctive nomenclature routine");
										}
										for (int i = 0; i < locants.size(); i++)
										{
											Attribute tempVar(XmlDeclarations::LOCANT_ATR, locants[i]);
											conjunctiveGroups[i]->addAttribute(&tempVar);
										}
										possibleLocant->detach();
									}
								}
							}
						}

						void ComponentProcessor::matchLocantsToDirectFeatures(Element *subOrRoot) throw(ComponentGenerationException)
						{
							std::vector<Element*> locants = subOrRoot->getChildElements(XmlDeclarations::LOCANT_EL);
							std::vector<Element*> groups = subOrRoot->getChildElements(XmlDeclarations::GROUP_EL);
							for (auto group : groups)
							{
								if (group->getAttributeValue(XmlDeclarations::SUBTYPE_ATR).equals(XmlDeclarations::HANTZSCHWIDMAN_SUBTYPE_VAL))
								{ //handle Hantzch-widman systems
									if (group->getAttribute(XmlDeclarations::ADDBOND_ATR) != nullptr)
									{ //special case for partunsatring
										//exception for where a locant is supposed to indicate the location of a double bond...
										std::vector<Element*> deltas = subOrRoot->getChildElements(XmlDeclarations::DELTA_EL);
										if (deltas.empty())
										{
											Element *delta = new TokenEl(XmlDeclarations::DELTA_EL);
											Element *appropriateLocant = OpsinTools::getPreviousSiblingIgnoringCertainElements(group, std::vector<std::wstring>{XmlDeclarations::HETEROATOM_EL, XmlDeclarations::MULTIPLIER_EL});
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
											if (appropriateLocant != nullptr && appropriateLocant->getName() == XmlDeclarations::LOCANT_EL && appropriateLocant->getValue().split(L",")->length == 1)
											{
												delta->setValue(appropriateLocant->getValue());
												OpsinTools::insertBefore(appropriateLocant, delta);
												appropriateLocant->detach();
//JAVA TO C++ CONVERTER TODO TASK: The Java Collection 'remove(Object)' method is not converted:
												locants.remove(appropriateLocant);
											}
											else
											{
												delta->setValue(L"");
												subOrRoot->insertChild(delta, 0); //no obvious attempt to set double bond position, potentially ambiguous, valency will be used to choose later
											}
										}
									}
									if (locants.size() > 0)
									{
										Element *locantBeforeHWSystem = nullptr;
										std::vector<Element*> heteroAtoms;
										int indexOfGroup = subOrRoot->find(group);
										for (int j = indexOfGroup - 1; j >= 0; j--)
										{
											std::wstring elName = subOrRoot->getChild(j)->getName();
											if (elName == XmlDeclarations::LOCANT_EL)
											{
												locantBeforeHWSystem = subOrRoot->getChild(j);
												break;
											}
											else if (elName == XmlDeclarations::HETEROATOM_EL)
											{
												Element *heteroAtom = subOrRoot->getChild(j);
												heteroAtoms.push_back(heteroAtom);
												if (heteroAtom->getAttribute(XmlDeclarations::LOCANT_ATR) != nullptr)
												{ //locants already assigned, assumedly by process multipliers
													break;
												}
											}
											else
											{
												break;
											}
										}
										std::reverse(heteroAtoms.begin(), heteroAtoms.end());
										if (locantBeforeHWSystem != nullptr)
										{
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
											std::vector<std::wstring> locantValues = locantBeforeHWSystem->getValue().split(L",");
											//detect a solitary locant in front of a HW system and prevent it being assigned.
											//something like 1-aziridin-1-yl never means the N is at position 1 as it is at position 1 by convention
											//this special case is not applied to pseudo HW like systems e.g. [1]oxacyclotetradecine
											if (locantValues.size() == 1 && group->getFrag()->getAtomCount() <= 10)
											{
//JAVA TO C++ CONVERTER TODO TASK: The Java Collection 'remove(Object)' method is not converted:
												locants.remove(locantBeforeHWSystem); //don't assign this locant
											}
											else
											{
												if (locantValues.size() == heteroAtoms.size())
												{
													for (int j = 0; j < locantValues.size(); j++)
													{
														std::wstring locantValue = locantValues[j];
														Attribute tempVar(XmlDeclarations::LOCANT_ATR, locantValue);
														heteroAtoms[j]->addAttribute(&tempVar);
													}
													locantBeforeHWSystem->detach();
//JAVA TO C++ CONVERTER TODO TASK: The Java Collection 'remove(Object)' method is not converted:
													locants.remove(locantBeforeHWSystem);
												}
												else if (heteroAtoms.size() > 1)
												{
													throw ComponentGenerationException(L"Mismatch between number of locants and Hantzsch-Widman heteroatoms");
												}
											}
										}
									}
								}
							}
							assignSingleLocantsToAdjacentFeatures(locants);
						}

						void ComponentProcessor::assignSingleLocantsToAdjacentFeatures(std::vector<Element*> &locants)
						{
							for (auto locant : locants)
							{
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
								std::vector<std::wstring> locantValues = locant->getValue().split(L",");
								Element *referent = OpsinTools::getNextSibling(locant);
								if (referent != nullptr && locantValues.size() == 1)
								{
									std::wstring refName = referent->getName();
									if (refName == XmlDeclarations::ISOTOPESPECIFICATION_EL)
									{
										referent = OpsinTools::getNextSibling(referent);
										if (referent == nullptr)
										{
											return;
										}
										refName = referent->getName();
									}
									//Only assigning locants to elements that were not created by a multiplier
									if (referent->getAttribute(XmlDeclarations::LOCANT_ATR) == nullptr && referent->getAttribute(XmlDeclarations::MULTIPLIED_ATR) == nullptr && (refName == XmlDeclarations::UNSATURATOR_EL || refName == XmlDeclarations::SUFFIX_EL || refName == XmlDeclarations::HETEROATOM_EL || refName == XmlDeclarations::CONJUNCTIVESUFFIXGROUP_EL || refName == XmlDeclarations::SUBTRACTIVEPREFIX_EL || (refName == XmlDeclarations::HYDRO_EL && !boost::starts_with(referent->getValue(), L"per"))))
									{ //not perhydro
										Attribute tempVar(XmlDeclarations::LOCANT_ATR, locantValues[0]);
										referent->addAttribute(&tempVar);
										locant->detach();
									}
								}
							}
						}

						void ComponentProcessor::preliminaryProcessSuffixes(Element *group, std::vector<Element*> &suffixes) throw(ComponentGenerationException, StructureBuildingException)
						{
							Fragment *suffixableFragment = group->getFrag();

							if (group->getAttribute(XmlDeclarations::SUFFIXAPPLIESTO_ATR) != nullptr)
							{ //typically a trivial polyAcid or aminoAcid
								processSuffixAppliesTo(group, suffixes,suffixableFragment);
							}
							else
							{
								for (auto suffix : suffixes)
								{
									if (suffix->getAttribute(XmlDeclarations::ADDITIONALVALUE_ATR) != nullptr)
									{
										throw ComponentGenerationException(std::wstring(L"suffix: ") + suffix->getValue() + std::wstring(L" used on an inappropriate group"));
									}
								}
							}
							applyDefaultLocantsToSuffixesIfApplicable(group, suffixableFragment);

							std::vector<Fragment*> suffixFragments = resolveGroupAddingSuffixes(suffixes, suffixableFragment);
							state->xmlSuffixMap[group] = suffixFragments;
							bool suffixesResolved = false;
							if (group->getAttributeValue(XmlDeclarations::TYPE_ATR) == XmlDeclarations::CHALCOGENACIDSTEM_TYPE_VAL)
							{ //merge the suffix into the chalcogen acid stem e.g sulfonoate needs to be one fragment for infix replacement
								suffixApplier->resolveSuffixes(group, suffixes);
								suffixesResolved = true;
							}
							processSuffixPrefixes(suffixes); //e.g. carbox amide
							functionalReplacement->processInfixFunctionalReplacementNomenclature(suffixes, suffixFragments);
							processRemovalOfHydroxyGroupsRules(suffixes, suffixableFragment);

							if (group->getValue() == L"oxal")
							{ //oxalic acid is treated as a non carboxylic acid for the purposes of functional replacment. See P-65.2.3
								suffixApplier->resolveSuffixes(group, suffixes);
								group->getAttribute(XmlDeclarations::TYPE_ATR)->setValue(XmlDeclarations::NONCARBOXYLICACID_TYPE_VAL);
								suffixesResolved = true;
							}
							if (suffixesResolved)
							{
								//suffixes have already been resolved so need to be detached to avoid being passed to resolveSuffixes later
								for (int i = suffixes.size() - 1; i >= 0; i--)
								{
									Element *suffix = suffixes.erase(suffixes.begin() + i);
									suffix->detach();
								}
							}
							if (group->getAttribute(XmlDeclarations::NUMBEROFFUNCTIONALATOMSTOREMOVE_ATR) != nullptr)
							{
								int numberToRemove = std::stoi(group->getAttributeValue(XmlDeclarations::NUMBEROFFUNCTIONALATOMSTOREMOVE_ATR));
								if (numberToRemove > suffixableFragment->getFunctionalAtomCount())
								{
									throw ComponentGenerationException(L"Too many hydrogen for the number of positions on non carboxylic acid");
								}
								for (int i = 0; i < numberToRemove; i++)
								{
									Atom *functionalAtom = suffixableFragment->removeFunctionalAtom(0)->getAtom();
									functionalAtom->neutraliseCharge();
								}
							}
						}

						void ComponentProcessor::applyDefaultLocantsToSuffixesIfApplicable(Element *group, Fragment *suffixableFragment)
						{
							std::wstring defaultLocantsAtrValue = group->getAttributeValue(XmlDeclarations::SUFFIXAPPLIESTOBYDEFAULT_ATR);
							if (defaultLocantsAtrValue != L"")
							{
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
								std::vector<std::wstring> suffixInstructions = defaultLocantsAtrValue.split(L",");
								Element *suffix = OpsinTools::getNextNonChargeSuffix(group);
								if (suffix != nullptr)
								{
									std::vector<Element*> suffixes;
									while (suffix != nullptr)
									{
										suffixes.push_back(suffix);
										suffix = OpsinTools::getNextNonChargeSuffix(suffix);
									}
									if (suffixInstructions.size() == suffixes.size())
									{
										int firstIdInFragment = suffixableFragment->getIdOfFirstAtom();
										for (int i = 0; i < suffixInstructions.size(); i++)
										{
											std::wstring suffixInstruction = suffixInstructions[i];
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
											Attribute tempVar(XmlDeclarations::DEFAULTLOCANTID_ATR, Integer::toString(firstIdInFragment + std::stoi(suffixInstruction) - 1));
											suffixes[i]->addAttribute(&tempVar);
										}
									}
								}
							}
						}

						void ComponentProcessor::processSuffixAppliesTo(Element *group, std::vector<Element*> &suffixes, Fragment *suffixableFragment) throw(ComponentGenerationException)
						{
							//suffixAppliesTo attribute contains instructions for number/positions of suffix
							//this is of the form comma sepeated ids with the number of ids corresponding to the number of instances of the suffix
							Element *suffix = OpsinTools::getNextNonChargeSuffix(group);
							if (suffix == nullptr)
							{
								if (group->getAttributeValue(XmlDeclarations::TYPE_ATR) == XmlDeclarations::ACIDSTEM_TYPE_VAL)
								{
									throw ComponentGenerationException(L"No suffix where suffix was expected");
								}
							}
							else
							{
								if (suffixes.size() > 1 && group->getAttributeValue(XmlDeclarations::TYPE_ATR) == XmlDeclarations::ACIDSTEM_TYPE_VAL)
								{
									throw ComponentGenerationException(L"More than one suffix detected on trivial polyAcid. Not believed to be allowed");
								}

								std::wstring suffixInstruction = group->getAttributeValue(XmlDeclarations::SUFFIXAPPLIESTO_ATR);
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
								std::vector<std::wstring> suffixInstructions = suffixInstruction.split(L",");
								int firstIdInFragment = suffixableFragment->getIdOfFirstAtom();
								if (XmlDeclarations::CYCLEFORMER_SUBTYPE_VAL == suffix->getAttributeValue(XmlDeclarations::SUBTYPE_ATR))
								{
									if (suffixInstructions.size() != 2)
									{
										throw ComponentGenerationException(std::wstring(L"suffix: ") + suffix->getValue() + std::wstring(L" used on an inappropriate group"));
									}
									std::vector<std::wstring> locantIds(2);
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
									locantIds[0] = Integer::toString(firstIdInFragment + std::stoi(suffixInstructions[0]) - 1);
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
									locantIds[1] = Integer::toString(firstIdInFragment + std::stoi(suffixInstructions[1]) - 1);
									Attribute tempVar(XmlDeclarations::LOCANTID_ATR, StringTools::arrayToString(locantIds, L","));
									suffix->addAttribute(&tempVar);
									return;
								}
								bool symmetricSuffixes = true;
								if (suffix->getAttribute(XmlDeclarations::ADDITIONALVALUE_ATR) != nullptr)
								{ //handles amic, aldehydic, anilic and amoyl suffixes properly
									if (suffixInstructions.size() < 2)
									{
										throw ComponentGenerationException(std::wstring(L"suffix: ") + suffix->getValue() + std::wstring(L" used on an inappropriate group"));
									}
									symmetricSuffixes = false;
								}

								if (suffix->getAttribute(XmlDeclarations::LOCANT_ATR) == nullptr)
								{
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
									Attribute tempVar2(XmlDeclarations::LOCANTID_ATR, Integer::toString(firstIdInFragment + std::stoi(suffixInstructions[0]) - 1));
									suffix->addAttribute(&tempVar2);
								}
								for (int i = 1; i < suffixInstructions.size(); i++)
								{
									Element *newSuffix = new TokenEl(XmlDeclarations::SUFFIX_EL);
									if (symmetricSuffixes)
									{
										Attribute tempVar3(XmlDeclarations::VALUE_ATR, suffix->getAttributeValue(XmlDeclarations::VALUE_ATR));
										newSuffix->addAttribute(&tempVar3);
										Attribute tempVar4(XmlDeclarations::TYPE_ATR, suffix->getAttributeValue(XmlDeclarations::TYPE_ATR));
										newSuffix->addAttribute(&tempVar4);
										if (suffix->getAttribute(XmlDeclarations::SUBTYPE_ATR) != nullptr)
										{
											Attribute tempVar5(XmlDeclarations::SUBTYPE_ATR, suffix->getAttributeValue(XmlDeclarations::SUBTYPE_ATR));
											newSuffix->addAttribute(&tempVar5);
										}
										if (suffix->getAttribute(XmlDeclarations::INFIX_ATR) != nullptr && boost::starts_with(suffix->getAttributeValue(XmlDeclarations::INFIX_ATR), L"="))
										{ //clone infixes that effect double bonds but not single bonds e.g. maleamidate still should have one functional atom
											Attribute tempVar6(XmlDeclarations::INFIX_ATR, suffix->getAttributeValue(XmlDeclarations::INFIX_ATR));
											newSuffix->addAttribute(&tempVar6);
										}
									}
									else
									{
										Attribute tempVar7(XmlDeclarations::VALUE_ATR, suffix->getAttributeValue(XmlDeclarations::ADDITIONALVALUE_ATR));
										newSuffix->addAttribute(&tempVar7);
										Attribute tempVar8(XmlDeclarations::TYPE_ATR, XmlDeclarations::ROOT_EL);
										newSuffix->addAttribute(&tempVar8);
									}
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
									Attribute tempVar9(XmlDeclarations::LOCANTID_ATR, Integer::toString(firstIdInFragment + std::stoi(suffixInstructions[i]) - 1));
									newSuffix->addAttribute(&tempVar9);
									OpsinTools::insertAfter(suffix, newSuffix);
									suffixes.push_back(newSuffix);
								}
							}
						}

						std::vector<Fragment*> ComponentProcessor::resolveGroupAddingSuffixes(std::vector<Element*> &suffixes, Fragment *frag) throw(StructureBuildingException, ComponentGenerationException)
						{
							std::vector<Fragment*> suffixFragments;
							std::wstring groupType = frag->getType();
							std::wstring subgroupType = frag->getSubType();

							std::wstring suffixTypeToUse = L"";
							if (suffixApplier->isGroupTypeWithSpecificSuffixRules(groupType))
							{
								suffixTypeToUse = groupType;
							}
							else
							{
								suffixTypeToUse = XmlDeclarations::STANDARDGROUP_TYPE_VAL;
							}

							for (auto suffix : suffixes)
							{
								std::wstring suffixValue = suffix->getAttributeValue(XmlDeclarations::VALUE_ATR);

								bool cyclic; //needed for addSuffixPrefixIfNonePresentAndCyclic rule
								Atom *atomLikelyToBeUsedBySuffix = nullptr;

								std::wstring locant = suffix->getAttributeValue(XmlDeclarations::LOCANT_ATR);
								std::wstring locantId = suffix->getAttributeValue(XmlDeclarations::LOCANTID_ATR);

								if (locant != L"" && locant.find(L',') == std::wstring::npos)
								{
									atomLikelyToBeUsedBySuffix = frag->getAtomByLocant(locant);
								}
								else if (locantId != L"" && locantId.find(L',') == std::wstring::npos)
								{
									atomLikelyToBeUsedBySuffix = frag->getAtomByIDOrThrow(std::stoi(locantId));
								}
								if (atomLikelyToBeUsedBySuffix == nullptr)
								{
									//a locant has not been specified
									//also can happen in the cases of things like fused rings where the final numbering is not available so lookup by locant fails (in which case all the atoms will be cyclic anyway)
									atomLikelyToBeUsedBySuffix = frag->getFirstAtom();
								}
								cyclic = atomLikelyToBeUsedBySuffix->getAtomIsInACycle();

								std::vector<SuffixRule*> suffixRules = suffixApplier->getSuffixRuleTags(suffixTypeToUse, suffixValue, subgroupType);
								Fragment *suffixFrag = nullptr;
								/*
								 * Temp fragments are build for each addGroup rule and then merged into suffixFrag
								 */
								for (auto suffixRule : suffixRules)
								{
									switch (suffixRule->getType())
									{
									case addgroup:
									{
										std::wstring labels = suffixRule->getAttributeValue(XmlDeclarations::SUFFIXRULES_LABELS_ATR);
										if (labels == L"")
										{
											labels = XmlDeclarations::NONE_LABELS_VAL;
										}
										suffixFrag = state->fragManager->buildSMILES(suffixRule->getAttributeValue(XmlDeclarations::SUFFIXRULES_SMILES_ATR), XmlDeclarations::SUFFIX_TYPE_VAL, labels);
										std::vector<Atom*> atomList = suffixFrag->getAtomList();
										std::wstring functionalIdsAtr = suffixRule->getAttributeValue(XmlDeclarations::SUFFIXRULES_FUNCTIONALIDS_ATR);
										if (functionalIdsAtr != L"")
										{
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
											std::vector<std::wstring> relativeIdsOfFunctionalAtoms = functionalIdsAtr.split(L",");
											for (auto relativeId : relativeIdsOfFunctionalAtoms)
											{
												int atomIndice = std::stoi(relativeId) - 1;
												if (atomIndice >= atomList.size())
												{
													throw StructureBuildingException(L"Check suffixRules.xml: Atom requested to have a functionalAtom was not within the suffix fragment");
												}
												suffixFrag->addFunctionalAtom(atomList[atomIndice]);
											}
										}
										std::wstring outIdsAtr = suffixRule->getAttributeValue(XmlDeclarations::SUFFIXRULES_OUTIDS_ATR);
										if (outIdsAtr != L"")
										{
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
											std::vector<std::wstring> relativeIdsOfOutAtoms = outIdsAtr.split(L",");
											for (auto relativeId : relativeIdsOfOutAtoms)
											{
												int atomIndice = std::stoi(relativeId) - 1;
												if (atomIndice >= atomList.size())
												{
													throw StructureBuildingException(L"Check suffixRules.xml: Atom requested to have a outAtom was not within the suffix fragment");
												}
												suffixFrag->addOutAtom(atomList[atomIndice], 1, true);
											}
										}
										break;
									}
									case addSuffixPrefixIfNonePresentAndCyclic:
										if (cyclic && suffix->getAttribute(XmlDeclarations::SUFFIXPREFIX_ATR) == nullptr)
										{
											Attribute tempVar(XmlDeclarations::SUFFIXPREFIX_ATR, suffixRule->getAttributeValue(XmlDeclarations::SUFFIXRULES_SMILES_ATR));
											suffix->addAttribute(&tempVar);
										}
										break;
									case addFunctionalAtomsToHydroxyGroups:
										if (suffixFrag != nullptr)
										{
											throw ComponentGenerationException(L"addFunctionalAtomsToHydroxyGroups is not currently compatable with the addGroup suffix rule");
										}
										addFunctionalAtomsToHydroxyGroups(atomLikelyToBeUsedBySuffix);
										break;
									case chargeHydroxyGroups:
										if (suffixFrag != nullptr)
										{
											throw ComponentGenerationException(L"chargeHydroxyGroups is not currently compatable with the addGroup suffix rule");
										}
										chargeHydroxyGroups(atomLikelyToBeUsedBySuffix);
										break;
									case removeTerminalOxygen:
									{
										if (suffixFrag != nullptr)
										{
											throw ComponentGenerationException(L"removeTerminalOxygen is not currently compatible with the addGroup suffix rule");
										}
										int bondOrder = std::stoi(suffixRule->getAttributeValue(XmlDeclarations::SUFFIXRULES_ORDER_ATR));
										FragmentTools::removeTerminalOxygen(state, atomLikelyToBeUsedBySuffix, bondOrder);
										break;
									}
									default:
										break;
									}
								}
								if (suffixFrag != nullptr)
								{
									suffixFragments.push_back(suffixFrag);
									suffix->setFrag(suffixFrag);
								}
							}
							return suffixFragments;
						}

						void ComponentProcessor::processRemovalOfHydroxyGroupsRules(std::vector<Element*> &suffixes, Fragment *frag) throw(ComponentGenerationException)
						{
							std::wstring groupType = frag->getType();
							std::wstring subgroupType = frag->getSubType();
							std::wstring suffixTypeToUse = L"";
							if (suffixApplier->isGroupTypeWithSpecificSuffixRules(groupType))
							{
								suffixTypeToUse = groupType;
							}
							else
							{
								suffixTypeToUse = XmlDeclarations::STANDARDGROUP_TYPE_VAL;
							}
							for (auto suffix : suffixes)
							{
								std::wstring suffixValue = suffix->getAttributeValue(XmlDeclarations::VALUE_ATR);
								std::vector<SuffixRule*> suffixRules = suffixApplier->getSuffixRuleTags(suffixTypeToUse, suffixValue, subgroupType);
								for (auto suffixRule : suffixRules)
								{
									SuffixRuleType type = suffixRule->getType();
									if (type == SuffixRuleType::convertHydroxyGroupsToOutAtoms)
									{
										convertHydroxyGroupsToOutAtoms(frag);
									}
									else if (type == SuffixRuleType::convertHydroxyGroupsToPositiveCharge)
									{
										convertHydroxyGroupsToPositiveCharge(frag);
									}
								}
							}
						}

						void ComponentProcessor::addFunctionalAtomsToHydroxyGroups(Atom *atom) throw(StructureBuildingException)
						{
							std::vector<Atom*> neighbours = atom->getAtomNeighbours();
							for (auto neighbour : neighbours)
							{
								if (neighbour->getElement() == ChemEl::O && neighbour->getCharge() == 0 && neighbour->getBondCount() == 1 && atom->getBondToAtomOrThrow(neighbour)->getOrder() == 1)
								{
									neighbour->getFrag()->addFunctionalAtom(neighbour);
								}
							}
						}

						void ComponentProcessor::chargeHydroxyGroups(Atom *atom) throw(StructureBuildingException)
						{
							std::vector<Atom*> neighbours = atom->getAtomNeighbours();
							for (auto neighbour : neighbours)
							{
								if (neighbour->getElement() == ChemEl::O && neighbour->getCharge() == 0 && neighbour->getBondCount() == 1 && atom->getBondToAtomOrThrow(neighbour)->getOrder() == 1)
								{
									neighbour->addChargeAndProtons(-1, -1);
								}
							}
						}

						void ComponentProcessor::convertHydroxyGroupsToOutAtoms(Fragment *frag)
						{
							std::vector<Atom*> atomList = frag->getAtomList();
							for (auto atom : atomList)
							{
								if (atom->getElement() == ChemEl::O && atom->getCharge() == 0 && atom->getBondCount() == 1 && atom->getFirstBond()->getOrder() == 1 && atom->getOutValency() == 0)
								{
									Atom *adjacentAtom = atom->getAtomNeighbours().get(0);
									if (adjacentAtom->getElement() != ChemEl::O)
									{
										state->fragManager->removeAtomAndAssociatedBonds(atom);
										frag->addOutAtom(adjacentAtom, 1, true);
									}
								}
							}
						}

						void ComponentProcessor::convertHydroxyGroupsToPositiveCharge(Fragment *frag)
						{
							std::vector<Atom*> atomList = frag->getAtomList();
							for (auto atom : atomList)
							{
								if (atom->getElement() == ChemEl::O && atom->getCharge() == 0 && atom->getBondCount() == 1 && atom->getFirstBond()->getOrder() == 1 && atom->getOutValency() == 0)
								{
									Atom *adjacentAtom = atom->getAtomNeighbours().get(0);
									if (adjacentAtom->getElement() != ChemEl::O)
									{
										state->fragManager->removeAtomAndAssociatedBonds(atom);
										adjacentAtom->addChargeAndProtons(1, -1);
									}
								}
							}
						}

						void ComponentProcessor::processSuffixPrefixes(std::vector<Element*> &suffixes) throw(StructureBuildingException)
						{
							for (auto suffix : suffixes)
							{
								if (suffix->getAttribute(XmlDeclarations::SUFFIXPREFIX_ATR) != nullptr)
								{
									Fragment *suffixPrefixFrag = state->fragManager->buildSMILES(suffix->getAttributeValue(XmlDeclarations::SUFFIXPREFIX_ATR), XmlDeclarations::SUFFIX_TYPE_VAL, XmlDeclarations::NONE_LABELS_VAL);
									addFunctionalAtomsToHydroxyGroups(suffixPrefixFrag->getFirstAtom());
									if (suffix->getValue().endsWith(L"ate") || suffix->getValue().endsWith(L"at"))
									{
										chargeHydroxyGroups(suffixPrefixFrag->getFirstAtom());
									}
									Atom *firstAtomOfPrefix = suffixPrefixFrag->getFirstAtom();
									firstAtomOfPrefix->addLocant(L"X"); //make sure this atom is not given a locant
									Fragment *suffixFrag = suffix->getFrag();
									state->fragManager->incorporateFragment(suffixPrefixFrag, suffixFrag);

									//manipulate suffixFrag such that all the bonds to the first atom (the R)  go instead to the first atom of suffixPrefixFrag.
									//Then reconnect the R to that atom
									Atom *theR = suffixFrag->getFirstAtom();
									std::vector<Atom*> neighbours = theR->getAtomNeighbours();
									for (auto neighbour : neighbours)
									{
										Bond *b = theR->getBondToAtomOrThrow(neighbour);
										state->fragManager->removeBond(b);
										state->fragManager->createBond(neighbour, firstAtomOfPrefix, b->getOrder());
									}
									state->fragManager->createBond(firstAtomOfPrefix, theR, 1);
								}
							}
						}

						bool ComponentProcessor::checkLocantPresentOnPotentialRoot(BuildState *state, Element *startingElement, const std::wstring &locant) throw(StructureBuildingException)
						{
							bool foundSibling = false;
							Deque<Element*> *s = new ArrayDeque<Element*>();
							s->add(startingElement);
							bool doneFirstIteration = false; //check on index only done on first iteration to only get elements with an index greater than the starting element
							while (s->size() > 0)
							{
								Element *currentElement = s->removeLast();
								Element *parent = currentElement->getParent();
								std::vector<Element*> siblings = OpsinTools::getChildElementsWithTagNames(parent, std::vector<std::wstring>{XmlDeclarations::BRACKET_EL, XmlDeclarations::SUBSTITUENT_EL, XmlDeclarations::ROOT_EL});
								int indexOfCurrentElement = parent->find(currentElement);

								for (auto bracketOrSub : siblings)
								{
									if (!doneFirstIteration && parent->find(bracketOrSub) <= indexOfCurrentElement)
									{
										continue;
									}
									if (bracketOrSub->getName().equals(XmlDeclarations::BRACKET_EL))
									{ //only want to consider implicit brackets, not proper brackets
										if (bracketOrSub->getAttribute(XmlDeclarations::TYPE_ATR) == nullptr)
										{
											continue;
										}
										s->add(bracketOrSub->getChild(0));
									}
									else
									{
										Element *group = bracketOrSub->getFirstChildElement(XmlDeclarations::GROUP_EL);
										Fragment *groupFrag = group->getFrag();
										if (groupFrag->hasLocant(locant))
										{
											return true;
										}
										std::vector<Fragment*> suffixes = state->xmlSuffixMap[group];
										if (suffixes.size() > 0)
										{
											for (auto suffix : suffixes)
											{
												if (suffix->hasLocant(locant))
												{
													return true;
												}
											}
										}
										std::vector<Element*> conjunctiveGroups = OpsinTools::getNextSiblingsOfType(group, XmlDeclarations::CONJUNCTIVESUFFIXGROUP_EL);
										for (auto conjunctiveGroup : conjunctiveGroups)
										{
											if (conjunctiveGroup->getFrag()->hasLocant(locant))
											{
												return true;
											}
										}
									}
									foundSibling = true;
								}
								doneFirstIteration = true;
							}

							if (!foundSibling)
							{ //Special case: anything the group could potentially substitute onto is in a bracket. The bracket is checked recursively
								s = new ArrayDeque<Element*>();
								s->add(startingElement);
								doneFirstIteration = false; //check on index only done on first iteration to only get elements with an index greater than the starting element
								while (s->size() > 0)
								{
									Element *currentElement = s->removeLast();
									Element *parent = currentElement->getParent();
									std::vector<Element*> siblings = OpsinTools::getChildElementsWithTagNames(parent, std::vector<std::wstring>{XmlDeclarations::BRACKET_EL, XmlDeclarations::SUBSTITUENT_EL, XmlDeclarations::ROOT_EL});
									int indexOfCurrentElement = parent->find(currentElement);

									for (auto bracketOrSub : siblings)
									{
										if (!doneFirstIteration && parent->find(bracketOrSub) <= indexOfCurrentElement)
										{
											continue;
										}
										if (bracketOrSub->getName().equals(XmlDeclarations::BRACKET_EL))
										{
											s->add(bracketOrSub->getChild(0));
										}
										else
										{
											Element *group = bracketOrSub->getFirstChildElement(XmlDeclarations::GROUP_EL);
											Fragment *groupFrag = group->getFrag();
											if (groupFrag->hasLocant(locant))
											{
												return true;
											}
											std::vector<Fragment*> suffixes = state->xmlSuffixMap[group];
											if (suffixes.size() > 0)
											{
												for (auto suffix : suffixes)
												{
													if (suffix->hasLocant(locant))
													{
														return true;
													}
												}
											}
											std::vector<Element*> conjunctiveGroups = OpsinTools::getNextSiblingsOfType(group, XmlDeclarations::CONJUNCTIVESUFFIXGROUP_EL);
											for (auto conjunctiveGroup : conjunctiveGroups)
											{
												if (conjunctiveGroup->getFrag()->hasLocant(locant))
												{
													return true;
												}
											}
										}
									}
									doneFirstIteration = true;
								}
							}

							return false;
						}

						void ComponentProcessor::handleGroupIrregularities(std::vector<Element*> &groups) throw(StructureBuildingException, ComponentGenerationException)
						{
							for (auto group : groups)
							{
								std::wstring groupValue = group->getValue();
								if (groupValue == L"porphyrin" || groupValue == L"porphin")
								{
									std::vector<Element*> hydrogenAddingEls = group->getParent()->getChildElements(XmlDeclarations::INDICATEDHYDROGEN_EL);
									bool implicitHydrogenExplicitlySet = false;
									for (auto hydrogenAddingEl : hydrogenAddingEls)
									{
										std::wstring locant = hydrogenAddingEl->getAttributeValue(XmlDeclarations::LOCANT_ATR);
										if (locant != L"" && (locant == L"21" || locant == L"22" || locant == L"23" || locant == L"24"))
										{
											implicitHydrogenExplicitlySet = true;
										}
									}
									if (!implicitHydrogenExplicitlySet)
									{
										//porphyrins implicitly have indicated hydrogen at the 21/23 positions
										//directly modify the fragment to avoid problems with locants in for example ring assemblies
										Fragment *frag = group->getFrag();
										frag->getAtomByLocantOrThrow(L"21")->setSpareValency(false);
										frag->getAtomByLocantOrThrow(L"23")->setSpareValency(false);
									}
								}
								else if (groupValue == L"xanthate" || groupValue == L"xanthat" || groupValue == L"xanthic acid" || groupValue == L"xanthicacid")
								{
									//This test needs to be here rather in the ComponentGenerator to correctly reject non substituted thioxanthates
									Element *wordRule = OpsinTools::getParentWordRule(group);
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
									if (wordRule->getAttributeValue(XmlDeclarations::WORDRULE_ATR) == WordRule::simple->toString())
									{
										if (OpsinTools::getDescendantElementsWithTagName(wordRule, XmlDeclarations::SUBSTITUENT_EL).empty())
										{
											throw ComponentGenerationException(groupValue + std::wstring(L" describes a class of compounds rather than a particular compound"));
										}
									}
								}
								else if (groupValue == L"adenosin" || groupValue == L"cytidin" || groupValue == L"guanosin" || groupValue == L"inosin" || groupValue == L"uridin" || groupValue == L"xanthosin")
								{
									//These groups are 2'-deoxy by convention
									Element *previous = OpsinTools::getPreviousSibling(group);
									if (previous != nullptr && previous->getName() == XmlDeclarations::SUBTRACTIVEPREFIX_EL && previous->getAttributeValue(XmlDeclarations::TYPE_ATR) == XmlDeclarations::DEOXY_TYPE_VAL && previous->getAttributeValue(XmlDeclarations::VALUE_ATR) == L"O" && previous->getAttribute(XmlDeclarations::LOCANT_ATR) == nullptr)
									{
										Element *prev2 = OpsinTools::getPrevious(previous);
										if (prev2 == nullptr || prev2->getName() != XmlDeclarations::SUBTRACTIVEPREFIX_EL)
										{
											Fragment *frag = group->getFrag();
											StructureBuildingMethods::applySubtractivePrefix(state, frag, ChemEl::O, L"2'");
											previous->detach();
										}
									}
								}

								if ((std::wstring(L"yes")) == group->getAttributeValue(XmlDeclarations::USABLEASJOINER_ATR) && group->getAttribute(XmlDeclarations::DEFAULTINID_ATR) == nullptr && group->getAttribute(XmlDeclarations::DEFAULTINLOCANT_ATR) == nullptr)
								{
									//makes linkers by default attach end to end
									Fragment *frag = group->getFrag();
									int chainLength = frag->getChainLength();
									if (chainLength > 1)
									{
										bool connectEndToEndWithPreviousSub = true;
										if (group->getAttributeValue(XmlDeclarations::TYPE_ATR).equals(XmlDeclarations::CHAIN_TYPE_VAL) && XmlDeclarations::ALKANESTEM_SUBTYPE_VAL == group->getAttributeValue(XmlDeclarations::SUBTYPE_ATR))
										{ //don't do this if the group is preceded by another alkaneStem e.g. methylethyl makes more sense as prop-2-yl rather than propyl
											Element *previousSubstituent = OpsinTools::getPreviousSibling(group->getParent());
											if (previousSubstituent != nullptr)
											{
												std::vector<Element*> previousSubstGroups = previousSubstituent->getChildElements(XmlDeclarations::GROUP_EL);
												if (previousSubstGroups.size() == 1)
												{
													Element *previousGroup = previousSubstGroups[0];
													if (previousGroup->getAttributeValue(XmlDeclarations::TYPE_ATR) == XmlDeclarations::CHAIN_TYPE_VAL && XmlDeclarations::ALKANESTEM_SUBTYPE_VAL == previousGroup->getAttributeValue(XmlDeclarations::SUBTYPE_ATR))
													{
														Element *suffixAfterGroup = OpsinTools::getNextSibling(previousGroup, XmlDeclarations::SUFFIX_EL);
														if (suffixAfterGroup == nullptr || suffixAfterGroup->getFrag() == nullptr || suffixAfterGroup->getFrag()->getOutAtomCount() == 0)
														{
															connectEndToEndWithPreviousSub = false;
														}
													}
												}
											}
										}
										if (connectEndToEndWithPreviousSub)
										{
											Element *parent = group->getParent();
											while (parent->getName() == XmlDeclarations::BRACKET_EL)
											{
												parent = parent->getParent();
											}
											if (parent->getName() != XmlDeclarations::ROOT_EL)
											{
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
												Attribute tempVar(XmlDeclarations::DEFAULTINID_ATR, Integer::toString(chainLength));
												group->addAttribute(&tempVar);
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
												frag->setDefaultInAtom(frag->getAtomByLocantOrThrow(Integer::toString(chainLength)));
											}
										}
									}
								}
							}
						}

						void ComponentProcessor::processHW(Element *subOrRoot) throw(StructureBuildingException, ComponentGenerationException)
						{
							std::vector<Element*> hwGroups = OpsinTools::getChildElementsWithTagNameAndAttribute(subOrRoot, XmlDeclarations::GROUP_EL, XmlDeclarations::SUBTYPE_ATR, XmlDeclarations::HANTZSCHWIDMAN_SUBTYPE_VAL);
							for (auto group : hwGroups)
							{
								Fragment *hwRing = group->getFrag();
								std::vector<Atom*> atomList = hwRing->getAtomList();
								bool noLocants = true;
								std::vector<Element*> prevs;
								Element *prev = OpsinTools::getPreviousSibling(group);
								while (prev != nullptr && prev->getName() == XmlDeclarations::HETEROATOM_EL)
								{
									prevs.push_back(prev);
									if (prev->getAttribute(XmlDeclarations::LOCANT_ATR) != nullptr)
									{
										noLocants = false;
									}
									prev = OpsinTools::getPreviousSibling(prev);
								}
								std::reverse(prevs.begin(), prevs.end());
								std::vector<Element*> &heteroatomsToProcess = prevs;

								if (atomList.size() == 6 && group->getValue().equals(L"an"))
								{
									bool hasNitrogen = false;
									bool hasSiorGeorSnorPb = false;
									bool saturatedRing = true;
									for (auto heteroatom : heteroatomsToProcess)
									{
										std::wstring heteroAtomElement = heteroatom->getAttributeValue(XmlDeclarations::VALUE_ATR);
										Matcher *m = OpsinTools::MATCH_ELEMENT_SYMBOL->matcher(heteroAtomElement);
										if (!m->find())
										{
											throw ComponentGenerationException(L"Failed to extract element from Hantzsch-Widman heteroatom");
										}
										heteroAtomElement = m->group();
										if (heteroAtomElement == L"N")
										{
											hasNitrogen = true;
										}
										if (heteroAtomElement == L"Si" || heteroAtomElement == L"Ge" || heteroAtomElement == L"Sn" || heteroAtomElement == L"Pb")
										{
											hasSiorGeorSnorPb = true;
										}
									}
									for (auto a : atomList)
									{
										if (a->hasSpareValency())
										{
											saturatedRing = false;
										}
									}
									if (saturatedRing && !hasNitrogen && hasSiorGeorSnorPb)
									{
										throw ComponentGenerationException(L"Blocked Hantzsch-Widman system (6 member saturated ring with no nitrogen but has Si/Ge/Sn/Pb)");
									}
								}
								StringBuilder *nameSB = new StringBuilder();
								for (auto heteroatom : heteroatomsToProcess)
								{
									std::wstring hetValue = heteroatom->getValue();
									if (boost::ends_with(hetValue, L"a"))
									{
										nameSB->append(hetValue.substr(0, hetValue.length() - 1));
									}
									else
									{
										nameSB->append(hetValue);
									}
								}
								nameSB->append(group->getValue());
								std::wstring name = nameSB->toString();
								group->setValue(name);
								if (noLocants && heteroatomsToProcess.size() > 0)
								{
									std::vector<std::wstring> specialRingInformation = specialHWRings[name];
									if (specialRingInformation.size() > 0)
									{
										std::wstring specialInstruction = specialRingInformation[0];
										if (specialInstruction != L"")
										{
											if (specialInstruction == L"blocked")
											{
												throw ComponentGenerationException(L"Blocked Hantzsch-Widman system");
											}
											else if (specialInstruction == L"saturated")
											{
												for (auto a : atomList)
												{
													a->setSpareValency(false);
												}
											}
											else if (specialInstruction == L"not_icacid")
											{
												if (group->getAttribute(XmlDeclarations::SUBSEQUENTUNSEMANTICTOKEN_ATR) == nullptr)
												{
													Element *nextEl = OpsinTools::getNextSibling(group);
													if (nextEl != nullptr && nextEl->getName() == XmlDeclarations::SUFFIX_EL && nextEl->getAttribute(XmlDeclarations::LOCANT_ATR) == nullptr && nextEl->getAttributeValue(XmlDeclarations::VALUE_ATR) == L"ic")
													{
														throw ComponentGenerationException(name + nextEl->getValue() + std::wstring(L" appears to be a generic class name, not a Hantzsch-Widman ring"));
													}
												}
											}
											else if (specialInstruction == L"not_nothingOrOlate")
											{
												if (group->getAttribute(XmlDeclarations::SUBSEQUENTUNSEMANTICTOKEN_ATR) == nullptr)
												{
													Element *nextEl = OpsinTools::getNextSibling(group);
													if (nextEl == nullptr || (nextEl != nullptr && nextEl->getName() == XmlDeclarations::SUFFIX_EL && nextEl->getAttribute(XmlDeclarations::LOCANT_ATR) == nullptr && nextEl->getAttributeValue(XmlDeclarations::VALUE_ATR) == L"ate"))
													{
														throw ComponentGenerationException(name + std::wstring(L" has the syntax for a Hantzsch-Widman ring but probably does not mean that in this context"));
													}
												}
											}
											else
											{
												throw ComponentGenerationException(L"OPSIN Bug: Unrecognised special Hantzsch-Widman ring instruction");
											}
										}
										//something like oxazole where by convention locants go 1,3 or a inorganic HW-like system
										for (int i = 1; i < specialRingInformation.size(); i++)
										{
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
											Atom *a = hwRing->getAtomByLocantOrThrow(Integer::toString(i));
											a->setElement(StringHelper::fromString<ChemEl>(specialRingInformation[i]));
										}
										for (auto p : heteroatomsToProcess)
										{
											p->detach();
										}
										heteroatomsToProcess.clear();
									}
								}

								//add locanted heteroatoms
								for (std::vector<Element*>::const_iterator it = heteroatomsToProcess.begin(); it != heteroatomsToProcess.end(); ++it)
								{
									Element *heteroatom = *it;
									std::wstring locant = heteroatom->getAttributeValue(XmlDeclarations::LOCANT_ATR);
									if (locant == L"")
									{
										continue;
									}
									std::wstring elementReplacement = heteroatom->getAttributeValue(XmlDeclarations::VALUE_ATR);
									Matcher *m = OpsinTools::MATCH_ELEMENT_SYMBOL->matcher(elementReplacement);
									if (!m->find())
									{
										throw ComponentGenerationException(L"Failed to extract element from Hantzsch-Widman heteroatom");
									}
									elementReplacement = m->group();
									Atom *a = hwRing->getAtomByLocantOrThrow(locant);
									a->setElement(StringHelper::fromString<ChemEl>(elementReplacement));
									if (heteroatom->getAttribute(XmlDeclarations::LAMBDA_ATR) != nullptr)
									{
										a->setLambdaConventionValency(std::stoi(heteroatom->getAttributeValue(XmlDeclarations::LAMBDA_ATR)));
									}
									heteroatom->detach();
									(*it)->remove();
								}

								std::vector<Element*> deltaEls = subOrRoot->getChildElements(XmlDeclarations::DELTA_EL);
								//add locanted double bonds and convert unlocanted to unsaturators
								for (auto deltaEl : deltaEls)
								{
									std::wstring locantOfDoubleBond = deltaEl->getValue();
									if (locantOfDoubleBond == L"")
									{
										Element *newUnsaturator = new TokenEl(XmlDeclarations::UNSATURATOR_EL);
										Attribute tempVar(XmlDeclarations::VALUE_ATR, L"2");
										newUnsaturator->addAttribute(&tempVar);
										OpsinTools::insertAfter(group, newUnsaturator);
									}
									else
									{
										Atom *firstInDoubleBond = hwRing->getAtomByLocantOrThrow(locantOfDoubleBond);
										FragmentTools::unsaturate(firstInDoubleBond, 2, hwRing);
									}
									deltaEl->detach();
								}

								//add unlocanted heteroatoms
								int hetAtomsToProcess = heteroatomsToProcess.size();
								if (hetAtomsToProcess > 0)
								{
									std::vector<Atom*> carbonAtomsInRing;
									for (auto atom : atomList)
									{
										if (atom->getElement() == ChemEl::C)
										{
											carbonAtomsInRing.push_back(atom);
										}
									}

									if (hetAtomsToProcess > 1 && hetAtomsToProcess < (carbonAtomsInRing.size() - 1))
									{
										Element *possibleBenzo = OpsinTools::getPreviousSibling(group, XmlDeclarations::GROUP_EL);
										//assume benzo fusions or hwring as a fusion prefix produce unambiguous heteroatom positioning
										if (!(possibleBenzo != nullptr && (possibleBenzo->getValue() == L"benz" || possibleBenzo->getValue() == L"benzo") || (std::wstring(L"o")) == group->getAttributeValue((XmlDeclarations::SUBSEQUENTUNSEMANTICTOKEN_ATR))))
										{
											state->addIsAmbiguous(std::wstring(L"Heteroatom positioning in the Hantzsch-Widman name ") + name);
										}
									}
									if (hetAtomsToProcess > carbonAtomsInRing.size())
									{
										throw StructureBuildingException(std::to_wstring(hetAtomsToProcess) + L" heteroatoms were specified for a Hantzsch-Widman ring with only " + carbonAtomsInRing.size() + std::wstring(L" atoms"));
									}
									for (int i = 0; i < hetAtomsToProcess; i++)
									{
										Element *heteroatom = heteroatomsToProcess[i];
										std::wstring elementReplacement = heteroatom->getAttributeValue(XmlDeclarations::VALUE_ATR);
										Matcher *m = OpsinTools::MATCH_ELEMENT_SYMBOL->matcher(elementReplacement);
										if (!m->find())
										{
											throw ComponentGenerationException(L"Failed to extract element from Hantzsch-Widman heteroatom");
										}
										elementReplacement = m->group();
										Atom *a = carbonAtomsInRing[i];
										a->setElement(StringHelper::fromString<ChemEl>(elementReplacement));
										if (heteroatom->getAttribute(XmlDeclarations::LAMBDA_ATR) != nullptr)
										{
											a->setLambdaConventionValency(std::stoi(heteroatom->getAttributeValue(XmlDeclarations::LAMBDA_ATR)));
										}
										heteroatom->detach();
									}
								}
							}
						}

						void ComponentProcessor::assignElementSymbolLocants(Element *subOrRoot) throw(StructureBuildingException)
						{
							std::vector<Element*> groups = subOrRoot->getChildElements(XmlDeclarations::GROUP_EL);
							Element *lastGroupElementInSubOrRoot = groups[groups.size() - 1];
							std::vector<Fragment*> suffixFragments(state->xmlSuffixMap[lastGroupElementInSubOrRoot]);
							Fragment *suffixableFragment = lastGroupElementInSubOrRoot->getFrag();
							//treat conjunctive suffixesas if they were suffixes
							std::vector<Element*> conjunctiveGroups = subOrRoot->getChildElements(XmlDeclarations::CONJUNCTIVESUFFIXGROUP_EL);
							for (auto group : conjunctiveGroups)
							{
								suffixFragments.push_back(group->getFrag());
							}
							FragmentTools::assignElementLocants(suffixableFragment, suffixFragments);
							for (int i = groups.size() - 2; i >= 0; i--)
							{
								FragmentTools::assignElementLocants(groups[i]->getFrag(), std::vector<Fragment*>());
							}
						}

						void ComponentProcessor::processRingAssemblies(Element *subOrRoot) throw(ComponentGenerationException, StructureBuildingException)
						{
							std::vector<Element*> ringAssemblyMultipliers = subOrRoot->getChildElements(XmlDeclarations::RINGASSEMBLYMULTIPLIER_EL);
							for (auto multiplier : ringAssemblyMultipliers)
							{
								int mvalue = std::stoi(multiplier->getAttributeValue(XmlDeclarations::VALUE_ATR));

								/*
								 * Populate locants with locants. Two locants are required for every pair of rings to be joined.
								 * e.g. bi requires 2, ter requires 4 etc.
								 */
								std::vector<std::vector<std::wstring>> ringJoiningLocants;
								Element *potentialLocant = OpsinTools::getPreviousSibling(multiplier);
								Element *group = OpsinTools::getNextSibling(multiplier, XmlDeclarations::GROUP_EL);
								if (potentialLocant != nullptr && (potentialLocant->getName() == XmlDeclarations::COLONORSEMICOLONDELIMITEDLOCANT_EL || potentialLocant->getName() == XmlDeclarations::LOCANT_EL))
								{ //a locant appears to have been provided to indicate how to connect the rings of the ringAssembly
									if (XmlDeclarations::ORTHOMETAPARA_TYPE_VAL == potentialLocant->getAttributeValue(XmlDeclarations::TYPE_ATR))
									{ //an OMP locant has been provided to indicate how to connect the rings of the ringAssembly
										std::wstring locant2 = potentialLocant->getValue();
										std::wstring locant1 = L"1";
										std::vector<std::wstring> locantArrayList;
										locantArrayList.push_back(L"1");
										locantArrayList.push_back(L"1'");
										ringJoiningLocants.push_back(locantArrayList);
										for (int i = 1; i < mvalue - 1; i++)
										{
											locantArrayList = std::vector<std::wstring>();
											locantArrayList.push_back(locant2 + StringTools::multiplyString(L"'", i));
											locantArrayList.push_back(locant1 + StringTools::multiplyString(L"'", i + 1));
											ringJoiningLocants.push_back(locantArrayList);
										}
										potentialLocant->detach();
									}
									else
									{
										std::wstring locantText = StringTools::removeDashIfPresent(potentialLocant->getValue());
										//locantText might be something like 1,1':3',1''
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
										std::vector<std::wstring> perRingLocantArray = OpsinTools::MATCH_COLONORSEMICOLON->split(locantText);
										if (perRingLocantArray.size() != (mvalue - 1))
										{
											throw ComponentGenerationException(std::wstring(L"Disagreement between number of locants(") + locantText + std::wstring(L") and ring assembly multiplier: ") + std::to_wstring(mvalue));
										}
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
										if (perRingLocantArray.size() != 1 || perRingLocantArray[0].split(L",")->length != 1)
										{ //if there is just a single locant it doesn't relate to how the rings are connected
											for (auto ringLocantArray : perRingLocantArray)
											{
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
												std::vector<std::wstring> locantArray = ringLocantArray.split(L",");
												if (locantArray.size() != 2)
												{
													throw ComponentGenerationException(std::wstring(L"missing locant, expected 2 locants: ") + ringLocantArray);
												}
												ringJoiningLocants.push_back(Arrays::asList(locantArray));
											}
											potentialLocant->detach();
										}
									}
								}

								Fragment *fragmentToResolveAndDuplicate = group->getFrag();
								Element *elementToResolve; //temporary element containing elements that should be resolved before the ring is duplicated
								Element *nextEl = OpsinTools::getNextSibling(multiplier);
								if (nextEl->getName() == XmlDeclarations::STRUCTURALOPENBRACKET_EL)
								{ //brackets have been provided to aid disambiguation. These brackets are detached e.g. bi(cyclohexyl)
									elementToResolve = new GroupingEl(XmlDeclarations::SUBSTITUENT_EL);
									Element *currentEl = nextEl;
									nextEl = OpsinTools::getNextSibling(currentEl);
									currentEl->detach();
									while (nextEl != nullptr && nextEl->getName() != XmlDeclarations::STRUCTURALCLOSEBRACKET_EL)
									{
										currentEl = nextEl;
										nextEl = OpsinTools::getNextSibling(currentEl);
										currentEl->detach();
										elementToResolve->addChild(currentEl);
									}
									if (nextEl != nullptr)
									{
										nextEl->detach();
									}
								}
								else
								{
									elementToResolve = determineElementsToResolveIntoRingAssembly(multiplier, ringJoiningLocants.size(), fragmentToResolveAndDuplicate->getOutAtomCount());
								}

								std::vector<Element*> suffixes = elementToResolve->getChildElements(XmlDeclarations::SUFFIX_EL);
								suffixApplier->resolveSuffixes(group, suffixes);
								int bondOrder = 1;
								if (fragmentToResolveAndDuplicate->getOutAtomCount() > 1)
								{
									throw StructureBuildingException(L"Ring assembly fragment should have one or no OutAtoms; not more than one!");
								}
								if (fragmentToResolveAndDuplicate->getOutAtomCount() == 1)
								{ //e.g. bicyclohexanylidene
									bondOrder = fragmentToResolveAndDuplicate->getOutAtom(0)->getValency();
								}

								bool twoRingsJoinedUsingSuffixPosition = ringJoiningLocants.empty() && mvalue == 2 && fragmentToResolveAndDuplicate->getOutAtomCount() == 1;
								if (!twoRingsJoinedUsingSuffixPosition && fragmentToResolveAndDuplicate->getOutAtomCount() == 1)
								{
									//remove yl (or the like). Need to make sure that resolveUnLocantedFeatures doesn't consider 2,2'-bipyridyl ambiguous due to the location of the ul
									fragmentToResolveAndDuplicate->removeOutAtom(0);
								}

								StructureBuildingMethods::resolveLocantedFeatures(state, elementToResolve);
								StructureBuildingMethods::resolveUnLocantedFeatures(state, elementToResolve);

								group->detach();
								OpsinTools::insertAfter(multiplier, group);

								if (twoRingsJoinedUsingSuffixPosition)
								{
									Fragment *clone = state->fragManager->copyAndRelabelFragment(fragmentToResolveAndDuplicate, 1);
									Atom *atomOnParent = fragmentToResolveAndDuplicate->getOutAtom(0)->getAtom();
									Atom *atomOnClone = clone->getOutAtom(0)->getAtom();
									fragmentToResolveAndDuplicate->removeOutAtom(0);
									clone->removeOutAtom(0);
									state->fragManager->incorporateFragment(clone, atomOnClone, fragmentToResolveAndDuplicate, atomOnParent, bondOrder);
								}
								else
								{
									std::vector<Fragment*> clonedFragments;
									for (int j = 1; j < mvalue; j++)
									{
										clonedFragments.push_back(state->fragManager->copyAndRelabelFragment(fragmentToResolveAndDuplicate, j));
									}
									Fragment *lastRingUnlocantedBondedTo = nullptr;
									for (int i = 0; i < mvalue - 1; i++)
									{
										Fragment *clone = clonedFragments[i];
										Atom *atomOnParent;
										Atom *atomOnLatestClone;
										if (ringJoiningLocants.size() > 0)
										{ //locants defined
											atomOnParent = fragmentToResolveAndDuplicate->getAtomByLocantOrThrow(ringJoiningLocants[i][0]);
											std::wstring secondLocant = ringJoiningLocants[i][1];
											if (mvalue == 2 && !boost::ends_with(secondLocant, L"'"))
											{
												//Allow prime to be (incorrectly) omitted on second locant in bi ring assemblies e.g. 2,2-bipyridine
												try
												{
													atomOnLatestClone = clone->getAtomByLocantOrThrow(secondLocant);
												}
												catch (const StructureBuildingException &e)
												{
													atomOnLatestClone = clone->getAtomByLocant(secondLocant + std::wstring(L"'"));
													if (atomOnLatestClone == nullptr)
													{
														throw e;
													}
												}
											}
											else
											{
												atomOnLatestClone = clone->getAtomByLocantOrThrow(secondLocant);
											}
										}
										else
										{
											std::vector<Atom*> potentialAtomsOnParent;
											if (lastRingUnlocantedBondedTo == nullptr)
											{
												potentialAtomsOnParent = FragmentTools::findSubstituableAtoms(fragmentToResolveAndDuplicate, bondOrder);
											}
											else
											{
												potentialAtomsOnParent = FragmentTools::findSubstituableAtoms(lastRingUnlocantedBondedTo, bondOrder);
											}
											std::vector<Atom*> potentialAtomsOnClone = FragmentTools::findSubstituableAtoms(clone, bondOrder);
											if (potentialAtomsOnParent.empty() || potentialAtomsOnClone.empty())
											{
												throw StructureBuildingException(L"Unable to find suitable atom for unlocanted ring assembly construction");
											}
											if (AmbiguityChecker::isSubstitutionAmbiguous(potentialAtomsOnParent, 1))
											{
												state->addIsAmbiguous(std::wstring(L"Choice of atoms to form ring assembly: ") + group->getValue());
											}
											if (AmbiguityChecker::isSubstitutionAmbiguous(potentialAtomsOnClone, 1))
											{
												state->addIsAmbiguous(std::wstring(L"Choice of atoms to form ring assembly: ") + group->getValue());
											}
											atomOnParent = potentialAtomsOnParent[0];
											atomOnLatestClone = potentialAtomsOnClone[0];
											lastRingUnlocantedBondedTo = clone;
										}
										state->fragManager->incorporateFragment(clone, atomOnLatestClone, fragmentToResolveAndDuplicate, atomOnParent, bondOrder);
									}
								}

								group->setValue(multiplier->getValue() + group->getValue());
								Element *possibleOpenStructuralBracket = OpsinTools::getPreviousSibling(multiplier);
								if (possibleOpenStructuralBracket != nullptr && possibleOpenStructuralBracket->getName() == XmlDeclarations::STRUCTURALOPENBRACKET_EL)
								{ //e.g. [2,2'-bipyridin].
									//To emphasise there can actually be two sets of structural brackets e.g. [1,1'-bi(cyclohexyl)]
									OpsinTools::getNextSibling(possibleOpenStructuralBracket, XmlDeclarations::STRUCTURALCLOSEBRACKET_EL)->detach();
									possibleOpenStructuralBracket->detach();
								}
								multiplier->detach();
							}
						}

						Element *ComponentProcessor::determineElementsToResolveIntoRingAssembly(Element *multiplier, int ringJoiningLocants, int outAtomCount) throw(ComponentGenerationException)
						{
							Element *elementToResolve = new GroupingEl(XmlDeclarations::SUBSTITUENT_EL);
							bool groupFound = false;
							bool inlineSuffixSeen = outAtomCount > 0;
							Element *currentEl = OpsinTools::getNextSibling(multiplier);
							while (currentEl != nullptr)
							{
								//Attach all until group found
								//Attach unlocanted charge suffixes/unsaturation
								//Attach one unlocanted unmultiplied inline suffix (or one locanted unmultiplied inline suffix if it is a bi ring assembly e.g. bipyridin-2-yl)
								Element *nextEl = OpsinTools::getNextSibling(currentEl);
								if (!groupFound)
								{
									currentEl->detach();
									elementToResolve->addChild(currentEl);
									if (currentEl->getName() == XmlDeclarations::GROUP_EL)
									{
										groupFound = true;
									}
								}
								else
								{
									if (currentEl->getName() == XmlDeclarations::SUFFIX_EL)
									{
										std::wstring suffixType = currentEl->getAttributeValue(XmlDeclarations::TYPE_ATR);
										if (suffixType == XmlDeclarations::CHARGE_TYPE_VAL && currentEl->getAttribute(XmlDeclarations::LOCANT_ATR) == nullptr)
										{
											currentEl->detach();
											elementToResolve->addChild(currentEl);
										}
										else if (!inlineSuffixSeen && suffixType == XmlDeclarations::INLINE_TYPE_VAL && currentEl->getAttributeValue(XmlDeclarations::MULTIPLIED_ATR) == L"" && (currentEl->getAttribute(XmlDeclarations::LOCANT_ATR) == nullptr || ((std::wstring(L"2")) == multiplier->getAttributeValue(XmlDeclarations::VALUE_ATR) && ringJoiningLocants == 0)) && currentEl->getFrag() == nullptr)
										{
											inlineSuffixSeen = true;
											currentEl->detach();
											elementToResolve->addChild(currentEl);
										}
										else
										{
											break;
										}
									}
									else if (currentEl->getName() == XmlDeclarations::UNSATURATOR_EL && currentEl->getAttribute(XmlDeclarations::LOCANT_ATR) == nullptr)
									{
										currentEl->detach();
										elementToResolve->addChild(currentEl);
									}
									else
									{
										break;
									}
								}
								currentEl = nextEl;
							}
							Element *parent = multiplier->getParent();
							if (parent->getName() != XmlDeclarations::SUBSTITUENT_EL && OpsinTools::getChildElementsWithTagNameAndAttribute(parent, XmlDeclarations::SUFFIX_EL, XmlDeclarations::TYPE_ATR, XmlDeclarations::INLINE_TYPE_VAL).size() > 0)
							{
								throw ComponentGenerationException(L"Unexpected radical adding suffix on ring assembly");
							}
							return elementToResolve;
						}

						void ComponentProcessor::processPolyCyclicSpiroNomenclature(Element *subOrRoot) throw(ComponentGenerationException, StructureBuildingException)
						{
							std::vector<Element*> polyCyclicSpiros = subOrRoot->getChildElements(XmlDeclarations::POLYCYCLICSPIRO_EL);
							if (polyCyclicSpiros.size() > 0)
							{
								Element *polyCyclicSpiroDescriptor = polyCyclicSpiros[0];
								std::wstring value = polyCyclicSpiroDescriptor->getAttributeValue(XmlDeclarations::VALUE_ATR);
								if (value == L"spiro")
								{
									if (polyCyclicSpiros.size() != 1)
									{
										throw ComponentGenerationException(L"Nested polyspiro systems are not supported");
									}
									processNonIdenticalPolyCyclicSpiro(polyCyclicSpiroDescriptor);
								}
								else if (value == L"spiroOldMethod")
								{
									processOldMethodPolyCyclicSpiro(polyCyclicSpiros);
								}
								else if (value == L"spirobi")
								{
									if (polyCyclicSpiros.size() != 1)
									{
										throw ComponentGenerationException(L"Nested polyspiro systems are not supported");
									}
									processSpiroBiOrTer(polyCyclicSpiroDescriptor, 2);
								}
								else if (value == L"spiroter")
								{
									if (polyCyclicSpiros.size() != 1)
									{
										throw ComponentGenerationException(L"Nested polyspiro systems are not supported");
									}
									processSpiroBiOrTer(polyCyclicSpiroDescriptor, 3);
								}
								else if (value == L"dispiroter")
								{
									if (polyCyclicSpiros.size() != 1)
									{
										throw ComponentGenerationException(L"Nested polyspiro systems are not supported");
									}
									processDispiroter(polyCyclicSpiroDescriptor);
								}
								else
								{
									throw ComponentGenerationException(L"Unsupported spiro system encountered");
								}
								polyCyclicSpiroDescriptor->detach();
							}
						}

						void ComponentProcessor::processNonIdenticalPolyCyclicSpiro(Element *polyCyclicSpiroDescriptor) throw(ComponentGenerationException, StructureBuildingException)
						{
							Element *subOrRoot = polyCyclicSpiroDescriptor->getParent();
							Element *openBracket = OpsinTools::getNextSibling(polyCyclicSpiroDescriptor);
							if (openBracket->getName() != XmlDeclarations::STRUCTURALOPENBRACKET_EL)
							{
								throw ComponentGenerationException(L"OPSIN Bug: Open bracket not found where open bracket expeced");
							}
							std::vector<Element*> spiroBracketElements = OpsinTools::getSiblingsUpToElementWithTagName(openBracket, XmlDeclarations::STRUCTURALCLOSEBRACKET_EL);
							Element *closeBracket = OpsinTools::getNextSibling(spiroBracketElements[spiroBracketElements.size() - 1]);
							if (closeBracket == nullptr || closeBracket->getName() != XmlDeclarations::STRUCTURALCLOSEBRACKET_EL)
							{
								throw ComponentGenerationException(L"OPSIN Bug: Open bracket not found where open bracket expeced");
							}

							std::vector<Element*> groups;
							for (auto spiroBracketElement : spiroBracketElements)
							{
								std::wstring name = spiroBracketElement->getName();
								if (name == XmlDeclarations::GROUP_EL)
								{
									groups.push_back(spiroBracketElement);
								}
								else if (name == XmlDeclarations::SPIROLOCANT_EL)
								{
									Element *spiroLocant = spiroBracketElement;
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
									std::vector<std::wstring> locants = StringTools::removeDashIfPresent(spiroLocant->getValue()).split(L",");
									if (locants.size() != 2)
									{
										throw ComponentGenerationException(L"Incorrect number of locants found before component of polycyclic spiro system");
									}
									bool changed = false;
									Matcher *m1 = matchAddedHydrogenBracket->matcher(locants[0]);
									if (m1->find())
									{
										Element *addedHydrogenElement = new TokenEl(XmlDeclarations::ADDEDHYDROGEN_EL);
										std::wstring addedHydrogenLocant = m1->group(1);
										int primes = StringTools::countTerminalPrimes(addedHydrogenLocant);
										if (primes > 0 && primes == (groups.size() - 1))
										{ //rings are primeless before spiro fusion (hydrogen is currently added before spiro fusion)
											addedHydrogenLocant = addedHydrogenLocant.substr(0, addedHydrogenLocant.length() - primes);
										}
										Attribute tempVar(XmlDeclarations::LOCANT_ATR, addedHydrogenLocant);
										addedHydrogenElement->addAttribute(&tempVar);
										OpsinTools::insertBefore(spiroLocant, addedHydrogenElement);
										locants[0] = m1->replaceAll(L"");
										changed = true;
									}
									Matcher *m2 = matchAddedHydrogenBracket->matcher(locants[1]);
									if (m2->find())
									{
										Element *addedHydrogenElement = new TokenEl(XmlDeclarations::ADDEDHYDROGEN_EL);
										std::wstring addedHydrogenLocant = m2->group(1);
										int primes = StringTools::countTerminalPrimes(addedHydrogenLocant);
										if (primes > 0 && primes == groups.size())
										{ //rings are primeless before spiro fusion (hydrogen is currently added before spiro fusion)
											addedHydrogenLocant = addedHydrogenLocant.substr(0, addedHydrogenLocant.length() - primes);
										}
										Attribute tempVar2(XmlDeclarations::LOCANT_ATR, addedHydrogenLocant);
										addedHydrogenElement->addAttribute(&tempVar2);
										OpsinTools::insertAfter(spiroLocant, addedHydrogenElement);
										locants[1] = m2->replaceAll(L"");
										changed = true;
									}
									if (changed)
									{
										Attribute tempVar3(XmlDeclarations::TYPE_ATR, XmlDeclarations::ADDEDHYDROGENLOCANT_TYPE_VAL);
										spiroLocant->addAttribute(&tempVar3);
									}
									spiroLocant->setValue(StringTools::arrayToString(locants, L","));
								}
							}
							int groupCount = groups.size();
							if (groupCount < 2)
							{
								throw ComponentGenerationException(L"OPSIN Bug: Atleast two groups were expected in polycyclic spiro system");
							}

							Element *firstGroup = groups[0];
							std::vector<Element*> firstGroupEls;
							int indexOfOpenBracket = subOrRoot->find(openBracket);
							Element *firstSpiroLocant = OpsinTools::getNextSibling(firstGroup, XmlDeclarations::SPIROLOCANT_EL);
							if (firstSpiroLocant == nullptr)
							{
								throw ComponentGenerationException(L"Unable to find spiroLocant for polycyclic spiro system");
							}
							int indexOfFirstSpiroLocant = subOrRoot->find(firstSpiroLocant);
							for (int i = indexOfOpenBracket + 1; i < indexOfFirstSpiroLocant; i++)
							{
								firstGroupEls.push_back(subOrRoot->getChild(i));
							}
							resolveFeaturesOntoGroup(firstGroupEls);
							Set<Atom*> *spiroAtoms = std::unordered_set<Atom*>();
							for (int i = 1; i < groupCount; i++)
							{
								Element *nextGroup = groups[i];
								Element *spiroLocant = OpsinTools::getNextSibling(groups[i - 1], XmlDeclarations::SPIROLOCANT_EL);
								if (spiroLocant == nullptr)
								{
									throw ComponentGenerationException(L"Unable to find spiroLocant for polycyclic spiro system");
								}
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
								std::vector<std::wstring> locants = spiroLocant->getValue().split(L",");

								std::vector<Element*> nextGroupEls;
								int indexOfLocant = subOrRoot->find(spiroLocant);
								int indexOfNextSpiroLocantOrEndOfSpiro = subOrRoot->find(i + 1 < groupCount ? OpsinTools::getNextSibling(nextGroup, XmlDeclarations::SPIROLOCANT_EL) : OpsinTools::getNextSibling(nextGroup, XmlDeclarations::STRUCTURALCLOSEBRACKET_EL));
								for (int j = indexOfLocant + 1; j < indexOfNextSpiroLocantOrEndOfSpiro; j++)
								{
									nextGroupEls.push_back(subOrRoot->getChild(j));
								}
								resolveFeaturesOntoGroup(nextGroupEls);

								spiroLocant->detach();
								Fragment *nextFragment = nextGroup->getFrag();
								FragmentTools::relabelNumericLocants(nextFragment->getAtomList(), StringTools::multiplyString(L"'", i));
								std::wstring secondLocant = locants[1];
								Atom *atomOnNextFragment;
								if (boost::ends_with(secondLocant, L"'"))
								{
									atomOnNextFragment = nextFragment->getAtomByLocantOrThrow(locants[1]);
								}
								else
								{
									//for simple spiro fusions the prime is often forgotten
									atomOnNextFragment = nextFragment->getAtomByLocantOrThrow(locants[1] + std::wstring(L"'"));
								}
								Atom *atomToBeReplaced = nullptr;
								for (int j = 0; j < i; j++)
								{
									atomToBeReplaced = groups[j]->getFrag().getAtomByLocant(locants[0]);
									if (atomToBeReplaced != nullptr)
									{
										break;
									}
								}
								if (atomToBeReplaced == nullptr)
								{
									throw ComponentGenerationException(std::wstring(L"Could not find the atom with locant ") + locants[0] + std::wstring(L" for use in polycyclic spiro system"));
								}
								spiroAtoms->add(atomToBeReplaced);
								if (atomToBeReplaced->getElement() != atomOnNextFragment->getElement())
								{
									//In well formed names these should be identical but by special case pick the heteroatom if the other is carbon
									if (atomToBeReplaced->getElement() != ChemEl::C && atomOnNextFragment->getElement() == ChemEl::C)
									{
										atomOnNextFragment->setElement(atomToBeReplaced->getElement());
									}
									else if (atomToBeReplaced->getElement() != ChemEl::C && atomOnNextFragment->getElement() != ChemEl::C)
									{
										throw ComponentGenerationException(std::wstring(L"Disagreement between which element the spiro atom should be: ") + atomToBeReplaced->getElement() + std::wstring(L" and ") + atomOnNextFragment->getElement());
									}
								}
								if (atomToBeReplaced->hasSpareValency())
								{
									atomOnNextFragment->setSpareValency(true);
								}
								state->fragManager->replaceAtomWithAnotherAtomPreservingConnectivity(atomToBeReplaced, atomOnNextFragment);
							}
							if (spiroAtoms->size() > 1)
							{
								Element *expectedMultiplier = OpsinTools::getPreviousSibling(polyCyclicSpiroDescriptor);
								if (expectedMultiplier != nullptr && expectedMultiplier->getName() == XmlDeclarations::MULTIPLIER_EL && std::stoi(expectedMultiplier->getAttributeValue(XmlDeclarations::VALUE_ATR)) == spiroAtoms->size())
								{
									expectedMultiplier->detach();
								}
							}
							Element *rootGroup = groups[groupCount - 1];
							Fragment *rootFrag = rootGroup->getFrag();
							std::wstring name = rootGroup->getValue();
							for (int i = 0; i < groupCount - 1; i++)
							{
								Element *group = groups[i];
								state->fragManager->incorporateFragment(group->getFrag(), rootFrag);
								name = group->getValue() + name;
								group->detach();
							}
							rootGroup->setValue(polyCyclicSpiroDescriptor->getValue() + name);
							openBracket->detach();
							closeBracket->detach();
						}

						void ComponentProcessor::processOldMethodPolyCyclicSpiro(std::vector<Element*> &spiroElements) throw(ComponentGenerationException, StructureBuildingException)
						{
							Element *firstSpiro = spiroElements[0];
							Element *subOrRoot = firstSpiro->getParent();
							Element *firstEl = subOrRoot->getChild(0);
							std::vector<Element*> elementsToResolve = OpsinTools::getSiblingsUpToElementWithTagName(firstEl, XmlDeclarations::POLYCYCLICSPIRO_EL);
							elementsToResolve.push_back(0, firstEl);
							resolveFeaturesOntoGroup(elementsToResolve);

							for (int i = 0; i < spiroElements.size(); i++)
							{
								Element *currentSpiro = spiroElements[i];
								Element *previousGroup = OpsinTools::getPreviousSibling(currentSpiro, XmlDeclarations::GROUP_EL);
								if (previousGroup == nullptr)
								{
									throw ComponentGenerationException(L"OPSIN bug: unable to locate group before polycylic spiro descriptor");
								}
								Element *nextGroup = OpsinTools::getNextSibling(currentSpiro, XmlDeclarations::GROUP_EL);
								if (nextGroup == nullptr)
								{
									throw ComponentGenerationException(L"OPSIN bug: unable to locate group after polycylic spiro descriptor");
								}
								Fragment *previousFrag = previousGroup->getFrag();
								Fragment *parentFrag = nextGroup->getFrag();
								FragmentTools::relabelNumericLocants(parentFrag->getAtomList(), StringTools::multiplyString(L"'",i + 1));
								elementsToResolve = OpsinTools::getSiblingsUpToElementWithTagName(currentSpiro, XmlDeclarations::POLYCYCLICSPIRO_EL);
								resolveFeaturesOntoGroup(elementsToResolve);

								std::wstring locant1 = L"";
								Element *possibleFirstLocant = OpsinTools::getPreviousSibling(currentSpiro);
								if (possibleFirstLocant != nullptr && possibleFirstLocant->getName() == XmlDeclarations::LOCANT_EL)
								{
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
									if (possibleFirstLocant->getValue().split(L",")->length == 1)
									{
										locant1 = possibleFirstLocant->getValue();
										possibleFirstLocant->detach();
									}
									else
									{
										throw ComponentGenerationException(L"Malformed locant before polycyclic spiro descriptor");
									}
								}
								Atom *atomToBeReplaced;
								if (locant1 != L"")
								{
									atomToBeReplaced = previousFrag->getAtomByLocantOrThrow(locant1);
								}
								else
								{
									std::vector<Atom*> potentialAtoms = FragmentTools::findSubstituableAtoms(previousFrag, 2);
									if (potentialAtoms.empty())
									{
										throw StructureBuildingException(L"No suitable atom found for spiro fusion");
									}
									if (AmbiguityChecker::isSubstitutionAmbiguous(potentialAtoms, 1))
									{
										state->addIsAmbiguous(std::wstring(L"Choice of atom for spiro fusion on: ") + previousGroup->getValue());
									}
									atomToBeReplaced = potentialAtoms[0];
								}
								Atom *atomOnParentFrag;
								std::wstring locant2 = L"";
								Element *possibleSecondLocant = OpsinTools::getNextSibling(currentSpiro);
								if (possibleSecondLocant != nullptr && possibleSecondLocant->getName() == XmlDeclarations::LOCANT_EL)
								{
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
									if (possibleSecondLocant->getValue().split(L",")->length == 1)
									{
										locant2 = possibleSecondLocant->getValue();
										possibleSecondLocant->detach();
									}
									else
									{
										throw ComponentGenerationException(L"Malformed locant after polycyclic spiro descriptor");
									}
								}
								if (locant2 != L"")
								{
									atomOnParentFrag = parentFrag->getAtomByLocantOrThrow(locant2);
								}
								else
								{
									std::vector<Atom*> potentialAtoms = FragmentTools::findSubstituableAtoms(parentFrag, 2);
									if (potentialAtoms.empty())
									{
										throw StructureBuildingException(L"No suitable atom found for spiro fusion");
									}
									if (AmbiguityChecker::isSubstitutionAmbiguous(potentialAtoms, 1))
									{
										state->addIsAmbiguous(std::wstring(L"Choice of atom for spiro fusion on: ") + nextGroup->getValue());
									};
									atomOnParentFrag = potentialAtoms[0];
								}
								state->fragManager->replaceAtomWithAnotherAtomPreservingConnectivity(atomToBeReplaced, atomOnParentFrag);
								if (atomToBeReplaced->hasSpareValency())
								{
									atomOnParentFrag->setSpareValency(true);
								}
								if (atomToBeReplaced->getCharge() != 0 && atomOnParentFrag->getCharge() == 0)
								{
									atomOnParentFrag->setCharge(atomToBeReplaced->getCharge());
									atomOnParentFrag->setProtonsExplicitlyAddedOrRemoved(atomToBeReplaced->getProtonsExplicitlyAddedOrRemoved());
								}
								state->fragManager->incorporateFragment(previousFrag, parentFrag);
								nextGroup->setValue(previousGroup->getValue() + currentSpiro->getValue() + nextGroup->getValue());
								previousGroup->detach();
							}
						}

						void ComponentProcessor::processSpiroBiOrTer(Element *polyCyclicSpiroDescriptor, int components) throw(ComponentGenerationException, StructureBuildingException)
						{
							Element *locant = OpsinTools::getPreviousSibling(polyCyclicSpiroDescriptor);
							std::wstring locantText;
							if (locant == nullptr || locant->getName() != XmlDeclarations::LOCANT_EL)
							{
								if (components == 2)
								{
									locantText = L"1,1'";
								}
								else
								{
									throw ComponentGenerationException(L"Unable to find locant indicating atoms to form polycyclic spiro system!");
								}
							}
							else
							{
								locantText = locant->getValue();
								locant->detach();
							}
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
							std::vector<std::wstring> locants = locantText.split(L",");
							if (locants.size() != components)
							{
								throw ComponentGenerationException(L"Mismatch between spiro descriptor and number of locants provided");
							}
							Element *group = OpsinTools::getNextSibling(polyCyclicSpiroDescriptor, XmlDeclarations::GROUP_EL);
							if (group == nullptr)
							{
								throw ComponentGenerationException(L"Cannot find group to which spirobi/ter descriptor applies");
							}

							determineFeaturesToResolveInSingleComponentSpiro(polyCyclicSpiroDescriptor);
							Fragment *fragment = group->getFrag();
							std::vector<Fragment*> clones;
							for (int i = 1; i < components ; i++)
							{
								clones.push_back(state->fragManager->copyAndRelabelFragment(fragment, i));
							}

							Atom *atomOnOriginalFragment = fragment->getAtomByLocantOrThrow(locants[0]);
							for (int i = 1; i < components ; i++)
							{
								Fragment *clone = clones[i - 1];
								Atom *atomToBeReplaced;
								if (components == 2 && !boost::ends_with(locants[i], L"'"))
								{
									//Allow prime to be (incorrectly) omitted on second locant in spirobi
									try
									{
										atomToBeReplaced = clone->getAtomByLocantOrThrow(locants[i]);
									}
									catch (const StructureBuildingException &e)
									{
										atomToBeReplaced = clone->getAtomByLocant(locants[i] + std::wstring(L"'"));
										if (atomToBeReplaced == nullptr)
										{
											throw e;
										}
									}
								}
								else
								{
									atomToBeReplaced = clone->getAtomByLocantOrThrow(locants[i]);
								}

								state->fragManager->replaceAtomWithAnotherAtomPreservingConnectivity(atomToBeReplaced, atomOnOriginalFragment);
								if (atomToBeReplaced->hasSpareValency())
								{
									atomOnOriginalFragment->setSpareValency(true);
								}
							}
							for (auto clone : clones)
							{
								state->fragManager->incorporateFragment(clone, fragment);
							}

							group->setValue(polyCyclicSpiroDescriptor->getValue() + group->getValue());
						}

						void ComponentProcessor::processDispiroter(Element *polyCyclicSpiroDescriptor) throw(StructureBuildingException, ComponentGenerationException)
						{
							std::wstring value = polyCyclicSpiroDescriptor->getValue();
							value = value.substr(0, value.length() - 10); //remove dispiroter
							value = StringTools::removeDashIfPresent(value);
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
							std::vector<std::wstring> locants = value.split(L":");
							Element *group = OpsinTools::getNextSibling(polyCyclicSpiroDescriptor, XmlDeclarations::GROUP_EL);
							if (group == nullptr)
							{
								throw ComponentGenerationException(L"Cannot find group to which dispiroter descriptor applies");
							}
							determineFeaturesToResolveInSingleComponentSpiro(polyCyclicSpiroDescriptor);
							Fragment *fragment = group->getFrag();
							std::vector<Fragment*> clones;
							for (int i = 1; i < 3 ; i++)
							{
								clones.push_back(state->fragManager->copyAndRelabelFragment(fragment, i));
							}
							for (auto clone : clones)
							{
								state->fragManager->incorporateFragment(clone, fragment);
							}

//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
							Atom *atomOnLessPrimedFragment = fragment->getAtomByLocantOrThrow(locants[0].split(L",")[0]);
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
							Atom *atomToBeReplaced = fragment->getAtomByLocantOrThrow(locants[0].split(L",")[1]);
							state->fragManager->replaceAtomWithAnotherAtomPreservingConnectivity(atomToBeReplaced, atomOnLessPrimedFragment);
							if (atomToBeReplaced->hasSpareValency())
							{
								atomOnLessPrimedFragment->setSpareValency(true);
							}

//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
							atomOnLessPrimedFragment = fragment->getAtomByLocantOrThrow(locants[1].split(L",")[0]);
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
							atomToBeReplaced = fragment->getAtomByLocantOrThrow(locants[1].split(L",")[1]);
							state->fragManager->replaceAtomWithAnotherAtomPreservingConnectivity(atomToBeReplaced, atomOnLessPrimedFragment);
							if (atomToBeReplaced->hasSpareValency())
							{
								atomOnLessPrimedFragment->setSpareValency(true);
							}

							group->setValue(std::wstring(L"dispiroter") + group->getValue());
						}

						void ComponentProcessor::determineFeaturesToResolveInSingleComponentSpiro(Element *polyCyclicSpiroDescriptor) throw(StructureBuildingException, ComponentGenerationException)
						{
							Element *possibleOpenBracket = OpsinTools::getNextSibling(polyCyclicSpiroDescriptor);
							std::vector<Element*> elementsToResolve;
							if (possibleOpenBracket->getName() == XmlDeclarations::STRUCTURALOPENBRACKET_EL)
							{
								possibleOpenBracket->detach();
								elementsToResolve = OpsinTools::getSiblingsUpToElementWithTagName(polyCyclicSpiroDescriptor, XmlDeclarations::STRUCTURALCLOSEBRACKET_EL);
								OpsinTools::getNextSibling(elementsToResolve[elementsToResolve.size() - 1])->detach(); //detach close bracket
							}
							else
							{
								elementsToResolve = OpsinTools::getSiblingsUpToElementWithTagName(polyCyclicSpiroDescriptor, XmlDeclarations::GROUP_EL);
							}
							resolveFeaturesOntoGroup(elementsToResolve);
						}

						void ComponentProcessor::resolveFeaturesOntoGroup(std::vector<Element*> &elementsToResolve) throw(StructureBuildingException, ComponentGenerationException)
						{
							if (elementsToResolve.empty())
							{
								return;
							}
							Element *substituentToResolve = new GroupingEl(XmlDeclarations::SUBSTITUENT_EL); //temporary element containing elements that should be resolved before the ring is cloned
							Element *parent = elementsToResolve[0]->getParent();
							int index = parent->find(elementsToResolve[0]);
							Element *group = nullptr;
							std::vector<Element*> suffixes;
							Element *locant = nullptr;
							for (auto element : elementsToResolve)
							{
								std::wstring elName = element->getName();
								if (elName == XmlDeclarations::GROUP_EL)
								{
									group = element;
								}
								else if (elName == XmlDeclarations::SUFFIX_EL)
								{
									suffixes.push_back(element);
								}
								else if (elName == XmlDeclarations::LOCANT_EL && group == nullptr)
								{
									locant = element;
								}
								element->detach();
								substituentToResolve->addChild(element);
							}
							if (group == nullptr)
							{
								throw ComponentGenerationException(L"OPSIN bug: group element should of been given to method");
							}
							if (locant != nullptr)
							{ //locant is probably an indirect locant, try and assign it
								std::vector<Element*> locantAble = findElementsMissingIndirectLocants(substituentToResolve, locant);
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
								std::vector<std::wstring> locantValues = locant->getValue().split(L",");
								if (locantAble.size() >= locantValues.size())
								{
									for (int i = 0; i < locantValues.size(); i++)
									{
										std::wstring locantValue = locantValues[i];
										Attribute tempVar(XmlDeclarations::LOCANT_ATR, locantValue);
										locantAble[i]->addAttribute(&tempVar);
									}
									locant->detach();
								}
							}
							if (!suffixes.empty())
							{
								suffixApplier->resolveSuffixes(group, suffixes);
								for (auto suffix : suffixes)
								{
									suffix->detach();
								}
							}
							if (substituentToResolve->getChildCount() != 0)
							{
								StructureBuildingMethods::resolveLocantedFeatures(state, substituentToResolve);
								StructureBuildingMethods::resolveUnLocantedFeatures(state, substituentToResolve);
								std::vector<Element*> children = substituentToResolve->getChildElements();
								for (int i = children.size() - 1; i >= 0; i--)
								{
									Element *child = children[i];
									child->detach();
									parent->insertChild(child, index);
								}
							}
						}

						ComponentProcessor::SortBridgesByHighestLocantedBridgehead::SortBridgesByHighestLocantedBridgehead(std::unordered_map<Fragment*, std::vector<Atom*>> &bridgeToRingAtoms) : bridgeToRingAtoms(bridgeToRingAtoms)
						{
						}

						int ComponentProcessor::SortBridgesByHighestLocantedBridgehead::compare(Fragment *bridge1, Fragment *bridge2)
						{
							std::vector<Atom*> ringAtoms1 = bridgeToRingAtoms[bridge1];
							int bridge1HighestRingLocant = std::max(getLocantNumber(ringAtoms1[0]),getLocantNumber(ringAtoms1[1]));

							std::vector<Atom*> ringAtoms2 = bridgeToRingAtoms[bridge2];
							int bridge2HighestRingLocant = std::max(getLocantNumber(ringAtoms2[0]),getLocantNumber(ringAtoms2[1]));

							if (bridge1HighestRingLocant > bridge2HighestRingLocant)
							{
								return -1;
							}
							if (bridge1HighestRingLocant < bridge2HighestRingLocant)
							{
								return 1;
							}
							return 0;
						}

						void ComponentProcessor::processFusedRingBridges(Element *subOrRoot) throw(StructureBuildingException)
						{
							std::vector<Element*> bridges = subOrRoot->getChildElements(XmlDeclarations::FUSEDRINGBRIDGE_EL);
							int bridgeCount = bridges.size();
							if (bridgeCount == 0)
							{
								return;
							}
							Element *groupEl = OpsinTools::getNextSibling(bridges[bridgeCount - 1], XmlDeclarations::GROUP_EL);
							Fragment *ringFrag = groupEl->getFrag();
							std::unordered_map<Fragment*, std::vector<Atom*>> bridgeToRingAtoms = new LinkedHashMap<Fragment*, std::vector<Atom*>>();
							for (auto bridge : bridges)
							{
								Element *possibleMultiplier = OpsinTools::getPreviousSibling(bridge);
								std::vector<std::vector<std::wstring>> locants;
								int multiplier = 1;
								if (possibleMultiplier != nullptr)
								{
									Element *possibleLocant;
									if (possibleMultiplier->getName() == XmlDeclarations::MULTIPLIER_EL)
									{
										multiplier = std::stoi(possibleMultiplier->getAttributeValue(XmlDeclarations::VALUE_ATR));
										possibleLocant = OpsinTools::getPreviousSibling(possibleMultiplier);
										possibleMultiplier->detach();
										if (possibleLocant != nullptr && possibleLocant->getName() == XmlDeclarations::COLONORSEMICOLONDELIMITEDLOCANT_EL)
										{
											locants = std::vector<std::vector<std::wstring>>();
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
											std::vector<std::wstring> locantsForEachMultiple = StringTools::removeDashIfPresent(possibleLocant->getValue()).split(L":");
											if (locantsForEachMultiple.size() != multiplier)
											{
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("Mismatch between locant and multiplier counts (" + locantsForEachMultiple.length + " and " + multiplier + "): " + possibleLocant.getValue());
												throw std::exception();
											}
											for (auto locantsForInstance : locantsForEachMultiple)
											{
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
												std::vector<std::wstring> locantArray = locantsForInstance.split(L",");
												if (locantArray.size() != 2)
												{
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("Expected two locants per bridge, but was: " + possibleLocant.getValue());
													throw std::exception();
												}
												locants.push_back(locantArray);
											}
											possibleLocant->detach();
										}
									}
									else
									{
										possibleLocant = possibleMultiplier;
										if (possibleLocant != nullptr && possibleLocant->getName() == XmlDeclarations::LOCANT_EL)
										{
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
											std::vector<std::wstring> locantArray = possibleLocant->getValue().split(L",");
											if (locantArray.size() == 2)
											{
												locants = std::vector<std::vector<std::wstring>>();
												locants.push_back(locantArray);
												possibleLocant->detach();
											}
										}
									}
								}
								for (int i = 0; i < multiplier; i++)
								{
									Fragment *bridgeFrag = state->fragManager->buildSMILES(bridge->getAttributeValue(XmlDeclarations::VALUE_ATR), groupEl, XmlDeclarations::NONE_LABELS_VAL);
									std::vector<Atom*> bridgeAtomList = bridgeFrag->getAtomList();
									bridgeFrag->addOutAtom(bridgeAtomList[0], 1, true);
									bridgeFrag->addOutAtom(bridgeAtomList[bridgeAtomList.size() - 1], 1, true);
									std::vector<Atom*> ringAtoms;
									if (locants.size() > 0)
									{
										std::vector<std::wstring> locantArray = locants[i];
										if (locantArray.size() == 2)
										{
											bridgeFrag->getOutAtom(0)->setLocant(locantArray[0]);
											bridgeFrag->getOutAtom(1)->setLocant(locantArray[1]);
										}
										ringAtoms = StructureBuildingMethods::formEpoxide(state, bridgeFrag, ringFrag->getDefaultInAtomOrFirstAtom());
									}
									else
									{
										std::vector<Atom*> possibleAtoms = FragmentTools::findSubstituableAtoms(ringFrag, 1);
										if (possibleAtoms.empty())
										{
											throw StructureBuildingException(L"Unable to find suitable atom to form bridge");
										}
										if (AmbiguityChecker::isSubstitutionAmbiguous(possibleAtoms, 1))
										{
											state->addIsAmbiguous(std::wstring(L"Addition of bridge to: ") + groupEl->getValue());
										}
										ringAtoms = StructureBuildingMethods::formEpoxide(state, bridgeFrag, possibleAtoms[0]);
									}
									bridgeToRingAtoms[bridgeFrag] = ringAtoms;
									state->fragManager->incorporateFragment(bridgeFrag, ringFrag);
								}
								bridge->detach();
							}
							int highestLocant = getHighestNumericLocant(ringFrag);
							std::vector<Fragment*> bridgeFragments(bridgeToRingAtoms.keySet());
//JAVA TO C++ CONVERTER TODO TASK: The 'Compare' parameter of std::sort produces a boolean value, while the Java Comparator parameter produces a tri-state result:
//ORIGINAL LINE: java.util.Collections.sort(bridgeFragments, new SortBridgesByHighestLocantedBridgehead(bridgeToRingAtoms));
							SortBridgesByHighestLocantedBridgehead tempVar(bridgeToRingAtoms);
							std::sort(bridgeFragments.begin(), bridgeFragments.end(), &tempVar);
							for (auto bridgeFragment : bridgeFragments)
							{
								std::vector<Atom*> bridgeFragmentAtoms = bridgeFragment->getAtomList();
								std::vector<Atom*> ringAtoms = bridgeToRingAtoms[bridgeFragment];
								if (getLocantNumber(ringAtoms[0]) <= getLocantNumber(ringAtoms[1]))
								{
									for (int i = bridgeFragmentAtoms.size() - 1; i >= 0; i--)
									{
										bridgeFragmentAtoms[i]->addLocant(std::to_wstring(++highestLocant));
									}
								}
								else
								{
									for (auto atom : bridgeFragmentAtoms)
									{
										atom->addLocant(std::to_wstring(++highestLocant));
									}
								}
							}
						}

						int ComponentProcessor::getLocantNumber(Atom *atom)
						{
							std::wstring locant = atom->getFirstLocant();
							if (locant != L"")
							{
								Matcher *m = OpsinTools::MATCH_NUMERIC_LOCANT->matcher(locant);
								if (m->matches())
								{
									return static_cast<Integer>(m->group(1));
								}
							}
							return 0;
						}

						int ComponentProcessor::getHighestNumericLocant(Fragment *ringFrag)
						{
							for (int i = 1; ; i++)
							{
								if (ringFrag->getAtomByLocant(std::to_wstring(i)) == nullptr)
								{
									return i - 1;
								}
							}
						}

						void ComponentProcessor::applyLambdaConvention(Element *subOrRoot) throw(StructureBuildingException)
						{
							std::vector<Element*> lambdaConventionEls = subOrRoot->getChildElements(XmlDeclarations::LAMBDACONVENTION_EL);
							for (auto lambdaConventionEl : lambdaConventionEls)
							{
								Fragment *frag = subOrRoot->getFirstChildElement(XmlDeclarations::GROUP_EL)->getFrag();
								if (lambdaConventionEl->getAttribute(XmlDeclarations::LOCANT_ATR) != nullptr)
								{
									frag->getAtomByLocantOrThrow(lambdaConventionEl->getAttributeValue(XmlDeclarations::LOCANT_ATR))->setLambdaConventionValency(std::stoi(lambdaConventionEl->getAttributeValue(XmlDeclarations::LAMBDA_ATR)));
								}
								else
								{
									if (frag->getAtomCount() != 1)
									{
										throw StructureBuildingException(L"Ambiguous use of lambda convention. Fragment has more than 1 atom but no locant was specified for the lambda");
									}
									frag->getFirstAtom()->setLambdaConventionValency(std::stoi(lambdaConventionEl->getAttributeValue(XmlDeclarations::LAMBDA_ATR)));
								}
								lambdaConventionEl->detach();
							}
						}

						void ComponentProcessor::handleMultiRadicals(Element *subOrRoot) throw(ComponentGenerationException, StructureBuildingException)
						{
							Element *group = subOrRoot->getFirstChildElement(XmlDeclarations::GROUP_EL);
							std::wstring groupValue = group->getValue();
							Fragment *thisFrag = group->getFrag();
							if (groupValue == L"methylene" || groupValue == L"methylen" || groupValue == L"oxy" || matchChalcogenReplacement->matcher(groupValue).matches())
							{ //resolves for example trimethylene to propan-1,3-diyl or dithio to disulfan-1,2-diyl. Locants may not be specified before the multiplier
								Element *beforeGroup = OpsinTools::getPreviousSibling(group);
								if (beforeGroup != nullptr && beforeGroup->getName() == XmlDeclarations::MULTIPLIER_ATR && beforeGroup->getAttributeValue(XmlDeclarations::TYPE_ATR) == XmlDeclarations::BASIC_TYPE_VAL && OpsinTools::getPreviousSibling(beforeGroup) == nullptr)
								{
									int multiplierVal = std::stoi(beforeGroup->getAttributeValue(XmlDeclarations::VALUE_ATR));
									if (!unsuitableForFormingChainMultiradical(group, beforeGroup))
									{
										if (groupValue == L"methylene" || groupValue == L"methylen")
										{
											group->getAttribute(XmlDeclarations::VALUE_ATR)->setValue(StringTools::multiplyString(L"C", multiplierVal));
										}
										else if (groupValue == L"oxy")
										{
											group->getAttribute(XmlDeclarations::VALUE_ATR)->setValue(StringTools::multiplyString(L"O", multiplierVal));
										}
										else if (groupValue == L"thio")
										{
											group->getAttribute(XmlDeclarations::VALUE_ATR)->setValue(StringTools::multiplyString(L"S", multiplierVal));
										}
										else if (groupValue == L"seleno")
										{
											group->getAttribute(XmlDeclarations::VALUE_ATR)->setValue(StringTools::multiplyString(L"[SeH?]", multiplierVal));
										}
										else if (groupValue == L"telluro")
										{
											group->getAttribute(XmlDeclarations::VALUE_ATR)->setValue(StringTools::multiplyString(L"[TeH?]", multiplierVal));
										}
										else
										{
											throw ComponentGenerationException(L"unexpected group value");
										}
										group->getAttribute(XmlDeclarations::OUTIDS_ATR)->setValue(std::wstring(L"1,") + std::to_wstring(std::stoi(beforeGroup->getAttributeValue(XmlDeclarations::VALUE_ATR))));
										group->setValue(beforeGroup->getValue() + groupValue);
										beforeGroup->detach();
										if (group->getAttribute(XmlDeclarations::LABELS_ATR) != nullptr)
										{ //use numeric numbering
											group->getAttribute(XmlDeclarations::LABELS_ATR)->setValue(XmlDeclarations::NUMERIC_LABELS_VAL);
										}
										else
										{
											Attribute tempVar(XmlDeclarations::LABELS_ATR, XmlDeclarations::NUMERIC_LABELS_VAL);
											group->addAttribute(&tempVar);
										}
										state->fragManager->removeFragment(thisFrag);
										thisFrag = resolveGroup(state, group);
										group->removeAttribute(group->getAttribute(XmlDeclarations::USABLEASJOINER_ATR));
									}
								}
							}

							if (group->getAttribute(XmlDeclarations::OUTIDS_ATR) != nullptr)
							{ //adds outIDs at the specified atoms
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
								std::vector<std::wstring> radicalPositions = group->getAttributeValue(XmlDeclarations::OUTIDS_ATR).split(L",");
								int firstIdInFrag = thisFrag->getIdOfFirstAtom();
								for (auto radicalID : radicalPositions)
								{
									thisFrag->addOutAtom(firstIdInFrag + std::stoi(radicalID) - 1, 1, true);
								}
							}
							int outAtomCount = thisFrag->getOutAtomCount();
							if (outAtomCount >= 2)
							{
								if (groupValue == L"amine" || groupValue == L"amin")
								{ //amine is a special case as it shouldn't technically be allowed but is allowed due to it's common usage in EDTA
									Element *previousGroup = OpsinTools::getPreviousGroup(group);
									Element *nextGroup = OpsinTools::getNextGroup(group);
									if (previousGroup == nullptr || previousGroup->getFrag()->getOutAtomCount() < 2 || nextGroup == nullptr)
									{ //must be preceded by a multi radical
										throw ComponentGenerationException(L"Invalid use of amine as a substituent!");
									}
								}
								if (state->currentWordRule == WordRule::polymer)
								{
									if (outAtomCount >= 3)
									{ //In poly mode nothing may have more than 2 outAtoms e.g. nitrilo is -N= or =N-
										int valency = 0;
										for (int i = 2; i < outAtomCount; i++)
										{
											OutAtom *nextOutAtom = thisFrag->getOutAtom(i);
											valency += nextOutAtom->getValency();
											thisFrag->removeOutAtom(nextOutAtom);
										}
										thisFrag->getOutAtom(1)->setValency(thisFrag->getOutAtom(1)->getValency() + valency);
									}
								}
							}

							if (outAtomCount == 2 && XmlDeclarations::EPOXYLIKE_SUBTYPE_VAL == group->getAttributeValue(XmlDeclarations::SUBTYPE_ATR))
							{
								Element *possibleLocant = OpsinTools::getPreviousSibling(group);
								if (possibleLocant != nullptr)
								{
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
									std::vector<std::wstring> locantValues = possibleLocant->getValue().split(L",");
									if (locantValues.size() == 2)
									{
										thisFrag->getOutAtom(0)->setLocant(locantValues[0]);
										thisFrag->getOutAtom(1)->setLocant(locantValues[1]);
										possibleLocant->detach();
										Attribute tempVar2(XmlDeclarations::LOCANT_ATR, locantValues[0]);
										subOrRoot->addAttribute(&tempVar2);
									}
								}
							}

							int totalOutAtoms = outAtomCount + calculateOutAtomsToBeAddedFromInlineSuffixes(group, subOrRoot->getChildElements(XmlDeclarations::SUFFIX_EL));
							if (totalOutAtoms >= 2)
							{
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
								Attribute tempVar3(XmlDeclarations::ISAMULTIRADICAL_ATR, Integer::toString(totalOutAtoms));
								group->addAttribute(&tempVar3);
							}
						}

						bool ComponentProcessor::unsuitableForFormingChainMultiradical(Element *group, Element *multiplierBeforeGroup)
						{
							Element *previousGroup = OpsinTools::getPreviousGroup(group);
							if (previousGroup != nullptr)
							{
								if (previousGroup->getAttribute(XmlDeclarations::ISAMULTIRADICAL_ATR) != nullptr)
								{
									if (previousGroup->getAttributeValue(XmlDeclarations::ACCEPTSADDITIVEBONDS_ATR) != L"" && OpsinTools::getPreviousSibling(previousGroup->getParent()) != nullptr)
									{
										return false;
									}
									//the initial multiplier is proceded by another multiplier e.g. bis(dithio)
									if (OpsinTools::getPrevious(multiplierBeforeGroup)->getName() == XmlDeclarations::MULTIPLIER_EL)
									{
										return false;
									}
									if (previousGroup->getAttributeValue(XmlDeclarations::ISAMULTIRADICAL_ATR) == multiplierBeforeGroup->getAttributeValue(XmlDeclarations::VALUE_ATR))
									{
										return true; //probably multiplicative
									}
									else
									{
										return false;
									}
								}
								else if (OpsinTools::getPreviousSibling(previousGroup, XmlDeclarations::MULTIPLIER_EL) == nullptr)
								{
									//This is a 99% solution to the detection of cases such as ethylidenedioxy == ethan-1,1-diyldioxy
									Fragment *previousGroupFrag = previousGroup->getFrag();
									int outAtomValency = 0;
									if (previousGroupFrag->getOutAtomCount() == 1)
									{
										outAtomValency = previousGroupFrag->getOutAtom(0)->getValency();
									}
									else
									{
										Element *suffix = OpsinTools::getNextSibling(previousGroup, XmlDeclarations::SUFFIX_EL);
										if (suffix != nullptr && suffix->getAttributeValue(XmlDeclarations::VALUE_ATR) == L"ylidene")
										{
											outAtomValency = 2;
										}
										if (suffix != nullptr && suffix->getAttributeValue(XmlDeclarations::VALUE_ATR) == L"ylidyne")
										{
											outAtomValency = 3;
										}
									}
									if (outAtomValency == std::stoi(multiplierBeforeGroup->getAttributeValue(XmlDeclarations::VALUE_ATR)))
									{
										return true;
									}
								}
							}
							return false;
						}

						int ComponentProcessor::calculateOutAtomsToBeAddedFromInlineSuffixes(Element *group, std::vector<Element*> &suffixes) throw(ComponentGenerationException)
						{
							int outAtomsThatWillBeAdded = 0;
							Fragment *frag = group->getFrag();
							std::wstring groupType = frag->getType();
							std::wstring subgroupType = frag->getSubType();
							std::wstring suffixTypeToUse = L"";
							if (suffixApplier->isGroupTypeWithSpecificSuffixRules(groupType))
							{
								suffixTypeToUse = groupType;
							}
							else
							{
								suffixTypeToUse = XmlDeclarations::STANDARDGROUP_TYPE_VAL;
							}

							std::vector<Fragment*> suffixList = state->xmlSuffixMap[group];

							for (auto suffix : suffixList)
							{
								outAtomsThatWillBeAdded += suffix->getOutAtomCount();
							}
							for (auto suffix : suffixes)
							{
								std::wstring suffixValue = suffix->getAttributeValue(XmlDeclarations::VALUE_ATR);
								std::vector<SuffixRule*> suffixRules = suffixApplier->getSuffixRuleTags(suffixTypeToUse, suffixValue, subgroupType);
								for (auto suffixRule : suffixRules)
								{
									if (suffixRule->getType() == SuffixRuleType::setOutAtom)
									{
										outAtomsThatWillBeAdded += 1;
									}
								}
							}
							return outAtomsThatWillBeAdded;
						}

						void ComponentProcessor::addImplicitBracketsToAminoAcids(std::vector<Element*> &groups, std::vector<Element*> &brackets)
						{
							for (int i = groups.size() - 1; i >= 0; i--)
							{
								Element *group = groups[i];
								if (group->getAttributeValue(XmlDeclarations::TYPE_ATR) == XmlDeclarations::AMINOACID_TYPE_VAL && OpsinTools::getNextGroup(group) != nullptr)
								{
									Element *possibleLocant = OpsinTools::getPreviousSiblingIgnoringCertainElements(group, std::vector<std::wstring>{XmlDeclarations::MULTIPLIER_EL});
									if (possibleLocant != nullptr && possibleLocant->getName() == XmlDeclarations::LOCANT_EL)
									{
										continue;
									}
									Element *subOrRoot = group->getParent();

									//now find the brackets/substituents before this element
									Element *previous = OpsinTools::getPreviousSibling(subOrRoot);
									std::vector<Element*> previousElements;
									while (previous != nullptr)
									{
										if (previous->getName() != XmlDeclarations::SUBSTITUENT_EL && previous->getName() != XmlDeclarations::BRACKET_EL)
										{
											break;
										}
										previousElements.push_back(previous);
										previous = OpsinTools::getPreviousSibling(previous);
									}
									if (previousElements.size() > 0)
									{ //an implicit bracket is needed
										std::reverse(previousElements.begin(), previousElements.end());
										Element *bracket = new GroupingEl(XmlDeclarations::BRACKET_EL);
										Attribute tempVar(XmlDeclarations::TYPE_ATR, XmlDeclarations::IMPLICIT_TYPE_VAL);
										bracket->addAttribute(&tempVar);
										Element *parent = subOrRoot->getParent();
										int indexToInsertAt = parent->find(previousElements[0]);
										for (auto element : previousElements)
										{
											element->detach();
											bracket->addChild(element);
										}

										subOrRoot->detach();
										bracket->addChild(subOrRoot);
										parent->insertChild(bracket, indexToInsertAt);
										brackets.push_back(bracket);
									}
								}
							}
						}

						void ComponentProcessor::implicitlyBracketToPreviousSubstituentIfAppropriate(Element *substituent, std::vector<Element*> &brackets) throw(ComponentGenerationException, StructureBuildingException)
						{
							std::wstring firstElInSubName = substituent->getChild(0)->getName();
							if (firstElInSubName == XmlDeclarations::LOCANT_EL || firstElInSubName == XmlDeclarations::MULTIPLIER_EL)
							{
								return;
							}

							Element *substituentGroup = substituent->getFirstChildElement(XmlDeclarations::GROUP_EL);
							//Only some substituents are valid joiners (e.g. no rings are valid joiners). Need to be atleast bivalent.
							if (substituentGroup->getAttribute(XmlDeclarations::USABLEASJOINER_ATR) == nullptr)
							{
								return;
							}

							//checks that the element before is a substituent or a bracket which will obviously include substituent/s
							//this makes sure that there would be more than more than just a substituent if a bracket is added
							Element *elementBeforeSubstituent = OpsinTools::getPreviousSibling(substituent);
							if (elementBeforeSubstituent == nullptr || elementBeforeSubstituent->getName() != XmlDeclarations::SUBSTITUENT_EL && elementBeforeSubstituent->getName() != XmlDeclarations::BRACKET_EL)
							{
								return;
							}

							Element *elementAftersubstituent = OpsinTools::getNextSibling(substituent);
							if (elementAftersubstituent != nullptr)
							{
								//Not preceded and followed by a bracket e.g. Not (benzyl)methyl(phenyl)amine	c.f. P-16.4.1.3 (draft 2004)
								if (elementBeforeSubstituent->getName() == XmlDeclarations::BRACKET_EL && XmlDeclarations::IMPLICIT_TYPE_VAL != elementBeforeSubstituent->getAttributeValue(XmlDeclarations::TYPE_ATR) && elementAftersubstituent->getName() == XmlDeclarations::BRACKET_EL)
								{
									Element *firstChildElementOfElementAfterSubstituent = elementAftersubstituent->getChild(0);
									if ((firstChildElementOfElementAfterSubstituent->getName() == XmlDeclarations::SUBSTITUENT_EL || firstChildElementOfElementAfterSubstituent->getName() == XmlDeclarations::BRACKET_EL) && OpsinTools::getPrevious(firstChildElementOfElementAfterSubstituent)->getName() != XmlDeclarations::HYPHEN_EL)
									{
										return;
									}
								}
							}
							//there must be an element after the substituent (or the substituent is being used for locanted ester formation) for the implicit bracket to be required
							if (!isSubBracketOrRoot(elementAftersubstituent))
							{
								if (!(elementAftersubstituent == nullptr && locantedEsterImplicitBracketSpecialCase(substituent, elementBeforeSubstituent)))
								{
									return;
								}
							}

							//look for hyphen between substituents, this seems to indicate implicit bracketing was not desired e.g. dimethylaminomethane vs dimethyl-aminomethane
							//an exception is made for groups like carbonyl/sulfonyl as these typically should be implicitly bracketed e.g. tert-butoxy-carbonyl
							Element *elementDirectlyBeforeSubstituent = OpsinTools::getPrevious(substituent->getChild(0)); //can't return null as we know elementBeforeSubstituent is not null
							if (elementDirectlyBeforeSubstituent->getName() == XmlDeclarations::HYPHEN_EL && !matchGroupsThatAreAlsoInlineSuffixes->matcher(substituentGroup->getValue()).matches())
							{
								return;
							}

							Fragment *frag = substituentGroup->getFrag();
							std::wstring theSubstituentSubType = substituentGroup->getAttributeValue(XmlDeclarations::SUBTYPE_ATR);
							std::wstring theSubstituentType = substituentGroup->getAttributeValue(XmlDeclarations::TYPE_ATR);

							//prevents alkyl chains being bracketed together e.g. ethylmethylamine
							//...unless it's something like 2-methylethyl where the first appears to be locanted onto the second
							std::vector<Element*> groupElements = OpsinTools::getDescendantElementsWithTagName(elementBeforeSubstituent, XmlDeclarations::GROUP_EL); //one for a substituent, possibly more for a bracket
							Element *lastGroupOfElementBeforeSub = groupElements[groupElements.size() - 1];
							if (lastGroupOfElementBeforeSub == nullptr)
							{
								throw ComponentGenerationException(L"No group where group was expected");
							}
							if (theSubstituentType == XmlDeclarations::CHAIN_TYPE_VAL && theSubstituentSubType == XmlDeclarations::ALKANESTEM_SUBTYPE_VAL && lastGroupOfElementBeforeSub->getAttributeValue(XmlDeclarations::TYPE_ATR) == XmlDeclarations::CHAIN_TYPE_VAL && lastGroupOfElementBeforeSub->getAttributeValue(XmlDeclarations::SUBTYPE_ATR) == XmlDeclarations::ALKANESTEM_SUBTYPE_VAL)
							{
								bool placeInImplicitBracket = false;

								Element *suffixAfterGroup = OpsinTools::getNextSibling(lastGroupOfElementBeforeSub, XmlDeclarations::SUFFIX_EL);
								//if the alkane ends in oxy, sulfinyl, sulfonyl etc. it's not a pure alkane
								//the outatom check rules out things like "oyl" which don't extend the chain
								if (suffixAfterGroup != nullptr && suffixAfterGroup->getFrag() != nullptr && suffixAfterGroup->getFrag()->getOutAtomCount() > 0)
								{
									placeInImplicitBracket = true;
								}
								//look for locants and check whether they appear to be referring to the other chain
								if (!placeInImplicitBracket)
								{
									std::vector<Element*> childrenOfElementBeforeSubstituent = elementBeforeSubstituent->getChildElements();
									boost::optional<bool> foundLocantNotReferringToChain = boost::none;
									for (auto childOfElBeforeSub : childrenOfElementBeforeSubstituent)
									{
										std::wstring currentElementName = childOfElBeforeSub->getName();
										if (currentElementName == XmlDeclarations::LOCANT_EL)
										{
											std::wstring locantText = childOfElBeforeSub->getValue();
											if (!frag->hasLocant(locantText))
											{
												foundLocantNotReferringToChain = true;
												break;
											}
											else
											{
												foundLocantNotReferringToChain = false;
											}
										}
										else if (currentElementName == XmlDeclarations::STEREOCHEMISTRY_EL)
										{
										}
										else
										{
											break;
										}
									}
									if (foundLocantNotReferringToChain && !foundLocantNotReferringToChain)
									{ //a locant was found and it appeared to refer to the other chain
										placeInImplicitBracket = true;
									}
								}
								if (!placeInImplicitBracket)
								{
									return;
								}
							}

							//prevent bracketing to multi radicals unless through substitution they are likely to cease being multiradicals
							if (lastGroupOfElementBeforeSub->getAttribute(XmlDeclarations::ISAMULTIRADICAL_ATR) != nullptr && lastGroupOfElementBeforeSub->getAttribute(XmlDeclarations::ACCEPTSADDITIVEBONDS_ATR) == nullptr && lastGroupOfElementBeforeSub->getAttribute(XmlDeclarations::IMINOLIKE_ATR) == nullptr)
							{
								return;
							}
							if (substituentGroup->getAttribute(XmlDeclarations::ISAMULTIRADICAL_ATR) != nullptr)
							{
								if (substituentGroup->getAttribute(XmlDeclarations::ACCEPTSADDITIVEBONDS_ATR) == nullptr && substituentGroup->getAttribute(XmlDeclarations::IMINOLIKE_ATR) == nullptr)
								{
									//after implicit bracketting the substituent should no longer be a multi-radical. If neither of the above attributes apply this can't happen
									return;
								}
								//being not substitutable doesn't mean it can't form additive bonds cf. oxy. Additive bonds can still benefit from implicit bracketing
								bool isSubstitutable = false;
								for (auto atom : frag->getAtomList())
								{
									if (StructureBuildingMethods::calculateSubstitutableHydrogenAtoms(atom) > 0)
									{
										isSubstitutable = true;
										break;
									}
								}
								if (!isSubstitutable && elementAftersubstituent != nullptr && elementAftersubstituent->getChild(0)->getName() == XmlDeclarations::MULTIPLIER_EL)
								{
									//return if multiplicative nomenclature detected, if the multiplier differs from the out atom count, additive bonds may still be possible
									if (frag->getOutAtomCount() == std::stoi(elementAftersubstituent->getChild(0)->getAttributeValue(XmlDeclarations::VALUE_ATR)))
									{
										std::wstring elType = elementAftersubstituent->getName();
										if (elType == XmlDeclarations::ROOT_EL)
										{
											return;
										}
										else if (elType == XmlDeclarations::SUBSTITUENT_EL)
										{
											std::vector<Element*> groups = OpsinTools::getDescendantElementsWithTagName(elementAftersubstituent, XmlDeclarations::GROUP_EL);
											for (auto group : groups)
											{
												if (group->getAttribute(XmlDeclarations::ISAMULTIRADICAL_ATR) != nullptr)
												{
													return; //a multi radical
												}
											}
										}
										else if (elType == XmlDeclarations::BRACKET_EL && OpsinTools::getDescendantElementsWithTagName(elementAftersubstituent, XmlDeclarations::ROOT_EL).size() > 0)
										{
											return;
										}
									}
								}
							}
							if (lastGroupOfElementBeforeSub->getAttribute(XmlDeclarations::IMINOLIKE_ATR) != nullptr && substituentGroup->getAttribute(XmlDeclarations::IMINOLIKE_ATR) != nullptr)
							{
								return; //possibly a multiplicative additive operation
							}

							if (implicitBracketWouldPreventAdditiveBonding(elementBeforeSubstituent, elementAftersubstituent))
							{
								return; //e.g. N-ethylmethylsulfonimidoyl
							}

							if (substituentGroup->getValue() == L"sulf" && frag->getAtomCount() == 1)
							{
								Element *suffix = OpsinTools::getNextSiblingIgnoringCertainElements(substituentGroup, std::vector<std::wstring>{XmlDeclarations::UNSATURATOR_EL});
								if (suffix != nullptr && suffix->getAttributeValue(XmlDeclarations::VALUE_ATR) == L"ylidene")
								{
									substituentGroup->removeAttribute(substituentGroup->getAttribute(XmlDeclarations::USABLEASJOINER_ATR));
									//TODO resolve suffixes as early as can be done unambiguously
									//e.g. it should be possible to know that sulfanylidene has 0 hydrogen but azaniumylidyne has 1
									return;
								}
							}

							//prevent bracketing perhalogeno terms 
							if (XmlDeclarations::PERHALOGENO_SUBTYPE_VAL == lastGroupOfElementBeforeSub->getAttributeValue(XmlDeclarations::SUBTYPE_ATR))
							{
								return;
							}

							/*
							 * locant may need to be moved. This occurs when the group in elementBeforeSubstituent is not supposed to be locanted onto
							 *  theSubstituentGroup
							 *  e.g. 2-aminomethyl-1-chlorobenzene where the 2 refers to the benzene NOT the methyl
							 */
							std::vector<Element*> locantRelatedElements; //sometimes moved
							std::vector<std::wstring> locantValues;
							std::vector<Element*> stereoChemistryElements; //always moved if bracketing occurs
							std::vector<Element*> childrenOfElementBeforeSubstituent = elementBeforeSubstituent->getChildElements();
							for (auto childOfElBeforeSub : childrenOfElementBeforeSubstituent)
							{
								std::wstring currentElementName = childOfElBeforeSub->getName();
								if (currentElementName == XmlDeclarations::STEREOCHEMISTRY_EL)
								{
									stereoChemistryElements.push_back(childOfElBeforeSub);
								}
								else if (currentElementName == XmlDeclarations::LOCANT_EL)
								{
									if (locantValues.size() > 0)
									{
										break;
									}
									locantRelatedElements.push_back(childOfElBeforeSub);
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
									locantValues = childOfElBeforeSub->getValue().split(L",");
								}
								else
								{
									break;
								}
							}

							//either all locants will be moved, or none
							bool moveLocants = false;
							if (locantValues.size() > 0)
							{
								Element *elAfterLocant = OpsinTools::getNextSibling(locantRelatedElements[0]);
								for (auto locantText : locantValues)
								{
									//Check the right fragment in the bracket:
									//if it only has 1 then assume locanted substitution onto it not intended. Or if doesn't have the required locant
									if (frag->getAtomCount() == 1 || !frag->hasLocant(locantText) || matchElementSymbolOrAminoAcidLocant->matcher(locantText).find() || (locantValues.size() == 1 && elAfterLocant->getName() == XmlDeclarations::MULTIPLIER_EL))
									{
										if (checkLocantPresentOnPotentialRoot(state, substituent, locantText))
										{
											moveLocants = true; //locant location is present elsewhere
											break;
										}
										else
										{
											if (frag->getAtomCount() == 1 && frag->hasLocant(locantText))
											{
												//1 locant was intended to locant onto fragment with 1 atom
											}
											else
											{
												moveLocants = true; //the fragment adjacent to the locant doesn't have this locant or doesn't need any indirect locants. Assume it will appear elsewhere later
												break;
											}
										}
									}
								}

								if (moveLocants && locantValues.size() > 1)
								{
									if (elAfterLocant != nullptr && elAfterLocant->getName() == XmlDeclarations::MULTIPLIER_EL)
									{
										Element *shouldBeAGroupOrSubOrBracket = OpsinTools::getNextSiblingIgnoringCertainElements(elAfterLocant, std::vector<std::wstring>{XmlDeclarations::MULTIPLIER_EL});
										if (shouldBeAGroupOrSubOrBracket != nullptr)
										{
											if ((shouldBeAGroupOrSubOrBracket->getName() == XmlDeclarations::GROUP_EL && elAfterLocant->getAttributeValue(XmlDeclarations::TYPE_ATR) == XmlDeclarations::GROUP_TYPE_VAL) || (matchGroupsThatAreAlsoInlineSuffixes->matcher(substituentGroup->getValue()).matches()))
											{ //e.g. 4,4'-dimethoxycarbonyl-2,2'-bioxazole --> 4,4'-di(methoxycarbonyl)-2,2'-bioxazole - e.g. 2,5-bisaminothiobenzene --> 2,5-bis(aminothio)benzene
												locantRelatedElements.push_back(elAfterLocant); //e.g. 1,5-bis-(4-methylphenyl)sulfonyl --> 1,5-bis-((4-methylphenyl)sulfonyl)
											}
											else if (XmlDeclarations::ORTHOMETAPARA_TYPE_VAL == locantRelatedElements[0]->getAttributeValue(XmlDeclarations::TYPE_ATR))
											{ //e.g. p-dimethylamino[ring]
												locantRelatedElements[0]->setValue(locantValues[1]);
											}
											else if (frag->getAtomCount() == 1)
											{ //e.g. 1,3,4-trimethylthiobenzene --> 1,3,4-tri(methylthio)benzene
												locantRelatedElements.push_back(elAfterLocant);
											}
											else
											{ //don't bracket other complex multiplied substituents (name hasn't given enough hints if indeed bracketing was expected)
												return;
											}
										}
										else
										{
											moveLocants = false;
										}
									}
									else
									{
										moveLocants = false;
									}
								}
							}

							Element *bracket = new GroupingEl(XmlDeclarations::BRACKET_EL);
							Attribute tempVar(XmlDeclarations::TYPE_ATR, XmlDeclarations::IMPLICIT_TYPE_VAL);
							bracket->addAttribute(&tempVar);

							for (auto stereoChemistryElement : stereoChemistryElements)
							{
								stereoChemistryElement->detach();
								bracket->addChild(stereoChemistryElement);
							}
							if (moveLocants)
							{
								for (auto locantElement : locantRelatedElements)
								{
									locantElement->detach();
									bracket->addChild(locantElement);
								}
							}

							/*
							 * Case when a multiplier should be moved
							 * e.g. tripropan-2-yloxyphosphane -->tri(propan-2-yloxy)phosphane or trispropan-2-ylaminophosphane --> tris(propan-2-ylamino)phosphane
							 */
							if (locantRelatedElements.empty())
							{
								Element *possibleMultiplier = childrenOfElementBeforeSubstituent[0];
								if (possibleMultiplier->getName() == XmlDeclarations::MULTIPLIER_EL && (matchGroupsThatAreAlsoInlineSuffixes->matcher(substituentGroup->getValue()).matches() || possibleMultiplier->getAttributeValue(XmlDeclarations::TYPE_ATR) == XmlDeclarations::GROUP_TYPE_VAL))
								{
									Element *desiredGroup = OpsinTools::getNextSiblingIgnoringCertainElements(possibleMultiplier, std::vector<std::wstring>{XmlDeclarations::MULTIPLIER_EL});
									if (desiredGroup != nullptr && desiredGroup->getName() == XmlDeclarations::GROUP_EL)
									{
										possibleMultiplier->detach();
										bracket->addChild(possibleMultiplier);
									}
								}
							}

							Element *parent = substituent->getParent();
							int startIndex = parent->find(elementBeforeSubstituent);
							int endIndex = parent->find(substituent);
							for (int i = 0 ; i <= (endIndex - startIndex); i++)
							{
								Element *n = parent->getChild(startIndex);
								n->detach();
								bracket->addChild(n);
							}
							parent->insertChild(bracket, startIndex);
							brackets.push_back(bracket);
						}

						bool ComponentProcessor::implicitBracketWouldPreventAdditiveBonding(Element *elementBeforeSubstituent, Element *elementAftersubstituent)
						{
							if (elementAftersubstituent != nullptr && elementAftersubstituent->getName() == XmlDeclarations::SUBSTITUENT_EL)
							{
								Element *groupAfterSubstituent = elementAftersubstituent->getFirstChildElement(XmlDeclarations::GROUP_EL);
								if (groupAfterSubstituent->getAttribute(XmlDeclarations::ACCEPTSADDITIVEBONDS_ATR) != nullptr && !isSubBracketOrRoot(OpsinTools::getNextSibling(elementAftersubstituent)))
								{
									if (elementBeforeSubstituent->getChild(0)->getName() == XmlDeclarations::LOCANT_EL)
									{
										Fragment *additiveAcceptingFrag = groupAfterSubstituent->getFrag();
										Element *viableSubstituent = elementBeforeSubstituent;
										while (viableSubstituent != nullptr)
										{
											if (viableSubstituent->getName() == XmlDeclarations::SUBSTITUENT_EL || viableSubstituent->getName() == XmlDeclarations::BRACKET_EL)
											{
												Element *possibleLocant = viableSubstituent->getChild(0);
												if (possibleLocant->getName() == XmlDeclarations::LOCANT_EL)
												{
													if (additiveAcceptingFrag->getFirstAtom()->equals(additiveAcceptingFrag->getAtomByLocant(possibleLocant->getValue())))
													{
														return false;
													}
												}
											}
											viableSubstituent = OpsinTools::getPreviousSibling(viableSubstituent);
										}
										return true;
									}
								}
							}
							return false;
						}

						bool ComponentProcessor::locantedEsterImplicitBracketSpecialCase(Element *substituent, Element *elementBeforeSubstituent)
						{
							if (substituent->getParent()->getName() == XmlDeclarations::WORD_EL && OpsinTools::getPreviousSibling(elementBeforeSubstituent) == nullptr && (state->currentWordRule == WordRule::ester || state->currentWordRule == WordRule::functionalClassEster || state->currentWordRule == WordRule::multiEster || state->currentWordRule == WordRule::acetal))
							{
								return true;
							}
							return false;
						}

						void ComponentProcessor::matchLocantsToIndirectFeatures(Element *subOrRoot) throw(StructureBuildingException)
						{
							/* Root fragments (or the root in a bracket) can have prefix-locants
							 * that work on suffixes - (2-furyl), 2-propanol, (2-propylmethyl), (2-propyloxy), 2'-Butyronaphthone.
							 */
							std::vector<Element*> locantEls = findLocantsThatCouldBeIndirectLocants(subOrRoot);

							if (locantEls.size() > 0)
							{
								Element *group = subOrRoot->getFirstChildElement(XmlDeclarations::GROUP_EL);
								Element *lastLocant = locantEls[locantEls.size() - 1]; //the locant that may apply to an unsaturator/suffix
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
								std::vector<std::wstring> locantValues = lastLocant->getValue().split(L",");
								if (locantValues.size() == 1 && group->getAttribute(XmlDeclarations::FRONTLOCANTSEXPECTED_ATR) != nullptr)
								{ //some trivial retained names like 2-furyl expect locants to be in front of them. For these the indirect intepretation will always be used rather than checking whether 2-(furyl) even makes sense
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
									std::vector<std::wstring> allowedLocants = group->getAttributeValue(XmlDeclarations::FRONTLOCANTSEXPECTED_ATR).split(L",");
									for (auto allowedLocant : allowedLocants)
									{
										if (locantValues[0] == allowedLocant)
										{
											Element *expectedSuffix = OpsinTools::getNextSibling(group);
											if (expectedSuffix != nullptr && expectedSuffix->getName() == XmlDeclarations::SUFFIX_EL && expectedSuffix->getAttribute(XmlDeclarations::LOCANT_ATR) == nullptr)
											{
												Attribute tempVar(XmlDeclarations::LOCANT_ATR, locantValues[0]);
												expectedSuffix->addAttribute(&tempVar);
												lastLocant->detach();
												return;
											}
											break;
										}
									}
								}
								bool allowIndirectLocants = true;
								if (state->currentWordRule == WordRule::multiEster && XmlDeclarations::ADDEDHYDROGENLOCANT_TYPE_VAL != lastLocant->getAttributeValue(XmlDeclarations::TYPE_ATR))
								{ //special case e.g. 1-benzyl 4-butyl terephthalate (locants do not apply to yls)
									Element *parentEl = subOrRoot->getParent();
									if (parentEl->getName() == XmlDeclarations::WORD_EL && parentEl->getAttributeValue(XmlDeclarations::TYPE_ATR) == XmlDeclarations::SUBSTITUENT_EL && parentEl->getChildCount() == 1 && locantValues.size() == 1 && XmlDeclarations::ORTHOMETAPARA_TYPE_VAL != lastLocant->getAttributeValue(XmlDeclarations::TYPE_ATR))
									{
										allowIndirectLocants = false;
									}
								}
								Fragment *fragmentAfterLocant = group->getFrag();
								if (fragmentAfterLocant->getAtomCount() <= 1)
								{
									allowIndirectLocants = false; //e.g. prevent 1-methyl as meth-1-yl is extremely unlikely to be the intended result
								}

								if (allowIndirectLocants)
								{
									/* The first locant is most likely a locant indicating where this subsituent should be attached.
									 * If the locant cannot be found on a potential root this cannot be the case though (assuming the name is valid of course)
									 */
									if (XmlDeclarations::ADDEDHYDROGENLOCANT_TYPE_VAL != lastLocant->getAttributeValue(XmlDeclarations::TYPE_ATR) && locantEls.size() == 1 && group->getAttribute(XmlDeclarations::ISAMULTIRADICAL_ATR) == nullptr && locantValues.size() == 1 && checkLocantPresentOnPotentialRoot(state, subOrRoot, locantValues[0]) && OpsinTools::getPreviousSibling(lastLocant, XmlDeclarations::LOCANT_EL) == nullptr)
									{
										return;
									}
									bool assignableToIndirectFeatures = true;
									std::vector<Element*> locantAble = findElementsMissingIndirectLocants(subOrRoot, lastLocant);
									if (locantAble.size() < locantValues.size())
									{
										assignableToIndirectFeatures = false;
									}
									else
									{
										for (auto locantValue : locantValues)
										{
											if (!fragmentAfterLocant->hasLocant(locantValue))
											{ //locant is not available on the group adjacent to the locant!
												assignableToIndirectFeatures = false;
											}
										}
									}

									if (!assignableToIndirectFeatures)
									{ //usually indicates the name will fail unless the suffix has the locant or heteroatom replacement will create the locant
										if (locantValues.size() == 1)
										{
											std::vector<Fragment*> suffixes = state->xmlSuffixMap[group];
											//I do not want to assign element locants as in locants on the suffix as I currently know of no examples where this actually occurs
											if (matchElementSymbolOrAminoAcidLocant->matcher(locantValues[0]).matches())
											{
												return;
											}
											for (auto suffix : suffixes)
											{
												if (suffix->hasLocant(locantValues[0]))
												{ //e.g. 2'-Butyronaphthone
													Atom *dummyRAtom = suffix->getFirstAtom();
													std::vector<Atom*> neighbours = dummyRAtom->getAtomNeighbours();
													Bond *b = nullptr;
													for (auto atom : neighbours)
													{
														std::vector<std::wstring> neighbourLocants = atom->getLocants();
														for (auto neighbourLocant : neighbourLocants)
														{
															if (OpsinTools::MATCH_NUMERIC_LOCANT->matcher(neighbourLocant).matches())
															{
																b = dummyRAtom->getBondToAtomOrThrow(atom);
																goto atomLoopBreak;
															}
														}
														atomLoopContinue:
													}
													atomLoopBreak:
													if (b != nullptr)
													{
														state->fragManager->removeBond(b); //the current bond between the dummy R and the suffix
														state->fragManager->createBond(dummyRAtom, suffix->getAtomByLocantOrThrow(locantValues[0]), b->getOrder());
														lastLocant->detach();
													}
												}
											}
										}
									}
									else
									{
										for (int i = 0; i < locantValues.size(); i++)
										{
											std::wstring locantValue = locantValues[i];
											Attribute tempVar2(XmlDeclarations::LOCANT_ATR, locantValue);
											locantAble[i]->addAttribute(&tempVar2);
										}
										lastLocant->detach();
									}
								}
							}
						}

						std::vector<Element*> ComponentProcessor::findLocantsThatCouldBeIndirectLocants(Element *subOrRoot)
						{
							std::vector<Element*> children = subOrRoot->getChildElements();
							std::vector<Element*> locantEls;
							for (auto el : children)
							{
								if (el->getName().equals(XmlDeclarations::LOCANT_EL))
								{
									Element *afterLocant = OpsinTools::getNextSibling(el);
									if (afterLocant != nullptr && afterLocant->getName() == XmlDeclarations::MULTIPLIER_EL)
									{ //locant should not be followed by a multiplier. c.f. 1,2,3-tributyl 2-acetyloxypropane-1,2,3-tricarboxylate
										continue;
									}
									locantEls.push_back(el);
								}
								else if (el->getName().equals(XmlDeclarations::GROUP_EL))
								{
									break;
								}
							}
							return locantEls;
						}

						std::vector<Element*> ComponentProcessor::findElementsMissingIndirectLocants(Element *subOrRoot, Element *locantEl)
						{
							std::vector<Element*> locantAble;
							std::vector<Element*> childrenOfSubOrBracketOrRoot = subOrRoot->getChildElements();
							for (auto el : childrenOfSubOrBracketOrRoot)
							{
								std::wstring name = el->getName();
								if (name == XmlDeclarations::SUFFIX_EL || name == XmlDeclarations::UNSATURATOR_EL || name == XmlDeclarations::CONJUNCTIVESUFFIXGROUP_EL)
								{
									if (el->getAttribute(XmlDeclarations::LOCANT_ATR) == nullptr && el->getAttribute(XmlDeclarations::LOCANTID_ATR) == nullptr && el->getAttribute(XmlDeclarations::MULTIPLIED_ATR) == nullptr)
									{ // shouldn't already have a locant or be multiplied (should of already had locants assignd to it if that were the case)
										if (subOrRoot->find(el) > subOrRoot->find(locantEl))
										{
											if (name == XmlDeclarations::SUFFIX_EL)
											{ //check a few special cases that must not be locanted
												Element *group = OpsinTools::getPreviousSibling(el, XmlDeclarations::GROUP_EL);
												std::wstring type = group->getAttributeValue(XmlDeclarations::TYPE_ATR);
												if ((type == XmlDeclarations::ACIDSTEM_TYPE_VAL && XmlDeclarations::CYCLEFORMER_SUBTYPE_VAL != el->getAttributeValue(XmlDeclarations::SUBTYPE_ATR)) || type == XmlDeclarations::NONCARBOXYLICACID_TYPE_VAL || type == XmlDeclarations::CHALCOGENACIDSTEM_TYPE_VAL)
												{
													continue;
												}
											}
											locantAble.push_back(el);
										}
									}
								}
							}
							return locantAble;
						}

						void ComponentProcessor::assignImplicitLocantsToDiTerminalSuffixes(Element *subOrRoot) throw(StructureBuildingException)
						{
							Element *terminalSuffix1 = subOrRoot->getFirstChildElement(XmlDeclarations::SUFFIX_EL);
							if (terminalSuffix1 != nullptr)
							{
								if (isATerminalSuffix(terminalSuffix1) && OpsinTools::getNextSibling(terminalSuffix1) != nullptr)
								{
									Element *terminalSuffix2 = OpsinTools::getNextSibling(terminalSuffix1);
									if (isATerminalSuffix(terminalSuffix2))
									{
										Element *hopefullyAChain = OpsinTools::getPreviousSibling(terminalSuffix1, XmlDeclarations::GROUP_EL);
										if (hopefullyAChain != nullptr && hopefullyAChain->getAttributeValue(XmlDeclarations::TYPE_ATR) == XmlDeclarations::CHAIN_TYPE_VAL)
										{
											int chainLength = hopefullyAChain->getFrag()->getChainLength();
											if (chainLength >= 2)
											{
												Attribute tempVar(XmlDeclarations::LOCANT_ATR, L"1");
												terminalSuffix1->addAttribute(&tempVar);
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
												Attribute tempVar2(XmlDeclarations::LOCANT_ATR, Integer::toString(chainLength));
												terminalSuffix2->addAttribute(&tempVar2);
											}
										}
									}
								}
							}
						}

						bool ComponentProcessor::isATerminalSuffix(Element *suffix)
						{
							return suffix->getName() == XmlDeclarations::SUFFIX_EL && suffix->getAttribute(XmlDeclarations::LOCANT_ATR) == nullptr && (suffix->getAttributeValue(XmlDeclarations::TYPE_ATR) == XmlDeclarations::INLINE_TYPE_VAL || XmlDeclarations::TERMINAL_SUBTYPE_VAL == suffix->getAttributeValue(XmlDeclarations::SUBTYPE_ATR));
						}

						void ComponentProcessor::processConjunctiveNomenclature(Element *subOrRoot) throw(ComponentGenerationException, StructureBuildingException)
						{
							std::vector<Element*> conjunctiveGroups = subOrRoot->getChildElements(XmlDeclarations::CONJUNCTIVESUFFIXGROUP_EL);
							int conjunctiveGroupCount = conjunctiveGroups.size();
							if (conjunctiveGroupCount > 0)
							{
								Element *ringGroup = subOrRoot->getFirstChildElement(XmlDeclarations::GROUP_EL);
								Fragment *ringFrag = ringGroup->getFrag();
								if (ringFrag->getOutAtomCount() != 0)
								{
									throw ComponentGenerationException(L"OPSIN Bug: Ring fragment should have no radicals");
								}
								for (int i = 0; i < conjunctiveGroupCount; i++)
								{
									Element *conjunctiveGroup = conjunctiveGroups[i];
									Fragment *conjunctiveFragment = conjunctiveGroup->getFrag();
									std::wstring locant = conjunctiveGroup->getAttributeValue(XmlDeclarations::LOCANT_ATR);
									Atom *atomToConnectToOnConjunctiveFrag = FragmentTools::lastNonSuffixCarbonWithSufficientValency(conjunctiveFragment);
									if (atomToConnectToOnConjunctiveFrag == nullptr)
									{
										throw ComponentGenerationException(L"OPSIN Bug: Unable to find non suffix carbon with sufficient valency");
									}
									if (locant != L"")
									{
										state->fragManager->createBond(atomToConnectToOnConjunctiveFrag, ringFrag->getAtomByLocantOrThrow(locant), 1);
									}
									else
									{
										std::vector<Atom*> possibleAtoms = FragmentTools::findSubstituableAtoms(ringFrag, 1);
										if (possibleAtoms.empty())
										{
											throw StructureBuildingException(L"No suitable atom found for conjunctive operation");
										}
										if (AmbiguityChecker::isSubstitutionAmbiguous(possibleAtoms, 1))
										{
											state->addIsAmbiguous(std::wstring(L"Connection of conjunctive group to: ") + ringGroup->getValue());
										}
										state->fragManager->createBond(atomToConnectToOnConjunctiveFrag, possibleAtoms[0], 1);
									}
									state->fragManager->incorporateFragment(conjunctiveFragment, ringFrag);
								}
							}
						}

						void ComponentProcessor::processBiochemicalLinkageDescriptors(std::vector<Element*> &substituents, std::vector<Element*> &brackets) throw(StructureBuildingException)
						{
							for (auto substituent : substituents)
							{
								std::vector<Element*> bioLinkLocants = substituent->getChildElements(XmlDeclarations::BIOCHEMICALLINKAGE_EL);
								if (bioLinkLocants.size() > 0)
								{
									if (bioLinkLocants.size() > 1)
									{
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("OPSIN Bug: More than 1 biochemical linkage locant associated with subsituent");
										throw std::exception();
									}
									Element *bioLinkLocant = bioLinkLocants[0];
									std::wstring bioLinkLocantStr = bioLinkLocant->getValue();
									bioLinkLocantStr = bioLinkLocantStr.substr(1, (bioLinkLocantStr.length() - 1) - 1); //strip brackets
									checkAndApplyFirstLocantOfBiochemicalLinkage(substituent, bioLinkLocantStr);
									int secondLocantStartPos = std::max((int)bioLinkLocantStr.rfind(L'>'), (int)bioLinkLocantStr.rfind(L'-')) + 1;
									std::wstring locantToConnectTo = bioLinkLocantStr.substr(secondLocantStartPos);
									Element *parent = substituent->getParent();
									Attribute *locantAtr = new Attribute(XmlDeclarations::LOCANT_ATR, std::wstring(L"O") + locantToConnectTo);

									Element *elementAfterSubstituent = OpsinTools::getNextSibling(substituent);
									bool hasAdjacentGroupToSubstitute = isSubBracketOrRoot(elementAfterSubstituent);


									/* If a biochemical is not at the end of a scope but is preceded by substituents/brackets
									 * these are bracketted and the locant assigned to the bracket.
									 * Else If the group is the only thing in a bracket the locant is assigned to the bracket (this is used to describe branches)
									 * Else the locant is assigned to the substituent
									 */
									bool bracketAdded = false;
									if (hasAdjacentGroupToSubstitute)
									{
										//now find the brackets/substituents before this element
										Element *previous = OpsinTools::getPreviousSibling(substituent);
										std::vector<Element*> previousElements;
										while (previous != nullptr)
										{
											if (previous->getName() != XmlDeclarations::SUBSTITUENT_EL && previous->getName() != XmlDeclarations::BRACKET_EL)
											{
												break;
											}
											previousElements.push_back(previous);
											previous = OpsinTools::getPreviousSibling(previous);
										}
										if (previousElements.size() > 0)
										{ //an explicit bracket is needed
											std::reverse(previousElements.begin(), previousElements.end());
											Element *bracket = new GroupingEl(XmlDeclarations::BRACKET_EL);
											bracket->addAttribute(locantAtr);
											int indexToInsertAt = parent->find(previousElements[0]);
											for (auto element : previousElements)
											{
												element->detach();
												bracket->addChild(element);
											}

											substituent->detach();
											bracket->addChild(substituent);
											parent->insertChild(bracket, indexToInsertAt);
											brackets.push_back(bracket);
											bracketAdded = true;
											if (substituent->getAttribute(XmlDeclarations::LOCANT_ATR) != nullptr)
											{
												throw StructureBuildingException(std::wstring(L"Substituent with biochemical linkage descriptor should not also have a locant: ") + substituent->getAttributeValue(XmlDeclarations::LOCANT_ATR));
											}
										}
									}

									if (!bracketAdded)
									{
										Element *elToAddAtrTo;
										if (parent->getName() == XmlDeclarations::BRACKET_EL && !hasAdjacentGroupToSubstitute)
										{
											elToAddAtrTo = parent;
										}
										else
										{
											elToAddAtrTo = substituent;
										}
										if (elToAddAtrTo->getAttribute(XmlDeclarations::LOCANT_ATR) != nullptr)
										{
											throw StructureBuildingException(std::wstring(L"Substituent with biochemical linkage descriptor should not also have a locant: ") + elToAddAtrTo->getAttributeValue(XmlDeclarations::LOCANT_ATR));
										}
										elToAddAtrTo->addAttribute(locantAtr);
									}
									bioLinkLocant->detach();
								}
							}

							for (auto bracket : brackets)
							{
								std::vector<Element*> bioLinkLocants = bracket->getChildElements(XmlDeclarations::BIOCHEMICALLINKAGE_EL);
								if (bioLinkLocants.size() > 0)
								{
									if (bioLinkLocants.size() > 1)
									{
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("OPSIN Bug: More than 1 biochemical linkage locant associated with bracket");
										throw std::exception();
									}
									Element *bioLinkLocant = bioLinkLocants[0];
									Element *substituent = OpsinTools::getPreviousSibling(bioLinkLocant);
									if (substituent == nullptr || substituent->getName() != XmlDeclarations::SUBSTITUENT_EL)
									{
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("OPSIN Bug: Substituent expected before biochemical linkage locant");
										throw std::exception();
									}
									std::wstring bioLinkLocantStr = bioLinkLocant->getValue();
									bioLinkLocantStr = bioLinkLocantStr.substr(1, (bioLinkLocantStr.length() - 1) - 1);
									checkAndApplyFirstLocantOfBiochemicalLinkage(substituent, bioLinkLocantStr);
									int secondLocantStartPos = std::max((int)bioLinkLocantStr.rfind(L'>'), (int)bioLinkLocantStr.rfind(L'-')) + 1;
									std::wstring locantToConnectTo = bioLinkLocantStr.substr(secondLocantStartPos);
									if (bracket->getAttribute(XmlDeclarations::LOCANT_ATR) != nullptr)
									{
										throw StructureBuildingException(std::wstring(L"Substituent with biochemical linkage descriptor should not also have a locant: ") + bracket->getAttributeValue(XmlDeclarations::LOCANT_ATR));
									}
									Attribute tempVar(XmlDeclarations::LOCANT_ATR, std::wstring(L"O") + locantToConnectTo);
									bracket->addAttribute(&tempVar);
									bioLinkLocant->detach();
								}
							}
						}

						bool ComponentProcessor::isSubBracketOrRoot(Element *element)
						{
							return element != nullptr && (element->getName() == XmlDeclarations::SUBSTITUENT_EL || element->getName() == XmlDeclarations::BRACKET_EL || element->getName() == XmlDeclarations::ROOT_EL);
						}

						void ComponentProcessor::checkAndApplyFirstLocantOfBiochemicalLinkage(Element *substituent, const std::wstring &biochemicalLinkage) throw(StructureBuildingException)
						{
							Element *group = substituent->getFirstChildElement(XmlDeclarations::GROUP_EL);
							Fragment *frag = group->getFrag();
							std::wstring firstLocant = biochemicalLinkage.substr(0, (int)biochemicalLinkage.find(L'-'));
							if (group->getAttributeValue(XmlDeclarations::TYPE_ATR) == XmlDeclarations::CARBOHYDRATE_TYPE_VAL)
							{
								Atom *anomericAtom = frag->getAtomByLocantOrThrow(firstLocant);
								bool anomericIsOutAtom = false;
								for (int i = 0; i < frag->getOutAtomCount(); i++)
								{
									if (frag->getOutAtom(i)->getAtom()->equals(anomericAtom))
									{
										anomericIsOutAtom = true;
									}
								}
								if (!anomericIsOutAtom)
								{
									throw StructureBuildingException(std::wstring(L"Invalid glycoside linkage descriptor. Locant: ") + firstLocant + std::wstring(L" should point to the anomeric carbon"));
								}
							}
							else
							{
								Atom *positionOfPhospho = frag->getAtomByLocantOrThrow(std::wstring(L"O") + firstLocant);
								if (positionOfPhospho->getBondCount() != 1)
								{
									throw StructureBuildingException(firstLocant + std::wstring(L" should be the carbon to which a hydroxy group is attached!"));
								}
								if (frag->getOutAtomCount() == 1)
								{
									Atom *atomToConnect = frag->getOutAtom(0)->getAtom();
									state->fragManager->createBond(positionOfPhospho, atomToConnect, 1);
								}
								else
								{
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("OPSIN Bug: Biochemical linkage only expected on groups with 1 OutAtom");
									throw std::exception();
								}
							}

							if (OpsinTools::getNextGroup(group) == nullptr)
							{
								throw StructureBuildingException(std::wstring(L"Biochemical linkage descriptor should be followed by another biochemical: ") + biochemicalLinkage);
							}
						}

						void ComponentProcessor::moveErroneouslyPositionedLocantsAndMultipliers(std::vector<Element*> &brackets)
						{
							for (int i = brackets.size() - 1; i >= 0; i--)
							{
								Element *bracket = brackets[i];
								std::vector<Element*> childElements = bracket->getChildElements();
								bool hyphenPresent = false;
								int childCount = childElements.size();
								if (childCount == 2)
								{
									for (int j = childCount - 1; j >= 0; j--)
									{
										if (childElements[j]->getName() == XmlDeclarations::HYPHEN_EL)
										{
											hyphenPresent = true;
										}
									}
								}
								if (childCount == 1 || hyphenPresent && childCount == 2)
								{
									std::vector<Element*> substituentContent = childElements[0]->getChildElements();
									if (substituentContent.size() >= 2)
									{
										Element *locant = nullptr;
										Element *multiplier = nullptr;
										Element *possibleMultiplier = substituentContent[0];
										if (substituentContent[0]->getName() == XmlDeclarations::LOCANT_EL)
										{ //probably erroneous locant
											locant = substituentContent[0];
											possibleMultiplier = substituentContent[1];
										}
										if (possibleMultiplier->getName() == XmlDeclarations::MULTIPLIER_EL)
										{ //erroneously placed multiplier present
											multiplier = possibleMultiplier;
										}
										if (locant != nullptr)
										{
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
											if (multiplier == nullptr || locant->getValue().split(L",")->length == std::stoi(multiplier->getAttributeValue(XmlDeclarations::VALUE_ATR)))
											{
												locant->detach();
												OpsinTools::insertBefore(childElements[0], locant);
											}
											else
											{
												continue;
											}
										}
										if (multiplier != nullptr)
										{
											multiplier->detach();
											OpsinTools::insertBefore(childElements[0], multiplier);
										}
									}
								}
							}
						}

						void ComponentProcessor::addImplicitBracketsWhenFirstSubstituentHasTwoMultipliers(Element *substituent, std::vector<Element*> &brackets)
						{
							if (substituent->getName() != XmlDeclarations::SUBSTITUENT_EL)
							{
								return;
							}
							std::vector<Element*> multipliers;
							for (int i = 0, len = substituent->getChildCount(); i < len; i++)
							{
								Element *child = substituent->getChild(i);
								if (child->getName() == XmlDeclarations::MULTIPLIER_EL)
								{
									multipliers.push_back(child);
								}
								else
								{
									break;
								}
							}
							if (multipliers.size() != 2)
							{
								return;
							}
							Element *bracket = new GroupingEl(XmlDeclarations::BRACKET_EL);
							Attribute tempVar(XmlDeclarations::TYPE_ATR, XmlDeclarations::IMPLICIT_TYPE_VAL);
							bracket->addAttribute(&tempVar);
							Element *parent = substituent->getParent();
							std::vector<Element*> elsToAddToBracket = parent->getChildElements();
							Element *wordMultiplier = multipliers[0];
							wordMultiplier->detach();
							bracket->addChild(wordMultiplier);
							for (auto el : elsToAddToBracket)
							{
								el->detach();
								bracket->addChild(el);
							}
							parent->addChild(bracket);
							brackets.push_back(bracket);
						}

						void ComponentProcessor::assignLocantsToMultipliedRootIfPresent(Element *rightMostElement) throw(ComponentGenerationException, StructureBuildingException)
						{
							std::vector<Element*> multipliers = rightMostElement->getChildElements(XmlDeclarations::MULTIPLIER_EL);
							if (multipliers.size() == 1)
							{
								Element *multiplier = multipliers[0];
								if (OpsinTools::getPrevious(multiplier) == nullptr)
								{
									throw StructureBuildingException(L"OPSIN bug: Unacceptable input to function");
								}
								std::vector<Element*> locants = rightMostElement->getChildElements(XmlDeclarations::MULTIPLICATIVELOCANT_EL);
								if (locants.size() > 1)
								{
									throw ComponentGenerationException(L"OPSIN bug: Only none or one multiplicative locant expected");
								}
								int multiVal = std::stoi(multiplier->getAttributeValue(XmlDeclarations::VALUE_ATR));
								if (locants.empty())
								{
									Attribute tempVar(XmlDeclarations::INLOCANTS_ATR, XmlDeclarations::INLOCANTS_DEFAULT);
									rightMostElement->addAttribute(&tempVar);
								}
								else
								{
									Element *locantEl = locants[0];
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
									std::vector<std::wstring> locantValues = locantEl->getValue().split(L",");
									if (locantValues.size() == multiVal)
									{
										Attribute tempVar2(XmlDeclarations::INLOCANTS_ATR, locantEl->getValue());
										rightMostElement->addAttribute(&tempVar2);
										locantEl->detach();
									}
									else
									{
										throw ComponentGenerationException(L"Mismatch between number of locants and number of roots");
									}
								}
							}
							else if (rightMostElement->getName() == XmlDeclarations::BRACKET_EL)
							{
								assignLocantsToMultipliedRootIfPresent(rightMostElement->getChild(rightMostElement->getChildCount() - 1));
							}
						}

						void ComponentProcessor::addImplicitBracketsWhenSubstituentHasTwoLocants(Element *substituent, std::vector<Element*> &brackets)
						{
							Element *siblingSubstituent = OpsinTools::getNextSibling(substituent);
							if (siblingSubstituent != nullptr && siblingSubstituent->getName() == XmlDeclarations::SUBSTITUENT_EL)
							{
								std::vector<Element*> locants = getLocantsAtStartOfSubstituent(substituent);
								if (locants.size() == 2 && locantsAreSingular(locants) && getLocantsAtStartOfSubstituent(siblingSubstituent).empty())
								{
									Element *bracket = new GroupingEl(XmlDeclarations::BRACKET_EL);
									Attribute tempVar(XmlDeclarations::TYPE_ATR, XmlDeclarations::IMPLICIT_TYPE_VAL);
									bracket->addAttribute(&tempVar);
									Element *parent = substituent->getParent();
									int indexToInsertAt = parent->find(substituent);
									int elementsToMove = substituent->find(locants[0]) + 1;
									for (int i = 0; i < elementsToMove; i++)
									{
										Element *locantOrStereoToMove = substituent->getChild(0);
										locantOrStereoToMove->detach();
										bracket->addChild(locantOrStereoToMove);
									}
									substituent->detach();
									siblingSubstituent->detach();
									bracket->addChild(substituent);
									bracket->addChild(siblingSubstituent);
									parent->insertChild(bracket, indexToInsertAt);
									brackets.push_back(bracket);
								}
							}
						}

						std::vector<Element*> ComponentProcessor::getLocantsAtStartOfSubstituent(Element *substituent)
						{
							std::vector<Element*> locants;
							for (int i = 0, len = substituent->getChildCount(); i < len; i++)
							{
								Element *child = substituent->getChild(i);
								std::wstring currentElementName = child->getName();
								if (currentElementName == XmlDeclarations::LOCANT_EL)
								{
									locants.push_back(child);
								}
								else if (currentElementName == XmlDeclarations::STEREOCHEMISTRY_EL)
								{
									//ignore
								}
								else
								{
									break;
								}
							}
							return locants;
						}

						bool ComponentProcessor::locantsAreSingular(std::vector<Element*> &locants)
						{
							for (auto locant : locants)
							{
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
								if (locant->getValue().split(L",")->length > 1)
								{
									return false;
								}
							}
							return true;
						}

						void ComponentProcessor::assignLocantsAndMultipliers(Element *subOrBracket) throw(ComponentGenerationException, StructureBuildingException)
						{
							std::vector<Element*> locants = subOrBracket->getChildElements(XmlDeclarations::LOCANT_EL);
							int multiplier = 1;
							std::vector<Element*> multipliers = subOrBracket->getChildElements(XmlDeclarations::MULTIPLIER_EL);
							Element *parentElem = subOrBracket->getParent();
							bool oneBelowWordLevel = parentElem->getName() == XmlDeclarations::WORD_EL;
							Element *groupIfPresent = subOrBracket->getFirstChildElement(XmlDeclarations::GROUP_EL);
							if (multipliers.size() > 0)
							{
								if (multipliers.size() > 1)
								{
									throw ComponentGenerationException(subOrBracket->getName() + std::wstring(L" has multiple multipliers, unable to determine meaning!"));
								}
								if (oneBelowWordLevel && OpsinTools::getNextSibling(subOrBracket) == nullptr && OpsinTools::getPreviousSibling(subOrBracket) == nullptr)
								{
									return; //word level multiplier
								}
								multiplier = std::stoi(multipliers[0]->getAttributeValue(XmlDeclarations::VALUE_ATR));
								Attribute tempVar(XmlDeclarations::MULTIPLIER_ATR, multipliers[0]->getAttributeValue(XmlDeclarations::VALUE_ATR));
								subOrBracket->addAttribute(&tempVar);
								//multiplier is INTENTIONALLY not detached. As brackets/subs are only multiplied later on it is necessary at that stage to determine what elements (if any) are in front of the multiplier
								if (groupIfPresent != nullptr && XmlDeclarations::PERHALOGENO_SUBTYPE_VAL == groupIfPresent->getAttributeValue(XmlDeclarations::SUBTYPE_ATR))
								{
									throw StructureBuildingException(groupIfPresent->getValue() + std::wstring(L" cannot be multiplied"));
								}
							}
							if (locants.size() > 0)
							{
								if (multiplier == 1 && oneBelowWordLevel && OpsinTools::getPreviousSibling(subOrBracket) == nullptr)
								{ //locant might be word Level locant
									if (wordLevelLocantsAllowed(subOrBracket, locants.size()))
									{ //something like S-ethyl or S-(2-ethylphenyl) or S-4-tert-butylphenyl
										Element *locant = locants.erase(locants.begin());
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
										if (locant->getValue().split(L",")->length != 1)
										{
											throw ComponentGenerationException(L"Multiplier and locant count failed to agree; All locants could not be assigned!");
										}
										Attribute tempVar2(XmlDeclarations::LOCANT_ATR, locant->getValue());
										parentElem->addAttribute(&tempVar2);
										locant->detach();
										if (locants.empty())
										{
											return;
										}
									}
								}
								if (subOrBracket->getName() == XmlDeclarations::ROOT_EL)
								{
									locantsToDebugString(locants);
									throw ComponentGenerationException(locantsToDebugString(locants));
								}
								if (locants.size() != 1)
								{
									throw ComponentGenerationException(locantsToDebugString(locants));
								}
								Element *locantEl = locants[0];
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
								std::vector<std::wstring> locantValues = locantEl->getValue().split(L",");
								if (multiplier != locantValues.size())
								{
									throw ComponentGenerationException(L"Multiplier and locant count failed to agree; All locants could not be assigned!");
								}

								Element *parent = subOrBracket->getParent();
								//attempt to find cases where locant will not be utilised. A special case is made for carbonyl derivatives //e.g. 1H-2-benzopyran-1,3,4-trione 4-[N-(4-chlorophenyl)hydrazone]
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
								if (parent->getName() != XmlDeclarations::WORD_EL || parent->getAttributeValue(XmlDeclarations::TYPE_ATR) != WordType::full->toString() || !state->currentWordRule::equals(WordRule::carbonylDerivative))
								{
									std::vector<Element*> children = parent->getChildElements();
									bool foundSomethingToSubstitute = false;
									for (int i = parent->find(subOrBracket) + 1 ; i < children.size(); i++)
									{
										if (children[i]->getName() != XmlDeclarations::HYPHEN_EL)
										{
											foundSomethingToSubstitute = true;
										}
									}
									if (!foundSomethingToSubstitute)
									{
										throw ComponentGenerationException(locantsToDebugString(locants));
									}
								}
								if (groupIfPresent != nullptr && XmlDeclarations::PERHALOGENO_SUBTYPE_VAL == groupIfPresent->getAttributeValue(XmlDeclarations::SUBTYPE_ATR))
								{
									throw StructureBuildingException(groupIfPresent->getValue() + std::wstring(L" cannot be locanted"));
								}
								Attribute tempVar3(XmlDeclarations::LOCANT_ATR, locantEl->getValue());
								subOrBracket->addAttribute(&tempVar3);
								locantEl->detach();
							}
						}

						std::wstring ComponentProcessor::locantsToDebugString(std::vector<Element*> &locants)
						{
							StringBuilder *message = new StringBuilder(L"Unable to assign all locants. ");
							message->append((locants.size() > 1) ? L"These locants " : L"This locant ");
							message->append((locants.size() > 1) ? L"were " : L"was ");
							message->append(L"not assigned: ");
							for (auto locant : locants)
							{
								message->append(locant->getValue());
								message->append(L" ");
							}
							return message->toString();
						}

						bool ComponentProcessor::wordLevelLocantsAllowed(Element *subBracketOrRoot, int numberOflocants)
						{
							Element *parentElem = subBracketOrRoot->getParent();
							if (StringHelper::fromString<WordType>(parentElem->getAttributeValue(XmlDeclarations::TYPE_ATR)) == WordType::substituent && (OpsinTools::getNextSibling(subBracketOrRoot) == nullptr || numberOflocants >= 2))
							{
								if (state->currentWordRule == WordRule::ester || state->currentWordRule == WordRule::functionalClassEster || state->currentWordRule == WordRule::multiEster || state->currentWordRule == WordRule::acetal)
								{
									return true;
								}
							}
							if ((state->currentWordRule == WordRule::potentialAlcoholEster || state->currentWordRule == WordRule::amineDiConjunctiveSuffix || (state->currentWordRule == WordRule::ester && (OpsinTools::getNextSibling(subBracketOrRoot) == nullptr || numberOflocants >= 2))) && parentElem->getName() == XmlDeclarations::WORD_EL)
							{
								Element *wordRule = parentElem->getParent();
								std::vector<Element*> words = wordRule->getChildElements(XmlDeclarations::WORD_EL);
								Element *ateWord = words[words.size() - 1];
								if (parentElem == ateWord)
								{
									return true;
								}
							}
							if (state->currentWordRule == WordRule::acidReplacingFunctionalGroup && parentElem->getName() == XmlDeclarations::WORD_EL && (OpsinTools::getNextSibling(subBracketOrRoot) == nullptr || numberOflocants >= 2))
							{
								//e.g. diphosphoric acid 1,3-di(ethylamide)
								if (parentElem->getParent()->find(parentElem) > 0)
								{
									return true;
								}
							}

							return false;
						}

						void ComponentProcessor::processWordLevelMultiplierIfApplicable(Element *word, std::vector<Element*> &roots, int wordCount) throw(StructureBuildingException, ComponentGenerationException)
						{
							if (word->getChildCount() == 1)
							{
								Element *firstSubBrackOrRoot = word->getChild(0);
								Element *multiplier = firstSubBrackOrRoot->getFirstChildElement(XmlDeclarations::MULTIPLIER_EL);
								if (multiplier != nullptr)
								{
									int multiVal = std::stoi(multiplier->getAttributeValue(XmlDeclarations::VALUE_ATR));
									std::vector<Element*> locants = firstSubBrackOrRoot->getChildElements(XmlDeclarations::LOCANT_EL);
									bool assignLocants = false;
									bool wordLevelLocants = wordLevelLocantsAllowed(firstSubBrackOrRoot, locants.size()); //something like O,S-dimethyl phosphorothioate
									if (locants.size() > 1)
									{
										throw ComponentGenerationException(L"Unable to assign all locants");
									}
									std::vector<std::wstring> locantValues;
									if (locants.size() == 1)
									{
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
										locantValues = locants[0]->getValue()->split(L",");
										if (locantValues.size() == multiVal)
										{
											assignLocants = true;
											locants[0]->detach();
											if (wordLevelLocants)
											{
												Attribute tempVar(XmlDeclarations::LOCANT_ATR, locantValues[0]);
												word->addAttribute(&tempVar);
											}
											else
											{
												throw ComponentGenerationException(locantsToDebugString(locants));
											}
										}
										else
										{
											throw ComponentGenerationException(L"Unable to assign all locants");
										}
									}
									checkForNonConfusedWithNona(multiplier);
									if (wordCount == 1)
									{
										if (!isMonoFollowedByElement(multiplier, multiVal))
										{
											throw StructureBuildingException(L"Unexpected multiplier found at start of word. Perhaps the name is trivial e.g. triphosgene");
										}
									}
									if (multiVal == 1)
									{ //mono
										return;
									}
									std::vector<Element*> elementsNotToBeMultiplied; //anything before the multiplier
									for (int i = firstSubBrackOrRoot->find(multiplier) - 1 ; i >= 0 ; i--)
									{
										Element *el = firstSubBrackOrRoot->getChild(i);
										el->detach();
										elementsNotToBeMultiplied.push_back(el);
									}
									multiplier->detach();
									for (int i = multiVal - 1; i >= 1; i--)
									{
										Element *clone = state->fragManager->cloneElement(state, word);
										if (assignLocants)
										{
											clone->getAttribute(XmlDeclarations::LOCANT_ATR)->setValue(locantValues[i]);
										}
										OpsinTools::insertAfter(word, clone);
									}
									for (auto el : elementsNotToBeMultiplied)
									{ //re-add anything before multiplier to original word
										firstSubBrackOrRoot->insertChild(el, 0);
									}
								}
							}
							else if (roots.size() == 1)
							{
								if (OpsinTools::getDescendantElementsWithTagName(roots[0], XmlDeclarations::FRACTIONALMULTIPLIER_EL).size() > 0)
								{
									throw StructureBuildingException(L"Unexpected fractional multiplier found within chemical name");
								}
							}
						}

						void ComponentProcessor::checkForNonConfusedWithNona(Element *multiplier) throw(StructureBuildingException)
						{
							if (multiplier->getValue() == L"non")
							{
								std::wstring subsequentUnsemanticToken = multiplier->getAttributeValue(XmlDeclarations::SUBSEQUENTUNSEMANTICTOKEN_ATR);
								if (subsequentUnsemanticToken != L"" && subsequentUnsemanticToken.toLowerCase(Locale::ROOT)->startsWith(L"a"))
								{
									return;
								}
								throw StructureBuildingException(L"\"non\" probably means \"not\". If a multiplier of value 9 was intended \"nona\" should be used");
							}
						}

						bool ComponentProcessor::isMonoFollowedByElement(Element *multiplier, int multiVal)
						{
							if (multiVal == 1)
							{
								Element *possibleElement = OpsinTools::getNextSibling(multiplier);
								if (possibleElement != nullptr && possibleElement->getName() == XmlDeclarations::GROUP_EL && (XmlDeclarations::ELEMENTARYATOM_SUBTYPE_VAL == possibleElement->getAttributeValue(XmlDeclarations::SUBTYPE_ATR) || possibleElement->getValue() == L"hydrogen"))
								{
									return true;
								}
							}
							return false;
						}
					}
				}
			}
		}
	}
}
