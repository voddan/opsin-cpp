#pragma once

#include <string>
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
						///Thrown during finite-state parsing.
						/// 
						/// @author ptc24
						/// 
						/// </summary>
						class ParsingException : public std::exception
						{

						private:
							static constexpr long long serialVersionUID = 1LL;

						public:
							ParsingException();

							ParsingException(const std::wstring &message);

							ParsingException(const std::wstring &message, std::exception cause);

							ParsingException(std::exception cause);

						};

					}
				}
			}
		}
	}
}
