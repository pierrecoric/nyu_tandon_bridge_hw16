//pc3656
//hw-16 Stacks and Queues.

#include <iostream>
#include<fstream>
#include <vector>
using namespace std;

class Frame;
class Stack;
typedef Frame* framePtr;

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
        friend bool pascalCheck(istream& ins, Stack& s);
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

bool pascalCheck(istream& ins, Stack& s) {
    char next;
    
    while(ins >> next) {
        cout << next;
    }
    
    return false;
}

//Main function.
int main() {
    Stack s;
    framePtr f = new Frame('X');
    s.push(f);
    framePtr x = new Frame('s');
    s.push(x);
    s.print();
    s.push('p');
    s.push('e');
    cout << endl << s << endl;
    char ans('\0');
    cin >> s;
    cin >> s;
    cin >> s;
    cout << endl << s << endl;
    Stack t(s);
    cout << endl << t << endl;
    pascalCheck(cin, t);
    return 0;
}