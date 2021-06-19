/*
 * ======================================================================================
 *
 * 	Filename:     app.c
 * 	
 * 	Description:  This file represents the application demonstrating the use of MLD library
 *
 *	Version: 1.0
 *	Created: Saturday 19 June 2021 04:24:10 IST
 *	Revision: 1.0
 *	Compiler: gcc
 *
 *	Author: Harish Kumar, Embedded System Software Devleloper, harishec031@gmail.com
 *	Company: Cisco Systems, Inc(Mar 2021-Present), NXP Semiconductors(Apr 2019-Dec 2020)
 *	
 *	Copyright (c) 2021 Harish Kumar

 *	Permission is hereby granted, free of charge, to any person obtaining a copy
 *	of this software and associated documentation files (the "Software"), to deal
 *	in the Software without restriction, including without limitation the rights
 *	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * 	copies of the Software, and to permit persons to whom the Software is
 *	furnished to do so, subject to the following conditions:

 *	The above copyright notice and this permission notice shall be included in all
 *	copies or substantial portions of the Software.

 *	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *	SOFTWARE.
 *
 * ========================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "mld.h"
#include <stdio.h>

/* Application Structures */

typedef struct emp_ {
	char emp_name[30];
	unsigned int emp_id;
	unsigned int age;
	struct emp_ *mgr;
	float salary;
	int *p;
} emp_t;

typedef struct student_ {
	char stud_name[32];
	unsigned int rollno;
	unsigned int age;
	float aggregate;
	struct student_ *best_colleage;
} student_t;


int main(int argc, char **argv) {
	/* Step 1: Initlialize a new structure database */
	struct_db_t *struct_db = calloc(1, sizeof(struct_db_t));
	mld_init_primitive_data_types_support(struct_db);

	/* Step 2: Create structure record fo structure emp_t */
	static field_info_t emp_fields[] = {
		FIELD_INFO(emp_t, emp_name, CHAR, 0);
		FIELD_INFO(emp_t, emp_id, UINT32, 0);
		FIELD_INFO(emp_t, age, UINT32, 0);
		FIELD_INFO(emp_t, mgr, OBJ_PTR, 0);
		FIELD_INFO(emp_t, salary, FLOAT, 0);
		FIELD_INFO(emp_t, p, OBJ_PTR, 0);

	}

	/* Step 3: Register the structure in structure database */
	REG_STRUCT(struct_db, emp_t, emp_fields);

	static field_info_t stud_fields[] = {
		FIELD_INFO(student_t, stud_name, CHAR, 0);
		FIELD_INFO(student_t, rollno, UINT32, 0);
		FIELD_INFO(student_t, age, UINT32, 0);
		FIELD_INFO(student_t, aggregate, FLOAT, 0);
		FIELD_INFO(student_t, best_colleage, OBJ_PTR, student_t);
	}
	REG_STRUCT(struct_db, student_t, stud_fields);
	
	/* Verify the structure database by printing it */
	print_structure_db(struct_db);
}
