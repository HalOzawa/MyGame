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

    Data table[12][12];//��ŕύX����

    int hModel_;
    int width;
    int height;

public:
    int selectMode;
    int selectType;

    //�R���X�g���N�^
    Stage();

    //�f�X�g���N�^
    ~Stage();

    //������
    void Initialize();

    //�X�V
    void Update();

    //�`��
    void Draw();

    //�J��
    void Release();

    //LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};