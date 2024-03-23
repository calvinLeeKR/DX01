#pragma once

struct POS {
    int x;
    int y;

    bool operator == (const POS& other) const {
        return x == other.x && y == other.y;
    }
};

struct TILE 
{
    class UNIT* unit = nullptr;
};

class MAP 
{
public:
    TILE mTiles[100][100];

    void Update() {
        Soldier1* a = (Soldier1*) mTiles[1][1].unit;
        ...
    }

    void SetUnit(class UNIT* unit) {
        mTiles[unit->mTile.y][unit->mTile.x].unit = nullptr;
        
        int x = unit->mPosition.x / 16;
        int y = unit->mPosition.y / 16;

        mTiles[y][x].unit = unit;
        unit->mTile.x = x;
        unit->mTile.y = y;
    }
};


class UNIT
{

public:
    bool isMoving() {
        return !(mPosition == mTarget);
    }

    void Update() {
        if(isMoving()) {
            mDir.x = mTarget.x - mPosition.x;
            mDir.y = mTarget.y - mPosition.y;

            float len = sqrt((mDir.x * mDir.x) + (mDir.y * mDir.y));
            if (len < mSpeed) {
                mPosition.x = mTarget.x;
                mPosition.y = mTarget.y;
            }
            else {
                float fx = (float)mDir.x / len;
                float fy = (float)mDir.y / len;

                fx = fx * mSpeed;
                fy = fy * mSpeed;

                mDir.x = fx;
                mDir.y = fy;

                mPosition.x += mDir.x;
                mPosition.y += mDir.y;
            }

            g_Map->SetUnit(this);
        }

    }
    void Draw();

    void SetPostion(int tile_x, int tile_y) {
        mPosition.x = tile_x * 16;
        mPosition.y = tile_y * 16;
    }
    void MoveTo(int tile_x, int tile_y) {
        mTarget.x = tile_x * 16;
        mTarget.y = tile_y * 16;
    }

    POS mPosition;
    POS mTarget;
    POS mDir;
    float mSpeed;

    POS mTile;
};

