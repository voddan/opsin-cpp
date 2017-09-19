#pragma once

#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/IDManager.h"
#include <string>
#include <vector>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
class SMILESFragmentBuilder;
class StructureBuildingException;



//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//						import static org.junit.Assert.*;
//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//						import static org.mockito.Mockito.mock;



/// <summary>
/// Tests that fused ring numbering is working as expected. A heteroatom(n) has been placed at the expected locant 1 to make numbering unambiguous where due to symmetry geometric consideration are insufficient to deduce unique numbering
/// Currently interior atoms are not labelled. As this is not seen as a problem, any tests of compounds with interior atoms have not had locants assigned to the interior atoms
/// @author dl387
///
/// </summary>
class FusedRingNumbererTest {

private:
    SMILESFragmentBuilder *sBuilder = new SMILESFragmentBuilder(new IDManager());

public:
    virtual ~FusedRingNumbererTest() {
        delete sBuilder;
    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void aceanthrene() throws StructureBuildingException
    virtual void aceanthrene() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void acenaphthene() throws StructureBuildingException
    virtual void acenaphthene() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void acephenanthrene() throws StructureBuildingException
    virtual void acephenanthrene() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void arsanthrene() throws StructureBuildingException
    virtual void arsanthrene() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void arsanthridine() throws StructureBuildingException
    virtual void arsanthridine() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void arsindole() throws StructureBuildingException
    virtual void arsindole() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void arsindoline() throws StructureBuildingException
    virtual void arsindoline() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void betacarboline() throws StructureBuildingException
    virtual void betacarboline() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void boranthrene() throws StructureBuildingException
    virtual void boranthrene() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void cholanthrene() throws StructureBuildingException
    virtual void cholanthrene() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void thiochromane() throws StructureBuildingException
    virtual void thiochromane() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void selenochromane() throws StructureBuildingException
    virtual void selenochromane() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void tellurochromane() throws StructureBuildingException
    virtual void tellurochromane() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void thiochromene() throws StructureBuildingException
    virtual void thiochromene() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void selenochromene() throws StructureBuildingException
    virtual void selenochromene() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void tellurochromene() throws StructureBuildingException
    virtual void tellurochromene() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void coronene() throws StructureBuildingException
    virtual void coronene() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void indane() throws StructureBuildingException
    virtual void indane() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void isoarsindole() throws StructureBuildingException
    virtual void isoarsindole() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void isoarsinoline() throws StructureBuildingException
    virtual void isoarsinoline() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void thioisochromane() throws StructureBuildingException
    virtual void thioisochromane() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void selenoisochromane() throws StructureBuildingException
    virtual void selenoisochromane() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void telluroisochromane() throws StructureBuildingException
    virtual void telluroisochromane() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void isochromene() throws StructureBuildingException
    virtual void isochromene() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void thioisochromene() throws StructureBuildingException
    virtual void thioisochromene() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void selenoisochromene() throws StructureBuildingException
    virtual void selenoisochromene() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void telluroisochromene() throws StructureBuildingException
    virtual void telluroisochromene() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void isophosphindole() throws StructureBuildingException
    virtual void isophosphindole() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void isophosphinoline() throws StructureBuildingException
    virtual void isophosphinoline() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void isoviolanthrene() throws StructureBuildingException
    virtual void isoviolanthrene() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void mercuranthrene() throws StructureBuildingException
    virtual void mercuranthrene() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test @Ignore public void ovalene() throws StructureBuildingException
    virtual void ovalene() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void oxanthrene() throws StructureBuildingException
    virtual void oxanthrene() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void perylene() throws StructureBuildingException
    virtual void perylene() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void phenanthridine() throws StructureBuildingException
    virtual void phenanthridine() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void phenomercurine() throws StructureBuildingException
    virtual void phenomercurine() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void phenoxazine() throws StructureBuildingException
    virtual void phenoxazine() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void phenothiazine() throws StructureBuildingException
    virtual void phenothiazine() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void phenoselenazine() throws StructureBuildingException
    virtual void phenoselenazine() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void phenotellurazine() throws StructureBuildingException
    virtual void phenotellurazine() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void phenophosphazinine() throws StructureBuildingException
    virtual void phenophosphazinine() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void phenophosphazine() throws StructureBuildingException
    virtual void phenophosphazine() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void phenarsazinine() throws StructureBuildingException
    virtual void phenarsazinine() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void phenoarsazine() throws StructureBuildingException
    virtual void phenoarsazine() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void phenomercurazine() throws StructureBuildingException
    virtual void phenomercurazine() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void phenomercazine() throws StructureBuildingException
    virtual void phenomercazine() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void phenoxathiine() throws StructureBuildingException
    virtual void phenoxathiine() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void phenoxaselenine() throws StructureBuildingException
    virtual void phenoxaselenine() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void phenoxatellurine() throws StructureBuildingException
    virtual void phenoxatellurine() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void phenoxaphosphinine() throws StructureBuildingException
    virtual void phenoxaphosphinine() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void phenoxaphosphine() throws StructureBuildingException
    virtual void phenoxaphosphine() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void phenoxarsinine() throws StructureBuildingException
    virtual void phenoxarsinine() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void phenoxarsine() throws StructureBuildingException
    virtual void phenoxarsine() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void phenoxastibinine() throws StructureBuildingException
    virtual void phenoxastibinine() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void phenoxantimonine() throws StructureBuildingException
    virtual void phenoxantimonine() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void phenothiarsinine() throws StructureBuildingException
    virtual void phenothiarsinine() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void phenothiarsine() throws StructureBuildingException
    virtual void phenothiarsine() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void phosphanthrene() throws StructureBuildingException
    virtual void phosphanthrene() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void phosphindole() throws StructureBuildingException
    virtual void phosphindole() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void phosphinoline() throws StructureBuildingException
    virtual void phosphinoline() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void picene() throws StructureBuildingException
    virtual void picene() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void pleiadene() throws StructureBuildingException
    virtual void pleiadene() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void pyranthrene() throws StructureBuildingException
    virtual void pyranthrene() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void pyrrolizine() throws StructureBuildingException
    virtual void pyrrolizine() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void quinolizine() throws StructureBuildingException
    virtual void quinolizine() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void rubicene() throws StructureBuildingException
    virtual void rubicene() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void silanthrene() throws StructureBuildingException
    virtual void silanthrene() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void selenanthrene() throws StructureBuildingException
    virtual void selenanthrene() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void telluranthrene() throws StructureBuildingException
    virtual void telluranthrene() throw(StructureBuildingException);


//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void thianthrene() throws StructureBuildingException
    virtual void thianthrene() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void trindene() throws StructureBuildingException
    virtual void trindene() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void violanthrene() throws StructureBuildingException
    virtual void violanthrene() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void naphthotetraphene() throws StructureBuildingException
    virtual void naphthotetraphene() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void anthratetraphene() throws StructureBuildingException
    virtual void anthratetraphene() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void octalenotetraphene() throws StructureBuildingException
    virtual void octalenotetraphene() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void difficultChain() throws StructureBuildingException
    virtual void difficultChain() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void difficultChain2() throws StructureBuildingException
    virtual void difficultChain2() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void acrindoline() throws StructureBuildingException
    virtual void acrindoline() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void anthrazine() throws StructureBuildingException
    virtual void anthrazine() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void anthyridine() throws StructureBuildingException
    virtual void anthyridine() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void benzo_cd_azulene() throws StructureBuildingException
    virtual void benzo_cd_azulene() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void indeno_7_1_cd_azepine() throws StructureBuildingException
    virtual void indeno_7_1_cd_azepine() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test @Ignore public void tripleSubstituedSevenMembered() throws StructureBuildingException
    virtual void tripleSubstituedSevenMembered() throw(StructureBuildingException);

    /// <summary>
    /// Takes smiles and expected labels for a fused ring. Generates the fused ring, numbers it then compares to the given slash delimited labels </summary>
    /// <param name="smiles"> </param>
    /// <param name="labels"> </param>
    /// <exception cref="StructureBuildingException">  </exception>
private:
    void compareNumbering(const std::wstring &smiles, const std::wstring &labels) throw(StructureBuildingException);
};

