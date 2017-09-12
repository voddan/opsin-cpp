#pragma once

#include "WordType.h"
#include "WordRule.h"
#include "ChemEl.h"
#include <string>
#include <vector>
#include <cctype>
#include <stdexcept>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/optional.hpp>
#include "stringhelper.h"
#include "stringbuilder.h"

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace uk { namespace ac { namespace cam { namespace ch { namespace wwmm { namespace opsin { class WordRuleDescription; } } } } } }
namespace uk { namespace ac { namespace cam { namespace ch { namespace wwmm { namespace opsin { class ResourceGetter; } } } } } }
namespace uk { namespace ac { namespace cam { namespace ch { namespace wwmm { namespace opsin { class Element; } } } } } }
namespace uk { namespace ac { namespace cam { namespace ch { namespace wwmm { namespace opsin { class NameToStructureConfig; } } } } } }
namespace uk { namespace ac { namespace cam { namespace ch { namespace wwmm { namespace opsin { class ParsingException; } } } } } }

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
//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//						import static uk.ac.cam.ch.wwmm.opsin.XmlDeclarations.*;

						/// <summary>
						///The rules by which words are grouped together (e.g. in functional class nomenclature)
						/// 
						/// @author dl387
						/// 
						/// </summary>
						class WordRules
						{

							/// <summary>
							///The wordRules themselves. </summary>
						private:
							const std::vector<WordRuleDescription*> wordRuleList;

						public:
							enum class EndsWithGroup
							{
								acid,
								ateGroup
							};

						private:
							static Pattern *const icOrOusAcid = Pattern::compile(L"(ic|ous)([ ]?acid)?$");
							static Pattern *const ateOrIteOrAmide = Pattern::compile(L"(at|it|amid)e?$");

							/// <summary>
							/// Describes a word that a wordRule is looking for
							/// @author dl387
							/// 
							/// </summary>
						private:
							class WordDescription
							{
								/// <summary>
								///Whether the word is a full word, substituent word or functionalTerm word </summary>
							private:
								const WordType type;

								/// <summary>
								///A group with a hardcoded method for efficient detection </summary>
								const EndsWithGroup endsWithGroup;

								/// <summary>
								///A case insensitive pattern which attempts to match the end of the String value of the word </summary>
								Pattern *const endsWithPattern;

								/// <summary>
								///The case insensitive String value of the word </summary>
								const std::wstring value;

								/// <summary>
								/// Only applicable for functionalTerms. The string value of the functionalTerm's type attribute </summary>
								const std::wstring functionalGroupType;

								/// <summary>
								/// The value of the type attribute of the last group element in the word e.g. maybe aminoAcid </summary>
								const std::wstring endsWithGroupType;

								/// <summary>
								/// The value of the subType attribute of the last group element in the word e.g. maybe elementaryAtom </summary>
								const std::wstring endsWithGroupSubType;

								/// <summary>
								/// Makes a description of a word to looks for </summary>
								/// <param name="reader"> </param>
							public:
								virtual ~WordDescription()
								{
									delete endsWithPattern;
								}

								WordDescription(XMLStreamReader *reader);

								virtual WordType getType();

								virtual EndsWithGroup getEndsWithGroup();

								virtual Pattern *getEndsWithPattern();

								virtual std::wstring getValue();

								virtual std::wstring getFunctionalGroupType();

								virtual std::wstring getEndsWithGroupType();

								virtual std::wstring getEndsWithGroupSubType();
							};

							/// <summary>
							/// A representation of a wordRule element from wordRules.xml
							/// @author dl387
							/// 
							/// </summary>
						private:
							class WordRuleDescription
							{
							private:
								const std::vector<WordDescription*> wordDescriptions;
								const WordRule ruleName;
								const WordType ruleType;

							public:
								virtual std::vector<WordDescription*> getWordDescriptions();

								virtual WordRule getRuleName();

								virtual WordType getRuleType();

								/// <summary>
								/// Creates a wordRule from a wordRule element found in wordRules.xml </summary>
								/// <param name="reader"> </param>
								/// <exception cref="XMLStreamException">  </exception>
								WordRuleDescription(XMLStreamReader *reader) throw(XMLStreamException);
							};


							/// <summary>
							///Initialises the WordRules. </summary>
							/// <param name="resourceGetter"> </param>
							/// <exception cref="IOException">  </exception>
						public:
							WordRules(ResourceGetter *resourceGetter) throw(IOException);

							/// <summary>
							///Takes a molecule element and places the word elements into wordRule elements </summary>
							/// <param name="moleculeEl"> A molecule element with word children </param>
							/// <param name="n2sConfig"> </param>
							/// <param name="allowSpaceRemoval"> </param>
							/// <param name="componentRatios"> </param>
							/// <exception cref="ParsingException"> </exception>
							virtual void groupWordsIntoWordRules(Element *moleculeEl, NameToStructureConfig *n2sConfig, bool allowSpaceRemoval, std::vector<Integer> &componentRatios) throw(ParsingException);

						private:
							class WordRulesInstance
							{
												private:
													WordRules *outerInstance;

								Element *const moleculeEl;
								const bool allowRadicals;
								const bool allowSpaceRemoval;
								const boost::optional<int> expectedNumOfComponents;

							public:
								virtual ~WordRulesInstance()
								{
									delete moleculeEl;
									delete outerInstance;
								}

								WordRulesInstance(WordRules *outerInstance, Element *moleculeEl, NameToStructureConfig *n2sConfig, bool allowSpaceRemoval, std::vector<Integer> &componentRatios);

							private:
								bool matchWordRule(std::vector<Element*> &wordEls, int indexOfFirstWord) throw(ParsingException);

								bool endsWithGroupPredicateSatisfied(Element *wordEl, EndsWithGroup endsWithGroupPredicate) throw(ParsingException);

								bool oxidationNumberOrMultiplierIs(Element *elementaryAtomEl, Element *functionalGroupWord, int expectedVal) throw(ParsingException);

								Element *getLastGroupInWordRule(Element *wordEl);

								Element *getLastElementInWord(Element *wordEl);

								void applySimpleWordRule(std::vector<Element*> &wordEls, int indexOfFirstWord, Element *firstWord);


								void applySubstituentWordRule(std::vector<Element*> &wordEls, int indexOfFirstWord, Element *firstWord);

								/// <summary>
								/// Merges two adjacent words
								/// The latter word (wordToPotentiallyCombineWith) is merged into the former and removed from wordEls </summary>
								/// <param name="wordEls"> </param>
								/// <param name="firstWord"> </param>
								/// <param name="wordToPotentiallyCombineWith"> </param>
								/// <exception cref="ParsingException"> </exception>
								void joinWords(std::vector<Element*> &wordEls, Element *firstWord, Element *wordToPotentiallyCombineWith) throw(ParsingException);

								Element *convertFunctionalGroupIntoGroup(Element *word) throw(ParsingException);


								/// <summary>
								/// Sets the SMILES of the oxide group to be something like [O-2]
								/// ... unless the oxide group is multiplied and the elementaryAtom has no oxidation states greater 2
								/// in which case [O-][O-] would be assumed </summary>
								/// <param name="oxideGroup"> </param>
								/// <param name="elementaryAtom"> </param>
								void setOxideStructureAppropriately(Element *oxideGroup, Element *elementaryAtom);


								ChemEl getChemElFromElementaryAtomEl(Element *elementaryAtomEl);

								ChemEl getChemElFromWordWithFunctionalGroup(Element *functionalWord) throw(ParsingException);
							};

						};

					}
				}
			}
		}
	}
}
