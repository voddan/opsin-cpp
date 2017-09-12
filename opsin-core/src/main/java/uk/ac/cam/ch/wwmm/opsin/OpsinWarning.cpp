#include "OpsinWarning.h"

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

						OpsinWarning::OpsinWarning(OpsinWarningType type, const std::wstring &message) : type(type), message(message)
						{
						}

						OpsinWarning::OpsinWarningType OpsinWarning::getType()
						{
							return type;
						}

						std::wstring OpsinWarning::getMessage()
						{
							return message;
						}
					}
				}
			}
		}
	}
}
