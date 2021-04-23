#include "gradebook.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
    Peter Zhang
    11/22/2019
    Zybooks Gradebook
    This program has nine main functions and several other helper functions
        in order to make a functional Zybooks gradebook using linked lists.
*/

// unneeded
int checkStudentExist(Student *headStudentList, char last[], char first[]) {
    for (Student *cur = headStudentList; cur != NULL; cur = cur->next)
        if (strcmp(cur->firstName, first) == 0 &&
            strcmp(cur->lastName, last) == 0)
            return 1;

    return 0;
}

Student *getStudent(Student *headStudentList, char last[], char first[]) {
    for (Student *cur = headStudentList; cur != NULL; cur = cur->next)
        if (strcmp(cur->firstName, first) == 0 &&
            strcmp(cur->lastName, last) == 0)
            return cur;
    return NULL;
}
// finish this
Grade *getGrade(Student *headStudentGrade, char last[], char first[],
                char gradeName[]) {
    Student *selectedStudent = getStudent(headStudentGrade, last, first);
    if (selectedStudent != NULL) {
        for (Grade *cur = selectedStudent->headGradeList; cur != NULL;
             cur = cur->next)
            if (strcmp(cur->name, gradeName) == 0) return cur;
    }

    return NULL;
}

void printGrades(Student *student) {
    // printf("printing %s grades:", student->firstName);
    for (Grade *cur = student->headGradeList; cur != NULL; cur = cur->next) {
        printf("%s : %lf\n", cur->name, cur->value);
    }
}

Grade *makeGrade(Grade *next, char name[], double value) {
    Grade *gradeToAdd = malloc(sizeof(Grade));
    strcpy(gradeToAdd->name, name);
    gradeToAdd->value = value;
    gradeToAdd->next = next;
    return gradeToAdd;
}

Student *makeStudent(Student *next, char *last, char *first) {
    Student *studentToMake = malloc(sizeof(Student));
    studentToMake->lastName = malloc(sizeof(char) * strlen(last));
    studentToMake->firstName = malloc(sizeof(char) * strlen(first));
    strcpy(studentToMake->firstName, first);
    strcpy(studentToMake->lastName, last);
    studentToMake->next = next;
    studentToMake->headGradeList = NULL;
    return studentToMake;
}

int compareNames(Student *student, char last[], char first[]) {
    // before
    if (strcmp(student->lastName, last) < 0) return -1;
    // after
    if (strcmp(student->lastName, last) > 0) return 1;
    // same last name, check first
    if (strcmp(student->lastName, last) == 0) {
        if (strcmp(student->firstName, first) < 0) return -1;

        if (strcmp(student->firstName, first) > 0) return 1;
    }
    return -1;
}

int compareGrades(Grade *grade, char name[]) {
    const int pVal = 0, cVal = 1, LVal = 2;
    int ch, sec, ch2, sec2, val, val2;
    char gradeType, gradeType2;
    sscanf(name, "%c%d.%d", &gradeType, &ch, &sec);
    sscanf(grade->name, "%c%d.%d", &gradeType2, &ch2, &sec2);
    if (gradeType == 'P') val = pVal;
    if (gradeType == 'C') val = cVal;
    if (gradeType == 'L') val = LVal;

    if (gradeType2 == 'P') val2 = pVal;
    if (gradeType2 == 'C') val2 = cVal;
    if (gradeType2 == 'L') val2 = LVal;

    if (ch < ch2) return -1;
    if (ch > ch2) return 1;
    if (ch == ch2) {
        if (sec < sec2) return -1;
        if (sec > sec2) return 1;
        if (sec == sec2) {
            if (val < val2) return -1;
            if (val > val2) return 1;
        }
    }
    return -1;
}

Grade *addGradeOrderRec(Grade *gradeList, char gradeName[], double value) {
    if (gradeList == NULL) return makeGrade(NULL, gradeName, value);
    // update current
    if (strcmp(gradeList->name, gradeName) == 0) {
        gradeList->value = value;
        return gradeList;
    }
    if (compareGrades(gradeList, gradeName) < 0)
        return makeGrade(gradeList, gradeName, value);
    gradeList->next = addGradeOrderRec(gradeList->next, gradeName, value);
    return gradeList;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// add a student to the list of students
//	   1. Make sure a student by that name does not exist (print an error
//message and do nothing if it exists)
//	   2. Add a new student with that name (add-at-front)

Student *addStudent(Student *headStudentList, char last[], char first[]) {
    if (checkStudentExist(headStudentList, last, first)) {
        printf("Error: %s %s already exists\n", first, last);
        return headStudentList;
    }

    return makeStudent(headStudentList, last, first);
}

/////////////////////////////////////////////////////////////////////////////////////////////
// add a grade to the specified student
//   1. Make sure a student by that name exists (so you can add grade to it)
//   2. If the specifed grade already exists, update the grade's value to the
//   new value
//   3. Otherwise, add the grade to the end of the student's grade list
//   (add-at-end)

void addGrade(Student *headStudentList, char last[], char first[],
              char gradeName[], double value) {
    Student *studentToAdd = getStudent(headStudentList, last, first);
    if (studentToAdd != NULL) {
        Grade *gradeToAdd = makeGrade(NULL, gradeName, value);

        // if the grade does not exist
        if (getGrade(studentToAdd, last, first, gradeName) == NULL) {
            Grade *gradeList = studentToAdd->headGradeList;
            if (studentToAdd->headGradeList == NULL) {
                studentToAdd->headGradeList = gradeToAdd;
                // printf("1\n");
            } else {
                while (gradeList->next != NULL) gradeList = gradeList->next;
                gradeList->next = gradeToAdd;
                // printf("2\n");
            }
        } else  // update grade
        {
            getGrade(studentToAdd, last, first, gradeName)->value = value;
        }
    } else {
        printf("Student does not exist\n");
    }
    return;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// return the number of students in the list
// Iterate through the student list to count the number of students, return that
// count

int count(Student *headStudentList) {
    int num = 0;
    for (Student *cur = headStudentList; cur != NULL; cur = cur->next) num++;

    return num;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// print all the grades for a given student
//	1. Make sure a student with that name exists
//	2. Print the name of that student
//	3. Print all the "grade name: grade value" pairs for that student

void printStudent(Student *headStudentList, char last[], char first[]) {
    if (getStudent(headStudentList, last, first) != NULL) {
        printf("%s, %s:\n", last, first);
        printGrades(getStudent(headStudentList, last, first));
        return;
    }
    printf("%s %s does not exist\n", last, first);
    return;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// print all the students
// If the student list is empty, print a message to indicate it.
// Otherwise, for each student
//	   a. Print the name of that student
//	   b. Print all the "grade name: grade value" pairs for that student

void print(Student *headStudentList) {
    if (headStudentList == NULL) {
        printf("Student list is empty.\n");
        return;
    }
    for (Student *cur = headStudentList; cur != NULL; cur = cur->next) {
        printf("%s, %s:\n", cur->lastName, cur->firstName);
        printGrades(cur);
    }

    return;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// add a student (in alphabetical order)
//	1. Make sure a student by that name does not exist
//	2. Add a student with that name (add-in-order)

Student *addStudentOrdered(Student *headStudentList, char last[],
                           char first[]) {
    if (checkStudentExist(headStudentList, last, first)) {
        printf("Error: %s %s already exists\n", first, last);
        return headStudentList;
    }
    if (headStudentList == NULL) return makeStudent(NULL, last, first);
    if (compareNames(headStudentList, last, first) > 0)
        return makeStudent(headStudentList, last, first);
    headStudentList->next =
        addStudentOrdered(headStudentList->next, last, first);
    return headStudentList;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// adds a grade to the specified student in order
//	1. Make sure a student by that name exists (so you can add a grade to
//the student)
//	2. If the grade already exists, update the value
//	3. Otherwise, add the grade to the student (add-in-order)

void addGradeOrdered(Student *headStudentList, char last[], char first[],
                     char gradeName[], double value) {
    if (getStudent(headStudentList, last, first) == NULL) {
        printf("%s %s does not exist\n", first, last);
        return;
    }
    getStudent(headStudentList, last, first)->headGradeList = addGradeOrderRec(
        getStudent(headStudentList, last, first)->headGradeList, gradeName,
        value);
    return;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// remove a grade from a student
//	1. Make sure a student with that name exists
//	2. Make sure the grade exists in that student
//	3. Remove the grade from that student

void removeGrade(Student *headStudentList, char last[], char first[],
                 char gradeName[]) {
    if (getStudent(headStudentList, last, first) == NULL) {
        printf("%s %s does not exist\n", last, first);
        return;
    }
    if (getGrade(headStudentList, last, first, gradeName) == NULL) {
        printf("%s does not exist\n", gradeName);
        return;
    }
    if (strcmp(getStudent(headStudentList, last, first)->headGradeList->name,
               gradeName) == 0) {
        Grade *pDel = getStudent(headStudentList, last, first)->headGradeList;
        getStudent(headStudentList, last, first)->headGradeList =
            getStudent(headStudentList, last, first)->headGradeList->next;
        free(pDel);
        return;
    }
    for (Grade *cur = getStudent(headStudentList, last, first)->headGradeList;
         cur != NULL; cur = cur->next) {
        if (strcmp(cur->next->name, gradeName) == 0) {
            Grade *pDel = cur->next;
            cur->next = cur->next->next;
            free(pDel);
            return;
        }
    }
    return;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// remove a student
//	1. Make sure a student with that name exists
//	2. Remove that student and all the grades of that student

Student *removeStudent(Student *headStudentList, char last[], char first[]) {
    if (getStudent(headStudentList, last, first) == NULL) {
        printf("%s %s does not exist\n", last, first);
        return headStudentList;
    }
    for (Grade *cur = getStudent(headStudentList, last, first)->headGradeList;
         cur != NULL; cur = cur->next) {
        removeGrade(headStudentList, last, first, cur->name);
    }
    if (strcmp(headStudentList->firstName, first) == 0 &&
        strcmp(headStudentList->lastName, last) == 0) {
        Student *pDel = headStudentList;
        headStudentList = headStudentList->next;
        free(pDel);
        return headStudentList;
    }
    for (Student *cur = headStudentList; cur != NULL; cur = cur->next) {
        if (strcmp(cur->next->firstName, first) == 0 &&
            strcmp(cur->next->lastName, last) == 0) {
            Student *pDel = cur->next;
            cur->next = cur->next->next;
            free(pDel);
            return headStudentList;
        }
    }
    return NULL;
}