// Name        : ProjectTwo.cpp
// Author      : Samantha Vaillancourt
// Date        : April 19, 2026
// Description : This program manages a course catalog using a data structure to store course information.

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

// Define Course Data Structure
struct Course {
   std::string courseNumber;
    std::string courseTitle;
    std::vector<std::string> prerequisites;
};

// Node Structure for Binary Search Tree
struct Node {
    Course course;
    Node* left;
    Node* right;
    Node(Course c) : course(c), left(nullptr), right(nullptr) {}
};

// Insert Course into Binary Search Tree
Node* insert(Node* root, Course newCourse) {
    if (root == nullptr) {
        return new Node(newCourse);
    }
    if (newCourse.courseNumber < root->course.courseNumber) {
        root->left = insert(root->left, newCourse);
    }
    else if (newCourse.courseNumber > root->course.courseNumber) {
        root->right = insert(root->right, newCourse);
    }

    return root;
}

// Search for Course in Binary Search Tree
Course* findCourse(Node* root, string courseNumber) {
    if (root == nullptr) {
        return nullptr;
    }
    if (courseNumber == root->course.courseNumber) {
        return &root->course;
    }
    else if (courseNumber < root->course.courseNumber) {
        return findCourse(root->left, courseNumber);
    }
    else {
        return findCourse(root->right, courseNumber);
    }
}

// Print All Courses in Binary Search Tree (In-Order Traversal)
void printAllCourses(Node* node) {
    if (node != nullptr) {
        printAllCourses(node->left);
        cout << node->course.courseNumber << ": " << node->course.courseTitle << endl;

        printAllCourses(node->right);
    }
}

// Print Single Course Details
void printCourseInfo(Node* root, string courseNumber) {
    Course* course = findCourse(root, courseNumber);
    if (course == nullptr) {
        cout << "Course not found." << endl;
        return;
    }
    cout << course->courseNumber << ": " << course->courseTitle << endl;
    if (course->prerequisites.empty()) {
        cout << "Prerequisites: None" << endl;
    }
    else {
        cout << "Prerequisites:" << endl;
        for (std::string prereq : course->prerequisites) {
            Course* prereqCourse = findCourse(root, prereq);
            if (prereqCourse != nullptr) {
                cout << prereqCourse->courseNumber << ": " << prereqCourse->courseTitle << endl;
            }
        }
        cout << endl;
    }
}

// Load Courses from File
Node* loadCourses(string filename) {
    ifstream file(filename);
    string line;
    vector<string> courseNumbers;
    Node* root = nullptr;

    if (!file.is_open()) {
        cout << "Error: Cannot open file." << endl;
        return nullptr;
    }

    // First Pass: Read Course Numbers and Titles
    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        vector<string> tokens;

        while (getline(ss, token, ',')) {
            tokens.push_back(token);
        }
        if (tokens.size() < 2) {
            cout << "Invalid format, skipping line." << endl;
            continue;
            // Skip invalid lines
        }

        courseNumbers.push_back(tokens[0]);
    }
    // Reset file stream to beginning for second pass
    file.clear();
    file.seekg(0);

    // Second Pass: Read Prerequisites
    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        vector<string> tokens;

        while (getline(ss, token, ',')) {
            tokens.push_back(token);
        }
        if (tokens.size() < 2) {
            continue;
        }

        // Create Course object
        Course newCourse;
        newCourse.courseNumber = tokens[0];
        newCourse.courseTitle = tokens[1];

        // Read prerequisites
        for (int i = 2; i < tokens.size(); ++i) {
            if (find(courseNumbers.begin(), courseNumbers.end(), tokens[i]) != courseNumbers.end()) {
            newCourse.prerequisites.push_back(tokens[i]);
            }
            else {
                cout << "Invalid prerequisite: " << tokens[i] << endl;
            }
        }
        cout << endl;

        // Insert Course into BST
        root = insert(root, newCourse);
    }
    file.close();
    return root;
}

// Main Menu Function
int main() {
    Node* root = nullptr;
    int choice = 0;

    while (choice != 4) {
        cout << "\nMenu:" << endl;
        cout << "1. Load Data Structure" << endl;
        cout << "2. Print Course List" << endl;
        cout << "3. Print Course" << endl;
        cout << "4. Exit" << endl;
        cout << "Enter choice: ";

        // Validate user input
        if (!(cin >> choice)) {
            cout << "Invalid inpuit. Please enter a number between 1 and 4." << endl;
            cin.clear(); // Clear error state
            cin.ignore(1000, '\n'); // Discard invalid input
            continue;  
        }
        if (choice == 1) {
            string fileName;
            cout << "Enter file name: ";
            cin >> fileName;
            root = loadCourses(fileName);
            if (root != nullptr) {
                cout << "Data loaded successfully." << endl;
            }
        }
        else if (choice == 2) {
            if (root == nullptr) {
                cout << "No data loaded. Please load course data first." << endl;
            }
            else {
                printAllCourses(root);
            }
        }
        else if (choice == 3) {
            if (root == nullptr) {
                cout << "No data loaded. Please load data first." << endl;
                continue;
            }
            string courseNumber;
            cout << "Enter course number: ";
            cin >> courseNumber;

            // Convert to uppercase for case-insensitive search
            transform(courseNumber.begin(), courseNumber.end(), courseNumber.begin(), ::toupper);
            printCourseInfo(root, courseNumber);
        }
        else if (choice == 4) {
            cout << "Exiting program." << endl;
        }
        else {
            cout << "Invalid option. Please try again." << endl;
        }
    }
    return 0;
}
