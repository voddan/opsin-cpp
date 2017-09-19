#include "FusedRingNumbererTest.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/SMILESFragmentBuilder.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/StructureBuildingException.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/Element.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/Fragment.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/XmlDeclarations.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/FusedRingNumberer.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/Atom.h"


//						import static org.junit.Assert.*;
//						import static org.mockito.Mockito.mock;
using org::junit::Ignore;
using org::junit::Test;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void aceanthrene() throws StructureBuildingException
void FusedRingNumbererTest::aceanthrene() throw(StructureBuildingException) {
    compareNumbering(L"C1Cc2cccc3cc4ccccc4c1c23", L"1/2/2a/3/4/5/5a/6/6a/7/8/9/10/10a/10b/10c");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void acenaphthene() throws StructureBuildingException
void FusedRingNumbererTest::acenaphthene() throw(StructureBuildingException) {
    compareNumbering(L"C1Cc2cccc3cccc1c23", L"1/2/2a/3/4/5/5a/6/7/8/8a/8b");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void acephenanthrene() throws StructureBuildingException
void FusedRingNumbererTest::acephenanthrene() throw(StructureBuildingException) {
    compareNumbering(L"c1ccc2CCc3cc4ccccc4c1c23", L"1/2/3/3a/4/5/5a/6/6a/7/8/9/10/10a/10b/10c");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void arsanthrene() throws StructureBuildingException
void FusedRingNumbererTest::arsanthrene() throw(StructureBuildingException) {
    compareNumbering(L"C1=CC=CC=2[As]=C3C=CC=CC3=[As]C12", L"1/2/3/4/4a/5/5a/6/7/8/9/9a/10/10a");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void arsanthridine() throws StructureBuildingException
void FusedRingNumbererTest::arsanthridine() throw(StructureBuildingException) {
    compareNumbering(L"c1cccc2[as]cc3ccccc3c12", L"1/2/3/4/4a/5/6/6a/7/8/9/10/10a/10b");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void arsindole() throws StructureBuildingException
void FusedRingNumbererTest::arsindole() throw(StructureBuildingException) {
    compareNumbering(L"[as]1ccc2ccccc12", L"1/2/3/3a/4/5/6/7/7a");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void arsindoline() throws StructureBuildingException
void FusedRingNumbererTest::arsindoline() throw(StructureBuildingException) {
    compareNumbering(L"[as]1cccc2ccccc12", L"1/2/3/4/4a/5/6/7/8/8a");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void betacarboline() throws StructureBuildingException
void FusedRingNumbererTest::betacarboline() throw(StructureBuildingException) {
    compareNumbering(L"c1nccc2c3ccccc3nc12", L"1/2/3/4/4a/4b/5/6/7/8/8a/9/9a");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void boranthrene() throws StructureBuildingException
void FusedRingNumbererTest::boranthrene() throw(StructureBuildingException) {
    compareNumbering(L"C1=CC=CC=2[B]=C3C=CC=CC3=[B]C12", L"1/2/3/4/4a/5/5a/6/7/8/9/9a/10/10a");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void cholanthrene() throws StructureBuildingException
void FusedRingNumbererTest::cholanthrene() throw(StructureBuildingException) {
    compareNumbering(L"C1Cc2cccc3cc4c5ccccc5ccc4c1c23", L"1/2/2a/3/4/5/5a/6/6a/6b/7/8/9/10/10a/11/12/12a/12b/12c");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void thiochromane() throws StructureBuildingException
void FusedRingNumbererTest::thiochromane() throw(StructureBuildingException) {
    compareNumbering(L"S1CCCc2ccccc12", L"1/2/3/4/4a/5/6/7/8/8a");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void selenochromane() throws StructureBuildingException
void FusedRingNumbererTest::selenochromane() throw(StructureBuildingException) {
    compareNumbering(L"[Se]1CCCc2ccccc12", L"1/2/3/4/4a/5/6/7/8/8a");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void tellurochromane() throws StructureBuildingException
void FusedRingNumbererTest::tellurochromane() throw(StructureBuildingException) {
    compareNumbering(L"[Te]1CCCc2ccccc12", L"1/2/3/4/4a/5/6/7/8/8a");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void thiochromene() throws StructureBuildingException
void FusedRingNumbererTest::thiochromene() throw(StructureBuildingException) {
    compareNumbering(L"s1cccc2ccccc12", L"1/2/3/4/4a/5/6/7/8/8a");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void selenochromene() throws StructureBuildingException
void FusedRingNumbererTest::selenochromene() throw(StructureBuildingException) {
    compareNumbering(L"[se]1cccc2ccccc12", L"1/2/3/4/4a/5/6/7/8/8a");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void tellurochromene() throws StructureBuildingException
void FusedRingNumbererTest::tellurochromene() throw(StructureBuildingException) {
    compareNumbering(L"[Te]1CC=Cc2ccccc12", L"1/2/3/4/4a/5/6/7/8/8a");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void coronene() throws StructureBuildingException
void FusedRingNumbererTest::coronene() throw(StructureBuildingException) {
    compareNumbering(L"c1cc2ccc3ccc4ccc5ccc6ccc71.c28c3c4c5c6c78",
                     L"1/2/2a/3/4/4a/5/6/6a/7/8/8a/9/10/10a/11/12/12a/12b/12c/12d/12e/12f/12g");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void indane() throws StructureBuildingException
void FusedRingNumbererTest::indane() throw(StructureBuildingException) {
    compareNumbering(L"C1CCc2ccccc12", L"1/2/3/3a/4/5/6/7/7a");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void isoarsindole() throws StructureBuildingException
void FusedRingNumbererTest::isoarsindole() throw(StructureBuildingException) {
    compareNumbering(L"c1[as]cc2ccccc12", L"1/2/3/3a/4/5/6/7/7a");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void isoarsinoline() throws StructureBuildingException
void FusedRingNumbererTest::isoarsinoline() throw(StructureBuildingException) {
    compareNumbering(L"c1[as]ccc2ccccc12", L"1/2/3/4/4a/5/6/7/8/8a");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void thioisochromane() throws StructureBuildingException
void FusedRingNumbererTest::thioisochromane() throw(StructureBuildingException) {
    compareNumbering(L"C1SCCc2ccccc12", L"1/2/3/4/4a/5/6/7/8/8a");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void selenoisochromane() throws StructureBuildingException
void FusedRingNumbererTest::selenoisochromane() throw(StructureBuildingException) {
    compareNumbering(L"C1[Se]CCc2ccccc12", L"1/2/3/4/4a/5/6/7/8/8a");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void telluroisochromane() throws StructureBuildingException
void FusedRingNumbererTest::telluroisochromane() throw(StructureBuildingException) {
    compareNumbering(L"C1[Te]CCc2ccccc12", L"1/2/3/4/4a/5/6/7/8/8a");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void isochromene() throws StructureBuildingException
void FusedRingNumbererTest::isochromene() throw(StructureBuildingException) {
    compareNumbering(L"c1occc2ccccc12", L"1/2/3/4/4a/5/6/7/8/8a");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void thioisochromene() throws StructureBuildingException
void FusedRingNumbererTest::thioisochromene() throw(StructureBuildingException) {
    compareNumbering(L"c1sccc2ccccc12", L"1/2/3/4/4a/5/6/7/8/8a");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void selenoisochromene() throws StructureBuildingException
void FusedRingNumbererTest::selenoisochromene() throw(StructureBuildingException) {
    compareNumbering(L"c1[se]ccc2ccccc12", L"1/2/3/4/4a/5/6/7/8/8a");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void telluroisochromene() throws StructureBuildingException
void FusedRingNumbererTest::telluroisochromene() throw(StructureBuildingException) {
    compareNumbering(L"C1[Te]C=Cc2ccccc12", L"1/2/3/4/4a/5/6/7/8/8a");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void isophosphindole() throws StructureBuildingException
void FusedRingNumbererTest::isophosphindole() throw(StructureBuildingException) {
    compareNumbering(L"c1pcc2ccccc12", L"1/2/3/3a/4/5/6/7/7a");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void isophosphinoline() throws StructureBuildingException
void FusedRingNumbererTest::isophosphinoline() throw(StructureBuildingException) {
    compareNumbering(L"c1pccc2ccccc12", L"1/2/3/4/4a/5/6/7/8/8a");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void isoviolanthrene() throws StructureBuildingException
void FusedRingNumbererTest::isoviolanthrene() throw(StructureBuildingException) {
    compareNumbering(L"c1cccc2c3ccc4c5ccc6Cc7ccccc7c8ccc9c%10ccc%11Cc12.c3%11c4%10.c59c68",
                     L"1/2/3/4/4a/4b/5/6/6a/6b/7/8/8a/9/9a/10/11/12/13/13a/13b/14/15/15a/15b/16/17/17a/18/18a/18b/18c/18d/18e");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void mercuranthrene() throws StructureBuildingException
void FusedRingNumbererTest::mercuranthrene() throw(StructureBuildingException) {
    compareNumbering(L"c1cccc2[Hg]c3ccccc3[Hg]c12", L"1/2/3/4/4a/5/5a/6/7/8/9/9a/10/10a");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test @Ignore public void ovalene() throws StructureBuildingException
void FusedRingNumbererTest::ovalene() throw(StructureBuildingException) { //Transient BUG in path finding code
    compareNumbering(L"c1cc2ccc3ccc4cc5ccc6ccc7ccc8cc91.c19c2c3c4c9c5c6c7c8c19",
                     L"1/2/2a/3/4/4a/5/6/6a/7/7a/8/9/9a/10/11/11a/12/13/13a/14/14a/14b/14c/14d/14e/14f/14g/14h/14i/14j/14k");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void oxanthrene() throws StructureBuildingException
void FusedRingNumbererTest::oxanthrene() throw(StructureBuildingException) {
    compareNumbering(L"c1cccc2Oc3ccccc3Oc12", L"1/2/3/4/4a/5/5a/6/7/8/9/9a/10/10a");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void perylene() throws StructureBuildingException
void FusedRingNumbererTest::perylene() throw(StructureBuildingException) {
    compareNumbering(L"c1ccc2cccc3c4cccc5cccc6c71.c237.c456",
                     L"1/2/3/3a/4/5/6/6a/6b/7/8/9/9a/10/11/12/12a/12b/12c/12d");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void phenanthridine() throws StructureBuildingException
void FusedRingNumbererTest::phenanthridine() throw(StructureBuildingException) {
    compareNumbering(L"c1cccc2ncc3ccccc3c12", L"1/2/3/4/4a/5/6/6a/7/8/9/10/10a/10b");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void phenomercurine() throws StructureBuildingException
void FusedRingNumbererTest::phenomercurine() throw(StructureBuildingException) {
    compareNumbering(L"c1cccc2[Hg]c3ccccc3[Hg]c12", L"1/2/3/4/4a/5/5a/6/7/8/9/9a/10/10a");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void phenoxazine() throws StructureBuildingException
void FusedRingNumbererTest::phenoxazine() throw(StructureBuildingException) {
    compareNumbering(L"c1cccc2Oc3ccccc3nc12", L"1/2/3/4/4a/5/5a/6/7/8/9/9a/10/10a");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void phenothiazine() throws StructureBuildingException
void FusedRingNumbererTest::phenothiazine() throw(StructureBuildingException) {
    compareNumbering(L"c1cccc2Sc3ccccc3nc12", L"1/2/3/4/4a/5/5a/6/7/8/9/9a/10/10a");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void phenoselenazine() throws StructureBuildingException
void FusedRingNumbererTest::phenoselenazine() throw(StructureBuildingException) {
    compareNumbering(L"c1cccc2[Se]c3ccccc3nc12", L"1/2/3/4/4a/5/5a/6/7/8/9/9a/10/10a");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void phenotellurazine() throws StructureBuildingException
void FusedRingNumbererTest::phenotellurazine() throw(StructureBuildingException) {
    compareNumbering(L"c1cccc2[Te]c3ccccc3nc12", L"1/2/3/4/4a/5/5a/6/7/8/9/9a/10/10a");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void phenophosphazinine() throws StructureBuildingException
void FusedRingNumbererTest::phenophosphazinine() throw(StructureBuildingException) {
    compareNumbering(L"c1cccc2nc3ccccc3pc12", L"1/2/3/4/4a/5/5a/6/7/8/9/9a/10/10a");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void phenophosphazine() throws StructureBuildingException
void FusedRingNumbererTest::phenophosphazine() throw(StructureBuildingException) {
    compareNumbering(L"c1cccc2nc3ccccc3pc12", L"1/2/3/4/4a/5/5a/6/7/8/9/9a/10/10a");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void phenarsazinine() throws StructureBuildingException
void FusedRingNumbererTest::phenarsazinine() throw(StructureBuildingException) {
    compareNumbering(L"c1cccc2nc3ccccc3[as]c12", L"1/2/3/4/4a/5/5a/6/7/8/9/9a/10/10a");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void phenoarsazine() throws StructureBuildingException
void FusedRingNumbererTest::phenoarsazine() throw(StructureBuildingException) {
    compareNumbering(L"c1cccc2nc3ccccc3[as]c12", L"1/2/3/4/4a/5/5a/6/7/8/9/9a/10/10a");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void phenomercurazine() throws StructureBuildingException
void FusedRingNumbererTest::phenomercurazine() throw(StructureBuildingException) {
    compareNumbering(L"c1cccc2nc3ccccc3[Hg]c12", L"1/2/3/4/4a/5/5a/6/7/8/9/9a/10/10a");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void phenomercazine() throws StructureBuildingException
void FusedRingNumbererTest::phenomercazine() throw(StructureBuildingException) {
    compareNumbering(L"c1cccc2nc3ccccc3[Hg]c12", L"1/2/3/4/4a/5/5a/6/7/8/9/9a/10/10a");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void phenoxathiine() throws StructureBuildingException
void FusedRingNumbererTest::phenoxathiine() throw(StructureBuildingException) {
    compareNumbering(L"c1cccc2Oc3ccccc3Sc12", L"1/2/3/4/4a/5/5a/6/7/8/9/9a/10/10a");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void phenoxaselenine() throws StructureBuildingException
void FusedRingNumbererTest::phenoxaselenine() throw(StructureBuildingException) {
    compareNumbering(L"c1cccc2Oc3ccccc3[Se]c12", L"1/2/3/4/4a/5/5a/6/7/8/9/9a/10/10a");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void phenoxatellurine() throws StructureBuildingException
void FusedRingNumbererTest::phenoxatellurine() throw(StructureBuildingException) {
    compareNumbering(L"c1cccc2Oc3ccccc3[Te]c12", L"1/2/3/4/4a/5/5a/6/7/8/9/9a/10/10a");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void phenoxaphosphinine() throws StructureBuildingException
void FusedRingNumbererTest::phenoxaphosphinine() throw(StructureBuildingException) {
    compareNumbering(L"c1cccc2Oc3ccccc3pc12", L"1/2/3/4/4a/5/5a/6/7/8/9/9a/10/10a");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void phenoxaphosphine() throws StructureBuildingException
void FusedRingNumbererTest::phenoxaphosphine() throw(StructureBuildingException) {
    compareNumbering(L"c1cccc2Oc3ccccc3pc12", L"1/2/3/4/4a/5/5a/6/7/8/9/9a/10/10a");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void phenoxarsinine() throws StructureBuildingException
void FusedRingNumbererTest::phenoxarsinine() throw(StructureBuildingException) {
    compareNumbering(L"c1cccc2Oc3ccccc3[as]c12", L"1/2/3/4/4a/5/5a/6/7/8/9/9a/10/10a");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void phenoxarsine() throws StructureBuildingException
void FusedRingNumbererTest::phenoxarsine() throw(StructureBuildingException) {
    compareNumbering(L"c1cccc2Oc3ccccc3[as]c12", L"1/2/3/4/4a/5/5a/6/7/8/9/9a/10/10a");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void phenoxastibinine() throws StructureBuildingException
void FusedRingNumbererTest::phenoxastibinine() throw(StructureBuildingException) {
    compareNumbering(L"c1cccc2Oc3ccccc3[sb]c12", L"1/2/3/4/4a/5/5a/6/7/8/9/9a/10/10a");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void phenoxantimonine() throws StructureBuildingException
void FusedRingNumbererTest::phenoxantimonine() throw(StructureBuildingException) {
    compareNumbering(L"c1cccc2Oc3ccccc3[sb]c12", L"1/2/3/4/4a/5/5a/6/7/8/9/9a/10/10a");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void phenothiarsinine() throws StructureBuildingException
void FusedRingNumbererTest::phenothiarsinine() throw(StructureBuildingException) {
    compareNumbering(L"c1cccc2Sc3ccccc3[as]c12", L"1/2/3/4/4a/5/5a/6/7/8/9/9a/10/10a");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void phenothiarsine() throws StructureBuildingException
void FusedRingNumbererTest::phenothiarsine() throw(StructureBuildingException) {
    compareNumbering(L"c1cccc2Sc3ccccc3[as]c12", L"1/2/3/4/4a/5/5a/6/7/8/9/9a/10/10a");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void phosphanthrene() throws StructureBuildingException
void FusedRingNumbererTest::phosphanthrene() throw(StructureBuildingException) {
    compareNumbering(L"C1=CC=CC=2P=C3C=CC=CC3=PC12", L"1/2/3/4/4a/5/5a/6/7/8/9/9a/10/10a");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void phosphindole() throws StructureBuildingException
void FusedRingNumbererTest::phosphindole() throw(StructureBuildingException) {
    compareNumbering(L"p1ccc2ccccc12", L"1/2/3/3a/4/5/6/7/7a");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void phosphinoline() throws StructureBuildingException
void FusedRingNumbererTest::phosphinoline() throw(StructureBuildingException) {
    compareNumbering(L"p1cccc2ccccc12", L"1/2/3/4/4a/5/6/7/8/8a");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void picene() throws StructureBuildingException
void FusedRingNumbererTest::picene() throw(StructureBuildingException) {
    compareNumbering(L"c1cccc2ccc3c4ccc5ccccc5c4ccc3c21",
                     L"1/2/3/4/4a/5/6/6a/6b/7/8/8a/9/10/11/12/12a/12b/13/14/14a/14b");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void pleiadene() throws StructureBuildingException
void FusedRingNumbererTest::pleiadene() throw(StructureBuildingException) {
    compareNumbering(L"c1ccc2cccc3cc4ccccc4cc51.c235", L"1/2/3/3a/4/5/6/6a/7/7a/8/9/10/11/11a/12/12a/12b");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void pyranthrene() throws StructureBuildingException
void FusedRingNumbererTest::pyranthrene() throw(StructureBuildingException) {
    compareNumbering(L"c1cccc2c3cc4ccc5cc6ccccc6c6cc7ccc8cc12.c38c7c4c56",
                     L"1/2/3/4/4a/4b/5/5a/6/7/7a/8/8a/9/10/11/12/12a/12b/13/13a/14/15/15a/16/16a////");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void pyrrolizine() throws StructureBuildingException
void FusedRingNumbererTest::pyrrolizine() throw(StructureBuildingException) {
    compareNumbering(L"c1ccn2cccc12", L"1/2/3/4/5/6/7/7a");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void quinolizine() throws StructureBuildingException
void FusedRingNumbererTest::quinolizine() throw(StructureBuildingException) {
    compareNumbering(L"c1cccn2ccccc12", L"1/2/3/4/5/6/7/8/9/9a");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void rubicene() throws StructureBuildingException
void FusedRingNumbererTest::rubicene() throw(StructureBuildingException) {
    compareNumbering(L"c1ccc2c3ccccc3c3c4cccc5c6ccccc6c6c71.c237.c456",
                     L"1/2/3/3a/3b/4/5/6/7/7a/7b/7c/8/9/10/10a/10b/11/12/13/14/14a/14b/14c/14d/14e");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void silanthrene() throws StructureBuildingException
void FusedRingNumbererTest::silanthrene() throw(StructureBuildingException) {
    compareNumbering(L"C1=CC=CC=2[Si]=C3C=CC=CC3=[Si]C12", L"1/2/3/4/4a/5/5a/6/7/8/9/9a/10/10a");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void selenanthrene() throws StructureBuildingException
void FusedRingNumbererTest::selenanthrene() throw(StructureBuildingException) {
    compareNumbering(L"c1cccc2[Se]c3ccccc3[Se]c12", L"1/2/3/4/4a/5/5a/6/7/8/9/9a/10/10a");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void telluranthrene() throws StructureBuildingException
void FusedRingNumbererTest::telluranthrene() throw(StructureBuildingException) {
    compareNumbering(L"c1cccc2[Te]c3ccccc3[Te]c12", L"1/2/3/4/4a/5/5a/6/7/8/9/9a/10/10a");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void thianthrene() throws StructureBuildingException
void FusedRingNumbererTest::thianthrene() throw(StructureBuildingException) {
    compareNumbering(L"c1cccc2Sc3ccccc3Sc12", L"1/2/3/4/4a/5/5a/6/7/8/9/9a/10/10a");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void trindene() throws StructureBuildingException
void FusedRingNumbererTest::trindene() throw(StructureBuildingException) {
    compareNumbering(L"c1ccc2c3cccc3c4cccc4c12", L"1/2/3/3a/3b/4/5/6/6a/6b/7/8/9/9a/9b");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void violanthrene() throws StructureBuildingException
void FusedRingNumbererTest::violanthrene() throw(StructureBuildingException) {
    compareNumbering(L"c1cccc2Cc3ccc4c5ccc6Cc7ccccc7c8ccc9c%10ccc%11c12.c3%11c4%10.c59c68",
                     L"1/2/3/4/4a/5/5a/6/7/7a/7b/8/9/9a/10/10a/11/12/13/14/14a/14b/15/16/16a/16b/17/18/18a/18b////");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void naphthotetraphene() throws StructureBuildingException
void FusedRingNumbererTest::naphthotetraphene() throw(StructureBuildingException) {
    compareNumbering(L"c1cc2ccc3ccc4cc5ccccc5cc4c3c2c6ccccc16",
                     L"1/2/2a/3/4/4a/5/6/6a/7/7a/8/9/10/11/11a/12/12a/12b/12c/12d/13/14/15/16/16a");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void anthratetraphene() throws StructureBuildingException
void FusedRingNumbererTest::anthratetraphene() throw(StructureBuildingException) {
    compareNumbering(L"c1cc2ccc3ccc4cc5ccccc5cc4c3c2c6cc7ccccc7cc16",
                     L"1/2/2a/3/4/4a/5/6/6a/7/7a/8/9/10/11/11a/12/12a/12b/12c/12d/13/13a/14/15/16/17/17a/18/18a");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void octalenotetraphene() throws StructureBuildingException
void FusedRingNumbererTest::octalenotetraphene() throw(StructureBuildingException) {
    compareNumbering(L"c1ccc2ccc3ccc4cc5ccccc5cc4c3c2cc6ccccccc16",
                     L"1/2/3/3a/4/5/5a/6/7/7a/8/8a/9/10/11/12/12a/13/13a/13b/13c/14/14a/15/16/17/18/19/20/20a");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void difficultChain() throws StructureBuildingException
void FusedRingNumbererTest::difficultChain() throw(StructureBuildingException) {
    compareNumbering(L"C1C2C3C4CC5CC6CCCCCCC6CCC5CC4CC3C12",
                     L"1/1a/1b/1c/2/2a/3/3a/4/5/6/7/8/9/9a/10/11/11a/12/12a/13/13a/13b");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void difficultChain2() throws StructureBuildingException
void FusedRingNumbererTest::difficultChain2() throw(StructureBuildingException) {
    compareNumbering(L"C1C2CCCCC2C3C4C5C6CCC7C8CCCCC8C7CCC56C4CCC13",
                     L"1/1a/2/3/4/5/5a/5b/5c/5d/5e/6/7/7a/7b/8/9/10/11/11a/11b/12/13/13a/13b/14/15/15a");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void acrindoline() throws StructureBuildingException
void FusedRingNumbererTest::acrindoline() throw(StructureBuildingException) {
    compareNumbering(L"c1cccc2c3ccc4cc5ccccc5nc4c3nc12", L"1/2/3/4/4a/4b/5/6/6a/7/7a/8/9/10/11/11a/12/12a/12b/13/13a");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void anthrazine() throws StructureBuildingException
void FusedRingNumbererTest::anthrazine() throw(StructureBuildingException) {
    compareNumbering(L"c1cccc2cc3c4nc5ccc6cc7ccccc7cc6c5nc4ccc3cc12",
                     L"1/2/3/4/4a/5/5a/5b/6/6a/7/8/8a/9/9a/10/11/12/13/13a/14/14a/14b/15/15a/16/17/17a/18/18a");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void anthyridine() throws StructureBuildingException
void FusedRingNumbererTest::anthyridine() throw(StructureBuildingException) {
    compareNumbering(L"n1cccc2cc3cccnc3nc12", L"1/2/3/4/4a/5/5a/6/7/8/9/9a/10/10a");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void benzo_cd_azulene() throws StructureBuildingException
void FusedRingNumbererTest::benzo_cd_azulene() throw(StructureBuildingException) {
    compareNumbering(L"c1cc2cccc3ccccc1c23", L"1/2/2a/3/4/5/5a/6/7/8/9/9a/9b");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void indeno_7_1_cd_azepine() throws StructureBuildingException
void FusedRingNumbererTest::indeno_7_1_cd_azepine() throw(StructureBuildingException) {
    compareNumbering(L"c1nccc2ccc3cccc1c23", L"1/2/3/4/4a/5/6/6a/7/8/9/9a/9b");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test @Ignore public void tripleSubstituedSevenMembered() throws StructureBuildingException
void FusedRingNumbererTest::tripleSubstituedSevenMembered() throw(StructureBuildingException) {
    compareNumbering(L"C1NCCN2c3ncccc3Cc4ccccc4C12", L"1/2/3/4/5/5a/6/7/8/9/9a/10/10a/11/12/13/14/14a/14b");
    compareNumbering(L"c1cccc2C3CNCCN3c4ncccc4Cc12", L"1/2/3/4/5/5a/6/7/8/9/9a/10/10a/11/12/13/14/14a/14b");
}

void FusedRingNumbererTest::compareNumbering(const std::wstring &smiles,
                                             const std::wstring &labels) throw(StructureBuildingException) {
    Fragment * fusedRing = sBuilder->build(smiles, mock(Element::
    typeid), XmlDeclarations::NONE_LABELS_VAL);
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
    std::vector<std::wstring> labelArray = labels.split(L"/", -1);
    FusedRingNumberer::numberFusedRing(fusedRing);
    std::vector<Atom *> atomList = fusedRing->getAtomList();
    assertEquals(atomList.size(), labelArray.size()); //bug in test if not true!
    for (int i = 0; i < atomList.size(); i++) {
        if (labelArray[i] != L"") { //exterior atom locant
            assertEquals(labelArray[i], atomList[i]->getFirstLocant());
        }
    }
}
