#include "ReverseParseRules.h"
#include "OpsinRadixTrie.h"
#include "ResourceManager.h"
#include "ParseRulesResults.h"
#include "ParsingException.h"
#include "AnnotatorState.h"
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

						ReverseParseRules::ReverseParseRules(ResourceManager *resourceManager) throw(IOException) : chemAutomaton(resourceManager->getReverseChemicalAutomaton()), stateSymbols(chemAutomaton->getCharIntervals()), symbolTokenNamesDictReversed(resourceManager->getSymbolTokenNamesDictReversed()), symbolRegexAutomataDictReversed(resourceManager->getSymbolRegexAutomataDictReversed()), symbolRegexesDictReversed(resourceManager->getSymbolRegexesDictReversed())
						{
							resourceManager->populatedReverseTokenMappings();
						}

						ParseRulesResults *ReverseParseRules::getParses(const std::wstring &chemicalWord) throw(ParsingException)
						{
							AnnotatorState *initialState = new AnnotatorState(chemAutomaton->getInitialState(), L'\0', chemicalWord.length(), true, nullptr);
							std::wstring chemicalWordLowerCase = StringTools::lowerCaseAsciiString(chemicalWord);
							ArrayDeque<AnnotatorState*> *asStack = new ArrayDeque<AnnotatorState*>();
							asStack->add(initialState);

							int posInNameOfLastSuccessfulAnnotations = chemicalWord.length();
							std::vector<AnnotatorState*> successfulAnnotations;
							AnnotatorState *longestAnnotation = initialState; //this is the longest annotation. It does not necessarily end in an accept state
							int stateSymbolsSize = stateSymbols.size();
							while (!asStack->isEmpty())
							{
								AnnotatorState *as = asStack->removeFirst();
								int posInName = as->getPosInName();
								if (chemAutomaton->isAccept(as->getState()))
								{
									if (posInName <= posInNameOfLastSuccessfulAnnotations)
									{ //this annotation is worthy of consideration
										if (posInName < posInNameOfLastSuccessfulAnnotations)
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
								if (posInName < longestAnnotation->getPosInName())
								{
									longestAnnotation = as;
								}

								for (int i = 0; i < stateSymbolsSize; i++)
								{
									wchar_t annotationCharacter = stateSymbols[i];
									int potentialNextState = chemAutomaton->step(as->getState(), annotationCharacter);
									if (potentialNextState != -1)
									{ //-1 means this state is not accessible from the previous state
										OpsinRadixTrie *possibleTokenisationsTrie = symbolTokenNamesDictReversed[i];
										if (possibleTokenisationsTrie != nullptr)
										{
											std::vector<int> possibleTokenisations = possibleTokenisationsTrie->findMatchesReadingStringRightToLeft(chemicalWordLowerCase, posInName);
											if (possibleTokenisations.size() > 0)
											{ //next could be a token
												for (int j = 0, l = possibleTokenisations.size(); j < l; j++)
												{ //typically list size will be 1 so this is faster than an iterator
													int tokenizationIndex = possibleTokenisations[j];
													AnnotatorState *newAs = new AnnotatorState(potentialNextState, annotationCharacter, tokenizationIndex, false, as);
													//System.out.println("tokened " + chemicalWordLowerCase.substring(tokenizationIndex, posInName));
													asStack->add(newAs);
												}
											}
										}
										RunAutomaton *possibleAutomata = symbolRegexAutomataDictReversed[i];
										if (possibleAutomata != nullptr)
										{ //next could be an automaton
											int matchLength = runInReverse(possibleAutomata, chemicalWord, posInName);
											if (matchLength != -1)
											{ //matchLength = -1 means it did not match
												int tokenizationIndex = posInName - matchLength;
												AnnotatorState *newAs = new AnnotatorState(potentialNextState, annotationCharacter, tokenizationIndex, true, as);
												//System.out.println("neword automata " + chemicalWord.substring(tokenizationIndex, posInName));
												asStack->add(newAs);
											}
										}
										Pattern *possibleRegex = symbolRegexesDictReversed[i];
										if (possibleRegex != nullptr)
										{ //next could be a regex
											Matcher *mat = possibleRegex->matcher(chemicalWord).region(0, posInName);
											mat->useTransparentBounds(true);
											if (mat->find())
											{ //match at end (patterns use $ anchor)
												int tokenizationIndex = posInName - mat->group(0)->length();
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
							std::wstring unparseableName = chemicalWord.substr(0, longestAnnotation->getPosInName());
							if (successfulAnnotations.size() > 0)
							{ //at least some of the name could be interpreted into a substituent/full/functionalTerm
								int bestAcceptPosInName = -1;
								for (auto as : successfulAnnotations)
								{
									outputList.push_back(convertAnnotationStateToParseTokens(as, chemicalWord, chemicalWordLowerCase));
									bestAcceptPosInName = as->getPosInName(); //all acceptable annotator states found should have the same posInName
								}
								uninterpretableName = chemicalWord.substr(0, bestAcceptPosInName);
							}
							return new ParseRulesResults(outputList, uninterpretableName, unparseableName);
						}

						int ReverseParseRules::runInReverse(RunAutomaton *automaton, const std::wstring &s, int indexAfterFirstchar)
						{
							int state = automaton->getInitialState();
							int max = -1;
							for (int pos = indexAfterFirstchar - 1; ; pos--)
							{
								if (automaton->isAccept(state))
								{
									max = indexAfterFirstchar - 1 - pos;
								}
								if (pos == -1)
								{
									break;
								}
								state = automaton->step(state, s[pos]);
								if (state == -1)
								{
									break;
								}
							}
							return max;
						}

						ParseTokens *ReverseParseRules::convertAnnotationStateToParseTokens(AnnotatorState *as, const std::wstring &chemicalWord, const std::wstring &chemicalWordLowerCase)
						{
							std::vector<std::wstring> tokens;
							std::vector<wchar_t> annotations;
							AnnotatorState *previousAs;
							while ((previousAs = as->getPreviousAs()) != nullptr)
							{
								if (as->isCaseSensitive())
								{
									tokens.push_back(chemicalWord.substr(as->getPosInName(), previousAs->getPosInName() - as->getPosInName()));
								}
								else
								{
									tokens.push_back(chemicalWordLowerCase.substr(as->getPosInName(), previousAs->getPosInName() - as->getPosInName()));
								}
								annotations.push_back(as->getAnnot());
								as = previousAs;
							}
							return new ParseTokens(tokens, annotations);
						}
					}
				}
			}
		}
	}
}
