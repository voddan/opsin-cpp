#include "ParseRulesResults.h"
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

						ParseRulesResults::ParseRulesResults(std::vector<ParseTokens*> &parseTokensList, const std::wstring &uninterpretableName, const std::wstring &unparseableName) : parseTokensList(parseTokensList), uninterpretableName(uninterpretableName), unparseableName(unparseableName)
						{
						}

						std::vector<ParseTokens*> ParseRulesResults::getParseTokensList()
						{
						   return parseTokensList;
						}

						std::wstring ParseRulesResults::getUninterpretableName()
						{
						   return uninterpretableName;
						}

						std::wstring ParseRulesResults::getUnparseableName()
						{
							return unparseableName;
						}

						std::wstring ParseRulesResults::toString()
						{
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
						   return std::wstring(L"(") + parseTokensList.toString() + std::wstring(L", ") + uninterpretableName + std::wstring(L", ") + unparseableName + std::wstring(L")");
						}
					}
				}
			}
		}
	}
}
