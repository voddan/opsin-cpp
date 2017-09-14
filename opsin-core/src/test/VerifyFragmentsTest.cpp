#include "VerifyFragmentsTest.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/ResourceGetter.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/SMILESFragmentBuilder.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/IDManager.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/XmlDeclarations.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/Fragment.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/TokenEl.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/StructureBuildingException.h"


using org::junit::AfterClass;
using org::junit::BeforeClass;
using org::junit::Test;
//						import static org.junit.Assert.*;
using namespace uk::ac::cam::ch::wwmm::opsin;
//						import static uk.ac.cam.ch.wwmm.opsin.XmlDeclarations.*;
ResourceGetter *VerifyFragmentsTest::resourceGetter;
SMILESFragmentBuilder *VerifyFragmentsTest::sBuilder;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @BeforeClass public static void setUp()
void VerifyFragmentsTest::setUp() {
    resourceGetter = new ResourceGetter(L"uk/ac/cam/ch/wwmm/opsin/resources/");
    IDManager tempVar();
    sBuilder = new SMILESFragmentBuilder(&tempVar);
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @AfterClass public static void cleanUp()
void VerifyFragmentsTest::cleanUp() {
    resourceGetter = nullptr;
    sBuilder = nullptr;
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void verifySMILES() throws Exception
void VerifyFragmentsTest::verifySMILES() throw(std::exception) {

    XMLStreamReader *indexReader = resourceGetter->getXMLStreamReader(L"index.xml");
    while (indexReader->hasNext()) {
        if (indexReader->next() == XMLStreamConstants::START_ELEMENT &&
            indexReader->getLocalName().equals(L"tokenFile")) {
            XMLStreamReader *tokenReader = resourceGetter->getXMLStreamReader(indexReader->getElementText());
            while (tokenReader->hasNext()) {
                if (tokenReader->next() == XMLStreamConstants::START_ELEMENT) {
                    std::wstring tagName = tokenReader->getLocalName();
                    if (tagName == L"tokenLists") {
                        while (tokenReader->hasNext()) {
                            switch (tokenReader->next()) {
                                case XMLStreamConstants::START_ELEMENT:
                                    if (tokenReader->getLocalName().equals(L"tokenList")) {
                                        verifySmilesInTokenList(tokenReader);
                                    }
                                    break;
                            }
                            tokenReader++;
                        }
                    } else if (tagName == L"tokenList") {
                        verifySmilesInTokenList(tokenReader);
                    }
                }
                tokenReader++;
            }
        }
        indexReader++;
    }
    delete indexReader;
}

void VerifyFragmentsTest::verifySmilesInTokenList(XMLStreamReader *reader) throw(XMLStreamException) {
    std::wstring tagname = reader->getAttributeValue(nullptr, L"tagname");
    if (tagname == XmlDeclarations::GROUP_EL || tagname == XmlDeclarations::FUNCTIONALGROUP_EL ||
        tagname == XmlDeclarations::HETEROATOM_EL || tagname == XmlDeclarations::SUFFIXPREFIX_EL) {
        std::wstring type = reader->getAttributeValue(nullptr, XmlDeclarations::TYPE_ATR);
        std::wstring subType = reader->getAttributeValue(nullptr, XmlDeclarations::SUBTYPE_ATR);
        while (reader->hasNext()) {
            switch (reader->next()) {
                case XMLStreamConstants::START_ELEMENT:
                    if (reader->getLocalName().equals(L"token")) {
                        Fragment *mol = nullptr;
                        std::wstring smiles = L"";
                        try {
                            smiles = reader->getAttributeValue(nullptr, XmlDeclarations::VALUE_ATR);
                            std::wstring labels = reader->getAttributeValue(nullptr, XmlDeclarations::LABELS_ATR);
                            TokenEl *tokenEl = new TokenEl(XmlDeclarations::GROUP_EL);
                            if (type != L"") {
                                tokenEl->addAttribute(XmlDeclarations::TYPE_ATR, type);
                            }
                            if (subType != L"") {
                                tokenEl->addAttribute(XmlDeclarations::SUBTYPE_ATR, subType);
                            }

                            mol = sBuilder->build(smiles, tokenEl, labels != L"" ? labels : L"");
                        }
                        catch (const std::exception &e) {
                            e.printStackTrace();
                        }
                        assertNotNull(std::wstring(L"The following token's SMILES or labels were in error: ") + smiles,
                                      mol);
                        try {
                            mol->checkValencies();
                        }
                        catch (const StructureBuildingException &e) {
                            fail(std::wstring(
                                    L"The following token's SMILES produced a structure with invalid valency: ") +
                                 smiles);
                        }
                    }
                    break;
                case XMLStreamConstants::END_ELEMENT:
                    if (reader->getLocalName().equals(L"tokenList")) {
                        return;
                    }
                    break;
            }
            reader++;
        }
    }
}
