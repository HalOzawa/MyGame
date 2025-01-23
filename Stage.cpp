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

		//ビューポート行列
		XMMATRIX vp =
		{
			w, 0, 0, 0,
			0, -h, 0, 0,
			0, 0, 1, 0,
			w, h, 0, 1
		};

		//レイを飛ばす処理
		XMMATRIX matView = Camera::GetViewMatrix(); //ビュー行列
		XMMATRIX matProj = Camera::GetProjectionMatrix(); //プロジェクション行列

		//ビューポート
		XMMATRIX invVP = XMMatrixInverse(nullptr, vp);
		//プロジェクション変換
		XMMATRIX invProj = XMMatrixInverse(nullptr, matProj);
		//ビュー変換
		XMMATRIX invView = XMMatrixInverse(nullptr, matView);

        XMFLOAT3 mousePosBack = Input::GetMousePosition();
        mousePosBack.z = 0.0f;
        XMFLOAT3 mousePosFront = Input::GetMousePosition();
        mousePosFront.z = 1.0f;
        //mousePosFrontをベクトルに変換
        XMVECTOR vMouseFront = XMLoadFloat3(&mousePosFront); // カメラから見える範囲で一番後ろの面を取得
        //ここで変換する(前) 
        //invVP、invPrj、invViewをかける
        vMouseFront = XMVector3TransformCoord(vMouseFront, invVP * invProj * invView);
        //mousePosBackをベクトルに変換
        XMVECTOR vMouseBack = XMLoadFloat3(&mousePosBack);
        //invVP、invPrj、invViewをかける
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
//	WCHAR fileName[MAX_PATH] = L"無題.map";  //ファイル名を入れる変数
//
//	//「ファイルを保存」ダイアログの設定
//	OPENFILENAME ofn;                         	//名前をつけて保存ダイアログの設定用構造体
//	ZeroMemory(&ofn, sizeof(ofn));            	//構造体初期化
//	ofn.lStructSize = sizeof(OPENFILENAME);   	//構造体のサイズ
//	ofn.lpstrFilter = TEXT("マップデータ(*.map)\0*.map\0")        //─┬ファイルの種類
//					  TEXT("テキストデータ(*.txt)\0*.txt\0")
//					  TEXT("すべてのファイル(*.*)\0*.*\0\0");     //─┘
//	ofn.lpstrFile = fileName;               	//ファイル名
//	ofn.nMaxFile = MAX_PATH;               		//パスの最大文字数
//	ofn.Flags = OFN_OVERWRITEPROMPT;   			//フラグ（同名ファイルが存在したら上書き確認）
//	ofn.lpstrDefExt = L"map";                  	//デフォルト拡張子
//
//	//「ファイルを保存」ダイアログ
//	BOOL selFile;
//	selFile = GetSaveFileName(&ofn);
//
//	//キャンセルしたら中断
//	if (selFile == FALSE) return;
//
//	//ファイル作成/開く
//	HANDLE hFile;        //ファイルのハンドル
//	hFile = CreateFile(
//		fileName,                //ファイル名
//		GENERIC_WRITE,           //アクセスモード（書き込み用）
//		0,                       //共有（なし）
//		NULL,                    //セキュリティ属性（継承しない）
//		CREATE_ALWAYS,           //作成方法
//		FILE_ATTRIBUTE_NORMAL,   //属性とフラグ（設定なし）
//		NULL);				     //拡張属性（なし）
//
//	//ファイル書き込み
//	DWORD dwBytes = 0;			 //書き込み位置
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
//		hFile,                   //ファイルハンドル
//		MapData.c_str(),         //保存するデータ（文字列）
//		MapData.size(),	 //書き込む文字数
//		&dwBytes,                //書き込んだサイズを入れる変数
//		NULL);                   //オーバーラップド構造体（今回は使わない）
//
//	//ファイルを閉じる
//	CloseHandle(hFile);
//}
//
//void Stage::Open()
//{
//	WCHAR fileName[MAX_PATH] = L"無題.map";  //ファイル名を入れる変数
//
//	//「ファイルを保存」ダイアログの設定
//	OPENFILENAME ofn;                         	//名前をつけて保存ダイアログの設定用構造体
//	ZeroMemory(&ofn, sizeof(ofn));            	//構造体初期化
//	ofn.lStructSize = sizeof(OPENFILENAME);   	//構造体のサイズ
//	ofn.lpstrFilter = TEXT("マップデータ(*.map)\0*.map\0")        //─┬ファイルの種類
//					  TEXT("テキストデータ(*.txt)\0*.txt\0")
//					  TEXT("すべてのファイル(*.*)\0*.*\0\0");     //─┘
//	ofn.lpstrFile = fileName;               	//ファイル名
//	ofn.nMaxFile = MAX_PATH;               		//パスの最大文字数
//	ofn.Flags = OFN_FILEMUSTEXIST;   			//フラグ（同名ファイルが存在したら上書き確認）
//	ofn.lpstrDefExt = L"map";                  	//デフォルト拡張子
//
//	//「ファイルを保存」ダイアログ
//	BOOL selFile;
//	selFile = GetOpenFileName(&ofn);
//
//	//キャンセルしたら中断
//	if (selFile == FALSE) return;
//
//	HANDLE hFile;        //ファイルのハンドル
//	hFile = CreateFile(
//		fileName,                //ファイル名
//		GENERIC_READ,            //アクセスモード（書き込み用）
//		0,                       //共有（なし）
//		NULL,                    //セキュリティ属性（継承しない）
//		OPEN_EXISTING,           //作成方法
//		FILE_ATTRIBUTE_NORMAL,   //属性とフラグ（設定なし）
//		NULL);                   //拡張属性（なし）
//
//	//ファイルのサイズを取得
//	DWORD fileSize = GetFileSize(hFile, NULL);
//
//	string str;
//	//ファイルのサイズ分メモリを確保
//	char* data;
//	data = new char[fileSize];
//
//	DWORD dwBytes = 0; //読み込み位置
//
//	ReadFile(
//		hFile,     //ファイルハンドル
//		data,      //データを入れる変数
//		fileSize,  //読み込むサイズ
//		&dwBytes,  //読み込んだサイズ
//		NULL);     //オーバーラップド構造体（今回は使わない）
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

//ウィンドウプロシージャ（何かあった時によばれる関数）
//UINT typedef 別名 unsigned 符号なし　int 
LRESULT Stage::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);  //プログラム終了
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
