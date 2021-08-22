#include "GamePanel.h"

bool GamePanel::is_visible() const
{
    return visible;
}

void GamePanel::set_visible(bool flag)
{
    visible = flag;
}

void GamePanel::draw()
{
    if (visible)
    {
        on_draw();
    }
}

void GamePanel::on_draw()
{

}
