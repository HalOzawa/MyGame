#include "PlayScene.h"
#include "Fbx.h"
#include "Stage.h"
#include "Player.h"

//Stage* pStage;

PlayScene::PlayScene(GameObject* parent)
{
}

void PlayScene::Initialize()
{
	//pStage->Initialize();
	Instantiate<Player>(this);
	//Instantiate<Enemy>(this);
}

void PlayScene::Update()
{
	//pStage->Update();
}

void PlayScene::Draw()
{
	//pStage->Draw();
}

void PlayScene::Release()
{
	//pStage->Release();
}
