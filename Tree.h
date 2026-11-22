#include "ModularLink.h"


class Tree {
private:
    FileNode root;

    FileNode* findChild(FileNode& parent, const string& name, bool directory) {
        for (auto& child : parent.getChildren()) {
            if (child.getName() == name && child.isDirectory() == directory) {
                return &child;
            }
        }
        return nullptr;
    }

    string serializeNode(const FileNode& node) const {

        string data;

        vector<FileNode> sortedChildren = node.getChildren();

        sort(sortedChildren.begin(), sortedChildren.end(),
            [](const FileNode& a, const FileNode& b) {
                return a.getName() < b.getName();
            });

        for (const auto& child : sortedChildren) {

            if (child.isDirectory()) {
                data += "tree ";
                data += child.getName();
                data += "\n";

                data += serializeNode(child);
            }
            else {
                data += "blob ";
                data += child.getName();
                data += " ";
                data += child.getHash();
                data += "\n";
            }
        }

        return data;
    }

    // Recursively create Tree objects for directories
    string createTreeObject(FileNode& node, ObjectStore& objectStore) {

        string data;

        vector<FileNode> sortedChildren = node.getChildren();

        sort(sortedChildren.begin(), sortedChildren.end(),
             [](const FileNode& a, const FileNode& b) {
                 return a.getName() < b.getName();
             });

        for (auto& child : sortedChildren) {

            if (child.isDirectory()) {

                // Recursively create the directory's Tree object
                string childTreeHash = createTreeObject(child, objectStore);

                data += "tree ";
                data += child.getName();
                data += " ";
                data += childTreeHash;
                data += "\n";
            }
            else {

                data += "blob ";
                data += child.getName();
                data += " ";
                data += child.getHash();
                data += "\n";
            }
        }

        // Store this directory's Tree object
        return objectStore.create_object("tree", data);
    }

public:
    Tree() : root(".", true) {}

    void addFile(const string& filename, const string& hash) {

        filesystem::path path(filename);
        FileNode* current = &root;

        for (auto it = path.begin(); it != path.end(); ++it) {

            string part = it->string();
            bool isLast = next(it) == path.end();

            if (isLast) {
                current->addChild(FileNode(part, false, hash));
            }
            else {
                FileNode* directory = findChild(*current, part, true);

                if (directory == nullptr) {
                    current->addChild(FileNode(part, true));
                    directory = findChild(*current, part, true);
                }

                current = directory;
            }
        }
    }

    const FileNode& getRoot() const {
        return root;
    }

    // Used for viewing the tree structure
    void printDFS(const FileNode& node, int depth = 0) const {

        for (int i = 0; i < depth; i++) {
            cout << " ";
        }

        cout << node.getName();

        if (!node.isDirectory()) {
            cout << " [" << node.getHash() << "]";
        }

        cout << "\n";

        for (const auto& child : node.getChildren()) {
            printDFS(child, depth + 1);
        }
    }

    // Create the root Tree object and all nested Tree objects
    string createObject(ObjectStore& objectStore) {
        return createTreeObject(root, objectStore);
    }

    string serialize() const{
        return serializeNode(root);
    }
};