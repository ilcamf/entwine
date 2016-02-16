/******************************************************************************
* Copyright (c) 2016, Connor Manning (connor@hobu.co)
*
* Entwine -- Point cloud indexing
*
* Entwine is available under the terms of the LGPL2 license. See COPYING
* for specific license text and more information.
*
******************************************************************************/

#include <entwine/tree/clipper.hpp>

namespace entwine
{

bool Clipper::insert(const Id& chunkId, std::size_t chunkNum)
{
    const auto find(m_clips.find(chunkId));

    if (find != m_clips.end())
    {
        m_order.splice(m_order.begin(), m_order, find->second.it);
        return false;
    }
    else
    {
        const auto it(
                m_clips.insert(
                    std::make_pair(chunkId, ClipInfo(chunkNum))).first);

        m_order.push_front(&it->first);
        it->second.it = m_order.begin();
        return true;
    }
}

void Clipper::clip()
{
    const std::size_t count(m_clips.size() / 2);

    for (std::size_t i(0); i < count; ++i)
    {
        const Id& id(*m_order.back());
        const auto it(m_clips.find(id));

        m_builder.clip(id, it->second.chunkNum, m_id);

        m_clips.erase(it);
        m_order.pop_back();
    }
}

} // namespace entwine
