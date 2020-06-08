#pragma once

#ifndef _FUNCIONES_
#define _FUNCIONES_

#include <WinUser.h>
#include <wtypes.h>
#include <glm/glm.hpp> // vec2, vec3, vec4 y radians
#include <string>
#include <time.h>

#define MSG_CAPTION_ERROR "Ha ocurrido un error"

using namespace std;
using namespace glm;

class Fun
{

public:

    enum TipoMensaje
    {
        Default = MB_ICONINFORMATION,
        Informacion = MB_ICONINFORMATION,
        Alerta = MB_ICONWARNING,
        Error = MB_ICONERROR
    };

    static vec2 GetDesktopWindowSize()
    {
        int width = 0;
        int height = 0;

        GetDesktopResolution(width, height);
        
        return vec2(width, height);
    }

    static vec2 GetWindowSize(HWND hWnd)
    {
        RECT rect;
        vec2 size(0.0f, 0.0f);

        if (GetWindowRect(hWnd, &rect))
        {
            size.x = rect.right - rect.left;
            size.y = rect.bottom - rect.top;
        }

        return size;
    }

    static vec2 GetWindowPosition(HWND hWnd)
    {
        RECT rect;
        vec2 pos(0.0f, 0.0f);

        if (GetWindowRect(hWnd, &rect))
        {
            pos.x = rect.left;
            pos.y = rect.top;
        }

        return pos;
    }

    static vec2 GetCursorPosition(HWND hWnd)
    {
        POINT p;
        vec2 pos(0.0f, 0.0f);

        if (ScreenToClient(hWnd, &p))
        {
            pos.x = p.x;
            pos.y = p.y;
        }

        return pos;
    }

    static vec2 GetCursorPosition()
    {
        POINT p;
        vec2 pos(0.0f, 0.0f);

        if (GetCursorPos(&p))
        {
            pos.x = p.x;
            pos.y = p.y;
        }

        return pos;
    }

    static void EstablecerModoPantallaCompleta(bool activar = true)
    {
        DEVMODE dvScreenSettings;
        vec2 screenResSize = GetDesktopWindowSize();

        memset(&dvScreenSettings, 0, sizeof(dvScreenSettings));
        dvScreenSettings.dmSize = sizeof(dvScreenSettings);
        dvScreenSettings.dmPelsWidth = (unsigned long)screenResSize.x;
        dvScreenSettings.dmPelsHeight = (unsigned long)screenResSize.y;
        dvScreenSettings.dmBitsPerPel = 32;
        dvScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

        ChangeDisplaySettings(&dvScreenSettings, CDS_FULLSCREEN);
    }

    static void ShowMessage(string titulo, string texto, TipoMensaje tipoMensaje = TipoMensaje::Default)
    {
        MessageBox(0, texto.c_str(), titulo.c_str(), tipoMensaje | MB_OK);
    }

    static string ResourcePath(string fileName) 
    {
        char executablePath[1024] = { '\0' };
        DWORD charsCopied = GetModuleFileName(NULL, executablePath, 1024);

        if (charsCopied > 0 && charsCopied < 1024)
            return "../GCI_PF_NAVE/res/" + fileName;
    }

private:

    static void GetDesktopResolution(int& horizontal, int& vertical)
    {
        RECT desktop;

        const HWND hDesktop = GetDesktopWindow();
        GetWindowRect(hDesktop, &desktop);

        horizontal = desktop.right;
        vertical = desktop.bottom;
    }

};

#endif // !_FUNCIONES_