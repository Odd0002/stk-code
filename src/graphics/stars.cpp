//  SuperTuxKart - a fun racing game with go-kart
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 3
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#include "graphics/stars.hpp"

#include "graphics/irr_driver.hpp"
#include "graphics/material.hpp"
#include "graphics/material_manager.hpp"

#include "irrlicht.h"
#include <cmath>

const int STAR_AMOUNT = 8;
const float RADIUS = 0.7f;
const float STAR_SIZE = 0.4f;

Stars::Stars(scene::IAnimatedMeshSceneNode* parentKart)
{
    m_parent_kart_node = parentKart;
    m_enabled = false;

    video::ITexture* texture = irr_driver->getTexture("starparticle.png");
    Material* star_material = material_manager->getMaterial("starparticle.png");
    
    m_center = core::vector3df(0.0f, 0.6f, 0.0f);
    
    for (int n=0; n<STAR_AMOUNT; n++)
    {
        scene::ISceneNode* billboard = irr_driver->addBillboard(core::dimension2d< f32 >(STAR_SIZE, STAR_SIZE),
                                                                texture,
                                                                parentKart);
        star_material->setMaterialProperties(&(billboard->getMaterial(0)));
        billboard->setMaterialTexture(0, star_material->getTexture());
        
        billboard->setVisible(false);
        
        m_nodes.push_back(billboard);
    }
    
    // for debugging
    //m_enabled = true;
    //m_remaining_time = 99999.0f;
    //update(0);
}

// ----------------------------------------------------------------------------

Stars::~Stars()
{
    const int nodeAmount = m_nodes.size();
    for (int n=0; n<nodeAmount; n++)
    {
        m_parent_kart_node->removeChild(m_nodes[n]);
    }
}

// ----------------------------------------------------------------------------

void Stars::showFor(float time)
{
    m_enabled = true;
    m_remaining_time = time;
    
    const int nodeAmount = m_nodes.size();
    for (int n=0; n<nodeAmount; n++)
    {
        m_nodes[n]->setVisible(true);
    }
    
    // set stars initial position
    update(0);
}

// ----------------------------------------------------------------------------

void Stars::update(float delta_t)
{
    if (!m_enabled) return;
    
    m_remaining_time -= delta_t;
    if (m_remaining_time < 0)
    {
        m_enabled = false;
        
        const int nodeAmount = m_nodes.size();
        for (int n=0; n<nodeAmount; n++)
        {
            m_nodes[n]->setVisible(false);
        }
        return;
    }
    
    const int nodeAmount = m_nodes.size();
    for (int n=0; n<nodeAmount; n++)
    {
        // do one full rotation every 4 seconds (this "ranges" ranges from 0 to 1)
        float angle = (m_remaining_time / 4.0f) - (int)(m_remaining_time / 4);
        
        // each star must be at a different angle
        angle += n * (1.0f / STAR_AMOUNT);
        
        // keep angle in range [0, 1[
        angle -= (int)angle;
        
        // set position
        m_nodes[n]->setPosition(m_center + core::vector3df( std::cos(angle*M_PI*2.0f)*RADIUS,
                                                            0.0f,
                                                            std::sin(angle*M_PI*2.0f)*RADIUS));
    }
}

