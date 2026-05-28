#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h> 

#define mapWidth 80
#define mapHeight 25

typedef struct SObject {
    float x, y;
    float width, height;
    float vertSpeed;
    bool IsFly;
    char cType;
    float horizonSpeed;
} TObject;

char map[mapHeight][mapWidth + 1];
TObject mario;
int level = 1;
int score;
int maxLvl;

// объявления функций
void ClearMap();
void ShowMap();
void SetObjectPos(TObject *obj, float xPos, float yPos);
void InitObject(TObject *obj, float xPos, float yPos, float oWidth, float oHeight, char inType);
void CreateLevel(int lvl, int &brickLength, TObject* &brick, int &movingLength, TObject* &moving);
void PlayerDead(int brickLength, TObject* &brick, int movingLength, TObject* &moving);
bool IsCollision(TObject o1, TObject o2);
TObject &GetNewMoving(TObject* &moving, int &movingLength);
TObject &GetNewBrick(TObject* &brick, int &brickLength);
void VertMoveObject(TObject *obj, int &brickLength, TObject* &brick, int &movingLength, TObject* &moving);
void DeleteMoving(int i, int &movingLength, TObject* &moving);
void MarioCollision(int &brickLength, TObject* &brick, int &movingLength, TObject* &moving);
void HorizonMoveObject(TObject *obj, int &brickLength, TObject* &brick, int &movingLength, TObject* &moving);
bool IsPosInMap(int x, int y);
void PutObjectOnMap(TObject obj);
void setCur(int x, int y);
void HorizonMoveMap(float dx, int brickLength, TObject* &brick, int movingLength, TObject* &moving);
void PutScoreOnMap();

// реализации
void ClearMap() {
    for (int i = 0; i < mapWidth; i++) map[0][i] = ' ';
    map[0][mapWidth] = '\0';
    for (int j = 1; j < mapHeight; j++) sprintf(map[j], map[0]);
}

void ShowMap() {
    map[mapHeight - 1][mapWidth - 1] = '\0';
    for (int j = 0; j < mapHeight; j++) printf("%s\n", map[j]);
}

void SetObjectPos(TObject *obj, float xPos, float yPos) {
    obj->x = xPos;
    obj->y = yPos;
}

void InitObject(TObject *obj, float xPos, float yPos, float oWidth, float oHeight, char inType) {
    SetObjectPos(obj, xPos, yPos);
    obj->width = oWidth;
    obj->height = oHeight;
    obj->vertSpeed = 0;
    obj->IsFly = false;
    obj->cType = inType;
    obj->horizonSpeed = 0.2f;
}

void PlayerDead(int brickLength, TObject* &brick, int movingLength, TObject* &moving) {
    Sleep(500);
    CreateLevel(level, brickLength, brick, movingLength, moving);
}

bool IsCollision(TObject o1, TObject o2) {
    return (o1.x + o1.width > o2.x) && (o1.x < o2.x + o2.width) &&
           (o1.y + o1.height > o2.y) && (o1.y < o2.y + o2.height);
}

TObject &GetNewBrick(TObject* &brick, int &brickLength) {
    brickLength++;
    brick = (TObject*)realloc(brick, sizeof(TObject) * brickLength);
    return brick[brickLength - 1];
}

TObject &GetNewMoving(TObject* &moving, int &movingLength) {
    movingLength++;
    moving = (TObject*)realloc(moving, sizeof(TObject) * movingLength);
    return moving[movingLength - 1];
}

void DeleteMoving(int i, int &movingLength, TObject* &moving) {
    movingLength--;
    moving[i] = moving[movingLength];
    moving = (TObject*)realloc(moving, sizeof(TObject) * movingLength);
}

void VertMoveObject(TObject *obj, int &brickLength, TObject* &brick, int &movingLength, TObject* &moving) {
    obj->IsFly = true;
    obj->vertSpeed += 0.05f;
    SetObjectPos(obj, obj->x, obj->y + obj->vertSpeed);
    for (int i = 0; i < brickLength; i++) {
        if (IsCollision(*obj, brick[i])) {
            if (obj->vertSpeed > 0) obj->IsFly = false;
            if ((brick[i].cType == '?') && (obj->vertSpeed < 0) && (obj == &mario)) {
                brick[i].cType = '-';
                TObject &newMoving = GetNewMoving(moving, movingLength);
                InitObject(&newMoving, brick[i].x, brick[i].y - 3, 3, 2, '$');
                newMoving.vertSpeed = -0.7f;
            }
            obj->y -= obj->vertSpeed;
            obj->vertSpeed = 0;
            if (brick[i].cType == '+') {
                level++;
                if (level > maxLvl) level = 1;
                Sleep(500);
                CreateLevel(level, brickLength, brick, movingLength, moving);
            }
            break;
        }
    }
}

void MarioCollision(int &brickLength, TObject* &brick, int &movingLength, TObject* &moving) {
    for (int i = 0; i < movingLength; i++) {
        if (IsCollision(mario, moving[i])) {
            if (moving[i].cType == 'o') {
                if (mario.IsFly && (mario.vertSpeed > 0) && (mario.y + mario.height < moving[i].y + moving[i].height) * 0.5f) {
                    score += 50;
                    DeleteMoving(i, movingLength, moving);
                    i--;
                    continue;
                } else {
                    PlayerDead(brickLength, brick, movingLength, moving);
                }
            }
            if (moving[i].cType == '$') {
                score += 100;
                DeleteMoving(i, movingLength, moving);
                i--;
                continue;
            }
        }
    }
}

void HorizonMoveObject(TObject *obj, int &brickLength, TObject* &brick, int &movingLength, TObject* &moving) {
    obj->x += obj->horizonSpeed;
    for (int i = 0; i < brickLength; i++) {
        if (IsCollision(*obj, brick[i])) {
            obj->x -= obj->horizonSpeed;
            obj->horizonSpeed = -obj->horizonSpeed;
            return;
        }
    }
    if (obj->cType == 'o') {
        TObject tmp = *obj;
        VertMoveObject(&tmp, brickLength, brick, movingLength, moving);
        if (tmp.IsFly) {
            obj->x -= obj->horizonSpeed;
            obj->horizonSpeed = -obj->horizonSpeed;
        }
    }
}

bool IsPosInMap(int x, int y) {
    return (x >= 0 && x < mapWidth && y >= 0 && y < mapHeight);
}

void PutObjectOnMap(TObject obj) {
    int ix = (int)round(obj.x);
    int iy = (int)round(obj.y);
    int iWidth = (int)round(obj.width);
    int iHeight = (int)round(obj.height);
    for (int i = ix; i < ix + iWidth; i++)
        for (int j = iy; j < iy + iHeight; j++)
            if (IsPosInMap(i, j)) map[j][i] = obj.cType;
}

void setCur(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void HorizonMoveMap(float dx, int brickLength, TObject* &brick, int movingLength, TObject* &moving) {
    mario.x -= dx;
    for (int i = 0; i < brickLength; i++) {
        if (IsCollision(mario, brick[i])) {
            mario.x += dx;
            return;
        }
    }
    mario.x += dx;
    for (int i = 0; i < brickLength; i++) brick[i].x += dx;
    for (int i = 0; i < movingLength; i++) moving[i].x += dx;
}

void PutScoreOnMap() {
    char c[30];
    sprintf(c, "Score: %d", score);
    int len = strlen(c);
    for (int i = 0; i < len; i++) map[1][i + 5] = c[i];
}

void CreateLevel(int lvl, int &brickLength, TObject* &brick, int &movingLength, TObject* &moving) {
    brickLength = 0;
    brick = (TObject*)realloc(brick, 0);
    movingLength = 0;
    moving = (TObject*)realloc(moving, 0);
    InitObject(&mario, 39, 10, 3, 3, '@');
    score = 0;
    if (lvl == 1) {
        InitObject(&GetNewBrick(brick, brickLength), 20, 20, 40, 5, '#');
        InitObject(&GetNewBrick(brick, brickLength), 30, 10, 5, 3, '?');
        InitObject(&GetNewBrick(brick, brickLength), 50, 10, 5, 3, '?');
        InitObject(&GetNewBrick(brick, brickLength), 60, 15, 40, 10, '#');
        InitObject(&GetNewBrick(brick, brickLength), 60, 5, 10, 3, '-');
        InitObject(&GetNewBrick(brick, brickLength), 70, 5, 5, 3, '?');
        InitObject(&GetNewBrick(brick, brickLength), 75, 5, 5, 3, '-');
        InitObject(&GetNewBrick(brick, brickLength), 80, 5, 5, 3, '?');
        InitObject(&GetNewBrick(brick, brickLength), 85, 5, 10, 3, '-');
        InitObject(&GetNewBrick(brick, brickLength), 100, 20, 20, 5, '#');
        InitObject(&GetNewBrick(brick, brickLength), 120, 15, 10, 10, '#');
        InitObject(&GetNewBrick(brick, brickLength), 150, 20, 40, 5, '#');
        InitObject(&GetNewBrick(brick, brickLength), 210, 15, 10, 10, '+');
        InitObject(&GetNewMoving(moving, movingLength), 25, 10, 3, 2, 'o');
        InitObject(&GetNewMoving(moving, movingLength), 80, 10, 3, 2, 'o');
    }
    if (lvl == 2) {
        InitObject(&GetNewBrick(brick, brickLength), 20, 20, 40, 5, '#');
        InitObject(&GetNewBrick(brick, brickLength), 60, 15, 10, 10, '#');
        InitObject(&GetNewBrick(brick, brickLength), 80, 20, 20, 5, '#');
        InitObject(&GetNewBrick(brick, brickLength), 120, 15, 10, 10, '#');
        InitObject(&GetNewBrick(brick, brickLength), 150, 20, 40, 5, '#');
        InitObject(&GetNewBrick(brick, brickLength), 210, 15, 10, 10, '+');
        InitObject(&GetNewMoving(moving, movingLength), 25, 10, 3, 2, 'o');
        InitObject(&GetNewMoving(moving, movingLength), 80, 10, 3, 2, 'o');
        InitObject(&GetNewMoving(moving, movingLength), 65, 10, 3, 2, 'o');
        InitObject(&GetNewMoving(moving, movingLength), 120, 10, 3, 2, 'o');
        InitObject(&GetNewMoving(moving, movingLength), 160, 10, 3, 2, 'o');
        InitObject(&GetNewMoving(moving, movingLength), 175, 10, 3, 2, 'o');
    }
    if (lvl == 3) {
        InitObject(&GetNewBrick(brick, brickLength), 20, 20, 40, 5, '#');
        InitObject(&GetNewBrick(brick, brickLength), 80, 20, 15, 5, '#');
        InitObject(&GetNewBrick(brick, brickLength), 120, 15, 15, 10, '#');
        InitObject(&GetNewBrick(brick, brickLength), 160, 10, 15, 15, '+');
        InitObject(&GetNewMoving(moving, movingLength), 25, 10, 3, 2, 'o');
        InitObject(&GetNewMoving(moving, movingLength), 50, 10, 3, 2, 'o');
        InitObject(&GetNewMoving(moving, movingLength), 80, 10, 3, 2, 'o');
        InitObject(&GetNewMoving(moving, movingLength), 90, 10, 3, 2, 'o');
        InitObject(&GetNewMoving(moving, movingLength), 120, 10, 3, 2, 'o');
        InitObject(&GetNewMoving(moving, movingLength), 130, 10, 3, 2, 'o');
    }
    maxLvl = 3;
}

int main() {
    TObject *brick = NULL;
    int brickLength = 0;
    TObject *moving = NULL;
    int movingLength = 0;
    CreateLevel(level, brickLength, brick, movingLength, moving);
    do {
        ClearMap();
        if (!mario.IsFly && (GetKeyState(VK_SPACE) < 0)) mario.vertSpeed = -1.0f;
        if (GetKeyState('A') < 0) HorizonMoveMap(1.0f, brickLength, brick, movingLength, moving);
        if (GetKeyState('D') < 0) HorizonMoveMap(-1.0f, brickLength, brick, movingLength, moving);
        if (mario.y > mapHeight) PlayerDead(brickLength, brick, movingLength, moving);
        VertMoveObject(&mario, brickLength, brick, movingLength, moving);
        MarioCollision(brickLength, brick, movingLength, moving);
        for (int i = 0; i < brickLength; i++) PutObjectOnMap(brick[i]);
        for (int i = 0; i < movingLength; i++) {
            VertMoveObject(&moving[i], brickLength, brick, movingLength, moving);
            HorizonMoveObject(&moving[i], brickLength, brick, movingLength, moving);
            if (moving[i].y > mapHeight) {
                DeleteMoving(i, movingLength, moving);
                i--;
                continue;
            }
            PutObjectOnMap(moving[i]);
        }
        PutObjectOnMap(mario);
        PutScoreOnMap();
        setCur(0, 0);
        ShowMap();
        Sleep(10);
    } while (GetKeyState(VK_ESCAPE) >= 0);
    free(brick);
    free(moving);
    return 0;
}