#include "ValencyChecker.h"
#include "Atom.h"


const std::unordered_map<ChemEl, int> ValencyChecker::expectedDefaultValency = new java::util::EnumMap<ChemEl, int>(
        ChemEl::
typeid);
const std::unordered_map<ChemEl, int> ValencyChecker::valencyInHW = new java::util::EnumMap<ChemEl, int>(ChemEl::
typeid);
const std::unordered_map<ChemEl, std::unordered_map<int, std::vector<Integer>>> ValencyChecker::possibleStableValencies = new java::util::EnumMap<ChemEl, std::unordered_map<int, std::vector<Integer>>>(
        ChemEl::
typeid);

ValencyChecker::StaticConstructor::StaticConstructor() {
    expectedDefaultValency[ChemEl::B] = 3;
    expectedDefaultValency[ChemEl::Al] = 3;
    expectedDefaultValency[ChemEl::In] = 3;
    expectedDefaultValency[ChemEl::Ga] = 3;
    expectedDefaultValency[ChemEl::Tl] = 3;
    expectedDefaultValency[ChemEl::C] = 4;
    expectedDefaultValency[ChemEl::Si] = 4;
    expectedDefaultValency[ChemEl::Ge] = 4;
    expectedDefaultValency[ChemEl::Sn] = 4;
    expectedDefaultValency[ChemEl::Pb] = 4;
    expectedDefaultValency[ChemEl::N] = 3;
    expectedDefaultValency[ChemEl::P] = 3;
    expectedDefaultValency[ChemEl::As] = 3;
    expectedDefaultValency[ChemEl::Sb] = 3;
    expectedDefaultValency[ChemEl::Bi] = 3;
    expectedDefaultValency[ChemEl::O] = 2;
    expectedDefaultValency[ChemEl::S] = 2;
    expectedDefaultValency[ChemEl::Se] = 2;
    expectedDefaultValency[ChemEl::Te] = 2;
    expectedDefaultValency[ChemEl::Po] = 2;
    expectedDefaultValency[ChemEl::F] = 1;
    expectedDefaultValency[ChemEl::Cl] = 1;
    expectedDefaultValency[ChemEl::Br] = 1;
    expectedDefaultValency[ChemEl::I] = 1;
    expectedDefaultValency[ChemEl::At] = 1;

    //in order of priority in the HW system
    valencyInHW[ChemEl::F] = 1;
    valencyInHW[ChemEl::Cl] = 1;
    valencyInHW[ChemEl::Br] = 1;
    valencyInHW[ChemEl::I] = 1;
    valencyInHW[ChemEl::O] = 2;
    valencyInHW[ChemEl::S] = 2;
    valencyInHW[ChemEl::Se] = 2;
    valencyInHW[ChemEl::Te] = 2;
    valencyInHW[ChemEl::N] = 3;
    valencyInHW[ChemEl::P] = 3;
    valencyInHW[ChemEl::As] = 3;
    valencyInHW[ChemEl::Sb] = 3;
    valencyInHW[ChemEl::Bi] = 3;
    valencyInHW[ChemEl::Si] = 4;
    valencyInHW[ChemEl::Ge] = 4;
    valencyInHW[ChemEl::Sn] = 4;
    valencyInHW[ChemEl::Pb] = 4;
    valencyInHW[ChemEl::B] = 3;
    valencyInHW[ChemEl::Al] = 3;
    valencyInHW[ChemEl::Ga] = 3;
    valencyInHW[ChemEl::In] = 3;
    valencyInHW[ChemEl::Tl] = 3;
    valencyInHW[ChemEl::Hg] = 2;

    valencyInHW[ChemEl::C] = 4;

    possibleStableValencies[ChemEl::H] = std::unordered_map<int, std::vector<Integer>>();
    possibleStableValencies[ChemEl::He] = std::unordered_map<int, std::vector<Integer>>();
    possibleStableValencies[ChemEl::Li] = std::unordered_map<int, std::vector<Integer>>();
    possibleStableValencies[ChemEl::Be] = std::unordered_map<int, std::vector<Integer>>();
    possibleStableValencies[ChemEl::B] = std::unordered_map<int, std::vector<Integer>>();
    possibleStableValencies[ChemEl::C] = std::unordered_map<int, std::vector<Integer>>();
    possibleStableValencies[ChemEl::N] = std::unordered_map<int, std::vector<Integer>>();
    possibleStableValencies[ChemEl::O] = std::unordered_map<int, std::vector<Integer>>();
    possibleStableValencies[ChemEl::F] = std::unordered_map<int, std::vector<Integer>>();
    possibleStableValencies[ChemEl::Ne] = std::unordered_map<int, std::vector<Integer>>();
    possibleStableValencies[ChemEl::Na] = std::unordered_map<int, std::vector<Integer>>();
    possibleStableValencies[ChemEl::Mg] = std::unordered_map<int, std::vector<Integer>>();
    possibleStableValencies[ChemEl::Al] = std::unordered_map<int, std::vector<Integer>>();
    possibleStableValencies[ChemEl::Si] = std::unordered_map<int, std::vector<Integer>>();
    possibleStableValencies[ChemEl::P] = std::unordered_map<int, std::vector<Integer>>();
    possibleStableValencies[ChemEl::S] = std::unordered_map<int, std::vector<Integer>>();
    possibleStableValencies[ChemEl::Cl] = std::unordered_map<int, std::vector<Integer>>();
    possibleStableValencies[ChemEl::Ar] = std::unordered_map<int, std::vector<Integer>>();
    possibleStableValencies[ChemEl::K] = std::unordered_map<int, std::vector<Integer>>();
    possibleStableValencies[ChemEl::Ca] = std::unordered_map<int, std::vector<Integer>>();
    possibleStableValencies[ChemEl::Ga] = std::unordered_map<int, std::vector<Integer>>();
    possibleStableValencies[ChemEl::Ge] = std::unordered_map<int, std::vector<Integer>>();
    possibleStableValencies[ChemEl::As] = std::unordered_map<int, std::vector<Integer>>();
    possibleStableValencies[ChemEl::Se] = std::unordered_map<int, std::vector<Integer>>();
    possibleStableValencies[ChemEl::Br] = std::unordered_map<int, std::vector<Integer>>();
    possibleStableValencies[ChemEl::Kr] = std::unordered_map<int, std::vector<Integer>>();
    possibleStableValencies[ChemEl::Rb] = std::unordered_map<int, std::vector<Integer>>();
    possibleStableValencies[ChemEl::Sr] = std::unordered_map<int, std::vector<Integer>>();
    possibleStableValencies[ChemEl::In] = std::unordered_map<int, std::vector<Integer>>();
    possibleStableValencies[ChemEl::Sn] = std::unordered_map<int, std::vector<Integer>>();
    possibleStableValencies[ChemEl::Sb] = std::unordered_map<int, std::vector<Integer>>();
    possibleStableValencies[ChemEl::Te] = std::unordered_map<int, std::vector<Integer>>();
    possibleStableValencies[ChemEl::I] = std::unordered_map<int, std::vector<Integer>>();
    possibleStableValencies[ChemEl::Xe] = std::unordered_map<int, std::vector<Integer>>();
    possibleStableValencies[ChemEl::Cs] = std::unordered_map<int, std::vector<Integer>>();
    possibleStableValencies[ChemEl::Ba] = std::unordered_map<int, std::vector<Integer>>();
    possibleStableValencies[ChemEl::Tl] = std::unordered_map<int, std::vector<Integer>>();
    possibleStableValencies[ChemEl::Pb] = std::unordered_map<int, std::vector<Integer>>();
    possibleStableValencies[ChemEl::Bi] = std::unordered_map<int, std::vector<Integer>>();
    possibleStableValencies[ChemEl::Po] = std::unordered_map<int, std::vector<Integer>>();
    possibleStableValencies[ChemEl::At] = std::unordered_map<int, std::vector<Integer>>();
    possibleStableValencies[ChemEl::Rn] = std::unordered_map<int, std::vector<Integer>>();
    possibleStableValencies[ChemEl::Fr] = std::unordered_map<int, std::vector<Integer>>();
    possibleStableValencies[ChemEl::Ra] = std::unordered_map<int, std::vector<Integer>>();

    possibleStableValencies[ChemEl::H][0] = std::vector<boost::optional<int> *>{1};
    possibleStableValencies[ChemEl::He][0] = std::vector<boost::optional<int> *>{0};
    possibleStableValencies[ChemEl::Li][0] = std::vector<boost::optional<int> *>{1};
    possibleStableValencies[ChemEl::Be][0] = std::vector<boost::optional<int> *>{2};
    possibleStableValencies[ChemEl::B][0] = std::vector<boost::optional<int> *>{3};
    possibleStableValencies[ChemEl::C][0] = std::vector<boost::optional<int> *>{4};
    possibleStableValencies[ChemEl::N][0] = std::vector<boost::optional<int> *>{3};
    possibleStableValencies[ChemEl::O][0] = std::vector<boost::optional<int> *>{2};
    possibleStableValencies[ChemEl::F][0] = std::vector<boost::optional<int> *>{1};
    possibleStableValencies[ChemEl::Ne][0] = std::vector<boost::optional<int> *>{0};
    possibleStableValencies[ChemEl::Na][0] = std::vector<boost::optional<int> *>{1};
    possibleStableValencies[ChemEl::Mg][0] = std::vector<boost::optional<int> *>{2};
    possibleStableValencies[ChemEl::Al][0] = std::vector<boost::optional<int> *>{3};
    possibleStableValencies[ChemEl::Si][0] = std::vector<boost::optional<int> *>{4};
    possibleStableValencies[ChemEl::P][0] = std::vector<boost::optional<int> *>{3, 5};
    possibleStableValencies[ChemEl::S][0] = std::vector<boost::optional<int> *>{2, 4, 6};
    possibleStableValencies[ChemEl::Cl][0] = std::vector<boost::optional<int> *>{1, 3, 5, 7};
    possibleStableValencies[ChemEl::Ar][0] = std::vector<boost::optional<int> *>{0};
    possibleStableValencies[ChemEl::K][0] = std::vector<boost::optional<int> *>{1};
    possibleStableValencies[ChemEl::Ca][0] = std::vector<boost::optional<int> *>{2};
    possibleStableValencies[ChemEl::Ga][0] = std::vector<boost::optional<int> *>{3};
    possibleStableValencies[ChemEl::Ge][0] = std::vector<boost::optional<int> *>{4};
    possibleStableValencies[ChemEl::As][0] = std::vector<boost::optional<int> *>{3, 5};
    possibleStableValencies[ChemEl::Se][0] = std::vector<boost::optional<int> *>{2, 4, 6};
    possibleStableValencies[ChemEl::Br][0] = std::vector<boost::optional<int> *>{1, 3, 5, 7};
    possibleStableValencies[ChemEl::Kr][0] = std::vector<boost::optional<int> *>{0, 2};
    possibleStableValencies[ChemEl::Rb][0] = std::vector<boost::optional<int> *>{1};
    possibleStableValencies[ChemEl::Sr][0] = std::vector<boost::optional<int> *>{2};
    possibleStableValencies[ChemEl::In][0] = std::vector<boost::optional<int> *>{3};
    possibleStableValencies[ChemEl::Sn][0] = std::vector<boost::optional<int> *>{2, 4};
    possibleStableValencies[ChemEl::Sb][0] = std::vector<boost::optional<int> *>{3, 5};
    possibleStableValencies[ChemEl::Te][0] = std::vector<boost::optional<int> *>{2, 4, 6};
    possibleStableValencies[ChemEl::I][0] = std::vector<boost::optional<int> *>{1, 3, 5, 7};
    possibleStableValencies[ChemEl::Xe][0] = std::vector<boost::optional<int> *>{0, 2, 4, 6, 8};
    possibleStableValencies[ChemEl::Cs][0] = std::vector<boost::optional<int> *>{1};
    possibleStableValencies[ChemEl::Ba][0] = std::vector<boost::optional<int> *>{2};
    possibleStableValencies[ChemEl::Tl][0] = std::vector<boost::optional<int> *>{1, 3};
    possibleStableValencies[ChemEl::Pb][0] = std::vector<boost::optional<int> *>{2, 4};
    possibleStableValencies[ChemEl::Bi][0] = std::vector<boost::optional<int> *>{3, 5};
    possibleStableValencies[ChemEl::Po][0] = std::vector<boost::optional<int> *>{2, 4, 6};
    possibleStableValencies[ChemEl::At][0] = std::vector<boost::optional<int> *>{1, 3, 5, 7};
    possibleStableValencies[ChemEl::Rn][0] = std::vector<boost::optional<int> *>{0, 2, 4, 6, 8};
    possibleStableValencies[ChemEl::Fr][0] = std::vector<boost::optional<int> *>{1};
    possibleStableValencies[ChemEl::Ra][0] = std::vector<boost::optional<int> *>{2};

    possibleStableValencies[ChemEl::H][1] = std::vector<boost::optional<int> *>{0};
    possibleStableValencies[ChemEl::Li][1] = std::vector<boost::optional<int> *>{0};
    possibleStableValencies[ChemEl::Be][1] = std::vector<boost::optional<int> *>{1};
    possibleStableValencies[ChemEl::Be][2] = std::vector<boost::optional<int> *>{0};
    possibleStableValencies[ChemEl::B][2] = std::vector<boost::optional<int> *>{1};
    possibleStableValencies[ChemEl::B][1] = std::vector<boost::optional<int> *>{2};
    possibleStableValencies[ChemEl::B][-1] = std::vector<boost::optional<int> *>{4};
    possibleStableValencies[ChemEl::B][-2] = std::vector<boost::optional<int> *>{3};
    possibleStableValencies[ChemEl::C][2] = std::vector<boost::optional<int> *>{2};
    possibleStableValencies[ChemEl::C][1] = std::vector<boost::optional<int> *>{3};
    possibleStableValencies[ChemEl::C][-1] = std::vector<boost::optional<int> *>{3};
    possibleStableValencies[ChemEl::C][-2] = std::vector<boost::optional<int> *>{2};
    possibleStableValencies[ChemEl::N][2] = std::vector<boost::optional<int> *>{3};
    possibleStableValencies[ChemEl::N][1] = std::vector<boost::optional<int> *>{4};
    possibleStableValencies[ChemEl::N][-1] = std::vector<boost::optional<int> *>{2};
    possibleStableValencies[ChemEl::N][-2] = std::vector<boost::optional<int> *>{1};
    possibleStableValencies[ChemEl::O][2] = std::vector<boost::optional<int> *>{4};
    possibleStableValencies[ChemEl::O][1] = std::vector<boost::optional<int> *>{3, 5};
    possibleStableValencies[ChemEl::O][-1] = std::vector<boost::optional<int> *>{1};
    possibleStableValencies[ChemEl::O][-2] = std::vector<boost::optional<int> *>{0};
    possibleStableValencies[ChemEl::F][2] = std::vector<boost::optional<int> *>{3, 5};
    possibleStableValencies[ChemEl::F][1] = std::vector<boost::optional<int> *>{2};
    possibleStableValencies[ChemEl::F][-1] = std::vector<boost::optional<int> *>{0};
    possibleStableValencies[ChemEl::Na][1] = std::vector<boost::optional<int> *>{0};
    possibleStableValencies[ChemEl::Na][-1] = std::vector<boost::optional<int> *>{0};
    possibleStableValencies[ChemEl::Mg][2] = std::vector<boost::optional<int> *>{0};
    possibleStableValencies[ChemEl::Al][3] = std::vector<boost::optional<int> *>{0};
    possibleStableValencies[ChemEl::Al][2] = std::vector<boost::optional<int> *>{1};
    possibleStableValencies[ChemEl::Al][1] = std::vector<boost::optional<int> *>{2};
    possibleStableValencies[ChemEl::Al][-1] = std::vector<boost::optional<int> *>{4};
    possibleStableValencies[ChemEl::Al][-2] = std::vector<boost::optional<int> *>{3, 5};
    possibleStableValencies[ChemEl::Si][2] = std::vector<boost::optional<int> *>{2};
    possibleStableValencies[ChemEl::Si][1] = std::vector<boost::optional<int> *>{3};
    possibleStableValencies[ChemEl::Si][-1] = std::vector<boost::optional<int> *>{3, 5};
    possibleStableValencies[ChemEl::Si][-2] = std::vector<boost::optional<int> *>{2};
    possibleStableValencies[ChemEl::P][2] = std::vector<boost::optional<int> *>{3};
    possibleStableValencies[ChemEl::P][1] = std::vector<boost::optional<int> *>{4};
    possibleStableValencies[ChemEl::P][-1] = std::vector<boost::optional<int> *>{2, 4, 6};
    possibleStableValencies[ChemEl::P][-2] = std::vector<boost::optional<int> *>{1, 3, 5, 7};
    possibleStableValencies[ChemEl::S][2] = std::vector<boost::optional<int> *>{4};
    possibleStableValencies[ChemEl::S][1] = std::vector<boost::optional<int> *>{3, 5};
    possibleStableValencies[ChemEl::S][-1] = std::vector<boost::optional<int> *>{1, 3, 5, 7};
    possibleStableValencies[ChemEl::S][-2] = std::vector<boost::optional<int> *>{0};
    possibleStableValencies[ChemEl::Cl][2] = std::vector<boost::optional<int> *>{3, 5};
    possibleStableValencies[ChemEl::Cl][1] = std::vector<boost::optional<int> *>{2, 4, 6};
    possibleStableValencies[ChemEl::Cl][-1] = std::vector<boost::optional<int> *>{0};
    possibleStableValencies[ChemEl::K][1] = std::vector<boost::optional<int> *>{0};
    possibleStableValencies[ChemEl::K][-1] = std::vector<boost::optional<int> *>{0};
    possibleStableValencies[ChemEl::Ca][2] = std::vector<boost::optional<int> *>{0};
    possibleStableValencies[ChemEl::Ca][1] = std::vector<boost::optional<int> *>{1};
    possibleStableValencies[ChemEl::Ga][3] = std::vector<boost::optional<int> *>{0};
    possibleStableValencies[ChemEl::Ga][2] = std::vector<boost::optional<int> *>{1};
    possibleStableValencies[ChemEl::Ga][1] = std::vector<boost::optional<int> *>{0};
    possibleStableValencies[ChemEl::Ga][-1] = std::vector<boost::optional<int> *>{4};
    possibleStableValencies[ChemEl::Ga][-2] = std::vector<boost::optional<int> *>{3, 5};
    possibleStableValencies[ChemEl::Ge][4] = std::vector<boost::optional<int> *>{0};
    possibleStableValencies[ChemEl::Ge][1] = std::vector<boost::optional<int> *>{3};
    possibleStableValencies[ChemEl::Ge][-1] = std::vector<boost::optional<int> *>{3, 5};
    possibleStableValencies[ChemEl::Ge][-2] = std::vector<boost::optional<int> *>{2, 4, 6};
    possibleStableValencies[ChemEl::As][2] = std::vector<boost::optional<int> *>{3};
    possibleStableValencies[ChemEl::As][1] = std::vector<boost::optional<int> *>{4};
    possibleStableValencies[ChemEl::As][-1] = std::vector<boost::optional<int> *>{2, 4, 6};
    possibleStableValencies[ChemEl::As][-2] = std::vector<boost::optional<int> *>{1, 3, 5, 7};
    possibleStableValencies[ChemEl::As][-3] = std::vector<boost::optional<int> *>{0};
    possibleStableValencies[ChemEl::Se][2] = std::vector<boost::optional<int> *>{4};
    possibleStableValencies[ChemEl::Se][1] = std::vector<boost::optional<int> *>{3, 5};
    possibleStableValencies[ChemEl::Se][-1] = std::vector<boost::optional<int> *>{1, 3, 5, 7};
    possibleStableValencies[ChemEl::Se][-2] = std::vector<boost::optional<int> *>{0};
    possibleStableValencies[ChemEl::Br][2] = std::vector<boost::optional<int> *>{3, 5};
    possibleStableValencies[ChemEl::Br][1] = std::vector<boost::optional<int> *>{2, 4, 6};
    possibleStableValencies[ChemEl::Br][-1] = std::vector<boost::optional<int> *>{0};
    possibleStableValencies[ChemEl::Rb][1] = std::vector<boost::optional<int> *>{0};
    possibleStableValencies[ChemEl::Rb][-1] = std::vector<boost::optional<int> *>{0};
    possibleStableValencies[ChemEl::Sr][2] = std::vector<boost::optional<int> *>{0};
    possibleStableValencies[ChemEl::Sr][1] = std::vector<boost::optional<int> *>{1};
    possibleStableValencies[ChemEl::In][3] = std::vector<boost::optional<int> *>{0};
    possibleStableValencies[ChemEl::In][2] = std::vector<boost::optional<int> *>{1};
    possibleStableValencies[ChemEl::In][1] = std::vector<boost::optional<int> *>{0};
    possibleStableValencies[ChemEl::In][-1] = std::vector<boost::optional<int> *>{2, 4};
    possibleStableValencies[ChemEl::In][-2] = std::vector<boost::optional<int> *>{3, 5};
    possibleStableValencies[ChemEl::Sn][4] = std::vector<boost::optional<int> *>{0};
    possibleStableValencies[ChemEl::Sn][2] = std::vector<boost::optional<int> *>{0};
    possibleStableValencies[ChemEl::Sn][1] = std::vector<boost::optional<int> *>{3};
    possibleStableValencies[ChemEl::Sn][-1] = std::vector<boost::optional<int> *>{3, 5};
    possibleStableValencies[ChemEl::Sn][-2] = std::vector<boost::optional<int> *>{2, 4, 6};
    possibleStableValencies[ChemEl::Sb][3] = std::vector<boost::optional<int> *>{0};
    possibleStableValencies[ChemEl::Sb][2] = std::vector<boost::optional<int> *>{3};
    possibleStableValencies[ChemEl::Sb][1] = std::vector<boost::optional<int> *>{2, 4};
    possibleStableValencies[ChemEl::Sb][-1] = std::vector<boost::optional<int> *>{2, 4, 6};
    possibleStableValencies[ChemEl::Sb][-2] = std::vector<boost::optional<int> *>{1, 3, 5, 7};
    possibleStableValencies[ChemEl::Te][2] = std::vector<boost::optional<int> *>{2, 4};
    possibleStableValencies[ChemEl::Te][1] = std::vector<boost::optional<int> *>{3, 5};
    possibleStableValencies[ChemEl::Te][-1] = std::vector<boost::optional<int> *>{1, 3, 5, 7};
    possibleStableValencies[ChemEl::Te][-2] = std::vector<boost::optional<int> *>{0};
    possibleStableValencies[ChemEl::I][2] = std::vector<boost::optional<int> *>{3, 5};
    possibleStableValencies[ChemEl::I][1] = std::vector<boost::optional<int> *>{2, 4, 6};
    possibleStableValencies[ChemEl::I][-1] = std::vector<boost::optional<int> *>{0};
    possibleStableValencies[ChemEl::Cs][1] = std::vector<boost::optional<int> *>{0};
    possibleStableValencies[ChemEl::Cs][-1] = std::vector<boost::optional<int> *>{0};
    possibleStableValencies[ChemEl::Ba][2] = std::vector<boost::optional<int> *>{0};
    possibleStableValencies[ChemEl::Ba][1] = std::vector<boost::optional<int> *>{1};
    possibleStableValencies[ChemEl::Pb][2] = std::vector<boost::optional<int> *>{0};
    possibleStableValencies[ChemEl::Pb][1] = std::vector<boost::optional<int> *>{3};
    possibleStableValencies[ChemEl::Pb][-1] = std::vector<boost::optional<int> *>{3, 5};
    possibleStableValencies[ChemEl::Pb][-2] = std::vector<boost::optional<int> *>{2, 4, 6};
    possibleStableValencies[ChemEl::Bi][3] = std::vector<boost::optional<int> *>{0};
    possibleStableValencies[ChemEl::Bi][2] = std::vector<boost::optional<int> *>{3};
    possibleStableValencies[ChemEl::Bi][1] = std::vector<boost::optional<int> *>{2, 4};
    possibleStableValencies[ChemEl::Bi][-1] = std::vector<boost::optional<int> *>{2, 4, 6};
    possibleStableValencies[ChemEl::Bi][-2] = std::vector<boost::optional<int> *>{1, 3, 5, 7};
    possibleStableValencies[ChemEl::At][2] = std::vector<boost::optional<int> *>{3, 5};
    possibleStableValencies[ChemEl::At][1] = std::vector<boost::optional<int> *>{2, 4, 6};
    possibleStableValencies[ChemEl::At][-1] = std::vector<boost::optional<int> *>{0};
    possibleStableValencies[ChemEl::Fr][1] = std::vector<boost::optional<int> *>{0};
    possibleStableValencies[ChemEl::Ra][2] = std::vector<boost::optional<int> *>{0};
    possibleStableValencies[ChemEl::Ra][1] = std::vector<boost::optional<int> *>{1};
}

ValencyChecker::StaticConstructor ValencyChecker::staticConstructor;

boost::optional<int> ValencyChecker::getMaximumValency(ChemEl chemEl, int charge) {
    std::unordered_map<int, std::vector<Integer>> possibleStableValenciesForEl = possibleStableValencies[chemEl];
    if (possibleStableValenciesForEl.size() > 0) {
        std::vector<boost::optional<int>> possibleStableValenciesForElAndCharge = possibleStableValenciesForEl[charge];
        if (possibleStableValenciesForElAndCharge.size() > 0) {
            return possibleStableValenciesForElAndCharge[possibleStableValenciesForElAndCharge.size() - 1];
        }
    }
    return boost::none;
}

boost::optional<int> ValencyChecker::getMaximumValency(Atom *a) {
    boost::optional<int> maxVal;
    if (a->getLambdaConventionValency()) {
        maxVal = a->getLambdaConventionValency() + a->getProtonsExplicitlyAddedOrRemoved();
    } else {
        maxVal = getMaximumValency(a->getElement(), a->getCharge());
    }
    return maxVal;
}

bool ValencyChecker::checkValency(Atom *a) {
    int valency = a->getIncomingValency() + a->getOutValency();
    boost::optional<int> maxVal = getMaximumValency(a);
    if (!maxVal) {
        return true;
    }
    return valency <= maxVal;
}

bool ValencyChecker::checkValencyAvailableForBond(Atom *a, int bondOrder) {
    int valency = a->getIncomingValency() + bondOrder;
    boost::optional<int> maxVal = getMaximumValency(a);
    if (!maxVal) {
        return true;
    }
    return valency <= maxVal;
}

bool ValencyChecker::checkValencyAvailableForReplacementByHeteroatom(Atom *a, Atom *heteroatom) {
    int valency = a->getIncomingValency();
    valency += a->hasSpareValency() ? 1 : 0;
    valency += a->getOutValency();
    boost::optional<int> maxValOfHeteroAtom = getMaximumValency(heteroatom->getElement(), heteroatom->getCharge());
    return !maxValOfHeteroAtom || valency <= maxValOfHeteroAtom;
}

boost::optional<int> ValencyChecker::getDefaultValency(ChemEl chemlEl) {
    return expectedDefaultValency[chemlEl];
}

boost::optional<int> ValencyChecker::getHWValency(ChemEl chemEl) {
    return valencyInHW[chemEl];
}

std::vector<boost::optional<int>> ValencyChecker::getPossibleValencies(ChemEl chemEl, int charge) {
    std::unordered_map<int, std::vector<Integer>> possibleStableValenciesForEl = possibleStableValencies[chemEl];
    if (possibleStableValenciesForEl.empty()) {
        return nullptr;
    }
    return possibleStableValenciesForEl[charge];
}
