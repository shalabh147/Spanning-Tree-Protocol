#include "bridge.h"
#include <bits/stdc++.h> 
using namespace std;

bridge::bridge()
{}

bridge::bridge(string bridge_id, vector<string> v)
{
	this->bridge_id = bridge_id;
	this->dist_to_root = 0;
	this->root_believed = this->bridge_id;
	this->bj_stored = bridge_id;
	this->RP = "";//if it itself is a Root bridge then there does not exist an RP for that bridge.

	for(int i=0;i<v.size();i++)
	{
		status_ports[v[i]] = "DP";
	}

	//initially all ports marked as DP in the status map.
}

void bridge::change_bridge(string bi,int dist,string bj,string port)
{
	if(stoi(bi.substr(1)) < stoi(root_believed.substr(1)))
	{
		if(root_believed != this->bridge_id)    //if it doesnt believe itself to be the root bridge
		{
			root_believed = bi;
			dist_to_root = dist + 1;
			status_ports[RP] = "DP";
			status_ports[port] = "RP";
			RP = port;
			bj_stored = bj;
		}
		else
		{
			root_believed = bi;
			dist_to_root = dist + 1;
			status_ports[port] = "RP";
			RP = port;
			bj_stored = bj;
		}
	}
	else if(bi == root_believed && dist + 1 < this->dist_to_root)
	{
			dist_to_root = dist + 1;
			status_ports[RP] = "DP";
			status_ports[port] = "RP";
			RP = port;
			bj_stored = bj;
	}
	else if(bi == root_believed && dist + 1 == this->dist_to_root && bj_stored > bj)
	{
			bj_stored = bj;
			status_ports[RP] = "DP";
			status_ports[port] = "RP";
			RP = port;	
	}
	else if(bi == root_believed)
	{
		if(port != RP)
		{
			//means that the lan in between the two bridges who are exchanging messages has to decide whether its port for the bridge is DP or NP.
			if(dist == this->dist_to_root)
			{
				//now the lan in between assigns the bridge with smaller id as its designated bridge and other as its NP.
				if(stoi(bj.substr(1)) < stoi(this->bridge_id.substr(1)))
				{
					this->status_ports[port] = "NP";
				}
			}
			else if(dist < this->dist_to_root)
			{
				this->status_ports[port] = "NP";
			}
		}

	}
}

triplet bridge::create_message()
{
	return make_pair(this->root_believed,make_pair(this->dist_to_root,this->bridge_id));
}

vector<pairi> bridge::who_to_send()
{
	vector<pairi> v;
	//cout<<this->bridge_id<<" neighboring" <<neighbour_bridges.size()<<endl;

	for(auto it = neighbour_bridges.begin();it != neighbour_bridges.end();it++)
	{
		string b = it->first;
		vector<string> x = it->second;

		for(int i=0;i<x.size();i++)
		{
			//cout<<b<<" ";
			//cout<<x[i]<<" ";
			v.push_back(make_pair(b,x[i]));
		}
		//cout<<endl;
	}

	return v;
}

void bridge::update_fwd_table(string from,string to,string port)
{
	if(status_ports[port] != "NP")
	{	if(forwarding_table.count(from) == 0)
			forwarding_table[from] = port;}

}

vector<pairi> bridge::ftp_who_to_send(string from,string to,string port)
{
	vector<pairi> v;

	if(forwarding_table.count(to) > 0)
	{
		if(status_ports[port] == "NP")
			return v;
		else
		{
			for(auto it = neighbour_bridges.begin();it != neighbour_bridges.end();it++)
			{
				string b = it->first;
				vector<string> x = it->second;

				for(int i=0;i<x.size();i++)
				{
					//cout<<b<<" ";
					//cout<<x[i]<<" ";
					if(status_ports[x[i]] != "NP" && x[i]==forwarding_table[to] && x[i] != port)
						v.push_back(make_pair(b,x[i]));
				}
			//cout<<endl;
			}

			return v;
		}
	}
	else
	{
		if(status_ports[port] == "NP")
			return v;
		else
		{	
			for(auto it = neighbour_bridges.begin();it != neighbour_bridges.end();it++)
			{
				string b = it->first;
				vector<string> x = it->second;

				for(int i=0;i<x.size();i++)
				{
					//cout<<b<<" ";
					//cout<<x[i]<<" ";
					if(status_ports[x[i]] != "NP" && x[i]!=port)
						v.push_back(make_pair(b,x[i]));
				}
			//cout<<endl;
			}

			return v;

		}

	}


}

