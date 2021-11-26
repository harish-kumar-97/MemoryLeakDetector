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

	printf("\n" ANSI_COLOR_RED "Printing Structure Database" ANSI_COLOR_RESET);
	struct_db_rec_t *struct_rec = NULL;
	struct_rec = struct_db->head;
	printf("\nNo of Structures Registered = %d\n", struct_db->count);
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

static struct_db_rec_t
*struct_db_look_up(struct_db_t *struct_db,
			char *struct_name) {
	struct_db_rec_t *head = struct_db->head;
	if(!head) return NULL;
	for(; head; head = head->next) {
		if(strncmp(head->struct_name, struct_name, MAX_STRUCTURE_NAME_SIZE) == 0)
			return head;
	}
	return NULL;
}

static object_db_rec_t
*object_db_look_up(object_db_t *object_db, void *ptr) {
	object_db_rec_t *head = object_db->head;
	if(!head) return NULL;
	for (; head; head = head->next) {
		if(head->ptr == ptr)
			return head;
	}
	return NULL;
}

/* To work with objects */
static void add_object_to_object_db(object_db_t *object_db,
				void *ptr,
				int units,
				struct_db_rec_t *struct_rec,
				mld_boolean_t is_root) {
	object_db_rec_t *obj_rec = object_db_look_up(object_db, ptr);
	/* Don't add same object twice */
	assert(!obj_rec);
	
	obj_rec = calloc(1, sizeof(object_db_rec_t));
	
	obj_rec->next = NULL;
	obj_rec->ptr = ptr;
	obj_rec->units = units;
	obj_rec->struct_rec = struct_rec;
	obj_rec->is_visited = MLD_FALSE;
	obj_rec->is_root = is_root;

	object_db_rec_t *head = object_db->head;

	if(!head) {
		object_db->head = obj_rec;
		object_db->count++;
		return;
	}
	obj_rec->next = head;
	object_db->head = obj_rec;
	object_db->count++;
}

void *xcalloc(object_db_t *object_db, char *struct_name, int uints) {
	struct_db_rec_t *struct_rec = struct_db_look_up(object_db->struct_db, struct_name);
	assert(struct_rec);
	
	void *ptr = calloc(uints, struct_rec->ds_size);
	add_object_to_object_db(object_db, ptr, uints, struct_rec, MLD_FALSE);
	return ptr;
}

/* Print functions for object database */
void print_object_rec(object_db_rec_t *obj_rec, int i) {
	if(!obj_rec) return;
	printf(ANSI_COLOR_MAGENTA "-------------------------------------------------------------------------------------------------------------|\n" ANSI_COLOR_RESET);
	printf(ANSI_COLOR_YELLOW "%-3d ptr = %-10p | next = %-10p | units = %-4d | struct_name = %-10s | is_root = %s |\n" ANSI_COLOR_RESET, i, obj_rec->ptr, obj_rec->next, obj_rec->units, obj_rec->struct_rec->struct_name, obj_rec->is_root ? "TRUE" : "FALSE");
	printf(ANSI_COLOR_MAGENTA "-------------------------------------------------------------------------------------------------------------|\n" ANSI_COLOR_RESET);
}

void print_object_db(object_db_t *object_db) {
	object_db_rec_t *head = object_db->head;
	unsigned int i = 0;
	printf("\n" ANSI_COLOR_CYAN "PRINTING OBJECT DATABASE\n" ANSI_COLOR_RESET);
	for(; head; head = head->next) {
		print_object_rec(head, i++);
	}
}

/* The global object of the application which is not created by xcalloc
 * should be registered with MLD using below API 
 */
void mld_register_global_object_as_root(object_db_t *object_db,
								void *objptr,
								char *struct_name,
								unsigned int units) {
	struct_db_rec_t *struct_rec = struct_db_look_up(object_db->struct_db, struct_name);
	assert(struct_rec);

	/* Create a new object record and add to object database */
	add_object_to_object_db(object_db, objptr, units, struct_rec, MLD_TRUE);
}

/* Application might create an object using xcalloc, but at the same time
 * the object can be root object. Use this API to override the object flags
 * for the object already present in object db */
void
mld_set_dynamic_db_object_as_root(object_db_t *object_db, void *obj_ptr) {
	object_db_rec_t *obj_rec = object_db_look_up(object_db, obj_ptr);
	assert(obj_rec);

	obj_rec->is_root = MLD_TRUE;
}

void init_mld_algorithm(object_db_t *object_db) {
	object_db_rec_t *obj_rec = object_db->head;
	while(obj_rec) {
		obj_rec->is_visited = MLD_FALSE;
		obj_rec = obj_rec->next;
	}
}
object_db_rec_t* 
get_next_root_object(object_db_t *object_db, object_db_rec_t *root_obj) {
	object_db_rec_t *root_obj_rec = root_obj ? root_obj->next : object_db->head;
	while(root_obj_rec) {
		if(root_obj_rec->is_root)
			return root_obj_rec;
		root_obj_rec = root_obj_rec->next;
	}
	return NULL;
}

static void mld_explore_objects_recursively(object_db_t *object_db,                
                                  object_db_rec_t *parent_obj_rec){      
                                                                         
      unsigned int i , n_fields;                                         
      char *parent_obj_ptr = NULL,                                       
           *child_obj_offset = NULL;                                     
      void *child_object_address = NULL;
      field_info_t *field_info = NULL;                                   
  
      object_db_rec_t *child_object_rec = NULL;                          
      struct_db_rec_t *parent_struct_rec = parent_obj_rec->struct_rec;   
  
      /*Parent object must have already visited*/                        
      assert(parent_obj_rec->is_visited);                                
      
      for( i = 0; i < parent_obj_rec->units; i++){                       
      
          parent_obj_ptr = (char *)(parent_obj_rec->ptr) + (i * parent_struct_rec->ds_size);
      
          for(n_fields = 0; n_fields < parent_struct_rec->n_fields; n_fields++){      
      
              field_info = &parent_struct_rec->fields[n_fields];         
                                                                         
              /*We are only concerned with fields which are pointer to   
               * other objects*/                                         
              switch(field_info->dtype){                                 
                  case UINT8:                                            
                  case UINT32:                                           
                  case INT32:                                            
                  case CHAR:                                             
                  case FLOAT:                                            
                  case DOUBLE:
                  case OBJ_STRUCT:
                      break;                                             
                  case OBJ_PTR:
                  default:
                    ;                                                  
                                                                         
                  /*child_obj_offset is the memory location inside parent object              
                   * where address of next level object is stored*/      
                  child_obj_offset = parent_obj_ptr + field_info->offset;
                  memcpy(&child_object_address, child_obj_offset, sizeof(void *));            
  
                  /*child_object_address now stores the address of the next object in the     
                   * graph. It could be NULL, Handle that as well*/
                  if(!child_object_address) continue;
      
                  child_object_rec = object_db_look_up(object_db, child_object_address);      
      
                  assert(child_object_rec);                              
                  /* Since we are able to reach this child object "child_object_rec"          
                   * from parent object "parent_obj_ptr", mark this      
                   * child object as visited and explore its children recirsively. 
                   * If this child object is already visited, then do nothing - avoid infinite loops*/
                  if(!child_object_rec->is_visited){
                      child_object_rec->is_visited = MLD_TRUE;           
                      mld_explore_objects_recursively(object_db, child_object_rec);               
                  }
                  else{
                      continue; /*Do nothing, explore next child object*/
                  }                                                      
              }                                                          
          }
      }
}

void run_mld_algorithm(object_db_t *object_db) {
	init_mld_algorithm(object_db);
	object_db_rec_t *root_obj = get_next_root_object(object_db, NULL);
	while(root_obj){
		if(root_obj->is_visited) {
			root_obj = get_next_root_object(object_db, root_obj);
			continue;
		}
		root_obj->is_visited = MLD_TRUE;
		mld_explore_objects_recursively(object_db, root_obj);
		root_obj = get_next_root_object(object_db, root_obj);
	}
}

void print_obj_rec(object_db_rec_t *obj_rec, int i) {
	printf(ANSI_COLOR_MAGENTA "-----------------------------------------------------------------------------------------------------|\n"ANSI_COLOR_RESET);
	printf(ANSI_COLOR_YELLOW "%-3d next: %-10p | ptr: %-10p | units: %-4d | struct_name: %-10s | is_root: %s |\n" ANSI_COLOR_RESET, i, obj_rec->next, obj_rec->ptr, obj_rec->units, obj_rec->struct_rec->struct_name, obj_rec->is_root ? "TRUE" : "FALSE");
	printf(ANSI_COLOR_MAGENTA "-----------------------------------------------------------------------------------------------------|\n"ANSI_COLOR_RESET);
}

static void
mld_dump_object_rec_detail(object_db_rec_t *obj_rec){

    int n_fields = obj_rec->struct_rec->n_fields;
    field_info_t *field = NULL;

    int units = obj_rec->units, obj_index = 0,
        field_index = 0;

    for(; obj_index < units; obj_index++){
        char *current_object_ptr = (char *)(obj_rec->ptr) + \
                        (obj_index * obj_rec->struct_rec->ds_size);

        for(field_index = 0; field_index < n_fields; field_index++){
            
            field = &obj_rec->struct_rec->fields[field_index];

            switch(field->dtype){
                case UINT8:
                case INT32:
                case UINT32:
                    printf("%s[%d]->%s = %d\n", obj_rec->struct_rec->struct_name, obj_index, field->fname, *(int *)(current_object_ptr + field->offset));
                    break;
                case CHAR:
                    printf("%s[%d]->%s = %s\n", obj_rec->struct_rec->struct_name, obj_index, field->fname, (char *)(current_object_ptr + field->offset));
                    break;
                case FLOAT:
                    printf("%s[%d]->%s = %f\n", obj_rec->struct_rec->struct_name, obj_index, field->fname, *(float *)(current_object_ptr + field->offset));
                    break;
                case DOUBLE:
                    printf("%s[%d]->%s = %f\n", obj_rec->struct_rec->struct_name, obj_index, field->fname, *(double *)(current_object_ptr + field->offset));
                    break;
                case OBJ_PTR:
                    printf("%s[%d]->%s = %p\n", obj_rec->struct_rec->struct_name, obj_index, field->fname,  (void *)*(int *)(current_object_ptr + field->offset));
                    break;
                case OBJ_STRUCT:
                    /*Later*/
                    break;
                default:
                    break;
            }
        }
    }
}

void report_leaked_objects(object_db_t *object_db) {
	object_db_rec_t *obj_rec = object_db->head;
	int i = 0;
	
	printf("Leaked Objects:\n");
	while(obj_rec) {
		if(!obj_rec->is_visited) {
			print_obj_rec(obj_rec, ++i);
			mld_dump_object_rec_detail(obj_rec);
		}
		obj_rec = obj_rec->next;
	}
}

void mld_set_dynamic_object_as_root(object_db_t *object_db, void *obj_ptr) {
	object_db_rec_t *obj_rec = object_db_look_up(object_db, obj_ptr);
	assert(obj_rec);
	obj_rec->is_root = MLD_TRUE;
}
