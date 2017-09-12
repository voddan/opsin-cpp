#include "CASToolsTest.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/ParseRules.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/ResourceGetter.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/ResourceManager.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/ParsingException.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/CASTools.h"

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
//						import static org.junit.Assert.*;
						using org::junit::AfterClass;
						using org::junit::BeforeClass;
						using org::junit::Test;
ParseRules *CASToolsTest::parseRules;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @BeforeClass public static void setUp() throws java.io.IOException
						void CASToolsTest::setUp() throw(IOException)
						{
							ResourceGetter *rg = new ResourceGetter(L"uk/ac/cam/ch/wwmm/opsin/resources/");
							ResourceManager tempVar(rg);
							parseRules = new ParseRules(&tempVar);
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @AfterClass public static void cleanUp()
						void CASToolsTest::cleanUp()
						{
							parseRules = nullptr;
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void cas1() throws ParsingException
						void CASToolsTest::cas1() throw(ParsingException)
						{
							std::wstring name = CASTools::uninvertCASName(L"Silane, chloromethyl-", parseRules);
							assertEquals(L"chloromethyl-Silane", name);
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void cas2() throws ParsingException
						void CASToolsTest::cas2() throw(ParsingException)
						{
							std::wstring name = CASTools::uninvertCASName(L"Acetic acid, 2-ethoxy-2-thioxo-", parseRules);
							assertEquals(L"2-ethoxy-2-thioxo-Acetic acid", name);
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void cas3() throws ParsingException
						void CASToolsTest::cas3() throw(ParsingException)
						{
							std::wstring name = CASTools::uninvertCASName(L"Silanol, 1,1'-methylenebis-", parseRules);
							assertEquals(L"1,1'-methylenebis-Silanol", name);
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void cas4() throws ParsingException
						void CASToolsTest::cas4() throw(ParsingException)
						{
							std::wstring name = CASTools::uninvertCASName(L"Phosphonic acid, P,P'-(8-methylene-3,7,10,14-tetraoxo-4,6,11,13-tetraazahexadecane-1,16-diyl)-bis-, P,P,P',P'-tetramethyl ester", parseRules);
							assertEquals(L"P,P,P',P'-tetramethyl P,P'-(8-methylene-3,7,10,14-tetraoxo-4,6,11,13-tetraazahexadecane-1,16-diyl)-bis-Phosphonate", name);
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void cas5() throws ParsingException
						void CASToolsTest::cas5() throw(ParsingException)
						{
							std::wstring name = CASTools::uninvertCASName(L"Benzenamine, 3,3',3''-(1-ethenyl-2-ylidene)tris[6-methyl-", parseRules);
							assertEquals(L"3,3',3''-(1-ethenyl-2-ylidene)tris[6-methyl-Benzenamine]", name);
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void cas6() throws ParsingException
						void CASToolsTest::cas6() throw(ParsingException)
						{
							std::wstring name = CASTools::uninvertCASName(L"Pyridine, 3,3'-thiobis[6-chloro-", parseRules);
							assertEquals(L"3,3'-thiobis[6-chloro-Pyridine]", name);
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void cas7() throws ParsingException
						void CASToolsTest::cas7() throw(ParsingException)
						{
							std::wstring name = CASTools::uninvertCASName(L"1-Butanesulfonic acid, 2,4-diamino-3-chloro- 1-ethyl ester", parseRules);
							assertEquals(L"1-ethyl 2,4-diamino-3-chloro-1-Butanesulfonate", name);
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void cas8() throws ParsingException
						void CASToolsTest::cas8() throw(ParsingException)
						{
							std::wstring name = CASTools::uninvertCASName(L"Benzenecarboximidamide, N'-(1E)-1-propen-1-yl-N-(1Z)-1-propen-1-yl-", parseRules);
							assertEquals(L"N'-(1E)-1-propen-1-yl-N-(1Z)-1-propen-1-yl-Benzenecarboximidamide", name);
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void cas9() throws ParsingException
						void CASToolsTest::cas9() throw(ParsingException)
						{
							std::wstring name = CASTools::uninvertCASName(L"Phosphoric acid, ethyl dimethyl ester", parseRules);
							assertEquals(L"ethyl dimethyl Phosphorate", name);
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void cas10() throws ParsingException
						void CASToolsTest::cas10() throw(ParsingException)
						{
							std::wstring name = CASTools::uninvertCASName(L"2-Propanone, oxime", parseRules);
							assertEquals(L"2-Propanone oxime", name);
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void cas11() throws ParsingException
						void CASToolsTest::cas11() throw(ParsingException)
						{
							std::wstring name = CASTools::uninvertCASName(L"Disulfide, bis(2-chloroethyl)", parseRules);
							assertEquals(L"bis(2-chloroethyl) Disulfide", name);
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void cas12() throws ParsingException
						void CASToolsTest::cas12() throw(ParsingException)
						{
							std::wstring name = CASTools::uninvertCASName(L"Ethanimidic acid, N-nitro-, (1Z)-", parseRules);
							assertEquals(L"(1Z)-N-nitro-Ethanimidic acid", name);
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void cas13() throws ParsingException
						void CASToolsTest::cas13() throw(ParsingException)
						{
							std::wstring name = CASTools::uninvertCASName(L"2(1H)-Pyridinone, hydrazone, (2E)-", parseRules);
							assertEquals(L"(2E)-2(1H)-Pyridinone hydrazone", name);
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void cas14() throws ParsingException
						void CASToolsTest::cas14() throw(ParsingException)
						{
							std::wstring name = CASTools::uninvertCASName(L"benzoic acid, 4,4'-methylenebis[2-chloro-", parseRules);
							assertEquals(L"4,4'-methylenebis[2-chloro-benzoic acid]", name);
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void cas15() throws ParsingException
						void CASToolsTest::cas15() throw(ParsingException)
						{
							std::wstring name = CASTools::uninvertCASName(L"peroxide, ethyl methyl", parseRules);
							assertEquals(L"ethyl methyl peroxide", name);
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void cas16() throws ParsingException
						void CASToolsTest::cas16() throw(ParsingException)
						{
							std::wstring name = CASTools::uninvertCASName(L"Phosphonic diamide, P-phenyl- (8CI9CI)", parseRules);
							assertEquals(L"P-phenyl-Phosphonic diamide", name);
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void cas17() throws ParsingException
						void CASToolsTest::cas17() throw(ParsingException)
						{
							std::wstring name = CASTools::uninvertCASName(L"piperazinium, 1,1-dimethyl-, 2,2,2-trifluoroacetate hydrochloride", parseRules);
							assertEquals(L"1,1-dimethyl-piperazinium 2,2,2-trifluoroacetate hydrochloride", name);
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void cas18() throws ParsingException
						void CASToolsTest::cas18() throw(ParsingException)
						{
							std::wstring name = CASTools::uninvertCASName(L"Acetamide, ethylenebis(((ethyl)amino)-", parseRules);
							assertEquals(L"ethylenebis(((ethyl)amino)-Acetamide)", name);
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void cas19() throws ParsingException
						void CASToolsTest::cas19() throw(ParsingException)
						{
							std::wstring name = CASTools::uninvertCASName(L"Benzenesulfonic acid, 4-amino-, 1-methylhydrazide", parseRules);
							assertEquals(L"4-amino-Benzenesulfonic acid 1-methylhydrazide", name);
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void cas20() throws ParsingException
						void CASToolsTest::cas20() throw(ParsingException)
						{
							std::wstring name = CASTools::uninvertCASName(L"Acetaldehyde, O-methyloxime", parseRules);
							assertEquals(L"Acetaldehyde O-methyloxime", name);
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void cas21() throws ParsingException
						void CASToolsTest::cas21() throw(ParsingException)
						{
							std::wstring name = CASTools::uninvertCASName(L"Acetic acid, 2-amino-2-oxo-, 2-(phenylmethylene)hydrazide", parseRules);
							assertEquals(L"2-amino-2-oxo-Acetic acid 2-(phenylmethylene)hydrazide", name);
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void cas22() throws ParsingException
						void CASToolsTest::cas22() throw(ParsingException)
						{
							std::wstring name = CASTools::uninvertCASName(L"L-Alanine, N-carboxy-, 1-ethyl ester", parseRules);
							assertEquals(L"1-ethyl N-carboxy-L-Alaninate", name);
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void cas23() throws ParsingException
						void CASToolsTest::cas23() throw(ParsingException)
						{
							std::wstring name = CASTools::uninvertCASName(L"Pyridine, 3-(tetrahydro-2H-pyran-2-yl)-, (S)-", parseRules);
							assertEquals(L"(S)-3-(tetrahydro-2H-pyran-2-yl)-Pyridine", name);
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void cas24() throws ParsingException
						void CASToolsTest::cas24() throw(ParsingException)
						{
							std::wstring name = CASTools::uninvertCASName(L"Pyrrolo[1,2-a]pyrimidinium, 1-[4-[(aminoiminomethyl)amino]butyl]-7-[[2-[(aminoiminomethyl)-amino]ethyl]thio]-6-(11-dodecenyl)-2,3,4,6,7,8-hexahydro-6-hydroxy-, chloride, dihydrochloride", parseRules);
							assertEquals(L"1-[4-[(aminoiminomethyl)amino]butyl]-7-[[2-[(aminoiminomethyl)-amino]ethyl]thio]-6-(11-dodecenyl)-2,3,4,6,7,8-hexahydro-6-hydroxy-Pyrrolo[1,2-a]pyrimidinium chloride dihydrochloride", name);
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void cas25() throws ParsingException
						void CASToolsTest::cas25() throw(ParsingException)
						{
							//In acetic acid, sodium salt (1:1), the stoichiometry is removed prior to uninversion
							std::wstring name = CASTools::uninvertCASName(L"acetic acid, sodium salt", parseRules);
							assertEquals(L"acetic acid sodium salt", name);
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void commaDelimitedAcidSalt() throws ParsingException
						void CASToolsTest::commaDelimitedAcidSalt() throw(ParsingException)
						{
							//I don't think this is actually a CAS name, but it's easiest to correct it in this function 
							std::wstring name = CASTools::uninvertCASName(L"benzamide, trifluoroacetic acid salt", parseRules);
							assertEquals(L"benzamide trifluoroacetic acid salt", name);
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected=ParsingException.class) public void notCas1() throws ParsingException
						void CASToolsTest::notCas1() throw(ParsingException)
						{
							CASTools::uninvertCASName(L"hexanamine, hexylamine", parseRules);
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected=ParsingException.class) public void notCas2() throws ParsingException
						void CASToolsTest::notCas2() throw(ParsingException)
						{
							CASTools::uninvertCASName(L"cyclopropane-1,2-diyldicarbonyl diisocyanate, cyclopropane-1,2-diylbis(carbonyl)bisisocyanate", parseRules);
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected=ParsingException.class) public void notCas3() throws ParsingException
						void CASToolsTest::notCas3() throw(ParsingException)
						{
							CASTools::uninvertCASName(L"benzoic acid, ester", parseRules);
						}
					}
				}
			}
		}
	}
}
