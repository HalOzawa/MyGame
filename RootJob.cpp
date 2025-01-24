#include "RootJob.h"
#include "SceneManager.h"


RootJob::RootJob(GameObject *parent)
	:GameObject(parent,"RootJob")
{
}

RootJob::~RootJob()
{
}

void RootJob::Initialize()
{
	Instantiate<SceneManager>(this);
	//↑ここをシーンマネージャーに置き換えて、シーン制御を行いたい
}

void RootJob::Update()
{
}

void RootJob::Draw()
{

}

void RootJob::Release()
{
}
