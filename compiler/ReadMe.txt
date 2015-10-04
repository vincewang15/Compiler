Compiler
Shilai Lu
Chunyang Zhu
Botao Wang

Required Exercise:
	Design:
	Overall description of some important header and source file: 
		ASTNODE class was created in order to keep the structure information of a node. And each ASTNODE has four members as 
		node type, predicate list, child list and one token string to put parenthesis token which we also assume it to be one 
		node.
		 
		After each well-formed horn clause is recoginized, we build the abstract representation by means of recursive descent 
		parsing that is defned in ASTParser class. The result is a vector containing pointers to the root node of each well 
		formed Horn Clause.

		Table class provide methods to create a symbol table after abstract syntax tree was created. A symbol table has one 
		list of entries. In this lab, in order to maintain the information of BOUND, UNBOUND and NUMBER token, we modify the 
		member variable from last lab. An entry has members as type, string, a vector of entry, and a pointer points to BOUND
		UNBOUND and NUMBER token object. String member is used to hold name attribute of predicate. The vector of entry holds 
		entries of symbols appears. Since in this lab, only BOUND token should be added into the table, we simply keep all 
		UNBOUND variables and NUMBER constants within the symbol list of predicates entry. As each root thus has two child 
		roots among which HEAD and BODY node have the predicates we need. So we first get these two nodes from the child 
		list of root. Then since HEAD and BODY node has a predicate list to hold predicates, we iterates that list to extract 
		each predicate out. We create an entry structure which will be pushed back to the entry list of table class. So for 
		example, a predicate entry will be created as type=PREDICATE, string=greater, and the symbols will be extracted from 
		the predicate objects we previously get. 

		Unifier class provides unification algorithm, performs any substitutions needed to unify the head of the first clause 
		with a predicate in the body of the second clause, then replace the matched predicate in the copy of the second Horn 
		clause with the body of the copy of the first clause, finally it returns the vectors that contains new generated well 
		formed horn clauses. Consider in this lab, we no longer do unification between two predicate entries in the symbol table
		while manipulate on two different horn clause. Thus we changed the argumrnt to be passed into smart pointers pointing 
		to the roots of them. Moreover, the symbol table generated so far is also be passed into 'unify' function because we 
		keep the list of Horn clauses within a diferent distinct entity as vector. So it well be easier to find the value of 
		BOUND variable within the symbol table for that the BOUND tokens with the same name are different objects but there is 
		only one BOUND entry in symbol table. We keep horn clauses in another entity other than symbol table since it avoids 
		the time and writing codes to take out all predicates of one well-formed horn clause, and it's easier to access the well
		formed clause by its index. And in the 'Unifier' class, 'substitute' function is created to define new rules and call
		'replace' function to make that specific substitution for both predicates passed by. 
		 

	Main function design:
		In this lab, if the parse discovers that the Horn clause it is currently trying to parse is not well formed, upon 
		encountering such an error the program immediately start looking for the start of another well formed Horn clause. We 
		choose such way because as if we skip the rest of current line and start a new line, we may miss some well formed Horn 
		Clause in the rest of line.

		The main function acts like a while loop that is always true, so this program won't be terminated unless we input an 
		exit command. After it receives the command, it do relate execution which will be implemented in a if-else structure.
		If it receives process command, it works as the same as previous lab. 
		If an assert command is received, it simply write that horn clause into a temp file which will then be processed by 
		ASTParser class to determine whether it is a well formed horn clause. We do this is because our ASTParser calls Scanner 
		object to extract strings out of an input file, meanwhile, it classifies those strings into different token types, 
		otherwise we need to write the extraction and classification again.
		If resolve command is received, first to check if the Horn clauses involved are identical, or if two numbers were not 
		provided or if the head of the first clause cannot be unified with a predicate in the body of the second clause, the 
		program should simply output a message to that effect after re-displaying the same screen of Horn clauses. Otherwise,
		create an unifier object to unifying those two horn clauses.
		If set command is received, we do check as the requirement requires firstly, and we set the BOUND's value by accessing
		its value member by the mean of the pointer which we created in entry structure. So for example 'set x 1', first find 
		the exact entry object of that BOUND entry, set its value to 1 by ptr->val = 1. In this way we changed the Token attribute
		of that BOUND variable as well. And so the same design is applied in randomize command.  

	Implementation:
	Unifier implementation:
		Unifier class is created to do unification algorithm. which has 6 different functions as match, compare, substitute,
		replace, unify and print. At first, main function calls the unify method, Unify function calls match function to check 
		if they have the same name and number of symbols, if not, skip rest code and start unifying next two predicates. If 
		they matches, call compare function to compare their symbols. Again if compare returns false, skip rest code and start 
		unifying next two predicates. Otherwise, create two working copies and call substitue to make substitution. The working
		copies are vectors containing Token objects that are extracted from the horn clauses passed in. We choose this way is 
		because we think it is the simplest way to manipulate on unification while not change the attributes of original horn 
		clauses that have many smart pointers member. 

		Substitute function first checks whether the symbols at current position in either copies are constant, if so move to
		the next position since before any substitution, two constants must be the same. And as if it detects at least one 
		variable in either copies, an new substitution rule will be created and applied immediatelt to all positions in both 
		copies. 
		
		Replace function is so called to make specific replacement according to the rule passed by substitution function. It 
		iterates each position in both copies and see if there need to make replacement. We choose to implement	unification in 
		such a process is because as soon as we make replacements at all positions, such rule will never be applied any more, 
		and so each time we move to the next position, we don't need to iterates previous rules but just to see if a new rule 
		should be added to the substitution list. Moreover, this way, on the other hand, guarantees any unbound variable is not 
		subject to any previous substitutions during that resolution attempt, and for which the cumulative substitutions so far 
		produce the same value as the constant's value. This is because that variable is thus be substituted in all positions and
		will never appear.

		Finally, a structure of subRule is used to hold a list of substitution law and provide basic method to print out rules and 
		add rules.

	Up and down implementation:
		We realize this by calculating the index of the vector of horn clause. Upper and Lower are two indexes indicate the start 
		point and the end point to print. For example, up 3 means upper-3 and lower-3. The only focus should be put on the overflow
		and underflow. The index can not be smaller than 0 and the maximum size of vector.
	
	Test case: 
		( ( ascending 1 2 ) )
		( ( ascending X 3 ) )
		( ( ascending x Z ) ( ( ascending x Y ) ( ascending Y Z ) ) )
		( ( ascending x Z ) ( ( ascending x Y Y ) ( ascending Y Z ) ) ) )
		( ( same a a ) )                                
		( ( same 1 b ) )
		( ( same c 2 ) )                                
		( ( sametoo 1 2 ) )
		( ( sametoo 2 2 ) )             
		( ( sametoo q q ) )                                
		( ( lesser x y ) ( ( lesser x z ) ( lesser z y ) ) )  
		( ( lesser z 3 ) )                               
		Passing all the testing above. Code behaves as expected


Extra Credit:
	Design:
		For parsing conjunctive head, we could use the same code segment as body parser for multiple predicate. First, we try to parse
		out an optional predicate, if it doesn't return an empty node, it can be added into the child list of that head. The tric is to 
		peek the left parenthesis because if there are two continuous left parenthesis. that means it is the end of the head.
		
	Implementation:
		if (type != LEFTPAREN){
			prenode_optional = predicate_parser();
			while (prenode_optional != NULL){
				predlist.push_back((*prenode_optional).predlist1.front());
				child.push_back(prenode_optional);
				if (type != LEFTPAREN){
					prenode_optional = predicate_parser();
				}
				else
					prenode_optional = NULL;
			}
		}
		
	Test Case:
		 ( ( ascending x Z ) ( ascending X 3 ) ( ( ascending x Y ) ( ascending Y Z ) ) )
		 ( ( ascending x Z ) ( ascending X 3 ) ( ascending 1 2 ) ( ( ascending x Y ) ( ascending Y Z ) ) )

		 Passing the test cases, behaves as expected
