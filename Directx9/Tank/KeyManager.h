#pragma once
class KeyManager
{
private:
	static const int m_kKeyMax = 256;
	bitset<m_kKeyMax> m_keyUp;
	bitset<m_kKeyMax> m_keyDown;

	KeyManager();
	~KeyManager();
public:
	static KeyManager* Instance()
	{
		static KeyManager Instance;
		return &Instance;
	}

	HRESULT Init();
	void	Update();
	void	Destroy();

	bool IsOnceKeyDown(int key); //키를 한번만 눌렀는지
	bool IsOnceKeyUp(int key);   //키를 한번 눌렀다가 뗐는지
	bool IsStayKeyDown(int key); //키가 계속 눌려져 있는지
	bool IsToggleKey(int key);   //토글키(캡스락, 넘버락)가 On상태인지
};

