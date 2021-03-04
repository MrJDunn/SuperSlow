/*
  ==============================================================================

	Style.h
	Created: 4 Mar 2021 12:55:02pm
	Author:  Jeff

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class Style : public LookAndFeel_V4
{
public:
	Style() 
	{
		setColour(Slider::ColourIds::textBoxOutlineColourId, colors.invisible);
	};

	~Style() {};

	// For toggle buttons
	void drawTickBox(Graphics& g, Component& /*component*/,
		float x, float y, float w, float h,
		const bool ticked,
		const bool isEnabled,
		const bool shouldDrawButtonAsHighlighted,
		const bool shouldDrawButtonAsDown) override
	{
		ignoreUnused(isEnabled, shouldDrawButtonAsHighlighted, shouldDrawButtonAsDown);

		auto diameter = std::min(w, h);
		auto radius = diameter / 2.f;

		auto bounds = juce::Rectangle<int>(x + w / 2 - radius, y + h / 2 - radius, diameter, diameter).toFloat();

		if (ticked)
		{
			g.setGradientFill(ColourGradient(
				colors.tertiraryLight, bounds.getCentreX(), bounds.getCentreY(),
				colors.secondaryLight, bounds.getWidth() * 2, bounds.getHeight() * 2, true));

			auto ellipseSpace = bounds.reduced(2);
			g.fillEllipse(ellipseSpace);

			g.setColour(colors.primary);
			g.drawEllipse(bounds, 1.f);
		}
		else
		{
			g.setColour(colors.secondary);

			auto ellipseSpace = bounds.reduced(2);
			g.fillEllipse(ellipseSpace);

			g.setColour(colors.secondaryLight);
			g.drawEllipse(bounds, 1.f);
		}
	} // drawTickBox

	void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos,
		const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider) override
	{
		auto diameter = std::min(width, height);
		auto radius = diameter / 2.f;

		auto bounds = juce::Rectangle<int>(x + width / 2 - radius, y + height / 2 - radius, diameter, diameter).toFloat();

		auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
		auto lineW = 1.0f;// jmin(8.0f, radius * 0.5f);
		auto arcRadius = radius - lineW * 0.5f;

		g.setGradientFill(ColourGradient(
			colors.tertiraryLight, bounds.getCentreX(), bounds.getCentreY(),
			colors.secondaryLight, bounds.getCentreX() + 2 * diameter, bounds.getCentreY() + 2 * diameter, true));
		g.fillEllipse(bounds.reduced(4));

		Path valueArc;
		valueArc.addCentredArc(bounds.getCentreX(),
			bounds.getCentreY(),
			arcRadius,
			arcRadius,
			0.0f,
			rotaryStartAngle,
			toAngle,
			true);

		g.setColour(colors.primaryLight);
		g.strokePath(valueArc, PathStrokeType(lineW, PathStrokeType::curved, PathStrokeType::rounded));

		auto thumbWidth = lineW * 5.0f;
		Point<float> thumbPoint(bounds.getCentreX() + (arcRadius - 7) * std::cos(toAngle - MathConstants<float>::halfPi),
			bounds.getCentreY() + (arcRadius - 7) * std::sin(toAngle - MathConstants<float>::halfPi));

		g.setColour(colors.white);
		g.fillEllipse(juce::Rectangle<float>(thumbWidth, thumbWidth).withCentre(thumbPoint));


	} // drawRotarySlider

	void drawButtonBackground(Graphics& g,
		Button& button,
		const Colour& /*backgroundColour*/,
		bool /*shouldDrawButtonAsHighlighted*/,
		bool /*shouldDrawButtonAsDown*/) override
	{
		auto cornerSize = 5.0f;
		juce::Rectangle<int> boxBounds(0, 0, button.getWidth(), button.getHeight());

		g.setGradientFill(ColourGradient(
			button.getLookAndFeel().findColour(MidiKeyboardComponent::ColourIds::keySeparatorLineColourId),
			(float)button.getWidth(), (float)button.getHeight(),
			Colour::fromRGB(17, 17, 17),
			(float)button.getWidth(), (float)button.getHeight() / 2.0f, false));
		g.fillRoundedRectangle(boxBounds.toFloat(), cornerSize);

		g.setColour(Colour::fromRGB(15, 15, 15));
		g.drawRoundedRectangle(boxBounds.toFloat().reduced(0.5f, 0.5f), cornerSize, 0.25f);
	} // drawButtonBackground

	void fillTextEditorBackground(Graphics& g, int width, int height, TextEditor& textEditor) override
	{
		if (dynamic_cast<AlertWindow*> (textEditor.getParentComponent()) != nullptr)
		{
			auto cornerSize = 5.0f;
			juce::Rectangle<int> boxBounds(0, 0, width, height);

			g.setGradientFill(ColourGradient(
				findColour(MidiKeyboardComponent::ColourIds::keySeparatorLineColourId),
				(float)width, (float)height,
				Colour::fromRGB(17, 17, 17),
				(float)width, (float)height / 2.0f, false));
			g.fillRoundedRectangle(boxBounds.toFloat(), cornerSize);

			g.setColour(Colour::fromRGB(15, 15, 15));
			g.drawRoundedRectangle(boxBounds.toFloat().reduced(0.5f, 0.5f), cornerSize, 0.25f);

			g.setColour(textEditor.findColour(TextEditor::outlineColourId));
			g.drawHorizontalLine(height - 1, 0.0f, static_cast<float> (width));
		}
		else
		{
			auto cornerSize = 5.0f;
			juce::Rectangle<int> boxBounds(0, 0, width, height);

			g.setGradientFill(ColourGradient(
				findColour(MidiKeyboardComponent::ColourIds::keySeparatorLineColourId),
				(float)width, (float)height,
				Colour::fromRGB(17, 17, 17),
				(float)width, (float)height / 2.0f, false));
			g.fillRoundedRectangle(boxBounds.toFloat(), cornerSize);

			g.setColour(Colour::fromRGB(15, 15, 15));
			g.drawRoundedRectangle(boxBounds.toFloat().reduced(0.5f, 0.5f), cornerSize, 0.25f);
		}
	} // fillTextEditorBackground

	void drawTextEditorOutline(Graphics& g, int width, int height, TextEditor& textEditor)
	{
	}


	struct StyleColours
	{
		// Blues
		Colour primary = Colour::fromRGB(21, 101, 192);
		Colour primaryLight = Colour::fromRGB(94, 146, 243);
		Colour primaryDark = Colour::fromRGB(0, 60, 143);

		// Blacks
		Colour secondary = Colour::fromRGB(33, 33, 33);
		Colour secondaryLight = Colour::fromRGB(72, 72, 72);
		Colour secondaryDark = Colour::fromRGB(0, 0, 0);

		// Yellows
		Colour tertirary = Colour::fromRGB(255, 162, 0);
		Colour tertiraryLight = Colour::fromRGB(255, 210, 73);
		Colour tertiraryDark = Colour::fromRGB(198, 112, 0);

		// Utilitity
		Colour invisible = Colours::transparentWhite;
		Colour white = Colours::white;
		Colour black = Colours::black;
	} colors;
};
