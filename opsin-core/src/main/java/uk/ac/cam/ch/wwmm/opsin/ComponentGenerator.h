#pragma once

#include "ChemEl.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <cctype>
#include <stdexcept>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/optional.hpp>
#include "stringhelper.h"
#include "stringbuilder.h"

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace uk { namespace ac { namespace cam { namespace ch { namespace wwmm { namespace opsin { class NameToStructureConfig; } } } } } }
namespace uk { namespace ac { namespace cam { namespace ch { namespace wwmm { namespace opsin { class ComponentGenerationException; } } } } } }
namespace uk { namespace ac { namespace cam { namespace ch { namespace wwmm { namespace opsin { class Element; } } } } } }

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
//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//						import static uk.ac.cam.ch.wwmm.opsin.OpsinTools.*;

						/// <summary>
						///Does destructive procedural parsing on parser results.
						/// 
						/// @author ptc24
						/// @author dl387
						/// 
						/// </summary>
						class ComponentGenerator
						{

							/// <summary>
							/// Sort bridges such as the highest priority secondary bridges come first
							/// e.g. 1^(6,3).1^(15,13)
							/// rearranged to 1^(15,13).1^(6,3)
							/// @author dl387
							/// 
							/// </summary>
						private:
							class VonBaeyerSecondaryBridgeSort : public Comparator<std::unordered_map<std::wstring, int>>
							{

							public:
								virtual int compare(std::unordered_map<std::wstring, int> &bridge1, std::unordered_map<std::wstring, int> &bridge2);
							};

							//match a fusion bracket with only numerical locants. If this is followed by a HW group it probably wasn't a fusion bracket
						private:
							static Pattern *const matchNumberLocantsOnlyFusionBracket = Pattern::compile(L"\\[\\d+(,\\d+)*\\]");
							static Pattern *const matchCommaOrDot = Pattern::compile(L"[\\.,]");
							static Pattern *const matchAnnulene = Pattern::compile(L"[\\[\\(\\{]([1-9]\\d*)[\\]\\)\\}]annulen");
							static const std::wstring elementSymbols;
							static Pattern *const matchStereochemistry = Pattern::compile(L"(.*?)(SR|R/?S|r/?s|[Ee][Zz]|[RSEZrsezabx]|[cC][iI][sS]|[tT][rR][aA][nN][sS]|[aA][lL][pP][hH][aA]|[bB][eE][tT][aA]|[xX][iI]|[eE][xX][oO]|[eE][nN][dD][oO]|[sS][yY][nN]|[aA][nN][tT][iI]|M|P|Ra|Sa|Sp|Rp)");
							static Pattern *const matchStar = Pattern::compile(L"\\^?\\*");
							static Pattern *const matchRacemic = Pattern::compile(L"rac(\\.|em(\\.|ic)?)?-?", Pattern::CASE_INSENSITIVE);
							static Pattern *const matchRS = Pattern::compile(L"[Rr]/?[Ss]?|[Ss][Rr]?");
							static Pattern *const matchEZ = Pattern::compile(L"[EZez]|[Ee][Zz]");
							static Pattern *const matchAlphaBetaStereochem = Pattern::compile(L"a|b|x|[aA][lL][pP][hH][aA]|[bB][eE][tT][aA]|[xX][iI]");
							static Pattern *const matchCisTrans = Pattern::compile(L"[cC][iI][sS]|[tT][rR][aA][nN][sS]");
							static Pattern *const matchEndoExoSynAnti = Pattern::compile(L"[eE][xX][oO]|[eE][nN][dD][oO]|[sS][yY][nN]|[aA][nN][tT][iI]");
							static Pattern *const matchAxialStereo = Pattern::compile(L"M|P|Ra|Sa|Sp|Rp");
							static Pattern *const matchLambdaConvention = Pattern::compile(L"(\\S+)?lambda\\D*(\\d+)\\D*", Pattern::CASE_INSENSITIVE);
							static Pattern *const matchHdigit = Pattern::compile(L"H\\d");
							static Pattern *const matchDigit = Pattern::compile(L"\\d+");
							static Pattern *const matchNonDigit = Pattern::compile(L"\\D+");
							static Pattern *const matchSuperscriptedLocant = Pattern::compile(std::wstring(L"(") + elementSymbols + std::wstring(L"'*)[\\^\\[\\(\\{~\\*\\<]*(?:[sS][uU][pP][ ]?)?([^\\^\\[\\(\\{~\\*\\<\\]\\)\\}\\>]+)[^\\[\\(\\{]*"));
							static Pattern *const matchIUPAC2004ElementLocant = Pattern::compile(std::wstring(L"(\\d+'*)-(") + elementSymbols + std::wstring(L"'*)(.*)"));
							static Pattern *const matchBracketAtEndOfLocant = Pattern::compile(L"-?[\\[\\(\\{](.*)[\\]\\)\\}]$");
							static Pattern *const matchGreek = Pattern::compile(L"alpha|beta|gamma|delta|epsilon|zeta|eta|omega", Pattern::CASE_INSENSITIVE);
							static Pattern *const matchInlineSuffixesThatAreAlsoGroups = Pattern::compile(L"carbonyl|oxy|sulfenyl|sulfinyl|sulfonyl|selenenyl|seleninyl|selenonyl|tellurenyl|tellurinyl|telluronyl");


							NameToStructureConfig *const n2sConfig;

						public:
							virtual ~ComponentGenerator()
							{
								delete n2sConfig;
							}

							ComponentGenerator(NameToStructureConfig *n2sConfig);

							/// <summary>
							/// Processes a parse result destructively adding semantic information by processing the various micro syntaxes. </summary>
							/// <param name="parse"> </param>
							/// <exception cref="ComponentGenerationException">  </exception>
							virtual void processParse(Element *parse) throw(ComponentGenerationException);

							/// <summary>
							/// Resolves common ambiguities e.g. tetradeca being 4x10carbon chain rather than 14carbon chain </summary>
							/// <param name="subOrRoot"> </param>
							/// <exception cref="ComponentGenerationException"> </exception>
							static void resolveAmbiguities(Element *subOrRoot) throw(ComponentGenerationException);


							/// <summary>
							/// Removes hyphens from the end of locants if present
							/// Looks for locants of the form number-letter and converts them to letternumber
							/// e.g. 1-N becomes N1. 1-N is the IUPAC 2004 recommendation, N1 is the previous recommendation
							/// Strips indication of superscript
							/// Strips added hydrogen out of locants
							/// Strips stereochemistry out of locants
							/// Normalises case on greeks to lower case
							/// </summary>
							/// <param name="subOrRoot"> </param>
							/// <exception cref="ComponentGenerationException">  </exception>
							static void processLocants(Element *subOrRoot) throw(ComponentGenerationException);

							/// <summary>
							/// Looks for locants of the form (<anotherLocant>)?<multiplier><locant>
							/// and converts them to <modifiedlocant><multiplier>
							/// e.g. 2,4,6 tri O- -->O2,O4,O6 tri </summary>
							/// <param name="locant"> </param>
						private:
							static void ifCarbohydrateLocantConvertToAminoAcidStyleLocant(Element *locant);

							/// <summary>
							/// Takes a string of locants and splits on commas, but taking into account brackets
							/// e.g. 1,2(1H,2H),3 becomes [1][2(1H,2H)][3] </summary>
							/// <param name="locantString">
							/// @return </param>
							static std::vector<std::wstring> splitIntoIndividualLocants(const std::wstring &locantString);

							/// <summary>
							///Converts ortho/meta/para into locants
							/// Depending on context para, for example, will either become para or 1,para
							/// </summary>
							/// <param name="subOrRoot"> </param>
							/// <exception cref="ComponentGenerationException"> </exception>
							void convertOrthoMetaParaToLocants(Element *subOrRoot) throw(ComponentGenerationException);

							bool orthoMetaParaLocantIsTwoLocants(Element *ompLocant);

							/// <summary>
							/// Processes adjacent alkane stem component elements into a single alkaneStem group element with the appropriate SMILES
							/// e.g. dodecane would be "do" value=2 and "dec" value=10 -->alkaneStem with 12 carbons
							/// </summary>
							/// <param name="subOrRoot"> </param>
							void formAlkaneStemsFromComponents(Element *subOrRoot);

							/// <summary>
							/// Applies the traditional alkane modifiers: iso, tert, sec, neo by modifying the alkane chain's SMILES
							/// </summary>
							/// <param name="subOrRoot"> </param>
							/// <exception cref="ComponentGenerationException">  </exception>
							void processAlkaneStemModifications(Element *subOrRoot) throw(ComponentGenerationException);

							/// <summary>
							///Form heterogeneous hydrides/substituents
							/// These are chains of one heteroatom or alternating heteroatoms and are expressed using SMILES
							/// They are typically treated in an analogous way to alkanes </summary>
							/// <param name="subOrRoot"> The root/substituents </param>
							/// <exception cref="ComponentGenerationException">  </exception>
							void processHeterogenousHydrides(Element *subOrRoot) throw(ComponentGenerationException);

							/// <summary>
							/// Throws an exception if the given heteroatoms could be part of a valid Hantzch-widman ring
							/// For this to be true the first heteroatom must be higher priority than the second
							/// and the second must be compatible with a HW ane stem </summary>
							/// <param name="firstHeteroAtomSMILES"> </param>
							/// <param name="secondHeteroAtomSMILES"> </param>
							/// <exception cref="ComponentGenerationException">  </exception>
							void checkForAmbiguityWithHWring(const std::wstring &firstHeteroAtomSMILES, const std::wstring &secondHeteroAtomSMILES) throw(ComponentGenerationException);

							/// <summary>
							/// Are either of the elements Si/Ge/Sn/Pb </summary>
							/// <param name="atom1ChemEl"> </param>
							/// <param name="atom2ChemEl">
							/// @return </param>
							bool hasSiorGeorSnorPb(ChemEl atom1ChemEl, ChemEl atom2ChemEl);

							/// <summary>
							/// Throws an exception if the given heteroatoms could be part of a heterogenous hydride
							/// For this to be true the second heteroatom must be higher priority than the first </summary>
							/// <param name="firstHeteroAtomSMILES"> </param>
							/// <param name="secondHeteroAtomSMILES"> </param>
							/// <exception cref="ComponentGenerationException">  </exception>
							void checkForAmbiguityWithHeterogenousHydride(const std::wstring &firstHeteroAtomSMILES, const std::wstring &secondHeteroAtomSMILES) throw(ComponentGenerationException);

							/// <summary>
							/// Handle indicated hydrogen  e.g. 1H- in 1H-pyrrole
							/// </summary>
							/// <param name="subOrRoot"> The substituent/root to looks for indicated hydrogens in. </param>
							/// <exception cref="ComponentGenerationException">  </exception>
							void processIndicatedHydrogens(Element *subOrRoot) throw(ComponentGenerationException);

							/// <summary>
							/// Handles stereoChemistry in brackets: R/Z/E/Z/a/alpha/b/beta and cis/trans
							///  Will assign a locant to a stereoChemistry element if one was specified/available
							/// </summary>
							/// <param name="subOrRoot"> The substituent/root to looks for stereoChemistry in. </param>
							/// <exception cref="ComponentGenerationException"> </exception>
						public:
							virtual void processStereochemistry(Element *subOrRoot) throw(ComponentGenerationException);

						private:
							void processStereochemistryBracket(Element *stereoChemistryElement) throw(ComponentGenerationException);

							std::vector<std::wstring> splitStereoBracketIntoDescriptors(const std::wstring &stereoBracket);

							bool assignLocantUsingPreviousElementIfPresent(Element *stereoChemistryElement);

							void processLocantAssigningForEndoExoSynAnti(Element *stereoChemistryElement);

							void processUnbracketedAlphaBetaStereochemistry(Element *stereoChemistryElement) throw(ComponentGenerationException);

							void processRelativeCisTrans(Element *stereoChemistryElement);

							/// <summary>
							/// If the e/z term is next to an ene or ylidene duplicate the locant
							/// e.g. 2E,4Z-diene --> 2E,4Z-2,4-diene
							/// 2E-ylidene --> 2E-2-ylidene </summary>
							/// <param name="locantedUnbrackettedEzTerms"> </param>
							void duplicateLocantFromStereoTermIfAdjacentToEneOrYlidene(std::vector<Element*> &locantedUnbrackettedEzTerms);

							/// <summary>
							/// Looks for "suffixPrefix" and assigns their value them as an attribute of an adjacent suffix </summary>
							/// <param name="subOrRoot"> </param>
							/// <exception cref="ComponentGenerationException"> </exception>
							void processSuffixPrefixes(Element *subOrRoot) throw(ComponentGenerationException);

							/// <summary>
							/// Looks for infixes and assigns them to the next suffix using a semicolon delimited infix attribute
							/// If the infix/suffix block has been bracketed e.g (dithioate) then the infix is multiplied out
							/// If preceded by a suffixPrefix e.g. sulfono infixes are also multiplied out
							/// If a multiplier is present and neither of these cases are met then it is ambiguous as to whether the multiplier is referring to the infix or the infixed suffix
							/// This ambiguity is resolved in processInfixFunctionalReplacementNomenclature by looking at the structure of the suffix to be modified </summary>
							/// <param name="subOrRoot"> </param>
							/// <exception cref="ComponentGenerationException"> </exception>
							void processInfixes(Element *subOrRoot) throw(ComponentGenerationException);

							/// <summary>
							/// Identifies lambdaConvention elements.
							/// The elementsValue is expected to be a comma seperated lambda values and 0 or more locants. Where a lambda value has the following form:
							/// optional locant, the word lambda and then a number which is the valency specified (with possibly some attempt to indicate this number is superscripted)
							/// If the element is followed by heteroatoms (possibly multiplied) they are multiplied and the locant/lambda assigned to them
							/// Otherwise a new lambdaConvention element is created with the valency specified by the lambda convention taking the attribute "lambda"
							/// In the case where heteroatoms belong to a fused ring system a new lambdaConvention element is also created. The original locants are retained in the benzo specific fused ring nomenclature:
							/// 2H-5lambda^5-phosphinino[3,2-b]pyran --> 2H 5lambda^5 phosphinino[3,2-b]pyran  BUT
							/// 1lambda^4,5-Benzodithiepin  --> 1lambda^4 1,5-Benzodithiepin </summary>
							/// <param name="subOrRoot"> </param>
							/// <exception cref="ComponentGenerationException"> </exception>
							void processLambdaConvention(Element *subOrRoot) throw(ComponentGenerationException);

							/// <summary>
							///Finds matching open and close brackets, and places the
							/// elements contained within in a big &lt;bracket&gt; element.
							/// </summary>
							/// <param name="substituentsAndRoot">: The substituent/root elements at the current level of the tree </param>
							/// <exception cref="ComponentGenerationException"> </exception>
							void findAndStructureBrackets(std::vector<Element*> &substituentsAndRoot) throw(ComponentGenerationException);

							/// <summary>
							///Places the elements in substituents containing/between an open and close bracket
							/// in a &lt;bracket&gt; tag.
							/// </summary>
							/// <param name="openBracket"> The open bracket element </param>
							/// <param name="closeBracket"> The close bracket element </param>
							/// <returns> The bracket element thus created. </returns>
							/// <exception cref="ComponentGenerationException">  </exception>
							Element *structureBrackets(Element *openBracket, Element *closeBracket) throw(ComponentGenerationException);

							/// <summary>
							///Looks for annulen/polyacene/polyaphene/polyalene/polyphenylene/polynaphthylene/polyhelicene tags and replaces them with a group with appropriate SMILES. </summary>
							/// <param name="subOrRoot"> The subOrRoot to look for tags in </param>
							/// <exception cref="ComponentGenerationException"> </exception>
							void processHydroCarbonRings(Element *subOrRoot) throw(ComponentGenerationException);

							/// <summary>
							/// Handles irregular suffixes. e.g. Quinone and ylene </summary>
							/// <param name="subOrRoot"> </param>
							/// <exception cref="ComponentGenerationException">  </exception>
							void handleSuffixIrregularities(Element *subOrRoot) throw(ComponentGenerationException);

							/// <summary>
							/// Looks for alkaneStems followed by a bridge forming 'o' and makes them fused ring bridge elements </summary>
							/// <param name="group"> </param>
							void detectAlkaneFusedRingBridges(Element *group);

							/// <summary>
							///Looks (multiplier)cyclo/spiro/cyclo tags before chain
							/// and replaces them with a group with appropriate SMILES
							/// Note that only simple spiro tags are handled at this stage i.e. not dispiro </summary>
							/// <param name="group"> A group which is potentially a chain </param>
							/// <exception cref="ComponentGenerationException"> </exception>
							void processRings(Element *group) throw(ComponentGenerationException);

							/// <summary>
							/// Processes a spiro descriptor element.
							/// This modifies the provided chainGroup into the spiro system by replacing the value of the chain group with appropriate SMILES </summary>
							/// <param name="chainGroup"> </param>
							/// <param name="spiroEl"> </param>
							/// <exception cref="ComponentGenerationException"> </exception>
							/// <exception cref="NumberFormatException">  </exception>
							void processSpiroSystem(Element *chainGroup, Element *spiroEl) throw(NumberFormatException, ComponentGenerationException);

							/// <summary>
							/// If the integer given is > 9 return %ringClosure else just returns ringClosure </summary>
							/// <param name="ringClosure">
							/// @return </param>
							std::wstring ringClosure(int ringClosure);

							/// <summary>
							/// Prepares spiro string for processing </summary>
							/// <param name="text"> - string with spiro e.g. spiro[2.2] </param>
							/// <returns> array with number of carbons in each group and associated index of spiro atom </returns>
							std::vector<std::vector<int>> getSpiroDescriptors(const std::wstring &text);

							/// <summary>
							/// Finds the the carbon atom with the given locant in the provided SMILES
							/// Returns the next index which is expected to correspond to the atom's ring opening/s </summary>
							/// <param name="smiles"> string to search in </param>
							/// <param name="locant"> locant of the atom in given structure </param>
							/// <returns> index of ring openings </returns>
							/// <exception cref="ComponentGenerationException">  </exception>
							boost::optional<int> findIndexOfRingOpenings(const std::wstring &smiles, int locant) throw(ComponentGenerationException);

							/// <summary>
							/// Given an element corresponding to an alkane or other systematic chain and the preceding vonBaeyerBracket element:
							/// Generates the SMILES of the von baeyer system and assigns this to the chain Element
							/// Checks are done on the von baeyer multiplier and chain length
							/// The multiplier and vonBaeyerBracket are detached </summary>
							/// <param name="chainEl"> </param>
							/// <param name="vonBaeyerBracketEl"> </param>
							/// <exception cref="ComponentGenerationException"> </exception>
							void processVonBaeyerSystem(Element *chainEl, Element *vonBaeyerBracketEl) throw(ComponentGenerationException);

							/// <summary>
							/// Converts a chain group into a ring.
							/// The chain group can either be an alkane or heteroatom chain </summary>
							/// <param name="chainGroup"> </param>
							/// <param name="cycloEl"> </param>
							/// <exception cref="ComponentGenerationException"> </exception>
							void processCyclisedChain(Element *chainGroup, Element *cycloEl) throw(ComponentGenerationException);

							/// <summary>
							///Handles special cases in IUPAC nomenclature.
							/// Benzyl etc. </summary>
							/// <param name="group"> The group to look for irregularities in. </param>
							/// <exception cref="ComponentGenerationException"> </exception>
							void handleGroupIrregularities(Element *group) throw(ComponentGenerationException);
						};


					}
				}
			}
		}
	}
}
