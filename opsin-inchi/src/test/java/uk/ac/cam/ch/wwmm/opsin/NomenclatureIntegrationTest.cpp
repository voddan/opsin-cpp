#include "NomenclatureIntegrationTest.h"
#include "../../../../../../../../../../opsin-core/src/main/java/uk/ac/cam/ch/wwmm/opsin/NameToStructure.h"
#include "../../../../../../../../../../opsin-core/src/main/java/uk/ac/cam/ch/wwmm/opsin/NameToStructureConfig.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/NameToInchi.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/InchiPruner.h"

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
						using org::apache::commons::io::IOUtils;
						using org::junit::AfterClass;
						using org::junit::BeforeClass;
						using org::junit::Test;
NameToStructure *NomenclatureIntegrationTest::n2s;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @BeforeClass public static void setUp()
						void NomenclatureIntegrationTest::setUp()
						{
							n2s = NameToStructure::getInstance();
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @AfterClass public static void cleanUp()
						void NomenclatureIntegrationTest::cleanUp()
						{
							n2s = nullptr;
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testRadicals() throws java.io.IOException
						void NomenclatureIntegrationTest::testRadicals() throw(IOException)
						{
							NameToStructureConfig *n2sConfig = NameToStructureConfig::getDefaultConfigInstance();
							n2sConfig->setAllowRadicals(true);
							std::wstring file = L"radicals.txt";
							checkNamesAgainstInChIs(file, n2sConfig);
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testAcetals() throws java.io.IOException
						void NomenclatureIntegrationTest::testAcetals() throw(IOException)
						{
							NameToStructureConfig *n2sConfig = NameToStructureConfig::getDefaultConfigInstance();
							n2sConfig->setAllowRadicals(true);
							std::wstring file = L"acetals.txt";
							checkNamesAgainstInChIs(file, n2sConfig);
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testAlcoholEsters() throws java.io.IOException
						void NomenclatureIntegrationTest::testAlcoholEsters() throw(IOException)
						{
							NameToStructureConfig *n2sConfig = NameToStructureConfig::getDefaultConfigInstance();
							n2sConfig->setAllowRadicals(true);
							std::wstring file = L"alcoholEsters.txt";
							checkNamesAgainstInChIs(file, n2sConfig);
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testCarbohydrates() throws java.io.IOException
						void NomenclatureIntegrationTest::testCarbohydrates() throw(IOException)
						{
							NameToStructureConfig *n2sConfig = NameToStructureConfig::getDefaultConfigInstance();
							n2sConfig->setAllowRadicals(true);
							std::wstring file = L"carbohydrates.txt";
							checkNamesAgainstInChIs(file, n2sConfig);
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testChargeBalancing() throws java.io.IOException
						void NomenclatureIntegrationTest::testChargeBalancing() throw(IOException)
						{
							NameToStructureConfig *n2sConfig = NameToStructureConfig::getDefaultConfigInstance();
							n2sConfig->setAllowRadicals(true);
							std::wstring file = L"chargeBalancing.txt";
							checkNamesAgainstInChIs(file, n2sConfig);
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testConjunctiveNomenclature() throws java.io.IOException
						void NomenclatureIntegrationTest::testConjunctiveNomenclature() throw(IOException)
						{
							NameToStructureConfig *n2sConfig = NameToStructureConfig::getDefaultConfigInstance();
							n2sConfig->setAllowRadicals(true);
							std::wstring file = L"conjunctiveNomenclature.txt";
							checkNamesAgainstInChIs(file, n2sConfig);
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testCyclicSuffixes() throws java.io.IOException
						void NomenclatureIntegrationTest::testCyclicSuffixes() throw(IOException)
						{
							NameToStructureConfig *n2sConfig = NameToStructureConfig::getDefaultConfigInstance();
							n2sConfig->setAllowRadicals(true);
							std::wstring file = L"cyclicSuffixes.txt";
							checkNamesAgainstInChIs(file, n2sConfig);
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testEpoxyLike() throws java.io.IOException
						void NomenclatureIntegrationTest::testEpoxyLike() throw(IOException)
						{
							NameToStructureConfig *n2sConfig = NameToStructureConfig::getDefaultConfigInstance();
							n2sConfig->setAllowRadicals(true);
							std::wstring file = L"epoxyLike.txt";
							checkNamesAgainstInChIs(file, n2sConfig);
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testFunctionalReplacement() throws java.io.IOException
						void NomenclatureIntegrationTest::testFunctionalReplacement() throw(IOException)
						{
							NameToStructureConfig *n2sConfig = NameToStructureConfig::getDefaultConfigInstance();
							n2sConfig->setAllowRadicals(true);
							std::wstring file = L"functionalReplacement.txt";
							checkNamesAgainstInChIs(file, n2sConfig);
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testIsotopes() throws java.io.IOException
						void NomenclatureIntegrationTest::testIsotopes() throw(IOException)
						{
							NameToStructureConfig *n2sConfig = NameToStructureConfig::getDefaultConfigInstance();
							n2sConfig->setAllowRadicals(true);
							std::wstring file = L"isotopes.txt";
							checkNamesAgainstInChIs(file, n2sConfig);
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testAdditiveNomenclature() throws java.io.IOException
						void NomenclatureIntegrationTest::testAdditiveNomenclature() throw(IOException)
						{
							NameToStructureConfig *n2sConfig = NameToStructureConfig::getDefaultConfigInstance();
							n2sConfig->setAllowRadicals(true);
							std::wstring file = L"additiveNomenclature.txt";
							checkNamesAgainstInChIs(file, n2sConfig);
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testMultiplicativeNomenclature() throws java.io.IOException
						void NomenclatureIntegrationTest::testMultiplicativeNomenclature() throw(IOException)
						{
							NameToStructureConfig *n2sConfig = NameToStructureConfig::getDefaultConfigInstance();
							n2sConfig->setAllowRadicals(true);
							std::wstring file = L"multiplicativeNomenclature.txt";
							checkNamesAgainstInChIs(file, n2sConfig);
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testOmittedSpaces() throws java.io.IOException
						void NomenclatureIntegrationTest::testOmittedSpaces() throw(IOException)
						{
							NameToStructureConfig *n2sConfig = NameToStructureConfig::getDefaultConfigInstance();
							n2sConfig->setAllowRadicals(true);
							std::wstring file = L"omittedSpaces.txt";
							checkNamesAgainstInChIs(file, n2sConfig);
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testFunctionalClassNomenclature() throws java.io.IOException
						void NomenclatureIntegrationTest::testFunctionalClassNomenclature() throw(IOException)
						{
							NameToStructureConfig *n2sConfig = NameToStructureConfig::getDefaultConfigInstance();
							n2sConfig->setAllowRadicals(true);
							std::wstring file = L"functionalClasses.txt";
							checkNamesAgainstInChIs(file, n2sConfig);
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testFusedRingNomenclature() throws java.io.IOException
						void NomenclatureIntegrationTest::testFusedRingNomenclature() throw(IOException)
						{
							NameToStructureConfig *n2sConfig = NameToStructureConfig::getDefaultConfigInstance();
							n2sConfig->setAllowRadicals(true);
							std::wstring file = L"fusedRings.txt";
							checkNamesAgainstInChIs(file, n2sConfig);
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testInorganicNomenclature() throws java.io.IOException
						void NomenclatureIntegrationTest::testInorganicNomenclature() throw(IOException)
						{
							NameToStructureConfig *n2sConfig = NameToStructureConfig::getDefaultConfigInstance();
							n2sConfig->setAllowRadicals(true);
							std::wstring file = L"inorganics.txt";
							checkNamesAgainstInChIs(file, n2sConfig);
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testIonNomenclature() throws java.io.IOException
						void NomenclatureIntegrationTest::testIonNomenclature() throw(IOException)
						{
							NameToStructureConfig *n2sConfig = NameToStructureConfig::getDefaultConfigInstance();
							n2sConfig->setAllowRadicals(true);
							std::wstring file = L"ions.txt";
							checkNamesAgainstInChIs(file, n2sConfig);
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testSpiroNomenclature() throws java.io.IOException
						void NomenclatureIntegrationTest::testSpiroNomenclature() throw(IOException)
						{
							NameToStructureConfig *n2sConfig = NameToStructureConfig::getDefaultConfigInstance();
							n2sConfig->setAllowRadicals(true);
							std::wstring file = L"spiro.txt";
							checkNamesAgainstInChIs(file, n2sConfig);
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testOrganoMetallics() throws java.io.IOException
						void NomenclatureIntegrationTest::testOrganoMetallics() throw(IOException)
						{
							NameToStructureConfig *n2sConfig = NameToStructureConfig::getDefaultConfigInstance();
							n2sConfig->setAllowRadicals(true);
							std::wstring file = L"organometallics.txt";
							checkNamesAgainstInChIs(file, n2sConfig);
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testImplicitBracketting() throws java.io.IOException
						void NomenclatureIntegrationTest::testImplicitBracketting() throw(IOException)
						{
							NameToStructureConfig *n2sConfig = NameToStructureConfig::getDefaultConfigInstance();
							n2sConfig->setAllowRadicals(true);
							std::wstring file = L"implicitBracketting.txt";
							checkNamesAgainstInChIs(file, n2sConfig);
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testStereochemistry() throws java.io.IOException
						void NomenclatureIntegrationTest::testStereochemistry() throw(IOException)
						{
							NameToStructureConfig *n2sConfig = NameToStructureConfig::getDefaultConfigInstance();
							n2sConfig->setAllowRadicals(true);
							std::wstring file = L"stereochemistry.txt";
							checkNamesAgainstInChIs(file, n2sConfig);
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testMiscellany() throws java.io.IOException
						void NomenclatureIntegrationTest::testMiscellany() throw(IOException)
						{
							NameToStructureConfig *n2sConfig = NameToStructureConfig::getDefaultConfigInstance();
							n2sConfig->setAllowRadicals(true);
							std::wstring file = L"miscellany.txt";
							checkNamesAgainstInChIs(file, n2sConfig);
						}

						void NomenclatureIntegrationTest::checkNamesAgainstInChIs(const std::wstring &file, NameToStructureConfig *n2sConfig) throw(IOException)
						{
							InputStreamReader tempVar(this->getClass().getResourceAsStream(file), L"UTF-8");
							BufferedReader *input = new BufferedReader(&tempVar);
							try
							{
								std::wstring line = L"";
								while ((line = input->readLine()) != L"")
								{
									if (boost::starts_with(line, L"//"))
									{
										continue;
									}
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
									std::vector<std::wstring> lineArray = line.split(L"\t");
									std::wstring inchi = NameToInchi::convertResultToInChI(n2s->parseChemicalName(lineArray[0], n2sConfig));
									if (inchi != L"")
									{
										std::wstring opsinInchi = InchiPruner::mainChargeAndStereochemistryLayers(inchi);
										std::wstring referenceInchi = InchiPruner::mainChargeAndStereochemistryLayers(lineArray[1]);

										if (opsinInchi != referenceInchi)
										{
											fail(lineArray[0] + std::wstring(L" was misinterpreted as: ") + inchi);
										}
									}
									else
									{
										fail(lineArray[0] + std::wstring(L" was uninterpretable"));
									}
								}
							}
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to the exception 'finally' clause:
							finally
							{
								IOUtils::closeQuietly(input);
							}
						}
					}
				}
			}
		}
	}
}
