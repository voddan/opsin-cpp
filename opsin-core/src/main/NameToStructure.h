#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include "stringbuilder.h"

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
class ParseRules;
class Parser;
class SuffixRules;
class OpsinResult;
class NameToStructureConfig;


using org::apache::commons::cli::CommandLine;
using org::apache::commons::cli::Options;
using org::apache::log4j::Logger;


/// <summary>
/// The "master" class, to turn a name into a structure.
///
/// @author ptc24
/// @author dl387
/// </summary>
class NameToStructure {

private:
    static Logger *const LOG = Logger::getLogger(NameToStructure::
    typeid);

    /// <summary>
    ///Applies OPSIN's grammar to tokenise and assign meaning to tokens </summary>
    ParseRules *parseRules;

    /// <summary>
    ///Parses a chemical name into one (or more in the case of ambiguity) parse trees </summary>
    Parser *parser;

    /// <summary>
    ///Which suffixes apply to what and what their effects are </summary>
    SuffixRules *suffixRules;

    static NameToStructure *NTS_INSTANCE;

public:
    virtual ~NameToStructure() {
        delete parseRules;
        delete parser;
        delete suffixRules;
    }

//JAVA TO C++ CONVERTER WARNING: The following method was originally marked 'synchronized':
    static NameToStructure *getInstance();

    /// <summary>
    /// Returns the version of the OPSIN library </summary>
    /// <returns> Version number String </returns>
    static std::wstring getVersion();

    /// <summary>
    ///Initialises the name-to-structure converter.
    /// </summary>
    /// <exception cref="NameToStructureException"> If the converter cannot be initialised, most likely due to bad or missing data files. </exception>
private:
    NameToStructure();

    /// <summary>
    /// Convenience method for converting a name to CML with OPSIN's default options </summary>
    /// <param name="name"> The chemical name to parse. </param>
    /// <returns> A CML element, containing the parsed molecule, or null if the name was uninterpretable. </returns>
public:
    virtual std::wstring parseToCML(const std::wstring &name);

    /// <summary>
    /// Convenience method for converting a name to SMILES with OPSIN's default options </summary>
    /// <param name="name"> The chemical name to parse. </param>
    /// <returns> A SMILES string describing the parsed molecule, or null if the name was uninterpretable. </returns>
    virtual std::wstring parseToSmiles(const std::wstring &name);

    /// <summary>
    ///Parses a chemical name, returning an OpsinResult which represents the molecule.
    /// This object contains in the status whether the name was parsed successfully
    /// A message which may contain additional information if the status was warning/failure
    /// The OpsinResult has methods to generate a SMILES or CML representation
    /// For InChI, the OpsinResult should be given to the NameToInchi class
    /// </summary>
    /// <param name="name"> The chemical name to parse. </param>
    /// <returns> OpsinResult </returns>
    virtual OpsinResult *parseChemicalName(const std::wstring &name);

    /// <summary>
    ///Parses a chemical name, returning an OpsinResult which represents the molecule.
    /// This object contains in the status whether the name was parsed successfully
    /// A message which may contain additional information if the status was warning/failure
    /// CML and SMILES representations may be retrieved directly from the object
    /// InChI may be generate using NameToInchi
    /// </summary>
    /// <param name="name"> The chemical name to parse. </param>
    /// <param name="n2sConfig"> Options to control how OPSIN interprets the name. </param>
    /// <returns> OpsinResult </returns>
    virtual OpsinResult *parseChemicalName(const std::wstring &name, NameToStructureConfig *n2sConfig);

    /// <summary>
    /// Returns an OPSIN parser
    /// This can be used to determine whether a word can be interpreted as being part of a chemical name.
    /// Just because a word can be split into tokens does not mean the word constitutes a valid chemical name
    /// e.g. ester is interpretable but is not in itself a chemical name </summary>
    /// <returns> Opsin parser for recognition/parsing of a chemical word </returns>
    static ParseRules *getOpsinParser();

private:
    enum class InchiType {
        inchiWithFixedH,
        stdInchi,
        stdInchiKey
    };

    /// <summary>
    ///Run OPSIN as a command-line application.
    /// </summary>
    /// <param name="args"> </param>
    /// <exception cref="Exception"> </exception>
    static void main(std::vector<std::wstring> &args) throw(std::exception);

private:
    static void displayUsage(Options *options);

    static Options *buildCommandLineOptions();

    /// <summary>
    /// Uses the command line parameters to configure a new NameToStructureConfig </summary>
    /// <param name="cmd"> </param>
    /// <returns> The configured NameToStructureConfig </returns>
    static NameToStructureConfig *generateOpsinConfigObjectFromCmd(CommandLine *cmd);

    static void interactiveCmlOutput(InputStream *input, OutputStream *out,
                                     NameToStructureConfig *n2sconfig) throw(IOException, XMLStreamException);

    static void interactiveSmilesOutput(InputStream *input, OutputStream *out, NameToStructureConfig *n2sconfig,
                                        bool extendedSmiles) throw(IOException);

    static void interactiveInchiOutput(InputStream *input, OutputStream *out, NameToStructureConfig *n2sconfig,
                                       InchiType inchiType) throw(std::exception);
};

