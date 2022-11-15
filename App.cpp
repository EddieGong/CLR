#include "pch.h"

#include <CLRMain.h>

using namespace winrt::Windows;
using namespace winrt::Windows::ApplicationModel::Core;
using namespace winrt::Windows::Foundation::Numerics;
using namespace winrt::Windows::UI;
using namespace winrt::Windows::UI::Core;
using namespace winrt::Windows::UI::Input;

using namespace winrt::Windows::ApplicationModel;
using namespace winrt::Windows::ApplicationModel::Activation;

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

        CoreApplication::Resuming({ this, &App::OnResuming });

        if (main == nullptr)
        {
            main = winrt::make_self<CLRMain>();
        }
    }

    void Load(winrt::hstring const& /* entryPoint */)
    {
        main->Load();
    }

    void Uninitialize()
    {
    }

    void Run()
    {
        main->Run();
    }

    void SetWindow(CoreWindow const& window)
    {
        window.PointerCursor(CoreCursor(CoreCursorType::Arrow, 0));

        PointerVisualizationSettings visualizationSettings{ PointerVisualizationSettings::GetForCurrentView() };
        visualizationSettings.IsContactFeedbackEnabled(false);
        visualizationSettings.IsBarrelButtonFeedbackEnabled(false);
    }

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

private:
    winrt::com_ptr<CLRMain> main;
};

int __stdcall wWinMain(HINSTANCE, HINSTANCE, PWSTR, int)
{
    CoreApplication::Run(winrt::make<App>());
}
