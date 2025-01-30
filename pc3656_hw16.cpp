//pc3656
//hw-16 Stacks and Queues.

#include <iostream>
#include<fstream>
#include <vector>
#include <string>
using namespace std;

class Frame;
class Stack;
typedef Frame* framePtr;

bool openBracket(char c);
bool closeBracket(char c);
bool validPair(char a, char b);
bool isWhiteSpace(char c);
string getString();

//Frame class.
class Frame {
    private:
        char data;
        framePtr next;
    public:
        //Simple constructor.
        Frame() : data('\0'), next(nullptr) {};
        //Constructor that takes a char parameter.
        Frame(char c) : data(c), next(nullptr) {};
        //Copy constructor.
        Frame(const Frame& f);
        //Destructor
        ~Frame(){};
        //Accessors and Mutators.
        char getData() {return data;}
        void setData(char c) {data = c;}
        framePtr getNext() {return next;}
        //Overloading the = operator.
        Frame operator =(const Frame& other);
        friend class Stack;
};

//Copy constructor for a frame.
Frame::Frame(const Frame& f) {
    data = f.data;
    next = f.next;
}

//Overloading = operator for the Frame class..
Frame Frame::operator =(const Frame& other) {
    if(this != &other) {
        data = other.data;
        next = other.next;
    }
    return *this;
}

//Stack class
class Stack {
    private:
        int size;
        framePtr top;
    public:
        //Simple constructor.
        Stack() : top(nullptr), size(0) {};
        //Copy constructor definition.
        Stack(const Stack& original);
        //Destructor.
        ~Stack() {clear();}
        void clear();
        bool empty() const {return size == 0;}
        //Push a new frame.
        void push(framePtr& f);
        //Push a char directly.
        void push(char c);
        framePtr pop();
        void print() const;
        //Overloading <<.
        friend ostream& operator <<(ostream& outs, const Stack& s);
        friend istream& operator >>(istream& ins, Stack& s);
        friend bool stringStackCompare(string s, const Stack& src);
        friend bool pascalCheck(ifstream& ins);
        friend class Frame;
};

//Copy constructor.
Stack::Stack(const Stack& original) {
    //Initialization.
    top = nullptr;
    size = 0;
    //Make sure the original is not empty.
    if(!original.empty()){
        //Copy the top frame.
        framePtr oldPtr = original.top;
        top = new Frame(oldPtr->data);
        top -> next = nullptr;

        //Copy the rest.
        framePtr nextOne = top;
        oldPtr = oldPtr -> next;

        while(oldPtr != nullptr) {
            framePtr newOne = new Frame(oldPtr -> data);
            newOne -> next = nullptr;
            nextOne -> next = newOne;

            //Move to the next node.
            nextOne = nextOne -> next;
            oldPtr = oldPtr -> next;
        }
    }
}

//Clear function.
void Stack::clear() {
    while(top != nullptr) {
        framePtr temp;
        temp = top;
        top = top -> next;
        delete temp;
    }
}

//Pushing a new frame.
void Stack::push(framePtr& f) {
    //Pushing on an empty stack.
    if(top == nullptr) {
        top = f;
        size ++;
        return;
    }
    f -> next = top;
    top = f;
    size ++;
}

//Pushing a character.
void Stack::push(char c) {
    framePtr f = new Frame(c);
    //Pushing on an empty stack.
    if(top == nullptr) {
        top = f;
        size ++;
        return;
    }
    f -> next = top;
    top = f;
    size ++;
}

//Poping from the stack.
framePtr Stack::pop() {
    if(empty()) {
        cout << "Error: popping from an empty stack." << endl;
        exit(1);
    }
    //Create a new frame to return the result.
    framePtr result = new Frame(top -> data);
    //Create a temporary frame to delete the top frame.
    framePtr temp;
    temp = top;
    top = top -> next;
    delete temp;
    size --;
    return result;
}

//Function to print the stack.
void Stack::print() const {
    framePtr temp = top;
    while(temp != nullptr) {
        cout << temp -> getData();
        temp = temp -> next;
    }
}

//Overloading << for the Stack class.
ostream& operator <<(ostream& outs, const Stack& s) {
    framePtr temp = s.top;
    while(temp != nullptr) {
        outs << temp -> getData() << " -> ";
        temp = temp -> getNext();
    }
    return outs;
}

//Overloading >> for the Stack class.
istream& operator >>(istream& ins, Stack& s) {
    char c;
    ins >> c;
    s.push(c);
    return ins;
}

//Return true if the input file starts with begin, ends with end, and is made out of valid series of brackets.
//If returning false, it tells the user which mistake it encountered.
//It prints out the file as it reads it until encountering a mistake, so the user can locate it easily.
bool pascalCheck(ifstream& ins) {
    char next;
    bool recordBegin(true);
    Stack brackets, begin, end;

    while(ins.get(next)) {
        //Start by checking that the begining is correct.
        cout << next;
        if(recordBegin) {
            begin.push(next);
        }
        if(isWhiteSpace(next) || closeBracket(next) || openBracket(next)) {
            recordBegin = false;
            if (!stringStackCompare("begin", begin)) {
                cout << endl << "Invalid. The file does not begin with \"begin\"." << endl;
                return false;
            }
        }
        //If Closing symbol.
        if(closeBracket(next)) {
            if(brackets.empty()) {
                cout << endl << "Invalid: bracket closing on nothing." << endl;
                return false;
            }
            else {
                framePtr compare = new Frame;
                compare = brackets.pop();
                //If poping brackets result in an empty stack, it might be the beginning of the end.
                if(brackets.empty()) {
                    end.clear();
                }
                if(!validPair(compare -> getData(), next)) {
                    cout << endl << "Invalid: wrong bracket pair." << endl;
                    return false;
                }
            }
        }
        else if(openBracket(next)) {
            brackets.push(next);
        }
        //When we are out of brackets, listen for the end.
        if(brackets.empty()) {
            end.push(next);
        }
    }

    //If we are done reading and the brackets stack is empty:
    if(!brackets.empty()) {
        cout << endl << "Invalid: open bracket(s) left unclosed" << endl;
        return false;
    }
    else {
        //If the last word is not end
        if (!stringStackCompare("end", end)) {
            cout << endl << "Invalid. The file does not end with \"end\"." << endl;
            return false;
        }
        else return true;
    }
}

//Main function.
int main() {
    Stack s;
    ifstream file;
    cout << "Pascal validation program." << endl;
    cout << "Please enter the name of the file that you wish to validate: ";
    string filename = getString();
    //Check that the file exists.
    cout << endl;
    file.open(filename);
    if(pascalCheck(file)) {
        cout << filename << " is valid." << endl;
    }
    file.close();
    return 0;
}

//Return true if the variable is an opening bracket.
bool openBracket(char c) {
    string valid = "({[";
    for(int i = 0; i < valid.size(); i++) {
        if(c == valid[i]) {
            return true;
        }
    }
    return false;
}

//Return true if variable is a closing bracket.
bool closeBracket(char c) {
    string valid = "]})";
    for(int i = 0; i < valid.size(); i++) {
        if(c == valid[i]) {
            return true;
        }
    }
    return false;
}

//Retur true if the parameters are matching brackets.
bool validPair(char a, char b) {
    if(a == '{' && b == '}') {
        return true;
    }
    else if(a == '[' && b == ']') {
        return true;
    }
    else if(a == '(' && b == ')') {
        return true;
    } else return false;
}

//Return true if the end of the stack matches the string.
bool stringStackCompare(string s, const Stack& src) {
    int stringPosition = s.size() - 1;
    bool start(false);
    bool doneReading(false);
    //reading the stack.
    framePtr current = new Frame;
    current = src.top;

    //Going down the stack.
    while(current != nullptr) {
        char currentChar = current -> getData();
        if(!isWhiteSpace(currentChar)) {
            start = true;
        }
        if(start) {
            //When we have reached the end of the string.
            if(doneReading) {
                if(isWhiteSpace(currentChar)) {
                    return true;
                } else return false;
            }
            //if there is a mismatch.
            if (currentChar != s[stringPosition]) {
                return false;
            }
            if(stringPosition > 0) {
                stringPosition --;
            } else doneReading = true;
        }
        //Move to the next frame.
        current = current -> getNext();
    }
    if(stringPosition == 0) {
        return true;
    } else return false;
}

//Return true of the character is a whitespace.
bool isWhiteSpace(char c) {
    if(c == ' ') {
        return true;
    } 
    else if(c == '\n') {
        return true;
    }
    else if(c == '\t') {
        return true;
    } else return false;
}

//Helper function to get a string from the user.
string getString() {
    string s;
    char ans = '\0';
    while(ans != '\n') {
        cin.get(ans);
        if(ans != '\n') {
            s += ans;
        }
    }
    return s;
}