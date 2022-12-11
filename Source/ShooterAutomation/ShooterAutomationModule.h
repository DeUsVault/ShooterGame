// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

class FShooterAutomationModule : public IModuleInterface
{
	void StartupModule() override;
	void ShutdownModule() override;
	bool IsGameModule() const override { return false; }
};