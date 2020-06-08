#pragma once

#ifndef _GAMEPADRR_
#define _GAMEPADRR_

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <Xinput.h>

class GamePadRR
{

public:

    enum class Boton
    {
        A = XINPUT_GAMEPAD_A,
        B = XINPUT_GAMEPAD_B,
        X= XINPUT_GAMEPAD_X,
        Y= XINPUT_GAMEPAD_Y,
        START = XINPUT_GAMEPAD_START,
        BACK = XINPUT_GAMEPAD_BACK,
        ARRIBA = XINPUT_GAMEPAD_DPAD_UP,
        DERECHA = XINPUT_GAMEPAD_DPAD_RIGHT,
        ABAJO = XINPUT_GAMEPAD_DPAD_DOWN,
        IZQUIERDA = XINPUT_GAMEPAD_DPAD_LEFT,
        LT = XINPUT_GAMEPAD_LEFT_SHOULDER,
        STICK_IZQUIERDO = XINPUT_GAMEPAD_LEFT_THUMB,
        RT = XINPUT_GAMEPAD_RIGHT_SHOULDER,
        STICK_DERECHO = XINPUT_GAMEPAD_RIGHT_THUMB,
    };

private:
    
    XINPUT_STATE State;
    int NumeroControl;
    const float ValorMaximoStick = 32767.0f;
    const float SensibilidadMuerta = 0.19f;

    int botonesPresionados[14];

public:

    GamePadRR(int numeroControl = 1)
    {
        NumeroControl = numeroControl - 1;
    }

    bool EstaConectado()
    {
        LimpiarEstado();
        return XInputGetState(NumeroControl, &State) == ERROR_SUCCESS;
    }

    bool BotonPresionado(Boton boton)
    {
        if (!EstaConectado())
            return false;

        return (DWORD)boton == GetState().Gamepad.wButtons;
    }

    float GetStickLeftVertical()
    {
        if (!EstaConectado())
            return 0.0f;

        float valor = (float)GetState().Gamepad.sThumbLY / ValorMaximoStick;

        return valor > -SensibilidadMuerta && valor < SensibilidadMuerta ? 0.0f : valor;
    }

    float GetStickLeftHorizontal()
    {
        if (!EstaConectado())
            return 0.0f;

        float valor = (float)GetState().Gamepad.sThumbLX / ValorMaximoStick;

        return valor > -SensibilidadMuerta && valor < SensibilidadMuerta ? 0.0f : valor;
    }

    float GetStickRightVertical()
    {
        if (!EstaConectado())
            return 0.0f;

        float valor = (float)GetState().Gamepad.sThumbRY / ValorMaximoStick;

        return valor > -SensibilidadMuerta && valor < SensibilidadMuerta ? 0.0f : valor;
    }

    float GetStickRightHorizontal()
    {
        if (!EstaConectado())
            return 0.0f;

        float valor = (float)GetState().Gamepad.sThumbRX / ValorMaximoStick;

        return valor > -SensibilidadMuerta && valor < SensibilidadMuerta ? 0.0f : valor;
    }

private:

    XINPUT_STATE GetState()
    {
        LimpiarEstado();
        XInputGetState(NumeroControl, &State);

        return State;
    }

    void LimpiarEstado()
    {
        ZeroMemory(&State, sizeof(XINPUT_STATE));
    }

};

#endif // !_GAMEPADRR_