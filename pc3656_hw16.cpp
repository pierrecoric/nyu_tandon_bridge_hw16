//pc3656
//hw-16 Stacks and Queues.

#include <iostream>
#include <vector>
using namespace std;

class Frame;
class Stack;

typedef Frame* framePtr;

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

        char getData() {return data;}
        void setData(char c) {data = c;}
        framePtr getNext() {return next;}

        Frame operator =(const Frame& other);

        friend class Stack;
};

//Copy constructor for a frame.
Frame::Frame(const Frame& f) {
    data = f.data;
    next = f.next;
}

//Overloading = operator.
Frame Frame::operator =(const Frame& other) {
    if(this != &other) {
        data = other.data;
        next = other.next;
    }
    return *this;
}

class Stack {
    private:
        framePtr top;
    public:
        //Simple constructor.
        Stack() : top(nullptr) {};
        //Copy constructor definition.
        Stack(const Stack& original);
        //Destructor.
        //~Stack() {clear();}
        //Push a new frame.
        void push(framePtr& f);
        //Push a char directly.
        void push(char c);
        framePtr pop();
        //void clear();
        void print() const;
        //Overloading <<.
        friend ostream& operator <<(ostream& outs, const Stack& s);
        friend bool pascalCheck(const Stack& s);
        friend class Frame;
};

void Stack::push(framePtr& f) {
    //Pushing on an empty stack.
    if(top == nullptr) {
        top = f;
        return;
    }
    f -> next = top;
    top = f;
}

void Stack::push(char c) {
    framePtr f = new Frame(c);
    //Pushing on an empty stack.
    if(top == nullptr) {
        top = f;
        return;
    }
    f -> next = top;
    top = f;
}



//Function to print the stack.
void Stack::print() const {
    framePtr temp = top;
    while(temp != nullptr) {
        cout << temp -> getData();
        temp = temp -> next;
    }
}

//Overloading <<.
ostream& operator <<(ostream& outs, const Stack& s) {
    framePtr temp = s.top;
    while(temp != nullptr) {
        outs << temp -> getData() << " -> ";
        temp = temp -> getNext();
    }
    return outs;
}

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
    return 0;
}