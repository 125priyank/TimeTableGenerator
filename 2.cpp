#include <emscripten/bind.h>
#include<iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <array>
#include<tuple>
using namespace std;

// map<int, string> returnVectorData (string a, string b) {
//   map<int, string> v;
//   v[0] = a;
//   return v;
// }

std::vector<string> vec () {
  std::vector<string> v;
  return v;
}
std::vector<int> vec_int () {
  std::vector<int> v;
  return v;
}
map<string, tuple<string, string, int, int> > getCourse (vector<string> v1, vector<string> v2, vector<int> v3, vector<int> v4) {
  map<string, tuple<string, string, int, int> > v ;
  for(int i=0; i < v1.size(); i++)
    v[v1[i]] = make_tuple(v1[i], v2[i], v3[i], v4[i]);
  return v;
}

map<string, string> getTeacher(vector<string> v1, vector<string> v2)
{
  map<string, string> result;
  for(int i=0; i < v1.size(); i++)
    result[v1[i]] = v2[i];
  return result;
}
// map<string, map<int, string>> returnMapData () {
//   map<string, map<int, string>> m;
//   for(int i=0;i<10;i++)
//   		m["hello"][i] = "abcd";
//   // m.insert(pair<int, string>(10, "This is a string."));
//   return m;
// }

EMSCRIPTEN_BINDINGS(module) {
  // emscripten::function("returnMapData", &returnMapData);
  // emscripten::function("returnVectorData", &returnVectorData);
  emscripten::function("vec", &vec);
  emscripten::function("vec_int", &vec_int);
  emscripten::function("getTeacher", &getTeacher);
  emscripten::function("getCourse", &getCourse);

  // register bindings for string and string.
  // emscripten::register_map<string, map<int, string>>("map<string, map<int, string>>");
  // emscripten::register_map<int, string>("map<int, string>");
  emscripten::register_map<string, string>("map<string, string>");
  emscripten::register_map<string, tuple<string, string, int, int> >("map<string, tuple<string, string, int, int> >");
  emscripten::register_vector<string>("vector<string>");
  emscripten::register_vector<int>("vector<int>");
}
// var retVector = Module['returnVectorData']();

// // vector size
// var vectorSize = retVector.size();
// var retMap = Module['returnMapData']();

// // map size
// var mapSize = retMap.size();





// var a = "hello";
// undefined
// var retVector = Module['returnVectorData'](a);
// undefined
// retVector.size()
// 1
// var ret = Module['ret'](retVector);


// var vec = Module['vec']();
// var vec_int = Module['vec_int']();

// vec_int.push_back(1);

// vec_int.push_back(2);

// vec.push_back("abc");

// vec.push_back("def");

// var v1 = vec;

// var v2 = vec;

// var v3 = vec_int;

// var v4 = vec_int;

// var a = Module['ret'](v1, v2, v3, v4);