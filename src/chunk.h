#ifndef lox_c_chunk_h
#define lox_c_chunk_h

#include "common.h"
#include "value.h"

typedef enum {
    OP_CONSTANT,
    OP_CONSTANT_LONG,
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_NEGATE,
    OP_RETURN,
} OpCode;

typedef struct {
    int count;
    int capacity;
    int* lines;
} LineArray;

void initLineArray(LineArray* array);
void freeLineArray(LineArray* array);
void writeLineArray(LineArray* array, int line);

typedef struct {
    int count;
    int capacity;
    uint8_t* code;
    LineArray lines;
    ValueArray constants;
} Chunk;

void initChunk(Chunk* chunk);
void freeChunk(Chunk* chunk);
void writeChunk(Chunk* chunk, uint8_t byte, int line);
void writeConstant(Chunk* chunk, Value value, int line);
int addConstant(Chunk* chunk, Value value);
int getLine(Chunk* chunk, int offset);

#endif
