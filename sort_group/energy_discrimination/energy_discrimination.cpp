
#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include<map>
#include<string>
#include "../solution_items/PETsys_data_structure.h"
#define MAX_COINC 400000000


int main(int argc, char* argv[])
{	
	std::cout << "run this program: energy_discrimination.exe calibration_data peak_lookup_table"<<std::endl;

	int event_index = 0;
	char file_name[155];
	std::map<int, float> LUT_E;
	FILE* output;
	int n_coinc = -1;
	/*
	FILE* f_data;
	if (NULL == (f_data = fopen(argv[1], "rb"))) { exit(1); } // IF File name is not proper
	*/
	std::cout << "raw data successfully" << std::endl;

	std::ifstream ifs(argv[2]);
	std::ifstream in(argv[1]);
	uint8_t mh_n1;
	uint8_t mh_j1;
	long long time1;
	float e1;
	int id1;
	uint8_t mh_n2;
	uint8_t mh_j2;
	long long time2;
	float e2;
	int id2;

	int key;
	float value;
	while (ifs >> key >> value)
	{
		LUT_E[key] = value;
	}
	std::cout<<"lookup table read successfully" << std::endl;
	PETsys_Energy_Discrimnated_coincidence_event* PETsys_Energy_Discrimnated_coincidence_event_ptr = (PETsys_Energy_Discrimnated_coincidence_event*)malloc(MAX_COINC * sizeof(PETsys_Energy_Discrimnated_coincidence_event));

	sprintf(file_name, "energy_discriminated_event.lst");
	output = fopen(file_name, "wb");
	int n_coinc_valid = 0;



	PETsys_coincidence_event* PETsys_coinc_event = (PETsys_coincidence_event*)malloc(MAX_COINC * sizeof(PETsys_coincidence_event));;
	/*
	while (fread(&one_PETsys_coinc_event, sizeof(one_PETsys_coinc_event), 1, f_data))
	{
		if ((one_PETsys_coinc_event.e1>0.95*LUT_E[one_PETsys_coinc_event.id1]) && (one_PETsys_coinc_event.e2>0.95*LUT_E[one_PETsys_coinc_event.id2])\
			&& (one_PETsys_coinc_event.e1<1.05*LUT_E[one_PETsys_coinc_event.id1])&&(one_PETsys_coinc_event.e2<1.05*LUT_E[one_PETsys_coinc_event.id2])) //1st line of a group event
		{
			n_coinc++;
			PETsys_Energy_Discrimnated_coincidence_event_ptr[n_coinc].id1 = one_PETsys_coinc_event.id1;
			PETsys_Energy_Discrimnated_coincidence_event_ptr[n_coinc].id2 = one_PETsys_coinc_event.id2;
			PETsys_Energy_Discrimnated_coincidence_event_ptr[n_coinc].time1 = one_PETsys_coinc_event.time1;
			PETsys_Energy_Discrimnated_coincidence_event_ptr[n_coinc].time2 = one_PETsys_coinc_event.time2;
		}
		else
			continue;
	}

	*/
	std::cout << "line 63" << std::endl;
	if (!in.is_open()) {
		std::cout << "Error: opening file fail" << std::endl;
		exit(1);
	}
	n_coinc = 0;
	n_coinc_valid = 0;
	std::string line;
	while (std::getline(in, line) && n_coinc_valid < 10) {
		std::cout << line << std::endl;
		std::cout << n_coinc_valid << std::endl;

		std::stringstream ss(line);
		ss >> mh_n1 >> mh_j1 >> time1 >> e1 >> id1 >> mh_n2 >> mh_j2 >> time2 >> e2 >> id2;
		std::cout << e1 << "," << e2 << "," << LUT_E[id1] << "," << LUT_E[id2] << "," << std::endl;

		if ((e1>0.95*LUT_E[id1]) && (e2>0.95*LUT_E[id2])\
			&& (e1<1.05*LUT_E[id1]) && (e2<1.05*LUT_E[id2])) //1st line of a group event
		{
			PETsys_Energy_Discrimnated_coincidence_event_ptr[n_coinc_valid].id1 = id1;
			PETsys_Energy_Discrimnated_coincidence_event_ptr[n_coinc_valid].id2 = id2;
			PETsys_Energy_Discrimnated_coincidence_event_ptr[n_coinc_valid].time1 = time1;
			PETsys_Energy_Discrimnated_coincidence_event_ptr[n_coinc_valid].time2 = time2;			
			n_coinc_valid++;
		}
		else
			continue;

	}
	in.close();

	int t = fwrite(PETsys_Energy_Discrimnated_coincidence_event_ptr, sizeof(PETsys_Energy_Discrimnated_coincidence_event), n_coinc_valid, output);
	if (n_coinc_valid == (int)t) {
		printf("%s containing %d coincidence events was written successfully after Energy Discrimination.\n", file_name, n_coinc_valid);
	}
	else {
		printf("%s written with error. %d elements were written.\n", file_name, (int)t);
	}
	fclose(output);
}
