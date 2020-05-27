#include <iostream>
#include <map>
#include <vector>
#include <algorithm>    
#include <sstream>   
#include<tuple>
#include<fstream>

using namespace std;

vector<string> split(string line, char ch)
{
    vector<string> result;
    stringstream check1(line); 

    string intermediate; 
      
    // Tokenizing w.r.t. space ' ' 
    while(getline(check1, intermediate, ch)) 
    { 
        result.push_back(intermediate); 
    } 
    return result;
}


map<string, tuple<string, string, int, int> > getCourse()
{
  /*
    Returning parameters are
    course, tuple(className, branchName, countOfPeriods, no of continuos slots it takes)
  */

  map<string, tuple<string, string, int, int> > result;

  ifstream fin; 
  
  // by default open mode = ios::in mode 
  fin.open("course.txt"); 
  string line;

  // Execute a loop until EOF (End of File) 
  while (fin) { 

      // Read a Line from File 
      getline(fin, line);

      vector<string> res = split(line, ' ');
      result[res[1]] = make_tuple(res[2], res[0], stoi(res[3]), stoi(res[4]));
  } 

  // Close the file 
  fin.close(); 

  return result;
}


map<string, string> getTeacher()
{
  map<string, string> result;
  ifstream fin; 
  
  // by default open mode = ios::in mode 
  fin.open("teacher.txt"); 
  string line;

  // Execute a loop until EOF (End of File) 
  while (fin) { 

      // Read a Line from File 
      getline(fin, line);

      vector<string> res = split(line, ' ');
      result[res[0]] = res[1];
  } 

  // Close the file 
  fin.close(); 

  return result;
}

vector<string> getLabQueue(map<string, tuple<string, string, int, int> > courseCode)
{
  /*
    Returns Lab code as round robin approach vector string

  */
  map<string, tuple<string, string, int, int> > c = courseCode,d;
  map<string,int> unique;
  for(auto u:c)
  {
      if (get<0>(u.second).find("lab") != -1) {
          d[u.first] = u.second;
          unique[get<1>(u.second)] = 1;
      }
  }

  map<int, string> hsh;
  int len = unique.size();
  auto it = d.begin();
  string prev = get<1>(it->second);
  hsh[0] = it->first;
  it++;
  int i=0, j=0;
  for(;it != d.end(); it++)
  {
      string cur = get<1>(it->second);
      if(prev == cur)
        i+=len;
      else
        i=++j;
      hsh[i] = it->first;
      prev = get<1>(it->second);
  }

  vector<string> result;
  for(auto it:hsh){
    result.push_back(it.second);
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