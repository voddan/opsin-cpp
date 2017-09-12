#include "FunctionalAtom.h"
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

						FunctionalAtom::FunctionalAtom(Atom *atom) : atom(atom)
						{
						}

						Atom *FunctionalAtom::getAtom()
						{
							return atom;
						}
					}
				}
			}
		}
	}
}
