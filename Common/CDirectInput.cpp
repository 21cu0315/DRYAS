/**
 * @file CDirectInput.cpp
 * @brief DirectInputキーボードのクラス
 * @author 織戸　喜隆
 * @date 日付（2017.02.14）
 */

#include "stdafx.h"
#include "CDirectInput.h"

std::shared_ptr<CDirectInput> CDirectInput::m_instance = NULL;	 //!< 【クラス変数】

//ゲームパッドデバイスの作成-デバイス列挙の結果を受け取る構造体
typedef struct DeviceEnumParameter
{
	HWND							hWnd;
	LPDIRECTINPUT8					directInput;
	std::shared_ptr<CDirectInput>	instance;
	LPDIRECTINPUTDEVICE8* padDevice;
	int								findCount;
}DeviceEnumParameter;

/**
 * Private Methods
 */
CDirectInput::CDirectInput()
{
}

CDirectInput::~CDirectInput()
{
}

/**
 * @fn HRESULT CDirectInput::init(HWND inHWnd)
 * @brief 初期化
 * @param hWnd_		WindowsAPIウインドウハンドル
 * @return			WindowsAPI 実行結果
 */
HRESULT CDirectInput::init(HWND inHWnd)
{
	m_pDinput = NULL;		 //!< DirectInput オブジェクト
	m_pKeyDevice = NULL;	 //!< キーボード
	m_pMouseDevice = NULL;	 //!< マウス

	m_hWnd = inHWnd;

	ZeroMemory(&m_keyboardInput, sizeof(BYTE) * BUFFER_SIZE);		 //!< キーボードの入力情報
	ZeroMemory(&m_keyboardInputPrev, sizeof(BYTE) * BUFFER_SIZE);	 //!< 前回のキーボードの入力情報

	ZeroMemory(&m_pressed, sizeof(BYTE) * BUFFER_SIZE);		 //!< 押された
	ZeroMemory(&m_pressedOnce, sizeof(BYTE) * BUFFER_SIZE);	 //!< 押され続けた

	ZeroMemory(&m_diMouseState2, sizeof(DIMOUSESTATE2));	 //!< マウスの入力情報
	ZeroMemory(&m_mouseInputPrev, sizeof(DIMOUSESTATE2));	 //!< マウスの入力情報
	
	/**
	 * DirectInputオブジェクトの生成
	 */
	if (FAILED(DirectInput8Create(GetModuleHandle(NULL),
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(VOID**)&m_pDinput,
		NULL))) {
		return E_FAIL;
	}
	/**
	 *キーボードの初期化
	 *キーボードのデバイスオブジェクトの生成
	 */
	initKeyboard(inHWnd);
	
	// マウスのデバイスオブジェクトを作成
	initMouse(inHWnd);

	// パッドのデバイスオブジェクトを作成
	initPad(inHWnd);

	return S_OK;
}

/**
* @fn HRESULT CDirectInput::initKeyboard(HWND inHWnd)
* @brief キーボードの初期化
* @param hWnd_		WindowsAPIウインドウハンドル
* @return			WindowsAPI 実行結果
*/
HRESULT CDirectInput::initKeyboard(HWND inHWnd)
{
	if (FAILED(m_pDinput->CreateDevice(GUID_SysKeyboard, &m_pKeyDevice, NULL))) {
		return E_FAIL;
	}
	/**
	 *デバイスをキーボードに設定
	 */
	if (FAILED(m_pKeyDevice->SetDataFormat(&c_dfDIKeyboard))) {
		return E_FAIL;
	}
	/**
	 *協調レベルの設定
	 */
	if (FAILED(m_pKeyDevice->SetCooperativeLevel(inHWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND))) {
		return E_FAIL;
	}
	/**
	 *入力制御開始
	 */
	m_pKeyDevice->Acquire();
	return S_OK;
}

/**
* @fn HRESULT CDirectInput::initMouse(HWND inHWnd)
* @brief マウスの初期化
* @param hWnd_		WindowsAPIウインドウハンドル
* @return			WindowsAPI 実行結果
*/
HRESULT CDirectInput::initMouse(HWND inHWnd)
{
	if (FAILED(m_pDinput->CreateDevice(GUID_SysMouse, &m_pMouseDevice, NULL))) {
		return false;
	}
	// データフォーマットを設定
	// マウス用のデータ・フォーマットを設定
	if (FAILED(m_pMouseDevice->SetDataFormat(&c_dfDIMouse2))) {
		return E_FAIL;
	}
	// モードを設定（フォアグラウンド＆非排他モード）
	if (FAILED(m_pMouseDevice->SetCooperativeLevel(inHWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND))) {
		return E_FAIL;
	}
	//// デバイスの設定
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL;	// 相対値モードで設定（絶対値はDIPROPAXISMODE_ABS）

	if (FAILED(m_pMouseDevice->SetProperty(DIPROP_AXISMODE, &diprop.diph))) {
		return E_FAIL;
	}
	// 入力制御開始
	m_pMouseDevice->Acquire();
	return S_OK;
}

VOID CDirectInput::initPad(HWND inHWnd)
{
	DeviceEnumParameter parameter;

	parameter.hWnd = inHWnd;
	parameter.directInput = m_pDinput;
	parameter.instance = m_instance;
	parameter.padDevice = &m_pPadDevice;
	parameter.findCount = 0;


	// GAMEPADを調べる
	if (FAILED(m_pDinput->EnumDevices(
		DI8DEVTYPE_GAMEPAD,
		DeviceFindCallBack,
		&parameter,
		DIEDFL_ATTACHEDONLY)))
	{
		throw L"デバイスの列挙に失敗。";
	}

	// JOYSTICKを調べる
	m_pDinput->EnumDevices(
		DI8DEVTYPE_JOYSTICK,
		DeviceFindCallBack,
		&parameter,
		DIEDFL_ATTACHEDONLY
	);

	// どちらも見つけることが出来なかったら失敗
	if (parameter.findCount == 0)
	{
		return;
	}

	int count = 0;
	// 制御開始
	while (startPadControl() == FALSE)
	{
		Sleep(100);
		count++;
		if (count >= 5)
		{
			break;
		}
	}
}

/**
 * @fn BOOL CDirectInput::startPadControl()
 * @brief ゲームパッドの制御を開始
 * @param			無し
 * @retval TRUE：	成功
 * @retval FALSE：	失敗
 */
BOOL CDirectInput::startPadControl()
{
	// デバイスが生成されてない
	if (m_pPadDevice == nullptr)
	{
		return FALSE;
	}

	// 制御開始
	if (FAILED(m_pPadDevice->Acquire()))
	{
		return FALSE;
	}

	DIDEVCAPS cap;
	m_pPadDevice->GetCapabilities(&cap);
	// ポーリング判定
	if (cap.dwFlags & DIDC_POLLEDDATAFORMAT)
	{
		DWORD error = GetLastError();
		// ポーリング開始
		/*
			PollはAcquireの前に行うとされていたが、
			Acquireの前で実行すると失敗したので
			後で実行するようにした
		*/
		if (FAILED(m_pPadDevice->Poll()))
		{
			return FALSE;
		}
	}

	return TRUE;
}

/**
* @fn BOOL CDirectInput::DeviceFindCallBack(LPCDIDEVICEINSTANCE lpddi_, LPVOID pvRef_)
* @brief  デバイス発見時に実行される　コールバック
* @param lpddi_	デバイス情報を持つ構造体のポインタ
* @param pvRef_	EnumDevicesで渡した値(未使用)
* @retval TRUE：		列挙を継続する
* @retval FALSE：	列挙を継続しない
*/
BOOL CALLBACK CDirectInput::DeviceFindCallBack(LPCDIDEVICEINSTANCE lpddi_, LPVOID pvRef_)
{
	DeviceEnumParameter* parameter = (DeviceEnumParameter*)pvRef_;

	// 既に発見しているなら終了
	if (parameter->findCount >= 1)
	{
		return DIENUM_STOP;
	}

	// デバイス生成
	HRESULT hr = parameter->directInput->CreateDevice(
		lpddi_->guidInstance,
		parameter->padDevice,
		NULL);

	if (FAILED(hr))
	{
		return DIENUM_STOP;
	}

	// 入力フォーマットの指定
	LPDIRECTINPUTDEVICE8 device = *parameter->padDevice;

	if (FAILED(device->SetDataFormat(&c_dfDIJoystick)))
	{
		return DIENUM_STOP;
	}

	// プロパティの設定
	if (parameter->instance->setUpPadProperty(device) == false)
	{
		return DIENUM_STOP;
	}

	// 協調レベルの設定
	if (parameter->instance->setUpCooperativeLevel(parameter->hWnd, device) == false)
	{
		return DIENUM_STOP;
	}

	// 発見数をカウント
	parameter->findCount++;
	return DIENUM_CONTINUE;
}
/**
 * @fn BOOL CDirectInput::setUpPadProperty(const LPDIRECTINPUTDEVICE8 device_)
 * @brief ゲームパッドのプロパティ設定
 * @param device_	ゲームパッドのデバイス
 * @retval TRUE：	列挙を継続する
 * @retval FALSE：	列挙を継続しない
 */
BOOL CDirectInput::setUpPadProperty(const LPDIRECTINPUTDEVICE8 device_)
{
	// 軸モードを絶対値モードとして設定
	DIPROPDWORD diprop;
	ZeroMemory(&diprop, sizeof(diprop));
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.diph.dwObj = 0;
	diprop.dwData = DIPROPAXISMODE_ABS;	// 絶対値モードの指定(DIPROPAXISMODE_RELにしたら相対値)

	if (FAILED(m_pPadDevice->SetProperty(DIPROP_AXISMODE, &diprop.diph))) {
		return FALSE;
		//throw L"ゲームパッドのプロパティ設定に失敗。";
	}
	// X軸の値の範囲設定
	DIPROPRANGE diprg;
	ZeroMemory(&diprg, sizeof(diprg));
	diprg.diph.dwSize = sizeof(diprg);
	diprg.diph.dwHeaderSize = sizeof(diprg.diph);
	diprg.diph.dwHow = DIPH_BYOFFSET;
	diprg.diph.dwObj = DIJOFS_X;
	diprg.lMin = -1000;
	diprg.lMax = 1000;

	if (FAILED(m_pPadDevice->SetProperty(DIPROP_RANGE, &diprg.diph)))
	{
		return FALSE;
		//throw L"ゲームパッドのプロパティ設定に失敗。";
	}

	// Y軸の値の範囲設定
	diprg.diph.dwObj = DIJOFS_Y;
	if (FAILED(m_pPadDevice->SetProperty(DIPROP_RANGE, &diprg.diph)))
	{
		return FALSE;
	}

	return TRUE;
}
/**
 * @fn BOOL CDirectInput::setUpCooperativeLevel(const HWND hWnd_, const LPDIRECTINPUTDEVICE8 device_)
 * @brief ゲームパッドの協調レベルの設定
 * @param hWnd_		WindowsAPIウインドウハンドル
 * @param device_	ゲームパッドのデバイス
 * @retval TRUE：	成功
 * @retval FALSE：	失敗
 */
BOOL CDirectInput::setUpCooperativeLevel(const HWND hWnd_, const LPDIRECTINPUTDEVICE8 device_)
{
	if (FAILED(device_->SetCooperativeLevel(
		hWnd_,
		DISCL_NONEXCLUSIVE | DISCL_FOREGROUND))) {
		return FALSE;
	}
	return TRUE;
}

/**
 * @fn VOID CDirectInput::free()
 * @brief DirectInputのオブジェクトの開放
 * @param	無し
 * @return	無し
 */
VOID CDirectInput::free()
{
	if (m_pMouseDevice)
		m_pMouseDevice->Unacquire();
	SAFE_RELEASE(m_pMouseDevice);	 //!< マウス

	if (m_pKeyDevice)
		m_pKeyDevice->Unacquire();
	SAFE_RELEASE(m_pKeyDevice);		//!< キーボード

	SAFE_RELEASE(m_pDinput);		//!< DirectInput オブジェクト
}

//__________________________________________________________________
// Public Methods
//__________________________________________________________________

/**
 * @fn std::shared_ptr<CDirectInput> CDirectInput::getInstance(HWND inHWnd)
 * @brief インスタンスを取得 【クラスメソッド】<Singleton-pattern>
 * @param inHWnd	方向の配列
 * @return		インスタンス
 */
std::shared_ptr<CDirectInput> CDirectInput::getInstance(HWND inHWnd)
{
	if (m_instance == NULL) {
		m_instance = shared_ptr<CDirectInput>(new CDirectInput());
		m_instance->init(inHWnd);
	}
	return m_instance;
}

/**
 * @fn VOID CDirectInput::releaseInstance()
 * @brief インスタンスを解放 【クラスメソッド】<Singleton-pattern>
 * @param	無し
 * @return	無し
 */
VOID CDirectInput::releaseInstance()
{
	m_instance->free();
}

/**
 * @fn HRESULT CDirectInput::getState()
 * @brief キーボード・マウスのステータスの取得
 * @param	無し
 * @return   WindowsAPI 実行結果
 */
//
HRESULT CDirectInput::getState()
{
	/**
	 *キーボードの更新
	 */
	HRESULT hr = m_pKeyDevice->Acquire();

	if ((hr == DI_OK) || (hr == S_FALSE)) {
		memcpy(m_keyboardInputPrev, m_keyboardInput, sizeof(BYTE) * BUFFER_SIZE);	 //!<前回の状態を退避

		m_pKeyDevice->GetDeviceState(sizeof(m_keyboardInput), &m_keyboardInput);	 //!<キーボードの状態を取得
		for (int i = 0; i < BUFFER_SIZE; i++) {
			if (IS_KEY_PRESSED(m_keyboardInput[i])) {
				if ((!m_pressedOnce[i]) && (!m_pressed[i])) {
					m_pressedOnce[i] = TRUE;		 //!< 初めて押された
					m_pressed[i] = TRUE;
				}
				else {
					m_pressedOnce[i] = FALSE;
				}
			}
			else {
				m_pressedOnce[i] = FALSE;
				m_pressed[i] = FALSE;
			}
		}
	}

	/**
	*マウスの状態を取得します
	*/
	memcpy(&m_mouseInputPrev, &m_diMouseState2, sizeof(DIMOUSESTATE2));	 //!<前回の状態を退避
	hr = m_pMouseDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &m_diMouseState2);
	if (hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED) {
		hr = m_pMouseDevice->Acquire();
		while (hr == DIERR_INPUTLOST)
			hr = m_pMouseDevice->Acquire();
	}

	return S_OK;
}

/**
 * @fn BOOL CDirectInput::isKeyPressed(BYTE aKey)
 * @brief キーボードの押下
 * @param aKey	調べるキー
 * @return		押されたか
 */
BOOL CDirectInput::isKeyPressed(BYTE aKey)
{
	/**
	 *現在押している
	 */
	if (IS_KEY_PRESSED(m_keyboardInput[aKey]))
	{
		return TRUE;
	}
	return FALSE;
}

/**
 * @fn BOOL CDirectInput::isPressedOnce(BYTE aKey)
 * @brief 指定されたキーの押下が一度だけか
 * @param aKey	調べるキー
 * @return		押されたか
 */
BOOL CDirectInput::isPressedOnce(BYTE aKey)
{
	return m_pressedOnce[aKey];
}

/**
* @fn BOOL CDirectInput::isKeyTrigger(BYTE aKey)
* @brief  指定されたキーのトリガー
* @param aKey	調べるキー
* @return		前回押していて現在押している
*/
BOOL CDirectInput::isKeyTrigger(BYTE aKey)
{
	/**
	 *前回押していて現在押している
	 */
	if ((IS_KEY_PRESSED(m_keyboardInputPrev[aKey])) && (IS_KEY_PRESSED(m_keyboardInput[aKey])))
	{
		return TRUE;
	}
	return FALSE;
}

/**
 * @fn BOOL CDirectInput::isKeyReleased(BYTE aKey)
 * @brief  指定されたキーのチェック
 * @param aKey	調べるキー
 * @return		前回押していて現在離している
 */
BOOL CDirectInput::isKeyReleased(BYTE aKey)
{
	/**
	 *前回押していて現在離している
	 */
	if ((IS_KEY_PRESSED(m_keyboardInputPrev[aKey])) && !(IS_KEY_PRESSED(m_keyboardInput[aKey])))
	{
		return TRUE;
	}
	return FALSE;
}

/**
* @fn BOOL CDirectInput::isLeftButtonClicked()
* @brief  マウスの左ボタンのチェック
* @return		押している
*/
BOOL CDirectInput::isLeftButtonClicked()
{
	if ((IS_KEY_PRESSED(m_diMouseState2.rgbButtons[0])))
	{
		return TRUE;
	}
	return FALSE;
}
/**
* @fn BOOL CDirectInput::isRightButtonClicked()
* @brief  マウスの右ボタンのチェック
* @return		押している
*/
BOOL CDirectInput::isRightButtonClicked()
{
	if ((IS_KEY_PRESSED(m_diMouseState2.rgbButtons[1])))
	{
		return TRUE;
	}
	return FALSE;
}

/**
* @fn BOOL CDirectInput::isCenterButtonClicked()
* @brief  マウスの中ボタンのチェック
* @return		押している
*/
BOOL CDirectInput::isCenterButtonClicked()
{
	if ((IS_KEY_PRESSED(m_diMouseState2.rgbButtons[2])))
	{
		return TRUE;
	}
	return FALSE;
}

/**
* @fn BOOL CDirectInput::isLeftButtonReleased()
* @brief  マウスの左ボタンのチェック
* @return		前回押していて現在離している
*/
BOOL CDirectInput::isLeftButtonReleased()
{
	/**
	 *前回押していて現在離している
	 */
	if ((IS_KEY_PRESSED(m_mouseInputPrev.rgbButtons[0])) && !(IS_KEY_PRESSED(m_diMouseState2.rgbButtons[0])))
	{
		return TRUE;
	}
	return FALSE;
}

/**
* @fn BOOL CDirectInput::isRightButtonReleased()
* @brief  マウスの右ボタンのチェック
* @return		前回押していて現在離している
*/
BOOL CDirectInput::isRightButtonReleased()
{
	/**
	 *前回押していて現在離している
	 */
	if ((IS_KEY_PRESSED(m_mouseInputPrev.rgbButtons[1])) && !(IS_KEY_PRESSED(m_diMouseState2.rgbButtons[1])))
	{
		return TRUE;
	}
	return FALSE;
}

/**
* @fn BOOL CDirectInput::isCenterButtonReleased()
* @brief  マウスの中ボタンのチェック
* @return		前回押していて現在離している
*/
BOOL CDirectInput::isCenterButtonReleased()
{
	/**
	 *前回押していて現在離している
	 */
	if ((IS_KEY_PRESSED(m_mouseInputPrev.rgbButtons[2])) && !(IS_KEY_PRESSED(m_diMouseState2.rgbButtons[2])))
	{
		return TRUE;
	}
	return FALSE;
}

/**
* @fn XMFLOAT2 CDirectInput::getMousePos()
* @brief  マウスの座標取得
* @return		現在のマウスの座標(中心が(0,0))
*/
XMFLOAT2 CDirectInput::getMousePos()
{
	XMFLOAT2 ret;
	POINT pos;
	GetCursorPos(&pos);
	ScreenToClient(m_hWnd, &pos);
	ret.x = pos.x - (float)(WINDOW_WIDTH / 2);
	ret.y = (float)(WINDOW_HEIGHT / 2) - pos.y;
	return ret;
}
