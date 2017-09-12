#pragma once

#include <string>
#include <vector>
#include <stdexcept>
#include "stringbuilder.h"

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace uk { namespace ac { namespace cam { namespace ch { namespace wwmm { namespace opsin { class Fragment; } } } } } }
namespace uk { namespace ac { namespace cam { namespace ch { namespace wwmm { namespace opsin { class OpsinWarning; } } } } } }

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


						using org::apache::log4j::Logger;


						/// <summary>
						/// Holds the structure OPSIN has generated from a name
						/// Additionally holds a status code for whether name interpretation was successful
						/// @author dl387
						/// 
						/// </summary>
						class OpsinResult
						{
						private:
							static Logger *const LOG = Logger::getLogger(OpsinResult::typeid);
							Fragment *const structure;
							const OPSIN_RESULT_STATUS status;
							const std::wstring message;
							const std::wstring chemicalName;
							const std::vector<OpsinWarning*> warnings;

							/// <summary>
							/// Whether parsing the chemical name was successful, encountered problems or was unsuccessful.<br>
							/// If the result is not <seealso cref="OPSIN_RESULT_STATUS#FAILURE"/> then a structure has been generated
							/// @author dl387
							/// 
							/// </summary>
						public:
							enum class OPSIN_RESULT_STATUS
							{
								/// <summary>
								/// OPSIN successfully interpreted the name
								/// </summary>
								SUCCESS,
								/// <summary>
								/// OPSIN interpreted the name but detected a potential problem e.g. could not interpret stereochemistry<br>
								/// Currently, by default, WARNING is not used as stereochemistry failures are treated as failures<br>
								/// In the future, ambiguous chemical names may produce WARNING
								/// </summary>
								WARNING,
								/// <summary>
								/// OPSIN failed to interpret the name
								/// </summary>
								FAILURE
							};

						public:
							virtual ~OpsinResult()
							{
								delete structure;
							}

//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
							OpsinResult(Fragment *frag, OPSIN_RESULT_STATUS status, std::vector<OpsinWarning*> &warnings, const std::wstring &chemicalName);

							OpsinResult(Fragment *frag, OPSIN_RESULT_STATUS status, const std::wstring &message, const std::wstring &chemicalName);

							virtual Fragment *getStructure();

							/// <summary>
							/// Returns an enum with values SUCCESS, WARNING and FAILURE
							/// Currently warning is never used </summary>
							/// <returns> <seealso cref="OPSIN_RESULT_STATUS"/> status </returns>
							virtual OPSIN_RESULT_STATUS getStatus();

							/// <summary>
							/// Returns a message explaining why generation of a molecule from the name failed
							/// This string will be blank when no problems were encountered </summary>
							/// <returns> String explaining problems encountered </returns>
							virtual std::wstring getMessage();

							/// <summary>
							/// Returns the chemical name that this OpsinResult was generated from </summary>
							/// <returns> String containing the original chemical name </returns>
							virtual std::wstring getChemicalName();

							/// <summary>
							/// Generates the CML corresponding to the molecule described by the name
							/// If name generation failed i.e. the OPSIN_RESULT_STATUS is FAILURE then null is returned </summary>
							/// <returns> Chemical Markup Language as a String </returns>
							virtual std::wstring getCml();

							/// <summary>
							/// Generates the CML corresponding to the molecule described by the name
							/// If name generation failed i.e. the OPSIN_RESULT_STATUS is FAILURE then null is returned
							/// The CML is indented </summary>
							/// <returns> Idented Chemical Markup Language as a String </returns>
							virtual std::wstring getPrettyPrintedCml();

							/// <summary>
							/// Generates the SMILES corresponding to the molecule described by the name
							/// If name generation failed i.e. the OPSIN_RESULT_STATUS is FAILURE then null is returned </summary>
							/// <returns> SMILES as a String </returns>
							virtual std::wstring getSmiles();

							/// <summary>
							/// Experimental function that generates the extended SMILES corresponding to the molecule described by the name
							/// If name generation failed i.e. the OPSIN_RESULT_STATUS is FAILURE then null is returned
							/// If the molecule doesn't utilise any features made possible by extended SMILES this is equivalent to <seealso cref="#getSmiles()"/> </summary>
							/// <returns> Extended SMILES as a String </returns>
							virtual std::wstring getExtendedSmiles();

							/// <summary>
							/// A list of warnings encountered when the result was <seealso cref="OPSIN_RESULT_STATUS#WARNING"/><br>
							/// This list of warnings is immutable </summary>
							/// <returns> A list of <seealso cref="OpsinWarning"/> </returns>
							virtual std::vector<OpsinWarning*> getWarnings();

							/// <summary>
							/// Convenience method to check if one of the associated OPSIN warnings was <seealso cref="OpsinWarningType#APPEARS_AMBIGUOUS"/> </summary>
							/// <returns> true if name appears to be ambiguous </returns>
							virtual bool nameAppearsToBeAmbiguous();

							/// <summary>
							/// Convenience method to check if one of the associated OPSIN warnings was <seealso cref="OpsinWarningType#STEREOCHEMISTRY_IGNORED"/> </summary>
							/// <returns> true if stereochemistry was ignored to interpret the name </returns>
							virtual bool stereochemistryIgnored();

						};

					}
				}
			}
		}
	}
}
