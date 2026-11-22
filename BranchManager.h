#include "ModularLink.h"


class BranchManager{
    private:
        string headsPath;
        string currentBranch;

    public:
        BranchManager(const string& repositoryPath): headsPath(repositoryPath + "/refs/heads/"), currentBranch("main"){}

        void updateCurrentBranch(const string& commitHash){
            string branchPath=headsPath+currentBranch;

            ofstream branch(branchPath);

            if(!branch.is_open()){
                throw runtime_error("Could not update branch");
            }

            branch<<commitHash;
            branch.close();
        }

        string get_current_commit() const{
            string branchPath=headsPath+currentBranch;

            ifstream branch(branchPath);

            if(!branch.is_open()){
                return "";
            }

            string commitHash;
            getline(branch, commitHash);

            branch.close();

            return commitHash;
        }

        string get_current_branch() const{
            return currentBranch;
        }
};
