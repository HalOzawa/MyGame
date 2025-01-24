#pragma once
#include"Fbx.h"
#include"Transform.h"
class Stage
{
    FBX* pFbx[5];

    struct Data
    {
        int height;
        int type;
    };

    Data table[12][12];//後で変更する

    int hModel_;
    int width;
    int height;

public:
    int selectMode;
    int selectType;

    //コンストラクタ
    Stage();

    //デストラクタ
    ~Stage();

    //初期化
    void Initialize();

    //更新
    void Update();

    //描画
    void Draw();

    //開放
    void Release();

    //LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};