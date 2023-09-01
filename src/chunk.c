#include <stdlib.h>

#include "chunk.h"
#include "memory.h"


void initLineArray(LineArray* array) {
    array->count = 0;
    array->capacity = 0;
    array->lines = NULL;
}

void freeLineArray(LineArray* array) {
    FREE_ARRAY(int, array->lines, array->capacity);
}

void writeLineArray(LineArray* array, int line) {
    if (array->capacity < array->count + 2) {
        int oldCapacity = array->capacity;
        array->capacity = GROW_CAPACITY(oldCapacity);
        array->lines = GROW_ARRAY(
            int,
            array->lines,
            oldCapacity,
            array->capacity
        );
    }

    if (array->count > 0 && array->lines[array->count - 2] == line) {
        array->lines[array->count - 1]++;
    } else {
        array->lines[array->count] = line;
        array->count++;

        array->lines[array->count] = 1;
        array->count++;
    }
}

void initChunk(Chunk* chunk) {
    chunk->count = 0;
    chunk->capacity = 0;
    chunk->code = NULL;
    initLineArray(&chunk->lines);
    initValueArray(&chunk->constants);
}

void freeChunk(Chunk* chunk) {
    FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
    freeLineArray(&chunk->lines);
    freeValueArray(&chunk->constants);
    initChunk(chunk);
}

void writeChunk(Chunk* chunk, uint8_t byte, int line) {
    if (chunk->capacity < chunk->count + 1) {
        int oldCapacity = chunk->capacity;
        chunk->capacity = GROW_CAPACITY(oldCapacity);
        chunk->code = GROW_ARRAY(
            uint8_t,
            chunk->code,
            oldCapacity,
            chunk->capacity
        );
    }

    chunk->code[chunk->count] = byte;
    chunk->count++;

    writeLineArray(&chunk->lines, line);
}

int addConstant(Chunk* chunk, Value value) {
    writeValueArray(&chunk->constants, value);
    return chunk->constants.count - 1;
}

void writeConstant(Chunk* chunk, Value value, int line) {
    int constant = addConstant(chunk, value);

    if (constant < 256) {
        writeChunk(chunk, OP_CONSTANT, line);
        writeChunk(chunk, constant, line);
    } else {
        writeChunk(chunk, OP_CONSTANT_LONG, line);
        writeChunk(chunk, constant & 0xff, line);
        writeChunk(chunk, constant >> 8 & 0xff, line);
        writeChunk(chunk, constant >> 16 & 0xff, line);
    }
}

int getLine(Chunk* chunk, int offset) {
    int index = 0;
    int instruction = 0;

    while (index < chunk->lines.count && instruction + chunk->lines.lines[index + 1] < offset) {
        instruction += chunk->lines.lines[index + 1];
        index += 2;
    }

    if (index > chunk->lines.count) {
        return -1;
    }

    return chunk->lines.lines[index];
}
