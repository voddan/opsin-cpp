#pragma once

#include "AutomatonProvider.h"
#include <string>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace dk { namespace brics { namespace automaton { class Automaton; }}}

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
            /// Automaton provider based on <seealso cref="Datatypes"/>.
            /// </summary>
            class DatatypesAutomatonProvider : public AutomatonProvider {

            private:
                bool enable_unicodeblocks = false, enable_unicodecategories = false, enable_xml = false;

                /// <summary>
                /// Constructs a new automaton provider that recognizes all names
                /// from <seealso cref="Datatypes#get(String)"/>.
                /// </summary>
            public:
                DatatypesAutomatonProvider();

                /// <summary>
                /// Constructs a new automaton provider that recognizes some of the names
                /// from <seealso cref="Datatypes#get(String)"/> </summary>
                /// <param name="enable_unicodeblocks"> if true, enable Unicode block names </param>
                /// <param name="enable_unicodecategories"> if true, enable Unicode category names </param>
                /// <param name="enable_xml"> if true, enable XML related names </param>
                DatatypesAutomatonProvider(bool enable_unicodeblocks, bool enable_unicodecategories, bool enable_xml);

                virtual Automaton * getAutomaton(const std::wstring & name);
            };

        }
    }
}
