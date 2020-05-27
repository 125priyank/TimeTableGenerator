#include <iostream>
#include <map>
#include <vector>
#include <algorithm>    
#include<tuple>
#include <queue>

using namespace std;

std::vector<string> vec () {
  std::vector<string> v;
  return v;
}
std::vector<int> vec_int () {
  std::vector<int> v;
  return v;
}
map<string, tuple<string, string, int, int> > getCourse (vector<string> v0, vector<string> v1, vector<string> v2, vector<int> v3, vector<int> v4) {
  map<string, tuple<string, string, int, int> > v ;
  for(int i=0; i < v0.size(); i++)
    v[v1[i]] = make_tuple(v2[i], v0[i], v3[i], v4[i]);
  return v;
}

map<string, string> getTeacher(vector<string> v1, vector<string> v2)
{
  map<string, string> result;
  for(int i=0; i < v1.size(); i++)
    result[v1[i]] = v2[i];
  return result;
}
vector<string> getLabQueue(map<string, tuple<string, string, int, int> > courseCode)
{
  /*
    Returns Lab code as round robin approach vector string

  */
  map<string, queue<string>> hsh;
  for(auto u:courseCode)
  {
      if (get<0>(u.second).find("lab") != -1) {
          hsh[get<1>(u.second)].push(u.first);
      }
  }
  vector<string> result;
  int i=0, l = hsh.size(), j=1;
  auto it = hsh.begin();
  while(i < l)
  {   
      if(it == hsh.end())
        it = hsh.begin();
      if(it->second.empty() == false)
      {
        result.push_back(it->second.front());
        it->second.pop();
      }
      else
        i++;
      it++;
      j++;
  }
  return result;
}

vector<string> getSubjectQueue(map<string, tuple<string, string, int, int> > courseCode)
{
  /*
    Returns unique brnches

  */
  map<string, tuple<string, string, int, int> > c = courseCode;
  map<string,int> unique;
  for(auto u:c)
  {
    unique[get<1>(u.second)] = 1;
  }

  vector<string> result;
  for(auto it:unique)
    result.push_back(it.first);

  return result;
}

map<string, vector<string> > getCourse_WrtBranch(map<string, tuple<string, string, int, int> > courseCode)
{
  map<string, vector<string> > result;
  for(auto it:courseCode)
  {

    if (get<0>(it.second).find("class") != -1) {
         result[get<1>(it.second)].push_back(it.first);
    }
      
  }
  return result;
}

bool compareSlotVector(pair<int, int> a, pair<int, int> b)
{
  return a.first > b.first;
}

bool compareTeacherAllocate(vector<tuple<string, string, int> > a, vector<tuple<string, string, int> > b)
{
  return (a.size() < b.size());
}