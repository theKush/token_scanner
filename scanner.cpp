//Kush Patel
//Scanner Project
//Purpose & Requirements: The purpose of this program is to tokeninze the input string from a text file. The program tokenizes the longest possible string possible. 
//                        If the input consists of any non-token string, the program should return "error"
//						  The program should should output if the token is a id, number, plus, minus, times, divide, assignment, left-parentheses, right-parenthese.
//						  The program ignores any comments in the input file. Also, program should not consider "read" and "write" as an id.

#include <iostream>
#include <stack>
#include <string>
#include <fstream>
#include <iomanip>
using namespace std;
void Break_Up_Into_Stack(string s, stack<char> &theStack);

//enumerate all the states used in the program
enum state {FRESH, ASSIGN, PLUS, MINUS, TIMES, DIV, LPAREN, RPAREN,  ID, NUM, COMMENT, CLOSECOMMENT, COLON};


//takes the string created from the input file and puts each character of this string into a stack of characters.
void Break_Up_Into_Stack(string s, stack<char> &theStack)
{
    while (!theStack.empty())   {theStack.pop();} // empty the stack before use.

    for ( int pos = s.length() - 1; pos >= 0; pos--) // go through each character of string and push it onto a stack.
       theStack.push(s.at(pos));
}

//checks if the character c is a letter. Returns true if it is. Else, returns false.
//ASCII code for capital and small letters used to check.
bool isId(char c){
	int i = (int) c;
	if (i>=65 && i<=90){return true;}
	if(i>=97 && i<=122){return true;}
	return false;
}

//checks if the character c is a number from [0-9]. Returns true if it is. Else, returns false.
//ASCII code for numbers from [0-9] used.
bool isNum(char c){
	int i = (int) c;
	if(i>= 48 && i<=57){return true;}
	return false;
}

//checks if the character c is any kind of white space[tab, line feed, new line, carriage return, space]. Returns true if it is. Else, returns false.
//ASCII code for different white spaces used to check.
bool isWhiteSpace(char c){
	int i = (int) c;
	if(i == 9 || i == 10 || i == 13 || i == 32)
	{return true;}
	return false;
}

//determines if the next character c can be pushed onto the current character(state A of the character) to make the longest string possible. 
bool can_push(state A, char c)
{
	//anything can be pushed on the fresh state.
	if ((A == FRESH) && (isId(c) || isNum(c) || (c == '+') || (c == '-') || (c == '*') || (c == '/') || (c == '(') || (c == ')') || (c == ':')))
		{return true;}	
	//only numbers can be pushed onto a number to make the longest string.
	else if((A == NUM) && (isNum(c)))
		{return true;}
	//any number or letter can be pushed on to an id, to make the longest id string.
	else if (A == ID && (isId(c) || isNum(c) || ((int)c == 95)))
		{return true;}
	//assignment is formed by pushing and equal sign on to a colon.
	else if ((A == COLON) && (c =='='))
		{return true;}
	//to create closing comment tag you have to push a divide sign on to a multiplication sign.
	else if ((A == TIMES) && (c == '/'))
		{return true;}
	//to create opening comment tag you have to push a multiplication sign on to a divide sign.
	else if ((A == DIV) && (c == '*'))
		{return true;}
		
	return false; // return false if the character c cannot be pushed on to the current state.
}

//The state of the current string changes if you push anything on it. This function determines the new state of the string.
state new_state(state A, char c)
{
	//when a new character is pushed on to fresh state. its state changes to that characters states.
	if ((A == FRESH) && can_push(A, c)){
		if (isId(c)){return ID;}			//if c is a letter change state to ID
		else if (isNum(c)){return NUM;}		//if c is a number change state to Number
		else if (c == '+'){return PLUS;}	//if c is a plus sign, change state to plus
		else if (c == '*'){return TIMES;}	//if c is a multiplication sign, change state to times
		else if (c == '/'){return DIV;}		//if c is a divide sign, change state to divide.
		else if (c == '-'){return MINUS;}	//if c is a minus sign, change state to minus.
		else if (c == '('){return LPAREN;}	//if c is a left-parentheses, change state to left-parenttheses.
		else if (c == ')'){return RPAREN;}	//if c is a right-parenthese, change state to righ-parentheses.
		else if (c == ':'){return COLON;}	//if c is a colon, change state to colon. 
	}
	//if current state is colon. and c is and equal sign. change state to Assign.
	else if ((A == COLON) && can_push(A,c)){return ASSIGN;}
	//if current state is divide, and c is multiplication change state to comment.
	else if ((A == DIV) && can_push(A,c)){return CLOSECOMMENT;}
	//if current state is a number, and c is a number, you remain in number state.
	else if ((A == NUM) && can_push(A,c)){return NUM;}
	//if current stat is a id, and c is a number or id, you remain in id state.
	else if ((A == ID) && can_push(A,c)){return ID;}
	//if current state is multiplacation, and c is divide change state to comment.
	else if ((A == TIMES) && can_push(A,c)){return COMMENT;}
}

//check if the longest token created is valid or not. It is only valid when 'A' is one of the following states.
bool valid_token(state A){
	if(A == FRESH){return true;}
	else if(A == ID){return true;}
	else if(A == NUM){return true;}
	else if(A == ASSIGN){return true;}
	else if(A == TIMES){return true;}
	else if(A == DIV){return true;}
	else if(A == PLUS){return true;}
	else if(A == MINUS){return true;}
	else if(A == LPAREN){return true;}
	else if(A == RPAREN){return true;}
	else if(A == COMMENT){return true;}
	else if(A == CLOSECOMMENT){return true;}
	return false;
}

//go through each character of the input stack s and create longest possible string.
//the function returns a stack of string that has final states of the longest strings.
stack<string> scanner(stack<char> s)
{
	stack<string> t;		//final stack
	stack<string> error;	//error stack just to return error when no token is match
	stack<char> p;			//stack of character that holds all characters that create a longest string
	state A = FRESH;		//initial state is fresh
	while (true){
		if(s.empty()){return t;}	//return final stack t, when the input stack s is empty
		//remove any whitespaces from the input stack
		while(isWhiteSpace(s.top())){
			s.pop();
			if(s.empty()){return t;}	
		}
		//check the character at the top of the input stack s.
		if(isId(s.top()) || isNum(s.top()) || (s.top() == '+') || (s.top() == '-') || (s.top() == '*') || (s.top() == '/') || (s.top() == '(') || (s.top() == ')') || (s.top() == ':')){
			//empty out stack p before you begin.
			while(!p.empty()){
				p.pop();
				A = FRESH;
			}
			//if you can push the top character of s. keep pushing the top character of s into p until longest valid token is created.
			while(can_push(A,s.top())){
				p.push(s.top());
				A = new_state(A,s.top());
				s.pop();
				if(s.empty()) break;
			}
			//check if the current state of the token is a valid token and push it onto the final stack t.
			if (valid_token(A)){
				if (A == ASSIGN){t.push("assign");}
				else if (A == NUM){t.push("number");}
				else if (A == ID)
				{
					//to check if the string of ID matches with "write"
					stack<char> write;
					write.push('w'); write.push('r'); write.push('i'); write.push('t'); write.push('e'); 
					//to check if the string of ID matches with "read"
					stack<char> read;
					read.push('r'); read.push('e'); read.push('a'); read.push('d'); 
					//if "read" is matched. push "read" onto final stack t.
					if (read == p){t.push("read");}
					//if "write" is matched. push "write" onto final stack t.
					else if (write == p){t.push("write");}
					//the longest string is "id" if it is not "read" or "write"
					else {t.push("id");}
				}
				else if (A == LPAREN){t.push("left-parentheses");}
				else if (A == RPAREN){t.push("right-parentheses");}
				else if (A == PLUS){t.push("plus");}
				else if (A == MINUS){t.push("minus");}
				else if (A == TIMES){t.push("times");}
				else if (A == DIV){t.push("divide");}
				else if (A == COMMENT){t.push("comment");}
				else if (A == CLOSECOMMENT){t.push("closecomment");}
			}
			//return error if there is a non-valid token in the input and break out of the loop
			else{
				cout<<"error"<<endl;
				return error;
				break;
			}
		}
		//return error if there is a non-valid token in the input and break out of the loop
		else{
			cout<<"error"<<endl;
			return error;
			break;
		}	
	}
}

//print the final stack of strings.
void print_stack(stack<string> s)
{
	cout << "( ";
	while(!s.empty()){
		cout << s.top() << ", ";
		s.pop();
	}
	cout << ")" << endl << endl;

}

//remove any comments found in the final stack.
stack<string> final_stack(stack<string> s){
	stack<string> finalStack;
	while(!s.empty()){
		if(s.top() == "comment"){
			s.pop();
			while (s.top() != "closecomment"){
				s.pop();
			}
			s.pop();
		}else{
			finalStack.push(s.top());
			s.pop();
		}
	}
	return finalStack;
}

//main function of the program reads in the input text file and prints the final stack
int main(int argc, char *argv[])
{
	string aLine;						//each line of the input file
	string allLines;					//final string created from the input file
	stack<char> charStack;				//stack of input characters 
	stack<string> tokens;				//final stack 
	
	string inputfile;
	if ( argc != 2 ) // argc should be 2 for correct execution
    // We print argv[0] assuming it is the program name
	{cout<<"usage: "<< argv[0] <<" <filename>\n";}
	else{inputfile = argv[1];}
	
	
	ifstream testFile;
	testFile.open(inputfile);			//open the text file
	if (testFile.is_open())
	{
		while (testFile.good())			//read through the end of the text file.
		{
			getline(testFile,aLine);	//read each line of the text file
			while(testFile){
				allLines = allLines + " " + aLine;	// combine each line of test file into one single string.
				getline(testFile,aLine);
				
			}	
			cout << endl;
			Break_Up_Into_Stack(allLines, charStack);		//break that input string into a stack of characters.
			
			//if stack of characters is not empty perform the scanner function and print the final stack
			if(!charStack.empty())
			{
				tokens = scanner(charStack);	//perform the scanner function on the stack of characters
				tokens = final_stack(tokens);	//remove any comments from the final stack
				print_stack(tokens);			//print the final stack
			}
		}
		testFile.close();						//close the text file
	}
	else cout << "Unable to open file" << endl;	//if text file not found

}



