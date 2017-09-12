#pragma once

#include <string>
#include <stdexcept>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace uk { namespace ac { namespace cam { namespace ch { namespace wwmm { namespace opsin { class ResourceGetter; } } } } } }

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


						using org::apache::log4j::Logger;

						using dk::brics::automaton::RunAutomaton;

						/// <summary>
						/// Handles storing and retrieving automata to/from files
						/// This is highly useful to do as building these deterministic automata from scratch can take minutes
						/// @author dl387
						/// 
						/// </summary>
						class AutomatonInitialiser
						{

						private:
							static Logger *const LOG = Logger::getLogger(AutomatonInitialiser::typeid);
							ResourceGetter *const resourceGetter;

						public:
							virtual ~AutomatonInitialiser()
							{
								delete resourceGetter;
							}

							AutomatonInitialiser(const std::wstring &resourcePath);

							/// <summary>
							/// In preference serialised automata and their hashes will be looked for in the resource folder in your working directory
							/// If it cannot be found there then these files will be looked for in the standard resource folder
							/// (this is actually the standard behaviour of the resourceGetter but I'm reiterating it here as if the stored hash doesn't match
							/// the current hash then the creation of an updated serialised automaton and hash will occur in the working directory resource folder as the standard
							/// resource folder will not typically be writable) </summary>
							/// <param name="automatonName"> : A name for the automaton so that it can it can be saved/loaded from disk </param>
							/// <param name="regex"> : the regex from which to build the RunAutomaton </param>
							/// <param name="reverseAutomaton"> : should the automaton be reversed </param>
							/// <param name="tableize">: if true, a transition table is created which makes the run method faster in return of a higher memory usage (adds ~256kb) </param>
							/// <returns> A RunAutomaton, may have been built from scratch or loaded from a file </returns>
							virtual RunAutomaton *loadAutomaton(const std::wstring &automatonName, const std::wstring &regex, bool tableize, bool reverseAutomaton);

						private:
							bool isAutomatonCached(const std::wstring &automatonName, const std::wstring &regex);

							std::wstring getRegexHash(const std::wstring &regex);

							std::wstring getCachedRegexHash(const std::wstring &automatonName);

							RunAutomaton *loadCachedAutomaton(const std::wstring &automatonName) throw(IOException);

							static RunAutomaton *createAutomaton(const std::wstring &regex, bool tableize, bool reverseAutomaton);

							void cacheAutomaton(const std::wstring &automatonName, RunAutomaton *automaton, const std::wstring &regex);

						};

					}
				}
			}
		}
	}
}
