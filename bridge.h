#ifndef BRIDGE_H
#define BRIDGE_H
#include <bits/stdc++.h> 
using namespace std;

typedef pair<string,pair<int,string> > triplet; 
typedef pair<string,string> pairi;

struct comparator
{
	bool operator()(const string& s1 , const string& s2) const
	{
		return stoi(s1.substr(1)) < stoi(s2.substr(1));
	}
};

class bridge{

	
	//map<string,string> 

public:
	string bridge_id;
	string root_believed;
	int dist_to_root;
	map<string,string> status_ports;
	map<string,vector<string> > neighbour_bridges;
	string RP;   //the port that bridge thinks is the root port for that bridge. It is unique.
	string bj_stored;
	map<string,string,comparator> forwarding_table;
	bridge();
	bridge(string bridge_id, vector<string> v);
	void change_bridge(string bi,int dist,string bj,string port);
	triplet create_message();
	vector<pairi> who_to_send();
	void update_fwd_table(string from,string to,string port);
	vector<pairi> ftp_who_to_send(string from,string to,string port);


};
#endif