//pc3656
//hw-16 Stacks and Queues.
//Part 2

#include<iostream>
#include<vector>
using namespace std;

//Function to test part2
void vqTest();
class Vequtoreu {
    private:
        vector<int> Q;
        int startPos;
    public:
        //Simple constructor
        Vequtoreu();
        //Copy constructor
        Vequtoreu(const Vequtoreu& vq);
        //Destructor
        ~Vequtoreu() {};
        //Insert at the end.
        void insert(int n);
        //Remove from the front.
        int dequeue();
        //Return true if the Vequtoreu is empty.
        bool empty() {return Q.empty();}
        //Prints out the whole thing.
        void print();
        //Overloading << to output the queue.
        friend ostream& operator <<(ostream& outs, const Vequtoreu& vq);
};

//Simple constructor.
Vequtoreu::Vequtoreu() {
    Q.clear();
    startPos = 0;
}
//Copy constructor.
Vequtoreu::Vequtoreu(const Vequtoreu& vq) {
    Q = vq.Q;
    startPos = vq.startPos;
}

//Insert at the end.
void Vequtoreu::insert(int n) {
    if(empty()) {
        startPos = 0;
    }
    Q.push_back(n);
}

//Remove from the front.
int Vequtoreu::dequeue() {
    if(empty()) {
        cout << "Error dequeuing and empty queue" << endl;
        exit(1);
    }
    int result = Q[startPos];
    startPos ++;
    if(startPos == Q.size()) {
        Q.clear();
        startPos = 0;
    }
    return result;
}

//Print out the whole queue.
void Vequtoreu::print() {
    for(int i = startPos; i < Q.size(); i++) {
        cout << Q[i];
    }
}

//Overloading of << for the Vequtoreu class.
ostream& operator <<(ostream& outs, const Vequtoreu& vq) {
    outs << "[";
    for(int i = vq.startPos; i < vq.Q.size(); i++) {
        outs << vq.Q[i];
        if(i < vq.Q.size() - 1) {
            outs << ", ";
        }
    }
    outs << "]";
    return outs;
}

int main() {
    //vqTest();
    return 0;
}

//Testing the Vequtoreu class.
void vqTest() {
    cout << "Testing the Vequtoreu class." << endl;
    Vequtoreu vq;
    vq.insert(12);
    vq.insert(20);
    cout << vq.dequeue() << endl;
    cout << vq.dequeue() << endl;
    vq.print();
    vq.insert(20);
    vq.insert(21);
    vq.insert(22);
    vq.insert(23);
    vq.insert(24);
    vq.insert(25);
    vq.insert(26);
    cout << endl << vq << endl;
    cout << vq.dequeue() << endl;
    cout << endl << vq << endl;
}


