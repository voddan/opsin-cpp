#include "NameToStructure.h"
#include "ParseRules.h"
#include "Parser.h"
#include "SuffixRules.h"
#include "ResourceGetter.h"
#include "ResourceManager.h"
#include "WordRules.h"
#include "Tokeniser.h"
#include "NameToStructureException.h"
#include "OpsinResult.h"
#include "NameToStructureConfig.h"
#include "Element.h"
#include "PreProcessor.h"
#include "SortParses.h"
#include "Fragment.h"
#include "OpsinWarning.h"
#include "ComponentGenerator.h"
#include "BuildState.h"
#include "ComponentProcessor.h"
#include "SuffixApplier.h"
#include "StructureBuilder.h"
#include "OpsinTools.h"
#include "IndentingXMLStreamWriter.h"
#include "CMLWriter.h"


using org::apache::commons::cli::CommandLine;
using org::apache::commons::cli::CommandLineParser;
using org::apache::commons::cli::DefaultParser;
using org::apache::commons::cli::HelpFormatter;
using org::apache::commons::cli::Option;
using org::apache::commons::cli::Option::Builder;
using org::apache::commons::io::IOUtils;
using org::apache::commons::cli::Options;
using org::apache::commons::cli::UnrecognizedOptionException;
using org::apache::log4j::Level;
using org::apache::log4j::Logger;
using com::ctc::wstx::api::WstxOutputProperties;
using com::ctc::wstx::stax::WstxOutputFactory;
using OPSIN_RESULT_STATUS = OpsinResult::OPSIN_RESULT_STATUS;
NameToStructure *NameToStructure::NTS_INSTANCE;

//JAVA TO C++ CONVERTER WARNING: The following method was originally marked 'synchronized':
NameToStructure *NameToStructure::getInstance() {
    if (NTS_INSTANCE == nullptr) {
        NTS_INSTANCE = new NameToStructure();
    }
    return NTS_INSTANCE;
}

std::wstring NameToStructure::getVersion() {
    try {
        InputStream * is = NameToStructure::
        typeid->getResourceAsStream(L"opsinbuild.props");
        try {
            Properties *props = new Properties();
            props->load(is);
            return props->getProperty(L"version");
        }
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to the exception 'finally' clause:
        finally
        {
            IOUtils::closeQuietly(is);
        }
    }
    catch (const std::exception &e) {
        return L"";
    }
}

NameToStructure::NameToStructure() {
    LOG->debug(L"Initialising OPSIN... ");
    try {
        /*Initialise all of OPSIN's classes. Some classes are injected as dependencies into subsequent classes*/

        //Allows retrieving of OPSIN resources
        ResourceGetter *resourceGetter = new ResourceGetter(L"uk/ac/cam/ch/wwmm/opsin/resources/");
        ResourceManager *resourceManager = new ResourceManager(resourceGetter);
        WordRules *wordRules = new WordRules(resourceGetter);
        parseRules = new ParseRules(resourceManager);
        Tokeniser *tokeniser = new Tokeniser(parseRules);
        parser = new Parser(wordRules, tokeniser, resourceManager);
        suffixRules = new SuffixRules(resourceGetter);
    }
    catch (const std::exception &e) {
        throw NameToStructureException(e.what(), e);
    }
    LOG->debug(L"OPSIN initialised");
}

std::wstring NameToStructure::parseToCML(const std::wstring &name) {
    OpsinResult *result = parseChemicalName(name);
    std::wstring cml = result->getCml();
    if (cml != L"" && LOG->isDebugEnabled()) {
        LOG->debug(cml);
    }
    return cml;
}

std::wstring NameToStructure::parseToSmiles(const std::wstring &name) {
    OpsinResult *result = parseChemicalName(name);
    std::wstring smiles = result->getSmiles();
    LOG->debug(smiles);
    return smiles;
}

OpsinResult *NameToStructure::parseChemicalName(const std::wstring &name) {
    NameToStructureConfig *n2sConfig = NameToStructureConfig::getDefaultConfigInstance();
    return parseChemicalName(name, n2sConfig);
}

OpsinResult *NameToStructure::parseChemicalName(const std::wstring &name, NameToStructureConfig *n2sConfig) {
    if (name == L"") {
        throw std::invalid_argument("String given for name was null");
    }
    n2sConfig = n2sConfig->clone(); //avoid n2sconfig being modified mid name processing

    std::vector<Element *> parses;
    try {
        LOG->debug(name);
        std::wstring modifiedName = PreProcessor::preProcess(name);
        parses = parser->parse(n2sConfig, modifiedName);
//JAVA TO C++ CONVERTER TODO TASK: The 'Compare' parameter of std::sort produces a boolean value, while the Java Comparator parameter produces a tri-state result:
//ORIGINAL LINE: java.util.Collections.sort(parses, new SortParses());
        SortParses tempVar{};
        std::sort(parses.begin(), parses.end(), tempVar.cmp); //fewer tokens preferred
    }
    catch (const std::exception &e) {
        if (LOG->isDebugEnabled()) {
            LOG->debug(e.what(), e);
        }
        std::wstring message = e.what() != nullptr ? e.what() : L"exception with null message";
        return new OpsinResult(nullptr, OPSIN_RESULT_STATUS::FAILURE, message, name);
    }
    std::wstring reasonForFailure = L"";
    Fragment *fragGeneratedWithWarning = nullptr;
    std::vector<OpsinWarning *> warnings = Collections::emptyList();
    for (auto parse : parses) {
        try {
            if (LOG->isDebugEnabled()) {
                LOG->debug(parse->toXML());
            }
            //Performs XML manipulation e.g. nesting bracketing, processing some nomenclatures
            ComponentGenerator tempVar2(n2sConfig);
            (&tempVar2)->processParse(parse);
            if (LOG->isDebugEnabled()) {
                LOG->debug(parse->toXML());
            }
            BuildState *state = new BuildState(n2sConfig);
            //Converts the XML to fragments (handles many different nomenclatueres for describing structure). Assigns locants
            ComponentProcessor tempVar3(state, new SuffixApplier(state, suffixRules));
            (&tempVar3)->processParse(parse);
            if (LOG->isDebugEnabled()) {
                LOG->debug(parse->toXML());
            }
            //Constructs a single fragment from the fragments generated by the ComponentProcessor. Applies stereochemistry
            StructureBuilder tempVar4(state);
            Fragment *frag = (&tempVar4)->buildFragment(parse);
            if (LOG->isDebugEnabled()) {
                LOG->debug(parse->toXML());
            }
            if (state->getWarnings().empty()) {
                return new OpsinResult(frag, OPSIN_RESULT_STATUS::SUCCESS, L"", name);
            }
            if (fragGeneratedWithWarning == nullptr) {
                //record first frag that had a warning but try other parses as they may work without a warning
                fragGeneratedWithWarning = frag;
                warnings = state->getWarnings();
            }
        }
        catch (const std::exception &e) {
            if (reasonForFailure.length() == 0) {
                reasonForFailure = e.what() != nullptr ? e.what() : L"exception with null message";
            }
            if (LOG->isDebugEnabled()) {
                LOG->debug(e.what(), e);
            }
        }
    }
    if (fragGeneratedWithWarning != nullptr) {
        return new OpsinResult(fragGeneratedWithWarning, OPSIN_RESULT_STATUS::WARNING, warnings, name);
    }
    return new OpsinResult(nullptr, OPSIN_RESULT_STATUS::FAILURE, reasonForFailure, name);
}

ParseRules *NameToStructure::getOpsinParser() {
    NameToStructure *n2s = NameToStructure::getInstance();
    return n2s->parseRules;
}

void NameToStructure::main(std::vector<std::wstring> &args) throw(std::exception) {
    Options *options = buildCommandLineOptions();
    CommandLineParser *parser = new DefaultParser();
    CommandLine *cmd = nullptr;
    try {
        cmd = parser->parse(options, args);
    }
    catch (const UnrecognizedOptionException &e) {
        System::err::println(e->getMessage());
        exit(1);
    }
    if (cmd->hasOption(L"h")) {
        displayUsage(options);
    }
    if (cmd->hasOption(L"v")) {
        Logger::getLogger(L"uk.ac.cam.ch.wwmm.opsin").setLevel(Level::DEBUG);
    }

    NameToStructureConfig *n2sconfig = generateOpsinConfigObjectFromCmd(cmd);

    InputStream *input;
    OutputStream *output;
    std::vector<std::wstring> unparsedArgs = cmd->getArgs();
    if (unparsedArgs.empty()) {
        input = System::in;
        output = System::out;
    } else if (unparsedArgs.size() == 1) {
        File tempVar(unparsedArgs[0]);
        input = new FileInputStream(&tempVar);
        output = System::out;
    } else if (unparsedArgs.size() == 2) {
        File tempVar2(unparsedArgs[0]);
        input = new FileInputStream(&tempVar2);
        File tempVar3(unparsedArgs[1]);
        output = new FileOutputStream(&tempVar3);
    } else {
        input = nullptr;
        output = nullptr;
        displayUsage(options);
    }

    System::err::println(L"Run the jar using the -h flag for help. Enter a chemical name to begin:");
    std::wstring outputType = cmd->getOptionValue(L"o", L"smi");
    if (outputType.equalsIgnoreCase(L"cml")) {
        interactiveCmlOutput(input, output, n2sconfig);
    } else if (outputType.equalsIgnoreCase(L"smi") || outputType.equalsIgnoreCase(L"smiles")) {
        interactiveSmilesOutput(input, output, n2sconfig, false);
    } else if (outputType.equalsIgnoreCase(L"inchi")) {
        interactiveInchiOutput(input, output, n2sconfig, InchiType::inchiWithFixedH);
    } else if (outputType.equalsIgnoreCase(L"stdinchi")) {
        interactiveInchiOutput(input, output, n2sconfig, InchiType::stdInchi);
    } else if (outputType.equalsIgnoreCase(L"stdinchikey")) {
        interactiveInchiOutput(input, output, n2sconfig, InchiType::stdInchiKey);
    } else if (outputType.equalsIgnoreCase(L"extendedsmi") || outputType.equalsIgnoreCase(L"extendedsmiles") ||
               outputType.equalsIgnoreCase(L"cxsmi") || outputType.equalsIgnoreCase(L"cxsmiles")) {
        interactiveSmilesOutput(input, output, n2sconfig, true);
    } else {
        System::err::println(std::wstring(L"Unrecognised output format: ") + outputType);
        System::err::println(
                L"Expected output types are \"cml\", \"smi\", \"inchi\", \"stdinchi\" and \"stdinchikey\"");
        exit(1);
    }
    if (unparsedArgs.size() == 1) {
        delete input;
    } else if (unparsedArgs.size() == 2) {
        delete input;
        delete output;
    }
}

void NameToStructure::displayUsage(Options *options) {
    HelpFormatter *formatter = new HelpFormatter();
    std::wstring version = getVersion();
    formatter->printHelp(std::wstring(L"java -jar opsin-") + (version != L"" ? version : L"[version]") +
                         std::wstring(L"-jar-with-dependencies.jar [options] [inputfile] [outputfile]") +
                         OpsinTools::NEWLINE + std::wstring(
            L"OPSIN converts systematic chemical names to CML, SMILES or InChI/StdInChI/StdInChIKey") +
                         OpsinTools::NEWLINE + std::wstring(
            L"Names should be new line delimited and may be read from stdin (default) or a file and output to stdout (default) or a file"),
                         options);
    exit(0);
}

Options *NameToStructure::buildCommandLineOptions() {
    Options *options = new Options();
    Option::Builder *outputBuilder = Option::builder(L"o");
    outputBuilder->longOpt(L"output");
    outputBuilder->hasArg();
    outputBuilder->argName(L"format");
    StringBuilder *outputOptionsDesc = new StringBuilder();
    outputOptionsDesc->append(L"Sets OPSIN's output format (default smi)")->append(OpsinTools::NEWLINE);
    outputOptionsDesc->append(L"Allowed values are:")->append(OpsinTools::NEWLINE);
    outputOptionsDesc->append(L"cml for Chemical Markup Language")->append(OpsinTools::NEWLINE);
    outputOptionsDesc->append(L"smi for SMILES")->append(OpsinTools::NEWLINE);
    outputOptionsDesc->append(L"extendedsmi for Extended SMILES")->append(OpsinTools::NEWLINE);
    outputOptionsDesc->append(L"inchi for InChI (with FixedH)")->append(OpsinTools::NEWLINE);
    outputOptionsDesc->append(L"stdinchi for StdInChI")->append(OpsinTools::NEWLINE);
    outputOptionsDesc->append(L"stdinchikey for StdInChIKey");
    outputBuilder->desc(outputOptionsDesc->toString());
    options->addOption(outputBuilder->build());
    options->addOption(L"h", L"help", false, L"Displays the allowed command line flags");
    options->addOption(L"v", L"verbose", false, L"Enables debugging");

    options->addOption(L"a", L"allowAcidsWithoutAcid", false,
                       L"Allows interpretation of acids without the word acid e.g. \"acetic\"");
    options->addOption(L"f", L"detailedFailureAnalysis", false,
                       L"Enables reverse parsing to more accurately determine why parsing failed");
    options->addOption(L"r", L"allowRadicals", false, L"Enables interpretation of radicals");
    options->addOption(L"s", L"allowUninterpretableStereo", false,
                       L"Allows stereochemistry uninterpretable by OPSIN to be ignored");
    options->addOption(L"w", L"wildcardRadicals", false, L"Radicals are output as wildcard atoms");
    return options;
}

NameToStructureConfig *NameToStructure::generateOpsinConfigObjectFromCmd(CommandLine *cmd) {
    NameToStructureConfig *n2sconfig = new NameToStructureConfig();
    n2sconfig->setInterpretAcidsWithoutTheWordAcid(cmd->hasOption(L"a"));
    n2sconfig->setDetailedFailureAnalysis(cmd->hasOption(L"f"));
    n2sconfig->setAllowRadicals(cmd->hasOption(L"r"));
    n2sconfig->setWarnRatherThanFailOnUninterpretableStereochemistry(cmd->hasOption(L"s"));
    n2sconfig->setOutputRadicalsAsWildCardAtoms(cmd->hasOption(L"w"));
    return n2sconfig;
}

void NameToStructure::interactiveCmlOutput(InputStream *input, OutputStream *out,
                                           NameToStructureConfig *n2sconfig) throw(IOException, XMLStreamException) {
    NameToStructure *nts = NameToStructure::getInstance();
    InputStreamReader tempVar(input, L"UTF-8");
    BufferedReader *inputReader = new BufferedReader(&tempVar);
    XMLOutputFactory *factory = new WstxOutputFactory();
    factory->setProperty(WstxOutputProperties::P_OUTPUT_ESCAPE_CR, false);
    XMLStreamWriter *writer = factory->createXMLStreamWriter(out, L"UTF-8");
    writer = new IndentingXMLStreamWriter(writer, 2);
    writer->writeStartDocument();
    CMLWriter *cmlWriter = new CMLWriter(writer);
    cmlWriter->writeCmlStart();
    int id = 1;
    std::wstring name;
    while ((name = inputReader->readLine()) != L"") {
        OpsinResult *result = nts->parseChemicalName(name, n2sconfig);
        Fragment *structure = result->getStructure();
        cmlWriter->writeMolecule(structure, name, id++);
        writer->flush();
        if (structure == nullptr) {
            System::err::println(result->getMessage());
        }
    }
    cmlWriter->writeCmlEnd();
    writer->writeEndDocument();
    writer->flush();
    delete writer;
}

void NameToStructure::interactiveSmilesOutput(InputStream *input, OutputStream *out, NameToStructureConfig *n2sconfig,
                                              bool extendedSmiles) throw(IOException) {
    NameToStructure *nts = NameToStructure::getInstance();
    InputStreamReader tempVar(input, L"UTF-8");
    BufferedReader *inputReader = new BufferedReader(&tempVar);
    OutputStreamWriter tempVar2(out, L"UTF-8");
    BufferedWriter *outputWriter = new BufferedWriter(&tempVar2);
    std::wstring name;
    while ((name = inputReader->readLine()) != L"") {
        OpsinResult *result = nts->parseChemicalName(name, n2sconfig);
        std::wstring output = extendedSmiles ? result->getExtendedSmiles() : result->getSmiles();
        if (output == L"") {
            System::err::println(result->getMessage());
        } else {
            outputWriter->write(output);
        }
        outputWriter->newLine();
        outputWriter->flush();
    }
}

void NameToStructure::interactiveInchiOutput(InputStream *input, OutputStream *out, NameToStructureConfig *n2sconfig,
                                             InchiType inchiType) throw(std::exception) {
    NameToStructure *nts = NameToStructure::getInstance();
    InputStreamReader tempVar(input, L"UTF-8");
    BufferedReader *inputReader = new BufferedReader(&tempVar);
    OutputStreamWriter tempVar2(out, L"UTF-8");
    BufferedWriter *outputWriter = new BufferedWriter(&tempVar2);
    Class *c;
    try {
        c = Class::forName(L"uk.ac.cam.ch.wwmm.opsin.NameToInchi");
    }
    catch (const ClassNotFoundException &e) {
        System::err::println(L"Could not initialise NameToInChI module. Is it on your classpath?");
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException(e);
        throw std::exception();
    }
    Method *m;
    switch (inchiType) {
        case inchiWithFixedH:
            m = c->getMethod(L"convertResultToInChI", std::vector<Class *>{OpsinResult::typeid});
            break;
        case stdInchi:
            m = c->getMethod(L"convertResultToStdInChI", std::vector<Class *>{OpsinResult::typeid});
            break;
        case stdInchiKey:
            m = c->getMethod(L"convertResultToStdInChIKey", std::vector<Class *>{OpsinResult::typeid});
            break;
        default :
            throw std::invalid_argument(std::wstring(L"Unexepected enum value: ") + inchiType);
    }

    std::wstring name;
    while ((name = inputReader->readLine()) != L"") {
        OpsinResult *result = nts->parseChemicalName(name, n2sconfig);
        std::wstring output = static_cast<std::wstring>(m->invoke(nullptr, result));
        if (output == L"") {
            System::err::println(result->getMessage());
        } else {
            outputWriter->write(output);
        }
        outputWriter->newLine();
        outputWriter->flush();
    }
}
