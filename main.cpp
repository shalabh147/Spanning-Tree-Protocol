#include "bridgesim.h"
#include <bits/stdc++.h> 
using namespace std;



int main()
{
	int trace;
	cin>>trace;
	int number_of_bridges;
	cin>>number_of_bridges;
	string A;
	vector<bridge> Bridge_List;
	getline(cin,A);            //so that newline can be incorporated
	for(int i=0;i<number_of_bridges;i++)
	{	
		string S,T,L;
		vector<string> vec;
		vector<string> port_vector;
		getline(cin, S);
		//cout<<S<<" "<<S.size()<<endl;
		stringstream X(S);
		while(getline(X,T,':')){
			vec.push_back(T);
		}

		stringstream Y(vec[1]);
		while(getline(Y,L,' ')){
			port_vector.push_back(L);
		}

		port_vector.erase(port_vector.begin());

		/*for(int j=0;j<port_vector.size();j++)
		{
			cout<<port_vector[j]<<" ";
		}	
		cout<<endl;
*/
		Bridge_List.push_back(bridge(vec[0],port_vector));
		
	}



	map<string,vector<string> > lan_to_bridge;

	for(int i=0;i<Bridge_List.size();i++)
	{
		bridge b = Bridge_List[i];
		//map<string,string>::iterator it;

		for(auto it = b.status_ports.begin();it != b.status_ports.end();it++)
		{

			string s1 = it->first;
			//cout<<s1<<" "<<b.bridge_id<<endl;
			lan_to_bridge[s1].push_back(b.bridge_id); 
		}
	}

	map<string,string> lan_to_hosts;

	for(int i=0;i<lan_to_bridge.size();i++)
	{
		string S,T,L;
		vector<string> vec;
		vector<string> port_vector;
		getline(cin, S);

		stringstream X(S);
		while(getline(X,T,':')){
			vec.push_back(T);
		}

		stringstream Y(vec[1]);
		while(getline(Y,L,' ')){
			port_vector.push_back(L);
		}

		port_vector.erase(port_vector.begin());

		for(int j=0;j<port_vector.size();j++)
		{
			lan_to_hosts[port_vector[j]] = vec[0];
		}

	}

	/*for(auto it = lan_to_hosts.begin();it != lan_to_hosts.end(); it++)
	{
		cout<<it->first<<" "<<it->second<<endl;
	}
	*/



	for(int i=0;i<Bridge_List.size();i++)
	{
		//bridge b = Bridge_List[i];
		//map<string,string>::iterator it;

		for(auto it = Bridge_List[i].status_ports.begin();it != Bridge_List[i].status_ports.end();it++)
		{
			string s1 = it->first;
			for(int j = 0; j < lan_to_bridge[s1].size();j++)
			{	
				//cout<<j<<" ";
				//cout<<s1<<" "<<j<<endl;
				if(lan_to_bridge[s1][j] != Bridge_List[i].bridge_id)
				{   //cout<<s1<<" "<<j<<endl;
					Bridge_List[i].neighbour_bridges[lan_to_bridge[s1][j]].push_back(s1);
				}
			}
		}

	//	cout<<"size "<<b.neighbour_bridges.size()<<endl;
	}


	//cout<<"reached"<<endl;

	bridgesim Simulator(Bridge_List);


	Simulator.simulation(trace);

	map<string,bridge,comparator> m = Simulator.id_to_bridge;

	int num_queries;

	for(auto it = m.begin();it != m.end() ; it++)
	{
		bridge b = it->second;

		cout<<b.bridge_id<<":";
		for(auto it2=b.status_ports.begin();it2 != b.status_ports.end();it2++)
		{
			cout<<" "<<it2->first<<"-"<<it2->second;
		}	

		cout<<endl;
	}

	cin>>num_queries;

	string from,to;
	for(int i=0;i<num_queries;i++)
	{
		cin>>from>>to;
	//	cout<<from<<" "<<to<<endl;

		Simulator.ftpsimulate(trace,from,to,lan_to_hosts,lan_to_bridge);

		for(auto it = Simulator.id_to_bridge.begin(); it != Simulator.id_to_bridge.end(); it++)
		{
			bridge b = it->second;
			cout<<b.bridge_id<<":"<<endl;
			cout<<"HOST ID | FORWARDING PORT"<<endl;
			for(auto it2 = b.forwarding_table.begin(); it2 != b.forwarding_table.end() ; it2++)
			{
				cout<<it2->first<<" | "<<it2->second<<endl;
			}
		}

		cout<<endl;


	}


	

	










	


	//simulation now
}