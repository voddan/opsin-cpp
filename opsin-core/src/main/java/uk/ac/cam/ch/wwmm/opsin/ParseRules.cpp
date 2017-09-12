#include "ParseRules.h"
#include "OpsinRadixTrie.h"
#include "AnnotatorState.h"
#include "ResourceManager.h"
#include "ParseRulesResults.h"
#include "ParsingException.h"
#include "StringTools.h"
#include "ParseTokens.h"

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

						ParseRules::ParseRules(ResourceManager *resourceManager) : chemAutomaton(resourceManager->getChemicalAutomaton()), stateSymbols(chemAutomaton->getCharIntervals()), symbolTokenNamesDict(resourceManager->getSymbolTokenNamesDict()), symbolRegexAutomataDict(resourceManager->getSymbolRegexAutomataDict()), symbolRegexesDict(resourceManager->getSymbolRegexesDict()), initialState(new AnnotatorState(chemAutomaton->getInitialState(), L'\0', 0, true, nullptr))
						{
						}

						ParseRulesResults *ParseRules::getParses(const std::wstring &chemicalWord) throw(ParsingException)
						{
							std::wstring chemicalWordLowerCase = StringTools::lowerCaseAsciiString(chemicalWord);
							ArrayDeque<AnnotatorState*> *asStack = new ArrayDeque<AnnotatorState*>();
							asStack->add(initialState);

							int posInNameOfLastSuccessfulAnnotations = 0;
							std::vector<AnnotatorState*> successfulAnnotations;
							AnnotatorState *longestAnnotation = initialState; //this is the longest annotation. It does not necessarily end in an accept state
							int stateSymbolsSize = stateSymbols.size();
							while (!asStack->isEmpty())
							{
								AnnotatorState *as = asStack->removeFirst();
								int posInName = as->getPosInName();
								if (chemAutomaton->isAccept(as->getState()))
								{
									if (posInName >= posInNameOfLastSuccessfulAnnotations)
									{ //this annotation is worthy of consideration
										if (posInName > posInNameOfLastSuccessfulAnnotations)
										{ //this annotation is longer than any previously found annotation
											successfulAnnotations.clear();
											posInNameOfLastSuccessfulAnnotations = posInName;
										}
										else if (successfulAnnotations.size() > 128)
										{
											throw ParsingException(L"Ambiguity in OPSIN's chemical grammar has produced more than 128 annotations. Parsing has been aborted. Please report this as a bug");
										}
										successfulAnnotations.push_back(as);
									}
								}
								//record the longest annotation found so it can be reported to the user for debugging
								if (posInName > longestAnnotation->getPosInName())
								{
									longestAnnotation = as;
								}

								for (int i = 0; i < stateSymbolsSize; i++)
								{
									wchar_t annotationCharacter = stateSymbols[i];
									int potentialNextState = chemAutomaton->step(as->getState(), annotationCharacter);
									if (potentialNextState != -1)
									{ //-1 means this state is not accessible from the previous state
										OpsinRadixTrie *possibleTokenisationsTrie = symbolTokenNamesDict[i];
										if (possibleTokenisationsTrie != nullptr)
										{
											std::vector<int> possibleTokenisations = possibleTokenisationsTrie->findMatches(chemicalWordLowerCase, posInName);
											if (possibleTokenisations.size() > 0)
											{ //next could be a token
												for (int j = 0, l = possibleTokenisations.size(); j < l; j++)
												{ //typically list size will be 1 so this is faster than an iterator
													int tokenizationIndex = possibleTokenisations[j];
													AnnotatorState *newAs = new AnnotatorState(potentialNextState, annotationCharacter, tokenizationIndex, false, as);
													//System.out.println("tokened " + chemicalWordLowerCase.substring(posInName, tokenizationIndex));
													asStack->add(newAs);
												}
											}
										}
										RunAutomaton *possibleAutomata = symbolRegexAutomataDict[i];
										if (possibleAutomata != nullptr)
										{ //next could be an automaton
											int matchLength = possibleAutomata->run(chemicalWord, posInName);
											if (matchLength != -1)
											{ //matchLength = -1 means it did not match
												int tokenizationIndex = posInName + matchLength;
												AnnotatorState *newAs = new AnnotatorState(potentialNextState, annotationCharacter, tokenizationIndex, true, as);
												//System.out.println("neword automata " + chemicalWord.substring(posInName, tokenizationIndex));
												asStack->add(newAs);
											}
										}
										Pattern *possibleRegex = symbolRegexesDict[i];
										if (possibleRegex != nullptr)
										{ //next could be a regex
											Matcher *mat = possibleRegex->matcher(chemicalWord).region(posInName, chemicalWord.length());
											mat->useTransparentBounds(true);
											if (mat->lookingAt())
											{ //match at start
												int tokenizationIndex = posInName + mat->group(0)->length();
												AnnotatorState *newAs = new AnnotatorState(potentialNextState, annotationCharacter, tokenizationIndex, true, as);
												//System.out.println("neword regex " + mat.group(0));
												asStack->add(newAs);
											}
										}
									}
								}
							}
							std::vector<ParseTokens*> outputList;
							std::wstring uninterpretableName = chemicalWord;
							std::wstring unparseableName = chemicalWord.substr(longestAnnotation->getPosInName());
							if (successfulAnnotations.size() > 0)
							{ //at least some of the name could be interpreted into a substituent/full/functionalTerm
								int bestAcceptPosInName = -1;
								for (auto as : successfulAnnotations)
								{
									outputList.push_back(convertAnnotationStateToParseTokens(as, chemicalWord, chemicalWordLowerCase));
									bestAcceptPosInName = as->getPosInName(); //all acceptable annotator states found should have the same posInName
								}
								uninterpretableName = chemicalWord.substr(bestAcceptPosInName);
							}
							return new ParseRulesResults(outputList, uninterpretableName, unparseableName);
						}

						ParseTokens *ParseRules::convertAnnotationStateToParseTokens(AnnotatorState *as, const std::wstring &chemicalWord, const std::wstring &chemicalWordLowerCase)
						{
							std::vector<std::wstring> tokens;
							std::vector<wchar_t> annotations;
							AnnotatorState *previousAs;
							while ((previousAs = as->getPreviousAs()) != nullptr)
							{
								if (as->isCaseSensitive())
								{
									tokens.push_back(chemicalWord.substr(previousAs->getPosInName(), as->getPosInName() - previousAs->getPosInName()));
								}
								else
								{
									tokens.push_back(chemicalWordLowerCase.substr(previousAs->getPosInName(), as->getPosInName() - previousAs->getPosInName()));
								}
								annotations.push_back(as->getAnnot());
								as = previousAs;
							}
							std::reverse(tokens.begin(), tokens.end());
							std::reverse(annotations.begin(), annotations.end());
							return new ParseTokens(tokens, annotations);
						}
					}
				}
			}
		}
	}
}
