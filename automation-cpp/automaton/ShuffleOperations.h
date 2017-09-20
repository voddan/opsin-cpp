#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <list>
#include <boost/optional.hpp>
#include <set>
#include "stringbuilder.h"

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace dk { namespace brics { namespace automaton { class Automaton; }}}
namespace dk { namespace brics { namespace automaton { class ShuffleConfiguration; }}}
namespace dk { namespace brics { namespace automaton { class Transition; }}}
namespace dk { namespace brics { namespace automaton { class State; }}}

/*
 * dk.brics.automaton
 * 
 * Copyright (c) 2001-2017 Anders Moeller
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

namespace dk {
    namespace brics {
        namespace automaton {


            /// <summary>
            /// Automata operations involving shuffling.
            /// </summary>
            class ShuffleOperations final {

            private:
                ShuffleOperations();

                /// <summary>
                /// Returns an automaton that accepts the shuffle (interleaving) of
                /// the languages of the given automata.
                /// As a side-effect, both automata are determinized, if not already deterministic.
                /// Never modifies the input automata languages.
                /// <para>
                /// Complexity: quadratic in number of states (if already deterministic).
                /// </para>
                /// <para>
                /// <dl><dt><b>Author:</b></dt><dd>Torben Ruby
                /// &lt;<a href="mailto:ruby@daimi.au.dk">ruby@daimi.au.dk</a>&gt;</dd></dl>
                /// </para>
                /// </summary>
            public:
                static Automaton * shuffle(Automaton * a1, Automaton * a2);

                /// <summary>
                /// Returns a string that is an interleaving of strings that are accepted by
                /// <code>ca</code> but not by <code>a</code>. If no such string
                /// exists, null is returned. As a side-effect, <code>a</code> is determinized,
                /// if not already deterministic. Only interleavings that respect
                /// the suspend/resume markers (two BMP private code points) are considered if the markers are non-null.
                /// Also, interleavings never split surrogate pairs.
                /// <para>
                /// Complexity: proportional to the product of the numbers of states (if <code>a</code>
                /// is already deterministic).
                /// </para>
                /// </summary>
                static std::wstring
                shuffleSubsetOf(std::vector<Automaton *> * ca, Automaton * a, boost::optional<wchar_t> suspend_shuffle,
                                boost::optional<wchar_t> resume_shuffle);

            private:
                static void add(boost::optional<wchar_t> suspend_shuffle, boost::optional<wchar_t> resume_shuffle,
                                std::list<ShuffleConfiguration *> & pending, std::set<ShuffleConfiguration *> * visited,
                                ShuffleConfiguration * c, int i1, Transition * t1, Transition * t2, wchar_t min,
                                wchar_t max);

            public:
                class ShuffleConfiguration {

                public:
                    ShuffleConfiguration * prev;
                    std::vector<State *> ca_states;
                    State * a_state;
                    wchar_t min = L'\0';
                    int hash = 0;
                    bool shuffle_suspended = false;
                    bool surrogate = false;
                    int suspended1 = 0;

                    virtual ~ShuffleConfiguration() {
                        delete prev;
                        delete a_state;
                    }

                private:
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @SuppressWarnings("unused") private ShuffleConfiguration()
                    ShuffleConfiguration();

                public:
                    ShuffleConfiguration(std::vector<Automaton *> * ca, Automaton * a);

                    ShuffleConfiguration(ShuffleConfiguration * c, int i1, State * s1, wchar_t min);

                    ShuffleConfiguration(ShuffleConfiguration * c, int i1, State * s1, State * s2, wchar_t min);

                    virtual bool equals(void * obj);

                    virtual int hashCode();

                private:
                    void computeHash();
                };
            };

        }
    }
}
