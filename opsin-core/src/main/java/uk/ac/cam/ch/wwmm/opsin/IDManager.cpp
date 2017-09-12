#include "IDManager.h"

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

						int IDManager::getCurrentID()
						{
							return currentID;
						}

						IDManager::IDManager()
						{
							currentID = 0;
						}

						int IDManager::getNextID()
						{
							currentID += 1;
							return currentID;
						}
					}
				}
			}
		}
	}
}
