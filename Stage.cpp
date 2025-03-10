#include "Stage.h"
#include"Transform.h"
#include"Input.h"
#include"Camera.h"
#include"resource.h"
#include<locale>
#include<codecvt>
#include <sstream>
#include <iostream>

Stage::Stage() : pFbx(), width(12), height(12)
{
	for (int x = 0; x < width; x++)
	{
		for (int z = 0; z < height; z++)
		{
			table[x][z].height = 1;
			table[x][z].type = 0;
		}
	}
}

Stage::~Stage()
{
}

void Stage::Initialize()
{
	string fileName[] = { "BoxGrass", "BoxBrick", "BoxSand", "BoxWater", "BoxDefault" };

	for (int i = 0; i < 5; i++)
	{
		pFbx[i] = new FBX();
		string path = "Assets\\" + fileName[i] + ".fbx";
		pFbx[i]->Load(path);
	}
}

void Stage::Update()
{
	if (Input::IsMouseButtonDown(0))
	{
		float w = Direct3D::WINDOW_WIDTH / 2;
		float h = Direct3D::WINDOW_HEIGHT / 2;

		//�r���[�|�[�g�s��
		XMMATRIX vp =
		{
			w, 0, 0, 0,
			0, -h, 0, 0,
			0, 0, 1, 0,
			w, h, 0, 1
		};

		//���C���΂�����
		XMMATRIX matView = Camera::GetViewMatrix(); //�r���[�s��
		XMMATRIX matProj = Camera::GetProjectionMatrix(); //�v���W�F�N�V�����s��

		//�r���[�|�[�g
		XMMATRIX invVP = XMMatrixInverse(nullptr, vp);
		//�v���W�F�N�V�����ϊ�
		XMMATRIX invProj = XMMatrixInverse(nullptr, matProj);
		//�r���[�ϊ�
		XMMATRIX invView = XMMatrixInverse(nullptr, matView);

		XMFLOAT3 mousePosBack = Input::GetMousePosition();
		mousePosBack.z = 0.0f;
		XMFLOAT3 mousePosFront = Input::GetMousePosition();
		mousePosFront.z = 1.0f;
		//mousePosFront���x�N�g���ɕϊ�
		XMVECTOR vMouseFront = XMLoadFloat3(&mousePosFront); // �J�������猩����͈͂ň�Ԍ��̖ʂ��擾
		//�����ŕϊ�����(�O) 
		//invVP�AinvPrj�AinvView��������
		vMouseFront = XMVector3TransformCoord(vMouseFront, invVP * invProj * invView);
		//mousePosBack���x�N�g���ɕϊ�
		XMVECTOR vMouseBack = XMLoadFloat3(&mousePosBack);
		//invVP�AinvPrj�AinvView��������
		vMouseBack = XMVector3TransformCoord(vMouseBack, invVP * invProj * invView);

		RayCastData data;
		XMStoreFloat3(&data.start, vMouseFront);
		XMStoreFloat3(&data.dir, vMouseBack - vMouseFront);

		for (int x = 0; x < width; x++)
		{
			for (int z = 0; z < height; z++)
			{
				for (int y = 0; y < table[x][z].height; y++)
				{
					Transform trs;
					trs.position_.x = x;
					trs.position_.y = y;
					trs.position_.z = z;

					int type = table[x][z].type;
					pFbx[type]->RayCast(data, trs);

					if (data.hit == true)
					{
						switch (selectMode)
						{
						case 0:
							table[x][z].height++;
							break;
						case 1:
							if (table[x][z].height > 1)
								table[x][z].height--;
							break;
						case 2:
							table[x][z].type = selectType;
							break;
						}
						return;
					}
				}
			}
		}
	}
}

void Stage::Draw()
{
	for (int x = 0; x < width; x++)
	{
		for (int z = 0; z < height; z++)
		{
			for (int y = 0; y < table[x][z].height; y++)
			{
				Transform trs;
				trs.position_.x = x;
				trs.position_.y = y;
				trs.position_.z = z;

				int type = table[x][z].type;
				pFbx[type]->Draw(trs);
			}
		}
	}
	table[4][11] = { 3, 0 };
	table[5][11] = { 3, 0 };
	table[6][11] = { 3, 0 };
	table[4][10] = { 3, 0 };
	table[5][10] = { 3, 0 };
	table[6][10] = { 3, 0 };
	table[5][8] = { 3, 0 };
	table[1][2] = { 2, 0 };
	table[2][2] = { 2, 0 };
	table[1][3] = { 3, 0 };
	table[2][3] = { 3, 0 };
	table[3][3] = { 3, 0 };
	table[4][3] = { 3, 0 };
	table[5][3] = { 3, 0 };
	table[7][3] = { 3, 0 };
	table[9][3] = { 3, 0 };
	table[10][3] = { 3, 0 };
	table[11][3] = { 3, 0 };
	table[9][4] = { 3, 0 };
	table[10][4] = { 3, 0 };
	table[11][4] = { 3, 0 };

}

void Stage::Release()
{
	for (int i = 0; i < 5; i++)
	{
		pFbx[i]->Release();
		SAFE_DELETE(pFbx[i]);
	}
}