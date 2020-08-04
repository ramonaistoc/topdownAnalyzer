#include <iostream>
#include <map>
#include <vector>
#include<fstream>
#include<string>

using namespace std;

struct GIC
{
    map<char,vector<string>> prod;
    vector<char> terminals;
    vector<char> neterminals;
};

struct stare
{
    char state;
    int wordPoz;
    vector<string> alpha;
    string beta;
};

string addElements(GIC &g, string fileName)
{
    string word;
    //terminalele, neterminalele si productiile
    int x;
    ifstream read(fileName);

    getline(read, word,'\n');   //citesc cuvantul

    read>>x;
    int nrN = x;

    read>>x;
    int nrT = x;

    char y;

    for(int i =1; i<= nrT; i++)     //adaug terminalele
    {
        read>>y;
        g.terminals.push_back(y);
    }

    for(int i =1; i<= nrN; i++)     //adaug neterminalele
    {
        read>>y;
        g.neterminals.push_back(y);
    }

    /*cout<<" T "<<endl;
    for(auto j = g.terminals.begin(); j<g.terminals.end(); ++j)
        cout<<*j;

    cout<<endl<<" N "<<endl;
    for(auto j = g.neterminals.begin(); j<g.neterminals.end(); ++j)
        cout<<*j;
    cout<<endl;*/

    string line;

    while(getline(read, line,'\n'))
    {

        int i = 0;
        vector<string> p;
        char net = line[i];
        g.prod[net] = p;
        string buff= "";
        i=1;
        while(i<line.length())
        {
            if(line[i]!= '|')
            {
                buff = buff + line[i];
                i++;
            }
            else
            {
                g.prod[net].push_back(buff);
                i = i+1;
                buff = "";
            }
        }

    }

    read.close();
    return word;
}

void regula_1(stare &s, GIC g, string w)
{
    char a = s.beta[0];
    s.beta.erase(0,1);
    s.beta = g.prod[a][0] + s.beta;

    string b(1,a);
    b = b + to_string(0);

    s.alpha.push_back(b);
    //cout<<g.prod[a][0] <<endl;
}

void regula_2(GIC g, stare &s)
{
    string firstBetaChar(1, s.beta[0]);

    s.alpha.push_back(firstBetaChar);
    s.beta.erase(0,1);
    s.wordPoz += 1;
}

void regula_3(stare &s)
{
    s.state = 'r';
}

void regula_5(GIC g, stare &s)
{
    s.wordPoz -= 1;

    string lastChar = s.alpha.back();
    s.alpha.pop_back();
    s.beta = lastChar + s.beta;
}

void regula_6(GIC g, stare &s)
{
    string lastChar = s.alpha.back();
    char netProd = lastChar[0];
    lastChar.erase(0,1);
    int x = stoi(lastChar);     // iau indexul neterminalului din alpha ca sa aflu indexul productiei;
    // daca indexul e egal cu nr prod net atunci aplic 6a


    if(x < g.prod[netProd].size()-1)
    {
        s.state = 'q';

        s.alpha.pop_back();//scot pt ca 0 sa modific productia

        string b(1,netProd);
        b = b + to_string(x+1);
        s.alpha.push_back(b);

        s.beta.erase(0,g.prod[netProd][x].length());
        s.beta = g.prod[netProd][x+1] + s.beta;
    }
    else if( netProd == 'S')
    {
        s.state = 'e';
        s.beta.erase(0, s.beta.length());
        s.beta = "$";
        // s.wordPoz =  ar trebui sa actualizez si lungimea cuvantului

    }
    else // altfel practic sterg ultima productie din beta si adaug ultimul net din alpah
    {
        s.alpha.pop_back();
        s.beta.erase(0,g.prod[netProd][x].length());
        s.beta = netProd + s.beta;
    }

}

void afisare(stare &s)
{
    cout<<"("<<s.state<<","<<s.wordPoz<<",";
    for(auto i = s.alpha.begin(); i!= s.alpha.end(); ++i)
        cout<<(*i);
    cout<<","<<s.beta<<")"<<endl;
}

int main()
{
    GIC g;
    string word, filen;

    cout<<"introduceti numele fisierului: "; cin>>filen;

    word = addElements(g,filen);

    /*     cout<<endl<<"AFISARE"<<endl;
     for(auto i = g.prod['S'].begin(); i!=g.prod['S'].end(); ++i)
     {
         cout<<" "<<(*i);

     cout<<endl;
     }*/

    stare s;
    s.state = 'q';
    s.wordPoz = 0;
    s.beta = "S$";       //S fiind starea initiala

    while(1)
    {

        afisare(s);

        if(isupper(s.beta[0]) && s.state == 'q')
        {
            regula_1(s,g,word);
            cout<<"R1"<<endl;
            continue;
        }
        if(word[s.wordPoz] == s.beta[0] && s.state == 'q')
        {
            regula_2(g,s);
            cout<<"R2"<<endl;
            continue;
        }
        if(word[s.wordPoz] != s.beta[0] && s.state == 'q')
        {
            regula_3(s);
            cout<<"R3"<<endl;
            continue;
        }
        if(s.beta[0] == '$'&& s.wordPoz == word.length())
        {
            cout<<"succes"<<endl;
            cout<<"R4"<<endl;
            break;
        }
        if(s.state == 'r' &&  s.alpha.back().length() == 1)
        {
            regula_5(g,s);
            cout<<"R5"<<endl;
            continue;
        }

        regula_6(g,s);
        cout<<"R6"<<endl;
        if(s.state == 'e')
            break;

    }

    return 0;
}
