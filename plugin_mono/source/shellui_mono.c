#include "mono.h"
#include "../../common/plugin_common.h"
#include "shellui_mono.h"
#include <stdbool.h>

void UI_ResetItem(const char* id)
{
    const void* uii = Mono_Get_InstanceEx(App_Exe, "Sce.Vsh.ShellUI.Settings.Core", "UIManager", "Instance");
    final_printf("UIManager 0x%p\n", uii);
    if (!uii)
    {
        return;
    }
    void* buttonmethod = Mono_Get_Address_of_Method(App_Exe, "Sce.Vsh.ShellUI.Settings.Core", "UIManager", "ResetMenuItem", 1);
    final_printf("ResetMenuItem 0x%p\n", buttonmethod);
    if (buttonmethod)
    {
        void (*Reset)(const void* i, MonoString* mid) = (void*)buttonmethod;
        final_printf("Firing reset with id %s\n", id);
        Reset(uii, Mono_New_String(id));
    }
}

void UI_AddItem(void* e)
{
    const void* uii = Mono_Get_InstanceEx(App_Exe, "Sce.Vsh.ShellUI.Settings.Core", "UIManager", "Instance");
    final_printf("UIManager 0x%p\n", uii);
    if (!uii)
    {
        return;
    }
    void* buttonmethod = Mono_Get_Address_of_Method(App_Exe, "Sce.Vsh.ShellUI.Settings.Core", "UIManager", "InsertMenuItem", 2);
    final_printf("InsertMenuItem 0x%p\n", buttonmethod);
    if (buttonmethod)
    {
        void (*AddMenuItem)(const void* i, void* e, MonoString* mid) = (void*)buttonmethod;
        final_printf("Firing add!\n");
        AddMenuItem(uii, e, Mono_New_String(""));
    }
}

static void* UI_CreateButton(const char* bb, const char* Id, const char* Title, const char* Title2, const char* Icon)
{
    void* ButtonElementData = Mono_Get_Class(App_Exe, "Sce.Vsh.ShellUI.Settings.Core", bb);
    void* ElementData = Mono_Get_Class(App_Exe, "Sce.Vsh.ShellUI.Settings.Core", "ElementData");
    void* Instance = Mono_New_Object(ButtonElementData);
    mono_runtime_object_init(Instance);
    Mono_Set_Property(ElementData, Instance, "Id", Mono_New_String(Id));
    Mono_Set_Property(ElementData, Instance, "Title", Mono_New_String(Title));
    Mono_Set_Property(ElementData, Instance, "Description", Mono_New_String(Title2));
    if (Icon && Icon[0])
    {
        Mono_Set_Property(ElementData, Instance, "Icon", Mono_New_String(Icon));
    }
    return Instance;
}

void* UI_NewElementData(enum ShellUIElementType ElementType, const char* Id, const char* Title, const char* Title2, const char* Icon)
{
    bool is_default = false;
    const char* btype = "ButtonElementData";
    switch (ElementType)
    {
        case Button:
        {
            // will go to default return, so no need to copy code here
            break;
        }
        case ToggleSwitch:
        {
            btype = "ToggleSwitchElementData";
            break;
        }
        default:
        {
            is_default = true;
            break;
        }
    }
    if (is_default)
    {
        ffinal_printf("Fallback, create a button as default\n");
    }
    return UI_CreateButton(btype, Id, Title, Title2, Icon);
}

void UI_Push(const char* page, const char* root)
{
    const void* uii = Mono_Get_InstanceEx(App_Exe, "Sce.Vsh.ShellUI.Settings.Core", "UIManager", "Instance");
    if (!uii)
    {
        return;
    }
    static void* buttonmethod = 0;
    if (!buttonmethod)
    {
        buttonmethod = Mono_Get_Address_of_Method(App_Exe, "Sce.Vsh.ShellUI.Settings.Core", "UIManager", "Push", 3);
    }
    if (buttonmethod)
    {
        enum
        {
            kDefault = 0,
        };
        void (*Push)(const void* i, MonoString*, MonoString*, const uint32_t pushTransType) = (void*)buttonmethod;
        Push(uii, Mono_New_String(page), Mono_New_String(root && root[0] ? root : ""), kDefault);
    }
}

void UI_PushPage(const char* page)
{
    UI_Push(page, 0);
}

void BootPage(const char* page)
{
    static void* buttonmethod = 0;
    if (!buttonmethod)
    {
        buttonmethod = Mono_Get_Address_of_Method(App_Exe, "Sce.Vsh.ShellUI.AppSystem", "BootHelper", "BootAndAdd", 3);
    }
    if (buttonmethod)
    {
        bool (*boot_add)(void* s, void*, void*) = (void*)buttonmethod;
        char url[1024] = {0};
        snprintf(url, _countof_1(url), "pssettings:play?mode=settings&function=%s", page);
        boot_add(Mono_New_String(url), 0, 0);
    }
}
