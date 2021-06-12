// Fill out your copyright notice in the Description page of Project Settings.


#include "RunFlaskGameMode.h"
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <fstream>
//#define system(_x) (std::cout << (_x) << std::endl, system(_x))

std::string exec(const char* cmd) {
	std::array<char, 128> buffer;
	std::string result;
	std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(cmd, "r"), _pclose);
	if (!pipe) {
		throw std::runtime_error("popen() failed!");
	}
	while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
		result += buffer.data();
	}
	return result;
}

void ARunFlaskGameMode::StartPlay()
{
	Super::StartPlay();
	RunFlaskOnStartUp();
}

ARunFlaskGameMode::ARunFlaskGameMode()
{
	RunFlaskOnStartUp();
}

void ARunFlaskGameMode::RunFlaskOnStartUp()
{
	// std::ofstream out("C:\\Projects\\Unreal Projects\\VoiceProj\\Source\\Log.txt");
	//FPlatformProcess::CreateProc(TEXT("ping google.com -t"), nullptr, false, false, false, nullptr, 0, nullptr, nullptr);
	// for (auto &it : ConsoleCommands)
	// {
	// 	UE_LOG(LogTemp, Error, TEXT("%d"), ANSI_TO_TCHAR(std::system(TCHAR_TO_ANSI(*it))));
	// 	//out << system(TCHAR_TO_ANSI(*it)) << std::endl;
	// 	//out << system(TCHAR_TO_ANSI(*it)) << std::endl;
		
	// }
	// out.close();
	
}

//void ARunFlaskGameMode::StopThread()
//{
//
//}
//
//void ARunFlaskGameMode::EnsureCompletion()
//{
//
//}
//
//void ARunFlaskGameMode::Shutdown()
//{
//
//}
//
//bool ARunFlaskGameMode::IsThreadFinished()
//{
//	return true;
//}
