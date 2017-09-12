#include "ResourceManager.h"
#include "TokenEl.h"
#include "ResourceGetter.h"
#include "AutomatonInitialiser.h"
#include "OpsinRadixTrie.h"
#include "XmlDeclarations.h"
#include "ParsingException.h"

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
						using dk::brics::automaton::RunAutomaton;
						using namespace uk::ac::cam::ch::wwmm::opsin;
//						import static uk.ac.cam.ch.wwmm.opsin.XmlDeclarations.*;

						ResourceManager::ResourceManager(ResourceGetter *resourceGetter) throw(IOException) : resourceGetter(resourceGetter), automatonInitialiser(new AutomatonInitialiser(resourceGetter->getResourcePath() + std::wstring(L"serialisedAutomata/"))), symbolTokenNamesDict(std::vector<OpsinRadixTrie*>(grammarSymbolsSize)), symbolRegexAutomataDict(std::vector<RunAutomaton*>(grammarSymbolsSize)), symbolRegexesDict(std::vector<Pattern*>(grammarSymbolsSize)), chemicalAutomaton(processChemicalGrammar(false))
						{
							int grammarSymbolsSize = chemicalAutomaton->getCharIntervals()->length;
							processTokenFiles(false);
							processRegexTokenFiles(false);
						}

						void ResourceManager::processTokenFiles(bool reversed) throw(IOException)
						{
							XMLStreamReader *filesToProcessReader = resourceGetter->getXMLStreamReader(L"index.xml");
							try
							{
								while (filesToProcessReader->hasNext())
								{
									int event_Renamed = filesToProcessReader->next();
									if (event_Renamed == XMLStreamConstants::START_ELEMENT && filesToProcessReader->getLocalName().equals(L"tokenFile"))
									{
										std::wstring fileName = filesToProcessReader->getElementText();
										processTokenFile(fileName, reversed);
									}
									filesToProcessReader++;
								}
							}
							catch (const XMLStreamException &e)
							{
								throw IOException(L"Parsing exception occurred while reading index.xml", e);
							}
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to the exception 'finally' clause:
							finally
							{
								try
								{
									delete filesToProcessReader;
								}
								catch (const XMLStreamException &e)
								{
									throw IOException(L"Parsing exception occurred while reading index.xml", e);
								}
							}
						}

						void ResourceManager::processTokenFile(const std::wstring &fileName, bool reversed) throw(IOException)
						{
							XMLStreamReader *reader = resourceGetter->getXMLStreamReader(fileName);
							try
							{
								while (reader->hasNext())
								{
									if (reader->next() == XMLStreamConstants::START_ELEMENT)
									{
										std::wstring tagName = reader->getLocalName();
										if (tagName == L"tokenLists")
										{
											while (reader->hasNext())
											{
												switch (reader->next())
												{
												case XMLStreamConstants::START_ELEMENT:
													if (reader->getLocalName().equals(L"tokenList"))
													{
														processTokenList(reader, reversed);
													}
													break;
												}
												reader++;
											}
										}
										else if (tagName == L"tokenList")
										{
											processTokenList(reader, reversed);
										}
									}
									reader++;
								}
							}
							catch (const XMLStreamException &e)
							{
								throw IOException(std::wstring(L"Parsing exception occurred while reading ") + fileName, e);
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
									throw IOException(std::wstring(L"Parsing exception occurred while reading ") + fileName, e);
								}
							}
						}

						void ResourceManager::processTokenList(XMLStreamReader *reader, bool reversed) throw(XMLStreamException)
						{
							std::wstring tokenTagName = L"";
							boost::optional<wchar_t> symbol = boost::none;
							std::wstring type = L"";
							std::wstring subType = L"";
							bool ignoreWhenWritingXML = false;

							for (int i = 0, l = reader->getAttributeCount(); i < l; i++)
							{
								std::wstring atrName = reader->getAttributeLocalName(i);
								std::wstring atrValue = reader->getAttributeValue(i);
								if (atrName == L"tagname")
								{
									tokenTagName = atrValue;
								}
								else if (atrName == L"symbol")
								{
									symbol = atrValue[0];
								}
								else if (atrName == XmlDeclarations::TYPE_ATR)
								{
									type = atrValue;
								}
								else if (atrName == XmlDeclarations::SUBTYPE_ATR)
								{
									subType = atrValue;
								}
								else if (atrName == L"ignoreWhenWritingXML")
								{
									ignoreWhenWritingXML = atrValue == L"yes";
								}
								else
								{
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("Malformed tokenlist");
									throw std::exception();
								}
							}
							if (tokenTagName == L"" || !symbol)
							{
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("Malformed tokenlist");
								throw std::exception();
							}

							int index = Arrays::binarySearch(chemicalAutomaton->getCharIntervals(), symbol);
							if (index < 0)
							{
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException(symbol +" is associated with a tokenList of tagname " + tokenTagName +" however it is not actually used in OPSIN's grammar!!!");
								throw std::exception();
							}

							while (reader->hasNext())
							{
								switch (reader->next())
								{
								case XMLStreamConstants::START_ELEMENT:
									if (reader->getLocalName().equals(L"token"))
									{
										TokenEl *el;
										if (ignoreWhenWritingXML)
										{
											el = IGNORE_WHEN_WRITING_PARSE_TREE;
										}
										else
										{
											el = new TokenEl(tokenTagName);
											if (type != L"")
											{
												el->addAttribute(XmlDeclarations::TYPE_ATR, type);
											}
											if (subType != L"")
											{
												el->addAttribute(XmlDeclarations::SUBTYPE_ATR, subType);
											}
											for (int i = 0, l = reader->getAttributeCount(); i < l; i++)
											{
												el->addAttribute(reader->getAttributeLocalName(i), reader->getAttributeValue(i));
											}
										}
										std::wstring text = reader->getElementText();
										StringBuilder *sb = new StringBuilder(text.length());
										for (int i = 0, len = text.length(); i < len; i++)
										{
											wchar_t ch = text[i];
											if (ch == L'\\')
											{
												if (i + 1 >= len)
												{
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("Malformed token text: " + text);
													throw std::exception();
												}
												ch = text[++i];
											}
											else if (ch == L'|')
											{
												addToken(sb->toString(), el, symbol, index, reversed);
												sb->setLength(0);
												continue;
											}
											sb->append(ch);
										}
										addToken(sb->toString(), el, symbol, index, reversed);
									}
									break;
								case XMLStreamConstants::END_ELEMENT:
									if (reader->getLocalName().equals(L"tokenList"))
									{
										return;
									}
									break;
								}
								reader++;
							}
						}

						void ResourceManager::addToken(const std::wstring &text, TokenEl *el, boost::optional<wchar_t> symbol, int index, bool reversed)
						{
							std::unordered_map<wchar_t, TokenEl*> symbolToToken = tokenDict[text];
							if (symbolToToken.empty())
							{
								symbolToToken = std::unordered_map<wchar_t, TokenEl*>();
								tokenDict[text] = symbolToToken;
							}
							symbolToToken[symbol] = el;

							if (!reversed)
							{
								OpsinRadixTrie *trie = symbolTokenNamesDict[index];
								if (trie == nullptr)
								{
									trie = new OpsinRadixTrie();
									symbolTokenNamesDict[index] = trie;
								}
								trie->addToken(text);
							}
							else
							{
								OpsinRadixTrie *trie = symbolTokenNamesDictReversed[index];
								if (trie == nullptr)
								{
									trie = new OpsinRadixTrie();
									symbolTokenNamesDictReversed[index] = trie;
								}
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
								StringBuilder tempVar(text);
								trie->addToken((&tempVar)->reverse()->toString());
							}
						}

						void ResourceManager::processRegexTokenFiles(bool reversed) throw(IOException)
						{
							XMLStreamReader *reader = resourceGetter->getXMLStreamReader(L"regexTokens.xml");
							std::unordered_map<std::wstring, StringBuilder*> tempRegexes;
							Pattern *matchRegexReplacement = Pattern::compile(L"%.*?%");
							try
							{
								while (reader->hasNext())
								{
									if (reader->next() == XMLStreamConstants::START_ELEMENT)
									{
										std::wstring localName = reader->getLocalName();
										if (localName != L"regex" && localName != L"regexToken")
										{
											continue;
										}
										std::wstring re = reader->getAttributeValue(nullptr, L"regex");
										Matcher *m = matchRegexReplacement->matcher(re);
										StringBuilder *newValueSB = new StringBuilder();
										int position = 0;
										while (m->find())
										{ //replace sections enclosed in %..% with the appropriate regex
											newValueSB->append(re.substr(position, m->start() - position));
											StringBuilder *replacement = tempRegexes[m->group()];
											if (replacement == nullptr)
											{
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("Regex entry for: " + m.group() + " missing! Check regexTokens.xml");
												throw std::exception();
											}
											newValueSB->append(replacement);
											position = m->end();
										}
										newValueSB->append(re.substr(position));
										if (localName == L"regex")
										{
											std::wstring regexName = reader->getAttributeValue(nullptr, L"name");
											if (regexName == L"")
											{
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("Regex entry in regexTokenes.xml with no name. regex: " + newValueSB.toString());
												throw std::exception();
											}
											tempRegexes[regexName] = newValueSB;
											continue;
										}
										addRegexToken(reader, newValueSB->toString(), reversed);
									}
									reader++;
								}
							}
							catch (const XMLStreamException &e)
							{
								throw IOException(L"Parsing exception occurred while reading regexTokens.xml", e);
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
									throw IOException(L"Parsing exception occurred while reading regexTokens.xml", e);
								}
							}
						}

						void ResourceManager::addRegexToken(XMLStreamReader *reader, const std::wstring &regex, bool reversed)
						{
							std::wstring tokenTagName = L"";
							boost::optional<wchar_t> symbol = boost::none;
							std::wstring type = L"";
							std::wstring subType = L"";
							std::wstring value = L"";
							bool determinise = false;
							bool ignoreWhenWritingXML = false;

							for (int i = 0, l = reader->getAttributeCount(); i < l; i++)
							{
								std::wstring atrName = reader->getAttributeLocalName(i);
								std::wstring atrValue = reader->getAttributeValue(i);
								if (atrName == L"tagname")
								{
									tokenTagName = atrValue;
								}
								else if (atrName == L"symbol")
								{
									symbol = atrValue[0];
								}
								else if (atrName == XmlDeclarations::TYPE_ATR)
								{
									type = atrValue;
								}
								else if (atrName == XmlDeclarations::SUBTYPE_ATR)
								{
									subType = atrValue;
								}
								else if (atrName == L"value")
								{
									value = atrValue;
								}
								else if (atrName == L"determinise")
								{
									determinise = atrValue == L"yes";
								}
								else if (atrName == L"ignoreWhenWritingXML")
								{
									ignoreWhenWritingXML = atrValue == L"yes";
								}
								else if (atrName != L"regex")
								{
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("Malformed regexToken");
									throw std::exception();
								}
							}
							if (tokenTagName == L"" || !symbol)
							{
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("Malformed regexToken");
								throw std::exception();
							}

							if (!reversed)
							{
								//reSymbolTokenDict will be populated when the constructor is called for left-right parsing, hence skip for right-left 
								if (reSymbolTokenDict[symbol] != nullptr)
								{
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException(symbol +" is associated with multiple regular expressions. The following expression clashes: " + regex +" This should be resolved by combining regular expressions that map the same symbol");
									throw std::exception();
								}

								if (ignoreWhenWritingXML)
								{
									reSymbolTokenDict[symbol] = IGNORE_WHEN_WRITING_PARSE_TREE;
								}
								else
								{
									TokenEl *el = new TokenEl(tokenTagName);
									if (type != L"")
									{
										el->addAttribute(XmlDeclarations::TYPE_ATR, type);
									}
									if (subType != L"")
									{
										el->addAttribute(XmlDeclarations::SUBTYPE_ATR, subType);
									}
									if (value != L"")
									{
										el->addAttribute(XmlDeclarations::VALUE_ATR, value);
									}
									reSymbolTokenDict[symbol] = el;
								}
							}

							int index = Arrays::binarySearch(chemicalAutomaton->getCharIntervals(), symbol);
							if (index < 0)
							{
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException(symbol +" is associated with the regex " + regex +" however it is not actually used in OPSIN's grammar!!!");
								throw std::exception();
							}
							if (!reversed)
							{
								if (determinise)
								{ //should the regex be compiled into a DFA for faster execution?
									symbolRegexAutomataDict[index] = automatonInitialiser->loadAutomaton(tokenTagName + std::wstring(L"_") + std::to_wstring(static_cast<int>(symbol)), regex, false, false);
								}
								else
								{
									symbolRegexesDict[index] = Pattern::compile(regex);
								}
							}
							else
							{
								if (determinise)
								{ //should the regex be compiled into a DFA for faster execution?
									symbolRegexAutomataDictReversed[index] = automatonInitialiser->loadAutomaton(tokenTagName + std::wstring(L"_") + std::to_wstring(static_cast<int>(symbol)), regex, false, true);
								}
								else
								{
									symbolRegexesDictReversed[index] = Pattern::compile(regex + std::wstring(L"$"));
								}
							}
						}

						RunAutomaton *ResourceManager::processChemicalGrammar(bool reversed) throw(IOException)
						{
							XMLStreamReader *reader = resourceGetter->getXMLStreamReader(L"regexes.xml");
							std::unordered_map<std::wstring, StringBuilder*> regexDict;
							Pattern *matchRegexReplacement = Pattern::compile(L"%.*?%");
							try
							{
								while (reader->hasNext())
								{
									if (reader->next() == XMLStreamConstants::START_ELEMENT && reader->getLocalName().equals(L"regex"))
									{
										std::wstring name = reader->getAttributeValue(nullptr, L"name");
										std::wstring value = reader->getAttributeValue(nullptr, L"value");
										Matcher *m = matchRegexReplacement->matcher(value);
										StringBuilder *newValueSB = new StringBuilder();
										int position = 0;
										while (m->find())
										{
											newValueSB->append(value.substr(position, m->start() - position));
											StringBuilder *replacement = regexDict[m->group()];
											if (replacement == nullptr)
											{
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("Regex entry for: " + m.group() + " missing! Check regexes.xml");
												throw std::exception();
											}
											newValueSB->append(replacement);
											position = m->end();
										}
										newValueSB->append(value.substr(position));
										if (regexDict[name] != nullptr)
										{
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("Regex entry: " + name + " has duplicate definitions! Check regexes.xml");
											throw std::exception();
										}
										regexDict[name] = newValueSB;
									}
									reader++;
								}
							}
							catch (const XMLStreamException &e)
							{
								throw IOException(L"Parsing exception occurred while reading regexes.xml", e);
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
									throw IOException(L"Parsing exception occurred while reading regexes.xml", e);
								}
							}

							std::wstring re = regexDict[L"%chemical%"]->toString();
							if (!reversed)
							{
								return automatonInitialiser->loadAutomaton(L"chemical", re, true, false);
							}
							else
							{
								return automatonInitialiser->loadAutomaton(L"chemical", re, true, true);
							}
						}

//JAVA TO C++ CONVERTER WARNING: The following method was originally marked 'synchronized':
						void ResourceManager::populatedReverseTokenMappings() throw(IOException)
						{
							if (reverseChemicalAutomaton == nullptr)
							{
								reverseChemicalAutomaton = processChemicalGrammar(true);
							}
							int grammarSymbolsSize = reverseChemicalAutomaton->getCharIntervals()->length;
							if (symbolTokenNamesDictReversed.empty())
							{
								symbolTokenNamesDictReversed = std::vector<OpsinRadixTrie*>(grammarSymbolsSize);
								processTokenFiles(true);
							}
							if (symbolRegexAutomataDictReversed.empty() && symbolRegexesDictReversed.empty())
							{
								symbolRegexAutomataDictReversed = std::vector<RunAutomaton*>(grammarSymbolsSize);
								symbolRegexesDictReversed = std::vector<Pattern*>(grammarSymbolsSize);
								processRegexTokenFiles(true);
							}
						}

						TokenEl *ResourceManager::makeTokenElement(const std::wstring &tokenString, boost::optional<wchar_t> symbol) throw(ParsingException)
						{
							std::unordered_map<wchar_t, TokenEl*> annotationToToken = tokenDict[tokenString];
							if (annotationToToken.size() > 0)
							{
								TokenEl *token = annotationToToken[symbol];
								if (token != nullptr)
								{
									if (token == IGNORE_WHEN_WRITING_PARSE_TREE)
									{
										return nullptr;
									}
									return token->copy(tokenString);
								}
							}
							TokenEl *regexToken = reSymbolTokenDict[symbol];
							if (regexToken != nullptr)
							{
								if (regexToken == IGNORE_WHEN_WRITING_PARSE_TREE)
								{
									return nullptr;
								}
								return regexToken->copy(tokenString);
							}
							throw ParsingException(std::wstring(L"Parsing Error: This is a bug in the program. A token element could not be found for token: ") + tokenString + std::wstring(L" using annotation symbol: ") + symbol);
						}

						RunAutomaton *ResourceManager::getChemicalAutomaton()
						{
							return chemicalAutomaton;
						}

						std::vector<OpsinRadixTrie*> ResourceManager::getSymbolTokenNamesDict()
						{
							return symbolTokenNamesDict;
						}

						std::vector<RunAutomaton*> ResourceManager::getSymbolRegexAutomataDict()
						{
							return symbolRegexAutomataDict;
						}

						std::vector<Pattern*> ResourceManager::getSymbolRegexesDict()
						{
							return symbolRegexesDict;
						}

						RunAutomaton *ResourceManager::getReverseChemicalAutomaton()
						{
							return reverseChemicalAutomaton;
						}

						std::vector<OpsinRadixTrie*> ResourceManager::getSymbolTokenNamesDictReversed()
						{
							return symbolTokenNamesDictReversed;
						}

						std::vector<RunAutomaton*> ResourceManager::getSymbolRegexAutomataDictReversed()
						{
							return symbolRegexAutomataDictReversed;
						}

						std::vector<Pattern*> ResourceManager::getSymbolRegexesDictReversed()
						{
							return symbolRegexesDictReversed;
						}
					}
				}
			}
		}
	}
}
