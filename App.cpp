#include <Framework.h>

import Utils;

using namespace winrt::Windows;
using namespace winrt::Windows::ApplicationModel::Core;
using namespace winrt::Windows::Foundation::Numerics;
using namespace winrt::Windows::UI;
using namespace winrt::Windows::UI::Core;
using namespace winrt::Windows::UI::Input;

using namespace winrt::Windows::ApplicationModel;
using namespace winrt::Windows::ApplicationModel::Activation;
using namespace winrt::Windows::Graphics::Display;

using namespace CLR;

struct App : winrt::implements<App, IFrameworkViewSource, IFrameworkView>
{
    IFrameworkView CreateView()
    {
        return *this;
    }

    void Initialize(CoreApplicationView const& applicationView)
    {
        applicationView.Activated({ this, &App::OnActivated });

        CoreApplication::Suspending({ this, &App::OnSuspending });
        CoreApplication::Resuming  ({ this, &App::OnResuming });

        if (framework == nullptr)
        {
            framework = winrt::make_self<Framework>();
            framework->Init();
        }
    }

    void SetWindow(CoreWindow const& window)
    {
        window.PointerCursor(CoreCursor(CoreCursorType::Arrow, 0));
        window.Activated        ({ this, &App::OnWindowActivationChanged });
        window.SizeChanged      ({ this, &App::OnWindowSizeChanged });
        window.Closed           ({ this, &App::OnWindowClosed });
        window.VisibilityChanged({ this, &App::OnVisibilityChanged });

        DisplayInformation currentDisplayInformation{ DisplayInformation::GetForCurrentView() };
        currentDisplayInformation.DpiChanged          ({ this, &App::OnDpiChanged });
        currentDisplayInformation.OrientationChanged  ({ this, &App::OnOrientationChanged });
        currentDisplayInformation.StereoEnabledChanged({ this, &App::OnStereoEnabledChanged });
        DisplayInformation::DisplayContentsInvalidated({ this, &App::OnDisplayContentsInvalidated });

        PointerVisualizationSettings visualizationSettings{ PointerVisualizationSettings::GetForCurrentView() };
        visualizationSettings.IsContactFeedbackEnabled(false);
        visualizationSettings.IsBarrelButtonFeedbackEnabled(false);

        framework->SetWindow(window);
    }

    void Load(winrt::hstring const& /* entryPoint */)
    {
        framework->Load();
    }

    void Run()
    {
        framework->Run();
    }

    void Uninitialize()
    {
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Callback fuctions

    void OnActivated(CoreApplicationView const& /* applicationView */, IActivatedEventArgs const& /* args */)
    {
        CoreWindow window = CoreWindow::GetForCurrentThread();
        window.Activate();
    }

    winrt::fire_and_forget OnSuspending(IInspectable const& /* sender */, SuspendingEventArgs const& args)
    {
        //auto lifetime = get_strong();

        //// Save app state asynchronously after requesting a deferral. Holding a deferral
        //// indicates that the application is busy performing suspending operations. Be
        //// aware that a deferral may not be held indefinitely. After about five seconds,
        //// the app will be forced to exit.
        //SuspendingDeferral deferral = args.SuspendingOperation().GetDeferral();

        co_await winrt::resume_background();

        //m_deviceResources->Trim();

        //m_main->Suspend();

        //deferral.Complete();
    }

    void OnResuming(IInspectable const& /* sender */, IInspectable const& /* args */)
    {
        // Restore any data or state that was unloaded on suspend. By default, data
        // and state are persisted when resuming from suspend. Note that this event
        // does not occur if the app was previously terminated.
        //m_main->Resume();
    }

    void OnWindowClosed(CoreWindow const& /* sender */, CoreWindowEventArgs const& /* args */)
    {
        //m_main->Close();
    }

    void OnWindowActivationChanged(CoreWindow const& /* sender */, WindowActivatedEventArgs const& /*args*/)
    {
        //m_main->WindowActivationChanged(args.WindowActivationState());
    }

    void OnWindowSizeChanged(CoreWindow const& /* window */, WindowSizeChangedEventArgs const& /*args*/)
    {
        //m_deviceResources->SetLogicalSize(args.Size());
        //m_main->CreateWindowSizeDependentResources();
    }

    void OnVisibilityChanged(CoreWindow const& /* sender */, VisibilityChangedEventArgs const& /*args*/)
    {
        //m_main->Visibility(args.Visible());
    }

    void OnDpiChanged(DisplayInformation const& /*sender*/, IInspectable const& /* args */)
    {
        //m_deviceResources->SetDpi(sender.LogicalDpi());
        //m_main->CreateWindowSizeDependentResources();
    }

    void OnOrientationChanged(DisplayInformation const& /*sender*/, IInspectable const& /* args */)
    {
        //m_deviceResources->SetCurrentOrientation(sender.CurrentOrientation());
        //m_main->CreateWindowSizeDependentResources();
    }

    void OnStereoEnabledChanged(DisplayInformation const& /* sender */, IInspectable const& /* args */)
    {
        //m_deviceResources->UpdateStereoState();
        //m_main->CreateWindowSizeDependentResources();
    }

    void OnDisplayContentsInvalidated(DisplayInformation const& /* sender */, IInspectable const& /* args */)
    {
        //m_deviceResources->ValidateDevice();
    }

private:
    winrt::com_ptr<Framework> framework;
};

int __stdcall wWinMain(HINSTANCE, HINSTANCE, PWSTR, int)
{
    CoreApplication::Run(winrt::make<App>());
}
