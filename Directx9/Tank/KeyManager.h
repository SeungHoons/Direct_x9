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

	bool IsOnceKeyDown(int key); //Ű�� �ѹ��� ��������
	bool IsOnceKeyUp(int key);   //Ű�� �ѹ� �����ٰ� �ô���
	bool IsStayKeyDown(int key); //Ű�� ��� ������ �ִ���
	bool IsToggleKey(int key);   //���Ű(ĸ����, �ѹ���)�� On��������
};

