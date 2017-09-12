#include "WordRulesOmittedSpaceCorrector.h"
#include "BuildState.h"
#include "Element.h"
#include "StructureBuildingException.h"
#include "OpsinTools.h"
#include "XmlDeclarations.h"
#include "WordRule.h"
#include "Fragment.h"
#include "GroupingEl.h"
#include "Attribute.h"
#include "Atom.h"
#include "StereoAnalyser.h"
#include "AmbiguityChecker.h"
#include "OutAtom.h"
#include "ChemEl.h"
#include "FragmentTools.h"

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
//						import static uk.ac.cam.ch.wwmm.opsin.StructureBuildingMethods.findRightMostGroupInSubBracketOrRoot;

						WordRulesOmittedSpaceCorrector::WordRulesOmittedSpaceCorrector(BuildState *state, Element *parse) : parse(parse)
						{
							this->state = state;
						}

						void WordRulesOmittedSpaceCorrector::correctOmittedSpaces() throw(StructureBuildingException)
						{
							std::vector<Element*> wordRules = OpsinTools::getDescendantElementsWithTagName(parse, XmlDeclarations::WORDRULE_EL);
							for (auto wordRule : wordRules)
							{
								WordRule wordRuleVal = StringHelper::fromString<WordRule>(wordRule->getAttributeValue(XmlDeclarations::WORDRULE_ATR));
								if (wordRuleVal == WordRule::divalentFunctionalGroup)
								{
									checkAndCorrectOmittedSpacesInDivalentFunctionalGroupRule(wordRule);
								}
								else if (wordRuleVal == WordRule::simple)
								{
									//note that this function may change the word rule to ester
									checkAndCorrectOmittedSpaceEster(wordRule);
								}
							}
						}

						void WordRulesOmittedSpaceCorrector::checkAndCorrectOmittedSpacesInDivalentFunctionalGroupRule(Element *divalentFunctionalGroupWordRule)
						{
							std::vector<Element*> substituentWords = OpsinTools::getChildElementsWithTagNameAndAttribute(divalentFunctionalGroupWordRule, XmlDeclarations::WORD_EL, XmlDeclarations::TYPE_ATR, XmlDeclarations::SUBSTITUENT_TYPE_VAL);
							if (substituentWords.size() == 1)
							{ //potentially has been "wrongly" interpreted e.g. ethylmethyl ketone is more likely to mean ethyl methyl ketone
								std::vector<Element*> children = OpsinTools::getChildElementsWithTagNames(substituentWords[0], std::vector<std::wstring>{XmlDeclarations::SUBSTITUENT_EL, XmlDeclarations::BRACKET_EL});
								if (children.size() == 2)
								{
									Element *firstSubOrbracket = children[0];
									//rule out correct usage e.g. diethyl ether and locanted substituents e.g. 2-methylpropyl ether
									if (firstSubOrbracket->getAttribute(XmlDeclarations::LOCANT_ATR) == nullptr && firstSubOrbracket->getAttribute(XmlDeclarations::MULTIPLIER_ATR) == nullptr)
									{
										Element *firstGroup = findRightMostGroupInSubBracketOrRoot(firstSubOrbracket);
										Fragment *firstFrag = firstGroup->getFrag();
										if (hasSingleMonoValentCarbonOrSiliconRadical(firstFrag))
										{
											Element *subToMove = children[1];
											subToMove->detach();
											Element *newWord = new GroupingEl(XmlDeclarations::WORD_EL);
											Attribute tempVar(XmlDeclarations::TYPE_ATR, XmlDeclarations::SUBSTITUENT_TYPE_VAL);
											newWord->addAttribute(&tempVar);
											newWord->addChild(subToMove);
											OpsinTools::insertAfter(substituentWords[0], newWord);
										}
									}
								}
							}
						}

						void WordRulesOmittedSpaceCorrector::checkAndCorrectOmittedSpaceEster(Element *wordRule) throw(StructureBuildingException)
						{
							std::vector<Element*> words = wordRule->getChildElements(XmlDeclarations::WORD_EL);
							if (words.size() != 1)
							{
								return;
							}
							Element *word = words[0];
							std::wstring wordRuleContents = wordRule->getAttributeValue(XmlDeclarations::VALUE_ATR);
							if (matchAteOrIteEnding->matcher(wordRuleContents).find())
							{
								std::vector<Element*> children = OpsinTools::getChildElementsWithTagNames(word, std::vector<std::wstring>{XmlDeclarations::SUBSTITUENT_EL, XmlDeclarations::BRACKET_EL, XmlDeclarations::ROOT_EL});
								if (children.size() >= 2)
								{
									Element *rootEl = children[children.size() - 1];
									Element *rootGroup = findRightMostGroupInSubBracketOrRoot(rootEl);
									Fragment *rootFrag = rootGroup->getFrag();
									int functionalAtomsCount = rootFrag->getFunctionalAtomCount();
									int rootMultiplier = 1;
									{
										std::wstring rootElMultiplierAtrVal = rootEl->getAttributeValue(XmlDeclarations::MULTIPLIER_ATR);
										if (rootElMultiplierAtrVal != L"")
										{
											rootMultiplier = std::stoi(rootElMultiplierAtrVal);
											functionalAtomsCount *= rootMultiplier;
										}
									}
									if (functionalAtomsCount > 0)
									{
										std::vector<Element*> substituents = children.subList(0, children.size() - 1);
										int substituentCount = substituents.size();
										if (substituentCount == 1 && rootMultiplier > 1)
										{
											return;
										}
										Element *firstChild = substituents[0];
										if (!checkSuitabilityOfSubstituentForEsterFormation(firstChild, functionalAtomsCount))
										{
											return;
										}
										std::wstring multiplierValue = firstChild->getAttributeValue(XmlDeclarations::MULTIPLIER_ATR);
										if (specialCaseWhereEsterPreferred(findRightMostGroupInSubBracketOrRoot(firstChild), multiplierValue, rootGroup, substituentCount))
										{
											transformToEster(wordRule, firstChild);
										}
										else if (substituentCount > 1 && (allBarFirstSubstituentHaveLocants(substituents) || insufficientSubstitutableHydrogenForSubstitution(substituents, rootFrag, rootMultiplier)))
										{
											transformToEster(wordRule, firstChild);
										}
										else if ((substituentCount == 1 || rootMultiplier > 1) && substitutionWouldBeAmbiguous(rootFrag, multiplierValue))
										{
											//either 1 substituent or multiplicative nomenclature (in the multiplicative nomenclature case many substituents will not have locants)
											transformToEster(wordRule, firstChild);
										}
									}
								}
							}
						}

						bool WordRulesOmittedSpaceCorrector::allBarFirstSubstituentHaveLocants(std::vector<Element*> &substituentsAndBrackets)
						{
							if (substituentsAndBrackets.size() <= 1)
							{
								return false;
							}
							for (int i = 1; i < substituentsAndBrackets.size(); i++)
							{
								if (substituentsAndBrackets[i]->getAttribute(XmlDeclarations::LOCANT_ATR) == nullptr)
								{
									return false;
								}
							}
							return true;
						}

						bool WordRulesOmittedSpaceCorrector::insufficientSubstitutableHydrogenForSubstitution(std::vector<Element*> &substituentsAndBrackets, Fragment *frag, int rootMultiplier)
						{
							int substitutableHydrogens = getAtomForEachSubstitutableHydrogen(frag).size() * rootMultiplier;
							for (int i = 1; i < substituentsAndBrackets.size(); i++)
							{
								Element *subOrBracket = substituentsAndBrackets[i];
								Fragment *f = findRightMostGroupInSubBracketOrRoot(subOrBracket).getFrag();
								std::wstring multiplierValue = subOrBracket->getAttributeValue(XmlDeclarations::MULTIPLIER_ATR);
								int multiplier = 1;
								if (multiplierValue != L"")
								{
									multiplier = std::stoi(multiplierValue);
								}
								substitutableHydrogens -= (getTotalOutAtomValency(f) * multiplier);
							}
							Element *potentialEsterSub = substituentsAndBrackets[0];
							int firstFragSubstitutableHydrogenRequired = getTotalOutAtomValency(findRightMostGroupInSubBracketOrRoot(potentialEsterSub).getFrag());
							std::wstring multiplierValue = potentialEsterSub->getAttributeValue(XmlDeclarations::MULTIPLIER_ATR);
							int multiplier = 1;
							if (multiplierValue != L"")
							{
								multiplier = std::stoi(multiplierValue);
							}
							if (substitutableHydrogens >= 0 && (substitutableHydrogens - (firstFragSubstitutableHydrogenRequired * multiplier)) < 0)
							{
								return true;
							}
							return false;
						}

						int WordRulesOmittedSpaceCorrector::getTotalOutAtomValency(Fragment *f)
						{
							int outAtomValency = 0;
							for (int i = 0, l = f->getOutAtomCount(); i < l; i++)
							{
								outAtomValency += f->getOutAtom(i)->getValency();
							}
							return outAtomValency;
						}

						bool WordRulesOmittedSpaceCorrector::specialCaseWhereEsterPreferred(Element *substituentGroupEl, const std::wstring &multiplierValue, Element *rootGroup, int numOfSubstituents)
						{
							if (multiplierValue != L"" && std::stoi(multiplierValue) == 1)
							{
								return true;
							}
							std::wstring rootGroupName = rootGroup->getParent()->getValue();
							if (substituentGroupEl->getAttributeValue(XmlDeclarations::TYPE_ATR) == XmlDeclarations::CHAIN_TYPE_VAL && XmlDeclarations::ALKANESTEM_SUBTYPE_VAL == substituentGroupEl->getAttributeValue(XmlDeclarations::SUBTYPE_ATR))
							{
								if (std::regex_match(substituentGroupEl->getParent()->getValue(), std::regex(substituentGroupEl->getValue() + std::wstring(L"yl-?"))) && std::regex_match(rootGroupName, std::regex(L".*(form|methan|acet|ethan)[o]?ate?")))
								{
									return true;
								}
							}
							if ((boost::ends_with(rootGroupName, L"carbamate") || boost::ends_with(rootGroupName, L"carbamat")) && numOfSubstituents >= 2)
							{
								Element *temp = substituentGroupEl->getParent();
								while (temp->getParent() != nullptr)
								{
									temp = temp->getParent();
								}
								if (temp->getChildElements(XmlDeclarations::WORDRULE_EL).size() == 1)
								{
									return true;
								}
							}
							return false;
						}

						bool WordRulesOmittedSpaceCorrector::substitutionWouldBeAmbiguous(Fragment *frag, const std::wstring &multiplierValue)
						{
							int multiplier = 1;
							if (multiplierValue != L"")
							{
								multiplier = std::stoi(multiplierValue);
							}
							if (multiplier == 1 && frag->getDefaultInAtom() != nullptr)
							{
								return false;
							}
							std::vector<Atom*> atomForEachSubstitutableHydrogen = getAtomForEachSubstitutableHydrogen(frag);
							if (atomForEachSubstitutableHydrogen.size() == multiplier)
							{
								return false;
							}
							StereoAnalyser *analyser = new StereoAnalyser(frag);
							Set<std::wstring> *uniqueEnvironments = std::unordered_set<std::wstring>();
							for (auto a : atomForEachSubstitutableHydrogen)
							{
								uniqueEnvironments->add(AmbiguityChecker::getAtomEnviron(analyser, a));
							}
							if (uniqueEnvironments->size() == 1 && (multiplier == 1 || multiplier == atomForEachSubstitutableHydrogen.size() - 1))
							{
								return false;
							}
							return true;
						}

						bool WordRulesOmittedSpaceCorrector::checkSuitabilityOfSubstituentForEsterFormation(Element *subOrBracket, int rootFunctionalAtomsCount)
						{
							if (subOrBracket->getAttribute(XmlDeclarations::LOCANT_ATR) != nullptr)
							{
								return false;
							}
							Fragment *rightMostGroup = findRightMostGroupInSubBracketOrRoot(subOrBracket).getFrag();
							if (!hasSingleMonoValentCarbonOrSiliconRadical(rightMostGroup))
							{
								return false;
							}
							std::wstring multiplierStr = subOrBracket->getAttributeValue(XmlDeclarations::MULTIPLIER_ATR);
							if (multiplierStr != L"")
							{
								int multiplier = std::stoi(multiplierStr);
								if (multiplier > rootFunctionalAtomsCount)
								{
									return false;
								}
							}
							return true;
						}

						bool WordRulesOmittedSpaceCorrector::hasSingleMonoValentCarbonOrSiliconRadical(Fragment *frag)
						{
							if (frag->getOutAtomCount() == 1)
							{
								OutAtom *outAtom = frag->getOutAtom(0);
								if (outAtom->getValency() == 1 && (outAtom->getAtom()->getElement() == ChemEl::C || outAtom->getAtom()->getElement() == ChemEl::Si))
								{
									return true;
								}
							}
							return false;
						}

						std::vector<Atom*> WordRulesOmittedSpaceCorrector::getAtomForEachSubstitutableHydrogen(Fragment *frag)
						{
							std::vector<Atom*> substitutableAtoms;
							std::vector<Atom*> atomList = frag->getAtomList();
							for (auto atom : atomList)
							{
								if (FragmentTools::isCharacteristicAtom(atom))
								{
									continue;
								}
								int currentExpectedValency = atom->determineValency(true);
								int currentValency = (atom->getIncomingValency() + (atom->hasSpareValency() ? 1 : 0) + atom->getOutValency());
								for (int i = currentValency; i < currentExpectedValency; i++)
								{
									substitutableAtoms.push_back(atom);
								}
							}
							return substitutableAtoms;
						}

						void WordRulesOmittedSpaceCorrector::transformToEster(Element *parentSimpleWordRule, Element *substituentOrBracket) throw(StructureBuildingException)
						{
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
							parentSimpleWordRule->getAttribute(XmlDeclarations::WORDRULE_ATR)->setValue(WordRule::ester->toString());
							std::vector<Element*> childElsOfSub = substituentOrBracket->getChildElements();
							Element *lastChildElOfSub = childElsOfSub[childElsOfSub.size() - 1];
							if (lastChildElOfSub->getName() == XmlDeclarations::HYPHEN_EL)
							{
								lastChildElOfSub->detach();
							}
							substituentOrBracket->detach();
							Element *newSubstituentWord = new GroupingEl(XmlDeclarations::WORD_EL);
							Attribute tempVar(XmlDeclarations::TYPE_ATR, XmlDeclarations::SUBSTITUENT_TYPE_VAL);
							newSubstituentWord->addAttribute(&tempVar);
							newSubstituentWord->addChild(substituentOrBracket);
							parentSimpleWordRule->insertChild(newSubstituentWord, 0);
							std::wstring multiplierStr = substituentOrBracket->getAttributeValue(XmlDeclarations::MULTIPLIER_ATR);
							if (multiplierStr != L"")
							{
								substituentOrBracket->removeAttribute(substituentOrBracket->getAttribute(XmlDeclarations::MULTIPLIER_ATR));
								int multiplier = std::stoi(multiplierStr);
								for (int i = 1; i < multiplier; i++)
								{
									Element *clone = state->fragManager->cloneElement(state, newSubstituentWord);
									OpsinTools::insertAfter(newSubstituentWord, clone);
								}
							}
						}
					}
				}
			}
		}
	}
}
