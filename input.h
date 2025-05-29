#ifndef INPUT_H
#define INPUT_H

typedef struct {
    int w;
    int a;
    int s;
    int d;
    int q;
    int e;
} InputState;

void update_input(InputState* input);

#endif

