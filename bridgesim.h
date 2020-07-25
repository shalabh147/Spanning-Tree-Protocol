#ifndef BRIDGESIM_H
#define BRIDGESIM_H
#include "bridge.h"
#include <bits/stdc++.h>

using namespace std;


class bridgesim{
	
public:
	map<string,bridge,comparator> id_to_bridge;
	vector<map<string,vector<pair<pairi , triplet> > > > sim_vector;
	vector<map<string,vector<pairi> > > ftp_sim;

	bridgesim();
	bridgesim(vector<bridge> bridgelist);
	void simulation(int trace);
	void ftpsimulate(int trace,string from,string to,map<string,string> lan_to_hosts,map<string,vector<string> > lan_to_bridge);
};
#endif