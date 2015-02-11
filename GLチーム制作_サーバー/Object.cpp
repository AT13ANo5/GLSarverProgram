#include "Object.h"
CObject* CObject::Top[] = {nullptr};
CObject* CObject::Cur[] = {nullptr};

CObject::CObject(int priority)
{
	Priority = priority;
	DeathFlag = false;
	LinkList();
}
CObject::~CObject()
{
	UnlinkList();
}
//=============================================================================
//自身をリストに追加
//=============================================================================
void CObject::LinkList(void)
{
	if(Top[Priority] != nullptr)//二つ目以降の処理
	{
		CObject* pScene = Cur[Priority];
		pScene->Next = this;
		Prev = pScene;
		Next = nullptr;
		Cur[Priority] = this;
	}
	else//最初の一つの時の処理
	{
		Top[Priority] = this;
		Cur[Priority] = this;
		Prev = nullptr;
		Next = nullptr;
	}
}

//=============================================================================
//自身をリストから削除
//=============================================================================
void CObject::UnlinkList(void)
{
	if(Prev == nullptr)//先頭
	{
		if(Next != nullptr)//次がある
		{
			Next->Prev = nullptr;
			Top[Priority] = Next;
		}
		else//最後の一つだった
		{
			Top[Priority] = nullptr;
			Cur[Priority] = nullptr;
		}
	}
	else if(Next == nullptr)//終端
	{
		if(Prev != nullptr)//前がある
		{
			Prev->Next = nullptr;
			Cur[Priority] = Prev;
		}
		else//最後の一つだった
		{
			Top[Priority] = nullptr;
			Cur[Priority] = nullptr;
		}
	}
	else//前後にデータがあるとき
	{
		Prev->Next = Next;
		Next->Prev = Prev;
	}

	Prev = nullptr;
	Next = nullptr;
}
void CObject::Clear(void)
{
	for(int cnt=0;cnt<LAYER_NUM;cnt++)
	{
		Top[cnt] = nullptr;
		Cur[cnt] = nullptr;
	}
}

void CObject::Release(void)
{
	DeathFlag = true;
	
}

void CObject::UpdateAll(void)
{
	for(int cnt=0;cnt<LAYER_NUM;cnt++)
	{
		CObject* Scene = Top[cnt];
		CObject* Next = nullptr;
		while(Scene)
		{
			Next = Scene->Next;
			Scene->Update();
			if (Scene->DeathFlag)
			{
				Scene->Uninit();
			}
			Scene = Next;
		}
	}
}

void CObject::DrawAll(void)
{
	for(int cnt=0;cnt<LAYER_NUM;cnt++)
	{
		CObject* Scene = Top[cnt];
		CObject* Next = nullptr;
		while(Scene)
		{
			Next = Scene->Next;
			Scene->Draw();
			Scene = Next;
		}
	}
}

void CObject::ReleaseAll(void)
{
	for(int cnt=0;cnt<LAYER_NUM;cnt++)
	{
		CObject* Scene = Top[cnt];
		CObject* Next = nullptr;
		while(Scene)
		{
			Next = Scene->Next;
			Scene->Release();
			if(Scene->DeathFlag)
			{
				Scene->Uninit();
			}
			Scene = Next;
		}
	}
}