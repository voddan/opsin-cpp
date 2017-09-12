#include "ParsingException.h"

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

						ParsingException::ParsingException() : Exception()
						{
						}

						ParsingException::ParsingException(const std::wstring &message) : Exception(message)
						{
						}

						ParsingException::ParsingException(const std::wstring &message, std::exception cause) : Exception(message, cause)
						{
						}

						ParsingException::ParsingException(std::exception cause) : Exception(cause)
						{
						}
					}
				}
			}
		}
	}
}
