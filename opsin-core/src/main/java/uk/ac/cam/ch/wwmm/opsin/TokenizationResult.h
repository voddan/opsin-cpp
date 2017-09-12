#pragma once

#include <string>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace uk { namespace ac { namespace cam { namespace ch { namespace wwmm { namespace opsin { class Parse; } } } } } }

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

						/// 
						/// <summary>
						/// @author dl387
						/// 
						/// </summary>
						class TokenizationResult
						{

						private:
							Parse *const parse;
							std::wstring workingName;
							std::wstring unparsableName;
							std::wstring unparsedName;
							std::wstring uninterpretableName;


						public:
							virtual ~TokenizationResult()
							{
								delete parse;
							}

							TokenizationResult(const std::wstring &name);

							virtual bool isSuccessfullyTokenized();

							virtual Parse *getParse();

							virtual void setUninterpretableName(const std::wstring &name);

							virtual std::wstring getUninterpretableName();

							virtual std::wstring getWorkingName();

							virtual void setWorkingName(const std::wstring &name);

							virtual bool isFullyInterpretable();

							virtual std::wstring getUnparsableName();

							virtual void setUnparsableName(const std::wstring &name);

							virtual std::wstring getUnparsedName();

							virtual void setUnparsedName(const std::wstring &name);

							virtual void setErrorFields(const std::wstring &unparsedName, const std::wstring &uninterpretableName, const std::wstring &unparsableName);
						};

					}
				}
			}
		}
	}
}
