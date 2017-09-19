#pragma once

#include <string>
#include <stdexcept>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/predicate.hpp>


/// <summary>
/// Handles I/O:
/// Gets resource files from packages which is useful for including data from the JAR file.
/// Provides OutputStreams for the serialisation of automata.
///
/// @author ptc24
/// @author dl387
///
/// </summary>
class ResourceGetter {

private:
    static XMLInputFactory *const xmlInputFactory;

    const std::wstring resourcePath;
    const std::wstring workingDirectory;

private:
    class StaticConstructor {
    public:
        StaticConstructor();
    };

private:
    static ResourceGetter::StaticConstructor staticConstructor;


    /// <summary>
    /// Sets up a resourceGetter to get resources from a particular path.
    ///  /-separated - e.g. uk.ac.ch.cam.wwmm.opsin.resources should be
    ///  /uk/ac/cam/ch/wwmm/opsin/resources/
    /// </summary>
    /// <param name="resourcePath"> The /-separated resource path. </param>
public:
    ResourceGetter(const std::wstring &resourcePath);

    /// <summary>
    /// Gets the resourcePath used to initialise this ResourceGetter
    /// @return
    /// </summary>
    virtual std::wstring getResourcePath();

    /// <summary>
    ///Fetches a data file from resourcePath,
    /// and returns an XML stream reader for it
    /// </summary>
    /// <param name="name"> The name of the file to parse. </param>
    /// <returns> An XMLStreamReader </returns>
    /// <exception cref="IOException">  </exception>
    virtual XMLStreamReader *getXMLStreamReader(const std::wstring &name) throw(IOException);

private:
    File *getFile(const std::wstring &name);

    File *getResDir();

    /// <summary>
    ///Fetches a data file from resourcePath, and returns the entire contents
    /// as a string.
    /// </summary>
    /// <param name="name"> The file to fetch. </param>
    /// <returns> The contents of the file as a string or "" if an IOException occurred </returns>
public:
    virtual std::wstring getFileContentsAsString(const std::wstring &name);

    /// <summary>
    ///Fetches a data file from the working directory or resourcePath as an InputStream.
    /// </summary>
    /// <param name="name"> The name of the file to get an InputStream of. </param>
    /// <returns> An InputStream corresponding to the file. </returns>
    /// <exception cref="IOException">  </exception>
    virtual InputStream *getInputstreamFromFileName(const std::wstring &name) throw(IOException);

    /// <summary>
    ///Sets up an output stream to which a resource file can be written; this
    /// resource file will be in a subdirectory of the resources directory in
    /// the working directory.
    /// </summary>
    /// <param name="name"> The name of the file to write. </param>
    /// <returns> The output stream. </returns>
    /// <exception cref="IOException">  </exception>
    virtual OutputStream *getOutputStream(const std::wstring &name) throw(IOException);

private:
    File *getFileForWriting(const std::wstring &name) throw(IOException);
};

