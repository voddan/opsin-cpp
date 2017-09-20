#include "StringUnionOperations.h"
#include "Transition.h"

namespace dk
{
	namespace brics
	{
		namespace automaton
		{

java::util::Comparator< std::string*> *const StringUnionOperations::LEXICOGRAPHIC_ORDER = new ComparatorAnonymousInnerClass();

			StringUnionOperations::ComparatorAnonymousInnerClass::ComparatorAnonymousInnerClass()
			{
			}

			int StringUnionOperations::ComparatorAnonymousInnerClass::compare(std::string *s1, std::string *s2)
			{
				constexpr int lens1 = s1->length();
				constexpr int lens2 = s2->length();
				constexpr int max = std::min(lens1, lens2);

				for (int i = 0; i < max; i++)
				{
					constexpr wchar_t c1 = s1->charAt(i);
					constexpr wchar_t c2 = s2->charAt(i);
					if (c1 != c2)
					{
						return c1 - c2;
					}
				}
				return lens1 - lens2;
			}

std::vector<State*> const StringUnionOperations::State::NO_STATES = std::vector<State*>(0);

			State *StringUnionOperations::State::getState(wchar_t label)
			{
				constexpr int index = Arrays::binarySearch(labels, label);
				return index >= 0 ? states[index] : nullptr;
			}

			std::vector<wchar_t> StringUnionOperations::State::getTransitionLabels()
			{
				return this->labels;
			}

			std::vector<State*> StringUnionOperations::State::getStates()
			{
				return this->states;
			}

			bool StringUnionOperations::State::equals(void *obj)
			{
				State * const other = static_cast<State*>(obj);
				return is_final == other->is_final && Arrays::equals(this->labels, other->labels) && referenceEquals(this->states, other->states);
			}

			bool StringUnionOperations::State::hasChildren()
			{
				return labels.size() > 0;
			}

			bool StringUnionOperations::State::isFinal()
			{
				return is_final;
			}

			int StringUnionOperations::State::hashCode()
			{
				int hash = is_final ? 1 : 0;

				hash ^= hash * 31 + this->labels.size();
				for (auto c : this->labels)
				{
					hash ^= hash * 31 + c;
				}

				/*
				 * Compare the right-language of this state using reference-identity of
				 * outgoing states. This is possible because states are interned (stored
				 * in registry) and traversed in post-order, so any outgoing transitions
				 * are already interned.
				 */
				for (auto s : this->states)
				{
					hash ^= System::identityHashCode(s);
				}

				return hash;
			}

			State *StringUnionOperations::State::newState(wchar_t label)
			{
				assert(Arrays::binarySearch(labels, label) < 0, std::wstring(L"State already has transition labeled: ") + StringHelper::toString(label));

				labels = copyOf(labels, labels.size() + 1);
				states = copyOf(states, states.size() + 1);

				labels[labels.size() - 1] = label;
				return states[states.size() - 1] = new State();
			}

			State *StringUnionOperations::State::lastChild()
			{
				assert(hasChildren(), L"No outgoing transitions.");
				return states[states.size() - 1];
			}

			State *StringUnionOperations::State::lastChild(wchar_t label)
			{
				constexpr int index = labels.size() - 1;
				State *s = nullptr;
				if (index >= 0 && labels[index] == label)
				{
					s = states[index];
				}
				assert(s == getState(label));
				return s;
			}

			void StringUnionOperations::State::replaceLastChild(State *state)
			{
				assert(hasChildren(), L"No outgoing transitions.");
				states[states.size() - 1] = state;
			}

			std::vector<wchar_t> StringUnionOperations::State::copyOf(std::vector<wchar_t> &original, int newLength)
			{
				std::vector<wchar_t> copy(newLength);
				System::arraycopy(original, 0, copy, 0, std::min(original.size(), newLength));
				return copy;
			}

			std::vector<State*> StringUnionOperations::State::copyOf(std::vector<State*> &original, int newLength)
			{
				std::vector<State*> copy(newLength);
				System::arraycopy(original, 0, copy, 0, std::min(original.size(), newLength));
				return copy;
			}

			bool StringUnionOperations::State::referenceEquals(std::vector<void*> &a1, std::vector<void*> &a2)
			{
				if (a1.size() != a2.size())
				{
					return false;
				}

				for (int i = 0; i < a1.size(); i++)
				{
					if (a1[i] != a2[i])
					{
						return false;
					}
				}

				return true;
			}

			void StringUnionOperations::add(std::string *current)
			{
				assert(register_Renamed.size() > 0, L"Automaton already built.");
				assert(current->length() > 0, L"Input sequences must not be empty.");
				assert(previous == nullptr || LEXICOGRAPHIC_ORDER::compare(previous, current) <= 0, std::wstring(L"Input must be sorted: ") + previous + std::wstring(L" >= ") + current);
				assert(setPrevious(current));

				// Descend in the automaton (find matching prefix). 
				int pos = 0, max = current->length();
				State *next, *state = root;
				while (pos < max && (next = state->lastChild(current->charAt(pos))) != nullptr)
				{
					state = next;
					pos++;
				}

				if (state->hasChildren())
				{
					replaceOrRegister(state);
				}

				addSuffix(state, current, pos);
			}

			State *StringUnionOperations::complete()
			{
				if (this->register_Renamed.empty())
				{
					throw IllegalStateException();
				}

				if (root->hasChildren())
				{
					replaceOrRegister(root);
				}

				register_Renamed.clear();
				return root;
			}

			dk::brics::automaton::State *StringUnionOperations::convert(State *s, IdentityHashMap<State*, dk::brics::automaton::State*> *visited)
			{
				dk::brics::automaton::State *converted = visited->get(s);
				if (converted != nullptr)
				{
					return converted;
				}

				converted = new dk::brics::automaton::State();
				converted->setAccept(s->is_final);

				visited->put(s, converted);
				int i = 0;
				std::vector<wchar_t> labels = s->labels;
				for (StringUnionOperations::State *target : s->states)
				{
					Transition tempVar(labels[i++], convert(target, visited));
					converted->addTransition(&tempVar);
				}

				return converted;
			}

			dk::brics::automaton::State *StringUnionOperations::build(std::vector< std::string*> &input)
			{
				StringUnionOperations * const builder = new StringUnionOperations();

				for (auto chs : input)
				{
					builder->add(chs);
				}

				IdentityHashMap<State*, dk::brics::automaton::State*> tempVar();
				return convert(builder->complete(), &tempVar);
			}

			bool StringUnionOperations::setPrevious(std::string *current)
			{
				if (previous == nullptr)
				{
					previous = new StringBuilder();
				}

				previous->setLength(0);
				previous->append(current);

				return true;
			}

			void StringUnionOperations::replaceOrRegister(State *state)
			{
				State * const child = state->lastChild();

				if (child->hasChildren())
				{
					replaceOrRegister(child);
				}

				State * const registered = register_Renamed[child];
				if (registered != nullptr)
				{
					state->replaceLastChild(registered);
				}
				else
				{
					register_Renamed[child] = child;
				}
			}

			void StringUnionOperations::addSuffix(State *state, std::string *current, int fromIndex)
			{
				constexpr int len = current->length();
				for (int i = fromIndex; i < len; i++)
				{
					state = state->newState(current->charAt(i));
				}
				state->is_final = true;
			}
		}
	}
}
