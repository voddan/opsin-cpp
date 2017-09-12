#include "PreProcessingException.h"

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

						PreProcessingException::PreProcessingException() : Exception()
						{
						}

						PreProcessingException::PreProcessingException(const std::wstring &message) : Exception(message)
						{
						}

						PreProcessingException::PreProcessingException(const std::wstring &message, std::exception cause) : Exception(message, cause)
						{
						}

						PreProcessingException::PreProcessingException(std::exception cause) : Exception(cause)
						{
						}
					}
				}
			}
		}
	}
}
