#include "WordRules.h"
#include "ResourceGetter.h"
#include "Element.h"
#include "NameToStructureConfig.h"
#include "ParsingException.h"
#include "XmlDeclarations.h"
#include "OpsinTools.h"
#include "GroupingEl.h"
#include "Attribute.h"
#include "FragmentTools.h"
#include "StringTools.h"
#include "TokenEl.h"

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

						WordRules::WordDescription::WordDescription(XMLStreamReader *reader) : type(static_cast<WordType>(atrValue)), endsWithGroup(static_cast<EndsWithGroup>(atrValue)), endsWithPattern(Pattern::compile(atrValue + std::wstring(L"$"), Pattern::CASE_INSENSITIVE)), value(atrValue), functionalGroupType(atrValue), endsWithGroupType(atrValue), endsWithGroupSubType(atrValue)
						{
							WordType type = nullptr;
							std::wstring value = L"";
							EndsWithGroup endsWithGroup = nullptr;
							Pattern *endsWithPattern = nullptr;
							std::wstring functionalGroupType = L"";
							std::wstring endsWithGroupType = L"";
							std::wstring endsWithGroupSubType = L"";
							for (int i = 0, l = reader->getAttributeCount(); i < l; i++)
							{
								std::wstring atrName = reader->getAttributeLocalName(i);
								std::wstring atrValue = reader->getAttributeValue(i);
								if (atrName == L"type")
								{
								}
								else if (atrName == L"value")
								{
								}
								else if (atrName == L"functionalGroupType")
								{
								}
								else if (atrName == L"endsWith")
								{
								}
								else if (atrName == L"endsWithRegex")
								{
								}
								else if (atrName == L"endsWithGroupType")
								{
								}
								else if (atrName == L"endsWithGroupSubType")
								{
								}
							}
							if (type == nullptr)
							{
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("Malformed wordRules");
								throw std::exception();
							}
							this->type = type;
							this->endsWithGroup = endsWithGroup;
							this->endsWithPattern = endsWithPattern;
							this->value = value;
							this->functionalGroupType = functionalGroupType;
							this->endsWithGroupType = endsWithGroupType;
							this->endsWithGroupSubType = endsWithGroupSubType;
						}

						WordType WordRules::WordDescription::getType()
						{
							return type;
						}

						EndsWithGroup WordRules::WordDescription::getEndsWithGroup()
						{
							return endsWithGroup;
						}

						Pattern *WordRules::WordDescription::getEndsWithPattern()
						{
							return endsWithPattern;
						}

						std::wstring WordRules::WordDescription::getValue()
						{
							return value;
						}

						std::wstring WordRules::WordDescription::getFunctionalGroupType()
						{
							return functionalGroupType;
						}

						std::wstring WordRules::WordDescription::getEndsWithGroupType()
						{
							return endsWithGroupType;
						}

						std::wstring WordRules::WordDescription::getEndsWithGroupSubType()
						{
							return endsWithGroupSubType;
						}

						std::vector<WordDescription*> WordRules::WordRuleDescription::getWordDescriptions()
						{
							return wordDescriptions;
						}

						WordRule WordRules::WordRuleDescription::getRuleName()
						{
							return ruleName;
						}

						WordType WordRules::WordRuleDescription::getRuleType()
						{
							return ruleType;
						}

						WordRules::WordRuleDescription::WordRuleDescription(XMLStreamReader *reader) throw(XMLStreamException) : wordDescriptions(Collections::unmodifiableList(wordDescriptions)), ruleName(static_cast<WordRule>(reader->getAttributeValue(nullptr, L"name"))), ruleType(static_cast<WordType>(reader->getAttributeValue(nullptr,L"type")))
						{
							std::vector<WordDescription*> wordDescriptions;
							while (reader->hasNext())
							{
								int event_Renamed = reader->next();
								if (event_Renamed == XMLStreamConstants::START_ELEMENT)
								{
									if (reader->getLocalName().equals(L"word"))
									{
										WordDescription tempVar(reader);
										wordDescriptions.push_back(&tempVar);
									}
								}
								else if (event_Renamed == XMLStreamConstants::END_ELEMENT)
								{
									if (reader->getLocalName().equals(L"wordRule"))
									{
										break;
									}
								}
								reader++;
							}
						}

						WordRules::WordRules(ResourceGetter *resourceGetter) throw(IOException) : wordRuleList(Collections::unmodifiableList(wordRuleList))
						{
							std::vector<WordRuleDescription*> wordRuleList;
							XMLStreamReader *reader = resourceGetter->getXMLStreamReader(L"wordRules.xml");
							try
							{
								while (reader->hasNext())
								{
									if (reader->next() == XMLStreamConstants::START_ELEMENT && reader->getLocalName().equals(L"wordRule"))
									{
										WordRuleDescription tempVar(reader);
										wordRuleList.push_back(&tempVar);
									}
									reader++;
								}
							}
							catch (const XMLStreamException &e)
							{
								throw IOException(L"Parsing exception occurred while reading wordRules.xml", e);
							}
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to the exception 'finally' clause:
							finally
							{
								try
								{
									delete reader;
								}
								catch (const XMLStreamException &e)
								{
									throw IOException(L"Parsing exception occurred while reading wordRules.xml", e);
								}
							}
						}

						void WordRules::groupWordsIntoWordRules(Element *moleculeEl, NameToStructureConfig *n2sConfig, bool allowSpaceRemoval, std::vector<Integer> &componentRatios) throw(ParsingException)
						{
							WordRulesInstance *instance = new WordRulesInstance(this, moleculeEl, n2sConfig, allowSpaceRemoval, componentRatios);
							std::vector<Element*> wordEls = moleculeEl->getChildElements(XmlDeclarations::WORD_EL);
							//note that multiple words in wordEls may be later replaced by a wordRule element
							for (int i = 0; i < wordEls.size(); i++)
							{
								if (instance->matchWordRule(wordEls, i))
								{
									i = -1; //if function did something
								}
							}
							std::vector<Element*> wordRuleEls = moleculeEl->getChildElements();
							for (auto wordRuleEl : wordRuleEls)
							{
								if (!wordRuleEl->getName().equals(XmlDeclarations::WORDRULE_EL))
								{
									throw ParsingException(std::wstring(L"Unable to assign wordRule to: ") + wordRuleEl->getAttributeValue(XmlDeclarations::VALUE_ATR));
								}
							}
						}

						WordRules::WordRulesInstance::WordRulesInstance(WordRules *outerInstance, Element *moleculeEl, NameToStructureConfig *n2sConfig, bool allowSpaceRemoval, std::vector<Integer> &componentRatios) : moleculeEl(moleculeEl), allowRadicals(n2sConfig->isAllowRadicals()), allowSpaceRemoval(allowSpaceRemoval), expectedNumOfComponents(componentRatios.size() > 0 ? componentRatios.size() : nullptr), outerInstance(outerInstance)
						{
						}

						bool WordRules::WordRulesInstance::matchWordRule(std::vector<Element*> &wordEls, int indexOfFirstWord) throw(ParsingException)
						{
							for (auto wordRuleDesc : outerInstance->wordRuleList)
							{
								int i = indexOfFirstWord;
								std::vector<WordDescription*> wordDescriptions = wordRuleDesc->getWordDescriptions();
								int wordsInWordRule = wordDescriptions.size();
								if (i + wordsInWordRule <= wordEls.size())
								{ //need sufficient words to match the word rule
									for (int j = 0; j < wordsInWordRule; j++)
									{
										Element *wordEl = wordEls[i + j];
										WordDescription *wd = wordDescriptions[j];
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
										if (!wd->getType()::toString()->equals(wordEl->getAttributeValue(XmlDeclarations::TYPE_ATR)))
										{
											goto wordRuleLoopContinue; //type mismatch;
										}

										std::wstring functionalGroupTypePredicate = wd->getFunctionalGroupType();
										if (functionalGroupTypePredicate != L"")
										{
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
											if (!WordType::functionalTerm->toString()->equals(wordEl->getAttributeValue(XmlDeclarations::TYPE_ATR)))
											{
												goto wordRuleLoopContinue;
											}
											Element *lastEl = getLastElementInWord(wordEl);
											if (lastEl == nullptr)
											{
												throw ParsingException(L"OPSIN Bug: Cannot find the functional element in a functionalTerm");
											}
											while (lastEl->getName() == XmlDeclarations::CLOSEBRACKET_EL || lastEl->getName() == XmlDeclarations::STRUCTURALCLOSEBRACKET_EL)
											{
												lastEl = OpsinTools::getPreviousSibling(lastEl);
												if (lastEl == nullptr)
												{
													throw ParsingException(L"OPSIN Bug: Cannot find the functional element in a functionalTerm");
												}
											}
											if (functionalGroupTypePredicate != lastEl->getAttributeValue(XmlDeclarations::TYPE_ATR))
											{
												goto wordRuleLoopContinue;
											}
										}

										EndsWithGroup endsWithGroupPredicate = wd->getEndsWithGroup();
										if (endsWithGroupPredicate != nullptr && !endsWithGroupPredicateSatisfied(wordEl, endsWithGroupPredicate))
										{
											goto wordRuleLoopContinue;
										}

										std::wstring valuePredicate = wd->getValue();
										if (valuePredicate != L"" && !wordEl->getAttributeValue(XmlDeclarations::VALUE_ATR).toLowerCase(Locale::ROOT).equals(valuePredicate))
										{ //word string contents mismatch
											goto wordRuleLoopContinue;
										}

										Pattern *endsWithPatternPredicate = wd->getEndsWithPattern();
										if (endsWithPatternPredicate != nullptr)
										{
											if (!endsWithPatternPredicate->matcher(wordEl->getAttributeValue(XmlDeclarations::VALUE_ATR)).find())
											{
												goto wordRuleLoopContinue;
											}
										}

										std::wstring endsWithGroupTypePredicate = wd->getEndsWithGroupType();
										if (endsWithGroupTypePredicate != L"")
										{
											Element *lastGroupInWordRule = getLastGroupInWordRule(wordEl);
											if (lastGroupInWordRule == nullptr || endsWithGroupTypePredicate != lastGroupInWordRule->getAttributeValue(XmlDeclarations::TYPE_ATR))
											{
												goto wordRuleLoopContinue;
											}
										}

										std::wstring endsWithSubGroupTypePredicate = wd->getEndsWithGroupSubType();
										if (endsWithSubGroupTypePredicate != L"")
										{
											Element *lastGroupInWordRule = getLastGroupInWordRule(wordEl);
											if (lastGroupInWordRule == nullptr || endsWithSubGroupTypePredicate != lastGroupInWordRule->getAttributeValue(XmlDeclarations::SUBTYPE_ATR))
											{
												goto wordRuleLoopContinue;
											}
										}
									}
									//Word Rule matches!
									Element *wordRuleEl = new GroupingEl(XmlDeclarations::WORDRULE_EL);
									WordRule wordRule = wordRuleDesc->getRuleName();
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
									Attribute tempVar(XmlDeclarations::TYPE_ATR, wordRuleDesc->getRuleType()::toString());
									wordRuleEl->addAttribute(&tempVar);
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
									Attribute tempVar2(XmlDeclarations::WORDRULE_EL, wordRule::toString());
									wordRuleEl->addAttribute(&tempVar2);

									/*
									 * Some wordRules can not be entirely processed at the structure building stage
									 */
									switch (wordRule)
									{
									case functionGroupAsGroup:
									{
										//convert the functional term into a full term
										Element *functionalWord = wordEls[i + wordsInWordRule -1];
										if (functionalWord->getAttributeValue(XmlDeclarations::TYPE_ATR) != XmlDeclarations::FUNCTIONALTERM_EL || wordsInWordRule > 2)
										{
											throw ParsingException(L"OPSIN bug: Problem with functionGroupAsGroup wordRule");
										}
										convertFunctionalGroupIntoGroup(functionalWord);
										if (wordsInWordRule == 2)
										{
											joinWords(wordEls, wordEls[i], functionalWord);
											wordsInWordRule = 1;
										}
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
										wordRuleEl->getAttribute(XmlDeclarations::WORDRULE_ATR)->setValue(WordRule::simple->toString());
										break;
									}
									case carbonylDerivative:
									case acidReplacingFunctionalGroup:
										//e.g. acetone 4,4-diphenylsemicarbazone. This is better expressed as a full word as the substituent actually locants onto the functional term
										for (int j = 1; j < (wordsInWordRule - 1); j++)
										{
											Element *wordEl = wordEls[i + j];
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
											if (WordType::substituent->toString()->equals(wordEl->getAttributeValue(XmlDeclarations::TYPE_ATR)))
											{
												joinWords(wordEls, wordEls[i + j], wordEls[i + j + 1]);
												wordsInWordRule--;
												std::vector<Element*> functionalTerm = OpsinTools::getDescendantElementsWithTagName(wordEls[i + j], XmlDeclarations::FUNCTIONALTERM_EL); //rename functionalTerm element to root
												if (functionalTerm.size() != 1)
												{
													throw ParsingException(std::wstring(L"OPSIN bug: Problem with ") + wordRule + std::wstring(L" wordRule"));
												}
												functionalTerm[0]->setName(XmlDeclarations::ROOT_EL);
												std::vector<Element*> functionalGroups = OpsinTools::getDescendantElementsWithTagName(functionalTerm[0], XmlDeclarations::FUNCTIONALGROUP_EL); //rename functionalGroup element to group
												if (functionalGroups.size() != 1)
												{
													throw ParsingException(std::wstring(L"OPSIN bug: Problem with ") + wordRule + std::wstring(L" wordRule"));
												}
												functionalGroups[0]->setName(XmlDeclarations::GROUP_EL);
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
												wordEls[i + j]->getAttribute(XmlDeclarations::TYPE_ATR).setValue(WordType::full->toString());
											}
										}
										break;
									case additionCompound:
									case oxide:
									{
										//is the halide/pseudohalide/oxide actually a counterion rather than covalently bonded
										Element *possibleElementaryAtomContainingWord = wordEls[i];
										std::vector<Element*> elementaryAtoms = OpsinTools::getDescendantElementsWithTagNameAndAttribute(possibleElementaryAtomContainingWord, XmlDeclarations::GROUP_EL, XmlDeclarations::SUBTYPE_ATR, XmlDeclarations::ELEMENTARYATOM_SUBTYPE_VAL);
										if (elementaryAtoms.size() == 1)
										{
											Element *elementaryAtom = elementaryAtoms[0];
											ChemEl chemEl1 = getChemElFromElementaryAtomEl(elementaryAtom);
											if (wordRule == WordRule::oxide)
											{
												if (wordsInWordRule != 2)
												{
													throw ParsingException(std::wstring(L"OPSIN bug: Problem with ") + wordRule + std::wstring(L" wordRule"));
												}
												Element *oxideWord = wordEls[i + 1];
												ChemEl chemEl2 = getChemElFromWordWithFunctionalGroup(oxideWord);
												if (!FragmentTools::isCovalent(chemEl1, chemEl2))
												{
													Element *oxideGroup = convertFunctionalGroupIntoGroup(oxideWord);
													setOxideStructureAppropriately(oxideGroup, elementaryAtom);
													applySimpleWordRule(wordEls, indexOfFirstWord, possibleElementaryAtomContainingWord);
													goto wordRuleLoopContinue;
												}
											}
											else
											{
												for (int j = 1; j < wordsInWordRule; j++)
												{
													Element *functionalGroup = wordEls[i + j];
													ChemEl chemEl2 = getChemElFromWordWithFunctionalGroup(functionalGroup);
													if (!FragmentTools::isCovalent(chemEl1, chemEl2))
													{ //use separate word rules for ionic components
														bool specialCaseCovalency = false;
														if (chemEl2::isHalogen() && wordsInWordRule == 2)
														{
															switch (chemEl1)
															{
															case Mg:
																if (possibleElementaryAtomContainingWord->getChildCount() > 1)
																{
																	//treat grignards (i.e. substitutedmagnesium halides) as covalent
																	specialCaseCovalency = true;
																}
																break;
															case Al:
																if (chemEl2 == ChemEl::Cl || chemEl2 == ChemEl::Br || chemEl2 == ChemEl::I)
																{
																	specialCaseCovalency = true;
																}
																break;
															case Ti:
																if (oxidationNumberOrMultiplierIs(elementaryAtom, functionalGroup, 4) && (chemEl2 == ChemEl::Cl || chemEl2 == ChemEl::Br || chemEl2 == ChemEl::I))
																{
																	specialCaseCovalency = true;
																}
																break;
															case V:
																if (oxidationNumberOrMultiplierIs(elementaryAtom, functionalGroup, 4) && chemEl2 == ChemEl::Cl)
																{
																	specialCaseCovalency = true;
																}
																break;
															case Zr:
															case Hf:
																if (oxidationNumberOrMultiplierIs(elementaryAtom, functionalGroup, 4) && chemEl2 == ChemEl::Br)
																{
																	specialCaseCovalency = true;
																}
																break;
															case U:
																if (oxidationNumberOrMultiplierIs(elementaryAtom, functionalGroup, 6) && (chemEl2 == ChemEl::F || chemEl2 == ChemEl::Cl))
																{
																	specialCaseCovalency = true;
																}
																break;
															case Np:
															case Pu:
																if (oxidationNumberOrMultiplierIs(elementaryAtom, functionalGroup, 6) && chemEl2 == ChemEl::F)
																{
																	specialCaseCovalency = true;
																}
																break;
															default:
																break;
															}
														}
														else if (chemEl2 == ChemEl::H && wordsInWordRule == 2)
														{
															if (chemEl1 == ChemEl::Al)
															{
																//aluminium hydrides are covalent
																specialCaseCovalency = true;
															}
														}
														if (!specialCaseCovalency)
														{
															goto wordRuleLoopContinue;
														}
													}
												}
											}
										}
										break;
									}
									case potentialAlcoholEster:
										if (expectedNumOfComponents && expectedNumOfComponents == moleculeEl->getChildCount())
										{
											//don't apply this wordRule if doing so makes the number of components incorrect
											goto wordRuleLoopContinue;
										}
										break;
									default:
										break;
									}

									std::vector<std::wstring> wordValues;
									Element *parentEl = wordEls[i]->getParent();
									int indexToInsertAt = parentEl->find(wordEls[i]);
									for (int j = 0; j < wordsInWordRule; j++)
									{
										Element *wordEl = wordEls.erase(wordEls.begin() + i);
										wordEl->detach();
										wordRuleEl->addChild(wordEl);
										wordValues.push_back(wordEl->getAttributeValue(XmlDeclarations::VALUE_ATR));
									}
									Attribute tempVar3(XmlDeclarations::VALUE_ATR, StringTools::stringListToString(wordValues, L" "));
									wordRuleEl->addAttribute(&tempVar3); //The bare string of all the words under this wordRule
									parentEl->insertChild(wordRuleEl, indexToInsertAt);
									wordEls.push_back(i, wordRuleEl);
									return true;
								}
								wordRuleLoopContinue:
							}
							wordRuleLoopBreak:
							Element *firstWord = wordEls[indexOfFirstWord];
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
							if (firstWord->getName() == XmlDeclarations::WORD_EL && WordType::full->toString()->equals(firstWord->getAttributeValue(XmlDeclarations::TYPE_ATR)))
							{ //No wordRule -->wordRule="simple"
								applySimpleWordRule(wordEls, indexOfFirstWord, firstWord);
								return false;
							}
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
							else if (allowSpaceRemoval && WordType::substituent->toString()->equals(firstWord->getAttributeValue(XmlDeclarations::TYPE_ATR)))
							{
								/*
								 * substituents may join together or to a full e.g. 2-ethyl toluene -->2-ethyltoluene
								 * 1-chloro 2-bromo ethane --> 1-chloro-2-bromo ethane then subsequently 1-chloro-2-bromo-ethane
								 */
								if (indexOfFirstWord + 1 < wordEls.size())
								{
									Element *wordToPotentiallyCombineWith = wordEls[indexOfFirstWord + 1];
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
									if (WordType::full->toString()->equals(wordToPotentiallyCombineWith->getAttributeValue(XmlDeclarations::TYPE_ATR)) || WordType::substituent->toString()->equals(wordToPotentiallyCombineWith->getAttributeValue(XmlDeclarations::TYPE_ATR)))
									{
										joinWords(wordEls, firstWord, wordToPotentiallyCombineWith);
										return true;
									}
								}
							}
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
							else if (WordType::functionalTerm->toString()->equals(firstWord->getAttributeValue(XmlDeclarations::TYPE_ATR)) && firstWord->getAttributeValue(XmlDeclarations::VALUE_ATR).toLowerCase(Locale::ROOT).equals(L"salt"))
							{
								wordEls.erase(wordEls.begin() + indexOfFirstWord);
								firstWord->detach();
								if (moleculeEl->getAttribute(XmlDeclarations::ISSALT_ATR) == nullptr)
								{
									moleculeEl->addAttribute(XmlDeclarations::ISSALT_ATR, L"yes");
								}
								return true;
							}
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
							if (allowRadicals && wordEls.size() == 1 && indexOfFirstWord == 0 && firstWord->getName() == XmlDeclarations::WORD_EL && WordType::substituent->toString()->equals(firstWord->getAttributeValue(XmlDeclarations::TYPE_ATR)))
							{
								//name is all one substituent, make this a substituent and finish
								applySubstituentWordRule(wordEls, indexOfFirstWord, firstWord);
							}
							return false;
						}

						bool WordRules::WordRulesInstance::endsWithGroupPredicateSatisfied(Element *wordEl, EndsWithGroup endsWithGroupPredicate) throw(ParsingException)
						{
							Element *lastEl = getLastElementInWord(wordEl);
							if (lastEl == nullptr)
							{
								return false;
							}
							std::wstring elName = lastEl->getName();
							while (elName == XmlDeclarations::CLOSEBRACKET_EL || elName == XmlDeclarations::STRUCTURALCLOSEBRACKET_EL || elName == XmlDeclarations::ISOTOPESPECIFICATION_EL)
							{
								lastEl = OpsinTools::getPreviousSibling(lastEl);
								if (lastEl == nullptr)
								{
									return false;
								}
								elName = lastEl->getName();
							}

							if (endsWithGroupPredicate == EndsWithGroup::acid)
							{
								if (elName == XmlDeclarations::SUFFIX_EL)
								{
									if (icOrOusAcid->matcher(lastEl->getAttributeValue(XmlDeclarations::VALUE_ATR)).find())
									{
										return true;
									}
								}
								else if (elName == XmlDeclarations::GROUP_EL)
								{
									if (lastEl->getAttribute(XmlDeclarations::FUNCTIONALIDS_ATR) != nullptr && icOrOusAcid->matcher(lastEl->getValue()).find())
									{
										return true;
									}
								}
							}
							else if (endsWithGroupPredicate == EndsWithGroup::ateGroup)
							{
								if (elName == XmlDeclarations::GROUP_EL)
								{
									if (lastEl->getAttribute(XmlDeclarations::FUNCTIONALIDS_ATR) != nullptr && ateOrIteOrAmide->matcher(lastEl->getValue()).find())
									{
										return true;
									}
								}
								else
								{
									while (lastEl != nullptr && elName == XmlDeclarations::SUFFIX_EL)
									{
										std::wstring suffixValAtr = lastEl->getAttributeValue(XmlDeclarations::VALUE_ATR);
										if (ateOrIteOrAmide->matcher(suffixValAtr).find() || suffixValAtr == L"glycoside")
										{
											return true;
										}
										//glycoside is not always the last suffix
										lastEl = OpsinTools::getPreviousSibling(lastEl, XmlDeclarations::SUFFIX_EL);
									}
								}
							}
							return false;
						}

						bool WordRules::WordRulesInstance::oxidationNumberOrMultiplierIs(Element *elementaryAtomEl, Element *functionalGroupWord, int expectedVal) throw(ParsingException)
						{
							std::vector<Element*> functionalGroups = OpsinTools::getDescendantElementsWithTagName(functionalGroupWord, XmlDeclarations::FUNCTIONALGROUP_EL);
							if (functionalGroups.size() != 1)
							{
								throw ParsingException(L"OPSIN bug: Unable to find functional group in oxide or addition compound rule");
							}
							Element *possibleMultiplier = OpsinTools::getPreviousSibling(functionalGroups[0]);
							if (possibleMultiplier != nullptr && possibleMultiplier->getName() == XmlDeclarations::MULTIPLIER_EL)
							{
								return std::stoi(possibleMultiplier->getAttributeValue(XmlDeclarations::VALUE_ATR)) == expectedVal;

							}
							else
							{
								Element *possibleOxidationNumber = OpsinTools::getNextSibling(elementaryAtomEl);
								if (possibleOxidationNumber != nullptr && possibleOxidationNumber->getName() == XmlDeclarations::OXIDATIONNUMBERSPECIFIER_EL)
								{
									return std::stoi(possibleOxidationNumber->getAttributeValue(XmlDeclarations::VALUE_ATR)) == expectedVal;
								}
							}
							return false;
						}

						Element *WordRules::WordRulesInstance::getLastGroupInWordRule(Element *wordEl)
						{
							Element *lastEl = getLastElementInWord(wordEl);
							if (lastEl->getName() == XmlDeclarations::GROUP_EL)
							{
								return lastEl;
							}
							else
							{
								std::vector<Element*> groups = lastEl->getParent()->getChildElements(XmlDeclarations::GROUP_EL);
								if (groups.size() > 0)
								{
									return groups[groups.size() - 1];
								}
							}
							return nullptr;
						}

						Element *WordRules::WordRulesInstance::getLastElementInWord(Element *wordEl)
						{
							std::vector<Element*> children = wordEl->getChildElements();
							Element *lastChild = children[children.size() - 1];
							while (lastChild->getChildCount() != 0)
							{
								children = lastChild->getChildElements();
								lastChild = children[children.size() - 1];
							}
							return lastChild;
						}

						void WordRules::WordRulesInstance::applySimpleWordRule(std::vector<Element*> &wordEls, int indexOfFirstWord, Element *firstWord)
						{
							Element *parentEl = firstWord->getParent();
							int indexToInsertAt = parentEl->find(firstWord);
							Element *wordRuleEl = new GroupingEl(XmlDeclarations::WORDRULE_EL);
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
							Attribute tempVar(XmlDeclarations::WORDRULE_ATR, WordRule::simple->toString());
							wordRuleEl->addAttribute(&tempVar); //No wordRule
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
							Attribute tempVar2(XmlDeclarations::TYPE_ATR, WordType::full->toString());
							wordRuleEl->addAttribute(&tempVar2);
							Attribute tempVar3(XmlDeclarations::VALUE_ATR, firstWord->getAttributeValue(XmlDeclarations::VALUE_ATR));
							wordRuleEl->addAttribute(&tempVar3);
							firstWord->detach();
							wordRuleEl->addChild(firstWord);
							wordEls[indexOfFirstWord] = wordRuleEl;
							parentEl->insertChild(wordRuleEl, indexToInsertAt);
						}

						void WordRules::WordRulesInstance::applySubstituentWordRule(std::vector<Element*> &wordEls, int indexOfFirstWord, Element *firstWord)
						{
							Element *parentEl = firstWord->getParent();
							int indexToInsertAt = parentEl->find(firstWord);
							Element *wordRuleEl = new GroupingEl(XmlDeclarations::WORDRULE_EL);
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
							Attribute tempVar(XmlDeclarations::WORDRULE_ATR, WordRule::substituent->toString());
							wordRuleEl->addAttribute(&tempVar);
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
							Attribute tempVar2(XmlDeclarations::TYPE_ATR, WordType::full->toString());
							wordRuleEl->addAttribute(&tempVar2);
							Attribute tempVar3(XmlDeclarations::VALUE_ATR, firstWord->getAttributeValue(XmlDeclarations::VALUE_ATR));
							wordRuleEl->addAttribute(&tempVar3);
							firstWord->detach();
							wordRuleEl->addChild(firstWord);
							wordEls[indexOfFirstWord] = wordRuleEl;
							parentEl->insertChild(wordRuleEl, indexToInsertAt);
						}

						void WordRules::WordRulesInstance::joinWords(std::vector<Element*> &wordEls, Element *firstWord, Element *wordToPotentiallyCombineWith) throw(ParsingException)
						{
//JAVA TO C++ CONVERTER TODO TASK: The Java Collection 'remove(Object)' method is not converted:
							wordEls.remove(wordToPotentiallyCombineWith);
							wordToPotentiallyCombineWith->detach();
							std::vector<Element*> substituentEls = firstWord->getChildElements(XmlDeclarations::SUBSTITUENT_EL);
							if (substituentEls.empty())
							{
								throw ParsingException(L"OPSIN Bug: Substituent element not found where substituent element expected");
							}
							Element *finalSubstituent = substituentEls[substituentEls.size() - 1];
							std::vector<Element*> finalSubstituentChildren = finalSubstituent->getChildElements();
							if (finalSubstituentChildren[finalSubstituentChildren.size() - 1]->getName() != XmlDeclarations::HYPHEN_EL)
							{ //add an implicit hyphen if one is not already present
								Element *implicitHyphen = new TokenEl(XmlDeclarations::HYPHEN_EL, L"-");
								finalSubstituent->addChild(implicitHyphen);
							}
							std::vector<Element*> elementsToMergeIntoSubstituent = wordToPotentiallyCombineWith->getChildElements();
							for (int j = elementsToMergeIntoSubstituent.size() - 1 ; j >= 0; j--)
							{
								Element *el = elementsToMergeIntoSubstituent[j];
								el->detach();
								OpsinTools::insertAfter(finalSubstituent, el);
							}
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
							if (WordType::full->toString()->equals(wordToPotentiallyCombineWith->getAttributeValue(XmlDeclarations::TYPE_ATR)))
							{
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
								firstWord->getAttribute(XmlDeclarations::TYPE_ATR)->setValue(WordType::full->toString());
							}
							firstWord->getAttribute(XmlDeclarations::VALUE_ATR)->setValue(firstWord->getAttributeValue(XmlDeclarations::VALUE_ATR) + wordToPotentiallyCombineWith->getAttributeValue(XmlDeclarations::VALUE_ATR));
						}

						Element *WordRules::WordRulesInstance::convertFunctionalGroupIntoGroup(Element *word) throw(ParsingException)
						{
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
							word->getAttribute(XmlDeclarations::TYPE_ATR)->setValue(WordType::full->toString());
							std::vector<Element*> functionalTerms = OpsinTools::getDescendantElementsWithTagName(word, XmlDeclarations::FUNCTIONALTERM_EL);
							if (functionalTerms.size() != 1)
							{
								throw ParsingException(L"OPSIN Bug: Exactly 1 functionalTerm expected in functionalGroupAsGroup wordRule");
							}
							Element *functionalTerm = functionalTerms[0];
							functionalTerm->setName(XmlDeclarations::ROOT_EL);
							std::vector<Element*> functionalGroups = functionalTerm->getChildElements(XmlDeclarations::FUNCTIONALGROUP_EL);
							if (functionalGroups.size() != 1)
							{
								throw ParsingException(L"OPSIN Bug: Exactly 1 functionalGroup expected in functionalGroupAsGroup wordRule");
							}
							Element *functionalGroup = functionalGroups[0];
							functionalGroup->setName(XmlDeclarations::GROUP_EL);
							functionalGroup->getAttribute(XmlDeclarations::TYPE_ATR)->setValue(XmlDeclarations::SIMPLEGROUP_TYPE_VAL);
							Attribute tempVar(XmlDeclarations::SUBTYPE_ATR, XmlDeclarations::SIMPLEGROUP_SUBTYPE_VAL);
							functionalGroup->addAttribute(&tempVar);
							return functionalGroup;
						}

						void WordRules::WordRulesInstance::setOxideStructureAppropriately(Element *oxideGroup, Element *elementaryAtom)
						{
							bool chainInterpretation = false;
							boost::optional<int> multiplierVal = boost::none;
							Element *possibleMultiplier = OpsinTools::getPreviousSibling(oxideGroup);
							if (possibleMultiplier != nullptr && possibleMultiplier->getName() == XmlDeclarations::MULTIPLIER_EL)
							{
								multiplierVal = std::stoi(possibleMultiplier->getAttributeValue(XmlDeclarations::VALUE_ATR));
								if (multiplierVal > 1)
								{
									std::wstring commonOxidationStatesAndMax = elementaryAtom->getAttributeValue(XmlDeclarations::COMMONOXIDATIONSTATESANDMAX_ATR);
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
									if (commonOxidationStatesAndMax == L"" || static_cast<Integer>(commonOxidationStatesAndMax.split(L":")[1]) <= 2)
									{
										chainInterpretation = true;
									}
								}
							}

							Attribute *value = oxideGroup->getAttribute(XmlDeclarations::VALUE_ATR);
							std::wstring smiles = value->getValue();
							std::wstring element;
							if (smiles == L"O")
							{
								element = L"O";
							}
							else if (smiles == L"S")
							{
								element = L"S";
							}
							else if (boost::starts_with(smiles, L"[Se"))
							{
								element = L"Se";
							}
							else if (boost::starts_with(smiles, L"[Te"))
							{
								element = L"Te";
							}
							else
							{
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("OPSIN Bug: Unexpected smiles for oxideGroup: " + smiles);
								throw std::exception();
							}
							if (chainInterpretation)
							{
								StringBuilder *sb = new StringBuilder();
								sb->append(L'[');
								sb->append(element);
								sb->append(L"-]");
								for (int i = 2; i < multiplierVal; i++)
								{
									sb->append(L'[');
									sb->append(element);
									sb->append(L']');
								}
								sb->append(L'[');
								sb->append(element);
								sb->append(L"-]");
								value->setValue(sb->toString());
								possibleMultiplier->detach();
							}
							else
							{
								value->setValue(std::wstring(L"[") + element + std::wstring(L"-2]"));
							}
						}

						ChemEl WordRules::WordRulesInstance::getChemElFromElementaryAtomEl(Element *elementaryAtomEl)
						{
							std::wstring elementStr = elementaryAtomEl->getAttributeValue(XmlDeclarations::VALUE_ATR);
							if (boost::starts_with(elementStr, L"["))
							{
								int len = elementStr.length() - 1;
								for (int i = 1; i < len; i++)
								{
									wchar_t ch = elementStr[i];
									if ((ch >= L'A' && ch <= L'Z') || (ch >= L'a' && ch <= L'z'))
									{
										if (i + 1 < len)
										{
											wchar_t ch2 = elementStr[i + 1];
											if ((ch2 >= L'A' && ch2 <= L'Z') || (ch2 >= L'a' && ch2 <= L'z'))
											{
												//two letter element
												elementStr = elementStr.substr(i, 2);
												break;
											}
										}
										//one letter element
										elementStr = elementStr.substr(i, 1);
										break;
									}
								}
							}
							return StringHelper::fromString<ChemEl>(elementStr);
						}

						ChemEl WordRules::WordRulesInstance::getChemElFromWordWithFunctionalGroup(Element *functionalWord) throw(ParsingException)
						{
							std::vector<Element*> functionalGroups = OpsinTools::getDescendantElementsWithTagName(functionalWord, XmlDeclarations::FUNCTIONALGROUP_EL);
							if (functionalGroups.size() != 1)
							{
								throw ParsingException(L"OPSIN bug: Unable to find functional group in oxide or addition compound rule");
							}
							std::wstring smiles = functionalGroups[0]->getAttributeValue(XmlDeclarations::VALUE_ATR);
							std::wstring elementStr = L"";
							for (int i = 0; i < smiles.length(); i++)
							{
								if (std::isupper(smiles[i]))
								{
									elementStr += smiles[i];
									if (i + 1 < smiles.length() && std::islower(smiles[i + 1]))
									{
										elementStr += smiles[i + 1];
									}
									break;
								}
							}
							return StringHelper::fromString<ChemEl>(elementStr);
						}
					}
				}
			}
		}
	}
}
