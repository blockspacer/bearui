#include "BearUI.hpp"

BearUI::Classic::BearUICursorText::BearUICursorText(bsize sizeText):SizeText(sizeText)
{
	

}

BearUI::Classic::BearUICursorText::~BearUICursorText()
{
}

void BearUI::Classic::BearUICursorText::Draw(BearUI * ui, BearCore::BearTime time)
{
	float SizeChunk =static_cast<float>( SizeText / 6);
	float Width = static_cast<float>(SizeText / 16);;
	float Height = static_cast<float>(SizeText);
	if (Width == 0)
	{
		Width = 1;
	}
	if (SizeText %2==0)
	{
		Height += 1;
	}
	Position+= BearCore::BearVector2<float>(-SizeChunk, -(Height/2));
	UITextureUp.Position = Position + BearCore::BearVector2<float>(-SizeChunk, 0);
	UITextureUp.Size.y = Width;
	UITextureUp.Size.x = SizeChunk + SizeChunk + 1;
	UITextureUp.Draw(ui, time);
	UITextureMidle.Position = Position + BearCore::BearVector2<float>(0, Width);
	UITextureMidle.Size.x = Width;
	UITextureMidle.Size.y = Height - Width * 2;
	UITextureMidle.Draw(ui, time);
	UITextureDown.Position = Position + BearCore::BearVector2<float>(-SizeChunk, Height - Width);
	UITextureDown.Size.x = SizeChunk + SizeChunk + 1;
	UITextureDown.Size.y = Width;
	UITextureDown.Draw(ui, time);
}

void BearUI::Classic::BearUICursorText::Reset()
{
}

void BearUI::Classic::BearUICursorText::Unload()
{
	UITextureUp.Unload();
	UITextureMidle.Unload();
	UITextureDown.Unload();
}

void BearUI::Classic::BearUICursorText::Reload()
{
	UITextureUp.Reload();
	UITextureMidle.Reload();
	UITextureDown.Reload();
}
