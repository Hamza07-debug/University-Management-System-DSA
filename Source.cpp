#include "header.h"
#include <ctime>

// ==================== UTILITY FUNCTIONS ====================

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void printHeader(string title) {
    int totalWidth = 70;
    int titleLen = title.length();
    int padding = (totalWidth - titleLen - 2) / 2;

    cout << "\n";
    cout << BOLD << CYAN << string(totalWidth, '=') << RESET << endl;
    cout << BOLD << CYAN << string(padding, ' ') << YELLOW << title << CYAN << string(totalWidth - padding - titleLen, ' ') << RESET << endl;
    cout << BOLD << CYAN << string(totalWidth, '=') << RESET << endl;
}

void printSeparator() {
    cout << CYAN << string(70, '-') << RESET << endl;
}

void pauseScreen() {
    cout << YELLOW << "\nPress Enter to continue..." << RESET;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void printSuccess(string message) {
    cout << "\n" << GREEN << "[SUCCESS] " << message << RESET << endl;
}

void printError(string message) {
    cout << "\n" << RED << "[ERROR] " << message << RESET << endl;
}

void printInfo(string message) {
    cout << "\n" << BLUE << "[INFO] " << message << RESET << endl;
}

void printWarning(string message) {
    cout << "\n" << YELLOW << "[WARNING] " << message << RESET << endl;
}

void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int getIntInput(string prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            return value;
        }
        else {
            printError("Invalid input! Please enter a valid number.");
            clearInputBuffer();
        }
    }
}

string getCurrentTimestamp() {
    time_t now = time(0);
    tm ltm;

#ifdef _WIN32
    localtime_s(&ltm, &now);
#else
    localtime_r(&now, &ltm);
#endif

    string timestamp = to_string(1900 + ltm.tm_year) + "-" +
        (ltm.tm_mon + 1 < 10 ? "0" : "") + to_string(1 + ltm.tm_mon) + "-" +
        (ltm.tm_mday < 10 ? "0" : "") + to_string(ltm.tm_mday) + " " +
        (ltm.tm_hour < 10 ? "0" : "") + to_string(ltm.tm_hour) + ":" +
        (ltm.tm_min < 10 ? "0" : "") + to_string(ltm.tm_min);

    return timestamp;
}

bool isValidEmail(string email) {
    size_t atPos = email.find('@');
    if (atPos == string::npos) {
        return false;
    }

    if (email.find("@gmail.com") != string::npos ||
        email.find("@yahoo.com") != string::npos ||
        email.find("@hotmail.com") != string::npos ||
        email.find("@outlook.com") != string::npos ||
        email.find("@edu.pk") != string::npos) {
        return true;
    }

    return false;
}

bool isValidPassword(string password) {
    if (password.length() < 8) {
        return false;
    }

    bool hasUpper = false;
    bool hasLower = false;

    for (int i = 0; i < password.length(); i++) {
        if (password[i] >= 'A' && password[i] <= 'Z') {
            hasUpper = true;
        }
        if (password[i] >= 'a' && password[i] <= 'z') {
            hasLower = true;
        }
    }

    return hasUpper && hasLower;
}

bool isValidRole(string role) {
    string lowerRole = role;
    for (int i = 0; i < lowerRole.length(); i++) {
        if (lowerRole[i] >= 'A' && lowerRole[i] <= 'Z') {
            lowerRole[i] = lowerRole[i] + 32;
        }
    }

    return (lowerRole == "student" || lowerRole == "teacher" || lowerRole == "admin");
}

// ==================== MODULE A: HASH TABLE ====================

HashTable::HashTable() {
    capacity = 10;
    size = 0;
    currentLoggedInUser = "";
    currentUserRole = "";
    table = new HashEntry * [capacity];
    for (int i = 0; i < capacity; i++) {
        table[i] = NULL;
    }
}

HashTable::~HashTable() {
    for (int i = 0; i < capacity; i++) {
        HashEntry* current = table[i];
        while (current != NULL) {
            HashEntry* temp = current;
            current = current->next;
            delete temp;
        }
    }
    delete[] table;
}

int HashTable::hashFunction(string key) {
    int hash = 0;
    for (int i = 0; i < key.length(); i++) {
        hash = (hash * 31 + key[i]) % capacity;
    }
    return hash < 0 ? -hash : hash;
}

void HashTable::resize() {
    int oldCapacity = capacity;
    capacity = capacity * 2;
    HashEntry** oldTable = table;
    table = new HashEntry * [capacity];

    for (int i = 0; i < capacity; i++) {
        table[i] = NULL;
    }

    size = 0;

    for (int i = 0; i < oldCapacity; i++) {
        HashEntry* current = oldTable[i];
        while (current != NULL) {
            registerUser(current->userName, current->password, current->role,
                current->department, current->email);
            HashEntry* temp = current;
            current = current->next;
            delete temp;
        }
    }

    delete[] oldTable;
}

void HashTable::registerUser(string userName, string password, string role, string department, string email) {
    if (searchUser(userName) != NULL) {
        printError("Username '" + userName + "' already exists!");
        return;
    }

    if (size >= capacity * 0.7) {
        resize();
    }

    int index = hashFunction(userName);
    HashEntry* newEntry = new HashEntry();
    newEntry->userName = userName;
    newEntry->password = password;
    newEntry->role = role;
    newEntry->department = department;
    newEntry->email = email;
    newEntry->next = table[index];
    table[index] = newEntry;
    size++;

    printSuccess("User '" + userName + "' registered successfully!");
}

bool HashTable::login(string userName, string password) {
    HashEntry* user = searchUser(userName);
    if (user == NULL) {
        printError("User '" + userName + "' not found!");
        return false;
    }

    if (user->password == password) {
        currentLoggedInUser = userName;
        currentUserRole = user->role;
        printSuccess("Login successful!");
        cout << GREEN << "Welcome back, " << BOLD << user->userName << RESET << GREEN << " (" << user->role << ")!" << RESET << endl;
        return true;
    }
    else {
        printError("Incorrect password!");
        return false;
    }
}

void HashTable::logout() {
    if (currentLoggedInUser != "") {
        printSuccess("User '" + currentLoggedInUser + "' logged out successfully!");
        currentLoggedInUser = "";
        currentUserRole = "";
    }
}

string HashTable::getCurrentUser() {
    return currentLoggedInUser;
}

string HashTable::getCurrentUserRole() {
    return currentUserRole;
}

bool HashTable::isLoggedIn() {
    return currentLoggedInUser != "";
}

void HashTable::updateProfile(string userName, string department, string email) {
    HashEntry* user = searchUser(userName);
    if (user == NULL) {
        printError("User '" + userName + "' not found!");
        return;
    }

    user->department = department;
    user->email = email;
    printSuccess("Profile updated successfully!");
}

void HashTable::deleteUser(string userName, Graph* graph) {
    int index = hashFunction(userName);
    HashEntry* current = table[index];
    HashEntry* prev = NULL;

    while (current != NULL) {
        if (current->userName == userName) {
            if (graph != NULL) {
                graph->cancelAllReservationsByUser(userName);
            }

            if (prev == NULL) {
                table[index] = current->next;
            }
            else {
                prev->next = current->next;
            }
            delete current;
            size--;
            printSuccess("User '" + userName + "' deleted successfully!");
            printInfo("All room reservations by this user have been cancelled.");
            return;
        }
        prev = current;
        current = current->next;
    }

    printError("User '" + userName + "' not found!");
}

HashEntry* HashTable::searchUser(string userName) {
    int index = hashFunction(userName);
    HashEntry* current = table[index];

    while (current != NULL) {
        if (current->userName == userName) {
            return current;
        }
        current = current->next;
    }

    return NULL;
}

void HashTable::displayAllUsers() {
    if (isEmpty()) {
        printInfo("No users registered in the system.");
        return;
    }

    printHeader("ALL REGISTERED USERS");
    int userCount = 0;

    for (int i = 0; i < capacity; i++) {
        if (table[i] != NULL) {
            cout << MAGENTA << "\nBucket [" << i << "]:" << RESET << endl;
            HashEntry* current = table[i];
            while (current != NULL) {
                userCount++;
                cout << CYAN << "  " << userCount << ". Username: " << BOLD << current->userName << RESET << endl;
                cout << "     Role: " << current->role << endl;
                cout << "     Department: " << current->department << endl;
                cout << "     Email: " << current->email << endl;
                cout << "     Last Booked Room: " << (current->lastBookedRoom.empty() ? "None" : current->lastBookedRoom) << endl;
                printSeparator();
                current = current->next;
            }
        }
    }
    cout << GREEN << "\nTotal Users: " << userCount << RESET << endl;
}

void HashTable::updateLastBookedRoom(string userName, string roomID) {
    HashEntry* user = searchUser(userName);
    if (user != NULL) {
        user->lastBookedRoom = roomID;
    }
}

bool HashTable::isEmpty() {
    return size == 0;
}

void HashTable::sendNotification(string userName, string message) {
    HashEntry* user = searchUser(userName);
    if (user != NULL) {
        cout << YELLOW << "\n[NOTIFICATION] " << BOLD << "@" << userName << RESET << YELLOW << ": " << message << RESET << endl;
    }
}

// ==================== MODULE B: GRAPH ====================

Graph::Graph() {
    head = NULL;
}

Graph::~Graph() {
    while (head != NULL) {
        Building* temp = head;
        head = head->next;

        Edge* edge = temp->edgeList;
        while (edge != NULL) {
            Edge* edgeTemp = edge;
            edge = edge->next;
            delete edgeTemp;
        }

        delete temp->roomTree;
        delete temp;
    }
}

Building* Graph::findBuilding(string name) {
    Building* current = head;
    while (current != NULL) {
        if (current->buildingName == name) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

int Graph::graphHashFunction(string key) {
    int hash = 0;
    for (int i = 0; i < key.length(); i++) {
        hash = (hash + key[i]) % 1000;
    }
    return hash;
}

void Graph::addBuilding(string name) {
    if (findBuilding(name) != NULL) {
        printError("Building '" + name + "' already exists!");
        return;
    }

    Building* newBuilding = new Building();
    newBuilding->buildingName = name;
    newBuilding->roomTree = new AVLTree();
    newBuilding->next = head;
    head = newBuilding;

    printSuccess("Building '" + name + "' added successfully!");
}

void Graph::removeBuilding(string name, ComplaintQueue* queue) {
    Building* current = head;
    Building* prev = NULL;

    while (current != NULL) {
        if (current->buildingName == name) {
            if (queue != NULL) {
                queue->removeComplaintsByBuilding(name);
            }

            if (prev == NULL) {
                head = current->next;
            }
            else {
                prev->next = current->next;
            }

            Edge* edge = current->edgeList;
            while (edge != NULL) {
                Edge* temp = edge;
                edge = edge->next;
                delete temp;
            }

            delete current->roomTree;
            delete current;

            Building* temp = head;
            while (temp != NULL) {
                Edge* e = temp->edgeList;
                Edge* prevE = NULL;
                while (e != NULL) {
                    if (e->destBuildingName == name) {
                        if (prevE == NULL) {
                            temp->edgeList = e->next;
                        }
                        else {
                            prevE->next = e->next;
                        }
                        Edge* delE = e;
                        e = e->next;
                        delete delE;
                    }
                    else {
                        prevE = e;
                        e = e->next;
                    }
                }
                temp = temp->next;
            }

            printSuccess("Building '" + name + "' removed successfully!");
            printInfo("All rooms and related complaints have been removed.");
            return;
        }
        prev = current;
        current = current->next;
    }

    printError("Building '" + name + "' not found!");
}

void Graph::addPath(string src, string dest, int weight, string relation) {
    Building* srcBuilding = findBuilding(src);
    Building* destBuilding = findBuilding(dest);

    if (srcBuilding == NULL || destBuilding == NULL) {
        printError("One or both buildings not found!");
        return;
    }

    Edge* newEdge1 = new Edge();
    newEdge1->destBuildingName = dest;
    newEdge1->weight = weight;
    newEdge1->relation = relation;
    newEdge1->next = srcBuilding->edgeList;
    srcBuilding->edgeList = newEdge1;

    Edge* newEdge2 = new Edge();
    newEdge2->destBuildingName = src;
    newEdge2->weight = weight;
    newEdge2->relation = relation;
    newEdge2->next = destBuilding->edgeList;
    destBuilding->edgeList = newEdge2;

    printSuccess("Path added between '" + src + "' and '" + dest + "'!");
}

void Graph::removePath(string src, string dest) {
    Building* srcBuilding = findBuilding(src);
    Building* destBuilding = findBuilding(dest);

    if (srcBuilding == NULL || destBuilding == NULL) {
        printError("One or both buildings not found!");
        return;
    }

    Edge* current = srcBuilding->edgeList;
    Edge* prev = NULL;
    while (current != NULL) {
        if (current->destBuildingName == dest) {
            if (prev == NULL) {
                srcBuilding->edgeList = current->next;
            }
            else {
                prev->next = current->next;
            }
            delete current;
            break;
        }
        prev = current;
        current = current->next;
    }

    current = destBuilding->edgeList;
    prev = NULL;
    while (current != NULL) {
        if (current->destBuildingName == src) {
            if (prev == NULL) {
                destBuilding->edgeList = current->next;
            }
            else {
                prev->next = current->next;
            }
            delete current;
            break;
        }
        prev = current;
        current = current->next;
    }

    printSuccess("Path removed between '" + src + "' and '" + dest + "'!");
}

bool Graph::isConnected(string src, string dest) {
    Building* srcBuilding = findBuilding(src);
    if (srcBuilding == NULL) {
        return false;
    }

    Edge* current = srcBuilding->edgeList;
    while (current != NULL) {
        if (current->destBuildingName == dest) {
            return true;
        }
        current = current->next;
    }

    return false;
}

void Graph::BFS(string startBuilding) {
    Building* start = findBuilding(startBuilding);
    if (start == NULL) {
        printError("Building '" + startBuilding + "' not found!");
        return;
    }

    printHeader("BFS TRAVERSAL");
    cout << "Starting from: " << BOLD << startBuilding << RESET << "\n" << endl;

    bool* visited = new bool[1000];
    for (int i = 0; i < 1000; i++) {
        visited[i] = false;
    }

    QueueNode* front = NULL;
    QueueNode* rear = NULL;

    QueueNode* newNode = new QueueNode();
    newNode->buildingName = startBuilding;
    front = rear = newNode;
    visited[graphHashFunction(startBuilding)] = true;

    cout << CYAN << "Traversal Order: " << RESET;
    while (front != NULL) {
        string current = front->buildingName;
        cout << GREEN << current << RESET << " ";

        QueueNode* temp = front;
        front = front->next;
        delete temp;

        if (front == NULL) {
            rear = NULL;
        }

        Building* b = findBuilding(current);
        Edge* edge = b->edgeList;

        while (edge != NULL) {
            int idx = graphHashFunction(edge->destBuildingName);
            if (!visited[idx]) {
                visited[idx] = true;
                QueueNode* newN = new QueueNode();
                newN->buildingName = edge->destBuildingName;

                if (rear == NULL) {
                    front = rear = newN;
                }
                else {
                    rear->next = newN;
                    rear = newN;
                }
            }
            edge = edge->next;
        }
    }

    cout << endl;
    delete[] visited;
}

void Graph::DFS(string startBuilding) {
    Building* start = findBuilding(startBuilding);
    if (start == NULL) {
        printError("Building '" + startBuilding + "' not found!");
        return;
    }

    printHeader("DFS TRAVERSAL");
    cout << "Starting from: " << BOLD << startBuilding << RESET << "\n" << endl;

    bool* visited = new bool[1000];
    for (int i = 0; i < 1000; i++) {
        visited[i] = false;
    }

    StackNode* top = NULL;

    StackNode* newNode = new StackNode();
    newNode->buildingName = startBuilding;
    newNode->next = top;
    top = newNode;

    cout << CYAN << "Traversal Order: " << RESET;
    while (top != NULL) {
        string current = top->buildingName;
        StackNode* temp = top;
        top = top->next;
        delete temp;

        int idx = graphHashFunction(current);
        if (!visited[idx]) {
            visited[idx] = true;
            cout << GREEN << current << RESET << " ";

            Building* b = findBuilding(current);
            Edge* edge = b->edgeList;

            while (edge != NULL) {
                int edgeIdx = graphHashFunction(edge->destBuildingName);
                if (!visited[edgeIdx]) {
                    StackNode* newN = new StackNode();
                    newN->buildingName = edge->destBuildingName;
                    newN->next = top;
                    top = newN;
                }
                edge = edge->next;
            }
        }
    }

    cout << endl;
    delete[] visited;
}

void Graph::shortestPath(string src, string dest) {
    Building* srcBuilding = findBuilding(src);
    Building* destBuilding = findBuilding(dest);

    if (srcBuilding == NULL || destBuilding == NULL) {
        printError("One or both buildings not found!");
        return;
    }

    int* dist = new int[1000];
    bool* visited = new bool[1000];
    string* parent = new string[1000];

    for (int i = 0; i < 1000; i++) {
        dist[i] = 999999;
        visited[i] = false;
        parent[i] = "";
    }

    int srcIdx = graphHashFunction(src);
    dist[srcIdx] = 0;

    for (int count = 0; count < 100; count++) {
        int minDist = 999999;
        int minIdx = -1;
        string minBuilding = "";

        Building* temp = head;
        while (temp != NULL) {
            int idx = graphHashFunction(temp->buildingName);
            if (!visited[idx] && dist[idx] < minDist) {
                minDist = dist[idx];
                minIdx = idx;
                minBuilding = temp->buildingName;
            }
            temp = temp->next;
        }

        if (minIdx == -1) {
            break;
        }

        visited[minIdx] = true;

        Building* current = findBuilding(minBuilding);
        Edge* edge = current->edgeList;

        while (edge != NULL) {
            int destIdx = graphHashFunction(edge->destBuildingName);
            if (!visited[destIdx]) {
                int newDist = dist[minIdx] + edge->weight;
                if (newDist < dist[destIdx]) {
                    dist[destIdx] = newDist;
                    parent[destIdx] = minBuilding;
                }
            }
            edge = edge->next;
        }
    }

    int destIdx = graphHashFunction(dest);

    if (dist[destIdx] == 999999) {
        printInfo("No path exists between '" + src + "' and '" + dest + "'!");
    }
    else {
        printHeader("SHORTEST PATH (DIJKSTRA)");
        cout << "From: " << GREEN << src << RESET << " --> To: " << GREEN << dest << RESET << endl;
        cout << "Total Distance: " << YELLOW << dist[destIdx] << RESET << " units\n" << endl;

        string path[100];
        int pathLen = 0;
        string current = dest;

        while (current != src && current != "") {
            path[pathLen] = current;
            pathLen++;
            int idx = graphHashFunction(current);
            current = parent[idx];
        }
        path[pathLen] = src;
        pathLen++;

        cout << CYAN << "Path: " << RESET;
        for (int i = pathLen - 1; i >= 0; i--) {
            cout << BOLD << path[i] << RESET;
            if (i > 0) {
                cout << " -> ";
            }
        }
        cout << endl;
    }

    delete[] dist;
    delete[] visited;
    delete[] parent;
}

void Graph::displayAdjacencyList() {
    if (isEmpty()) {
        printInfo("No buildings in the campus map.");
        return;
    }

    printHeader("CAMPUS MAP - ADJACENCY LIST");
    Building* current = head;

    while (current != NULL) {
        cout << "\n" << BOLD << YELLOW << current->buildingName << RESET << " -->";
        Edge* edge = current->edgeList;

        if (edge == NULL) {
            cout << " " << RED << "[No connections]" << RESET;
        }

        while (edge != NULL) {
            cout << " " << CYAN << edge->destBuildingName << RESET
                << " [w=" << edge->weight
                << ", " << edge->relation
                << ", " << edge->status << "]";
            if (edge->next != NULL) {
                cout << " -->";
            }
            edge = edge->next;
        }
        cout << endl;
        current = current->next;
    }
    printSeparator();
}

Building* Graph::getBuildingNode(string name) {
    return findBuilding(name);
}

bool Graph::isEmpty() {
    return head == NULL;
}

void Graph::cancelAllReservationsByUser(string userName) {
    Building* current = head;
    while (current != NULL) {
        if (current->roomTree != NULL) {
            current->roomTree->cancelAllReservationsByUser(userName);
        }
        current = current->next;
    }
}

// ==================== MODULE C: AVL TREE ====================

AVLTree::AVLTree() {
    root = NULL;
}

AVLTree::~AVLTree() {
    destroyTree(root);
}

void AVLTree::destroyTree(AVLNode* node) {
    if (node != NULL) {
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }
}

int AVLTree::getHeight(AVLNode* node) {
    if (node == NULL) {
        return 0;
    }
    return node->height;
}

int AVLTree::getBalance(AVLNode* node) {
    if (node == NULL) {
        return 0;
    }
    return getHeight(node->left) - getHeight(node->right);
}

AVLNode* AVLTree::rightRotate(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = 1 + (getHeight(y->left) > getHeight(y->right) ? getHeight(y->left) : getHeight(y->right));
    x->height = 1 + (getHeight(x->left) > getHeight(x->right) ? getHeight(x->left) : getHeight(x->right));

    return x;
}

AVLNode* AVLTree::leftRotate(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = 1 + (getHeight(x->left) > getHeight(x->right) ? getHeight(x->left) : getHeight(x->right));
    y->height = 1 + (getHeight(y->left) > getHeight(y->right) ? getHeight(y->left) : getHeight(y->right));

    return y;
}

AVLNode* AVLTree::insertNode(AVLNode* node, string roomID, int floorNumber, string roomType) {
    if (node == NULL) {
        AVLNode* newNode = new AVLNode();
        newNode->roomID = roomID;
        newNode->floorNumber = floorNumber;
        newNode->roomType = roomType;
        return newNode;
    }

    if (roomID < node->roomID) {
        node->left = insertNode(node->left, roomID, floorNumber, roomType);
    }
    else if (roomID > node->roomID) {
        node->right = insertNode(node->right, roomID, floorNumber, roomType);
    }
    else {
        return node;
    }

    node->height = 1 + (getHeight(node->left) > getHeight(node->right) ? getHeight(node->left) : getHeight(node->right));

    int balance = getBalance(node);

    if (balance > 1 && roomID < node->left->roomID) {
        return rightRotate(node);
    }

    if (balance < -1 && roomID > node->right->roomID) {
        return leftRotate(node);
    }

    if (balance > 1 && roomID > node->left->roomID) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    if (balance < -1 && roomID < node->right->roomID) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

void AVLTree::insertRoom(string roomID, int floorNumber, string roomType) {
    if (searchRoom(roomID) != NULL) {
        printError("Room '" + roomID + "' already exists!");
        return;
    }
    root = insertNode(root, roomID, floorNumber, roomType);
    printSuccess("Room '" + roomID + "' inserted successfully!");
}

AVLNode* AVLTree::minValueNode(AVLNode* node) {
    AVLNode* current = node;
    while (current->left != NULL) {
        current = current->left;
    }
    return current;
}

AVLNode* AVLTree::deleteNode(AVLNode* node, string roomID) {
    if (node == NULL) {
        return node;
    }

    if (roomID < node->roomID) {
        node->left = deleteNode(node->left, roomID);
    }
    else if (roomID > node->roomID) {
        node->right = deleteNode(node->right, roomID);
    }
    else {
        if (node->left == NULL || node->right == NULL) {
            AVLNode* temp = node->left ? node->left : node->right;

            if (temp == NULL) {
                temp = node;
                node = NULL;
            }
            else {
                *node = *temp;
            }
            delete temp;
        }
        else {
            AVLNode* temp = minValueNode(node->right);
            node->roomID = temp->roomID;
            node->floorNumber = temp->floorNumber;
            node->roomType = temp->roomType;
            node->isReserved = temp->isReserved;
            node->reservedBy = temp->reservedBy;
            node->right = deleteNode(node->right, temp->roomID);
        }
    }

    if (node == NULL) {
        return node;
    }

    node->height = 1 + (getHeight(node->left) > getHeight(node->right) ? getHeight(node->left) : getHeight(node->right));

    int balance = getBalance(node);

    if (balance > 1 && getBalance(node->left) >= 0) {
        return rightRotate(node);
    }

    if (balance > 1 && getBalance(node->left) < 0) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    if (balance < -1 && getBalance(node->right) <= 0) {
        return leftRotate(node);
    }

    if (balance < -1 && getBalance(node->right) > 0) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

void AVLTree::deleteRoom(string roomID) {
    if (searchRoom(roomID) == NULL) {
        printError("Room '" + roomID + "' not found!");
        return;
    }
    root = deleteNode(root, roomID);
    printSuccess("Room '" + roomID + "' deleted successfully!");
}

AVLNode* AVLTree::searchNode(AVLNode* node, string roomID) {
    if (node == NULL || node->roomID == roomID) {
        return node;
    }

    if (roomID < node->roomID) {
        return searchNode(node->left, roomID);
    }

    return searchNode(node->right, roomID);
}

AVLNode* AVLTree::searchRoom(string roomID) {
    return searchNode(root, roomID);
}

void AVLTree::searchByTypeUtil(AVLNode* node, string type) {
    if (node == NULL) {
        return;
    }

    searchByTypeUtil(node->left, type);

    if (node->roomType == type) {
        cout << CYAN << "  Room ID: " << BOLD << node->roomID << RESET << endl;
        cout << "  Floor: " << node->floorNumber << endl;
        cout << "  Type: " << node->roomType << endl;
        cout << "  Reserved: " << (node->isReserved ? "Yes (by " + node->reservedBy + ")" : "No") << endl;
        printSeparator();
    }

    searchByTypeUtil(node->right, type);
}

void AVLTree::searchRoomsByType(string type) {
    if (isEmpty()) {
        printInfo("No rooms available in this building.");
        return;
    }

    printHeader("ROOMS OF TYPE: " + type);
    searchByTypeUtil(root, type);
}

void AVLTree::reserveRoom(string roomID, string userName) {
    AVLNode* room = searchRoom(roomID);
    if (room == NULL) {
        printError("Room '" + roomID + "' not found!");
        return;
    }

    if (room->isReserved) {
        printError("Room '" + roomID + "' is already reserved by '" + room->reservedBy + "'!");
        return;
    }

    room->isReserved = true;
    room->reservedBy = userName;
    printSuccess("Room '" + roomID + "' reserved successfully by '" + userName + "'!");
}

void AVLTree::cancelReservation(string roomID) {
    AVLNode* room = searchRoom(roomID);
    if (room == NULL) {
        printError("Room '" + roomID + "' not found!");
        return;
    }

    if (!room->isReserved) {
        printError("Room '" + roomID + "' is not reserved!");
        return;
    }

    room->isReserved = false;
    room->reservedBy = "";
    printSuccess("Reservation for room '" + roomID + "' cancelled successfully!");
}

void AVLTree::inorderUtil(AVLNode* node) {
    if (node == NULL) {
        return;
    }

    inorderUtil(node->left);
    cout << CYAN << "  Room: " << BOLD << node->roomID << RESET << " | Floor: " << node->floorNumber
        << " | Type: " << node->roomType
        << " | Reserved: " << (node->isReserved ? "Yes" : "No") << endl;
    inorderUtil(node->right);
}

void AVLTree::printInorder() {
    if (isEmpty()) {
        printInfo("No rooms available in this building.");
        return;
    }
    printHeader("INORDER TRAVERSAL");
    inorderUtil(root);
}

void AVLTree::preorderUtil(AVLNode* node) {
    if (node == NULL) {
        return;
    }

    cout << CYAN << "  Room: " << BOLD << node->roomID << RESET << " | Floor: " << node->floorNumber
        << " | Type: " << node->roomType
        << " | Reserved: " << (node->isReserved ? "Yes" : "No") << endl;
    preorderUtil(node->left);
    preorderUtil(node->right);
}

void AVLTree::printPreorder() {
    if (isEmpty()) {
        printInfo("No rooms available in this building.");
        return;
    }
    printHeader("PREORDER TRAVERSAL");
    preorderUtil(root);
}

void AVLTree::postorderUtil(AVLNode* node) {
    if (node == NULL) {
        return;
    }

    postorderUtil(node->left);
    postorderUtil(node->right);
    cout << CYAN << "  Room: " << BOLD << node->roomID << RESET << " | Floor: " << node->floorNumber
        << " | Type: " << node->roomType
        << " | Reserved: " << (node->isReserved ? "Yes" : "No") << endl;
}

void AVLTree::printPostorder() {
    if (isEmpty()) {
        printInfo("No rooms available in this building.");
        return;
    }
    printHeader("POSTORDER TRAVERSAL");
    postorderUtil(root);
}

int AVLTree::heightTree() {
    return getHeight(root);
}

int AVLTree::countLeavesUtil(AVLNode* node) {
    if (node == NULL) {
        return 0;
    }

    if (node->left == NULL && node->right == NULL) {
        return 1;
    }

    return countLeavesUtil(node->left) + countLeavesUtil(node->right);
}

int AVLTree::countLeaves() {
    return countLeavesUtil(root);
}

int AVLTree::countInternalUtil(AVLNode* node) {
    if (node == NULL || (node->left == NULL && node->right == NULL)) {
        return 0;
    }

    return 1 + countInternalUtil(node->left) + countInternalUtil(node->right);
}

int AVLTree::countInternalNodes() {
    return countInternalUtil(root);
}

void AVLTree::cancelAllReservationsByUserUtil(AVLNode* node, string userName) {
    if (node == NULL) {
        return;
    }

    cancelAllReservationsByUserUtil(node->left, userName);

    if (node->isReserved && node->reservedBy == userName) {
        node->isReserved = false;
        node->reservedBy = "";
    }

    cancelAllReservationsByUserUtil(node->right, userName);
}

void AVLTree::cancelAllReservationsByUser(string userName) {
    cancelAllReservationsByUserUtil(root, userName);
}

bool AVLTree::isEmpty() {
    return root == NULL;
}

// ==================== MODULE D: COMPLAINT QUEUE ====================

ComplaintQueue::ComplaintQueue() {
    front = NULL;
    rear = NULL;
    nextTicketID = 1;
    count = 0;
}

ComplaintQueue::~ComplaintQueue() {
    while (front != NULL) {
        Complaint* temp = front;
        front = front->next;
        delete temp;
    }
}

void ComplaintQueue::enqueue(string raisedBy, string building, string roomID, string description, string timestamp) {
    Complaint* newComplaint = new Complaint();
    newComplaint->ticketID = nextTicketID;
    nextTicketID++;
    newComplaint->raisedBy = raisedBy;
    newComplaint->building = building;
    newComplaint->roomID = roomID;
    newComplaint->description = description;
    newComplaint->timestamp = timestamp;

    if (rear == NULL) {
        front = rear = newComplaint;
    }
    else {
        rear->next = newComplaint;
        rear = newComplaint;
    }

    count++;
    printSuccess("Complaint submitted successfully!");
    cout << YELLOW << "Your Ticket ID: #" << newComplaint->ticketID << RESET << endl;
}

Complaint ComplaintQueue::dequeue() {
    Complaint result;

    if (front == NULL) {
        printInfo("No complaints in queue!");
        result.ticketID = -1;
        return result;
    }

    Complaint* temp = front;
    result.ticketID = temp->ticketID;
    result.raisedBy = temp->raisedBy;
    result.building = temp->building;
    result.roomID = temp->roomID;
    result.description = temp->description;
    result.timestamp = temp->timestamp;

    front = front->next;

    if (front == NULL) {
        rear = NULL;
    }

    delete temp;
    count--;

    return result;
}

Complaint ComplaintQueue::peek() {
    Complaint result;

    if (front == NULL) {
        printInfo("No complaints in queue!");
        result.ticketID = -1;
        return result;
    }

    result.ticketID = front->ticketID;
    result.raisedBy = front->raisedBy;
    result.building = front->building;
    result.roomID = front->roomID;
    result.description = front->description;
    result.timestamp = front->timestamp;

    return result;
}

void ComplaintQueue::displayQueue() {
    if (front == NULL) {
        printInfo("No pending complaints!");
        return;
    }

    printHeader("PENDING COMPLAINTS");
    Complaint* current = front;
    int index = 1;

    while (current != NULL) {
        cout << "\n" << YELLOW << "Complaint #" << index << ":" << RESET << endl;
        cout << CYAN << "  Ticket ID: " << BOLD << current->ticketID << RESET << endl;
        cout << "  Raised By: " << current->raisedBy << endl;
        cout << "  Building: " << current->building << endl;
        cout << "  Room ID: " << current->roomID << endl;
        cout << "  Description: " << current->description << endl;
        cout << "  Timestamp: " << current->timestamp << endl;
        printSeparator();
        current = current->next;
        index++;
    }
}

void ComplaintQueue::displayStatistics() {
    printHeader("COMPLAINT STATISTICS");
    cout << "\n" << CYAN << "Total Pending Complaints: " << BOLD << count << RESET << endl;
    cout << CYAN << "Next Ticket ID: " << BOLD << nextTicketID << RESET << endl;
    cout << CYAN << "Queue Status: " << (isEmpty() ? RED "Empty" : GREEN "Active") << RESET << endl;
    printSeparator();
}

bool ComplaintQueue::isEmpty() {
    return front == NULL;
}

void ComplaintQueue::removeComplaintsByBuilding(string buildingName) {
    if (front == NULL) {
        return;
    }

    while (front != NULL && front->building == buildingName) {
        Complaint* temp = front;
        front = front->next;
        delete temp;
        count--;
    }

    if (front == NULL) {
        rear = NULL;
        return;
    }

    Complaint* current = front;
    while (current->next != NULL) {
        if (current->next->building == buildingName) {
            Complaint* temp = current->next;
            current->next = temp->next;
            if (temp == rear) {
                rear = current;
            }
            delete temp;
            count--;
        }
        else {
            current = current->next;
        }
    }
}

// ==================== MODULE E: MESSAGING SYSTEM ====================

MessagingSystem::MessagingSystem() {
    head = NULL;
}

MessagingSystem::~MessagingSystem() {
    while (head != NULL) {
        UserConversations* tempUser = head;
        head = head->next;

        ConversationNode* conv = tempUser->conversationList;
        while (conv != NULL) {
            Message* msg = conv->messageStackTop;
            while (msg != NULL) {
                Message* tempMsg = msg;
                msg = msg->next;
                delete tempMsg;
            }
            ConversationNode* tempConv = conv;
            conv = conv->next;
            delete tempConv;
        }
        delete tempUser;
    }
}

MessagingSystem::UserConversations* MessagingSystem::findUser(string userName) {
    UserConversations* current = head;
    while (current != NULL) {
        if (current->userName == userName) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

ConversationNode* MessagingSystem::findConversation(UserConversations* user, string friendName) {
    ConversationNode* current = user->conversationList;
    while (current != NULL) {
        if (current->friendName == friendName) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void MessagingSystem::pushMessage(Message*& stackTop, Message* newMsg) {
    newMsg->next = stackTop;
    stackTop = newMsg;
}

Message* MessagingSystem::popMessage(Message*& stackTop) {
    if (stackTop == NULL) {
        return NULL;
    }

    Message* temp = stackTop;
    stackTop = stackTop->next;
    return temp;
}

Message* MessagingSystem::peekMessage(Message* stackTop) {
    return stackTop;
}

void MessagingSystem::sendMessage(string fromUser, string toUser, string messageText, string timestamp) {
    UserConversations* fromUserConv = findUser(fromUser);
    if (fromUserConv == NULL) {
        fromUserConv = new UserConversations();
        fromUserConv->userName = fromUser;
        fromUserConv->next = head;
        head = fromUserConv;
    }

    UserConversations* toUserConv = findUser(toUser);
    if (toUserConv == NULL) {
        toUserConv = new UserConversations();
        toUserConv->userName = toUser;
        toUserConv->next = head;
        head = toUserConv;
    }

    ConversationNode* fromConv = findConversation(fromUserConv, toUser);
    if (fromConv == NULL) {
        fromConv = new ConversationNode();
        fromConv->friendName = toUser;
        fromConv->next = fromUserConv->conversationList;
        fromUserConv->conversationList = fromConv;
    }

    ConversationNode* toConv = findConversation(toUserConv, fromUser);
    if (toConv == NULL) {
        toConv = new ConversationNode();
        toConv->friendName = fromUser;
        toConv->next = toUserConv->conversationList;
        toUserConv->conversationList = toConv;
    }

    Message* newMsg1 = new Message();
    newMsg1->fromUser = fromUser;
    newMsg1->toUser = toUser;
    newMsg1->messageText = messageText;
    newMsg1->timestamp = timestamp;
    pushMessage(fromConv->messageStackTop, newMsg1);

    Message* newMsg2 = new Message();
    newMsg2->fromUser = fromUser;
    newMsg2->toUser = toUser;
    newMsg2->messageText = messageText;
    newMsg2->timestamp = timestamp;
    pushMessage(toConv->messageStackTop, newMsg2);

    printSuccess("Message sent successfully!");
}

void MessagingSystem::viewLatestMessage(string user1, string user2) {
    UserConversations* userConv = findUser(user1);
    if (userConv == NULL) {
        printError("User '" + user1 + "' has no conversations!");
        return;
    }

    ConversationNode* conv = findConversation(userConv, user2);
    if (conv == NULL || conv->messageStackTop == NULL) {
        printInfo("No messages found between '" + user1 + "' and '" + user2 + "'!");
        return;
    }

    Message* latest = peekMessage(conv->messageStackTop);
    if (latest != NULL) {
        printHeader("LATEST MESSAGE");
        cout << "\n" << CYAN << "From: " << BOLD << latest->fromUser << RESET << endl;
        cout << CYAN << "To: " << BOLD << latest->toUser << RESET << endl;
        cout << CYAN << "Message: " << WHITE << latest->messageText << RESET << endl;
        cout << CYAN << "Time: " << YELLOW << latest->timestamp << RESET << endl;
        printSeparator();
    }
}

void MessagingSystem::popLatestMessage(string user1, string user2) {
    UserConversations* userConv = findUser(user1);
    if (userConv == NULL) {
        printError("User '" + user1 + "' has no conversations!");
        return;
    }

    ConversationNode* conv = findConversation(userConv, user2);
    if (conv == NULL || conv->messageStackTop == NULL) {
        printInfo("No messages found between '" + user1 + "' and '" + user2 + "'!");
        return;
    }

    Message* popped = popMessage(conv->messageStackTop);
    if (popped != NULL) {
        printHeader("POPPED MESSAGE");
        cout << "\n" << CYAN << "From: " << BOLD << popped->fromUser << RESET << endl;
        cout << CYAN << "To: " << BOLD << popped->toUser << RESET << endl;
        cout << CYAN << "Message: " << WHITE << popped->messageText << RESET << endl;
        cout << CYAN << "Time: " << YELLOW << popped->timestamp << RESET << endl;
        printSeparator();
        printSuccess("Message removed from conversation!");
        delete popped;
    }
}

void MessagingSystem::displayConversation(string user1, string user2) {
    UserConversations* userConv = findUser(user1);
    if (userConv == NULL) {
        printInfo("No conversations found for user '" + user1 + "'!");
        return;
    }

    ConversationNode* conv = findConversation(userConv, user2);
    if (conv == NULL || conv->messageStackTop == NULL) {
        printInfo("No messages between '" + user1 + "' and '" + user2 + "'!");
        return;
    }

    printHeader("CONVERSATION: " + user1 + " <-> " + user2);
    Message* current = conv->messageStackTop;
    int msgCount = 0;

    while (current != NULL) {
        msgCount++;
        cout << "\n" << YELLOW << "Message #" << msgCount << ":" << RESET << endl;
        cout << CYAN << "  From: " << BOLD << current->fromUser << RESET << endl;
        cout << CYAN << "  To: " << BOLD << current->toUser << RESET << endl;
        cout << CYAN << "  Message: " << WHITE << current->messageText << RESET << endl;
        cout << CYAN << "  Time: " << YELLOW << current->timestamp << RESET << endl;
        printSeparator();
        current = current->next;
    }

    cout << GREEN << "\nTotal Messages: " << msgCount << " (Most recent first)" << RESET << endl;
}

bool MessagingSystem::hasMessages(string user1, string user2) {
    UserConversations* userConv = findUser(user1);
    if (userConv == NULL) {
        return false;
    }

    ConversationNode* conv = findConversation(userConv, user2);
    if (conv == NULL || conv->messageStackTop == NULL) {
        return false;
    }

    return true;
}