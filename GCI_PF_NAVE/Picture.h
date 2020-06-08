#pragma once

#ifndef _PICTURE_
#define _PICTURE_

#include <Windows.h>
#include <gdiplus.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <exception>
#include "funciones.h"

class Picture
{

private:

	char* RutaImagen = 0;
	char* ImageArray = 0;
	int Width = 0;
	int Height = 0;

public:

	Picture(const char* rutaImagen)
	{
		RutaImagen = (char*)rutaImagen;

		CargarImagen();
	}

	~Picture()
	{
		delete[] ImageArray;
	}

	char* GetRutaImagen()
	{
		return RutaImagen;
	}

	char* GetBytesArray()
	{
		return ImageArray;
	}

	int GetWidth()
	{
		return Width;
	}

	int GetHeight()
	{
		return Height;
	}

private:

	void CargarImagen()
	{
		Gdiplus::GdiplusStartupInput gdiplusStartupInput;
		ULONG_PTR gdiplusToken;

		GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

		// agregue una pequeña conversion
		const size_t cSize = strlen(RutaImagen) + 1;
		wchar_t* wc = new wchar_t[cSize];
		size_t outSize;
		mbstowcs_s(&outSize, wc, cSize, RutaImagen, cSize - 1);

		Gdiplus::Bitmap* bitmap = new Gdiplus::Bitmap(wc);
		Gdiplus::BitmapData* bitmapData = new Gdiplus::BitmapData;

		Width = bitmap->GetWidth();
		Height = bitmap->GetHeight();

		Gdiplus::Rect rect(0, 0, Width, Height);

		bitmap->LockBits(&rect, Gdiplus::ImageLockModeRead, PixelFormat32bppARGB, bitmapData);

		unsigned char* pixels = (unsigned char*)bitmapData->Scan0;
		int tamaño = Width * Height * 4;

		try
		{
			ImageArray = new char[tamaño];
		}
		catch (std::exception& e)
		{
			Fun::ShowMessage(MSG_CAPTION_ERROR, "Espacio no suficiente en memoria para cargar imagen", Fun::TipoMensaje::Error);
			throw e;
		}

		for (unsigned int i = 0; i < (Width * Height * 4); i += 4)
		{
			ImageArray[i + 2] = pixels[i];
			ImageArray[i + 1] = pixels[i + 1];
			ImageArray[i] = pixels[i + 2];
			ImageArray[i + 3] = pixels[i + 3];
		}

		bitmap->UnlockBits(bitmapData);

		delete bitmapData;
		delete bitmap;

		Gdiplus::GdiplusShutdown(gdiplusToken);
	}

};

#endif // !_PICTURE_
