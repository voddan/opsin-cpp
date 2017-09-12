#include "AutomatonInitialiser.h"
#include "ResourceGetter.h"

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
						using org::apache::commons::io::IOUtils;
						using org::apache::log4j::Logger;
						using dk::brics::automaton::Automaton;
						using dk::brics::automaton::RegExp;
						using dk::brics::automaton::RunAutomaton;
						using dk::brics::automaton::SpecialOperations;

						AutomatonInitialiser::AutomatonInitialiser(const std::wstring &resourcePath) : resourceGetter(new ResourceGetter(resourcePath))
						{
						}

						RunAutomaton *AutomatonInitialiser::loadAutomaton(const std::wstring &automatonName, const std::wstring &regex, bool tableize, bool reverseAutomaton)
						{
							if (reverseAutomaton)
							{
								automatonName += std::wstring(L"_reversed_");
							}
							try
							{
								if (isAutomatonCached(automatonName, regex))
								{
									return loadCachedAutomaton(automatonName);
								}
							}
							catch (const IOException &e)
							{
								LOG->warn(std::wstring(L"Error loading cached automaton: ") + automatonName, e);
							}
							RunAutomaton *automaton = createAutomaton(regex, tableize, reverseAutomaton);
							cacheAutomaton(automatonName, automaton, regex);
							return automaton;
						}

						bool AutomatonInitialiser::isAutomatonCached(const std::wstring &automatonName, const std::wstring &regex)
						{
							std::wstring currentRegexHash = getRegexHash(regex);
							std::wstring cachedRegexHash = getCachedRegexHash(automatonName);
							return currentRegexHash == cachedRegexHash;
						}

						std::wstring AutomatonInitialiser::getRegexHash(const std::wstring &regex)
						{
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
							return Integer::toString(regex.hashCode());
						}

						std::wstring AutomatonInitialiser::getCachedRegexHash(const std::wstring &automatonName)
						{
							/*This file contains the hashcode of the regex which was used to generate the automaton on the disk */
							return resourceGetter->getFileContentsAsString(automatonName + std::wstring(L"RegexHash.txt"));
						}

						RunAutomaton *AutomatonInitialiser::loadCachedAutomaton(const std::wstring &automatonName) throw(IOException)
						{
							InputStream *automatonInput = resourceGetter->getInputstreamFromFileName(automatonName + std::wstring(L"SerialisedAutomaton.aut"));
							try
							{
								BufferedInputStream tempVar(automatonInput);
								return RunAutomaton::load(&tempVar);
							}
							catch (const std::exception &e)
							{
								IOException *ioe = new IOException(L"Error loading automaton");
								ioe->initCause(e);
								throw ioe;
							}
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to the exception 'finally' clause:
							finally
							{
								IOUtils::closeQuietly(automatonInput);
							}
						}

						RunAutomaton *AutomatonInitialiser::createAutomaton(const std::wstring &regex, bool tableize, bool reverseAutomaton)
						{
							RegExp tempVar(regex);
							Automaton *a = (&tempVar)->toAutomaton();
							if (reverseAutomaton)
							{
								SpecialOperations::reverse(a);
							}
							return new RunAutomaton(a, tableize);
						}

						void AutomatonInitialiser::cacheAutomaton(const std::wstring &automatonName, RunAutomaton *automaton, const std::wstring &regex)
						{
							OutputStream *regexHashOutputStream = nullptr;
							OutputStream *automatonOutputStream = nullptr;
							try
							{
								regexHashOutputStream = resourceGetter->getOutputStream(automatonName + std::wstring(L"RegexHash.txt"));
								try
								{
									regexHashOutputStream->write(getRegexHash(regex).getBytes(L"UTF-8"));
								}
								catch (const UnsupportedEncodingException &e)
								{
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("Java VM is broken; UTF-8 should be supported", e);
									throw std::exception();
								}
								automatonOutputStream = resourceGetter->getOutputStream(automatonName + std::wstring(L"SerialisedAutomaton.aut"));
								automaton->store(automatonOutputStream);
							}
							catch (const IOException &e)
							{
								LOG->warn(std::wstring(L"Error serialising automaton: ") + automatonName, e);
							}
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to the exception 'finally' clause:
							finally
							{
								IOUtils::closeQuietly(regexHashOutputStream);
								IOUtils::closeQuietly(automatonOutputStream);
							}
						}
					}
				}
			}
		}
	}
}
