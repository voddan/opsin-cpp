#pragma once

#include <string>
#include <vector>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace uk { namespace ac { namespace cam { namespace ch { namespace wwmm { namespace opsin { class ParseTokens; } } } } } }

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
						///A "struct" containing data on the parsing of a word in a chemical name.
						/// 
						/// @author ptc24
						/// @author dl387
						/// 
						/// </summary>
						class ParseWord
						{
							/// <summary>
							///The word itself. </summary>
						private:
							const std::wstring word;
							/// <summary>
							///All of the possible tokenisations of the word. </summary>
							const std::vector<ParseTokens*> parseTokens;

						public:
							virtual ParseWord *deepCopy();

							ParseWord(const std::wstring &word, std::vector<ParseTokens*> &parseTokens);

							virtual std::wstring getWord();

							virtual std::vector<ParseTokens*> getParseTokens();

							virtual std::wstring toString() override;

						};

					}
				}
			}
		}
	}
}
