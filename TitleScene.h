#pragma once
#include "GameObject.h"

//前方宣言 
class Fbx;

class TitleScene :
    public GameObject
{
public:
    //コンストラクタ
    //引数：parent  親オブジェクト（SceneManager）
    TitleScene(GameObject* parent);

    //初期化
    void Initialize() override;

    //更新
    void Update() override;

    //描画
    void Draw() override;

    //開放
    void Release() override;
};

