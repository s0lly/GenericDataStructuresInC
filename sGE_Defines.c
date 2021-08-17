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

static i32 _Array_GetElementIndexFromIDCalc(void *data, i32 count, i32 i)
{
	for (i32 index = 0; index < count; index++)
	{
		if (*((i32 *)data + index) == i)
			return index;
	}
	return -1;
}


static void *_Array_GetElementFromIDCalc(void *data, i32 count, i32 i, u64 elementArrayOffsetBytes, u64 elementSize)
{
	i32 index = _Array_GetElementIndexFromIDCalc(data, count, i);

	if (index < 0)
	{
		return 0;
	}
	else
	{
		return (void *)((u8 *)data + elementArrayOffsetBytes + elementSize * index);
	}
}


static void *_Array_PushDataCalc(void *data, void *dataToCopy, u64 elementArrayOffsetBytes, u64 elementSize, u64 idArrayOffsetBytes, u64 idSize, u64 nextIDOffsetBytes, u64 countOffsetBytes)
{
	i32 count = *(i32 *)((u8 *)data + countOffsetBytes);
	memcpy(((u8 *)data + elementArrayOffsetBytes + elementSize * count), dataToCopy, elementSize);
	*(u32 *)((u8 *)data + idArrayOffsetBytes + idSize * count) = *(i32 *)((u8 *)data + nextIDOffsetBytes);
	(*(u32 *)((u8 *)data + nextIDOffsetBytes))++;
	(*(u32 *)((u8 *)data + countOffsetBytes))++;

	return (void *)((u8 *)data + elementArrayOffsetBytes + elementSize * count);
}


#define GenerateStructWithGenerics(prefix, type, n, struc)\
typedef struct prefix##type##ID\
{\
	i32 val;\
} prefix##type##ID;\
typedef struct type struc type;\
typedef struct prefix##type##Array\
{\
	prefix##type##ID ids[n];\
	prefix##type##ID nextID;\
	type e[n];\
	i32 count;\
} prefix##type##Array;



// TODO: Below functions require bounds checking etc, possibly via helper functions
#define Array_GetCountMax(arr) (sizeof((arr)->e) / sizeof(((arr)->e)[0]))
#define Array_GetElement(arr, n) (&(arr)->e[n])
#define Array_GetElementAtCount(arr) (&(arr)->e[(arr)->count])
#define Array_PushData(arr, data) _Array_PushDataCalc((arr), &data, (u8 *)((arr)->e) - (u8 *)((arr)), sizeof(((arr)->e)[0]), (u8 *)((arr)->ids) - (u8 *)((arr)), sizeof(((arr)->ids)[0]), (u8 *)(&(arr)->nextID) - (u8 *)((arr)), (u8 *)(&(arr)->count) - (u8 *)((arr)))
#define Array_PushZeroed(arr) { memset(&(arr)->e[(arr).count], 0, sizeof((arr)->e[(arr).count])); (arr)->count++; }
#define Array_ReplaceIndex(arr, index, data) { (arr)->e[(index)] = (data); }
#define Array_ResetIndex(arr, index) { memset(&(arr)->e[(index)], 0, sizeof((arr)->e[(index)])); }
#define Array_Pop(arr) { if ((arr).count > 0 ) (arr)->count--; }
#define Array_GetElementFromID(arr, i) _Array_GetElementFromIDCalc((arr), (arr)->count, (i).val, (u8 *)((arr)->e) - (u8 *)((arr)),  sizeof(((arr)->e)[0]))
#define Array_GetID(arr, n) ((arr)->ids[n])
#define Array_GetIDFromElement(arr, elem) ((arr)->ids[((u8 *)(elem) - (u8 *)(arr)->e) / sizeof(((arr)->e)[0])])
#define Array_GetIndexFromElement(arr, elem) (((u8 *)(elem) - (u8 *)(arr)->e) / sizeof(((arr)->e)[0]))
