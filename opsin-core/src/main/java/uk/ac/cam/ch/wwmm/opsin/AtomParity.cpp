#include "AtomParity.h"
#include "Atom.h"

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

						AtomParity::AtomParity(std::vector<Atom*> &atomRefs4, int parity)
						{
							if (atomRefs4.size() != 4)
							{
								throw std::invalid_argument("atomRefs4 must contain references to 4 atoms");
							}
							this->atomRefs4 = atomRefs4;
							this->parity = parity;
						}

						std::vector<Atom*> AtomParity::getAtomRefs4()
						{
							return atomRefs4;
						}

						void AtomParity::setAtomRefs4(std::vector<Atom*> &atomRefs4)
						{
							this->atomRefs4 = atomRefs4;
						}

						int AtomParity::getParity()
						{
							return parity;
						}

						void AtomParity::setParity(int parity)
						{
							this->parity = parity;
						}
					}
				}
			}
		}
	}
}
