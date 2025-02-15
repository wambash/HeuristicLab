#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <map>
#ifdef _WIN32
#include <direct.h>
#elif defined __linux__
#include <sys/stat.h>
#endif

//INCLUDE LIBRARIES
#include "utils/utils.cpp"
#include "cec20_test_func.cpp"

//INCLUDE ALGORITHM
#include "alg/PSONovD.cpp"

//choose nickname for it
const char *alg_name = "PSONovD";
Algorithm alg;

//CHOOSE NUMBER OF RUNS
#define RUNS 30

//OPTIONAL: COMMENT OUT ALGORITHMS YOU DON'T WANT TO RUN
std::map<std::string, int> algorithms{
	{"BentCigar", 1},
	{"RotatedSchwefel", 2},
	{"Lunacek", 3},
	{"Rosenbrock", 4},
	{"Hybrid1", 5},
	{"Hybrid6", 6},
//	{"Hybrid5", 7},
	{"Composition2", 8},
	{"Composition4", 9},
	{"Composition5", 10},
};

//OPTIONAL: COMMENT OUT DIMENSIONS YOU DON'T WANT TO RUN (tested are 10,20)
const int implementedDimensions[] = {
	//5,
	10,
	//15,
	//20,
	//30,
	//50,
	//100
};

#define BOUNDARY_LOW -100
#define BOUNDARY_UP 100

void makeCSVfile(std::string filename, std::vector<std::vector<result>> &result, std::string algName)
{
	std::ofstream file;
	std::string folder = "csv/" + algName;
#if defined(_WIN32)
	_mkdir(folder.c_str());
#else
	mkdir(folder.c_str(), 0775);
#endif
	file.open("csv/" + algName + "/" + filename + ".csv");
	//for docker
	//file.open("/usr/src/results/" + filename + ".csv");

	std::vector<std::string> tempRes;
	for (unsigned int i = 0; i < result.size(); i++)
	{
		for (unsigned int p = 0; p < result.at(i).size(); p++)
		{

			if (i == 0)
			{
				tempRes.push_back(std::to_string(static_cast<int>(result.at(i).at(p).fez)) + ";" + std::to_string(result.at(i).at(p).cost));
			}
			else
			{
				tempRes.at(p) = tempRes.at(p) + ";" + std::to_string(result.at(i).at(p).cost);
			}
		}
	}
	for (int i = 0; i < tempRes.size(); i++)
	{
		file << tempRes.at(i) + "\n";
	}
	file.close();
	std::cout << "created file" << std::endl;
}

int dimensionSize = 0;

int main()
{
	std::cout << "start" << std::endl;

	for (int dimensionSize : implementedDimensions)
	{
		std::map<std::string, int>::iterator i;
		for (i = algorithms.begin(); i != algorithms.end(); i++)
		{
			std::vector<std::vector<result>> csv;
			for (int j = 0; j < RUNS; j++)
			{
				csv.push_back(alg.run(dimensionSize, i->second, BOUNDARY_LOW, BOUNDARY_UP));
			}

			std::string algName = alg_name;
			makeCSVfile(algName + "_" + i->first + "_" + std::to_string(dimensionSize) + "d", csv, algName);
		}
	}

	std::cout << "finish" << std::endl;
	return 0;
}
