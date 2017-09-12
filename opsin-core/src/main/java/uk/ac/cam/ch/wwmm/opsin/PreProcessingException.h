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
						///Thrown during preprocessing.
						/// 
						/// @author dl387
						/// 
						/// </summary>
						class PreProcessingException : public std::exception
						{

						private:
							static constexpr long long serialVersionUID = 1LL;

						public:
							PreProcessingException();

							PreProcessingException(const std::wstring &message);

							PreProcessingException(const std::wstring &message, std::exception cause);

							PreProcessingException(std::exception cause);

						};

					}
				}
			}
		}
	}
}
