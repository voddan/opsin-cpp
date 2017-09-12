#include "BuildState.h"
#include "FragmentManager.h"
#include "Element.h"
#include "Fragment.h"
#include "NameToStructureConfig.h"
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
						using OpsinWarningType = uk::ac::cam::ch::wwmm::opsin::OpsinWarning::OpsinWarningType;

						BuildState::BuildState(NameToStructureConfig *n2sConfig) : fragManager(new FragmentManager(&tempVar, idManager)), xmlSuffixMap(std::unordered_map<Element*, std::vector<Fragment*>>()), n2sConfig(n2sConfig)
						{
							IDManager *idManager = new IDManager();
						}

						std::vector<OpsinWarning*> BuildState::getWarnings()
						{
							return warnings;
						}

						void BuildState::addWarning(OpsinWarningType type, const std::wstring &message)
						{
							OpsinWarning tempVar(type, message);
							warnings.push_back(&tempVar);
						}

						void BuildState::addIsAmbiguous(const std::wstring &message)
						{
							OpsinWarning tempVar(OpsinWarningType::APPEARS_AMBIGUOUS, message);
							warnings.push_back(&tempVar);
						}
					}
				}
			}
		}
	}
}
