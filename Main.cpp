#include "header.h"

void userMenu(HashTable& hashTable, Graph& graph);
void campusMapMenu(Graph& graph, ComplaintQueue& complaintQueue);
void roomMenu(Graph& graph, HashTable& hashTable);
void complaintMenu(ComplaintQueue& queue, HashTable& hashTable, Graph& graph);
void messagingMenu(MessagingSystem& messaging, HashTable& hashTable);

int main() {
    HashTable hashTable;
    Graph graph;
    ComplaintQueue complaintQueue;
    MessagingSystem messagingSystem;

    int choice = 0;

    while (true) {
        clearScreen();
        printHeader("UNIVERSITY MANAGEMENT SYSTEM");

        // Display logged in user info
        if (hashTable.isLoggedIn()) {
            cout << "\n" << GREEN << "Logged in as: " << BOLD << hashTable.getCurrentUser()
                << RESET << GREEN << " [" << hashTable.getCurrentUserRole() << "]" << RESET << endl;
        }
        else {
            cout << "\n" << YELLOW << "Not logged in - Limited access" << RESET << endl;
        }

        cout << "\n";

        string role = hashTable.getCurrentUserRole();

        // Convert role to lowercase for comparison
        for (int i = 0; i < role.length(); i++) {
            if (role[i] >= 'A' && role[i] <= 'Z') {
                role[i] = role[i] + 32;
            }
        }

        // Display menu based on role
        cout << CYAN << "  1. " << WHITE << "User & Authentication" << RESET << endl;
        cout << CYAN << "  2. " << WHITE << "Campus Map" << RESET << endl;

        // Students can only view campus, complaints, and messaging
        if (role == "student" || role == "teacher" || role == "admin") {
            cout << CYAN << "  3. " << WHITE << "Complaints" << RESET << endl;
            cout << CYAN << "  4. " << WHITE << "Messaging" << RESET << endl;
        }

        // Teachers and Admin can book rooms
        if (role == "teacher" || role == "admin") {
            cout << CYAN << "  5. " << WHITE << "Rooms & Scheduling" << RESET << endl;
        }

        // Admin has full access (already shown above)

        // Logout option if logged in
        if (hashTable.isLoggedIn()) {
            cout << CYAN << "  6. " << YELLOW << "Logout" << RESET << endl;
        }

        cout << CYAN << "  0. " << RED << "Exit System" << RESET << endl;
        cout << "\n";
        printSeparator();

        choice = getIntInput(YELLOW "Enter your choice: " RESET);

        if (choice == 1) {
            userMenu(hashTable, graph);
        }
        else if (choice == 2) {
            campusMapMenu(graph, complaintQueue);
        }
        else if (choice == 3) {
            if (role == "student" || role == "teacher" || role == "admin") {
                complaintMenu(complaintQueue, hashTable, graph);
            }
            else {
                printError("Please login first to access this feature!");
                pauseScreen();
            }
        }
        else if (choice == 4) {
            if (role == "student" || role == "teacher" || role == "admin") {
                messagingMenu(messagingSystem, hashTable);
            }
            else {
                printError("Please login first to access this feature!");
                pauseScreen();
            }
        }
        else if (choice == 5) {
            if (role == "teacher" || role == "admin") {
                roomMenu(graph, hashTable);
            }
            else if (role == "student") {
                printError("Students cannot access room booking. Only teachers and admin can book rooms!");
                pauseScreen();
            }
            else {
                printError("Please login as teacher or admin to access this feature!");
                pauseScreen();
            }
        }
        else if (choice == 6) {
            if (hashTable.isLoggedIn()) {
                hashTable.logout();
                pauseScreen();
            }
            else {
                printError("Invalid choice!");
                pauseScreen();
            }
        }
        else if (choice == 0) {
            clearScreen();
            printHeader("THANK YOU!");
            cout << GREEN << "\nExiting University Management System..." << RESET << endl;
            cout << YELLOW << "Goodbye! Have a great day!" << RESET << endl;
            printSeparator();
            break;
        }
        else {
            printError("Invalid choice! Please select a valid option.");
            pauseScreen();
        }
    }

    return 0;
}

void userMenu(HashTable& hashTable, Graph& graph) {
    int choice = 0;

    while (true) {
        clearScreen();
        printHeader("USER & AUTHENTICATION");

        // Show current login status
        if (hashTable.isLoggedIn()) {
            cout << "\n" << GREEN << "Currently logged in as: " << BOLD << hashTable.getCurrentUser() << RESET << endl;
        }

        cout << "\n";
        cout << CYAN << "  1. " << WHITE << "Register New User" << RESET << endl;
        cout << CYAN << "  2. " << WHITE << "Login" << RESET << endl;

        if (hashTable.isLoggedIn()) {
            cout << CYAN << "  3. " << WHITE << "Update Profile" << RESET << endl;
            cout << CYAN << "  4. " << WHITE << "Search User" << RESET << endl;
            cout << CYAN << "  5. " << YELLOW << "Logout" << RESET << endl;

            // Only admin can delete users and view all users
            string role = hashTable.getCurrentUserRole();
            for (int i = 0; i < role.length(); i++) {
                if (role[i] >= 'A' && role[i] <= 'Z') {
                    role[i] = role[i] + 32;
                }
            }

            if (role == "admin") {
                cout << CYAN << "  6. " << WHITE << "Delete User (Admin)" << RESET << endl;
                cout << CYAN << "  7. " << WHITE << "Display All Users (Admin)" << RESET << endl;
            }
        }

        cout << CYAN << "  0. " << RED << "Back to Main Menu" << RESET << endl;
        cout << "\n";
        printSeparator();

        choice = getIntInput(YELLOW "Enter your choice: " RESET);

        if (choice == 1) {
            clearScreen();
            printHeader("REGISTER NEW USER");

            string userName, password, role, department, email;

            cout << CYAN << "\nEnter Username: " << RESET;
            cin >> userName;

            cout << CYAN << "Enter Password (min 8 chars, 1 uppercase, 1 lowercase): " << RESET;
            cin >> password;

            // Validate password
            if (!isValidPassword(password)) {
                printError("Invalid password! Must be at least 8 characters with 1 uppercase and 1 lowercase letter.");
                pauseScreen();
                continue;
            }

            cout << CYAN << "Enter Role (student/teacher/admin): " << RESET;
            cin >> role;

            // Validate role
            if (!isValidRole(role)) {
                printError("Invalid role! Must be 'student', 'teacher', or 'admin'.");
                pauseScreen();
                continue;
            }

            cout << CYAN << "Enter Department: " << RESET;
            cin.ignore();
            getline(cin, department);

            // Validate department is not just numbers
            bool isAllDigits = true;
            for (int i = 0; i < department.length(); i++) {
                if (!isdigit(department[i])) {
                    isAllDigits = false;
                    break;
                }
            }

            if (isAllDigits || department.empty()) {
                printError("Department cannot be empty or contain only numbers!");
                pauseScreen();
                continue;
            }

            cout << CYAN << "Enter Email (@gmail.com, @yahoo.com, @hotmail.com, @outlook.com, @edu.pk): " << RESET;
            cin >> email;

            // Validate email
            if (!isValidEmail(email)) {
                printError("Invalid email! Must include @ and end with a valid domain.");
                printInfo("Valid domains: @gmail.com, @yahoo.com, @hotmail.com, @outlook.com, @edu.pk");
                pauseScreen();
                continue;
            }

            hashTable.registerUser(userName, password, role, department, email);
            pauseScreen();
        }
        else if (choice == 2) {
            if (hashTable.isLoggedIn()) {
                printWarning("You are already logged in as '" + hashTable.getCurrentUser() + "'");
                printInfo("Please logout first to login with another account.");
                pauseScreen();
                continue;
            }

            clearScreen();
            printHeader("USER LOGIN");

            string userName, password;

            cout << CYAN << "\nEnter Username: " << RESET;
            cin >> userName;
            cout << CYAN << "Enter Password: " << RESET;
            cin >> password;

            if (hashTable.login(userName, password)) {
                printInfo("Redirecting to main menu...");
            }
            pauseScreen();
        }
        else if (choice == 3) {
            if (!hashTable.isLoggedIn()) {
                printError("Please login first!");
                pauseScreen();
                continue;
            }

            clearScreen();
            printHeader("UPDATE PROFILE");

            string userName, department, email;

            userName = hashTable.getCurrentUser();
            cout << YELLOW << "\nUpdating profile for: " << BOLD << userName << RESET << endl;
            cout << CYAN << "Enter New Department: " << RESET;
            cin.ignore();
            getline(cin, department);
            cout << CYAN << "Enter New Email: " << RESET;
            cin >> email;

            // Validate email
            if (!isValidEmail(email)) {
                printError("Invalid email format!");
                pauseScreen();
                continue;
            }

            hashTable.updateProfile(userName, department, email);
            pauseScreen();
        }
        else if (choice == 4) {
            if (!hashTable.isLoggedIn()) {
                printError("Please login first!");
                pauseScreen();
                continue;
            }

            clearScreen();
            printHeader("SEARCH USER");

            string userName;

            cout << CYAN << "\nEnter Username to search: " << RESET;
            cin >> userName;

            HashEntry* user = hashTable.searchUser(userName);
            if (user != NULL) {
                printSuccess("User Found!");
                printSeparator();
                cout << CYAN << "Username: " << BOLD << user->userName << RESET << endl;
                cout << CYAN << "Role: " << WHITE << user->role << RESET << endl;
                cout << CYAN << "Department: " << WHITE << user->department << RESET << endl;
                cout << CYAN << "Email: " << WHITE << user->email << RESET << endl;
                cout << CYAN << "Last Booked Room: " << WHITE << (user->lastBookedRoom.empty() ? "None" : user->lastBookedRoom) << RESET << endl;
                printSeparator();
            }
            else {
                printError("User '" + userName + "' not found!");
            }
            pauseScreen();
        }
        else if (choice == 5) {
            if (hashTable.isLoggedIn()) {
                hashTable.logout();
                pauseScreen();
                break; // Return to main menu after logout
            }
            else {
                printError("Invalid choice!");
                pauseScreen();
            }
        }
        else if (choice == 6) {
            string role = hashTable.getCurrentUserRole();
            for (int i = 0; i < role.length(); i++) {
                if (role[i] >= 'A' && role[i] <= 'Z') {
                    role[i] = role[i] + 32;
                }
            }

            if (role != "admin") {
                printError("Access Denied! Only admins can delete users.");
                pauseScreen();
                continue;
            }

            clearScreen();
            printHeader("DELETE USER");

            string userName;

            cout << CYAN << "\nEnter Username to delete: " << RESET;
            cin >> userName;

            hashTable.deleteUser(userName, &graph);
            pauseScreen();
        }
        else if (choice == 7) {
            string role = hashTable.getCurrentUserRole();
            for (int i = 0; i < role.length(); i++) {
                if (role[i] >= 'A' && role[i] <= 'Z') {
                    role[i] = role[i] + 32;
                }
            }

            if (role != "admin") {
                printError("Access Denied! Only admins can view all users.");
                pauseScreen();
                continue;
            }

            clearScreen();
            hashTable.displayAllUsers();
            pauseScreen();
        }
        else if (choice == 0) {
            break;
        }
        else {
            printError("Invalid choice! Please select a valid option.");
            pauseScreen();
        }
    }
}

void campusMapMenu(Graph& graph, ComplaintQueue& complaintQueue) {
    int choice = 0;

    while (true) {
        clearScreen();
        printHeader("CAMPUS MAP");
        cout << "\n";
        cout << CYAN << "  1. " << WHITE << "Add Building" << RESET << endl;
        cout << CYAN << "  2. " << WHITE << "Remove Building" << RESET << endl;
        cout << CYAN << "  3. " << WHITE << "Add Path Between Buildings" << RESET << endl;
        cout << CYAN << "  4. " << WHITE << "Remove Path" << RESET << endl;
        cout << CYAN << "  5. " << WHITE << "Find Shortest Path (Dijkstra)" << RESET << endl;
        cout << CYAN << "  6. " << WHITE << "BFS from Building" << RESET << endl;
        cout << CYAN << "  7. " << WHITE << "DFS from Building" << RESET << endl;
        cout << CYAN << "  8. " << WHITE << "Display Adjacency List" << RESET << endl;
        cout << CYAN << "  9. " << RED << "Back to Main Menu" << RESET << endl;
        cout << "\n";
        printSeparator();

        choice = getIntInput(YELLOW "Enter your choice (1-9): " RESET);

        if (choice == 1) {
            clearScreen();
            printHeader("ADD BUILDING");

            string buildingName;

            cout << CYAN << "\nEnter Building Name: " << RESET;
            cin.ignore();
            getline(cin, buildingName);

            graph.addBuilding(buildingName);
            pauseScreen();
        }
        else if (choice == 2) {
            clearScreen();
            printHeader("REMOVE BUILDING");

            string buildingName;

            cout << CYAN << "\nEnter Building Name to remove: " << RESET;
            cin.ignore();
            getline(cin, buildingName);

            graph.removeBuilding(buildingName, &complaintQueue);
            pauseScreen();
        }
        else if (choice == 3) {
            clearScreen();
            printHeader("ADD PATH");

            string src, dest, relation;
            int weight;

            cout << CYAN << "\nEnter Source Building: " << RESET;
            cin.ignore();
            getline(cin, src);
            cout << CYAN << "Enter Destination Building: " << RESET;
            getline(cin, dest);

            weight = getIntInput(CYAN "Enter Weight (distance in meters): " RESET);

            cout << CYAN << "Enter Relation (road/walkway): " << RESET;
            cin >> relation;

            graph.addPath(src, dest, weight, relation);
            pauseScreen();
        }
        else if (choice == 4) {
            clearScreen();
            printHeader("REMOVE PATH");

            string src, dest;

            cout << CYAN << "\nEnter Source Building: " << RESET;
            cin.ignore();
            getline(cin, src);
            cout << CYAN << "Enter Destination Building: " << RESET;
            getline(cin, dest);

            graph.removePath(src, dest);
            pauseScreen();
        }
        else if (choice == 5) {
            clearScreen();

            string src, dest;

            cout << CYAN << "\nEnter Source Building: " << RESET;
            cin.ignore();
            getline(cin, src);
            cout << CYAN << "Enter Destination Building: " << RESET;
            getline(cin, dest);

            graph.shortestPath(src, dest);
            pauseScreen();
        }
        else if (choice == 6) {
            clearScreen();

            string buildingName;

            cout << CYAN << "\nEnter Starting Building for BFS: " << RESET;
            cin.ignore();
            getline(cin, buildingName);

            graph.BFS(buildingName);
            pauseScreen();
        }
        else if (choice == 7) {
            clearScreen();

            string buildingName;

            cout << CYAN << "\nEnter Starting Building for DFS: " << RESET;
            cin.ignore();
            getline(cin, buildingName);

            graph.DFS(buildingName);
            pauseScreen();
        }
        else if (choice == 8) {
            clearScreen();
            graph.displayAdjacencyList();
            pauseScreen();
        }
        else if (choice == 9) {
            break;
        }
        else {
            printError("Invalid choice! Please enter a number between 1-9.");
            pauseScreen();
        }
    }
}

void roomMenu(Graph& graph, HashTable& hashTable) {
    int choice = 0;
    string selectedBuilding = "";

    while (true) {
        clearScreen();
        printHeader("ROOMS & SCHEDULING (AVL Tree)");
        cout << "\nCurrently Selected Building: " << (selectedBuilding.empty() ? "[None Selected]" : selectedBuilding) << "\n" << endl;
        cout << CYAN << " 1. " << WHITE "Select Building" RESET << endl;
        cout << CYAN << " 2. " << WHITE "Insert Room" RESET << endl;
        cout << CYAN << " 3. " << WHITE "Delete Room" RESET << endl;
        cout << CYAN << " 4. " << WHITE "Search Room" RESET << endl;
        cout << CYAN << " 5. " << WHITE "Search Rooms by Type" RESET << endl;
        cout << CYAN << " 6. " << WHITE "Reserve Room" RESET << endl;
        cout << CYAN << " 7. " << WHITE "Cancel Reservation" RESET << endl;
        cout << CYAN << " 8. " << WHITE "Print Inorder Traversal" RESET << endl;
        cout << CYAN << " 9. " << WHITE "Print Preorder Traversal" RESET << endl;
        cout << CYAN << " 10. " << WHITE "Print Postorder Traversal" RESET << endl;
        cout << CYAN << " 11. " << RED "Back to Main Menu" RESET << endl;
        cout << "\n";
        printSeparator();
        cout << YELLOW "Enter your choice (1-11): " RESET;
        cin >> choice;

        if (choice == 1) {
            clearScreen();
            printHeader("SELECT BUILDING");

            cout << "\nEnter Building Name: ";
            cin.ignore();
            getline(cin, selectedBuilding);

            Building* building = graph.getBuildingNode(selectedBuilding);
            if (building == NULL) {
                cout << "\n[ERROR] Building '" << selectedBuilding << "' not found!" << endl;
                selectedBuilding = "";
            }
            else {
                cout << "\n[SUCCESS] Building '" << selectedBuilding << "' selected!" << endl;
            }
            pauseScreen();
        }
        else if (choice >= 2 && choice <= 10) {
            if (selectedBuilding.empty()) {
                cout << "\n[ERROR] Please select a building first!" << endl;
                pauseScreen();
                continue;
            }

            Building* building = graph.getBuildingNode(selectedBuilding);

            if (choice == 2) {
                clearScreen();
                printHeader("INSERT ROOM");

                string roomID, roomType;
                int floorNumber;

                cout << "\nBuilding: " << selectedBuilding << endl;
                cout << "Enter Room ID: ";
                cin >> roomID;
                cout << "Enter Floor Number: ";
                cin >> floorNumber;
                cout << "Enter Room Type (classroom/lab/office): ";
                cin >> roomType;

                building->roomTree->insertRoom(roomID, floorNumber, roomType);
                pauseScreen();
            }
            else if (choice == 3) {
                clearScreen();
                printHeader("DELETE ROOM");

                string roomID;

                cout << "\nBuilding: " << selectedBuilding << endl;
                cout << "Enter Room ID to delete: ";
                cin >> roomID;

                building->roomTree->deleteRoom(roomID);
                pauseScreen();
            }
            else if (choice == 4) {
                clearScreen();
                printHeader("SEARCH ROOM");

                string roomID;

                cout << "\nBuilding: " << selectedBuilding << endl;
                cout << "Enter Room ID to search: ";
                cin >> roomID;

                AVLNode* room = building->roomTree->searchRoom(roomID);

                if (room != NULL) {
                    cout << "\n[SUCCESS] Room Found!" << endl;
                    printSeparator();
                    cout << "Room ID: " << room->roomID << endl;
                    cout << "Floor: " << room->floorNumber << endl;
                    cout << "Type: " << room->roomType << endl;
                    cout << "Reserved: " << (room->isReserved ? "Yes (by " + room->reservedBy + ")" : "No") << endl;
                    printSeparator();
                }
                else {
                    cout << "\n[ERROR] Room '" << roomID << "' not found!" << endl;
                }
                pauseScreen();
            }
            else if (choice == 5) {
                clearScreen();

                string roomType;

                cout << "\nBuilding: " << selectedBuilding << endl;
                cout << "Enter Room Type to search: ";
                cin >> roomType;

                building->roomTree->searchRoomsByType(roomType);
                pauseScreen();
            }
            else if (choice == 6) {
                clearScreen();
                printHeader("RESERVE ROOM");

                string roomID, userName;

                cout << "\nBuilding: " << selectedBuilding << endl;
                cout << "Enter Room ID to reserve: ";
                cin >> roomID;
                cout << "Enter Username: ";
                cin >> userName;

                HashEntry* user = hashTable.searchUser(userName);
                if (user == NULL) {
                    cout << "\n[ERROR] User '" << userName << "' not found! Please register first." << endl;
                }
                else {
                    building->roomTree->reserveRoom(roomID, userName);
                    hashTable.updateLastBookedRoom(userName, roomID);
                }
                pauseScreen();
            }
            else if (choice == 7) {
                clearScreen();
                printHeader("CANCEL RESERVATION");

                string roomID;

                cout << "\nBuilding: " << selectedBuilding << endl;
                cout << "Enter Room ID to cancel reservation: ";
                cin >> roomID;

                building->roomTree->cancelReservation(roomID);
                pauseScreen();
            }
            else if (choice == 8) {
                clearScreen();
                building->roomTree->printInorder();
                pauseScreen();
            }
            else if (choice == 9) {
                clearScreen();
                building->roomTree->printPreorder();
                pauseScreen();
            }
            else if (choice == 10) {
                clearScreen();
                building->roomTree->printPostorder();
                pauseScreen();
            }
        }
        else if (choice == 11) {
            break;
        }
        else {
            cout << "\n[ERROR] Invalid choice! Please enter a number between 1-11." << endl;
            pauseScreen();
        }
    }
}

void complaintMenu(ComplaintQueue& queue, HashTable& hashTable, Graph& graph) {
    int choice = 0;

    while (true) {
        clearScreen();
        printHeader("COMPLAINT MANAGEMENT");
        cout << "\n";
        cout << CYAN << "  1." WHITE << " Submit New Complaint" RESET << endl;
        cout << CYAN << "  2." WHITE << " Process Next Complaint" RESET << endl;
        cout << CYAN << "  3." WHITE << " View Next Complaint"  RESET << endl;
        cout << CYAN << "  4." WHITE << " Display All Pending Complaints" RESET << endl;
        cout << CYAN << "  5." WHITE << " Complaint Statistics" RESET << endl;
        cout << CYAN << "  6." RED << " Back to Main Menu" RESET << endl;
        cout << "\n";
        printSeparator();
        cout << YELLOW "Enter your choice (1-6): " RESET;
        cin >> choice;

        if (choice == 1) {
            clearScreen();
            printHeader("SUBMIT NEW COMPLAINT");

            string raisedBy, building, roomID, description;

            cout << "\nEnter Your Username: ";
            cin >> raisedBy;

            HashEntry* user = hashTable.searchUser(raisedBy);
            if (user == NULL) {
                cout << "\n[ERROR] User '" << raisedBy << "' not found! Please register first." << endl;
                pauseScreen();
                continue;
            }

            cout << "Enter Building Name: ";
            cin.ignore();
            getline(cin, building);

            // Validate building exists
            Building* buildingNode = graph.getBuildingNode(building);
            if (buildingNode == NULL) {
                cout << "\n[ERROR] Building '" << building << "' not found in campus map!" << endl;
                cout << "Please add the building first in Campus Map section." << endl;
                pauseScreen();
                continue;
            }

            cout << "Enter Room ID: ";
            cin >> roomID;
            cout << "Enter Complaint Description: ";
            cin.ignore();
            getline(cin, description);

            string timestamp = getCurrentTimestamp();
            queue.enqueue(raisedBy, building, roomID, description, timestamp);
            pauseScreen();
        }
        else if (choice == 2) {
            clearScreen();
            printHeader("PROCESS COMPLAINT");

            Complaint c = queue.dequeue();
            if (c.ticketID != -1) {
                cout << "\n[SUCCESS] Complaint Processed!" << endl;
                printSeparator();
                cout << "Ticket ID: " << c.ticketID << endl;
                cout << "Raised By: " << c.raisedBy << endl;
                cout << "Building: " << c.building << endl;
                cout << "Room ID: " << c.roomID << endl;
                cout << "Description: " << c.description << endl;
                cout << "Timestamp: " << c.timestamp << endl;
                printSeparator();
            }
            pauseScreen();
        }
        else if (choice == 3) {
            clearScreen();
            printHeader("VIEW NEXT COMPLAINT");

            Complaint c = queue.peek();
            if (c.ticketID != -1) {
                cout << "\n[INFO] Next Complaint in Queue:" << endl;
                printSeparator();
                cout << "Ticket ID: " << c.ticketID << endl;
                cout << "Raised By: " << c.raisedBy << endl;
                cout << "Building: " << c.building << endl;
                cout << "Room ID: " << c.roomID << endl;
                cout << "Description: " << c.description << endl;
                cout << "Timestamp: " << c.timestamp << endl;
                printSeparator();
            }
            pauseScreen();
        }
        else if (choice == 4) {
            clearScreen();
            queue.displayQueue();
            pauseScreen();
        }
        else if (choice == 5) {
            clearScreen();
            queue.displayStatistics();
            pauseScreen();
        }
        else if (choice == 6) {
            break;
        }
        else {
            cout << "\n[ERROR] Invalid choice! Please enter a number between 1-6." << endl;
            pauseScreen();
        }
    }
}

void messagingMenu(MessagingSystem& messaging, HashTable& hashTable) {
    int choice = 0;

    while (true) {
        clearScreen();
        printHeader("MESSAGING SYSTEM");
        cout << "\n";
        cout << CYAN << "  1." WHITE << " Send Message" RESET << endl;
        cout << CYAN << "  2." WHITE << " View Latest Message" RESET << endl;
        cout << CYAN << "  3." WHITE << " Pop Latest Message" RESET << endl;
        cout << CYAN << "  4." WHITE << " Display Full Conversation" RESET << endl;
        cout << CYAN << "  5." RED << " Back to Main Menu" RESET << endl;
        cout << "\n";
        printSeparator();
        cout << YELLOW "Enter your choice (1-5): " RESET;
        cin >> choice;

        if (choice == 1) {
            clearScreen();
            printHeader("SEND MESSAGE");

            string fromUser, toUser, messageText;

            cout << "\nEnter Your Username: ";
            cin >> fromUser;

            HashEntry* sender = hashTable.searchUser(fromUser);
            if (sender == NULL) {
                cout << "\n[ERROR] User '" << fromUser << "' not found! Please register first." << endl;
                pauseScreen();
                continue;
            }

            cout << "Enter Recipient Username: ";
            cin >> toUser;

            HashEntry* recipient = hashTable.searchUser(toUser);
            if (recipient == NULL) {
                cout << "\n[ERROR] User '" << toUser << "' not found!" << endl;
                pauseScreen();
                continue;
            }

            cout << "Enter Message: ";
            cin.ignore();
            getline(cin, messageText);

            string timestamp = getCurrentTimestamp();
            messaging.sendMessage(fromUser, toUser, messageText, timestamp);
            pauseScreen();
        }
        else if (choice == 2) {
            clearScreen();
            printHeader("VIEW LATEST MESSAGE");

            string user1, user2;

            cout << "\nEnter First Username: ";
            cin >> user1;
            cout << "Enter Second Username: ";
            cin >> user2;

            messaging.viewLatestMessage(user1, user2);
            pauseScreen();
        }
        else if (choice == 3) {
            clearScreen();
            printHeader("POP LATEST MESSAGE");

            string user1, user2;

            cout << "\nEnter First Username: ";
            cin >> user1;
            cout << "Enter Second Username: ";
            cin >> user2;

            messaging.popLatestMessage(user1, user2);
            pauseScreen();
        }
        else if (choice == 4) {
            clearScreen();

            string user1, user2;

            cout << "\nEnter First Username: ";
            cin >> user1;
            cout << "Enter Second Username: ";
            cin >> user2;

            messaging.displayConversation(user1, user2);
            pauseScreen();
        }
        else if (choice == 5) {
            break;
        }
        else {
            cout << "\n[ERROR] Invalid choice! Please enter a number between 1-5." << endl;
            pauseScreen();
        }
    }
}