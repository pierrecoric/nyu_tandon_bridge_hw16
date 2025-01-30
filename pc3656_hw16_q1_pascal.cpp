//pc3656
//hw-16 Stacks and Queues.
//Part 1

#include <iostream>
#include<fstream>
#include <string>
using namespace std;

//Class definitions.
class Frame;
class Stack;
typedef Frame* framePtr;

//Prototypes of helpers functions.
bool openBracket(char c);
bool closeBracket(char c);
bool validPair(char a, char b);
bool isWhiteSpace(char c);
void printPascal();
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
    if(f.next != nullptr) {
        next = new Frame(*f.next);
    }
    else {
        next = nullptr;
    }
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
        framePtr top;
        int size;
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
        Frame pop();
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
        size ++;

        //Copy the rest.
        framePtr nextOne = top;
        oldPtr = oldPtr -> next;

        while(oldPtr != nullptr) {
            framePtr newOne = new Frame(oldPtr -> data);
            newOne -> next = nullptr;
            nextOne -> next = newOne;
            size ++;

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
Frame Stack::pop() {
    if(empty()) {
        cout << "Error: popping from an empty stack." << endl;
        exit(1);
    }
    //Create a new frame to return the result.
    Frame result(top -> data);
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
    Frame compare;

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
                compare = brackets.pop();
                //If poping brackets result in an empty stack, it might be the beginning of the end.
                if(brackets.empty()) {
                    end.clear();
                }
                if(!validPair(compare.getData(), next)) {
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
    ifstream file;
    cout << "Pascal validation program." << endl;
    cout << "Please enter the name of the file that you wish to validate: ";
    string filename = getString();
    //Check that the file exists.
    cout << endl;
    file.open(filename);
    if(pascalCheck(file)) {
        printPascal();
        cout << endl;
        cout << filename << " is valid." << endl;
        cout << "Pascal is proud of you." << endl;
    }
    file.close();

    //Function to test the part 2.
    //vqTest();
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

//Return true if the character is a whitespace.
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

//Pascal is proud of you.
void printPascal() {
    cout 	<< "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$B%W&*bQLXu0qqq0moB$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" << endl
	<< "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$BbUj11[[ili!i_+ii+<i+>i!~(zo@$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" << endl
	<< "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$Mw)<!l>!i[1{_}}r|xf[ct[+!({{[+ii_rzmM$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" << endl
	<< "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$#Q|(crt{~|0dQUnn|cczv(1ft]i!il_-_][{|ruzq*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" << endl
	<< "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$Bwu]-}dU1u}Zo&pQUJruYULmCnzj[+_)cQZmXwQQJm0QdM$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" << endl
	<< "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$B&hUjt)]i_jz}jQZ%8@*ZJC00JLJqmLJj](ftQmkdCQmZOdkbha$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" << endl
	<< "$$$$$$$$$$$$$$$$$$$$$$$$$$$$oh)j]-[[!iil<_-Oba&%%$$&ohOmZJcZhOUzv|)JaZYU}tLnZCpOwo$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" << endl
	<< "$$$$$$$$$$$$$$$$$$$$$$$$$$$%1n+!ii<<>ll_)nqM8@$$$$$$$$$@*hOLmqmJCwvCnunz}t}[{trjmk$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" << endl
	<< "$$$$$$$$$$$$$$$$$$$$$$$$$$$Z+|!ill_{1}cm#BB@$$$$$$$$$$$$$$*MhhvJvXt{~t(|zrj}1|f~XYhW$$$$$$$$$$$$$$$$$$$$$$$$$$$$" << endl
	<< "$$$$$$$$$$$$$$$$$$$$$$$$B*ob||lll)rjuQbMB$$$$$$$$$$$$$$$$$$B*hYfn|tj-][){xZbQrnzzJLqO@$$$$$$$$$$$$$$$$$$$$$$$$$$" << endl
	<< "$$$$$$$$$$$$$$$$$$$$$$$WL(c()u}l}Jucwao8B@$$$$$$@@$$B$$$$$$$$Mo0[]]|nj||vvjJdZJmvJLdOJ$$$$$$$$$$$$$$$$$$$$$$$$$$" << endl
	<< "$$$$$$$$$$$$$$$$$$$$$$$Z<QamCn_<COJpook#8$$$$$$$$$$$$$$$$$$$$$@&oz<--~)1((utX1jntCCmCU%$$$$$$$$$$$$$$$$$$$$$$$$$" << endl
	<< "$$$$$$$$$$$$$$$$$$$$$$$0uv|rj|!]}xcXQaqLM@$8@$$@WMbka&$$$$$$$$%omc1~~~+~+rj]-]~+}+|QOOa$$$$$$$$$$$$$$$$$$$$$$$$$" << endl
	<< "$$$$$$$$$$$$$$$$$$$$$$$*cv-}}li_->~il!(n&@@oMact]-~~~+}U#$$$$%MqYJr{]l1[-|U0Cn|1n}|tJuh$$$$$$$$$$$$$$$$$$$$$$$$$" << endl
	<< "$$$$$$$$$$$$$$$$$$$$$$$d]]!}l!}t(_~>lli1a@@%k|1]-[[_)nCCnJo$@bCucfj1~i<-{fpbwzjjvx|])Jd$$$$$$$$$$$$$$$$$$$$$$$$$" << endl
	<< "$$$$$$$$$$$$$$$$$$$$$$$n[!~>!!>~1|zx(<l-#B@$dx+(vYYwUftZM#Ob@oObJj[+>lli>cdhdUcXnf)(+|Jq@$$$$$$$$$$$$$$$$$$$$$$$" << endl
	<< "$$$$$$$$$$$$$$$$$$$$$$$rf_<>ll>>>r-l<ii)8$@8dcnr)|rnJdqzup8@$$@mLYut]i!>icbbhcvXXu(c>_XLYa$$$$$$$$$$$$$$$$$$$$$$" << endl
	<< "$$$$$$$$$$$$$$$$$$$$$$$a1~ll!l]_fQv}|_fb$B%8&#tnOhz{_<jJo*M@$$@8YJCur_<~}umhZn><[{f}}tcXXfB$$$$$$$BB@$B$$$$$$$$$" << endl
	<< "$$$$$$$$$$$$$$$$$$$$$$$@d{lll>)j|cLv[ch$$@%@B%OLQOwUcCqk%@$$$$pmYZYcr~1{f0pzt>i>(fncrjnLO0k$$$$$$$B%@$$$$$$$$$$$" << endl
	<< "$$$$$$$$$$$$$$$$$$$$%hM#adZu!_ubmOQ}|d@@$@@$$$@MbdOZmb8@$$$$$$hmfCzv}+{>{1nr[i>}jXOU(_fwOb$$$$$B8B@B$$$$$$$$$$$$" << endl
	<< "$$$$$$$$$$$$$$$$$$$$$apM**o*)]YM88C]m%$$$$$$$$$$$B#8B@$$$$$$$$8dUcxr[}__~jft<_~[cCQJi]cxXUOb#W%&8B@$$BB$$@$$$$$$" << endl
	<< "$$$$$$@B%@B$$@$$@$$$$hXcwQJO}<rmpv}1*$$$@@@@@$$$$$$$$$$$$$$$$$$pmx_]|[<-f|1-<+_xCmzc]-{}1fu()uwWB%%@B8B8B$$$$$$$" << endl
	<< "$$$$$$$@%$B@8B%%@$@$$#0rvcvz]l_ff]>c8$$$$$$$W@%@$$$$$$$$$$$$@%&#X1-|[1<-)|}[+}j(fYv<!~>li[tfrxxZhMW&*&8M%8@B$$$$" << endl
	<< "$$$$$$&8BB@B&&B&%@$%W%Mcvnft>ll})<-vUJOUf{|Yp8$B8BBB$$$%@$B&@%%av({)n|x|vv]+>-1r+~{+!<[>1t[]}jzckh8@&#%WW8@@$$$$" << endl
	<< "$$$$@8W%&8&W&#8*WMMda#huCjr<lll~t-_[_~_}vh8@$@B@@BB&%B$B@@%B%B&pu|}|j([~!l~!>>__>i~~l!1}+>_(u]{zwh@W8W%MM8&%@$$$" << endl
	<< "$$$@W@%WB8&&M#8#8M8W8h#vJcX-lil-)f}]}tvuh8$$$$&*W%$@@$$$$$$@$$8ac)[{(1|~!i_i~~il~!lll]>!_}xj(}nOw**###%88MM8&@$$" << endl
	<< "$$$@&8%8M&B%8W#W#MaM*oodwxf[_i!<ju__~~tu{|cOoqu}r0k&B@$$$$$@@@#0xf{{rf[>!!l!!!!l!llli!~}fnnt<~]jCmmdM8&#&&8M%8$$" << endl
	<< "$$$%&&#&#8M%&888**o*od*dwJn>{i><1ntj[f)fQo*QJXa8hZLm#B@$$$$@%*Czxt(ru]<_lll>>}+i>>>i[_-]|-1-!ili{j0dpooW*W8%&@$$" << endl
	<< "$$$B&8#M8W8M8%8M8hM*#aabOQ)_((+!!-f1|fj|xJuO*B%B$@8&%BB@@B8%oLuurf|_<]r(|_-+i_1}t}ii)]]_}ll>+}_>)1nwZoWMMW&8WW$$" << endl
	<< "$$$$8W8#&&%MW&W#%M8M*W#ahant1>lli!_}{rXnjj{jXJJbo&%%%%&MobmJux({-|uUYzLmCt|><>>~+<}>!i<!lliill!!->+1zOhMWBBM%&$$" << endl
	<< "$$$B8MW88BB8#*WM#oWMaM*hwQXi!i>|->i]tuLbM8B@B%*qhhpkwQJcrftu((uCwLQ0qbZvcJ)ll!--+li>!liilli<i_<i+-i~{rqh##&%oB$$" << endl
	<< "$$$8MW&MM8W#Mo##WMoWaooa*oU){_{f(-i_1{rq#B@@@%#qJvrrft(1||_[umqqdhkbbbdWmJn[-+]-ii]+>i!!!!i!!ili_iii_rqb*#W*88$$" << endl
	<< "$$$B%M#%W88%WMM&o8*a#**oohbj(__-[-ll~1|t(vXUzcjfntf|1}(_(uOhhaoobqbbo#dZQJXXxcu[j)_->i!i_|]~!l!i<}1}uuw**WM&MB$$" << endl
	<< "$$B@WW#&&%8&MW#&##WaM&**ohqii<~i]1]!ll_{}1}([{}}(fx|jXUhM&M#*W#bphW#qQb%8aYufvJjt{<!<i<i]~<!!>!<~][ffrzdkaW%MM8$" << endl
	<< "$$B8B%W&8*8###8#Moaoaao#adzi>l~>~+lll_[[jx1|tjrrvXqhM#M88B&M8kk*M%ok#W$%#qUc0u_llllll>>i!~]_l<+-}+<-|xOLmdMaM8@$" << endl
	<< "$$$$8%&%MM&W&MW#o*odha*o0ti<<liil!!>{1fcJj{}|jfCk#%@%%8B@8M&kMM@Wa#WMWWhmOqm+l!!llllllil<-}_l>ii_rvvxnrcLpaMMM8$" << endl
	<< "$$@%%&%&&8%8&W*obkmUuUXt]~!il!!lll+xfnvvtt(xXcbB$$$@B%@B%&MhW$$&%%%B&Mbmphd>>!lllll!i!l![>>>ll>_[|-(}tjJUmbo&WW$" << endl
	<< "$$8B%@%$$@$&bJXu|]_}[[t_[>~lllll!}xczUU1tzLwq8$$$$$$@B@B%#o8$$$BBB%8WddhaX~!l!llllll+<>l<><>-+i<+i!!>(cdbba#**@$" << endl
	<< "$$$$$$$$$&qY1_!-<-]>>[z]!!lllll<|(uvXf|rc0q&$$$$$$$$@@&8W*B$$$$B@@B8pYf-i]<!lllllll<rr{]~<<--{-~>!!>_~-}wh*Wp88$" << endl
	<< "$$$$$$@B&Y|i_ll[1]!!]_i!!lll!l(cccnzfxcXm8@$$$$$$$$$$8B*M@$$$$8%@%Q_!!i-i>!!lllllli(<>~||tjuzcrr|11-}[{_fd*d#h&$" << endl
	<< "$$$$$B&Mzx1}}li){~]__lii!lll<fXYYUn(cYUb@$$$$$$$$$@$@8@%$$$$@o&Bq-l!<>>i>>!!lllll<+i}fcuczUjYYJLmuurujtf{Xqop#*$" << endl
	<< "$$$88@Mh>x>[~i~{~[~!lii~li+-nvcXcX)jzJkBB@$$$$$$@B&o*8@$$$$%*@&fl>~i>!i<!i!llll<[<)vXuunC000wqOLJCCJXXYUjxXhMh#$" << endl
	<< "$$B88%BU>r<[}+~)>-l!!~!>_-(jzcuYj(j|1xJwM%@$@$@8&ok*&$$$$%BMB0+i(~!~><>i!!li!_{}-rcYQLCOYJmwwwwLXQYqCYvcrjucwkh$" << endl
	<< "$$*@&%&ni}i~f<>]!l!_~<]_|[j]{_>!>[~~1<~<(QaMWaakoaW@$$$$B*#*)>[{>>ii~>>-il!_{[+|rxXYcvYUQCJU0mpcJuULmmLmzJUJcJq8" << endl
	<< "$$B8%%%}!!ii(i>i~i>[i<~~)!!!>!l-(vtvn1f1>!(MLbqa#B$$$$$8h*Y>-(<<><~<>+<l!_}[[]-(uXvcuLC0XUL0CJJJLLUzXQmpOCLuJvLo" << endl
	<< "$$$$BWQ!ll!<-i~~_<_>i~<~~>!ll!-[zwwrfXZQC]~C*C*%$@$$$%M#Z-i}n{<>i_~~++ii){1)--{cXJXYLm0JLCUXCJOLunCUuXJOqOQUL0Lh" << endl
	<< "$$$$Bb}i!>l!{->]_~[+~~~>>>!!><1fUbzXOXbhqO}_%q*$@B&#ohL|_~fvn--i~~>>>!~t>{__[1{cQJXXLLC0O0XCUUYXtczCXYXUC0OhdQZh" << endl
	<< "$$$8ht}-!!l!])~i~_>+~+<<>l!!>i()((|XnYXLhwc+nBLq*abY1[j|[vr(-<>~><~!l)j++~-~~1xwZJzJLCLZQY0LQ00QZCCJ0zJUbmQqbhaa" << endl
	<< "$$8dc{]l!ill![}}li_>>}]!l>i>!li<{+-[t_f(xYJ{<v&kz[>~vX(+~[[-<>i<<<ii{]~-<-~i_zvUXCJQJCOLJmQU0CJzULUYXXzYJC00daM#" << endl
	<< "$$$Uu]]l+<>!!l~~}il<~>i!]_ii!lll!l!>+]((j-~~-<|_l<+1|[+~~+~<~<~!~i_{>!+<+ii<rrYCLJCCJX0U0U0O0Zwm0JOQCQQOZz0wZZw*" << endl
	<< "$$%zz-~!<+i!!illi~liili_[~iii>>_!lll!l!+>><tXXtli[[}+_<+~~><<>ii>](!!<!!!l~jcQXLZwQpJCOQOwqwbqqZwdm0Omqwdmwbbboh" << endl
	<< "$$ovU}>il+<i><!!l!i<l>}~<]n[-~-<!!l>!l!l!}YJtxcli_}~__<+~<<><>!i(+!i<illl>tncXuYYQQOLw0wZmp00wZdqqwQ00OmmXLmwdoo" << endl
	<< "$$dtr<<!ll~-lll!ll!>-i-<-~ll<+~>i!>>!!l]zurt1(|i!--_~-i~->!>i!_x<i~_<iill|uczxJnXUCU0qwhpqbpbdkbpwOOpOwpppXzXYda" << endl
	<< "$$0{(<i>!lli>~!!i!li_}<+_<!+~!!ii+>!!itv{[~-1)[_i+_>~]<i<i!!_{t>+{][<l!l}ncYXcx-xXYLJLm0qpQOmqdq0qdbpqmqo*bZXzO*" << endl
	<< "$$Yt><!!!lll!i_>!!!!ll[_]ii>i<{(]il!)zX])]i!~}~ii>i-~j!!!_(x{>i_[1_!i>l!)frzX}+[UOZwmOppwmOdQOwwbkpdbbpZqho*wcXh" << endl
	<< "$$|><!l>llll!l!~<i!il!ll~+_[1rf}>![nJCJr_<<+>]-_<!!!r]<<{|-i!i~t[<!~-!l<(zCJu)}|OZmqwwwpwZwppbbhwqqpaoobmZdaMLJk" << endl;
}