#include "Player.h"
#include "Fbx.h"
#include "Model.h"
#include "Input.h"

Player::Player(GameObject* parent)
	:GameObject(parent, "Player"), hModel_(-1)
{
}

Player::~Player()
{
}

void Player::Initialize()
{
	hModel_ = Model::Load("Assets/Oden.fbx");
	this->transform_.scale_.x = 0.2;
	this->transform_.scale_.y = 0.2;
	this->transform_.scale_.z = 0.2;
	this->transform_.position_.y = 1;
}

void Player::Update()
{
	//transform_.rotate_.y += 1;
	float speed = 1.0f;
	if (Input::IsKeyDown(DIK_W))
	{
		transform_.position_.z += speed;
	}
	if (Input::IsKeyDown(DIK_S))
	{
		transform_.position_.z -= speed;
	}
	if (Input::IsKeyDown(DIK_A))
	{
		transform_.position_.x -= speed;
	}
	if (Input::IsKeyDown(DIK_D))
	{
		transform_.position_.x += speed;
	}
}

void Player::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Player::Release()
{
}
