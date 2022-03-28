#include "fitTree.h"
#include "SimpleHistSVC.h"
#include "TChain.h"
#include <map>
#include <string>
#include <iostream>
#include <vector>
#include <cstring>
#include <fstream>


using namespace std;

void ShowUsage(string main_exe) {
    cout << "Usage: \n";
    cout << "/Path/to/" << main_exe << " -i <input files list> -o <output file> -task <task name>" << endl;
    cout << "/Path/to/" << main_exe << " -i <input file name> -o <output file> -task <task name>" << endl;
}

map<string,string> parse_argv(int argc,char *argv[]) {
    map<string,string> config;
    config["events"] = "-1";
    map<string,string> name_map = {{"-i","input_list"},{"-o","output_file"},{"-task","task"},{"-n","events"}};
    int n_arg = 1;
    while(n_arg<argc){
        if(name_map.find(argv[n_arg])!=name_map.end()){
            config[name_map[argv[n_arg]]] = argv[n_arg+1];
            n_arg += 2;
        }
        else {
            ShowUsage(argv[0]);
            throw 1;
        }
    }
    return config;
}

vector<int> split (string s, string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    vector<int> res;

    while ((pos_end = s.find (delimiter, pos_start)) != string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (atoi(token.c_str()));        
    }

    res.push_back (atoi(s.substr (pos_start).c_str()));
    return res;
}

int main(int argc,char *argv[]) {
    map<string,string> config = parse_argv(argc,argv);    
    string tree_name = "Calo01IslandFits/fits/fitTree";

    string file_list = config["input_list"];
    vector<string> list_of_files;
    if(file_list.find(".list")>file_list.length() && file_list.find(".root")<file_list.length()) {
        list_of_files.push_back(file_list);
    }
    else {        
        ifstream file(file_list.c_str());
        string str;
        while (std::getline(file, str))
        {
         if(str.find("#")==0) {
            continue;
         }     
         list_of_files.push_back(str);         
        }
    }

    TChain * fChain = new TChain(tree_name.c_str());
    for(string file_path : list_of_files){
        fChain->Add(file_path.c_str());
        cout <<"adding: "<<file_path<<endl;
    }

    //Book analyses and TTree
    TFile * output_file = new TFile(config["output_file"].c_str(),"RECREATE");
    TDirectory * dir = output_file->mkdir(config["task"].c_str());
    int events = atoi(config["events"].c_str());
    cout << "\nTotal entries: "<< fChain->GetEntries() << endl;
    cout << "Task to do:      "<< config["task"] << endl;
    cout <<"Output file:      "<< config["output_file"] << endl;
    cout <<"Using events:     "<< config["events"] << endl;


    fitTree analyser(config["task"]);

    dir->cd();
    analyser.SetFile(fChain,dir);

    analyser.Loop(events);
    analyser.WriteToFile();

    cout<<"finished "<<config["hit_name"]<<endl;    
    output_file->Close();

    return 0;
}
