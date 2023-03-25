#pragma once
#include "BCatsData.h"

namespace BCats {
	namespace ViewData {
		extern void render_cats(Data::CatRarity rarity);
		extern void on_gui_render(bool viewModifiedData);
	}
}