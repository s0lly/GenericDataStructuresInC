#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>
#include <stddef.h>


typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef float f32;
typedef double f64;

typedef int32_t b32;


// TODO: Below functions require bounds checking etc, possibly via helper functions
// NOTE: IDs always start at 1, this allows us to identify unitialized IDs via a 0 result
#define GenerateStructWithGenerics(type, n, struc)\
typedef struct type##ID\
{\
	i32 val;\
} type##ID;\
typedef struct type struc type;\
typedef struct type##Array\
{\
	type##ID ids[n];\
	type##ID nextID;\
	type e[n];\
	i32 count;\
} type##Array;\
static i32 type##Array_GetCountMax(type##Array *arr)\
{\
	return (i32)(sizeof((arr)->e) / sizeof(((arr)->e)[0]));\
}\
static type * type##Array_GetElement(type##Array *arr, i32 index)\
{\
	return (&(arr)->e[index]);\
}\
static type * type##Array_GetElementAtCount(type##Array *arr)\
{\
	return (&(arr)->e[(arr)->count]);\
}\
static type * type##Array_PushData(type##Array *arr, type data)\
{\
	assert((arr)->count < type##Array_GetCountMax(arr));\
	(arr)->e[(arr)->count] = data;\
	(arr)->ids[(arr)->count] = (type##ID) { ((arr)->nextID.val + 1) };\
	(arr)->count++;\
	(arr)->nextID.val++;\
	return (&(arr)->e[(arr)->count - 1]);\
}\
static type * type##Array_PushZeroed(type##Array *arr)\
{\
	return type##Array_PushData(arr, (type) { 0 });\
}\
static void type##Array_ReplaceIndex(type##Array *arr, i32 index, type data)\
{\
	assert(index >= 0 && index < (arr)->count);\
	(arr)->e[index] = data;\
}\
static void type##Array_ResetIndex(type##Array *arr, i32 index)\
{\
	type##Array_ReplaceIndex(arr, index, (type) { 0 });\
	(arr)->ids[index].val = 0;\
}\
static type type##Array_Pop(type##Array *arr)\
{\
	type result = (arr)->e[(arr)->count - 1];\
	type##Array_ResetIndex(arr, (arr)->count - 1);\
	return result;\
}\
static i32 _##type##Array_GetIndexFromID(type##Array *arr, type##ID IDToCheck)\
{\
	for (i32 indexCounter = 0; indexCounter < (arr)->count; indexCounter++)\
	{\
		if ((arr)->ids[indexCounter].val == IDToCheck.val)\
			return indexCounter;\
	}\
	return -1;\
}\
static type * type##Array_GetElementFromID(type##Array *arr, type##ID IDToCheck)\
{\
	i32 index = _##type##Array_GetIndexFromID(arr, IDToCheck);\
	if (index == -1)\
	{\
		return 0;\
	}\
	return &((arr)->e[index]);\
}\
static type##ID type##Array_GetID(type##Array *arr, i32 index)\
{\
	return (arr)->ids[index];\
}\
static i32 type##Array_GetIndexFromElement(type##Array *arr, type *elem)\
{\
	return (i32)(elem - &(arr)->e[0]);\
}\
static type##ID type##Array_GetIDFromElement(type##Array *arr, type *elem)\
{\
	i32 index = type##Array_GetIndexFromElement(arr, elem);\
	return type##Array_GetID(arr, index);\
}\
static void type##Array_Swap(type##Array *arr, i32 indexFirst, i32 indexSecond)\
{\
	type tempType = (arr)->e[indexFirst];\
	type##ID tempID = (arr)->ids[indexFirst];\
	(arr)->e[indexFirst] = (arr)->e[indexSecond];\
	(arr)->ids[indexFirst] = (arr)->ids[indexSecond];\
	(arr)->e[indexSecond] = tempType;\
	(arr)->ids[indexSecond] = tempID;\
}
