#pragma once
#include "Statebox.h"
class Statebox;
namespace presets
{
	enum isEnabled
	{
		OFF,
		ON
	};

	enum FPS
	{
		FRAMES_60,
		VS,
		UNLIMITED
	};

	enum Construction
	{
		NOTHING,
		TRIANGULAR,
		INCENTER
	};

	StateMenu stateMenu;

	const Vector2f size(500, 100);

	Statebox enableLatex(Vector2f(), size, "Enable LaTeX", 2);
	isEnabled latex;
	Statebox enableFullProvePrinting(Vector2f(), size, "Step-by-step solution", 2);
	isEnabled provePrinting;
	Statebox setFPS(Vector2f(), size, "Max framerate", 3);
	FPS fps;
	Statebox constructionPreset(Vector2f(), size, "Presets", 3);
	Construction preset;

	void stateboxesInit()
	{
		enableLatex.setStateTexture(0, "OFF", "Textures\\Button_textures\\Test.png");
		enableLatex.setStateTexture(1, "ON", "Textures\\Button_textures\\Test.png");

		enableFullProvePrinting.setStateTexture(0, "OFF", "Textures\\Button_textures\\Test.png");
		enableFullProvePrinting.setStateTexture(1, "ON", "Textures\\Button_textures\\Test.png");

		setFPS.setStateTexture(0, "60", "Textures\\Button_textures\\Test.png");
		setFPS.setStateTexture(1, "Vertical Sync", "Textures\\Button_textures\\Test.png");
		setFPS.setStateTexture(2, "Unlimited", "Textures\\Button_textures\\Test.png");

		constructionPreset.setStateTexture(0, "Nothing", "Textures\\Button_textures\\Test.png");
		constructionPreset.setStateTexture(1, "Triangular", "Textures\\Button_textures\\Test.png");
		constructionPreset.setStateTexture(2, "Incenter", "Textures\\Button_textures\\Test.png");

		stateMenu.pushStatebox(&enableLatex);
		stateMenu.pushStatebox(&enableFullProvePrinting);
		stateMenu.pushStatebox(&setFPS);
		stateMenu.pushStatebox(&constructionPreset);

		stateMenu.update();
	}

	void stateboxesApply()
	{
		latex = (isEnabled)enableLatex.getState();
		provePrinting = (isEnabled)enableFullProvePrinting.getState();
		fps = (FPS)setFPS.getState();
		preset = (Construction)constructionPreset.getState();
	}
}