#pragma once
#include "GameObject.h"

class Fbx;

class Player :
	public GameObject
{
    int hModel_;
	XMVECTOR front_;
	float speed_ = 0.1f;
public:
    //コンストラクタ
    Player(GameObject* parent);

    //デストラクタ
    ~Player();

    //初期化
    void Initialize();

    //更新
    void Update();

    //描画
    void Draw();

    //開放
    void Release();
};

