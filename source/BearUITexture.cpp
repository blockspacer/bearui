#include "BearUI.hpp"

BearUITexture::BearUITexture():Color(BearColor::White), TextureUV(0,0,1,1), Alpha(false)
{
	
}

BearUITexture::~BearUITexture()
{
}

float BearUITexture::CalcWidth() const
{
	return 0.0f;
}

float BearUITexture::CalcHeight() const
{
	return 0.0f;
}

void BearUITexture::Draw(BearUI * ui,BearTime time)
{
	if (Visible)return;
	m_vertex[0].position.set(Position.x, Position.y + Size.y);
	m_vertex[1].position.set(Position.x + Size.x, Position.y);
	m_vertex[2].position.set(Position.x, Position.y);
	m_vertex[3].position.set(Position.x + Size.x, Position.y + Size.y);
	m_vertex[0].uv.set(TextureUV.x, TextureUV.y1);
	m_vertex[1].uv.set(TextureUV.x1 + TextureUV.x, TextureUV.y + TextureUV.y);
	m_vertex[2].uv.set(TextureUV.x, TextureUV.y);
	m_vertex[3].uv.set(TextureUV.x1 + TextureUV.x, TextureUV.y1 + TextureUV.y);

	bear_copy(m_vertex[0].color.Array, Color.R32G32B32A32, 4);
	bear_copy(m_vertex[1].color.Array, Color.R32G32B32A32, 4);
	bear_copy(m_vertex[2].color.Array, Color.R32G32B32A32, 4);
	bear_copy(m_vertex[3].color.Array, Color.R32G32B32A32, 4);
	ui->Render(this);
}

void BearUITexture::Reset()
{
	
}

void BearUITexture::Unload()
{
	Texture.clear();
}

void BearUITexture::Reload(BearUIResourcesManager* Manager)
{
	if (Texture.empty())
	{
		Texture = Manager->GetTexture(TEXT(""));
	}
}

