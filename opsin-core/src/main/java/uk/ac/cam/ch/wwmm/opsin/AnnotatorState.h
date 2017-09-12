#pragma once

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


						/// <summary>
						/// Contains the state needed during finite-state parsing
						/// From this the tokens string and their semantics can be generated
						/// @author Daniel
						/// 
						/// </summary>
						class AnnotatorState
						{

							/// <summary>
							/// The current state of the DFA. </summary>
						private:
							const int state;
							/// <summary>
							/// The annotation so far. </summary>
							const wchar_t annot;

							/// <summary>
							/// The index of the first char in the chemical name that has yet to be tokenised </summary>
							const int posInName;

//JAVA TO C++ CONVERTER NOTE: Fields cannot have the same name as methods:
							const bool isCaseSensitive_Renamed;

							AnnotatorState *const previousAs;


						public:
							virtual ~AnnotatorState()
							{
								delete previousAs;
							}

							AnnotatorState(int state, wchar_t annot, int posInName, bool isCaseSensitive, AnnotatorState *previousAs);

							/// <summary>
							/// The current state in the DFA
							/// @return
							/// </summary>
							virtual int getState();

							/// <summary>
							/// The annotation that was consumed to transition to this state
							/// @return
							/// </summary>
							virtual wchar_t getAnnot();

							/// <summary>
							/// The index of the first char in the chemical name that has yet to be tokenised (at the point of creating this AnnotatorState)
							/// @return
							/// </summary>
							virtual int getPosInName();

							/// <summary>
							/// Where the corresponding token is case sensitive
							/// @return
							/// </summary>
							virtual bool isCaseSensitive();

							/// <summary>
							/// The last annotator state for the previous token (or null if this is the first)
							/// @return
							/// </summary>
							virtual AnnotatorState *getPreviousAs();
						};

					}
				}
			}
		}
	}
}
