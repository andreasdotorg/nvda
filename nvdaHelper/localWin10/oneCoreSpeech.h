/*
Header for C dll bridge to Windows OneCore voices.
This file is a part of the NVDA project.
URL: http://www.nvaccess.org/
Copyright 2016-2022 Tyler Spivey, NV Access Limited.
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 2.0, as published by
    the Free Software Foundation.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
This license can be found at:
http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
*/

#include <mutex>
#pragma once
#define export __declspec(dllexport)

typedef void (*ocSpeech_Callback)(BYTE* data, int length, const wchar_t* markers);

class SpeechCounter {
private:
	std::mutex speechThreadsMutex_;
	std::condition_variable cond_var_;
	std::atomic_int speechThreads;
public:
	std::mutex preventSpeechMutex_;
	SpeechCounter();
	void waitUntilSpeechFinished();
	void markCallbackFinished();
	void markSpeechStarted();
	void reset();
	bool hasSpeechFinished();
};

class OcSpeech {
private:
	winrt::Windows::Media::SpeechSynthesis::SpeechSynthesizer synth{ nullptr };
	ocSpeech_Callback callback;
public:
	OcSpeech();
	winrt::fire_and_forget speak(winrt::hstring text);
	void setCallback(ocSpeech_Callback fn);
	std::wstring getVoices();
	winrt::hstring getCurrentVoiceId();
	void setVoice(int index);
	winrt::hstring getCurrentVoiceLanguage();
	double getPitch();
	void setPitch(double pitch);
	double getVolume();
	void setVolume(double volume);
	double getRate();
	void setRate(double rate);
	void performCallback(
		BYTE* data,
		int length,
		const wchar_t* markers
	);
	SpeechCounter _speechCounter;
};

extern "C" {
	export bool __stdcall ocSpeech_supportsProsodyOptions();
	export OcSpeech* __stdcall ocSpeech_initialize();
	export void __stdcall ocSpeech_terminate(OcSpeech* instance);
	export void __stdcall ocSpeech_setCallback(OcSpeech* instance, ocSpeech_Callback fn);
	export void __stdcall ocSpeech_speak(OcSpeech* instance, wchar_t* text);
	export BSTR __stdcall ocSpeech_getVoices(OcSpeech* instance);
	export const wchar_t* __stdcall ocSpeech_getCurrentVoiceId(OcSpeech* instance);
	export void __stdcall ocSpeech_setVoice(OcSpeech* instance, int index);
	export const wchar_t* __stdcall ocSpeech_getCurrentVoiceLanguage(OcSpeech* instance);
	export double __stdcall ocSpeech_getPitch(OcSpeech* instance);
	export void __stdcall ocSpeech_setPitch(OcSpeech* instance, double pitch);
	export double __stdcall ocSpeech_getVolume(OcSpeech* instance);
	export void __stdcall ocSpeech_setVolume(OcSpeech* instance, double volume);
	export double __stdcall ocSpeech_getRate(OcSpeech* instance);
	export void __stdcall ocSpeech_setRate(OcSpeech* instance, double rate);
}
