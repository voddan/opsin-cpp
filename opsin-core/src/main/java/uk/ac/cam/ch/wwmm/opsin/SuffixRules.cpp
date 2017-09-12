#include "SuffixRules.h"
#include "SuffixRule.h"
#include "ResourceGetter.h"
#include "XmlDeclarations.h"
#include "SuffixRuleType.h"
#include "Attribute.h"
#include "ComponentGenerationException.h"

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

						SuffixRules::ApplicableSuffix::ApplicableSuffix(const std::wstring &requiredSubType, std::vector<SuffixRule*> &suffixRules) : requiredSubType(requiredSubType), suffixRules(suffixRules)
						{
						}

						SuffixRules::SuffixRules(ResourceGetter *resourceGetter) throw(IOException) : suffixApplicability(generateSuffixApplicabilityMap(resourceGetter, suffixRulesMap))
						{
							std::unordered_map<std::wstring, std::vector<SuffixRule*>> suffixRulesMap = generateSuffixRulesMap(resourceGetter);
						}

						std::unordered_map<std::wstring, std::vector<SuffixRule*>> SuffixRules::generateSuffixRulesMap(ResourceGetter *resourceGetter) throw(IOException)
						{
							std::unordered_map<std::wstring, std::vector<SuffixRule*>> suffixRulesMap;
							XMLStreamReader *reader = resourceGetter->getXMLStreamReader(L"suffixRules.xml");
							try
							{
								while (reader->hasNext())
								{
									if (reader->next() == XMLStreamConstants::START_ELEMENT && reader->getLocalName().equals(XmlDeclarations::SUFFIXRULES_RULE_EL))
									{
										std::wstring ruleValue = reader->getAttributeValue(nullptr, XmlDeclarations::SUFFIXRULES_VALUE_ATR);
										if (suffixRulesMap[ruleValue].size() > 0)
										{
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("Suffix: " + ruleValue + " appears multiple times in suffixRules.xml");
											throw std::exception();
										}
										suffixRulesMap[ruleValue] = processSuffixRules(reader);
									}
									reader++;
								}
							}
							catch (const XMLStreamException &e)
							{
								throw IOException(L"Parsing exception occurred while reading suffixRules.xml", e);
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
									throw IOException(L"Parsing exception occurred while reading suffixRules.xml", e);
								}
							}
							return suffixRulesMap;
						}

						std::vector<SuffixRule*> SuffixRules::processSuffixRules(XMLStreamReader *reader) throw(XMLStreamException)
						{
							std::wstring startingElName = reader->getLocalName();
							std::vector<SuffixRule*> rules;
							while (reader->hasNext())
							{
								switch (reader->next())
								{
								case XMLStreamConstants::START_ELEMENT:
								{
									std::wstring tagName = reader->getLocalName();
									SuffixRuleType type = StringHelper::fromString<SuffixRuleType>(tagName);
									std::vector<Attribute*> attributes;
									for (int i = 0, l = reader->getAttributeCount(); i < l; i++)
									{
										Attribute tempVar(reader->getAttributeLocalName(i), reader->getAttributeValue(i));
										attributes.push_back(&tempVar);
									}
									SuffixRule tempVar2(type, attributes);
									rules.push_back(&tempVar2);
									break;
								}
								case XMLStreamConstants::END_ELEMENT:
									if (reader->getLocalName().equals(startingElName))
									{
										return rules;
									}
									break;
								}
								reader++;
							}
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("Malformed suffixRules.xml");
							throw std::exception();
						}

						std::unordered_map<std::wstring, std::unordered_map<std::wstring, std::vector<ApplicableSuffix*>>> SuffixRules::generateSuffixApplicabilityMap(ResourceGetter *resourceGetter, std::unordered_map<std::wstring, std::vector<SuffixRule*>> &suffixRulesMap) throw(IOException)
						{
							std::unordered_map<std::wstring, std::unordered_map<std::wstring, std::vector<ApplicableSuffix*>>> suffixApplicability;
							XMLStreamReader *reader = resourceGetter->getXMLStreamReader(L"suffixApplicability.xml");
							try
							{
								while (reader->hasNext())
								{
									if (reader->next() == XMLStreamConstants::START_ELEMENT && reader->getLocalName().equals(XmlDeclarations::SUFFIXAPPLICABILITY_GROUPTYPE_EL))
									{
										std::unordered_map<std::wstring, std::vector<ApplicableSuffix*>> suffixToRuleMap;
										suffixApplicability[reader->getAttributeValue(nullptr, XmlDeclarations::SUFFIXAPPLICABILITY_TYPE_ATR)] = suffixToRuleMap;
										while (reader->hasNext())
										{
											int event_Renamed = reader->next();
											if (event_Renamed == XMLStreamConstants::START_ELEMENT && reader->getLocalName().equals(XmlDeclarations::SUFFIXAPPLICABILITY_SUFFIX_EL))
											{
												std::wstring suffixValue = reader->getAttributeValue(nullptr, XmlDeclarations::SUFFIXAPPLICABILITY_VALUE_ATR);
												std::vector<ApplicableSuffix*> suffixList = suffixToRuleMap[suffixValue];
												//can have multiple entries if subType attribute is set
												if (suffixToRuleMap[suffixValue].empty())
												{
													suffixList = std::vector<ApplicableSuffix*>();
													suffixToRuleMap[suffixValue] = suffixList;
												}
												std::wstring requiredSubType = reader->getAttributeValue(nullptr, XmlDeclarations::SUFFIXAPPLICABILITY_SUBTYPE_ATR);
												std::wstring suffixRuleName = reader->getElementText();
												std::vector<SuffixRule*> suffixRules = suffixRulesMap[suffixRuleName];
												if (suffixRules.empty())
												{
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("Suffix: " + suffixRuleName +" does not have a rule associated with it in suffixRules.xml");
													throw std::exception();
												}
												ApplicableSuffix tempVar(requiredSubType, suffixRules);
												suffixList.push_back(&tempVar);
											}
											else if (event_Renamed == XMLStreamConstants::END_ELEMENT && reader->getLocalName().equals(XmlDeclarations::SUFFIXAPPLICABILITY_GROUPTYPE_EL))
											{
												break;
											}
											reader++;
										}
									}
									reader++;
								}
							}
							catch (const XMLStreamException &e)
							{
								throw IOException(L"Parsing exception occurred while reading suffixApplicability.xml", e);
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
									throw IOException(L"Parsing exception occurred while reading suffixApplicability.xml", e);
								}
							}
							return suffixApplicability;
						}

						std::vector<SuffixRule*> SuffixRules::getSuffixRuleTags(const std::wstring &suffixTypeToUse, const std::wstring &suffixValue, const std::wstring &subgroupType) throw(ComponentGenerationException)
						{
							std::unordered_map<std::wstring, std::vector<ApplicableSuffix*>> groupToSuffixMap = suffixApplicability[suffixTypeToUse];
							if (groupToSuffixMap.empty())
							{
								throw ComponentGenerationException(std::wstring(L"Suffix Type: ") + suffixTypeToUse + std::wstring(L" does not have a corresponding groupType entry in suffixApplicability.xml"));
							}
							std::vector<ApplicableSuffix*> potentiallyApplicableSuffixes = groupToSuffixMap[suffixValue];
							if (potentiallyApplicableSuffixes.empty() || potentiallyApplicableSuffixes.empty())
							{
								throw ComponentGenerationException(std::wstring(L"Suffix: ") + suffixValue + std::wstring(L" does not apply to the group it was associated with (type: ") + suffixTypeToUse + std::wstring(L") according to suffixApplicability.xml"));
							}
							std::vector<SuffixRule*> suffixRules;
							for (auto suffix : potentiallyApplicableSuffixes)
							{
								if (suffix->requiredSubType != nullptr)
								{
									if (!suffix->requiredSubType.equals(subgroupType))
									{
										continue;
									}
								}
								if (suffixRules.size() > 0)
								{
									throw ComponentGenerationException(std::wstring(L"Suffix: ") + suffixValue + std::wstring(L" appears multiple times in suffixApplicability.xml"));
								}
								suffixRules = suffix->suffixRules;
							}
							if (suffixRules.empty())
							{
								throw ComponentGenerationException(std::wstring(L"Suffix: ") + suffixValue + std::wstring(L" does not apply to the group it was associated with (type: ") + suffixTypeToUse + std::wstring(L") due to the group's subType: ") + subgroupType + std::wstring(L" according to suffixApplicability.xml"));
							}
							return suffixRules;
						}

						bool SuffixRules::isGroupTypeWithSpecificSuffixRules(const std::wstring &groupType)
						{
							return suffixApplicability.find(groupType) != suffixApplicability.end();
						}
					}
				}
			}
		}
	}
}
