#pragma once

#include <string>
#include <vector>
#include <stdexcept>

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
						///A "struct" containing data a possible tokenisation of a word in a chemical name.
						/// 
						/// @author ptc24
						/// @author dl387
						/// 
						/// </summary>
						class ParseTokens
						{
							/// <summary>
							///The tokens that the word is made up of. </summary>
						private:
							const std::vector<std::wstring> tokens;

							/// <summary>
							///A list of possible annotations of that token. </summary>
							const std::vector<wchar_t> annotations;


							/// <summary>
							/// Creates a parseTokens from an existing list of tokens and annotations
							/// The lists should be of identical lengths otherwise an exception is thrown </summary>
							/// <param name="tokens"> </param>
							/// <param name="annotations"> </param>
						public:
							ParseTokens(std::vector<std::wstring> &tokens, std::vector<wchar_t> &annotations);

							virtual std::vector<std::wstring> getTokens();

							virtual std::vector<wchar_t> getAnnotations();

							virtual std::wstring toString() override;

							virtual bool equals(void *other) override;

							virtual int hashCode() override;
						};

					}
				}
			}
		}
	}
}
