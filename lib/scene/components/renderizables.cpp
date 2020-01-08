#include "renderizables.hpp"

#include <lib/include/liblog.hpp>
#include <lib/scene/scenenode.hpp>
#include <lib/scene/scenenode.hpp>

namespace lib::scene
{
	Renderizables::Renderizables() {}

    Renderizables::~Renderizables() = default;

    void Renderizables::postUpdate()
    {
        for (auto&& renderizable : render_nodes_)
        {
            renderizable->render();
        }
    }

    void Renderizables::onAttached()
    {
    }

    void Renderizables::removeRenderizable(const sptr<Renderizable>& element)
    {
        log_assert(element.get() != nullptr, "Received empty renderizable node to be deleted");

        render_nodes_.erase_values(element);
    }

    void Renderizables::clearRenderizables()
    {
        render_nodes_.clear();
    }

    void Renderizables::for_each_node(function<void(const sptr<Renderizable>&)> action) const
    {
        std::for_each(render_nodes_.cbegin(), render_nodes_.cend(), action);
    }

    void Renderizables::addRenderizable(sptr<Renderizable> newElement)
    {
        render_nodes_.push_back(std::move(newElement));
    }
}
