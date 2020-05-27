#include <iostream>
#include <map>
#include <vector>
#include <algorithm>    // std::shuffle
#include <random>       // std::default_random_engine
#include <chrono>       // std::chrono::system_clock
#include <sstream>   
#include<tuple>
#include <emscripten/bind.h>
#include"ttbl_content.h"
using namespace std;

map<int, string> returnVectorData () {
  map<int, string> v;
  return v;
}

map<string, map<int, string>> returnMapData ()
{

	
 // Class name 
  string className[] = {"classB5", "classElec", "classMng", "labCom", "labElec"};
  // Branch name
  string branchName[] = {"cse2", "cse3", "ece3"};

  // Course Code and their type and count
  map<string, tuple<string, string, int, int>> courseCode = getCourse();

  // Teacher Code and their subject course code
  map<string, string> teacherCode = getTeacher();

  // No of days in a week
  int noDay = 5;
  // No of hours or teaching slot in a week
  int noHour = 10;
  // Total slots
  int totSlots = noDay * noHour;
  // Break(khana) slot number
  int breakSlot = 5;

  // Size of Class and teacher hash = noDay * noHour
  // Teacher Hash
  map<string, map<int,int>> teachHash;
  // Class Hash
  map<string, map<int,int>> classHash;  
  // Branch_year Hash
  map<string, map<int,int>> branchHash;

  // Result storing
  map<string, map<int, string>> branchTimeTable;
  
// LAB ALLOCATION
  // Lab allocation  ---  rules are
  // 1. Time slot (will improve it in the future)
  // 		|->at end of day (slot no-(7, 8, 9), (17, 18, 19)...) 
  // 		|->at start of day when end is not available(slot no - (0, 1, 2), (10, 11, 12))
  // 2. Round robin approach
  // 		|->start with a some branch, allocate one lab
  // 		|->go to second branch check constraints and allocate as per tha
  // 3. Constraints are
  // 		1. check branchHash for lab space at end slots
  // 				1. check for classHash(lab)
  // 						1. check for teachHash
  // 								allocate
  // 			GOTO NEXT DAY SLOTS
  // 		if all end slots over then go for starting slots()
  // 4. For Morning lab slots
  // 		|->Make a hash for counting how many times we have visited a slot of branch and also allocated
  // 		|->the day which has less hash value , lab is allocated that day after checking constraints
// Lab queue for round robin allocation approach
  vector<string> labQueue = getLabQueue(courseCode);

  // Getting info about the allocated lab wrt course code
  map<string, int> labAllocated;

  // Visited hash for getting count of labs in each days for morning lab allocation
  map<string, map<int,int>> visitedDay;
  // Count variable for storing the count of number of allocation of labs
  int countLabAllocated = 0;


// Allocation of Lab at end slots
  for(int j=0; j < labQueue.size(); j++)
  {

  		string courseId = labQueue[j];
  		string clas = get<0>(courseCode[courseId]);
  		string branch = get<1>(courseCode[courseId]);
  		string teacher = teacherCode[courseId];
  		for(int i=0; i < noDay; i++){

  			// Last slot
  			int slotNo = (i+1)*noHour - 1;
	  		if(branchHash[branch][slotNo] == 0)
	  		{
	  				if(classHash[clas][slotNo] == 0)
	  				{
	  						if(teachHash[teacher][slotNo] == 0)
	  						{
	  								// Allocate
	  								int noSlots = get<3>(courseCode[courseId]);
	  								for(int k=0; k < noSlots; k++)
	  								{
	  										teachHash[teacher][slotNo-k] = 1;
	  										classHash[clas][slotNo-k] = 1;
	  										branchHash[branch][slotNo-k] = 1;
	  										branchTimeTable[branch][slotNo-k] = courseId;
	  								}

	  								// Visited day hash
	  								visitedDay[branch][i]++;
	  								// countLab allocated
	  								countLabAllocated++;
	  								// Lab allocation log
	  								labAllocated[courseId]++;
	  								break;
	  						}
	  				}
	  		}
	  	}
  }

// Allocation of Lab at starting slots
if(countLabAllocated < labQueue.size())
{
		for(int j=0; j < labQueue.size(); j++)
  	{

  		string courseId = labQueue[j];
  		if(labAllocated[courseId] == get<2>(courseCode[courseId]))
  			continue;
  		// first slot
  		string clas = get<0>(courseCode[courseId]);
  		string branch = get<1>(courseCode[courseId]);
  		string teacher = teacherCode[courseId];

  		for(int i=0; i < noDay; i++){
  			// This day has a lab at its end slots
  			if(visitedDay[branch][i])
  				continue;
  			int slotNo = i*noHour;
	  		if(branchHash[branch][slotNo] == 0)
	  		{
	  				if(classHash[clas][slotNo] == 0)
	  				{
	  						if(teachHash[teacher][slotNo] == 0)
	  						{
	  								// Allocate
	  								int noSlots = get<3>(courseCode[courseId]);
	  								for(int k=0; k < noSlots; k++)
	  								{
	  										teachHash[teacher][slotNo+k] = 1;
	  										classHash[clas][slotNo+k] = 1;
	  										branchHash[branch][slotNo+k] = 1;
	  										branchTimeTable[branch][slotNo+k] = courseId;
	  								}

	  								// Visited day hash
	  								visitedDay[branch][i] += noSlots;
	  								// countLab allocated
	  								countLabAllocated++;
	  								// Lab allocation log
	  								labAllocated[courseId]++;
	  								break;
	  						}
	  				}
	  		}
	  	}
  }
}

	
// Class allocation   ---  rules are
// 1. Round robin approach
// 		|->each branch will get a maximum of 3 subjects to allocate (same algo as lab for round robin)
// 2. Check for continuous 3 time slots in branchHash
// 		True -> Check 3 time slots in classHash for availability
// 				True -> Teacher assignment algo
// 	Goto next slot(increment by one slot) - if break(khana) slot comes goto next day
// 3. Similar as (2) but for 2 continous slots
// 4. Similar as (2) but for 1 continous slots

// Teacher assignment algo
// 1. Make a vector of techer code having index as slot no - (0,1,2), (3,4,5)
// 2. Sort the vector wrt size
// 3. Make a hash of teacher code which has been used in the 3 slot allocation and 
// 4. iterate over each slot teacher codes and if there is no entry for a teacher code then allocate it as 1 and goto next slot
// 4. repeat

vector<string> classQueue = getSubjectQueue(courseCode);
map<string, vector<string>> courseWrtBranch = getCourse_WrtBranch(courseCode);
// Stores the count of every course code which has been allocated
map<string, int> classAllocatedNos;

unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
std::default_random_engine e(seed);

int bInitialize[] = {0, breakSlot, 0, breakSlot, 0, breakSlot, 0, breakSlot};
int bCondition[] ={breakSlot, noHour, breakSlot, noHour, breakSlot, noHour, breakSlot, noHour};
int continueCondition[] = {8, 8, 6, 6, 10, 10, 10, 10};
int slotCondtion[] = {4, 4, 3, 3, 2, 2, 1, 1};
for(int p=0; p < 8; p++){



for(int i=0; i < 10; i++)			// Use flags for loop breaking
{

		// Random seed
		for(int j=0; j < classQueue.size(); j++)	
		{
			string branch = classQueue[j];
			string courseId = courseWrtBranch[branch][0];
			string clas = get<0>(courseCode[courseId]);
			int flagAllocation = 0;

			for(int a=0; a < noDay && flagAllocation == 0; a++)
			{
					int countContinous = 0;
					int prevBranch = 0;
					int prevClass = 0;
					vector<pair<int, int>> slotVector;
					// Continuing for only 4slot type
					if(p < 2 && visitedDay[branch][a] + 4 >= continueCondition[p])
						continue;
					// Continuing those day in which 4 classes and 1 lab is held
					if(visitedDay[branch][a] > continueCondition[p])
						continue;
					// Add something to variable b because always it runs for monday i.e slot no - (0-4)
					for(int b=bInitialize[p] + a*10; b < bCondition[p] + a*10; b++)		// This loop counts contiguous slots in branchHash and classHash
					{
							if(branchHash[branch][b] == 0 && classHash[clas][b] == 0 ){
									if(countContinous == 0 || countContinous > 0 && prevBranch == 0 && prevClass == 0){
										countContinous++;
										if(countContinous == slotCondtion[p])
										{
											slotVector.push_back(make_pair(countContinous, b));
										}
									}
								}
								else{
									countContinous = 0;
								}
							prevBranch = branchHash[branch][b];
							prevClass = classHash[clas][b];
					}
					// Sorted slotVector in Decreasing order of their size
					sort(slotVector.begin(), slotVector.end(), compareSlotVector);
					int slotNoToAllocate = 0;

					// Make a Hash for courses which has classes left
					for(auto it:slotVector)
					{
							if(flagAllocation == 1)
									break;

							countContinous = it.first;
							slotNoToAllocate = it.second + 1;
							slotNoToAllocate -= countContinous;
							countContinous = min(slotCondtion[p], countContinous);


							// Apply Teacher assignment algo
							vector<pair<string, string>> teacher;
							vector<string> course = courseWrtBranch[branch];
							// 1. Step
							// Teachercode, course code, slot no
							vector<vector<tuple<string, string, int>>> slotTeacher(100);
							// Shuffling course codes
							shuffle(course.begin(), course.end(), e);

							for(auto itr:course)
							{
								if(classAllocatedNos[itr] < 4)
										teacher.push_back(make_pair(teacherCode[itr], itr));
							}
							for(int b = slotNoToAllocate; b < countContinous + slotNoToAllocate; b++){
								
									for(auto itr:teacher)
									{
											if(teachHash[itr.first][b] == 0)
													slotTeacher[b].push_back(make_tuple(itr.first, itr.second, b));

									}
							}
							// 2. Step
							sort(slotTeacher.begin(), slotTeacher.end(), compareTeacherAllocate);

							// 3. Step
							map<string, int> teacherSelected, teacherSelectedTemp;
							int countConClass = 0;
							for(int b = 0; b < slotTeacher.size(); b++)
							{
									if(slotTeacher[b].empty())
											continue;
									for(auto u: slotTeacher[b])
									{
											if(teacherSelectedTemp[get<0>(u)] == 0)
											{
													teacherSelectedTemp[get<0>(u)] = 1;
													countConClass+=1;
													break;

											}
											// Giving a space in b/w classes if no teacher is available
									}


							}

							if(countConClass == slotCondtion[p])
									{
										for(int b = 0; b < slotTeacher.size(); b++)
										{
												if(slotTeacher[b].empty())
														continue;
												for(auto u: slotTeacher[b])
												{
														if(teacherSelected[get<0>(u)] == 0)
														{
																teacherSelected[get<0>(u)] = 1;
																// Allocate
																teachHash[get<0>(u)][get<2>(u)] = 1;
																teachHash[get<0>(u)][get<2>(u)+1] = 1;
																branchHash[branch][get<2>(u)] = stoi(get<1>(u));
																classHash[clas][get<2>(u)] = 1;
																classAllocatedNos[get<1>(u)]++;
																flagAllocation = 1;
																branchTimeTable[branch][get<2>(u)] = get<1>(u);
																// Visited count
																visitedDay[branch][a]++;
																break;

														}
														// Giving a space in b/w classes if no teacher is available
												}


										}
									}

							

							// get<2>(courseCode[courseId]);  use it to get number of classes for a course code in a week
							// If allocation happens break the loop
					}
				}

		}

}
}

for(auto it: branchTimeTable)
{
		for(int i=0;i<noHour*noDay;i++)
		{
			if(it.second[i].empty())
				branchTimeTable[it.first][i] = "0";
		}
}


return branchTimeTable;
}


EMSCRIPTEN_BINDINGS(module) {
  emscripten::function("returnMapData", &returnMapData);
  emscripten::function("returnVectorData", &returnVectorData);

  // register bindings for string and string.
  emscripten::register_map<string, map<int, string>>("map<string, map<int, string>>");
  emscripten::register_map<int, string>("map<int, string>");
}



// var retVector = Module['returnVectorData']();

// // vector size
// var vectorSize = retVector.size();
// var retMap = Module['returnMapData']();

// // map size
// var mapSize = retMap.size();
// retVector = retMap.get("cse2");
// for(var i=0; i<retVector.size(); i++){
//     console.log(retVector.get(i));}