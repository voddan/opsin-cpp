#pragma once

#include <vector>
#include <stdexcept>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
class Fragment;
class FragmentManager;
class StructureBuildingException;





//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//						import static org.junit.Assert.*;
using namespace uk::ac::cam::ch::wwmm::opsin;
//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//						import static uk.ac.cam.ch.wwmm.opsin.XmlDeclarations.*;

class FragmentTest {

private:
    Fragment *frag;
    FragmentManager *fm;

public:
    virtual ~FragmentTest() {
        delete frag;
        delete fm;
    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Before public void setUp()
    virtual void setUp();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testFragment()
    virtual void testFragment();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testAddAtom()
    virtual void testAddAtom();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testAddBond()
    virtual void testAddBond();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testImportFrag() throws StructureBuildingException
    virtual void testImportFrag() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testImportFragWithIntraFragBonds1() throws StructureBuildingException
    virtual void testImportFragWithIntraFragBonds1() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testImportFragWithIntraFragBonds2() throws StructureBuildingException
    virtual void testImportFragWithIntraFragBonds2() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testGetIDFromLocant()
    virtual void testGetIDFromLocant();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testGetAtomByLocant()
    virtual void testGetAtomByLocant();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testGetAtomByID()
    virtual void testGetAtomByID();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testFindBond()
    virtual void testFindBond();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testGetChainLength()
    virtual void testGetChainLength();


//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testRelabelSuffixLocants() throws StructureBuildingException
    virtual void testRelabelSuffixLocants() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testLabelCarbamimidamido() throws StructureBuildingException
    virtual void testLabelCarbamimidamido() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testLabelHydrazonoHydrazide() throws StructureBuildingException
    virtual void testLabelHydrazonoHydrazide() throw(StructureBuildingException);


//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testLabelCarbonimidoyl() throws StructureBuildingException
    virtual void testLabelCarbonimidoyl() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testLabelHydrazonicAmide() throws StructureBuildingException
    virtual void testLabelHydrazonicAmide() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testLabelHydrazonate() throws StructureBuildingException
    virtual void testLabelHydrazonate() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testLabelHexanDiamide() throws StructureBuildingException
    virtual void testLabelHexanDiamide() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testLabelDiimidooxalicDiamide() throws StructureBuildingException
    virtual void testLabelDiimidooxalicDiamide() throw(StructureBuildingException);


//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testLabelHydrazinecarbohydrazide() throws StructureBuildingException
    virtual void testLabelHydrazinecarbohydrazide() throw(StructureBuildingException);


//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testLabelCarbonicDihydrazide() throws StructureBuildingException
    virtual void testLabelCarbonicDihydrazide() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testLabelSulfonoThioate() throws StructureBuildingException
    virtual void testLabelSulfonoThioate() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testLabelAcetoanilide() throws StructureBuildingException
    virtual void testLabelAcetoanilide() throw(StructureBuildingException);


//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testLabelPyridine() throws StructureBuildingException
    virtual void testLabelPyridine() throw(StructureBuildingException);


//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testLabelPiperazine() throws StructureBuildingException
    virtual void testLabelPiperazine() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testLabelCarboximidohydrazide() throws StructureBuildingException
    virtual void testLabelCarboximidohydrazide() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testIndicatedHydrogen() throws StructureBuildingException
    virtual void testIndicatedHydrogen() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testSpareValenciesOnAromaticAtoms() throws StructureBuildingException
    virtual void testSpareValenciesOnAromaticAtoms() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testConvertSpareValenciesToDoubleBonds() throws StructureBuildingException
    virtual void testConvertSpareValenciesToDoubleBonds() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testGetAtomNeighbours() throws StructureBuildingException
    virtual void testGetAtomNeighbours() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testIsCharacteristicAtomSuffix() throws StructureBuildingException
    virtual void testIsCharacteristicAtomSuffix() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testIsCharacteristicAtomAldehyde() throws StructureBuildingException
    virtual void testIsCharacteristicAtomAldehyde() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testIsCharacteristicAtomFunctionalAtom() throws StructureBuildingException
    virtual void testIsCharacteristicAtomFunctionalAtom() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testIsCharacteristicAtomHydroxy() throws StructureBuildingException
    virtual void testIsCharacteristicAtomHydroxy() throw(StructureBuildingException);

};

