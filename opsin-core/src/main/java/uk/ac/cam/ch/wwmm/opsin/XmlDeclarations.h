#pragma once

#include <string>

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
						/// Contains static final strings corresponding to XML element names and attributes employed by OPSIN
						/// @author dl387
						/// 
						/// </summary>
						class XmlDeclarations
						{

							//TODO are all these types and subtypes actually a good idea considering the vast majority are never used?

							/*
							 * The container XML elements. These are generated by OPSIN
							 */
							/// <summary>
							///Define a scope for determining what group a substituent should bond to </summary>
						public:
							static const std::wstring BRACKET_EL;

							/// <summary>
							///Contains a functional group or class. These terms typically effect the chosen wordRule for the name </summary>
							static const std::wstring FUNCTIONALTERM_EL;

							/// <summary>
							///The top most element in OPSIN's parse tree. As a name can describe multiple molecules the same is confusingly true of this element </summary>
							static const std::wstring MOLECULE_EL;

							/// <summary>
							///Contains a substituent. A substituent will after the ComponentProcessor contain one group </summary>
							static const std::wstring SUBSTITUENT_EL;

							/// <summary>
							///Contains a root group(the rightmost in a word). A root will after the ComponentProcessor contain one group </summary>
							static const std::wstring ROOT_EL;

							/// <summary>
							///Contains brackets/substituents/root. Generally these correspond to words in the original chemical name (unless erroneous/omitted spaces were present) </summary>
							static const std::wstring WORD_EL;

							/// <summary>
							///Contains words/wordRules. The value of the wordRule indicates how the StructureBuilder should process its children </summary>
							static const std::wstring WORDRULE_EL;


							/*
							 * The token XML elements. These are generally produced by the parser from the tokenised chemical name
							 * Some are produced by OPSIN in the ComponentGenerator/ComponentProcessor
							 */

							/// <summary>
							///Adds a hydrogen to an unsaturated system, this is hydrogen that is added due to a suffix and is expressed in a locant e.g. 1(2H) </summary>
							static const std::wstring ADDEDHYDROGEN_EL;

							/// <summary>
							///A component of an alkaneStem e.g. [octa][hexaconta][tetract]ane will have three alkaneStemComponents </summary>
							static const std::wstring ALKANESTEMCOMPONENT;

							/// <summary>
							///Something like tert/iso/sec Modifies an alkaneStem in the ComponentGenerator </summary>
							static const std::wstring ALKANESTEMMODIFIER_EL;

							/// <summary>
							///An annulene. Converted to a group by the ComponentGenerator </summary>
							static const std::wstring ANNULEN_EL;

							/// <summary>
							///A bridge described in SMILES for used on rings </summary>
							static const std::wstring FUSEDRINGBRIDGE_EL;

							/// <summary>
							///An O that indicates that the preceding alkaneStem is in fact a bridge </summary>
							static const std::wstring BRIDGEFORMINGO_EL;

							/// <summary>
							///A locant indicating the positions for a glycosidic linkage. The first locant will point to an alpha carbon
							/// Also used to indicate joining of nucleosyl groups
							/// </summary>
							static const std::wstring BIOCHEMICALLINKAGE_EL;

							/// <summary>
							///Indicates the size of the ring in a carbohydrate e.g. furanose = 5 </summary>
							static const std::wstring CARBOHYDRATERINGSIZE_EL;

							/// <summary>
							///A charge specifier e.g. (2+). Value is the charge to set something to </summary>
							static const std::wstring CHARGESPECIFIER_EL;

							/// <summary>
							///Created by the ComponentProcessor. Something like the acetic acid in benzene-1,3,5-triacetic acid </summary>
							static const std::wstring CONJUNCTIVESUFFIXGROUP_EL;

							/// <summary>
							///Used by the ComponentGenerator to group elements into bracket elements </summary>
							static const std::wstring CLOSEBRACKET_EL;

							/// <summary>
							///Used by the ComponentGenerator to modify alkanes into cycloalkanes </summary>
							static const std::wstring CYCLO_EL;

							/// <summary>
							/// A delta used to indicate the position of a double bond in older nomenclature </summary>
							static const std::wstring DELTA_EL;

							/// <summary>
							///A fractional multiplier e.g. hemi </summary>
							static const std::wstring FRACTIONALMULTIPLIER_EL;

							/// <summary>
							///A functional Class such as acid. Does not correspond to a fragment </summary>
							static const std::wstring FUNCTIONALCLASS_EL;

							/// <summary>
							///A functional group such as alcohol or sulfone. Describes a fragment </summary>
							static const std::wstring FUNCTIONALGROUP_EL;

							/// <summary>
							///Currently just poly or oligo for polymers </summary>
							static const std::wstring FUNCTIONALMODIFIER_EL;

							/// <summary>
							///A fusion bracket. Used in fusion nomenclature </summary>
							static const std::wstring FUSION_EL;

							/// <summary>
							///Define a scope for determining what group a substituent should bond to </summary>
							static const std::wstring GROUP_EL;

							/// <summary>
							///A heteroatom. Could be part of a Hantzsch Widman ring or a replacement prefix </summary>
							static const std::wstring HETEROATOM_EL;

							/// <summary>
							///Adds a hydrogen to an unsaturated system (hydro/perhydro) </summary>
							static const std::wstring HYDRO_EL;

							/// <summary>
							///One of the systematic hydrocarbon fused ring series e.g. tetralene, pentalene. Converted to a group by the ComponentGenerator </summary>
							static const std::wstring HYDROCARBONFUSEDRINGSYSTEM_EL;

							/// <summary>
							///Adds a hydrogen to an unsaturated system to indicate what atoms are saturated in a system where not all atoms with spare valency can form double bonds e.g.  e.g. 2H-pyran </summary>
							static const std::wstring INDICATEDHYDROGEN_EL;

							/// <summary>
							///Specifies that one of more atoms are enriched with a particular isotope </summary>
							static const std::wstring ISOTOPESPECIFICATION_EL;

							/// <summary>
							///A hyphen between two substituents. Used as hint that the two substituents do not join together </summary>
							static const std::wstring HYPHEN_EL;

							/// <summary>
							///ine as in the end of an aminoAcid. Has no meaning </summary>
							static const std::wstring INE_EL;

							/// <summary>
							///An infix. This performs functionalReplacement on a suffix </summary>
							static const std::wstring INFIX_EL;

							/// <summary>
							///Indicates that a heteroatom or atom should be in a specific valency </summary>
							static const std::wstring LAMBDACONVENTION_EL;

							/// <summary>
							///A locant e.g. where a substituent should attach </summary>
							static const std::wstring LOCANT_EL;

							/// <summary>
							///Used by the ComponentGenerator to group elements into bracket elements </summary>
							static const std::wstring OPENBRACKET_EL;

							/// <summary>
							///otho/meta/para Converted to a locant by the ComponentProcessor </summary>
							static const std::wstring ORTHOMETAPARA_EL;

							/// <summary>
							///Describes the number of spiro centres in a poly cyclic spiro system </summary>
							static const std::wstring POLYCYCLICSPIRO_EL;

							/// <summary>
							///A locant indicating through which atoms a multiplied parent in multiplicative nomenclature is connected </summary>
							static const std::wstring MULTIPLICATIVELOCANT_EL;

							/// <summary>
							///A multiplier e.g. indicating multiplication of a heteroatom or substituent </summary>
							static const std::wstring MULTIPLIER_EL;

							/// <summary>
							///e.g. (III), Specifies the oxidation number of an atom. Value is the oxidation number to set something to </summary>
							static const std::wstring OXIDATIONNUMBERSPECIFIER_EL;

							/// <summary>
							///Used amongst other things to indicate how the rings of a ring assembly should be assembled </summary>
							static const std::wstring COLONORSEMICOLONDELIMITEDLOCANT_EL;

							/// <summary>
							///Used to indicate how many rings are in a ring assembly </summary>
							static const std::wstring RINGASSEMBLYMULTIPLIER_EL;

							/// <summary>
							///A spiro system. Converted to a group by the ComponentGenerator </summary>
							static const std::wstring SPIRO_EL;

							/// <summary>
							///A locant that seperates components of a spiro system </summary>
							static const std::wstring SPIROLOCANT_EL;

							/// <summary>
							///Something like R/S/E/Z. Indicates stereochemical configuration </summary>
							static const std::wstring STEREOCHEMISTRY_EL;

							/// <summary>
							///Present in complicated nomenclature e.g. ring assemblies to avoid ambiguity </summary>
							static const std::wstring STRUCTURALCLOSEBRACKET_EL;

							/// <summary>
							///Present in complicated nomenclature to avoid ambiguity </summary>
							static const std::wstring STRUCTURALOPENBRACKET_EL;

							/// <summary>
							///Indicates replacement of a group by hydrogen e.g. deoxy means replace OH with H </summary>
							static const std::wstring SUBTRACTIVEPREFIX_EL;

							/// <summary>
							///A suffix e.g. amide, al, yl etc. </summary>
							static const std::wstring SUFFIX_EL;

							/// <summary>
							///Something like sulfon/carbo/carbox that modifies a following suffix </summary>
							static const std::wstring SUFFIXPREFIX_EL;

							/// <summary>
							///ene/yne, indicated that a double/triple bond should be formed at a saturated location </summary>
							static const std::wstring UNSATURATOR_EL;

							/// <summary>
							///A vonBaeyer system. Converted to a group by the ComponentGenerator </summary>
							static const std::wstring VONBAEYER_EL;

							/*
							 * The token XML attributes. These are generally produced by the parser from the tokenised chemical name
							 * Some are produced by OPSIN in the ComponentGenerator/ComponentProcessor
							 */

							/// <summary>
							///The semantic meaning of the token. Exact meaning is dependent on the element type e.g. SMILES for a group but a number for a multiplier </summary>
							static const std::wstring VALUE_ATR;

							/// <summary>
							///The type of the token. Possible values are enumerated with strings ending in _TYPE_VAL </summary>
							static const std::wstring TYPE_ATR;

							/// <summary>
							///The subType of the token. Possible values are enumerated with strings ending in _SUBTYPE_VAL </summary>
							static const std::wstring SUBTYPE_ATR;

							/// <summary>
							///Whether the group can be additively bonded to. e.g. thio </summary>
							static const std::wstring ACCEPTSADDITIVEBONDS_ATR;

							/// <summary>
							///Used to add a higher order bond at a position that can be subsequently specified.
							/// Syntax: semicolon delimited list of the format: orderOfBond space ("id"|"locant"|"defaultId"|"defaultLocant") space (id|locant) 
							/// </summary>
							static const std::wstring ADDBOND_ATR;

							/// <summary>
							///Used to add a group at a position that can be subsequently specified
							/// Syntax: semicolon delimited list of the format: SMILESofGroupToBeAdded space ("id"|"locant"|"defaultId"|"defaultLocant") space (id|locant) [space locantLabel]. 
							/// </summary>
							static const std::wstring ADDGROUP_ATR;

							/// <summary>
							///Used to set a heteroatom at a position that can be subsequently specified
							/// Syntax: semicolon delimited list of the format: elementOfAtom space ("id"|"locant"|"defaultId"|"defaultLocant") space (id|locant). 
							/// </summary>
							static const std::wstring ADDHETEROATOM_ATR;

							/// <summary>
							///Another value that the token takes. e.g. for suffix tokens that add two suffixes to the molecule </summary>
							static const std::wstring ADDITIONALVALUE_ATR;

							/// <summary>
							///Listed in a clockwise order, the locants of the atoms that define a pseudo 2D plane for alpha/beta stereochemistry </summary>
							static const std::wstring ALPHABETACLOCKWISEATOMORDERING_ATR;

							/// <summary>
							///For elements, the typical oxidation states (comma separated) then a colon and the maximum oxidation station </summary>
							static const std::wstring COMMONOXIDATIONSTATESANDMAX_ATR;

							/// <summary>
							///The ID of the atom which by default an incoming fragment should connect to. ID is relative to this particular fragment (first atom =1) </summary>
							static const std::wstring DEFAULTINID_ATR;

							/// <summary>
							///The locant of the atom which by default an incoming fragment should connect to* </summary>
							static const std::wstring DEFAULTINLOCANT_ATR;

							/// <summary>
							///Works like the locant attribute but refers to the atoms OPSIN ID. Will be overridden by the locant/locantId attribute </summary>
							static const std::wstring DEFAULTLOCANTID_ATR;

							/// <summary>
							///A comma separated list of locants that are expected in front of a group for either xylene-like nomenclature or as indirect locants </summary>
							static const std::wstring FRONTLOCANTSEXPECTED_ATR;

							/// <summary>
							///A comma separated list of relative IDs at which to add functionalAtoms </summary>
							static const std::wstring FUNCTIONALIDS_ATR;

							/// <summary>
							///Numbering to use when ring is part of a fused ring system </summary>
							static const std::wstring FUSEDRINGNUMBERING_ATR;

							/// <summary>
							///Semi-colon delimited list of labels for * atoms, where the * atoms represent generic groups e.g. Alkyl </summary>
							static const std::wstring HOMOLOGY_ATR;

							/// <summary>
							///Indicates that the substituent can either be -X- or X=  depending on context cf. imino or methylene </summary>
							static const std::wstring IMINOLIKE_ATR;

							/// <summary>
							///The functional replacement specified by an infix to be performed on the suffix </summary>
							static const std::wstring INFIX_ATR;

							/// <summary>
							///Defines the locants for which a radical will connect to another group in multiplicative nomenclature e.g. in 2,2'-methylenedipyridine the 2,2' become inlocants of the pyridine </summary>
							static const std::wstring INLOCANTS_ATR;

							/// <summary>
							///Determined by the <seealso cref="ComponentProcessor"/>. True if a fragment has more than two radical positions e.g. ethan-1,2-diyl not ethanylidene </summary>
							static const std::wstring ISAMULTIRADICAL_ATR;

							/// <summary>
							///Was the word salt encountered indicating that a salt was expected? </summary>
							static const std::wstring ISSALT_ATR;

							/// <summary>
							///Slash delimited list of locants. List must be the same length as number of atoms. Multiple locants can be given to an atom by comma delimiting them </summary>
							static const std::wstring LABELS_ATR;

							/// <summary>
							///Added to a heteroatom or LAMBDACONVENTION_EL to indicate the desired valency </summary>
							static const std::wstring LAMBDA_ATR;

							/// <summary>
							///Locant used when deciding where to apply an operation </summary>
							static const std::wstring LOCANT_ATR;

							/// <summary>
							///Works like a locant but refers to the atom's OPSIN id </summary>
							static const std::wstring LOCANTID_ATR;

							/// <summary>
							///Indicates that this trivial name has the opposite D/L stereochemistry to others in its class i.e. L- for carbohydrates or D- for amino acids </summary>
							static const std::wstring NATURALENTISOPPOSITE_ATR;

							/// <summary>
							/// Used as a fudge for some hydrogen esters e.g. dihydrogenphosphate </summary>
							static const std::wstring NUMBEROFFUNCTIONALATOMSTOREMOVE_ATR;

							/// <summary>
							///Indicates that an element has been multiplied. Prevents badly assigning indirect locants </summary>
							static const std::wstring MULTIPLIED_ATR;

							/// <summary>
							///Indicates how many times a bracket/substituent should be multiplied </summary>
							static const std::wstring MULTIPLIER_ATR;

							/// <summary>
							/// The name that was inputted into OPSIN's parser. Attribute of molecule </summary>
							static const std::wstring NAME_ATR;

							/// <summary>
							///A comma separated list of relative IDs at which to add OutAtoms </summary>
							static const std::wstring OUTIDS_ATR;

							/// <summary>
							///Indicates that a substituent/bracket has been processed by StructureBuildingMethods </summary>
							static const std::wstring RESOLVED_ATR;

							/// <summary>
							///Placed on a word rule if explicit stoichiometry has been provided. Value is always an integer </summary>
							static const std::wstring STOICHIOMETRY_ATR;

							/// <summary>
							/// Holds the value of any tokens for which XML was not generated by the parser e.g. an optional e. Multiple elided tokens will be concatenated </summary>
							static const std::wstring SUBSEQUENTUNSEMANTICTOKEN_ATR;

							/// <summary>
							///A comma separated list of relatives IDs indicating where to add suffix/es </summary>
							static const std::wstring SUFFIXAPPLIESTO_ATR;

							/// <summary>
							///A relatives ID indicating at what position to attach a suffix to by default </summary>
							static const std::wstring SUFFIXAPPLIESTOBYDEFAULT_ATR;

							/// <summary>
							///Added by the ComponentGenerator to a suffix </summary>
							static const std::wstring SUFFIXPREFIX_ATR;

							/// <summary>
							///Can the substituent be implicitly bracketed to a previous substitutent e.g. methylaminobenzene --> (methylamino)benzene as amino has this attribute </summary>
							static const std::wstring USABLEASJOINER_ATR;

							/// <summary>
							///The wordRule that a wordRule element corresponds to </summary>
							static const std::wstring WORDRULE_ATR;

							/*
							 * The values the type attribute can take
							 * Type is expected to be present at minimum on all group elements
							 */
							/// <summary>
							///A term like amide or hydrazide that replaces a functional hydroxy group </summary>
							static const std::wstring ACIDREPLACINGFUNCTIONALGROUP_TYPE_VAL;

							/// <summary>
							///A trivial carboxylic acid. These by default do not have their acid groups which are then added on using suffixes </summary>
							static const std::wstring ACIDSTEM_TYPE_VAL;

							/// <summary>
							///This stereochemistry element conveys alpha/beta stereochemistry </summary>
							static const std::wstring ALPHA_OR_BETA_TYPE_VAL;

							/// <summary>
							///An aminoAcid. These by default do not have their acid groups which are then added on using suffixes. Notably these suffixes do NOT correspond to tokens in the input chemical name! </summary>
							static const std::wstring AMINOACID_TYPE_VAL;

							/// <summary>
							///A subtractive prefix that removes a terminal chalcogen and forms an intramolecular bridge to another </summary>
							static const std::wstring ANHYDRO_TYPE_VAL;

							/// <summary>
							///This stereochemistry element conveys axial stereochemistry
							/// These indicate the postion of groups are an axis/plane/helix. This is expressed by the descriptors: M, P, Ra, Sa, Rp, Sp
							/// </summary>
							static const std::wstring AXIAL_TYPE_VAL;

							/// <summary>
							///A normal multiplier e.g. di </summary>
							static const std::wstring BASIC_TYPE_VAL;

							/// <summary>
							///An isotopeSpecification using boughton system nomenclature </summary>
							static const std::wstring BOUGHTONSYSTEM_TYPE_VAL;

							/// <summary>
							///A locant enclosed in square brackets e.g. [5] </summary>
							static const std::wstring BRACKETEDLOCANT_TYPE_VAL;

							/// <summary>
							///This stereochemistry element specifies stereochemistry in a carbohydrate e.g. gluco is  r/l/r/r (position of hydroxy in a fischer projection) </summary>
							static const std::wstring CARBOHYDRATECONFIGURATIONPREFIX_TYPE_VAL;

							/// <summary>
							///Groups formed in accordance with carbohydrate nomenclature </summary>
							static const std::wstring CARBOHYDRATE_TYPE_VAL;

							/// <summary>
							///Indicates the group should be acyclic </summary>
							static const std::wstring CHAIN_TYPE_VAL;

							/// <summary>
							///This suffix modifies charge </summary>
							static const std::wstring CHARGE_TYPE_VAL;

							/// <summary>
							///This stereochemistry element conveys cis/trans stereochemistry </summary>
							static const std::wstring CISORTRANS_TYPE_VAL;

							/// <summary>
							///This stereochemistry element conveys R/S stereochemistry </summary>
							static const std::wstring R_OR_S_TYPE_VAL;

							/// <summary>
							///This stereochemistry element conveys E/Z stereochemistry </summary>
							static const std::wstring E_OR_Z_TYPE_VAL;

							/// <summary>
							///This group is a sulfur/selenium/tellurium acid with the acidic hydroxy missing </summary>
							static const std::wstring CHALCOGENACIDSTEM_TYPE_VAL;

							/// <summary>
							///A subtractive prefix that removes a hydrogen to covert a hydroxy into a carbonyl or convert a bond to a double/triple bond </summary>
							static const std::wstring DEHYDRO_TYPE_VAL;

							/// <summary>
							///A subtractive prefix that removes a terminal hydroxy like atom </summary>
							static const std::wstring DEOXY_TYPE_VAL;

							/// <summary>
							///A functional group describing a divalent group </summary>
							static const std::wstring DIVALENTGROUP_TYPE_VAL;

							/// <summary>
							/// This stereochemsitry element indicates the configuration of an amino acid/carbohydrate relative to glyceraldehyde </summary>
							static const std::wstring DLSTEREOCHEMISTRY_TYPE_VAL;

							/// <summary>
							///This stereochemistry element conveys endo/exo/syn/anti stereochemistry
							/// These indicate relative orientation of groups attached to non-bridgehead atoms in a bicyclo[x.y.z]alkane (x >= y > z > 0)
							/// </summary>
							static const std::wstring ENDO_EXO_SYN_ANTI_TYPE_VAL;

							/// <summary>
							///A group that is functional class e.g. O for anhydride </summary>
							static const std::wstring FUNCTIONALCLASS_TYPE_VAL;

							/// <summary>
							///A multiplier for groups of terms e.g. bis </summary>
							static const std::wstring GROUP_TYPE_VAL;

							/// <summary>
							///An implicit bracket. Implicit brackets are added where a bracket is needed to give the intended meaning </summary>
							static const std::wstring IMPLICIT_TYPE_VAL;

							/// <summary>
							///This suffix adds a radical to the preceding group e.g. yl, oyl </summary>
							static const std::wstring INLINE_TYPE_VAL;

							/// <summary>
							///An isotopeSpecification using IUPAC nomenclature </summary>
							static const std::wstring IUPACSYSTEM_TYPE_VAL;

							/// <summary>
							///This functional group is monovalent e.g. alcohol </summary>
							static const std::wstring MONOVALENTGROUP_TYPE_VAL;

							/// <summary>
							///This functional group is monovalent and describes a specific compound e.g. cyanide </summary>
							static const std::wstring MONOVALENTSTANDALONEGROUP_TYPE_VAL;

							/// <summary>
							///A non carboxylic acid e.g. phosphoric </summary>
							static const std::wstring NONCARBOXYLICACID_TYPE_VAL;

							/// <summary>
							///This stereochemistry element describes the direction that plane polarised light is rotated </summary>
							static const std::wstring OPTICALROTATION_TYPE_VAL;

							/// <summary>
							///Indicates the locant was made from an ortho/meta/para term </summary>
							static const std::wstring ORTHOMETAPARA_TYPE_VAL;

							/// <summary>
							///This stereochemistry element conveys relative cis/trans stereochemistry e.g. r-1, c-2, t-3 </summary>
							static const std::wstring RELATIVECISTRANS_TYPE_VAL;

							/// <summary>
							///Indicates the group should be, at least in part, cyclic </summary>
							static const std::wstring RING_TYPE_VAL;

							/// <summary>
							///Indicates a group that does not allow suffixes </summary>
							static const std::wstring SIMPLEGROUP_TYPE_VAL;

							/// <summary>
							///Groups that do not have any special rules for suffix handling </summary>
							static const std::wstring STANDARDGROUP_TYPE_VAL;

							/// <summary>
							///A bracket containing R/S/E/Z descriptors </summary>
							static const std::wstring STEREOCHEMISTRYBRACKET_TYPE_VAL;

							/// <summary>
							///Indicates a group that is a substituent </summary>
							static const std::wstring SUBSTITUENT_TYPE_VAL;

							/// <summary>
							///A locant that also indicated the addition of hydrogen e.g.2(1H); not used to locant onto another group </summary>
							static const std::wstring ADDEDHYDROGENLOCANT_TYPE_VAL;

							/// <summary>
							///Indicates a group that is a suffix </summary>
							static const std::wstring SUFFIX_TYPE_VAL;

							/// <summary>
							///A suffix that does not add a radical, hence will be present only on the root group </summary>
							static const std::wstring ROOT_TYPE_VAL;

							/// <summary>
							///A multiplier for a Von Baeyer system e.g. bi in bicyclo </summary>
							static const std::wstring VONBAEYER_TYPE_VAL;


							/*
							 * The values the subType attribute can take
							 * subType is expected to be present at minimum on all group elements
							 */

							/// <summary>
							///The stem of an alkane e.g. "eth" </summary>
							static const std::wstring ALKANESTEM_SUBTYPE_VAL;
							/// <summary>
							///An anhydride functional term e.g. "thioanhydride" </summary>
							static const std::wstring ANHYDRIDE_SUBTYPE_VAL;
							/// <summary>
							///An aryl subsituent or stem e.g. "phenyl", "styr" </summary>
							static const std::wstring ARYLSUBSTITUENT_SUBTYPE_VAL;
							//FIXME ideally carbohydrates and nucleotides/nucleosides/natural products should have a common type or subtype
							/// <summary>
							///Nucleotides/nucleosides/natural products. Carbohydrates can be detected by <seealso cref="XmlDeclarations#CARBOHYDRATE_TYPE_VAL"/> </summary>
							static const std::wstring BIOCHEMICAL_SUBTYPE_VAL;
							/// <summary>
							///A trivial carbohydrate stem for an aldose e.g. "galact" </summary>
							static const std::wstring CARBOHYDRATESTEMALDOSE_SUBTYPE_VAL;
							/// <summary>
							///A trivial carbohydrate stem for a ketose e.g. "fruct" </summary>
							static const std::wstring CARBOHYDRATESTEMKETOSE_SUBTYPE_VAL;
							/// <summary>
							///A suffix that forms a cycle e.g. imide, lactam, sultam </summary>
							static const std::wstring CYCLEFORMER_SUBTYPE_VAL;
							/// <summary>
							///A hydrocarbon stem that is typically followed by an unsaturator e.g. "adamant" </summary>
							static const std::wstring CYCLICUNSATURABLEHYDROCARBON_SUBTYPE_VAL;
							/// <summary>
							///Replacmenet terms that are not substituents e.g.  amido/hydrazido/imido/nitrido </summary>
							static const std::wstring DEDICATEDFUNCTIONALREPLACEMENTPREFIX_SUBTYPE_VAL;
							/// <summary>
							///An atom e.g. "lithium" </summary>
							static const std::wstring ELEMENTARYATOM_SUBTYPE_VAL;
							/// <summary>
							///An amino acid that ends in an e.g. tryptoph </summary>
							static const std::wstring ENDINAN_SUBTYPE_VAL;
							/// <summary>
							///An amino acid that ends in ic e.g. aspart </summary>
							static const std::wstring ENDINIC_SUBTYPE_VAL;
							/// <summary>
							///An amino acid that ends in ine e.g. alan </summary>
							static const std::wstring ENDININE_SUBTYPE_VAL;
							/// <summary>
							///A substituent that is expected to form a bridge e.g. "epoxy", "epiimino" </summary>
							static const std::wstring EPOXYLIKE_SUBTYPE_VAL;
							/// <summary>
							///A ring that will be fused onto another ring e.g. "benzo", "pyrido", "pyridino" </summary>
							static const std::wstring FUSIONRING_SUBTYPE_VAL;
							/// <summary>
							///A group that can be suffixed e.g. "hydrazin" </summary>
							static const std::wstring GROUPSTEM_SUBTYPE_VAL;
							/// <summary>
							///A halide or pseudo halide e.g. "bromo", "cyano". Can be functional replacment terms when preceding certain non-carboxylic acids </summary>
							static const std::wstring HALIDEORPSEUDOHALIDE_SUBTYPE_VAL;
							/// <summary>
							///The stem of a hantzch Widman ring sytem e.g. "an", "ol", "olidin" </summary>
							static const std::wstring HANTZSCHWIDMAN_SUBTYPE_VAL;
							/// <summary>
							///A heteroatom hydride e.g. "az" "sulf" (will be followed by an unsaturator, may be preceded by a multiplier to form the heteroatom equivalent of alkanes) </summary>
							static const std::wstring HETEROSTEM_SUBTYPE_VAL;
							/// <summary>
							///A group with no special properties Similar to: <seealso cref="XmlDeclarations#NONE_SUBTYPE_VAL"/> </summary>
							static const std::wstring SIMPLEGROUP_SUBTYPE_VAL;
							/// <summary>
							///A substituent which intrinsically forms multiple bonds e.g. "siloxane", "thio" </summary>
							static const std::wstring MULTIRADICALSUBSTITUENT_SUBTYPE_VAL;
							/// <summary>
							///A non-carboxylic acid which cannot form a substituent e.g. "bor" </summary>
							static const std::wstring NOACYL_SUBTYPE_VAL;
							/// <summary>
							///A group with no special properties Similar to: <seealso cref="XmlDeclarations#SIMPLEGROUP_SUBTYPE_VAL"/> </summary>
							static const std::wstring NONE_SUBTYPE_VAL;
							/// <summary>
							///oxido/sulfido/selenido/tellurido These are handled similarly to oxide e.g. might give -[O-] or =O </summary>
							static const std::wstring OXIDOLIKE_SUBTYPE_VAL;
							/// <summary>
							///A term indicating replacement of all substitutable hydrogens by a halogen e.g. "perchloro" </summary>
							static const std::wstring PERHALOGENO_SUBTYPE_VAL;
							/// <summary>
							/// phospho and other very related substituents. Strongly prefer forming bonds to hydroxy groups </summary>
							static const std::wstring PHOSPHO_SUBTYPE_VAL;
							/// <summary>
							///A ring group e.g. "pyridin" </summary>
							static const std::wstring RING_SUBTYPE_VAL;
							/// <summary>
							/// A component of a salt e.g "hydrate", "2HCl" </summary>
							static const std::wstring SALTCOMPONENT_SUBTYPE_VAL;
							/// <summary>
							///A substitutent with no suffix e.g. "amino" </summary>
							static const std::wstring SIMPLESUBSTITUENT_SUBTYPE_VAL;
							/// <summary>
							///A substituent expecting a suffix e.g."bor" "vin" </summary>
							static const std::wstring SUBSTITUENT_SUBTYPE_VAL;
							/// <summary>
							///A group representing a straight chain carbohydrate of a certain length with undefined stereochemistry e.g. hex in hexose </summary>
							static const std::wstring SYSTEMATICCARBOHYDRATESTEMALDOSE_SUBTYPE_VAL;
							/// <summary>
							///A group representing a straight chain carbohydrate of a certain length with undefined stereochemistry e.g. hex in hex-2-ulose </summary>
							static const std::wstring SYSTEMATICCARBOHYDRATESTEMKETOSE_SUBTYPE_VAL;
							/// <summary>
							///A suffix that attaches to the end of a chain e.g. "aldehyde", "ic acid" </summary>
							static const std::wstring TERMINAL_SUBTYPE_VAL;
							/// <summary>
							///An acid that when suffixed with yl gives an acyl group e.g. "acet" </summary>
							static const std::wstring YLFORACYL_SUBTYPE_VAL;
							/// <summary>
							///An acid that has undefined meaning when suffixed with yl </summary>
							static const std::wstring YLFORNOTHING_SUBTYPE_VAL;
							/// <summary>
							///An acid that when suffixed with yl gives an alkyl group e.g. "laur" </summary>
							static const std::wstring YLFORYL_SUBTYPE_VAL;

							/// <summary>
							///Requests that no labelling should be applied </summary>
							static const std::wstring NONE_LABELS_VAL;

							/// <summary>
							///Requests that labelling be done like a fused ring. It is assumed that the order of the atoms is locant 1 as the first atom </summary>
							static const std::wstring FUSEDRING_LABELS_VAL;

							/// <summary>
							///Requests that labelling be 1, 2, 3 etc. It is assumed that the order of the atoms is locant 1 as the first atom </summary>
							static const std::wstring NUMERIC_LABELS_VAL;

							/// <summary>
							/// InLocants have not been specified </summary>
							static const std::wstring INLOCANTS_DEFAULT;

							/// <summary>
							/// See suffixRules.dtd
							/// </summary>
							static const std::wstring SUFFIXRULES_RULE_EL;
							static const std::wstring SUFFIXRULES_VALUE_ATR;
							static const std::wstring SUFFIXRULES_SMILES_ATR;
							static const std::wstring SUFFIXRULES_LABELS_ATR;
							static const std::wstring SUFFIXRULES_FUNCTIONALIDS_ATR;
							static const std::wstring SUFFIXRULES_OUTIDS_ATR;
							static const std::wstring SUFFIXRULES_KETONELOCANT_ATR;
							static const std::wstring SUFFIXRULES_ORDER_ATR;
							static const std::wstring SUFFIXRULES_OUTVALENCY_ATR;
							static const std::wstring SUFFIXRULES_CHARGE_ATR;
							static const std::wstring SUFFIXRULES_PROTONS_ATR;
							static const std::wstring SUFFIXRULES_ELEMENT_ATR;

							/// <summary>
							/// See suffixApplicability.dtd
							/// </summary>
							static const std::wstring SUFFIXAPPLICABILITY_GROUPTYPE_EL;
							static const std::wstring SUFFIXAPPLICABILITY_SUFFIX_EL;
							static const std::wstring SUFFIXAPPLICABILITY_TYPE_ATR;
							static const std::wstring SUFFIXAPPLICABILITY_VALUE_ATR;
							static const std::wstring SUFFIXAPPLICABILITY_SUBTYPE_ATR;
						};

					}
				}
			}
		}
	}
}
