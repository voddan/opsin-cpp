#pragma once

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

						enum class SuffixRuleType
						{
							addgroup,
							addSuffixPrefixIfNonePresentAndCyclic,
							setOutAtom,
							changecharge,
							addFunctionalAtomsToHydroxyGroups,
							chargeHydroxyGroups,
							removeTerminalOxygen,
							convertHydroxyGroupsToOutAtoms,
							convertHydroxyGroupsToPositiveCharge,
							setAcidicElement
						};

					}
				}
			}
		}
	}
}
