#pragma once
namespace BearUI
{
	
	namespace Classic
	{


		class BEARUI_API BearUIButton :public BearUIItem
		{
			BEAR_CLASS_WITHOUT_COPY(BearUIButton);
		public:
			struct StyleDefault
			{
				StyleDefault() :ShowBackground(true)
				{
					Colors[A_Default][CT_Plane].Set(uint8(50), uint8(50), uint8(50));
					Colors[A_Default][CT_Background].Set(uint8(90), uint8(90), uint8(90));

					Colors[A_MouseEnter][CT_Plane].Set(uint8(50), uint8(50), uint8(50));
					Colors[A_MouseEnter][CT_Background].Set(uint8(0), uint8(100), uint8(150));

					Colors[A_MouseClick][CT_Plane].Set(uint8(0), uint8(100), uint8(150));
					Colors[A_MouseClick][CT_Background].Set(uint8(0), uint8(100), uint8(150));

					Colors[A_Disable][CT_Plane].Set(uint8(200), uint8(200), uint8(200));
					Colors[A_Disable][CT_Background].Set(uint8(200), uint8(200), uint8(200));
				}
				enum Action
				{
					A_Default = 0,
					A_MouseEnter,
					A_MouseClick,
					A_Disable,
					A_Count,
				};
				enum ColorType
				{
					CT_Plane,
					CT_Background,
					CT_Count,
				};
				bool ShowBackground;
				BearColor Colors[A_Count][CT_Count];
			};

			struct StyleWithoutBackground :public StyleDefault
			{
				StyleWithoutBackground()
				{
					ShowBackground = false;
					Colors[A_Default][CT_Plane].Set(uint8(70), uint8(70), uint8(70));
					Colors[A_MouseEnter][CT_Plane].Set(uint8(100), uint8(100), uint8(100));
					Colors[A_MouseClick][CT_Plane].Set(uint8(150), uint8(150), uint8(150));
					Colors[A_Disable][CT_Plane].Set(uint8(200), uint8(200), uint8(200));
				}
			};
			struct StyleWithoutBackgroundDark :public StyleDefault
			{
				StyleWithoutBackgroundDark()
				{
					ShowBackground = false;
					Colors[A_Default][CT_Plane].Set(uint8(35), uint8(35), uint8(35));
					Colors[A_MouseEnter][CT_Plane].Set(uint8(70), uint8(70), uint8(70));
					Colors[A_MouseClick][CT_Plane].Set(uint8(100), uint8(100), uint8(100));
					Colors[A_Disable][CT_Plane].Set(uint8(200), uint8(200), uint8(200));
				}
			};
		public:
			BearUIButton();
			~BearUIButton();
			BearFontRef Font;
			BearString Text;
			StyleDefault StyleConfig;

			template<class CL, typename F>
			inline void SetCallback(CL*cl, F f)
			{
				m_call_back =bear_create_class_function_ref(f);
				m_call_back_class = reinterpret_cast<void*>(cl);
			}
			enum EFlags
			{
				F_CallBackPress = 1,
			};

			inline bool IsPress()const
			{
				return m_press;
			}
			enum EStyle
			{
				S_WithoutBackground = 1,
				S_Triangle = 2
			};
			BearUITriangle::EStyle TriangleStyle;
			virtual float  CalcWidth() const;
			virtual float  CalcHeight() const;
		protected:

			virtual void Update(BearTime time);
			virtual void OnMessage(int32 message);
			virtual bool OnMouse(float x, float y);
			virtual bool OnKeyDown(BearInput::Key key);
			virtual bool OnKeyUp(BearInput::Key key);
			virtual void Reset();
			virtual void KillFocus();
			virtual void Reload();
		private:
			BearUITexture UITexture;
			BearUITexture UITextureBack;
			BearUIText UIText;
			BearUITriangle UITriangle;;
			BearClassFunctionRef*m_call_back;
			void*m_call_back_class;
			bool m_press;
		};
	};
}