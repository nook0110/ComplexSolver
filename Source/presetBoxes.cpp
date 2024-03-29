#include "presetBoxes.h"
using namespace presets;


StateMenu stateMenu;

const Vector2f size(1000, 200);

Statebox enableLatex(Vector2f(), size, "Enable LaTeX", 2);
isEnabled latex;
Statebox enableFullProvePrinting(Vector2f(), size, "Step-by-step solution", 2);
isEnabled provePrinting;
Statebox setFPS(Vector2f(), size, "Max framerate", 3);
FPS fps;
Statebox constructionPreset(Vector2f(), size, "Presets", 4);
Construction preset;

void presets::stateboxesInit()
{
	{
		enableLatex.setStateTexture(0, "OFF", "Textures/Checkbox_textures/PresetOFF.png");
		enableLatex.setStateTexture(1, "ON", "Textures/Checkbox_textures/PresetON.png");

		enableFullProvePrinting.setStateTexture(0, "OFF", "Textures/Checkbox_textures/PresetOFF.png");
		enableFullProvePrinting.setStateTexture(1, "ON", "Textures/Checkbox_textures/PresetON.png");

		setFPS.setStateTexture(0, "60", "Textures/Checkbox_textures/Preset60.png");
		setFPS.setStateTexture(1, "Vertical Sync", "Textures/Checkbox_textures/PresetVS.png");
		setFPS.setStateTexture(2, "Unlimited", "Textures/Checkbox_textures/PresetUnlimited.png");

		constructionPreset.setStateTexture(0, "Empty", "Textures/Checkbox_textures/PresetEmpty.png");
		constructionPreset.setStateTexture(1, "Triangle", "Textures/Checkbox_textures/PresetTriangle.png");
		constructionPreset.setStateTexture(2, "Incenter", "Textures/Checkbox_textures/PresetIncenter.png");
		constructionPreset.setStateTexture(3, "Orthocenter", "Textures/Checkbox_textures/PresetOrthocenter.png");
		//stateMenu.pushStatebox(&enableLatex);
		//stateMenu.pushStatebox(&enableFullProvePrinting);
		stateMenu.pushStatebox(&setFPS);
		stateMenu.pushStatebox(&constructionPreset);

		stateMenu.update();
	}
}

void presets::stateboxesApply()
{
	latex = (isEnabled)enableLatex.getState();
	provePrinting = (isEnabled)enableFullProvePrinting.getState();
	fps = (FPS)setFPS.getState();
	preset = (Construction)constructionPreset.getState();
}
