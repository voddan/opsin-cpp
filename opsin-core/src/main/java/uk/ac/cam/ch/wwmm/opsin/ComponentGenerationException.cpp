#include "ComponentGenerationException.h"

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

						ComponentGenerationException::ComponentGenerationException() : Exception()
						{
						}

						ComponentGenerationException::ComponentGenerationException(const std::wstring &message) : Exception(message)
						{
						}

						ComponentGenerationException::ComponentGenerationException(const std::wstring &message, std::exception cause) : Exception(message, cause)
						{
						}

						ComponentGenerationException::ComponentGenerationException(std::exception cause) : Exception(cause)
						{
						}
					}
				}
			}
		}
	}
}
