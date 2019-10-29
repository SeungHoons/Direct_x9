#pragma once
#include <vector>

#define g_pSceneManager SceneManager::GetInstance()

class Scene;
class SceneManager
{
private:
	Scene* m_pNowScene;


	SceneManager();
	~SceneManager();
public:
	static SceneManager* GetInstance()
	{
		static SceneManager instance;
		return &instance;
	}

	void Init();
	void Update();
	void Render();
	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void Release();

};

