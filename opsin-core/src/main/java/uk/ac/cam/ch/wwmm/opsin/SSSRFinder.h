#pragma once

#include <unordered_map>
#include <vector>
#include <algorithm>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace uk { namespace ac { namespace cam { namespace ch { namespace wwmm { namespace opsin { class Fragment; } } } } } }
namespace uk { namespace ac { namespace cam { namespace ch { namespace wwmm { namespace opsin { class Ring; } } } } } }
namespace uk { namespace ac { namespace cam { namespace ch { namespace wwmm { namespace opsin { class Atom; } } } } } }
namespace uk { namespace ac { namespace cam { namespace ch { namespace wwmm { namespace opsin { class Bond; } } } } } }

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
						/// Class for finding SSSR
						/// The algorithm employed does not work in some corner cases
						/// 
						/// @author pm286
						/// @author dl387
						/// 
						/// </summary>
						class SSSRFinder
						{

							/// <summary>
							/// get set of smallest rings.
							/// In corner cases the list of rings returned will not be the SSSR </summary>
							/// <param name="frag"> </param>
							/// <returns> list of rings </returns>
						public:
							static std::vector<Ring*> getSetOfSmallestRings(Fragment *frag);

							/// <summary>
							/// get list of rings.
							/// not necessarily SSSR </summary>
							/// <param name="atomSet"> </param>
							/// <returns> list of rings </returns>
						private:
							static std::vector<Ring*> getRings(std::vector<Atom*> &atomSet);

							static Ring *getRing(Bond *bond, std::unordered_map<Atom*, Atom*> &atomToParentMap);

							static std::vector<Bond*> getAncestors1(Atom *atom, std::unordered_map<Atom*, Atom*> &atomToParentMap);

							static void expand(Atom *atom, Atom *parentAtom, Set<Atom*> *usedAtoms, std::unordered_map<Atom*, Atom*> &atomToParentMap, Set<Bond*> *linkBondSet);

							static bool reduceRingSizes(Ring *ring, std::vector<Ring*> &newList);


							static std::vector<Bond*> symmetricDifference(std::vector<Bond*> &bondSet1, std::vector<Bond*> &bondSet2);

						};

					}
				}
			}
		}
	}
}
