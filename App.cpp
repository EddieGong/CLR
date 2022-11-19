#include <Framework.h>

import Utils;

using namespace winrt::Windows::ApplicationModel;
using namespace winrt::Windows::ApplicationModel::Core;
using namespace winrt::Windows::ApplicationModel::Activation;
using namespace winrt::Windows::UI::Core;
using namespace winrt::Windows::UI::Input;
using namespace winrt::Windows::Graphics::Display;

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
            framework = winrt::make_self<CLR::Framework>();
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
        CLR::ASSERT(false, "Uninitialize is NOT supported");
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Callback fuctions

    void OnActivated(CoreApplicationView const& /* applicationView */, IActivatedEventArgs const& /* args */)
    {
        CoreWindow::GetForCurrentThread().Activate();
    }

    winrt::fire_and_forget OnSuspending(IInspectable const& /* sender */, SuspendingEventArgs const& args)
    {
        auto lifetime = get_strong();

        //// Save app state asynchronously after requesting a deferral. Holding a deferral
        //// indicates that the application is busy performing suspending operations. Be
        //// aware that a deferral may not be held indefinitely. After about five seconds,
        //// the app will be forced to exit.
        SuspendingDeferral deferral = args.SuspendingOperation().GetDeferral();

        co_await winrt::resume_background();

        framework->Suspend();

        deferral.Complete();
    }

    void OnResuming(IInspectable const& /* sender */, IInspectable const& /* args */)
    {
        // Restore any data or state that was unloaded on suspend. By default, data
        // and state are persisted when resuming from suspend. Note that this event
        // does not occur if the app was previously terminated.
        framework->Resume();
    }

    void OnWindowClosed(CoreWindow const& /* sender */, CoreWindowEventArgs const& /* args */)
    {
        framework->Close();
    }

    void OnWindowActivationChanged(CoreWindow const& /* sender */, WindowActivatedEventArgs const& args)
    {
        framework->WindowActivationChanged(args.WindowActivationState());
    }

    void OnWindowSizeChanged(CoreWindow const& /* window */, WindowSizeChangedEventArgs const& args)
    {
        framework->WindowSizeChanged(CLR::Size(args.Size().Width, args.Size().Height));
    }

    void OnVisibilityChanged(CoreWindow const& /* sender */, VisibilityChangedEventArgs const& args)
    {
        framework->Visibility(args.Visible());
    }

    void OnDpiChanged(DisplayInformation const& sender, IInspectable const& /*args*/)
    {
        framework->DpiChanged(sender.LogicalDpi());
    }

    void OnOrientationChanged(DisplayInformation const& /*sender*/, IInspectable const& /* args */)
    {
        CLR::ASSERT(false, "OnOrientationChanged is NOT Supported");
    }

    void OnStereoEnabledChanged(DisplayInformation const& /* sender */, IInspectable const& /* args */)
    {
        CLR::ASSERT(false, "OnStereoEnabledChanged is NOT Supported");
    }

    void OnDisplayContentsInvalidated(DisplayInformation const& /* sender */, IInspectable const& /* args */)
    {
        framework->ValidateGraphicsDevice();
    }

private:
    winrt::com_ptr<CLR::Framework> framework;
};

int __stdcall wWinMain(HINSTANCE, HINSTANCE, PWSTR, int)
{
    CoreApplication::Run(winrt::make<App>());
}
