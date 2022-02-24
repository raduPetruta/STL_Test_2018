#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <unordered_map>
using namespace std;

ifstream fi("input.txt");
ofstream fo("output.txt");

int nrUserRole; int nrRolePermission; int nrUserPermission;

struct asocieri {
    string user; 
    string role;
};
struct roles {
    string role; 
    string permission; 
    string levelOfAcces;
};
struct perm {
    string user;
    string permission;
};

//three vectors for input
vector<asocieri> Asocieri;
vector<roles> Roles;
vector<perm> Perm;

vector<string> rez;    //for initial levels of acces
vector<string> rez_Temp; //for modified output of levels of acces
vector<string> admins;

unordered_map<string, vector<string>> levelsAcces;

void getInput() {

    // get every user and their roles
    fi >> nrUserRole;
    for (int i = 0; i < nrUserRole; i++) {
        asocieri temp;
        fi >> temp.user >> temp.role;
        Asocieri.push_back(temp);
    }

    //get every role and its permissions and levels of acces
    fi >> nrRolePermission;
    for (int i = 0; i < nrRolePermission; i++) {
        roles temp;
        fi >> temp.role >> temp.permission >> temp.levelOfAcces;
        Roles.push_back(temp);
    }

    //get the user that we need to search for
    fi >> nrUserPermission;
    for (int i = 0; i < nrUserPermission; i++) {
        perm temp;
        fi >> temp.user >> temp.permission;
        Perm.push_back(temp);
    }

    //a vector of strings for the levels of acces that is initially set to "----";
    for (int i = 0; i < nrUserPermission; i++)
        rez_Temp.push_back("----");

    fi.close();    
}
void solveA() {

    //we go through every user that we need to search for
    for (int i = 0; i < Perm.size(); i++) {
    
        string userToSearch = Perm[i].user;
        string permToSearch = Perm[i].permission;
        string allLevels;

        //for every user's role in the input list we search the permission
        for (int j = 0; j < Asocieri.size(); j++) {           
            if (Asocieri[j].user == userToSearch) {
                
                string roleToSearch = Asocieri[j].role;

                //for every permision we search the levels of acces
                for (int k = 0; k < Roles.size(); k++) {
                    
                    if (Roles[k].role == roleToSearch) {

                        //if the role is found we save the levels of acces
                        if (Roles[k].permission == permToSearch) 
                            allLevels += Roles[k].levelOfAcces;
                    }
                }
            }
            else continue;
        }
        
        if (allLevels == "")
            rez.push_back("----");
        else 
            rez.push_back(allLevels);
    }

    //for every level we determine if it has rwxd levels and how many of them
    for (int i = 0; i < rez.size(); i++) {
            int nrR = 0, nrW = 0, nrX = 0, nrD = 0;

            for (int j = 0; j < rez[i].size(); j++) {
                if (rez[i][j] == 'r')
                    nrR++;
                if (rez[i][j] == 'w')
                    nrW++;
                if (rez[i][j] == 'x')
                    nrX++;
                if (rez[i][j] == 'd')
                    nrD++;
            }

            //if we find a 'd' then we stop and the string becomes "---d";
            if (nrD != 0) {
                rez_Temp[i][3] = 'd';
                continue;
            }
            else {
                
                //if we don't have 'd' we modify accordingly

                if (nrR != 0)
                    rez_Temp[i][0] = 'r';
                if (nrW != 0)
                    rez_Temp[i][1] = 'w';
                if (nrX != 0)
                    rez_Temp[i][2] = 'x';
            }
    }
    
    //output for A
    for (int i = 0; i < rez_Temp.size(); i++) {
        fo << Perm[i].user << " " << Perm[i].permission << " ";
        fo << rez_Temp[i] << endl;
    }
}
 
void solveB() {

    //we use a map of string and vector<string> to store for every user all their levels of acces
    for (int i = 0; i < rez_Temp.size(); i++) 
        levelsAcces[Perm[i].user].push_back(rez_Temp[i]);
    
    //if a user has all the levels of acces == "rwx-" than he is admin
    for (auto& it : levelsAcces) {
        
        bool isAdmin = true;
        auto& first = "rwx-"; 
        
        //check to see if all the levels are equal to "rwx-" and if their not empty
        for (auto& iii : it.second) {
            if (iii != first || iii == "----") {
                isAdmin = false;
                break;
            }
        }
        if (isAdmin == true) 
            fo << it.first << " is admin" << endl;
    }
}

int main(){
    
    getInput();
    solveA();
    solveB();
    
    fo.close();
    return 0;
}
