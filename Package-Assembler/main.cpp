#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<algorithm>
#include<map>
#include<list>

struct Package{
  int x,y,z;
  std::string text;
  friend std::ostream& operator<<(std::ostream& out, const Package& p);
};

std::ostream& operator<<(std::ostream& out, const Package& p)
{
    return out << p.x << '\t' << p.y << '\t' << p.z << '\t' << p.text;
}

void Output(std::map<int, std::list<Package>> m);

int main(int argc, char* argv[])
{
    std::string line;
    std::map<int, std::list<Package>> message;
    while(std::getline(std::cin, line))
    {
        std::stringstream in(line);
        Package p;
        in >> p.x >> p.y >> p.z;
        getline(in, p.text);
        auto iter = message.find(p.z);
        if (iter!=message.end())
            iter->second.push_back(p);
        else{
            message[p.x].push_back(p);
        }
    }
    Output(message);
}

void Output(std::map<int, std::list<Package>> m)
{
    for(auto q:m)
    {
        q.second.sort([](const Package &a, const Package &b){return a.y < b.y;});
        for(auto iter = q.second.begin(); iter != q.second.end(); iter++)
        {
            std::cout << (*iter) << '\n';
        }
    }
}
