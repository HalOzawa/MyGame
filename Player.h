#pragma once
#include "GameObject.h"

class Fbx;

class Player :
	public GameObject
{
    int hModel_;

public:
    //�R���X�g���N�^
    Player(GameObject* parent);

    //�f�X�g���N�^
    ~Player();

    //������
    void Initialize();

    //�X�V
    void Update();

    //�`��
    void Draw();

    //�J��
    void Release();
};

