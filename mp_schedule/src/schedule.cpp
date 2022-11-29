/**
 * @file schedule.cpp
 * Exam scheduling using graph coloring
 */

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <map>
#include <unordered_map>

#include "schedule.h"
#include "utils.h"
#include <algorithm>

using namespace std;

/**
 * Given a filename to a CSV-formatted text file, create a 2D vector of strings where each row
 * in the text file is a row in the V2D and each comma-separated value is stripped of whitespace
 * and stored as its own string. 
 * 
 * Your V2D should match the exact structure of the input file -- so the first row, first column
 * in the original file should be the first row, first column of the V2D.
 *  
 * @param filename The filename of a CSV-formatted text file. 
 */
V2D file_to_V2D(const std::string & filename){
    // Your code here!
    V2D v2d;

    string fileContent = file_to_string(filename);
    vector<string> lineContent;
    SplitString(fileContent, '\n', lineContent);

    for (string line : lineContent) {
        vector<string> wordContent;
        SplitString(line, ',', wordContent);
        vector<string> v1d;
        for (string word : wordContent) {
            word = Trim(word);
            v1d.push_back(word);
        }
        v2d.push_back(v1d);
    }

    return v2d;
}

/**
 * Given a course roster and a list of students and their courses, 
 * perform data correction and return a course roster of valid students (and only non-empty courses).
 * 
 * A 'valid student' is a student who is both in the course roster and the student's own listing contains the course
 * A course which has no students (or all students have been removed for not being valid) should be removed
 * 
 * @param cv A 2D vector of strings where each row is a course ID followed by the students in the course
 * @param student A 2D vector of strings where each row is a student ID followed by the courses they are taking
 */
V2D clean(const V2D & cv, const V2D & student){
    // YOUR CODE HERE
    map<string, vector<string>> courseStudentMap;
    map<string, vector<string>> studentCourseMap;
    vector<string> courseKeys;

    for (vector<string> row : cv) {
        string courseKey = row[0];
        courseKeys.push_back(courseKey);
        vector<string> students;
        for (size_t i = 1; i < row.size(); i++) {
            students.push_back(row[i]);
        }
        courseStudentMap[courseKey] = students;
    }

    for (vector<string> row : student) {
        string studentKey = row[0];
        vector<string> courses;
        for (size_t i = 1; i < row.size(); i++) {
            courses.push_back(row[i]);
        }
        studentCourseMap[studentKey] = courses;
    }

    V2D v2d;

    for (string course : courseKeys) {
        vector<string> v1d;
        v1d.push_back(course);
        for (string student : courseStudentMap[course]) {
            if (studentCourseMap.find(student) != studentCourseMap.end()) {
                if (find(studentCourseMap[student].begin(), studentCourseMap[student].end(), course) 
                != studentCourseMap[student].end()) {
                    v1d.push_back(student);
                }
            }
        }
        if (v1d.size() > 1) {
            v2d.push_back(v1d);
        }
    }

    return v2d;
}

/**
 * Given a collection of courses and a list of available times, create a valid scheduling (if possible).
 * 
 * A 'valid schedule' should assign each course to a timeslot in such a way that there are no conflicts for exams
 * In other words, two courses who share a student should not share an exam time.
 * Your solution should try to minimize the total number of timeslots but should not exceed the timeslots given.
 * 
 * The output V2D should have one row for each timeslot, even if that timeslot is not used.
 * 
 * As the problem is NP-complete, your first scheduling might not result in a valid match. Your solution should 
 * continue to attempt different schedulings until 1) a valid scheduling is found or 2) you have exhausted all possible
 * starting positions. If no match is possible, return a V2D with one row with the string '-1' as the only value. 
 * 
 * @param courses A 2D vector of strings where each row is a course ID followed by the students in the course
 * @param timeslots A vector of strings giving the total number of unique timeslots
 */
V2D schedule(const V2D &courses, const std::vector<std::string> &timeslots){
    // Your code here!
    map<string, vector<string>> studentCourseMap;
    vector<string> studentKeys;
    vector<string> courseKeys;

    for (vector<string> row : courses) {
        string courseKey = row[0];
        courseKeys.push_back(courseKey);
        vector<string> courseList;
        courseList.push_back(courseKey);
        for (size_t i = 1; i < row.size(); i++) {
            if (studentCourseMap.find(row[i]) != studentCourseMap.end()) {
                studentCourseMap[row[i]].push_back(courseKey);
            } else {
                studentCourseMap[row[i]] = courseList;
                studentKeys.push_back(row[i]);
            }
        }
    }

    map<string, set<string>> courseAdjacencyList;

    for (string student : studentKeys) {
        vector<string> courseList = studentCourseMap[student];

        for (size_t i = 0; i < courseList.size(); i++) {
            for (size_t j = i + 1 ; j < courseList.size(); j++) {
                if (courseAdjacencyList.find(courseList[i]) != courseAdjacencyList.end()) {
                    courseAdjacencyList[courseList[i]].insert(courseList[j]);
                } else {
                    set<string> courseInsert;
                    courseInsert.insert(courseList[j]);
                    courseAdjacencyList[courseList[i]] = courseInsert;
                }

                if (courseAdjacencyList.find(courseList[j]) != courseAdjacencyList.end()) {
                    courseAdjacencyList[courseList[j]].insert(courseList[i]);
                } else {
                    set<string> courseInsert;
                    courseInsert.insert(courseList[i]);
                    courseAdjacencyList[courseList[j]] = courseInsert;
                }
            }
        }
    }
    
    map<size_t, vector<string>> degreeCourseMap;
    size_t highestDegree = 0;

    for (string courseKey : courseKeys) {
        size_t courseKeySize = courseAdjacencyList[courseKey].size() - 1;

        if (degreeCourseMap.find(courseKeySize) != degreeCourseMap.end()) {
            degreeCourseMap[courseKeySize].push_back(courseKey);
        } else {
            vector<string> courseInsert;
            courseInsert.push_back(courseKey);
            degreeCourseMap[courseKeySize] = courseInsert;
        }

        if (courseKeySize > highestDegree) {
            highestDegree = courseKeySize;
        }
    }

    map<string, int> courseColor;
    size_t highestColor = timeslots.size();

    for (int i = highestDegree; i >= 0; i--) {
        vector<string> currentCourses = degreeCourseMap[i];
        for (string course : currentCourses) {
            vector<int> colorsUsed;
            colorsUsed.resize(highestColor, -1);

            set<string> neighboringCourses = courseAdjacencyList[course];

            for (string neighbor : neighboringCourses) {
                if (courseColor.find(neighbor) != courseColor.end()) {
                    colorsUsed[courseColor[neighbor]]++;
                }
            }

            for (int j = 0; j < (int) colorsUsed.size(); j++) {
                if (colorsUsed[j] == -1) {
                    courseColor[course] = j;
                    break;
                }
            }
        }
    }

    V2D v2d;

    if (courseKeys.size() != courseColor.size()) {
        vector<string> v1d;
        v1d.push_back("-1");
        v2d.push_back(v1d);
        return v2d;
    }
    

    for (int i = 0; i < (int) timeslots.size(); i++) {
        vector<string> v1d;
        v1d.push_back(timeslots[i]);

        for (string course : courseKeys) {
            if (courseColor[course] == i) {
                v1d.push_back(course);
            }
        }

        v2d.push_back(v1d);
    }

    return v2d;
}