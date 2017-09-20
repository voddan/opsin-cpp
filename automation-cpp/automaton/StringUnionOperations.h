#pragma once

#include "State.h"
#include <string>
#include <unordered_map>
#include <vector>
#include "stringhelper.h"
#include "stringbuilder.h"

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace dk { namespace brics { namespace automaton { class State; } } }

namespace dk
{
	namespace brics
	{
		namespace automaton
		{


			/// <summary>
			/// Operations for building minimal deterministic automata from sets of strings. 
			/// The algorithm requires sorted input data, but is very fast (nearly linear with the input size).
			/// 
			/// @author Dawid Weiss
			/// </summary>
			class StringUnionOperations final
			{

				/// <summary>
				/// Lexicographic order of input sequences.
				/// </summary>
			public:
				static Comparator< std::string*> *const LEXICOGRAPHIC_ORDER;

			private:
				class ComparatorAnonymousInnerClass //: public Comparator< std::string*>
				{
				public:
					ComparatorAnonymousInnerClass();

					virtual int compare(std::string *s1, std::string *s2);
				};

				/// <summary>
				/// State with <code>char</code> labels on transitions.
				/// </summary>
			public:
				class State final
				{

					/// <summary>
					/// An empty set of labels. </summary>
				private:
					static std::vector<wchar_t> const NO_LABELS = std::vector<wchar_t>(0);

					/// <summary>
					/// An empty set of states. </summary>
					static std::vector<State*> const NO_STATES;

					/// <summary>
					/// Labels of outgoing transitions. Indexed identically to <seealso cref="#states"/>.
					/// Labels must be sorted lexicographically.
					/// </summary>
				public:
					std::vector<wchar_t> labels = NO_LABELS;

					/// <summary>
					/// States reachable from outgoing transitions. Indexed identically to
					/// <seealso cref="#labels"/>.
					/// </summary>
					std::vector<State*> states = NO_STATES;

					/// <summary>
					/// <code>true</code> if this state corresponds to the end of at least one
					/// input sequence.
					/// </summary>
					bool is_final = false;

					/// <summary>
					/// Returns the target state of a transition leaving this state and labeled
					/// with <code>label</code>. If no such transition exists, returns
					/// <code>null</code>.
					/// </summary>
					State *getState(wchar_t label);

					/// <summary>
					/// Returns an array of outgoing transition labels. The array is sorted in 
					/// lexicographic order and indexes correspond to states returned from 
					/// <seealso cref="#getStates()"/>.
					/// </summary>
					std::vector<wchar_t> getTransitionLabels();

					/// <summary>
					/// Returns an array of outgoing transitions from this state. The returned
					/// array must not be changed.
					/// </summary>
					std::vector<State*> getStates();

					/// <summary>
					/// Two states are equal if:
					/// <ul>
					/// <li>they have an identical number of outgoing transitions, labeled with
					/// the same labels</li>
					/// <li>corresponding outgoing transitions lead to the same states (to states
					/// with an identical right-language).
					/// </ul>
					/// </summary>
					virtual bool equals(void *obj) override;

					/// <summary>
					/// Return <code>true</code> if this state has any children (outgoing
					/// transitions).
					/// </summary>
					bool hasChildren();

					/// <summary>
					/// Is this state a final state in the automaton?
					/// </summary>
					bool isFinal();

					/// <summary>
					/// Compute the hash code of the <i>current</i> status of this state.
					/// </summary>
					virtual int hashCode() override;

					/// <summary>
					/// Create a new outgoing transition labeled <code>label</code> and return
					/// the newly created target state for this transition.
					/// </summary>
					State *newState(wchar_t label);

					/// <summary>
					/// Return the most recent transitions's target state.
					/// </summary>
					State *lastChild();

					/// <summary>
					/// Return the associated state if the most recent transition
					/// is labeled with <code>label</code>.
					/// </summary>
					State *lastChild(wchar_t label);

					/// <summary>
					/// Replace the last added outgoing transition's target state with the given
					/// state.
					/// </summary>
					void replaceLastChild(State *state);

					/// <summary>
					/// JDK1.5-replacement of <seealso cref="Arrays#copyOf(char[], int)"/>
					/// </summary>
				private:
					static std::vector<wchar_t> copyOf(std::vector<wchar_t> &original, int newLength);

					/// <summary>
					/// JDK1.5-replacement of <seealso cref="Arrays#copyOf(char[], int)"/>
					/// </summary>
				public:
					static std::vector<State*> copyOf(std::vector<State*> &original, int newLength);

					/// <summary>
					/// Compare two lists of objects for reference-equality.
					/// </summary>
				private:
					static bool referenceEquals(std::vector<void*> &a1, std::vector<void*> &a2);
				};

				/// <summary>
				/// "register" for state interning.
				/// </summary>
			private:
				std::unordered_map<State*, State*> register_Renamed;

				/// <summary>
				/// Root automaton state.
				/// </summary>
				State *root = new State();

				/// <summary>
				/// Previous sequence added to the automaton in <seealso cref="#add(CharSequence)"/>.
				/// </summary>
				StringBuilder *previous;

				/// <summary>
				/// Add another character sequence to this automaton. The sequence must be
				/// lexicographically larger or equal compared to any previous sequences
				/// added to this automaton (the input must be sorted).
				/// </summary>
			public:
				virtual ~StringUnionOperations()
				{
					delete root;
					delete previous;
				}

				void add(std::string *current);

				/// <summary>
				/// Finalize the automaton and return the root state. No more strings can be
				/// added to the builder after this call.
				/// </summary>
				/// <returns> Root automaton state. </returns>
				State *complete();

				/// <summary>
				/// Internal recursive traversal for conversion.
				/// </summary>
			private:
				static dk::brics::automaton::State *convert(State *s, IdentityHashMap<State*, dk::brics::automaton::State*> *visited);

				/// <summary>
				/// Build a minimal, deterministic automaton from a sorted list of strings.
				/// </summary>
			public:
				static dk::brics::automaton::State *build(std::vector< std::string*> &input);

				/// <summary>
				/// Copy <code>current</code> into an internal buffer.
				/// </summary>
			private:
				bool setPrevious(std::string *current);

				/// <summary>
				/// Replace last child of <code>state</code> with an already registered
				/// state or register the last child state.
				/// </summary>
				void replaceOrRegister(State *state);

				/// <summary>
				/// Add a suffix of <code>current</code> starting at <code>fromIndex</code>
				/// (inclusive) to state <code>state</code>.
				/// </summary>
				void addSuffix(State *state, std::string *current, int fromIndex);
			};

		}
	}
}
