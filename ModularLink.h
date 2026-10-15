#pragma once

#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <cstdint>
#include <vector>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <unordered_map>
#include <algorithm>

using namespace std;

#include "SHA1.h"
#include "ObjectStore.h"
#include "FileManager.h"
#include "FileNode.h"
#include "Tree.h"
#include "Commit.h"
#include "BranchManager.h"
#include "StagingArea.h"

