# Generic Data Structures In C

Objective:
- ability to use data structures with full functionality on any data type
- type safety is absolutely critical, maximum assistance in detecting / avoiding type errors, preferably at compile time / via the IDE
- easy to understand interface, descriptive but non-verbose if possible
- minimize overhead required by programmer to activate feature set for any particular data type
- single header file

Current data structures:
- Array (fixed size)


Example implementation:

````c
#include sGE_Define.h

// The user only needs to replace "typedef struct _x_ { } _x_;" with the below, where the 3rd parameter is the parentheses used
// to describe the struct contents. As this is a fixed array, the size is predetermined - currently this is preset for all
// Arrays of this type as one constant, with scope to generate different sized options as a future addition

GenerateStructWithGenerics
(
  Entity,
  ENTITY_COUNT_MAX,
  {
    b32 canMove;
    vec3 position;
    ...
  }
)


EntityArray *entities = malloc(sizeof(EntityArray));

temp = (Entity){ 0 };
temp.canMove = true;
temp.position = (vec3) { 0.0f, 1.0f, 0.0f };

Entity *spaceship = EntityArray_PushData(entities, temp);

...
...

for (i32 entityIndex = 0; entityIndex < entities->count; entityIndex++)
{
  Entity *currentEntity = EntityArray_GetElement(entities, entityIndex);
  if (currentEntity->canMove)
  {
    currentEntity->position = ...
  }
}

...
...

free(entities);
```` 
