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
	string fileName[] = { "BoxDefault", "BoxBrick", "BoxSand", "BoxWater", "BoxGrass" };

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
		XMStoreFloat3(& data.start, vMouseFront);
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

}

void Stage::Release()
{
	for (int i = 0; i < 5; i++) 
	{
		pFbx[i]->Release();
		SAFE_DELETE(pFbx[i]);
	}
}

//void Stage::Save()
//{
//	WCHAR fileName[MAX_PATH] = L"����.map";  //�t�@�C����������ϐ�
//
//	//�u�t�@�C����ۑ��v�_�C�A���O�̐ݒ�
//	OPENFILENAME ofn;                         	//���O�����ĕۑ��_�C�A���O�̐ݒ�p�\����
//	ZeroMemory(&ofn, sizeof(ofn));            	//�\���̏�����
//	ofn.lStructSize = sizeof(OPENFILENAME);   	//�\���̂̃T�C�Y
//	ofn.lpstrFilter = TEXT("�}�b�v�f�[�^(*.map)\0*.map\0")        //�����t�@�C���̎��
//					  TEXT("�e�L�X�g�f�[�^(*.txt)\0*.txt\0")
//					  TEXT("���ׂẴt�@�C��(*.*)\0*.*\0\0");     //����
//	ofn.lpstrFile = fileName;               	//�t�@�C����
//	ofn.nMaxFile = MAX_PATH;               		//�p�X�̍ő啶����
//	ofn.Flags = OFN_OVERWRITEPROMPT;   			//�t���O�i�����t�@�C�������݂�����㏑���m�F�j
//	ofn.lpstrDefExt = L"map";                  	//�f�t�H���g�g���q
//
//	//�u�t�@�C����ۑ��v�_�C�A���O
//	BOOL selFile;
//	selFile = GetSaveFileName(&ofn);
//
//	//�L�����Z�������璆�f
//	if (selFile == FALSE) return;
//
//	//�t�@�C���쐬/�J��
//	HANDLE hFile;        //�t�@�C���̃n���h��
//	hFile = CreateFile(
//		fileName,                //�t�@�C����
//		GENERIC_WRITE,           //�A�N�Z�X���[�h�i�������ݗp�j
//		0,                       //���L�i�Ȃ��j
//		NULL,                    //�Z�L�����e�B�����i�p�����Ȃ��j
//		CREATE_ALWAYS,           //�쐬���@
//		FILE_ATTRIBUTE_NORMAL,   //�����ƃt���O�i�ݒ�Ȃ��j
//		NULL);				     //�g�������i�Ȃ��j
//
//	//�t�@�C����������
//	DWORD dwBytes = 0;			 //�������݈ʒu
//
//	string MapData = "";
//
//	for (int x = 0; x < width; x++)
//	{
//		for (int z = 0; z < height; z++)
//		{
//			MapData += std::to_string(table[x][z].height) + ",";
//			MapData += std::to_string(table[x][z].type) + ",";
//		}
//	}
//
//	WriteFile(
//		hFile,                   //�t�@�C���n���h��
//		MapData.c_str(),         //�ۑ�����f�[�^�i������j
//		MapData.size(),	 //�������ޕ�����
//		&dwBytes,                //�������񂾃T�C�Y������ϐ�
//		NULL);                   //�I�[�o�[���b�v�h�\���́i����͎g��Ȃ��j
//
//	//�t�@�C�������
//	CloseHandle(hFile);
//}
//
//void Stage::Open()
//{
//	WCHAR fileName[MAX_PATH] = L"����.map";  //�t�@�C����������ϐ�
//
//	//�u�t�@�C����ۑ��v�_�C�A���O�̐ݒ�
//	OPENFILENAME ofn;                         	//���O�����ĕۑ��_�C�A���O�̐ݒ�p�\����
//	ZeroMemory(&ofn, sizeof(ofn));            	//�\���̏�����
//	ofn.lStructSize = sizeof(OPENFILENAME);   	//�\���̂̃T�C�Y
//	ofn.lpstrFilter = TEXT("�}�b�v�f�[�^(*.map)\0*.map\0")        //�����t�@�C���̎��
//					  TEXT("�e�L�X�g�f�[�^(*.txt)\0*.txt\0")
//					  TEXT("���ׂẴt�@�C��(*.*)\0*.*\0\0");     //����
//	ofn.lpstrFile = fileName;               	//�t�@�C����
//	ofn.nMaxFile = MAX_PATH;               		//�p�X�̍ő啶����
//	ofn.Flags = OFN_FILEMUSTEXIST;   			//�t���O�i�����t�@�C�������݂�����㏑���m�F�j
//	ofn.lpstrDefExt = L"map";                  	//�f�t�H���g�g���q
//
//	//�u�t�@�C����ۑ��v�_�C�A���O
//	BOOL selFile;
//	selFile = GetOpenFileName(&ofn);
//
//	//�L�����Z�������璆�f
//	if (selFile == FALSE) return;
//
//	HANDLE hFile;        //�t�@�C���̃n���h��
//	hFile = CreateFile(
//		fileName,                //�t�@�C����
//		GENERIC_READ,            //�A�N�Z�X���[�h�i�������ݗp�j
//		0,                       //���L�i�Ȃ��j
//		NULL,                    //�Z�L�����e�B�����i�p�����Ȃ��j
//		OPEN_EXISTING,           //�쐬���@
//		FILE_ATTRIBUTE_NORMAL,   //�����ƃt���O�i�ݒ�Ȃ��j
//		NULL);                   //�g�������i�Ȃ��j
//
//	//�t�@�C���̃T�C�Y���擾
//	DWORD fileSize = GetFileSize(hFile, NULL);
//
//	string str;
//	//�t�@�C���̃T�C�Y�����������m��
//	char* data;
//	data = new char[fileSize];
//
//	DWORD dwBytes = 0; //�ǂݍ��݈ʒu
//
//	ReadFile(
//		hFile,     //�t�@�C���n���h��
//		data,      //�f�[�^������ϐ�
//		fileSize,  //�ǂݍ��ރT�C�Y
//		&dwBytes,  //�ǂݍ��񂾃T�C�Y
//		NULL);     //�I�[�o�[���b�v�h�\���́i����͎g��Ȃ��j
//
//	for (int i = 0; i < fileSize; i++)
//	{
//		str = str + data[i];
//	}
//	char mapData[20 * 20];
//	int i = 0;
//	int j = 0;
//	bool roadHeight = true;
//	std::stringstream lsstr = (std::stringstream)str;
//	std::string lstr;
//	std::stringstream sstr;
//
//	while (getline(sstr, str, ','))
//	{
//		if (i >= 20)
//			i = 0;
//		if (roadHeight)
//			table[j][i].height = stoi(str);
//		else
//			table[j - 20][i].type = stoi(str);
//		i++;
//	}
//	j++;
//
//	CloseHandle(hFile);
//}

//�E�B���h�E�v���V�[�W���i�������������ɂ�΂��֐��j
//UINT typedef �ʖ� unsigned �����Ȃ��@int 
LRESULT Stage::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);  //�v���O�����I��
		return 0;
	case WM_MOUSEMOVE:
		Input::SetMousePosition(LOWORD(lParam), HIWORD(lParam));
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_MENU_NEW:
			break;
		case IDC_MENU_OPEN:
			//Open();
			break;
		case IDC_MENU_SAVE:
			//Save();
			break;
		default:
			break;
		}
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
