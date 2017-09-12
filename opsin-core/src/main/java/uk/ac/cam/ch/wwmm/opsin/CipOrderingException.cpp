#include "CipOrderingException.h"

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

						CipOrderingException::CipOrderingException() : StereochemistryException()
						{
						}

						CipOrderingException::CipOrderingException(const std::wstring &message) : StereochemistryException(message)
						{
						}

						CipOrderingException::CipOrderingException(const std::wstring &message, std::exception cause) : StereochemistryException(message, cause)
						{
						}

						CipOrderingException::CipOrderingException(std::exception cause) : StereochemistryException(cause)
						{
						}
					}
				}
			}
		}
	}
}
