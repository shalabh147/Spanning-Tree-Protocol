#include "bridgesim.h"
#include <bits/stdc++.h>
using namespace std;

bridgesim::bridgesim()
{

}

bridgesim::bridgesim(vector<bridge> bridgelist)
{
	for(int i=0;i<bridgelist.size();i++)
	{
		string x = bridgelist[i].bridge_id;
		id_to_bridge[x] = bridgelist[i];
	}
}

void bridgesim::simulation(int trace)
{
	int countdown_factor = 1;
	bool go = true;
	while(go)
	{
		if(!sim_vector.empty())
			{
				map<string,vector<pair<pairi , triplet> > > m = sim_vector[sim_vector.size()-1];
				vector<pair<pairi , triplet> > v = m["s"];
				map<string,vector<pair<pairi , triplet> > > m2;
				m2["r"] = v;
				//sim_vector.push_back(m2);
				//cout<<v.size()<<endl;

				for(int i=0;i<v.size();i++)
				{
					pair<pairi,triplet> new_p = v[i];
					pairi x = new_p.first;
					triplet y = new_p.second;

					string id = x.first;
					bridge* b = &id_to_bridge[id];
				//	cout<<"88yo"<<endl;
					//cout<<y.first<<" "<<y.second.first<<" "<<y.second.second<<" "<<x.second<<endl;
					b->change_bridge(y.first,y.second.first,y.second.second,x.second);

					triplet new_message = b->create_message();
					vector<pairi> to_send_bridges = b->who_to_send();
					//cout<<to_send_bridges.size()<<endl;
					for(int j=0;j<to_send_bridges.size();j++)
					{
						pairi p = to_send_bridges[j];
						string bridge_to_send = p.first;
						string port_through = p.second;

						m2["s"].push_back(make_pair(p,new_message));

					}

				}

				sim_vector.push_back(m2);
				//cout<<sim_vector.size()<<endl;
			}
		else
		{
			vector<pair<pairi , triplet> > v;
				map<string,vector<pair<pairi , triplet> > > m2;

				m2["r"] = v;
				//cout<<id_to_bridge.size()<<endl;
			for(auto it = id_to_bridge.begin();it != id_to_bridge.end();it++)
			{

				string x = it->first;
				bridge b = it->second;

				triplet new_message = b.create_message();
				vector<pairi> to_send_bridges = b.who_to_send();


				for(int j=0;j<to_send_bridges.size();j++)
				{
						pairi p = to_send_bridges[j];
						string bridge_to_send = p.first;
						string port_through = p.second;
						//cout<<p.first<<" "<<p.second<<endl;
						m2["s"].push_back(make_pair(p,new_message));

				}


			}


			//cout<<"reached 2"<<endl;

			sim_vector.push_back(m2);
			//cout<<sim_vector.size()<<endl;
		}



		if(countdown_factor == 0)
			break;

		bool same = true;

		//cout<<"hello"<<endl;

		string candidate_root = (id_to_bridge.begin()->second).root_believed;
		for(auto it = id_to_bridge.begin();it != id_to_bridge.end();it++)
		{
			string x = it->first;
			bridge b = it->second;

			//cout<<b.root_believed<<endl;
			//cout<<"reached3"<<endl;
			if(b.root_believed != candidate_root)
				same = false;

		}

		if(same)
			countdown_factor = 0;

	}


	if(trace)
	{
		for(int i=0;i<sim_vector.size();i++)
		{
			map<string,vector<pair<pairi , triplet> > > m = sim_vector[i];

			for(auto it = m.begin();it != m.end();it++)
			{
				if(it->first == "s")
				{
					for(int j=0;j<it->second.size();j++)
					{
						cout<<i<<" "<<"s"<<" "<<it->second[j].second.second.second<<" "<<"("<<it->second[j].second.first<<","<<it->second[j].second.second.first<<","<<it->second[j].second.second.second<<")"<<endl;
					}
				}
				else
				{
					for(int j=0;j<it->second.size();j++)
					{
						cout<<i<<" "<<"r"<<" "<<it->second[j].first.first<<" "<<"("<<it->second[j].second.first<<","<<it->second[j].second.second.first<<","<<it->second[j].second.second.second<<")"<<endl;
					}
				}

				cout<<endl;
			}
		}
	}

}

void bridgesim::ftpsimulate(int trace,string from,string to,map<string,string> lan_to_hosts,map<string,vector<string> > lan_to_bridge)
{
	string lan = lan_to_hosts[from];
	map<string,vector<pairi> > m;
	vector<pairi> v;
	//m["r"] = v;

	//cout<<"reached1"<<endl;

	for(int i=0;i<lan_to_bridge[lan].size();i++)
	{
		//bridge* b = &id_to_bridge[lan_to_bridge[lan][i]];
		//b->update_fwd_table(from,to,lan);
		//vector<pairi> send = b->ftp_who_to_send(from,to,lan);
		//for(int j=0;j<send.size();j++)
			m["r"].push_back(make_pair(lan_to_bridge[lan][i],lan));
	}

	//cout<<"hello11"<<endl;
	ftp_sim.push_back(m);

	//cout<<"hello"<<endl;
	bool go = true;
	int n=2;
	while(go)
	{
		vector<pairi> vec;
        vec = ftp_sim[ftp_sim.size()-1]["r"];
        map<string,vector<pairi> > m2;

      //  cout<<vec.size()<<endl;
        for(int i=0;i<vec.size();i++)
        {
            pairi new_p = vec[i];
          //  cout<<new_p.first<<" "<<new_p.second<<endl;
            bridge* b = &id_to_bridge[new_p.first];
            b->update_fwd_table(from,to,new_p.second);

            vector<pairi> send = b->ftp_who_to_send(from,to,new_p.second);
            //cout<<endl;
            //cout<<send.size()<<endl;
            //for(int j=0;j<send.size();j++)
            //{
            //cout<<send[j].first<<" "<<send[j].second<<endl;
        //}
            //cout<<send[1].first<<" "<<send[1].second<<endl;
        //    cout<<send.size()<<endl;
            for(int j=0;j<send.size();j++)
            {
                ftp_sim[ftp_sim.size()-1]["s"].push_back(make_pair(new_p.first,send[j].second));
            }

            for(int j=0;j<send.size();j++)
            {
                m2["r"].push_back(send[j]);
            }

        }
        if(ftp_sim[ftp_sim.size()-1]["s"].size()==0)
            break;

        ftp_sim.push_back(m2);
        n++;
        //cout<<n<<endl;
    }

  //  cout<<"henlo"<<endl;

    if(trace)
    {
        for(int i=0;i<ftp_sim.size();i++)
        {
        	map<string,vector<pairi> > m = ftp_sim[i];
            for(auto it = m.begin();it!=m.end();it++)
            {
            	if(it->first == "r")
	            {
	                for(int j=0;j<it->second.size();it++)
	                {
	                    cout<<i<<" "<<"r"<<" "<<(it->second)[j].first<<" "<<from<<" --> "<<to<<endl;
	                }
	            }
            	else
            	{	
	                for(int j=0;j<it->second.size();it++)
	                {
	                    cout<<i<<" "<<"s"<<" "<<(it->second)[j].first<<" "<<from<<" --> "<<to<<endl;
	                }
	            }
            }
        }
    }
}




