#include "ModularLink.h"


class TreeParser {
private:
    ObjectStore& objectStore;

    string findFileInTree(const string& treeHash, const filesystem::path& path) {

        if (path.empty())
            return "";

        string treeContent = objectStore.read_object_content(treeHash);

        istringstream input(treeContent);
        string line;

        string targetName = path.filename().string();
        filesystem::path remainingPath = path.parent_path();

        while (getline(input, line)) {

            istringstream entry(line);

            string type;
            string name;
            string hash;

            entry >> type >> name >> hash;

            // File entry
            if (type == "blob") {

                if (remainingPath.empty() && name == targetName) {
                    return hash;
                }
            }

            // Directory entry
            else if (type == "tree") {

                if (name == remainingPath.begin()->string()) {

                    filesystem::path nextPath;

                    auto it = remainingPath.begin();
                    ++it;

                    for (; it != remainingPath.end(); ++it) {
                        nextPath /= *it;
                    }

                    return findFileInTree(hash, nextPath / targetName);
                }
            }
        }

        return "";
    }

public:

    TreeParser(ObjectStore& store) : objectStore(store) {}

    string findBlobHash(const string& treeHash, const string& filepath) {

        filesystem::path path(filepath);

        return findFileInTree(treeHash, path);
    }
};