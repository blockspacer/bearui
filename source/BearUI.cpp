#include "BearUI.hpp"
#ifdef WINDOWS
BearCore::BearMap<int32, int32>* GWindowsKeys;
#endif


BearUI::BearUI::BearUI(bsize width, bsize height):m_focus_item(0)
{
	BearGraphics::BearShaderConstantsInitializer sconst_othro;
	sconst_othro.Constants.push_back(BearGraphics::CF_R32G32B32A32_FLOAT);
	m_screen_constant = BearGraphics::BearShaderConstantsRef(sconst_othro, true);

	m_screen.set(static_cast<float>(width), static_cast<float>(height), 1.f / static_cast<float>(width), 1.f / static_cast<float>(height));

	BearCore::bear_copy(m_screen_constant.Lock(), m_screen.array, sizeof(float) * 4);
	m_screen_constant.Unlock();

	BearGraphics::BearShaderConstantsInitializer sconst_color;
	sconst_color.Constants.push_back(BearGraphics::CF_R32G32B32A32_FLOAT);
	m_color_constant = BearGraphics::BearShaderConstantsRef(sconst_color, true);



	m_vertex_buffer.Create(sizeof(BearGraphics::BearVertexDefault) * 6);
	static int32 index_buffer[] = { 2,1,0,1,3,0 };
	m_index_buffer.Create(index_buffer, sizeof(index_buffer), false);

	m_size_screen.set(width, height);
}
BearUI::BearUI::~BearUI()
{
	m_color_constant.Clear(); 
	m_screen_constant.Clear();
	m_vertex_buffer.Clear();
	m_index_buffer.Clear();
#ifdef WINDOWS
	BearCore::bear_free(GWindowsKeys);
#endif
}

BearUI::BearFontRef BearUI::BearUI::GetFont(FontLang lang, bsize size)
{
	const bchar16*chars = L"";

	switch (lang)
	{
	case BearUI::BearUI::F_ENG:
		chars = L"0123456789qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM.,?/\\*-_+=(){}[]&^:;%$#�@\"'!`~><";
		break;
	case BearUI::BearUI::F_RUS:
		chars = L"0123456789qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM���������������������������������������������������������������.,?/\\*-_+=(){}[]&^:;%$#�@\"'!`~��><";
		break;
	default:
		break;
	}

	auto item = m_fonts.find(FontInfo(lang, size));
	if (item == m_fonts.end())
	{
		m_fonts.insert(FontInfo(lang, size));
		item = m_fonts.find(FontInfo(lang, size));
		item->second.LoadFromTTF(TEXT("C:/Windows/Fonts/arial.ttf"), chars, m_size_screen.x, m_size_screen.y, size);
	}
	return 	item->second;
}

BearUI::BearUIItem * BearUI::BearUI::PushItem(BearUIItem * item)
{
	item->Reset();
	m_items.push_back(item);
	m_static_items.push_back(item);
	return item;
}

BearUI::BearUIStaticItem * BearUI::BearUI::PushItem(BearUIStaticItem * item)
{
	item->Reset();
	m_static_items.push_back(item);
	return item;
}

void BearUI::BearUI::Resize(bsize width, bsize height)
{
	m_screen.set(static_cast<float>(width), static_cast<float>(height), 1.f / static_cast<float>(width), 1.f / static_cast<float>(height));

	BearCore::bear_copy(m_screen_constant.Lock(), m_screen.array, sizeof(float) * 4);
	m_screen_constant.Unlock();
	m_size_screen.set(width, height);
	Reset();
}


void BearUI::BearUI::Draw(float time)
{


	BearGraphics::BearRenderInterface::SetVertexState(BearGraphics::BearDefaultManager::GetVertexState(BearGraphics::DVS_UI));
	BearGraphics::BearRenderInterface::SetRasterizerState(BearGraphics::BearDefaultManager::GetRasterizerState());
	BearGraphics::BearRenderInterface::SetBlendState(BearGraphics::BearDefaultManager::GetBlendAlpha(), BearCore::BearColor::Transparent);
	/*{
		auto b = m_items.rbegin();
		auto e = m_items.rend();
		while (b != e)
		{
			(*b)->Draw(this, time);
			b++;
		}
	}*/
	{
		auto b = m_static_items.rbegin();
		auto e = m_static_items.rend();
		while (b != e)
		{
			(*b)->Draw(this, time);
			b++;
		}
	}
}

void BearUI::BearUI::Update(float time)
{
	auto b = m_items.rbegin();
	auto e = m_items.rend();
	while (b != e)
	{
		(*b)->Update();
		b++;
	}
	
}

void BearUI::BearUI::Unload()
{
	auto b = m_items.begin();
	auto e = m_items.end();
	while (b != e)
	{
		(*b)->Unload();
		b++;
	}
}

void BearUI::BearUI::Reload()
{
	auto b = m_items.begin();
	auto e = m_items.end();
	while (b != e)
	{
		(*b)->Reload();
		b++;
	}
}

void BearUI::BearUI::Reset()
{
	auto b = m_items.begin();
	auto e = m_items.end();
	while (b != e)
	{
		(*b)->Reset();
		b++;
	}
}

void BearUI::BearUI::KillFocus()
{
	m_focus_item = 0;
	auto b = m_items.begin();
	auto e = m_items.end();
	while (b != e)
	{
		(*b)->KillFocus();
		b++;
	}
}

void BearUI::BearUI::OnMouse(float x, float y)
{
	if (m_focus_item&&m_focus_item->OnMouse(x, y))
		return;
	auto b = m_items.begin();
	auto e = m_items.end();
	while (b != e)
	{
		if ((*b) != m_focus_item)
			if ((*b)->OnMouse(x, y))
				break;
		b++;
	}
}

void BearUI::BearUI::OnKeyDown(BearInput::Key key)
{
	if (m_focus_item&&m_focus_item->OnKeyDown(key))
		return;

	auto b = m_items.begin();
	auto e = m_items.end();
	while (b != e)
	{
		if (m_focus_item !=*b&&(*b)->OnKeyDown(key))
		{
			m_focus_item = (*b);
			UpdateFocus();
			break;
		}
		b++;
	}
}

void BearUI::BearUI::OnKeyUp(BearInput::Key key)
{
	if (m_focus_item&&m_focus_item->OnKeyUp(key))
		return;
	auto b = m_items.begin();
	auto e = m_items.end();
	while (b != e)
	{
		if (m_focus_item != *b &&(*b)->OnKeyUp(key))break;
		b++;
	}
}

void BearUI::BearUI::OnChar(bchar16 ch)
{
	if (m_focus_item)
	{
		m_focus_item->OnChar(ch);
		return;
	}

	/*auto b = m_items.begin();
	auto e = m_items.end();
	while (b != e)
	{
		if ((*b)->OnChar(ch))
		{
			return;
		}
		b++;
	}*/
}


void BearUI::BearUI::Render(BearUITexture * texture)
{
	BearGraphics::BearRenderInterface::SetVertexShader(BearGraphics::BearDefaultManager::GetVertexShader(BearGraphics::DVS_UI));
    BearGraphics::BearRenderInterface::SetPixelShader(BearGraphics::BearDefaultManager::GetPixelShader(BearGraphics::DPS_UITexture));


	BearGraphics::BearRenderInterface::SetVertexShaderConstants(0, m_screen_constant);

	if (texture->Flags.test(BearUIStaticItem::UI_NoClip))
	{
		BearGraphics::BearRenderInterface::DisableScissor( );
	}
	else
	{
		BearGraphics::BearRenderInterface::SetScissor( texture->Clip.x, texture->Clip.y, texture->Clip.x+ texture->Clip.x1, texture->Clip.y+ texture->Clip.y1);
	}

	BearCore::bear_copy(reinterpret_cast<float*>(m_color_constant.Lock()), texture->Color.GetFloat().array, 4);
	m_color_constant.Unlock();
	BearGraphics::BearRenderInterface::SetPixelShaderConstants(0, m_color_constant);

	BearGraphics::BearRenderInterface::SetPixelShaderResource(0, texture->Texture, BearGraphics::BearDefaultManager::GetSamplerState());
	BearCore::bear_copy(reinterpret_cast<BearGraphics::BearVertexDefault*>(m_vertex_buffer.Lock()), texture->m_vertex, 4);
	m_vertex_buffer.Unlock();
	BearGraphics::BearRenderInterface::SetVertexBuffer(m_vertex_buffer);
	BearGraphics::BearRenderInterface::SetIndexBuffer(m_index_buffer);
	BearGraphics::BearRenderInterface::DrawIndexed(6);
	BearGraphics::BearRenderInterface::DisableScissor();
}

void BearUI::BearUI::Render(BearUIText * text)
{
	BearGraphics::BearVertexDefault vertex[4];

	bsize size = text->Text.size();
	if (text->Font.Empty() || !size)return;

	BearGraphics::BearRenderInterface::SetPixelShader(BearGraphics::BearDefaultManager::GetPixelShader(BearGraphics::DPS_UIText));
	BearGraphics::BearRenderInterface::SetVertexShader(BearGraphics::BearDefaultManager::GetVertexShader(BearGraphics::DVS_UI));

	BearGraphics::BearRenderInterface::SetVertexShaderConstants(0, m_screen_constant);

	if (text->Flags.test(BearUIStaticItem::UI_NoClip))
	{
		BearGraphics::BearRenderInterface::DisableScissor();
	}
	else
	{
		BearGraphics::BearRenderInterface::SetScissor(text->Clip.x, text->Clip.y, text->Clip.x + text->Clip.x1, text->Clip.y + text->Clip.y1);
	}

	BearCore::bear_copy(reinterpret_cast<float*>(m_color_constant.Lock()), text->Color.GetFloat().array, 4);
	m_color_constant.Unlock();

	BearGraphics::BearRenderInterface::SetPixelShaderConstants(0, m_color_constant);
	BearGraphics::BearRenderInterface::SetPixelShaderResource(0, *text->Font.GetTexture(), BearGraphics::BearDefaultManager::GetSamplerState());
	BearGraphics::BearRenderInterface::SetVertexBuffer(m_vertex_buffer);
	BearGraphics::BearRenderInterface::SetIndexBuffer(m_index_buffer);

	auto&font = *text->Font.GetListChars();
	auto pos = text->Position+text->ShiftPosition;

	float up = 0;
	if (text->Style.is(BearUIText::ST_CenterOfHeight))
	{
		up = text->GetMaxHeightCharInLine(*text->Text);
		up = static_cast<float>(text->Font.GetHieght()) - up;
		up /= 2.f;
		up = floorf(up);
		up = -up;
	}
	BearCore::BearVector4<float> TextureUV;

	for (bsize i = 0; i < size; i++)
	{
		bchar16 ch_ = BearCore::BearEncoding::ToUTF16(text->Text[i]);
		if (ch_ == TEXT(' '))
		{
			auto item = font.find(ch_);
			pos.x += item->second.Advance.x;
		}
		else if (ch_ == TEXT('\t'))
		{
			ch_ = TEXT(' ');
			auto item = font.find(ch_);
			pos.x += item->second.Advance.x * 4;
		}
		else if (ch_ == TEXT('\n'))
		{
			pos.y += text->Font.GetHieght();
			pos.x = text->Position.x+ text->ShiftPosition.x;
			if (text->Style.is(BearUIText::ST_CenterOfHeight))
			{
				up = text->GetMaxHeightCharInLine(*text->Text+i+1);
				up = static_cast<float>(text->Font.GetHieght()) - up;
				up /= 2.f;
				up = floorf(up);
				up = -up;
			}
		}
		else if (ch_ != TEXT('\r'))
		{
			auto item = font.find(ch_);
			if (item != font.end())
			{

				TextureUV = item->second.TextureUV;
				if (pos.x + item->second.Advance.x > text->Rect.x1 + text->Position.x&&text->Style.test(text->ST_MaxWidth))
				{
					pos.y += text->Font.GetHieght();
					pos.x = text->Position.x;
					if (text->Style.is(BearUIText::ST_CenterOfHeight))
					{
						up = text->GetMaxHeightCharInLine(*text->Text + i + 1);
						up = static_cast<float>(text->Font.GetHieght()) - up;
						up /= 2.f;
						up = floorf(up);
						up = -up;
					}
				}

				{
					float x = pos.x + item->second.Position.x, y = pos.y + item->second.Position.y+ up;
					float width = item->second.Size.x;
					float height = item->second.Size.y;

					/*x = floor(x);
					y = floor(y);
					width = floor(width);
					height = floor(height);*/

					vertex[0].Position.set(x, y + height);
					vertex[1].Position.set(x + width, y);
					vertex[2].Position.set(x, y);
					vertex[3].Position.set(x + width, y + height);
					vertex[0].TextureUV.set(TextureUV.x, TextureUV.y + TextureUV.y1);
					vertex[1].TextureUV.set(TextureUV.x1 + TextureUV.x, TextureUV.y);
					vertex[2].TextureUV.set(TextureUV.x, TextureUV.y);
					vertex[3].TextureUV.set(TextureUV.x1 + TextureUV.x, TextureUV.y1 + TextureUV.y);

					BearCore::bear_copy(reinterpret_cast<BearGraphics::BearVertexDefault*>(m_vertex_buffer.Lock()), vertex, 4);
					m_vertex_buffer.Unlock();
				
					BearGraphics::BearRenderInterface::DrawIndexed(6);

				}
				pos.x += item->second.Advance.x;
			}
		}

	}
	BearGraphics::BearRenderInterface::DisableScissor();
}

void BearUI::BearUI::Render(BearUITriangle * triangle)
{
	BearGraphics::BearVertexDefault vertex[3];
	BearGraphics::BearRenderInterface::SetVertexShader(BearGraphics::BearDefaultManager::GetVertexShader(BearGraphics::DVS_UI));
	BearGraphics::BearRenderInterface::SetPixelShader(BearGraphics::BearDefaultManager::GetPixelShader(BearGraphics::DPS_UITexture));


	BearGraphics::BearRenderInterface::SetVertexShaderConstants(0, m_screen_constant);

	if (triangle->Flags.test(BearUIStaticItem::UI_NoClip))
	{
		BearGraphics::BearRenderInterface::DisableScissor();
	}
	else
	{
		BearGraphics::BearRenderInterface::SetScissor(triangle->Clip.x, triangle->Clip.y, triangle->Clip.x + triangle->Clip.x1, triangle->Clip.y + triangle->Clip.y1);
	}

	BearCore::bear_copy(reinterpret_cast<float*>(m_color_constant.Lock()), triangle->Color.GetFloat().array, 4);
	m_color_constant.Unlock();
	BearGraphics::BearRenderInterface::SetPixelShaderConstants(0, m_color_constant);

	BearGraphics::BearRenderInterface::SetPixelShaderResource(0, BearGraphics::BearDefaultManager::GetTexture2D(), BearGraphics::BearDefaultManager::GetSamplerState());

	auto rect = triangle->Rect;
	triangle->Size.set(rect.x1*triangle->Scale, rect.y1*triangle->Scale);

	triangle->Position += BearCore::BearFVector2((rect.x1- triangle->Size.x)/2.f, (rect.y1 - triangle->Size.y) / 2.f);
	if (triangle->Style.is(BearUITriangle::S_TriangleRight))
	{
		vertex[0].Position.set(triangle->Position.x, triangle->Position.y);
		vertex[1].Position.set(triangle->Position.x, triangle->Position.y+triangle->Size.y);
		vertex[2].Position.set(triangle->Position.x + triangle->Size.x, triangle->Position.y + triangle->Size.y/2.f);
	}
	else if (triangle->Style.is(BearUITriangle::S_TriangleLeft))
	{
		vertex[0].Position.set(triangle->Position.x, triangle->Position.y + triangle->Size.y / 2.f);
		vertex[1].Position.set(triangle->Position.x + triangle->Size.x, triangle->Position.y + triangle->Size.y);
		vertex[2].Position.set(triangle->Position.x + triangle->Size.x, triangle->Position.y );
	}
	else if (triangle->Style.is(BearUITriangle::S_TriangleDown))
	{
		vertex[0].Position.set(triangle->Position.x, triangle->Position.y);
		vertex[1].Position.set(triangle->Position.x + triangle->Size.x / 2.f, triangle->Position.y + triangle->Size.y);
		vertex[2].Position.set(triangle->Position.x + triangle->Size.x, triangle->Position.y);
	}
	else
	{

		vertex[0].Position.set(triangle->Position.x + triangle->Size.x / 2.f, triangle->Position.y);
		vertex[1].Position.set(triangle->Position.x , triangle->Position.y + triangle->Size.y);
		vertex[2].Position.set(triangle->Position.x + triangle->Size.x, triangle->Position.y + triangle->Size.y);
	}
	triangle->Rect = rect;
	BearCore::bear_copy(reinterpret_cast<BearGraphics::BearVertexDefault*>(m_vertex_buffer.Lock()), vertex, 3);
	m_vertex_buffer.Unlock();
	BearGraphics::BearRenderInterface::SetVertexBuffer(m_vertex_buffer);
	BearGraphics::BearRenderInterface::SetIndexBuffer(m_index_buffer);
	BearGraphics::BearRenderInterface::DrawIndexed(3);
	BearGraphics::BearRenderInterface::DisableScissor();
}

void BearUI::BearUI::UpdateFocus()
{
	auto b = m_items.begin();
	auto e = m_items.end();
	while (b != e)
	{
		if (*b != m_focus_item)(*b)->KillFocus();
		b++;
	}
}

