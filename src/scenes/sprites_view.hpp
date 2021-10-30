#pragma once

#include <systems/scenes/scene.hpp>
#include <systems/assets/sprite.hpp>

#include <imgui.h>
//

class SpritesView : public Scene
{
public:
    SpritesView(Context& context);

    void onFrame() override;

    bool active() const override { return true; }
    void active(bool active) override {}

private:
    void refresh();

private:
    std::vector<std::shared_ptr<Sprite>> _sprites;
};