#pragma once

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

						/// <summary>
						/// Allows OPSIN to be configured e.g. enable processing of radicals
						/// Example usage:
						/// NameToStructureConfig n2sConfig = new NameToStructureConfig();
						/// n2sconfig.setAllowRadicals(true);
						/// nts.parseChemicalName(chemicalName, n2sConfig) 
						/// where nts is an instance of NameToStructure
						/// @author dl387
						/// 
						/// </summary>
						class NameToStructureConfig : public Cloneable
						{

							// Fields set with default values
						private:
							bool allowRadicals = false;
							bool outputRadicalsAsWildCardAtoms = false;
							bool detailedFailureAnalysis = false;
							bool interpretAcidsWithoutTheWordAcid = false;
//JAVA TO C++ CONVERTER NOTE: Fields cannot have the same name as methods:
							bool warnRatherThanFailOnUninterpretableStereochemistry_Renamed = false;

							/// <summary>
							/// Constructs a NameToStructureConfig with default settings:
							/// allowRadicals = false
							/// outputRadicalsAsWildCardAtoms = false
							/// detailedFailureAnalysis = false
							/// interpretAcidsWithoutTheWordAcid = false
							/// warnRatherThanFailOnUninterpretableStereochemistry = false
							/// </summary>
						public:
							NameToStructureConfig();


							/// <summary>
							/// Are radicals allowed?  e.g. should fragments such as phenyl be interpretable </summary>
							/// <returns> whether radicals are allowed </returns>
							virtual bool isAllowRadicals();

							/// <summary>
							/// Sets whether radicals allowed? e.g. should fragments such as phenyl be interpretable
							/// </summary>
							virtual void setAllowRadicals(bool allowRadicals);

							/// <summary>
							/// Are radicals output as wildcard atoms e.g. [*]CC for ethyl </summary>
							/// <returns> whether radicals are output using explicit wildcard atoms </returns>
							virtual bool isOutputRadicalsAsWildCardAtoms();

							/// <summary>
							/// Should radicals be output as wildcard atoms e.g. [*]CC for ethyl (as opposed to [CH2]C)<br>
							/// Note that if this is set to true InChIs cannot be generated </summary>
							/// <param name="outputRadicalsAsWildCardAtoms"> </param>
							virtual void setOutputRadicalsAsWildCardAtoms(bool outputRadicalsAsWildCardAtoms);

							/// <summary>
							/// Should OPSIN attempt reverse parsing to more accurately determine why parsing failed </summary>
							/// <returns> whether a more precise cause of failure should be determined if parsing fails </returns>
							virtual bool isDetailedFailureAnalysis();

							/// <summary>
							/// Sets whether OPSIN should attempt reverse parsing to more accurately determine why parsing failed
							/// </summary>
							virtual void setDetailedFailureAnalysis(bool detailedFailureAnalysis);

							/// <summary>
							/// Are acids without the word "acid" interpretable e.g. should "acetic" be interpretable </summary>
							/// <returns> whether acids without the word "acid" should be interpretable </returns>
							virtual bool allowInterpretationOfAcidsWithoutTheWordAcid();


							/// <summary>
							/// Sets whether acids without the word "acid" interpretable e.g. should "acetic" be interpretable </summary>
							/// <param name="interpretAcidsWithoutTheWordAcid"> </param>
							virtual void setInterpretAcidsWithoutTheWordAcid(bool interpretAcidsWithoutTheWordAcid);

							/// <summary>
							/// If OPSIN cannot understand the stereochemistry in a name should OPSIN's result be a warning
							/// and structure with incomplete stereochemistry, or should failure be returned (Default) </summary>
							/// <returns> whether ignored stereochemistry is a warning (rather than a failure) </returns>
							virtual bool warnRatherThanFailOnUninterpretableStereochemistry();


							/// <summary>
							/// Sets whether if OPSIN cannot understand the stereochemistry in a name whether OPSIN's result should be a warning
							/// and structure with incomplete stereochemistry, or should failure be returned (Default) </summary>
							/// <param name="warnRatherThanFailOnUninterpretableStereochemistry"> </param>
							virtual void setWarnRatherThanFailOnUninterpretableStereochemistry(bool warnRatherThanFailOnUninterpretableStereochemistry);


							/// <summary>
							/// Constructs a NameToStructureConfig with default settings:
							/// allowRadicals = false
							/// outputRadicalsAsWildCardAtoms = false
							/// detailedFailureAnalysis = false
							/// interpretAcidsWithoutTheWordAcid = false
							/// warnRatherThanFailOnUninterpretableStereochemistry = false
							/// </summary>
							static NameToStructureConfig *getDefaultConfigInstance();

							virtual NameToStructureConfig *clone() override;

						};

					}
				}
			}
		}
	}
}
