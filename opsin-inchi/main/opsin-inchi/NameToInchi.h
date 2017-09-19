#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <stdexcept>
#include <boost/optional.hpp>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
class NameToStructure;
class OpsinResult;
class Fragment;


using org::apache::log4j::Logger;

using net::sf::jniinchi::JniInchiException;

/// <summary>
/// Allows the conversion of OPSIN's output into (Std)InChIs or StdInChIKeys
/// Also can be used, as a convenience method, to directly convert chemical names to (Std)InChIs or StdInChIKeys
/// @author dl387
///
/// </summary>
class NameToInchi {

private:
    static Logger *const LOG = Logger::getLogger(NameToInchi::
    typeid);
    NameToStructure *n2s;
public:
    virtual ~NameToInchi() {
        delete n2s;
    }

    NameToInchi();

    /// <summary>
    ///Parses a chemical name, returning an InChI representation of the molecule.
    /// </summary>
    /// <param name="name"> The chemical name to parse. </param>
    /// <returns> An InChI string, containing the parsed molecule, or null if the molecule would not parse. </returns>
    virtual std::wstring parseToInchi(const std::wstring &name);

    /// <summary>
    ///Parses a chemical name, returning a StdInChI representation of the molecule.
    /// Note that chemical names typically specify an exact tautomer which is not representable in StdInChI
    /// Use <seealso cref="#parseToInchi(String)"/> if you want to represent the exact tautomer using a fixed hydrogen layer
    /// </summary>
    /// <param name="name"> The chemical name to parse. </param>
    /// <returns> A StdInChI string, containing the parsed molecule, or null if the molecule would not parse. </returns>
    virtual std::wstring parseToStdInchi(const std::wstring &name);

    /// <summary>
    ///Parses a chemical name, returning a StdInChIKey for the molecule.
    /// Like StdInChI, StdInChIKeys aim to not be tautomer specific
    /// </summary>
    /// <param name="name"> The chemical name to parse. </param>
    /// <returns> A StdInChIKey string or null if the molecule would not parse. </returns>
    virtual std::wstring parseToStdInchiKey(const std::wstring &name);

    /// <summary>
    /// Converts an OPSIN result to InChI. Null is returned if this conversion fails </summary>
    /// <param name="result"> </param>
    /// <returns> String InChI </returns>
    static std::wstring convertResultToInChI(OpsinResult *result);

    /// <summary>
    /// Converts an OPSIN result to StdInChI. Null is returned if this conversion fails
    /// Note that chemical names typically specify an exact tautomer which is not representable in StdInChI
    /// Use <seealso cref="#convertResultToInChI(OpsinResult)"/> if you want to represent the exact tautomer using a fixed hydrogen layer </summary>
    /// <param name="result"> </param>
    /// <returns> String InChI </returns>
    static std::wstring convertResultToStdInChI(OpsinResult *result);

    /// <summary>
    /// Converts an OPSIN result to a StdInChIKey. Null is returned if this conversion fails
    /// Like StdInChI, StdInChIKeys aim to not be tautomer specific </summary>
    /// <param name="result"> </param>
    /// <returns> String InChIKey </returns>
    static std::wstring convertResultToStdInChIKey(OpsinResult *result);

private:
    static std::wstring convertResultToInChI(OpsinResult *result, bool produceStdInChI);

    static std::wstring opsinFragmentToInchi(Fragment *frag, bool produceStdInChI) throw(JniInchiException);
};

