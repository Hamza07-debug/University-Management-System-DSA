#ifndef HEADER_H
#define HEADER_H

#include <iostream>
#include <string>
#include <iomanip>
#include <limits>
using namespace std;

// ==================== COLOR CODES ====================
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
#define BOLD    "\033[1m"

// ==================== BASE LINKED LIST NODE ====================
template <typename T>
struct Node {
    T data;
    Node* next;
    Node() : next(NULL) {}
};

// ==================== MODULE A: HASH TABLE WITH CHAINING ====================
struct HashEntry {
    string userName;
    string password;
    string role;
    string department;
    string email;
    string lastBookedRoom;
    HashEntry* next;

    HashEntry() {
        userName = "";
        password = "";
        role = "";
        department = "";
        email = "";
        lastBookedRoom = "";
        next = NULL;
    }
};

class HashTable {
private:
    HashEntry** table;
    int capacity;
    int size;
    string currentLoggedInUser;
    string currentUserRole;

    int hashFunction(string key);
    void resize();

public:
    HashTable();
    ~HashTable();

    void registerUser(string userName, string password, string role, string department, string email);
    bool login(string userName, string password);
    void logout();
    void updateProfile(string userName, string department, string email);
    void deleteUser(string userName, class Graph* graph);
    HashEntry* searchUser(string userName);
    void displayAllUsers();
    void updateLastBookedRoom(string userName, string roomID);
    bool isEmpty();
    void sendNotification(string userName, string message);
    string getCurrentUser();
    string getCurrentUserRole();
    bool isLoggedIn();
};

// ==================== MODULE B: GRAPH (CAMPUS MAP) ====================
struct Edge {
    string destBuildingName;
    string relation;
    string status;
    Edge* next;
    int weight;

    Edge() {
        destBuildingName = "";
        relation = "";
        status = "open";
        next = NULL;
        weight = 0;
    }
};

struct Building {
    string buildingName;
    Edge* edgeList;
    Building* next;
    class AVLTree* roomTree;

    Building() {
        buildingName = "";
        edgeList = NULL;
        next = NULL;
        roomTree = NULL;
    }
};

struct QueueNode {
    string buildingName;
    QueueNode* next;

    QueueNode() {
        buildingName = "";
        next = NULL;
    }
};

struct StackNode {
    string buildingName;
    StackNode* next;

    StackNode() {
        buildingName = "";
        next = NULL;
    }
};

class Graph {
private:
    Building* head;
    Building* findBuilding(string name);
    int graphHashFunction(string key);

public:
    Graph();
    ~Graph();

    void addBuilding(string name);
    void removeBuilding(string name, class ComplaintQueue* queue);
    void addPath(string src, string dest, int weight, string relation);
    void removePath(string src, string dest);
    bool isConnected(string src, string dest);
    void BFS(string startBuilding);
    void DFS(string startBuilding);
    void shortestPath(string src, string dest);
    void displayAdjacencyList();
    Building* getBuildingNode(string name);
    bool isEmpty();
    void cancelAllReservationsByUser(string userName);
};

// ==================== MODULE C: AVL TREE (ROOMS) ====================
struct AVLNode {
    string roomID;
    int floorNumber;
    string roomType;
    bool isReserved;
    string reservedBy;
    AVLNode* left;
    AVLNode* right;
    int height;

    AVLNode() {
        roomID = "";
        floorNumber = 0;
        roomType = "";
        isReserved = false;
        reservedBy = "";
        left = NULL;
        right = NULL;
        height = 1;
    }
};

class AVLTree {
private:
    AVLNode* root;

    int getHeight(AVLNode* node);
    int getBalance(AVLNode* node);
    AVLNode* rightRotate(AVLNode* y);
    AVLNode* leftRotate(AVLNode* x);
    AVLNode* insertNode(AVLNode* node, string roomID, int floorNumber, string roomType);
    AVLNode* deleteNode(AVLNode* node, string roomID);
    AVLNode* minValueNode(AVLNode* node);
    AVLNode* searchNode(AVLNode* node, string roomID);
    void searchByTypeUtil(AVLNode* node, string type);
    void inorderUtil(AVLNode* node);
    void preorderUtil(AVLNode* node);
    void postorderUtil(AVLNode* node);
    int countLeavesUtil(AVLNode* node);
    int countInternalUtil(AVLNode* node);
    void destroyTree(AVLNode* node);
    void cancelAllReservationsByUserUtil(AVLNode* node, string userName);

public:
    AVLTree();
    ~AVLTree();

    void insertRoom(string roomID, int floorNumber, string roomType);
    void deleteRoom(string roomID);
    AVLNode* searchRoom(string roomID);
    void searchRoomsByType(string type);
    void reserveRoom(string roomID, string userName);
    void cancelReservation(string roomID);
    void printInorder();
    void printPreorder();
    void printPostorder();
    int heightTree();
    int countLeaves();
    int countInternalNodes();
    void cancelAllReservationsByUser(string userName);
    bool isEmpty();
};

// ==================== MODULE D: QUEUE (COMPLAINTS) ====================
struct Complaint {
    int ticketID;
    string raisedBy;
    string building;
    string roomID;
    string description;
    string timestamp;
    Complaint* next;

    Complaint() {
        ticketID = 0;
        raisedBy = "";
        building = "";
        roomID = "";
        description = "";
        timestamp = "";
        next = NULL;
    }
};

class ComplaintQueue {
private:
    Complaint* front;
    Complaint* rear;
    int nextTicketID;
    int count;

public:
    ComplaintQueue();
    ~ComplaintQueue();

    void enqueue(string raisedBy, string building, string roomID, string description, string timestamp);
    Complaint dequeue();
    Complaint peek();
    void displayQueue();
    void displayStatistics();
    bool isEmpty();
    void removeComplaintsByBuilding(string buildingName);
};

// ==================== MODULE E: MESSAGING (STACKS) ====================
struct Message {
    string fromUser;
    string toUser;
    string messageText;
    string timestamp;
    Message* next;

    Message() {
        fromUser = "";
        toUser = "";
        messageText = "";
        timestamp = "";
        next = NULL;
    }
};

struct ConversationNode {
    string friendName;
    Message* messageStackTop;
    ConversationNode* next;

    ConversationNode() {
        friendName = "";
        messageStackTop = NULL;
        next = NULL;
    }
};

class MessagingSystem {
private:
    struct UserConversations {
        string userName;
        ConversationNode* conversationList;
        UserConversations* next;

        UserConversations() {
            userName = "";
            conversationList = NULL;
            next = NULL;
        }
    };

    UserConversations* head;

    UserConversations* findUser(string userName);
    ConversationNode* findConversation(UserConversations* user, string friendName);
    void pushMessage(Message*& stackTop, Message* newMsg);
    Message* popMessage(Message*& stackTop);
    Message* peekMessage(Message* stackTop);

public:
    MessagingSystem();
    ~MessagingSystem();

    void sendMessage(string fromUser, string toUser, string messageText, string timestamp);
    void viewLatestMessage(string user1, string user2);
    void popLatestMessage(string user1, string user2);
    void displayConversation(string user1, string user2);
    bool hasMessages(string user1, string user2);
};

// ==================== UTILITY FUNCTIONS ====================
void clearScreen();
void printHeader(string title);
void printSeparator();
void pauseScreen();
string getCurrentTimestamp();
bool isValidEmail(string email);
bool isValidPassword(string password);
bool isValidRole(string role);
int getIntInput(string prompt);
void clearInputBuffer();
void printSuccess(string message);
void printError(string message);
void printInfo(string message);
void printWarning(string message);

#endif