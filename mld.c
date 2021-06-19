/*
 * ======================================================================================
 *
 * 	Filename:     mld.c
 * 	
 * 	Description:  This file implements the files and routines for mld library
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
#include <assert.h>
#include <memory.h>
#include "mld.h"
#include "colors.h"

char *DATA_TYPE[] = {"UINT8", "UINT32", "INT32", "CHAR", "OBJ_PTR", "VOID_PTR", "FLOAT", "DOUBLE", "OBJ_STRUCT"};

/* Dumping functions */

void print_structure_rec(struct_db_rec_t *struct_rec) {
	if(!struct_rec) return;

	field_info_t *field = NULL;
	printf(ANSI_COLOR_CYAN "|-------------------------------------------------------------------------------------------------|\n" ANSI_COLOR_RESET);
	printf(ANSI_COLOR_YELLOW " %20s | | size = %-8d | #flds = %-3d |\n ANSI_COLOR_RESET", struct_rec->struct_name, struct_rec->ds_size, struct_rec->n_fields);
	printf(ANSI_COLOR_CYAN "|---------------------------------------------------------|--------------------------------------------------------------------|\n" ANSI_COLOR_RESET);
	
	for(int i = 0; i < struct_rec->n_fields; i++) {
		field = &struct_rec->fields[i];
		printf(" %-20s |", "");
		printf("%-3d %-20s | dtype = %-15s | size = %-5d | offset = %-6d | nstructname = %-20s |\n",
				i, field->fname, DATA_TYPE[field->dtype], field->size, field->offset, field->nested_str_name);
		printf(" %-20s |", "");
		printf(ANSI_COLOR_CYAN "-------------------------------------------------------------------------------------------------------------------------|\n" ANSI_COLOR_RESET);
	}
}

void print_structure_db(struct_db_t *struct_db) {
	if(!struct_db) return;

	printf("Printing Structure Database");
	struct_db_rec_t *struct_rec = NULL;
	struct_rec = struct_db->head;
	printf("No of Structures Registered = %d\n", struct_db->count);
	int i = 0;
	while(struct_rec) {
		printf("structure no : %d (%p)\n", i++, struct_rec);
		print_structure_rec(struct_rec);
		struct_rec = struct_rec->next;
	}
}

int add_structure_to_struct_db(struct_db_t *struct_db, struct_db_rec_t *struct_rec) {
	struct_db_rec_t *head = struct_db->head;

	if(!head) {
		struct_db->head = struct_rec;
		struct_rec->next = NULL;
		struct_db->count++;
		return 0;
	}

	struct_rec->next = head;
	struct_db->head = struct_rec;
	struct_db->count++;
	return 0;
}
