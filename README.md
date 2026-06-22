# 🎓 University Management System (DSA)

A comprehensive University Management System developed in **C++** using custom implementations of **Hash Tables, Graphs, AVL Trees, Queues, and Stacks**. This project demonstrates the practical application of Data Structures and Algorithms in managing university operations through a menu-driven console application.

---

## 📖 Overview

The University Management System is a console-based application designed to simulate various university management tasks. The project focuses on implementing fundamental data structures from scratch without using STL containers such as `vector`, `list`, `map`, `stack`, or `queue`.

The system integrates multiple modules including user management, campus navigation, room scheduling, complaint handling, and messaging services.

---

## ✨ Features

### 👤 User & Authentication Module (Hash Table)

* Register new users
* User login and authentication
* Update user profiles
* Search users
* Delete users
* Collision handling using chaining
* Dynamic hash table management

### 🏢 Campus Map Module (Graph)

* Add and remove buildings
* Add and remove paths between buildings
* Breadth First Search (BFS)
* Depth First Search (DFS)
* Connectivity checking
* Shortest path calculation using Dijkstra's Algorithm

### 🏫 Rooms & Scheduling Module (AVL Tree)

* Insert rooms
* Delete rooms
* Search rooms
* Search rooms by type
* Reserve rooms
* Cancel reservations
* Inorder, Preorder, and Postorder traversals
* Balanced AVL Tree operations

### 📋 Complaint Management Module (Queue)

* Submit maintenance complaints
* Process complaints in FIFO order
* View pending complaints
* Display complaint statistics

### 💬 Messaging System (Stack)

* Send messages between users
* View latest messages
* Maintain conversation history
* Notification management using stacks

### 🔗 Integration Features

* Room reservation tracking
* Automatic update of user booking history
* Data consistency across modules
* User validation before operations

---

## 🧩 Data Structures Used

| Data Structure | Purpose                           |
| -------------- | --------------------------------- |
| Linked List    | Base structure for all modules    |
| Hash Table     | User directory and authentication |
| Graph          | Campus map representation         |
| AVL Tree       | Room scheduling and management    |
| Queue          | Complaint processing              |
| Stack          | Messaging and notifications       |

---

## 📂 Project Structure

```text
University-Management-System-DSA/
│
├── header.h
│   ├── Structure definitions
│   ├── Class declarations
│   └── Function prototypes
│
├── source.cpp
│   ├── Hash Table implementation
│   ├── Graph implementation
│   ├── AVL Tree implementation
│   ├── Queue implementation
│   └── Stack implementation
│
├── main.cpp
│   ├── Menu-driven interface
│   └── Program execution
│
└── README.md
```

---

## 🛠 Technologies Used

* C++
* Object-Oriented Programming (OOP)
* Data Structures & Algorithms
* Linked Lists
* Hash Tables
* Graphs
* AVL Trees
* Queues
* Stacks

---

## 🚀 Learning Outcomes

This project helped in understanding and implementing:

* Custom Data Structure Design
* Hashing Techniques
* Collision Resolution through Chaining
* Graph Traversal Algorithms (BFS & DFS)
* Dijkstra's Shortest Path Algorithm
* Self-Balancing AVL Trees
* Queue and Stack Operations
* Multi-Module System Integration
* Dynamic Memory Management

---

## ▶️ How to Run

### Compile

```bash
g++ main.cpp source.cpp -o UniversityManagementSystem
```

### Execute

```bash
./UniversityManagementSystem
```

For Visual Studio:

1. Create a C++ Console Project.
2. Add `header.h`, `source.cpp`, and `main.cpp`.
3. Build and Run the project.

---

## 📌 Project Highlights

* Fully menu-driven console application
* No STL containers used for core data structures
* Custom implementations of all major data structures
* Modular and maintainable code structure
* Real-world application of Data Structures concepts

---

## 🎯 Academic Information

**Course:** Data Structures Lab

**Project Title:** University Management System

**Language:** C++

**Platform:** Console Application

**Institution:** FAST National University of Computer and Emerging Sciences

---

## 👨‍💻 Author

**Muhammad Hamza Afzaal**

BS Computer Science

FAST National University of Computer and Emerging Sciences

---

## 📜 License

This project was developed for academic and educational purposes.
