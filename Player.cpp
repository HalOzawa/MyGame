#include "Player.h"
#include "Fbx.h"
#include "Model.h"
#include "Input.h"
#include "Camera.h"

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
	XMMATRIX rotY = XMMatrixIdentity(); //行列の1単位行列
	XMVECTOR move{ 0,0,0,0 };
	XMVECTOR rotVec{ 0,0,0,0 };
	float dir = 0;
	if (Input::IsKeyDown(DIK_W))
	{
		transform_.position_.z += 1.0f;
	}
	if (Input::IsKeyDown(DIK_S))
	{
		transform_.position_.z -= 1.0f;
	}
	if (Input::IsKeyDown(DIK_A))
	{
		transform_.position_.x -= 1.0f;
	}
	if (Input::IsKeyDown(DIK_D))
	{
		transform_.position_.x += 1.0f;
	}

	//回転行列を求める
	rotY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
	rotVec = XMVector3TransformCoord(front_, rotY);
	move = speed_ * rotVec;
	XMVECTOR pos = XMLoadFloat3(&(transform_.position_));
	pos = pos + dir * move;
	XMStoreFloat3(&(transform_.position_), pos);

	//Camera::SetPosition(transform_.position_);
	//XMFLOAT3 camTarget;
	//XMStoreFloat3(&camTarget, pos + move);
	//Camera::SetTarget(camTarget);
}

void Player::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Player::Release()
{
}
