#include "AtomProperties.h"

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

const std::unordered_map<ChemEl, double> AtomProperties::elementToPaulingElectronegativity = new java::util::EnumMap<ChemEl, double>(ChemEl::typeid);
const std::unordered_map<ChemEl, int> AtomProperties::elementToHwPriority = new java::util::EnumMap<ChemEl, int>(ChemEl::typeid);

						AtomProperties::StaticConstructor::StaticConstructor()
						{
															elementToPaulingElectronegativity[ChemEl::H] = 2.20;
															elementToPaulingElectronegativity[ChemEl::Li] = 0.98;
															elementToPaulingElectronegativity[ChemEl::Be] = 1.57;
															elementToPaulingElectronegativity[ChemEl::B] = 2.04;
															elementToPaulingElectronegativity[ChemEl::C] = 2.55;
															elementToPaulingElectronegativity[ChemEl::N] = 3.04;
															elementToPaulingElectronegativity[ChemEl::O] = 3.44;
															elementToPaulingElectronegativity[ChemEl::F] = 3.98;
															elementToPaulingElectronegativity[ChemEl::Na] = 0.93;
															elementToPaulingElectronegativity[ChemEl::Mg] = 1.31;
															elementToPaulingElectronegativity[ChemEl::Al] = 1.61;
															elementToPaulingElectronegativity[ChemEl::Si] = 1.90;
															elementToPaulingElectronegativity[ChemEl::P] = 2.19;
															elementToPaulingElectronegativity[ChemEl::S] = 2.58;
															elementToPaulingElectronegativity[ChemEl::Cl] = 3.16;
															elementToPaulingElectronegativity[ChemEl::K] = 0.82;
															elementToPaulingElectronegativity[ChemEl::Ca] = 1.00;
															elementToPaulingElectronegativity[ChemEl::Sc] = 1.36;
															elementToPaulingElectronegativity[ChemEl::Ti] = 1.54;
															elementToPaulingElectronegativity[ChemEl::V] = 1.63;
															elementToPaulingElectronegativity[ChemEl::Cr] = 1.66;
															elementToPaulingElectronegativity[ChemEl::Mn] = 1.55;
															elementToPaulingElectronegativity[ChemEl::Fe] = 1.83;
															elementToPaulingElectronegativity[ChemEl::Co] = 1.88;
															elementToPaulingElectronegativity[ChemEl::Ni] = 1.91;
															elementToPaulingElectronegativity[ChemEl::Cu] = 1.90;
															elementToPaulingElectronegativity[ChemEl::Zn] = 1.65;
															elementToPaulingElectronegativity[ChemEl::Ga] = 1.81;
															elementToPaulingElectronegativity[ChemEl::Ge] = 2.01;
															elementToPaulingElectronegativity[ChemEl::As] = 2.18;
															elementToPaulingElectronegativity[ChemEl::Se] = 2.55;
															elementToPaulingElectronegativity[ChemEl::Br] = 2.96;
															elementToPaulingElectronegativity[ChemEl::Kr] = 3.00;
															elementToPaulingElectronegativity[ChemEl::Rb] = 0.82;
															elementToPaulingElectronegativity[ChemEl::Sr] = 0.95;
															elementToPaulingElectronegativity[ChemEl::Y] = 1.22;
															elementToPaulingElectronegativity[ChemEl::Zr] = 1.33;
															elementToPaulingElectronegativity[ChemEl::Nb] = 1.6;
															elementToPaulingElectronegativity[ChemEl::Mo] = 2.16;
															elementToPaulingElectronegativity[ChemEl::Tc] = 1.9;
															elementToPaulingElectronegativity[ChemEl::Ru] = 2.2;
															elementToPaulingElectronegativity[ChemEl::Rh] = 2.28;
															elementToPaulingElectronegativity[ChemEl::Pd] = 2.20;
															elementToPaulingElectronegativity[ChemEl::Ag] = 1.93;
															elementToPaulingElectronegativity[ChemEl::Cd] = 1.69;
															elementToPaulingElectronegativity[ChemEl::In] = 1.78;
															elementToPaulingElectronegativity[ChemEl::Sn] = 1.96;
															elementToPaulingElectronegativity[ChemEl::Sb] = 2.05;
															elementToPaulingElectronegativity[ChemEl::Te] = 2.1;
															elementToPaulingElectronegativity[ChemEl::I] = 2.66;
															elementToPaulingElectronegativity[ChemEl::Xe] = 2.60;
															elementToPaulingElectronegativity[ChemEl::Cs] = 0.79;
															elementToPaulingElectronegativity[ChemEl::Ba] = 0.89;
															elementToPaulingElectronegativity[ChemEl::La] = 1.1;
															elementToPaulingElectronegativity[ChemEl::Ce] = 1.12;
															elementToPaulingElectronegativity[ChemEl::Pr] = 1.13;
															elementToPaulingElectronegativity[ChemEl::Nd] = 1.14;
															elementToPaulingElectronegativity[ChemEl::Pm] = 1.13;
															elementToPaulingElectronegativity[ChemEl::Sm] = 1.17;
															elementToPaulingElectronegativity[ChemEl::Eu] = 1.2;
															elementToPaulingElectronegativity[ChemEl::Gd] = 1.2;
															elementToPaulingElectronegativity[ChemEl::Tb] = 1.1;
															elementToPaulingElectronegativity[ChemEl::Dy] = 1.22;
															elementToPaulingElectronegativity[ChemEl::Ho] = 1.23;
															elementToPaulingElectronegativity[ChemEl::Er] = 1.24;
															elementToPaulingElectronegativity[ChemEl::Tm] = 1.25;
															elementToPaulingElectronegativity[ChemEl::Yb] = 1.1;
															elementToPaulingElectronegativity[ChemEl::Lu] = 1.27;
															elementToPaulingElectronegativity[ChemEl::Hf] = 1.3;
															elementToPaulingElectronegativity[ChemEl::Ta] = 1.5;
															elementToPaulingElectronegativity[ChemEl::W] = 2.36;
															elementToPaulingElectronegativity[ChemEl::Re] = 1.9;
															elementToPaulingElectronegativity[ChemEl::Os] = 2.2;
															elementToPaulingElectronegativity[ChemEl::Ir] = 2.20;
															elementToPaulingElectronegativity[ChemEl::Pt] = 2.28;
															elementToPaulingElectronegativity[ChemEl::Au] = 2.54;
															elementToPaulingElectronegativity[ChemEl::Hg] = 2.00;
															elementToPaulingElectronegativity[ChemEl::Tl] = 1.62;
															elementToPaulingElectronegativity[ChemEl::Pb] = 2.33;
															elementToPaulingElectronegativity[ChemEl::Bi] = 2.02;
															elementToPaulingElectronegativity[ChemEl::Po] = 2.0;
															elementToPaulingElectronegativity[ChemEl::At] = 2.2;
															elementToPaulingElectronegativity[ChemEl::Rn] = 2.2;
															elementToPaulingElectronegativity[ChemEl::Fr] = 0.7;
															elementToPaulingElectronegativity[ChemEl::Ra] = 0.9;
															elementToPaulingElectronegativity[ChemEl::Ac] = 1.1;
															elementToPaulingElectronegativity[ChemEl::Th] = 1.3;
															elementToPaulingElectronegativity[ChemEl::Pa] = 1.5;
															elementToPaulingElectronegativity[ChemEl::U] = 1.38;
															elementToPaulingElectronegativity[ChemEl::Np] = 1.36;
															elementToPaulingElectronegativity[ChemEl::Pu] = 1.28;
															elementToPaulingElectronegativity[ChemEl::Am] = 1.13;
															elementToPaulingElectronegativity[ChemEl::Cm] = 1.28;
															elementToPaulingElectronegativity[ChemEl::Bk] = 1.3;
															elementToPaulingElectronegativity[ChemEl::Cf] = 1.3;
															elementToPaulingElectronegativity[ChemEl::Es] = 1.3;
															elementToPaulingElectronegativity[ChemEl::Fm] = 1.3;
															elementToPaulingElectronegativity[ChemEl::Md] = 1.3;
															elementToPaulingElectronegativity[ChemEl::No] = 1.3;
															elementToPaulingElectronegativity[ChemEl::Lr] = 1.3;
                                                    
															elementToHwPriority[ChemEl::F] = 23;
															elementToHwPriority[ChemEl::Cl] = 22;
															elementToHwPriority[ChemEl::Br] = 21;
															elementToHwPriority[ChemEl::I] = 20;
															elementToHwPriority[ChemEl::O] = 19;
															elementToHwPriority[ChemEl::S] = 18;
															elementToHwPriority[ChemEl::Se] = 17;
															elementToHwPriority[ChemEl::Te] = 16;
															elementToHwPriority[ChemEl::N] = 15;
															elementToHwPriority[ChemEl::P] = 14;
															elementToHwPriority[ChemEl::As] = 13;
															elementToHwPriority[ChemEl::Sb] = 12;
															elementToHwPriority[ChemEl::Bi] = 11;
															elementToHwPriority[ChemEl::Si] = 10;
															elementToHwPriority[ChemEl::Ge] = 9;
															elementToHwPriority[ChemEl::Sn] = 8;
															elementToHwPriority[ChemEl::Pb] = 7;
															elementToHwPriority[ChemEl::B] = 6;
															elementToHwPriority[ChemEl::Al] = 5;
															elementToHwPriority[ChemEl::Ga] = 4;
															elementToHwPriority[ChemEl::In] = 3;
															elementToHwPriority[ChemEl::Tl] = 2;
															elementToHwPriority[ChemEl::Hg] = 1;
						}

AtomProperties::StaticConstructor AtomProperties::staticConstructor;

						boost::optional<double> AtomProperties::getPaulingElectronegativity(ChemEl chemEl)
						{
							return elementToPaulingElectronegativity[chemEl];
						}

						boost::optional<int> AtomProperties::getHwpriority(ChemEl chemEl)
						{
							return elementToHwPriority[chemEl];
						}
					}
				}
			}
		}
	}
}
