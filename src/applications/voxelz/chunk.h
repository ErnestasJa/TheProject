#pragma once

#define CX 16
#define CY 16
#define CZ 16

struct chunk {
  uint8_t blk[CX][CY][CZ];
  GLuint vbo;
  int elements;
  bool changed;

  chunk() {
    memset(blk, 0, sizeof blk);
    vertices = 0;
    changed = true;
    glGenBuffers(1, &vbo);
  }

  ~chunk() {
    glDeleteBuffers(1, &vbo);
  }

  uint8_t get(int x, int y, int z) {
    return blk[x][y][z];
  }

  void set(int x, int y, int z, uint8_t type) {
    blk[x][y][z] = type;
    changed = true;
  }

  void update() {
    changed = false;
    // Fill in the VBO here
  }

  void render() {
    if(changed)
      update();
    // Render the VBO here
  }
};
