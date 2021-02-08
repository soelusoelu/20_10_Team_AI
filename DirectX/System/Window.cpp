#include "Window.h"
#include "../Imgui/imgui.h"
#include "../Imgui/imgui_impl_dx11.h"
#include "../Imgui/imgui_impl_win32.h"
#include "../Utility/LevelLoader.h"
#include "../Utility/StringUtil.h"

Window* gWindow = nullptr;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    return gWindow->msgProc(hWnd, uMsg, wParam, lParam);
}

Window::Window()
    : mhWnd(nullptr)
    , mWndClass()
    , mMouseWheelScrollValueSetter(nullptr)
    , mTitle("")
{
}

Window::~Window() {
    //ウィンドウクラスを登録解除
    UnregisterClass(mWndClass.lpszClassName, mWndClass.hInstance);
}

void Window::initialize(IMouseWheelScrollValueSetter* setter) {
    mMouseWheelScrollValueSetter = setter;
}

void Window::createWindow(HINSTANCE hInstance) {
    gWindow = this;

    // ウィンドウの定義
    mWndClass.cbSize = sizeof(WNDCLASSEX);
    //mWndClass.style = CS_HREDRAW | CS_VREDRAW;
    mWndClass.lpfnWndProc = WndProc;
    mWndClass.hInstance = hInstance;
    //mWndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    mWndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    //mWndClass.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
    auto wcharTitle = StringUtil::charToWchar(mTitle);
    mWndClass.lpszClassName = wcharTitle.c_str();
    //mWndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    //ウィンドウクラスをOSに登録
    RegisterClassEx(&mWndClass);

    //ウィンドウサイズ { X 座標 Y 座標 横幅 縦幅 }
    RECT wrc{};
#ifdef _DEBUG
    wrc = { 0, 0, mDebugWidth, mDebugHeight };
#else
    wrc = { 0, 0, mGameWidth, mGameHeight };
#endif // _DEBUG
    AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false); //自動でサイズ補正

    //ウィンドウの作成
    mhWnd = CreateWindow(
        mWndClass.lpszClassName,    //クラス名
        wcharTitle.c_str(),         //タイトルバーの文字
        WS_OVERLAPPEDWINDOW,        //標準的なウィンドウスタイル
        CW_USEDEFAULT,              //表示X座標(OSに任せる)
        CW_USEDEFAULT,              //表示Y座標(OSに任せる)
        wrc.right - wrc.left,       //ウィンドウ横幅
        wrc.bottom - wrc.top,       //ウィンドウ縦幅
        nullptr,                    //親ウィンドウハンドル
        nullptr,                    //メニューハンドル
        hInstance,                  //呼び出しアプリケーションハンドル
        nullptr                     //オプション
    );

    //ウインドウの表示
#ifdef _DEBUG
    //デバッグ時は最大表示
    ShowWindow(mhWnd, SW_MAXIMIZE);
#else
    //リリース時は1920 * 1080の場合最大表示
    //それ以外の場合ウィンドウサイズで表示
    if (mGameWidth == mStandardWidth && mGameHeight == mStandardHeight) {
        ShowWindow(mhWnd, SW_MAXIMIZE);
    } else {
        ShowWindow(mhWnd, SW_SHOW);
    }
#endif // _DEBUG

    UpdateWindow(mhWnd);
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

LRESULT Window::msgProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {
    switch (iMsg) {
    case WM_KEYDOWN:
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_MOUSEWHEEL:
        mMouseWheelScrollValueSetter->setWheelScrollValue(GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA);
        break;
    }

    ImGui_ImplWin32_WndProcHandler(hWnd, iMsg, wParam, lParam);
    return DefWindowProc(hWnd, iMsg, wParam, lParam);
}

HWND Window::gethWnd() const {
    return mhWnd;
}

void Window::update() {
    updateWindowToClientSize();
}

int Window::width() {
    return mGameWidth;
}

int Window::height() {
    return mGameHeight;
}

int Window::standardWidth() {
    return mStandardWidth;
}

int Window::standardHeight() {
    return mStandardHeight;
}

int Window::debugWidth() {
    return mDebugWidth;
}

int Window::debugHeight() {
    return mDebugHeight;
}

Vector2 Window::windowToClientSize() {
    return mWindowToClientSize;
}

Vector2 Window::getWindowCorrect() {
    return Vector2(
        static_cast<float>(mGameWidth) / static_cast<float>(mStandardWidth),
        static_cast<float>(mGameHeight) / static_cast<float>(mStandardHeight)
    );
}

void Window::loadProperties(const rapidjson::Value& inObj) {
    const auto& windowObj = inObj["window"];
    if (windowObj.IsObject()) {
        JsonHelper::getString(windowObj, "title", &mTitle);
        JsonHelper::getInt(windowObj, "windowWidth", &mWidth);
        JsonHelper::getInt(windowObj, "windowHeight", &mHeight);
        JsonHelper::getInt(windowObj, "releaseWindowWidth", &mReleaseWidth);
        JsonHelper::getInt(windowObj, "releaseWindowHeight", &mReleaseHeight);
        JsonHelper::getInt(windowObj, "windowStandardWidth", &mStandardWidth);
        JsonHelper::getInt(windowObj, "windowStandardHeight", &mStandardHeight);
        JsonHelper::getInt(windowObj, "windowDebugWidth", &mDebugWidth);
        JsonHelper::getInt(windowObj, "windowDebugHeight", &mDebugHeight);

#ifdef _DEBUG
        mGameWidth = mWidth;
        mGameHeight = mHeight;
#else
        mGameWidth = mReleaseWidth;
        mGameHeight = mReleaseHeight;
#endif // _DEBUG
    }
}

void Window::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const {
    rapidjson::Value props(rapidjson::kObjectType);
    JsonHelper::setString(alloc, &props, "title", mTitle);
    JsonHelper::setInt(alloc, &props, "windowWidth", mWidth);
    JsonHelper::setInt(alloc, &props, "windowHeight", mHeight);
    JsonHelper::setInt(alloc, &props, "releaseWindowWidth", mReleaseWidth);
    JsonHelper::setInt(alloc, &props, "releaseWindowHeight", mReleaseHeight);
    JsonHelper::setInt(alloc, &props, "windowStandardWidth", mStandardWidth);
    JsonHelper::setInt(alloc, &props, "windowStandardHeight", mStandardHeight);
    JsonHelper::setInt(alloc, &props, "windowDebugWidth", mDebugWidth);
    JsonHelper::setInt(alloc, &props, "windowDebugHeight", mDebugHeight);

    inObj.AddMember("window", props, alloc);
}

void Window::updateWindowToClientSize() {
    RECT wRect{}, cRect{}; //ウィンドウ全体の矩形、クライアント領域の矩形
    int ww = 0, wh = 0;        //ウィンドウ全体の幅、高さ
    int cw = 0, ch = 0;        //クライアント領域の幅、高さ //ウィンドウ全体の幅・高さを計算

    //ウィンドウ全体の幅・高さを計算
    //GetWindowRect(mhWnd, &wRect);
    //ww = wRect.right - wRect.left;
    //wh = wRect.bottom - wRect.top;
    //クライアント領域の幅・高さを計算
    GetClientRect(mhWnd, &cRect);
    cw = cRect.right - cRect.left;
    ch = cRect.bottom - cRect.top;
    //ウィンドウとクライアントの比率を計算
#ifdef _DEBUG
    mWindowToClientSize.x = static_cast<float>(mDebugWidth) / static_cast<float>(cw);
    mWindowToClientSize.y = static_cast<float>(mDebugHeight) / static_cast<float>(ch);
#else
    mWindowToClientSize.x = static_cast<float>(mReleaseWidth) / static_cast<float>(cw);
    mWindowToClientSize.y = static_cast<float>(mReleaseHeight) / static_cast<float>(ch);
#endif // _DEBUG
}
