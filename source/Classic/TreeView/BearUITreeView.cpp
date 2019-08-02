#include "BearUI.hpp"

BearUI::Classic::BearUITreeView::BearUITreeView():ScrollBarHeight(0)
{
	Select = 0;
	ColorPlaneBackground.Set(uint8(100), uint8(100), uint8(100));
	ColorPlane.Set(uint8(20), uint8(20), uint8(20));
	ColorSelect.Set(uint8(67), uint8(67), uint8(67));
	ColorSelectFocus.Set(uint8(0), uint8(120), uint8(200));
	CallBack = 0;
	UIScrollBar.Visible = true;
	bUpdateScrollBar =true;
}

BearUI::Classic::BearUITreeView::~BearUITreeView()
{
	if(CallBack)
	CallBack->Destroy();
}

void BearUI::Classic::BearUITreeView::Reset()
{
	PopItems();
	Clip = Rect;
	Clip += BearCore::BearVector4<float>(1, 1, -2, -2);
	UIPlane.Rect = Rect;
	UIPlane.Rect +=  BearCore::BearVector4<float>(1, 1, -2, -2);
	UIPlane.Color = ColorPlane;
	UIPlaneBackground.Color = ColorPlaneBackground;
	UIPlaneBackground.Rect = Rect;

	UIScrollBar.Position = Position;
	UIScrollBar.Position.y += 1;
	UIScrollBar.Size.y = Size.y-2;
	UIScrollBar.Size.x = static_cast<float>(Font.GetHieght());
	UIScrollBar.Position.x += Size.x - 2 - static_cast<float>(Font.GetHieght());

	PushItem(&UIScrollBar);
	for (auto b = Nodes.begin(), e = Nodes.end(); b != e; b++)
	{
		(*b)->Clip = Clip;
		(*b)->Font = Font;
		(*b)->Parent = this;
		(*b)->Reset();
		PushItem(**b);
		(*b)->PushNodesToParent();
	
	}
	{
		float Height = 0;
		for (auto b = Nodes.begin(), e = Nodes.end(); b != e; b++)
		{
			(*b)->Update(BearCore::BearTime());
			Height += (*b)->GetHeight();
		}
		UIScrollBar.ScrollZoneView = Rect.y1 / (Height);
	}
	PushItem(&UIPlane);
	PushItem(&UIPlaneBackground);
	bUpdateScrollBar = true;
	BearUIItem::Reset();
}

BearUI::Classic::BearUITreeNode * BearUI::Classic::BearUITreeView::Find(const bchar * text)
{
	auto item = BearCore::bear_find_if(Nodes.begin(), Nodes.end(), [text](const BearCore::BearMemoryRef< BearUITreeNode> &a) {return a->Text == text; });
	if (item == Nodes.end())
		return NULL;
	return **item;
}

const BearUI::Classic::BearUITreeNode * BearUI::Classic::BearUITreeView::Find(const bchar * text) const
{
	auto item = BearCore::bear_find_if(Nodes.begin(), Nodes.end(), [text](const BearCore::BearMemoryRef< BearUITreeNode> &a) {return a->Text == text; });
	if (item == Nodes.end())
		return NULL;
	return **item;
}

BearUI::Classic::BearUITreeNode & BearUI::Classic::BearUITreeView::Add(const bchar * text)
{
	auto *Node = BearCore::bear_new< BearUITreeNode>();
	Node->Text = text;
	Nodes.push_back(Node);
	//Reset();
	return *Node;
}

void BearUI::Classic::BearUITreeView::Update(BearCore::BearTime time)
{
	if (bUpdateScrollBar)UpdateScrollBar();
	bool bViewedAll = ViewedAll();

	float Height = 0;

	auto size = Size;
	auto clip = Clip;
	float delata_y = 0;
	if (!bViewedAll)
	{
		size.x -= static_cast<float>(Font.GetHieght());
		clip.x1-= static_cast<float>(Font.GetHieght());
		delata_y = UIScrollBar.ScrollPosition*ScrollBarHeight;
		UIScrollBar.Visible = false;
		Height -= delata_y;
	}
	else
	{
		UIScrollBar.Visible = true;
	}


	for (auto b = Nodes.begin(), e = Nodes.end(); b != e; b++)
	{
		(*b)->Position = Position;
		(*b)->Position.y += Height;
		(*b)->Size = size;
		(*b)->Clip = clip;
		(*b)->Update(time);
		Height += (*b)->GetHeight();
	}
	BearUIItem::Update(time);
}

void BearUI::Classic::BearUITreeView::OnMessage(int32 message)
{
	switch (message)
	{
	case M_ClickSelect:
		if (CallBack)CallBack->Call<void>(CallBack_Class, Select);
		break;
	case M_UpdateScrollBar:
		bUpdateScrollBar = true;
		break;
	default:
		break;
	}
	BearUIItem::OnMessage(message);
}

void BearUI::Classic::BearUITreeView::UpdateScrollBar()
{
	float old_delata_y = UIScrollBar.ScrollPosition*ScrollBarHeight;
	bUpdateScrollBar = false;
	float Height = 0;
	for (auto b = Nodes.begin(), e = Nodes.end(); b != e; b++)
	{
		(*b)->Update(BearCore::BearTime());
		Height += (*b)->GetHeight();
	}
	UIScrollBar.ScrollOne = static_cast<float>(Font.GetHieght()) / (Height);
	UIScrollBar.SetZoneView ( Rect.y1 / (Height));

	ScrollBarHeight =BearCore::bear_max( Height - (Rect.y1 - 2),0.f);


	//UIScrollBar.Reset();
	if (old_delata_y > ScrollBarHeight)old_delata_y = ScrollBarHeight;
	if (ScrollBarHeight == 0)
		UIScrollBar.SetPosition(0);
	else
		UIScrollBar.SetPosition(old_delata_y / ScrollBarHeight);
}

bool BearUI::Classic::BearUITreeView::ViewedAll()
{

	float Height = 0;
	for (auto b = Nodes.begin(), e = Nodes.end(); b != e; b++)
	{
		(*b)->Position = Position;
		(*b)->Position.y += Height;
		(*b)->Size = Size;
		(*b)->Clip = Clip;
		(*b)->Update(BearCore::BearTime());
		Height += (*b)->GetHeight();
	}

	return Height <= Rect.y1 - 2;
}
