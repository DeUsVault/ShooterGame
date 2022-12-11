// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

class FShooterGameModule : public IModuleInterface
{
	void StartupModule() override;
	void ShutdownModule() override;
	bool IsGameModule() const override { return true; }
};